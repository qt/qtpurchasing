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

#include "qiosinapppurchasebackend_p.h"
#include "qiosinapppurchaseproduct_p.h"
#include "qiosinapppurchasetransaction_p.h"

#include <QtCore/QString>

#import <StoreKit/StoreKit.h>

@interface InAppPurchaseManager : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
    QIosInAppPurchaseBackend *backend;
    NSMutableArray *pendingTransactions;
}

-(void)requestProductData:(NSString *)identifier;
-(void)processPendingTransactions;

@end

@implementation InAppPurchaseManager

-(id)initWithBackend:(QIosInAppPurchaseBackend *)iapBackend {
    if (self = [super init]) {
        backend = iapBackend;
        pendingTransactions = [[NSMutableArray alloc] init];
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    }
    return self;
}

-(void)dealloc
{
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    [pendingTransactions release];
    [super dealloc];
}

-(void)requestProductData:(NSString *)identifier
{
    NSSet *productId = [NSSet setWithObject:identifier];
    SKProductsRequest *productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productId];
    productsRequest.delegate = self;
    [productsRequest start];
}

-(void)processPendingTransactions
{
    NSMutableArray *registeredTransactions = [NSMutableArray array];

    for (SKPaymentTransaction *transaction in pendingTransactions) {
        QInAppTransaction::TransactionStatus status = [InAppPurchaseManager statusFromTransaction:transaction];

        QIosInAppPurchaseProduct *product = backend->registeredProductForProductId(QString::fromNSString(transaction.payment.productIdentifier));

        if (product) {
            //It is possible that the product doesn't exist yet (because of previous restores).
            QIosInAppPurchaseTransaction *qtTransaction = new QIosInAppPurchaseTransaction(transaction, status, product, backend);
            [registeredTransactions addObject:transaction];
            QMetaObject::invokeMethod(backend, "registerTransaction", Qt::AutoConnection, Q_ARG(QIosInAppPurchaseTransaction*, qtTransaction));
        }
    }

    //Remove registeredTransactions from pendingTransactions
    [pendingTransactions removeObjectsInArray:registeredTransactions];
}


//SKProductsRequestDelegate
-(void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    NSArray *products = response.products;
    SKProduct *product = [products count] == 1 ? [[products firstObject] retain] : nil;

    if (product == nil) {
        //Invalid product ID
        NSString *invalidId = [response.invalidProductIdentifiers firstObject];
        QMetaObject::invokeMethod(backend, "registerQueryFailure", Qt::AutoConnection, Q_ARG(QString, QString::fromNSString(invalidId)));
    } else {
        //Valid product query
        //Create a QIosInAppPurchaseProduct
        QIosInAppPurchaseProduct *validProduct = new QIosInAppPurchaseProduct(product, backend->productTypeForProductId(QString::fromNSString([product productIdentifier])), backend);
        QMetaObject::invokeMethod(backend, "registerProduct", Qt::AutoConnection, Q_ARG(QIosInAppPurchaseProduct*, validProduct));
    }

    [request release];
}

+(QInAppTransaction::TransactionStatus)statusFromTransaction:(SKPaymentTransaction *)transaction
{
    QInAppTransaction::TransactionStatus status;
    switch (transaction.transactionState) {
        case SKPaymentTransactionStatePurchasing:
            //Ignore the purchasing state as it's not really a transaction
            //And its important that it doesn't need to be finalized as
            //Calling finishTransaction: on a transaction that is
            //in the SKPaymentTransactionStatePurchasing state throws an exception
            status = QInAppTransaction::Unknown;
            break;
        case SKPaymentTransactionStatePurchased:
            status = QInAppTransaction::PurchaseApproved;
            break;
        case SKPaymentTransactionStateFailed:
            status = QInAppTransaction::PurchaseFailed;
            break;
        case SKPaymentTransactionStateRestored:
            status = QInAppTransaction::PurchaseRestored;
            break;
        default:
            status = QInAppTransaction::Unknown;
            break;
    }
    return status;
}

