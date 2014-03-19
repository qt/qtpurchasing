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

#ifndef QINAPPPRODUCTQMLTYPE_P_H
#define QINAPPPRODUCTQMLTYPE_P_H

#include <QtPurchasing/qinappproduct.h>
#include <QtQml/qqmlparserstatus.h>
#include <QtCore/qobject.h>

QT_BEGIN_NAMESPACE

class QInAppStore;
class QInAppTransaction;
class QInAppProductQmlType : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_ENUMS(Status)
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(QString price READ price NOTIFY priceChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
public:
    enum Status {
        Uninitialized,
        PendingRegistration,
        Registered,
        Unknown
    };

    explicit QInAppProductQmlType(QInAppProduct::ProductType productType,
                                  QObject *parent = 0);

    Q_INVOKABLE void purchase();

    void setIdentifier(const QString &identifier);
    QString identifier() const;

    Status status() const;
    QString price() const;
    QString title() const;
    QString description() const;

    void setStore(QInAppStore *store);

Q_SIGNALS:
    void purchaseSucceeded(QInAppTransaction *transaction);
    void purchaseFailed(QInAppTransaction *transaction);
    void purchaseRestored(QInAppTransaction *transaction);
    void identifierChanged();
    void statusChanged();
    void priceChanged();
    void titleChanged();
    void descriptionChanged();

protected:
    void componentComplete();
    void classBegin() {}

private Q_SLOTS:
    void handleTransaction(QInAppTransaction *transaction);
    void handleProductRegistered(QInAppProduct *product);
    void handleProductUnknown(QInAppProduct::ProductType, const QString &identifier);

private:
    void setProduct(QInAppProduct *product);
    void updateProduct();

    QString m_identifier;
    Status m_status;
    QInAppProduct::ProductType m_requiredType;
    bool m_componentComplete;

    QInAppStore *m_store;
    QInAppProduct *m_product;
};

QT_END_NAMESPACE

#endif // QINAPPPRODUCTQMLTYPE_P_H
