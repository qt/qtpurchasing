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

#ifndef QIOSINAPPPURCHASEBACKEND_P_H
#define QIOSINAPPPURCHASEBACKEND_P_H

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

#include "qinapppurchasebackend_p.h"
#include "qinappproduct.h"
#include "qinapptransaction.h"

#include <QtCore/QHash>

QT_BEGIN_NAMESPACE

class QIosInAppPurchaseProduct;
class QIosInAppPurchaseTransaction;

class QIosInAppPurchaseBackend : public QInAppPurchaseBackend
{
    Q_OBJECT
public:
    QIosInAppPurchaseBackend(QObject *parent = 0);
    ~QIosInAppPurchaseBackend();

    void initialize();
    bool isReady() const;
    void queryProduct(QInAppProduct::ProductType productType, const QString &identifier);
    void restorePurchases();
    void setPlatformProperty(const QString &propertyName, const QString &value);

    //Called by InAppPurchaseManager
    Q_INVOKABLE void registerProduct(QIosInAppPurchaseProduct *product);
    Q_INVOKABLE void registerQueryFailure(const QString &productId);
    Q_INVOKABLE void registerTransaction(QIosInAppPurchaseTransaction *transaction);
    QInAppProduct::ProductType productTypeForProductId(const QString &productId);
    QIosInAppPurchaseProduct *registeredProductForProductId(const QString &productId);

private:
    void *m_iapManager;
    QHash<QString, QInAppProduct::ProductType> m_productTypeForPendingId;
    QHash<QString, QIosInAppPurchaseProduct*> m_registeredProductForId;
};

#endif // QIOSINAPPPURCHASEBACKEND_P_H

QT_END_NAMESPACE
