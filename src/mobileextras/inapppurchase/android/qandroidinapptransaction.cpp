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

#include "qandroidinapptransaction_p.h"
#include "qandroidinapppurchasebackend_p.h"
#include "qinappproduct.h"

QT_BEGIN_NAMESPACE

QAndroidInAppTransaction::QAndroidInAppTransaction(const QString &signature,
                                                   const QString &data,
                                                   const QString &purchaseToken,
                                                   const QString &orderId,
                                                   TransactionStatus status,
                                                   QInAppProduct *product,
                                                   QObject *parent)
    : QInAppTransaction(status, product, parent)
    , m_signature(signature)
    , m_data(data)
    , m_purchaseToken(purchaseToken)
    , m_orderId(orderId)
{
    Q_ASSERT(qobject_cast<QAndroidInAppPurchaseBackend *>(parent) != 0);
}

QString QAndroidInAppTransaction::platformProperty(const QString &propertyName) const
{
    if (propertyName.compare(QStringLiteral("AndroidSignature"), Qt::CaseInsensitive) == 0)
        return m_signature;
    else if (propertyName.compare(QStringLiteral("AndroidPurchaseData"), Qt::CaseInsensitive) == 0)
        return m_data;
    else if (propertyName.compare(QStringLiteral("AndroidOrderId"), Qt::CaseInsensitive) == 0)
        return m_orderId;
    else
        return QInAppTransaction::platformProperty(propertyName);
}

void QAndroidInAppTransaction::finalize()
{
    QAndroidInAppPurchaseBackend *backend = qobject_cast<QAndroidInAppPurchaseBackend *>(parent());
    if (status() == PurchaseApproved) {
        if (product()->productType() == QInAppProduct::Consumable)
            backend->consumeTransaction(m_purchaseToken);
        else
            backend->registerFinalizedUnlockable(product()->identifier());
    }

    deleteLater();
}

QT_END_NAMESPACE
