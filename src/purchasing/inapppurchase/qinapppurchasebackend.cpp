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
