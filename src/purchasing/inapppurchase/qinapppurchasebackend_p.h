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
