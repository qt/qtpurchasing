/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Purchasing module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
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
                                                   const QDateTime &timestamp,
                                                   FailureReason failureReason,
                                                   const QString &errorString,
                                                   QObject *parent)
    : QInAppTransaction(status, product, parent)
    , m_signature(signature)
    , m_data(data)
    , m_purchaseToken(purchaseToken)
    , m_orderId(orderId)
    , m_timestamp(timestamp)
    , m_errorString(errorString)
    , m_failureReason(failureReason)
{
    Q_ASSERT(qobject_cast<QAndroidInAppPurchaseBackend *>(parent) != 0);
}

QString QAndroidInAppTransaction::orderId() const
{
    return m_orderId;
}

QDateTime QAndroidInAppTransaction::timestamp() const
{
    return m_timestamp;
}

QString QAndroidInAppTransaction::errorString() const
{
    return m_errorString;
}

QInAppTransaction::FailureReason QAndroidInAppTransaction::failureReason() const
{
    return m_failureReason;
}

QString QAndroidInAppTransaction::platformProperty(const QString &propertyName) const
{
    if (propertyName.compare(QStringLiteral("AndroidSignature"), Qt::CaseInsensitive) == 0)
        return m_signature;
    else if (propertyName.compare(QStringLiteral("AndroidPurchaseData"), Qt::CaseInsensitive) == 0)
        return m_data;
    else
        return QInAppTransaction::platformProperty(propertyName);
}

void QAndroidInAppTransaction::finalize()
{
    QAndroidInAppPurchaseBackend *backend = qobject_cast<QAndroidInAppPurchaseBackend *>(parent());
    if (status() == PurchaseApproved || status() == PurchaseRestored) {
        if (product()->productType() == QInAppProduct::Consumable)
            backend->consumeTransaction(m_purchaseToken);
        else
            backend->registerFinalizedUnlockable(product()->identifier(), m_purchaseToken);
    }

    deleteLater();
}

QT_END_NAMESPACE
