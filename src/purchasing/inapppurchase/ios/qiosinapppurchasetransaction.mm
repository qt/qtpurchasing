/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the Qt Purchasing Add-on.
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
    , m_failureReason(NoFailure)
{
    if (status == PurchaseFailed) {
        m_failureReason = ErrorOccurred;
        switch (m_nativeTransaction.error.code) {
        case SKErrorClientInvalid:
            m_errorString = QStringLiteral("Client Invalid");
            break;
        case SKErrorPaymentCancelled:
            m_errorString = QStringLiteral("Payment Cancelled");
            m_failureReason = CanceledByUser;
            break;
        case SKErrorPaymentInvalid:
            m_errorString = QStringLiteral("Payment Invalid");
            break;
        case SKErrorPaymentNotAllowed:
            m_errorString = QStringLiteral("Payment Not Allowed");
            break;
        case SKErrorStoreProductNotAvailable:
            m_errorString = QStringLiteral("Store Product Not Available");
            break;
        case SKErrorUnknown:
        default:
            m_errorString = QStringLiteral("Unknown");
        }
    }
}

void QIosInAppPurchaseTransaction::finalize()
{
    [[SKPaymentQueue defaultQueue] finishTransaction:m_nativeTransaction];
}

QString QIosInAppPurchaseTransaction::orderId() const
{
    return QString::fromNSString(m_nativeTransaction.transactionIdentifier);
}

QInAppTransaction::FailureReason QIosInAppPurchaseTransaction::failureReason() const
{
    return m_failureReason;
}

QString QIosInAppPurchaseTransaction::errorString() const
{
    return m_errorString;
}

QDateTime QIosInAppPurchaseTransaction::timestamp() const
{
    //Get time in seconds since 1970
    double timeInterval = [[m_nativeTransaction transactionDate] timeIntervalSince1970];
    return QDateTime::fromMSecsSinceEpoch(qint64(timeInterval * 1000));
}

QT_END_NAMESPACE

#include "moc_qiosinapppurchasetransaction_p.cpp"
