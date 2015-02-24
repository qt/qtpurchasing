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

#ifndef QINAPPPURCHASEBACKEND_P_H
#define QINAPPPURCHASEBACKEND_P_H

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
#include <QtCore/qobject.h>

QT_BEGIN_NAMESPACE

class QInAppProduct;
class QInAppTransaction;
class QInAppStore;
class QInAppPurchaseBackend : public QObject
{
    Q_OBJECT
public:
    struct Product
    {
        Product(QInAppProduct::ProductType type, const QString &id)
            : productType(type), identifier(id)
        {
        }

        QInAppProduct::ProductType productType;
        QString identifier;
    };

    explicit QInAppPurchaseBackend(QObject *parent = 0);

    virtual void initialize();
    virtual bool isReady() const;

    virtual void queryProducts(const QList<Product> &products);
    virtual void queryProduct(QInAppProduct::ProductType productType, const QString &identifier);
    virtual void restorePurchases();

    virtual void setPlatformProperty(const QString &propertyName, const QString &value);

    void setStore(QInAppStore *store) { m_store = store; }
    QInAppStore *store() const { return m_store; }

Q_SIGNALS:
    void ready();
    void transactionReady(QInAppTransaction *transaction);
    void productQueryFailed(QInAppProduct::ProductType productType, const QString &identifier);
    void productQueryDone(QInAppProduct *product);

private:
    QInAppStore *m_store;
};

QT_END_NAMESPACE

#endif // QINAPPPURCHASEBACKEND_P_H
