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

#include "qiosinapppurchasetransaction_p.h"
#include "qiosinapppurchasebackend_p.h"

#import <StoreKit/StoreKit.h>

QT_BEGIN_NAMESPACE

QIosInAppPurchaseTransaction::QIosInAppPurchaseTransaction(SKPaymentTransaction *transaction,
                                           const TransactionStatus status,
                                           QInAppProduct *product,
                                           QIosInAppPurchaseBackend *backend)
    : QInAppTransaction(status, product, backend)
    , m_nativeTransaction(transaction)
{
}

void QIosInAppPurchaseTransaction::finalize()
{
    [[SKPaymentQueue defaultQueue] finishTransaction:m_nativeTransaction];
}

QString QIosInAppPurchaseTransaction::platformProperty(const QString &propertyName) const
{
    if (propertyName == QStringLiteral("transactionIdentifier")) {
        return QString::fromNSString(m_nativeTransaction.transactionIdentifier);
    }

    if (propertyName == QStringLiteral("error")) {
        if (status() != PurchaseFailed)
            return QString();
        QString errorString;
        switch (m_nativeTransaction.error.code) {
        case SKErrorClientInvalid:
            errorString = QStringLiteral("Client Invalid");
            break;
        case SKErrorPaymentCancelled:
            errorString = QStringLiteral("Payment Cancelled");
            break;
        case SKErrorPaymentInvalid:
            errorString = QStringLiteral("Payment Invalid");
            break;
        case SKErrorPaymentNotAllowed:
            errorString = QStringLiteral("Payment Not Allowed");
            break;
        case SKErrorStoreProductNotAvailable:
            errorString = QStringLiteral("Store Product Not Available");
        case SKErrorUnknown:
        default:
            errorString = QStringLiteral("Unknown");
            break;
        }
        return errorString;
    }

    return QInAppTransaction::platformProperty(propertyName);
}

QT_END_NAMESPACE

#include "moc_qiosinapppurchasetransaction_p.cpp"
