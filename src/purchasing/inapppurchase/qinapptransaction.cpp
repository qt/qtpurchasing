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

#include "qinapptransaction.h"

QT_BEGIN_NAMESPACE

struct QInAppTransactionPrivate
{
    QInAppTransactionPrivate(QInAppTransaction::TransactionStatus s,
                             QInAppProduct *p)
        : status(s)
        , product(p)
    {
    }

    QInAppTransaction::TransactionStatus status;
    QInAppProduct *product;
};

/*!
 *  \class QInAppTransaction
 *  \brief Contains information about a transaction in the external app store
 *
 * QInAppTransaction contains information about a transaction in the external app store and is
 * usually provided as a result of calling QInAppProduct::purchase(). When the purchase flow has
 * been completed by the user (confirming the purchase, for instance by entering their password),
 * the QInAppStore instance containing the product will emit a QInAppStore::transactionReady()
 * signal with data about the transaction.
 *
 * The status() provides information on whether the transaction was successful or not. If it was
 * successful, then the application should take appropriate action. When the necessary action has
 * been performed, finalize() should be called. The finalize() function should be called regardless
 * of the status of the transaction.
 *
 * It is important that the application stores the purchase information before calling finalize().
 * If a transaction is not finalized (for example because the application was interrupted before
 * it had a chance to save the information), then the transaction will be emitted again the next
 * time the product is registered by QInAppStore::registerProduct().
 *
 * Transactions can also be emitted after calling QInAppStore::restorePurchases(), at which point
 * a new transaction will be emitted for each previously purchased unlockable product with the
 * status of PurchaseRestored.
 *
 * \note Since transactions may under certain circumstances be emitted for the same transaction
 * several times, the application should always check if the transaction has been registered
 * before. Do not expect each transaction to be unique.
 */

/*!
 * \internal
 */\
QInAppTransaction::QInAppTransaction(TransactionStatus status, QInAppProduct *product, QObject *parent)
    : QObject(parent)
{
    d = QSharedPointer<QInAppTransactionPrivate>(new QInAppTransactionPrivate(status, product));
}

/*!
 * \internal
 */\
QInAppTransaction::~QInAppTransaction()
{
}

/*!
 * Returns the product which is the object of this transaction.
 */
QInAppProduct *QInAppTransaction::product() const
{
    return d->product;
}

/*!
 * Returns the status of the transaction. If the purchase was successfully
 * completed, the status will be PurchaseApproved. If the purchase failed
 * or was unsuccessful then the status will be PurchaseFailed.  If the
 * transaction was restored as a result of calling QInAppStore::restorePurchases()
 * then the status will be PurchaseRestored.
 */

QInAppTransaction::TransactionStatus QInAppTransaction::status() const
{
    return d->status;
}

/*!
 * Returns a unique identifier for this transaction. This value may be an empty
 * string if no transaction was registered (for example for canceled purchases).
 *
 * \sa platformProperty()
 */
QString QInAppTransaction::orderId() const
{
    return QString();
}

/*!
 * Returns the platform-specific property given by \a propertyName.
 *
 * The following properties are available on Google Play:
 * \list
 * \li AndroidSignature: The signature of the transaction, as given by the
 *     private key for the application.
 * \li AndroidPurchaseData: The purchase data returned by the Google Play store.
 * \endlist
 * These properties can be used to verify the purchase using the public key of
 * your application. It is also possible to have the back-end verify the purchases
 * by passing in the public key before registering products, using
 * QInAppStore::setPlatformProperty().
 */
QString QInAppTransaction::platformProperty(const QString &propertyName) const
{
    Q_UNUSED(propertyName);
    return QString();
}

/*!
 * \fn void QInAppTransaction::finalize()
 *
 * Call this when the application has finished performing all necessary reactions
 * to the purchase. If the status is PurchaseApproved, the application should
 * store the information about the transaction in a safe way before finalizing it.
 * All transactions should be finalized.
 */

QT_END_NAMESPACE
