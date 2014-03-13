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

#ifndef QINAPPSTORE_P_H
#define QINAPPSTORE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qinappproduct.h"
#include "qinapppurchasebackend_p.h"
#include <QtCore/qmutex.h>

QT_BEGIN_NAMESPACE

class QInAppProduct;
class QInAppPurchaseBackend;

class QInAppStorePrivate
{
public:
    QInAppStorePrivate()
        : backend(0)
        , hasCalledInitialize(false)
        , pendingRestorePurchases(false)
    {
    }

    ~QInAppStorePrivate()
    {
        delete backend;
    }

    QHash<QString, QInAppProduct::ProductType> pendingProducts;
    QHash<QString, QInAppProduct *> registeredProducts;
    QInAppPurchaseBackend *backend;
    bool hasCalledInitialize;
    bool pendingRestorePurchases;
};

QT_END_NAMESPACE

#endif // QINAPPSTORE_P_H