//SKPaymentTransactionObserver
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    Q_UNUSED(queue);
    for (SKPaymentTransaction *transaction in transactions) {
        //Create QIosInAppPurchaseTransaction
        QInAppTransaction::TransactionStatus status = [InAppPurchaseManager statusFromTransaction:transaction];

        if (status == QInAppTransaction::Unknown)
            continue;

        QIosInAppPurchaseProduct *product = backend->registeredProductForProductId(QString::fromNSString(transaction.payment.productIdentifier));

        if (product) {
            //It is possible that the product doesn't exist yet (because of previous restores).
            QIosInAppPurchaseTransaction *qtTransaction = new QIosInAppPurchaseTransaction(transaction, status, product, backend);
            QMetaObject::invokeMethod(backend, "registerTransaction", Qt::AutoConnection, Q_ARG(QIosInAppPurchaseTransaction*, qtTransaction));
        } else {
            //Add the transaction to the pending transactions list
            [pendingTransactions addObject:transaction];
        }
    }
}

@end


QT_BEGIN_NAMESPACE

QIosInAppPurchaseBackend::QIosInAppPurchaseBackend(QObject *parent)
    : QInAppPurchaseBackend(parent)
    , m_iapManager(0)
{
}

QIosInAppPurchaseBackend::~QIosInAppPurchaseBackend()
{
    [(InAppPurchaseManager*)m_iapManager release];
}

void QIosInAppPurchaseBackend::initialize()
{
    m_iapManager = [[InAppPurchaseManager alloc] initWithBackend:this];
    emit QInAppPurchaseBackend::ready();
}

bool QIosInAppPurchaseBackend::isReady() const
{
    if (m_iapManager)
        return true;
    return false;
}

void QIosInAppPurchaseBackend::queryProduct(QInAppProduct::ProductType productType, const QString &identifier)
{
    Q_UNUSED(productType)

    if (m_productTypeForPendingId.contains(identifier)) {
        qWarning("Product query already pending for %s", qPrintable(identifier));
        return;
    }

    m_productTypeForPendingId[identifier] = productType;

    [(InAppPurchaseManager*)m_iapManager requestProductData:(identifier.toNSString())];
}

void QIosInAppPurchaseBackend::restorePurchases()
{
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

void QIosInAppPurchaseBackend::setPlatformProperty(const QString &propertyName, const QString &value)
{
    Q_UNUSED(propertyName);
    Q_UNUSED(value);
}

void QIosInAppPurchaseBackend::registerProduct(QIosInAppPurchaseProduct *product)
{
    QHash<QString, QInAppProduct::ProductType>::iterator it = m_productTypeForPendingId.find(product->identifier());
    Q_ASSERT(it != m_productTypeForPendingId.end());

    m_registeredProductForId[product->identifier()] = product;
    emit productQueryDone(product);
    m_productTypeForPendingId.erase(it);
    [m_iapManager processPendingTransactions];
}

void QIosInAppPurchaseBackend::registerQueryFailure(const QString &productId)
{
    QHash<QString, QInAppProduct::ProductType>::iterator it = m_productTypeForPendingId.find(productId);
    Q_ASSERT(it != m_productTypeForPendingId.end());

    emit QInAppPurchaseBackend::productQueryFailed(it.value(), it.key());
    m_productTypeForPendingId.erase(it);
}

void QIosInAppPurchaseBackend::registerTransaction(QIosInAppPurchaseTransaction *transaction)
{
    emit QInAppPurchaseBackend::transactionReady(transaction);
}

QInAppProduct::ProductType QIosInAppPurchaseBackend::productTypeForProductId(const QString &productId)
{
    return m_productTypeForPendingId[productId];
}

QIosInAppPurchaseProduct *QIosInAppPurchaseBackend::registeredProductForProductId(const QString &productId)
{
    return m_registeredProductForId[productId];
}

QT_END_NAMESPACE

#include "moc_qiosinapppurchasebackend_p.cpp"
