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

#include "qandroidinappproduct_p.h"
#include "qandroidinapppurchasebackend_p.h"

QT_BEGIN_NAMESPACE

QAndroidInAppProduct::QAndroidInAppProduct(QAndroidInAppPurchaseBackend *backend,
                                           const QString &price,
                                           ProductType productType,
                                           const QString &identifier,
                                           QObject *parent)
    : QInAppProduct(price, productType, identifier, parent)
    , m_backend(backend)
{
}


void QAndroidInAppProduct::purchase()
{
    m_backend->purchaseProduct(this);
}

QT_END_NAMESPACE

