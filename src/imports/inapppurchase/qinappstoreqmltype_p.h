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

#ifndef QINAPPSTOREQMLTYPE_P_H
#define QINAPPSTOREQMLTYPE_P_H

#include "qinappproductqmltype_p.h"
#include <QtQml/qqmllist.h>

#include <QtCore/qlist.h>
#include <QtCore/qobject.h>

QT_BEGIN_NAMESPACE

class QInAppStore;
class QInAppStoreQmlType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QInAppProductQmlType> products READ products DESIGNABLE false)
    Q_CLASSINFO("DefaultProperty", "products")
public:
    explicit QInAppStoreQmlType(QObject *parent = 0);

    QInAppStore *store() const;
    QQmlListProperty<QInAppProductQmlType> products();

    Q_INVOKABLE void restorePurchases();

private:
    QInAppStore *m_store;
    QList<QInAppProductQmlType *> m_products;
};

QT_END_NAMESPACE

#endif // QINAPPSTOREQMLTYPE_P_H
