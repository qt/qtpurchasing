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

#include "qinappstoreqmltype_p.h"
#include <QtMobileExtras/qinappstore.h>

static void addProduct(QQmlListProperty<QInAppProductQmlType> *property, QInAppProductQmlType *product)
{
    QInAppStoreQmlType *store = qobject_cast<QInAppStoreQmlType *>(property->object);
    Q_ASSERT(store != 0);
    product->setStore(store->store());

    QList<QInAppProductQmlType *> *m_products = reinterpret_cast<QList<QInAppProductQmlType *> *>(property->data);
    Q_ASSERT(m_products != 0);

    m_products->append(product);
}

static int productCount(QQmlListProperty<QInAppProductQmlType> *property)
{
    QList<QInAppProductQmlType *> *m_products = reinterpret_cast<QList<QInAppProductQmlType *> *>(property->data);
    Q_ASSERT(m_products != 0);

    return m_products->size();
}

static void clearProducts(QQmlListProperty<QInAppProductQmlType> *property)
{
    QList<QInAppProductQmlType *> *m_products = reinterpret_cast<QList<QInAppProductQmlType *> *>(property->data);
    Q_ASSERT(m_products != 0);

    foreach (QInAppProductQmlType *product, m_products)
        product->setStore(0);
    m_products->clear();
}

static QInAppProductQmlType *productAt(QQmlListProperty<QInAppProductQmlType> *property, int index)
{
    QList<QInAppProductQmlType *> *m_products = reinterpret_cast<QList<QInAppProductQmlType *> *>(property->data);
    Q_ASSERT(m_products != 0);

    return m_products->at(index);
}

QInAppStoreQmlType::QInAppStoreQmlType(QObject *parent)
    : QObject(parent)
    , m_store(new QInAppStore(this))
{
}

QInAppStore *QInAppStoreQmlType::store() const
{
    return m_store;
}

QQmlListProperty<QInAppProductQmlType> QInAppStoreQmlType::products()
{
    return QQmlListProperty<QInAppProductQmlType>(this, &m_products, &addProduct, &productCount, &productAt, &clearProducts);
}
