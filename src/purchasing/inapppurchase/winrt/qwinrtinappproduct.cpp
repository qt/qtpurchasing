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

#include "qwinrtinappproduct_p.h"
#include "qwinrtinapppurchasebackend_p.h"

#include <QLoggingCategory>

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcPurchasingProduct, "qt.purchasing.product")

QWinRTInAppProduct::QWinRTInAppProduct(QWinRTInAppPurchaseBackend *backend,
                                           const QString &price,
                                           const QString &title,
                                           const QString &description,
                                           ProductType productType,
                                           const QString &identifier,
                                           QObject *parent)
    : QInAppProduct(price, title, description, productType, identifier, parent)
    , m_backend(backend)
{
    qCDebug(lcPurchasingProduct) << __FUNCTION__;
}


void QWinRTInAppProduct::purchase()
{
    qCDebug(lcPurchasingProduct) << __FUNCTION__;
    m_backend->purchaseProduct(this);
}

QT_END_NAMESPACE

