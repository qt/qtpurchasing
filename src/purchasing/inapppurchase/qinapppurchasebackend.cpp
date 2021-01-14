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

#include "qinapppurchasebackend_p.h"

QT_BEGIN_NAMESPACE

QInAppPurchaseBackend::QInAppPurchaseBackend(QObject *parent)
    : QObject(parent)
    , m_store(0)
{
}

void QInAppPurchaseBackend::initialize()
{
    emit ready();
}

bool QInAppPurchaseBackend::isReady() const
{
    return true;
}

void QInAppPurchaseBackend::queryProducts(const QList<Product> &products)
{
    for (const Product &product : products)
        queryProduct(product.productType, product.identifier);
}

void QInAppPurchaseBackend::queryProduct(QInAppProduct::ProductType productType,
                                                    const QString &identifier)
{
    qWarning("QInAppPurchaseBackend not implemented on this platform!");
    Q_UNUSED(productType);
    Q_UNUSED(identifier);
}

void QInAppPurchaseBackend::restorePurchases()
{
    qWarning("QInAppPurchaseBackend not implemented on this platform!");
}

void QInAppPurchaseBackend::setPlatformProperty(const QString &propertyName, const QString &value)
{
    Q_UNUSED(propertyName);
    Q_UNUSED(value);
}

QT_END_NAMESPACE
