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
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qdir.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qstandardpaths.h>

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

QString QAndroidInAppPurchaseBackend::finalizedUnlockableFileName() const
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    return path + QStringLiteral("/.qt-mobile-extras-data/iap_finalization.data");
}

void QAndroidInAppPurchaseBackend::initialize()
{
    m_javaObject.callMethod<void>("initializeConnection");

    QFile file(finalizedUnlockableFileName());
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        while (!stream.atEnd()) {
            QString identifier;
            stream >> identifier;
            m_finalizedUnlockableProducts.insert(identifier);
        }

    } else if (file.exists()) {
        qWarning("Failed to read from finalization data.");
    }
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
    QMutexLocker locker(&m_mutex);
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

void QAndroidInAppPurchaseBackend::consumeTransaction(const QString &purchaseToken)
{
    QMutexLocker locker(&m_mutex);
    m_javaObject.callMethod<void>("consumePurchase",
                                  "(Ljava/lang/String;)V",
                                  QAndroidJniObject::fromString(purchaseToken).object<jstring>());
}

void QAndroidInAppPurchaseBackend::registerFinalizedUnlockable(const QString &identifier)
{
    QMutexLocker locker(&m_mutex);
    m_finalizedUnlockableProducts.insert(identifier);

    QString fileName = finalizedUnlockableFileName();
    QDir().mkpath(QFileInfo(fileName).absolutePath());

    QFile file(fileName);
    if (!file.open(QIODevice::Append)) {
        qWarning("Failed to open file to store finalization info.");
        return;
    }

    QDataStream stream(&file);
    stream << identifier;
}

bool QAndroidInAppPurchaseBackend::transactionFinalizedForProduct(QInAppProduct *product)
{
    Q_ASSERT(m_infoForPurchase.contains(product->identifier()));
    return product->productType() == QInAppProduct::Consumable
            || m_finalizedUnlockableProducts.contains(product->identifier());
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
    QHash<QString, PurchaseInfo>::iterator it = m_infoForPurchase.find(product->identifier());
    if (it == m_infoForPurchase.end())
        return;

    const PurchaseInfo &info = it.value();
    if (!transactionFinalizedForProduct(product)) {
        QAndroidInAppTransaction *transaction = new QAndroidInAppTransaction(info.signature,
                                                                             info.data,
                                                                             info.purchaseToken,
                                                                             info.orderId,
                                                                             QInAppTransaction::PurchaseApproved,
                                                                             product,
                                                                             this);
        emit transactionReady(transaction);
    }

    m_infoForPurchase.erase(it);
}

void QAndroidInAppPurchaseBackend::registerProduct(const QString &productId, const QString &price)
{
    QMutexLocker locker(&m_mutex);
    QHash<QString, QInAppProduct::ProductType>::iterator it = m_productTypeForPendingId.find(productId);
    Q_ASSERT(it != m_productTypeForPendingId.end());

    QAndroidInAppProduct *product = new QAndroidInAppProduct(this, price, it.value(), it.key(), this);
    checkFinalizationStatus(product);

    emit productQueryDone(product);
    m_productTypeForPendingId.erase(it);
}

void QAndroidInAppPurchaseBackend::registerPurchased(const QString &identifier,
                                                     const QString &signature, const QString &data,
                                                     const QString &purchaseToken, const QString &orderId)
{
    QMutexLocker locker(&m_mutex);
    m_infoForPurchase.insert(identifier, PurchaseInfo(signature, data, purchaseToken, orderId));
}

void QAndroidInAppPurchaseBackend::registerReady()
{
    QMutexLocker locker(&m_mutex);
    m_isReady = true;
    emit ready();
}

void QAndroidInAppPurchaseBackend::handleActivityResult(int requestCode, int resultCode, const QAndroidJniObject &data)
{
    QInAppProduct *product = m_activePurchaseRequests.value(requestCode);
    if (product == 0) {
        qWarning("No product registered for requestCode %d", requestCode);
        return;
    }

    m_javaObject.callMethod<void>("handleActivityResult", "(IILandroid/content/Intent;Ljava/lang/String;)V",
                                  requestCode,
                                  resultCode,
                                  data.object<jobject>(),
                                  QAndroidJniObject::fromString(product->identifier()).object<jstring>());
}

void QAndroidInAppPurchaseBackend::purchaseProduct(QAndroidInAppProduct *product)
{
    QMutexLocker locker(&m_mutex);
    if (m_pendingPurchaseForIdentifier.contains(product->identifier())) {
        qWarning("Product %s is already being purchased. Finalize transaction and try again.",
                 qPrintable(product->identifier()));
        return;
    }

    m_pendingPurchaseForIdentifier[product->identifier()] = product;
    if (!m_javaObject.isValid()) {
        qWarning("Cannot request purchase, because Java backend is not initialized.");
        purchaseFailed(product);
        return;
    }

    QAndroidJniObject intentSender = m_javaObject.callObjectMethod("createBuyIntentSender",
                                                                   "(Ljava/lang/String;)Landroid/content/IntentSender;",
                                                                   QAndroidJniObject::fromString(product->identifier()).object<jstring>());
    if (!intentSender.isValid()) {
        qWarning("Unable to get intent sender from service");
        purchaseFailed(product);
        return;
    }

    int requestCode = 0;
    while (m_activePurchaseRequests.contains(requestCode)) {
        requestCode++;
        if (requestCode == 0) {
            qWarning("No available request code for purchase request.");
            return;
        }
    }

    m_activePurchaseRequests[requestCode] = product;
    QtAndroid::startIntentSender(intentSender, requestCode, this);
}

void QAndroidInAppPurchaseBackend::purchaseFailed(int requestCode)
{
    QMutexLocker locker(&m_mutex);
    QInAppProduct *product = m_activePurchaseRequests.take(requestCode);
    if (product == 0) {
        qWarning("No product registered for requestCode %d", requestCode);
        return;
    }

    purchaseFailed(product);
}

void QAndroidInAppPurchaseBackend::purchaseFailed(QInAppProduct *product)
{
    QInAppTransaction *transaction = new QAndroidInAppTransaction(QString(),
                                                                  QString(),
                                                                  QString(),
                                                                  QString(),
                                                                  QInAppTransaction::PurchaseFailed,
                                                                  product,
                                                                  this);
    emit transactionReady(transaction);
}

void QAndroidInAppPurchaseBackend::purchaseSucceeded(int requestCode,
                                                     const QString &signature,
                                                     const QString &data,
                                                     const QString &purchaseToken,
                                                     const QString &orderId)

{
    QMutexLocker locker(&m_mutex);
    QInAppProduct *product = m_activePurchaseRequests.take(requestCode);
    if (product == 0) {
        qWarning("No product registered for requestCode %d", requestCode);
        return;
    }

    QInAppTransaction *transaction = new QAndroidInAppTransaction(signature,
                                                                  data,
                                                                  purchaseToken,
                                                                  orderId,
                                                                  QInAppTransaction::PurchaseApproved,
                                                                  product,
                                                                  this);
    emit transactionReady(transaction);
}

QT_END_NAMESPACE
