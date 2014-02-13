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

#ifndef QINAPPPRODUCT_H
#define QINAPPPRODUCT_H

#include <QtCore/qobject.h>
#include <QtCore/qsharedpointer.h>
#include <QtMobileExtras/qmobileextrasglobal.h>

QT_BEGIN_NAMESPACE

class QInAppProductPrivate;
class Q_MOBILEEXTRAS_EXPORT QInAppProduct: public QObject
{
    Q_OBJECT
    Q_ENUMS(ProductType)
    Q_PROPERTY(QString identifier READ identifier CONSTANT)
    Q_PROPERTY(ProductType productType READ productType CONSTANT)
    Q_PROPERTY(QString price READ price CONSTANT)

public:
    enum ProductType
    {
        Consumable,
        Unlockable
    };

    ~QInAppProduct();

    QString identifier() const;
    ProductType productType() const;

    QString price() const;

    Q_INVOKABLE virtual void purchase() = 0;

protected:
    explicit QInAppProduct(const QString &price, ProductType productType, const QString &identifier, QObject *parent = 0);

private:
    friend class QInAppStore;
    Q_DISABLE_COPY(QInAppProduct)

    QSharedPointer<QInAppProductPrivate> d;
};

QT_END_NAMESPACE

#endif // QINAPPPRODUCT_H
