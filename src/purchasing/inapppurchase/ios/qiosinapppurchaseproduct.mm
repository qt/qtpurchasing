/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Purchasing module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
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
