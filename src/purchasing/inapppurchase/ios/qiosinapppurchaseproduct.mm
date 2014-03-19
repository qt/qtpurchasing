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

#include "qiosinapppurchaseproduct_p.h"
#include "qiosinapppurchasebackend_p.h"

#import <StoreKit/StoreKit.h>

//Use a Catagory to add a localizedPrice method to SKProduct
@interface SKProduct (LocalizedPrice)

@property (nonatomic, readonly) NSString *localizedPrice;

@end

@implementation SKProduct (LocalizedPrice)

- (NSString *)localizedPrice
{
    NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
    [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
    [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
    [numberFormatter setLocale:self.priceLocale];
    NSString *formattedString = [numberFormatter stringFromNumber:self.price];
    [numberFormatter release];
    return formattedString;
}

@end

QT_BEGIN_NAMESPACE

QIosInAppPurchaseProduct::QIosInAppPurchaseProduct(SKProduct *product,
                                                   ProductType productType,
                                                   QIosInAppPurchaseBackend *backend)
    : QInAppProduct(QString::fromNSString([product localizedPrice]),
                    QString::fromNSString([product localizedTitle]),
                    QString::fromNSString([product localizedDescription]),
                    productType,
                    QString::fromNSString([product productIdentifier]),
                    backend)
    , m_nativeProduct(product)
{
}

void QIosInAppPurchaseProduct::purchase()
{
    SKPayment *payment = [SKPayment paymentWithProduct:m_nativeProduct];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

QT_END_NAMESPACE

#include "moc_qiosinapppurchaseproduct_p.cpp"
