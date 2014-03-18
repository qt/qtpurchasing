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

#ifndef QINAPPSTORE_H
#define QINAPPSTORE_H

#include "qinappproduct.h"

#include <QtCore/qobject.h>
#include <QtPurchasing/qtpurchasingglobal.h>

QT_BEGIN_NAMESPACE

class QInAppStorePrivate;
class QInAppProduct;
class QInAppTransaction;
class Q_PURCHASING_EXPORT QInAppStore: public QObject
{
    Q_OBJECT
public:
    QInAppStore(QObject *parent = 0);
    ~QInAppStore();

    Q_INVOKABLE void restorePurchases();
    Q_INVOKABLE void registerProduct(QInAppProduct::ProductType productType, const QString &identifier);
    Q_INVOKABLE QInAppProduct *registeredProduct(const QString &identifier) const;
    Q_INVOKABLE void setPlatformProperty(const QString &propertyName, const QString &value);

Q_SIGNALS:
    void productRegistered(QInAppProduct *product);
    void productUnknown(QInAppProduct::ProductType productType, const QString &identifier);
    void transactionReady(QInAppTransaction *transaction);

private Q_SLOTS:
    void registerPendingProducts();
    void registerProduct(QInAppProduct *);

private:
    void setupBackend();

    Q_DISABLE_COPY(QInAppStore)
    QSharedPointer<QInAppStorePrivate> d;
};

QT_END_NAMESPACE

#endif // QINAPPSTORE_H
