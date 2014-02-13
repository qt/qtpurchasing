/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the Qt Mobile Extras Add-on.
**
** $QT_BEGIN_LICENSE$
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qandroidinapppurchasebackend_p.h"
#include "qandroidinappproduct_p.h"
#include "qandroidinapptransaction_p.h"

#include <QtAndroidExtras/qandroidfunctions.h>

QT_BEGIN_NAMESPACE

QAndroidInAppPurchaseBackend::QAndroidInAppPurchaseBackend(QObject *parent)
    : QInAppPurchaseBackend(parent)
    , m_mutex(QMutex::Recursive)
    , m_isReady(false)
{
    m_javaObject = QAndroidJniObject("com/digia/qt5/android/mobileextras/QtInAppPurchase",
                                       "(Landroid/content/Context;J)V",
                                       QtAndroid::androidActivity().object<jobject>(),
                                       reinterpret_cast<jlong>(this));
    if (!m_javaObject.isValid()) {
        qWarning("Cannot initialize IAP backend for Android due to missing dependency: QtInAppPurchase class");
        return;
    }
}

void QAndroidInAppPurchaseBackend::initialize()
{
    m_javaObject.callMethod<void>("initializeConnection");
#warning Read tokens here
}

bool QAndroidInAppPurchaseBackend::isReady() const
{
    QMutexLocker locker(&m_mutex);
    return m_isReady;
}

void QAndroidInAppPurchaseBackend::restorePurchases()
{
    // ### Go through existing purchases, remove finalization token and emit transactions
#warning Unimplemented
}

void QAndroidInAppPurchaseBackend::queryProduct(QInAppProduct::ProductType productType,
                                                const QString &identifier)
{
    QMutexLocker locker(&m_mutex);
    if (m_productTypeForPendingId.contains(identifier)) {
        qWarning("Product query already pending for %s", qPrintable(identifier));
        return;
    }

    m_productTypeForPendingId[identifier] = productType;
    m_javaObject.callMethod<void>("queryDetails",
                                    "(Ljava/lang/String;)V",
                                    QAndroidJniObject::fromString(identifier).object<jstring>());
}

void QAndroidInAppPurchaseBackend::setPlatformProperty(const QString &propertyName, const QString &value)
{
    if (propertyName.compare(QStringLiteral("AndroidPublicKey"), Qt::CaseInsensitive) == 0) {
        m_javaObject.callMethod<void>("setPublicKey",
                                        "(Ljava/lang/String;)",
                                        QAndroidJniObject::fromString(value).object<jstring>());
    }
}

void QAndroidInAppPurchaseBackend::registerQueryFailure(const QString &productId)
{
    QMutexLocker locker(&m_mutex);
    QHash<QString, QInAppProduct::ProductType>::iterator it = m_productTypeForPendingId.find(productId);
    Q_ASSERT(it != m_productTypeForPendingId.end());

    emit productQueryFailed(it.value(), it.key());
    m_productTypeForPendingId.erase(it);
}

bool QAndroidInAppPurchaseBackend::transactionFinalizedForProduct(QInAppProduct *product)
{
    Q_ASSERT(m_signatureAndDataForPurchase.contains(product->identifier()));
    return product->productType() == QInAppProduct::Consumable
            || m_finalizedUnlockableProducts.contains(product->identifier());
}

void QAndroidInAppPurchaseBackend::createTransactionForProduct(QInAppTransaction::TransactionStatus status,
                                                               QInAppProduct *product,
                                                               const QPair<QString, QString> &signatureAndData)
{
    QAndroidInAppTransaction *transaction = new QAndroidInAppTransaction(signatureAndData.first,
                                                                         signatureAndData.second,
                                                                         status,
                                                                         product,
                                                                         this);
    emit transactionReady(transaction);
}

void QAndroidInAppPurchaseBackend::checkFinalizationStatus(QInAppProduct *product)
{
    // Verifies the finalization status of an item based on the following logic:
    // 1. If the item is not purchased yet, do nothing (it's either never been purchased, or it's a
    //    consumed consumable.
    // 2. If the item is purchased, and it's a consumable, it's unfinalized. Emit a new transaction.
    //    Consumable items are consumed when they are finalized.
    // 3. If the item is purchased, and it's an unlockable, check the local cache for finalized
    //    unlockable purchases. If it's not there, then the transaction is unfinalized. This means
    //    that if the cache gets deleted or corrupted, the worst-case scenario is that the transactions
    //    are republished.
    QHash<QString, QPair<QString, QString> >::iterator it = m_signatureAndDataForPurchase.find(product->identifier());
    if (it == m_signatureAndDataForPurchase.end())
        return;

    if (!transactionFinalizedForProduct(product))
        createTransactionForProduct(QInAppTransaction::PurchaseApproved, product, it.value());

    m_signatureAndDataForPurchase.erase(it);
}

void QAndroidInAppPurchaseBackend::registerProduct(const QString &productId, const QString &price)
{
    QMutexLocker locker(&m_mutex);
    QHash<QString, QInAppProduct::ProductType>::iterator it = m_productTypeForPendingId.find(productId);
    Q_ASSERT(it != m_productTypeForPendingId.end());

    QAndroidInAppProduct *product = new QAndroidInAppProduct(price, it.value(), it.key(), this);
    checkFinalizationStatus(product);

    emit productQueryDone(product);
    m_productTypeForPendingId.erase(it);
}

void QAndroidInAppPurchaseBackend::registerPurchased(const QString &productId, const QString &signature, const QString &data)
{
    QMutexLocker locker(&m_mutex);
    m_signatureAndDataForPurchase.insert(productId, qMakePair(signature, data));
}


void QAndroidInAppPurchaseBackend::registerReady()
{
    QMutexLocker locker(&m_mutex);
    m_isReady = true;
    emit ready();
}

QT_END_NAMESPACE
