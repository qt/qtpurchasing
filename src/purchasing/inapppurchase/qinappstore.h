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

#ifndef QINAPPSTORE_H
#define QINAPPSTORE_H

#include <QtCore/qobject.h>
#include <QtPurchasing/qtpurchasingglobal.h>
#include <QtPurchasing/qinappproduct.h>

QT_BEGIN_NAMESPACE

class QInAppStorePrivate;
class QInAppProduct;
class QInAppTransaction;
class Q_PURCHASING_EXPORT QInAppStore: public QObject
{
    Q_OBJECT
public:
    explicit QInAppStore(QObject *parent = nullptr);
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
