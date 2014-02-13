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

#include "qinappproductqmltype_p.h"
#include "qinappstoreqmltype_p.h"
#include <QtMobileExtras/qinapptransaction.h>
#include <QtMobileExtras/qinappstore.h>
#include <QtCore/qcoreevent.h>

QT_BEGIN_NAMESPACE

QInAppProductQmlType::QInAppProductQmlType(QInAppProduct::ProductType requiredType, QObject *parent)
    : QObject(parent)
    , m_status(Uninitialized)
    , m_requiredType(requiredType)
    , m_componentComplete(false)
    , m_store(0)
    , m_product(0)
{
}

void QInAppProductQmlType::setStore(QInAppStore *store)
{
    if (m_store == store && m_store != 0)
        return;

    if (m_store != 0)
        m_store->disconnect(this);

    m_store = store;
    if (m_store == 0) {
        qWarning("Parent of products should be a Store instance.");
    } else {
        connect(m_store, SIGNAL(productRegistered(QInAppProduct*)),
                this, SLOT(handleProductRegistered(QInAppProduct *)));
        connect(m_store, SIGNAL(productUnknown(QInAppProduct::ProductType,QString)),
                this, SLOT(handleProductUnknown(QInAppProduct::ProductType,QString)));
        connect(m_store, SIGNAL(transactionReady(QInAppTransaction*)),
                this, SLOT(handleTransaction(QInAppTransaction*)));
    }

    updateProduct();
}

void QInAppProductQmlType::componentComplete()
{
    if (!m_componentComplete) {
        m_componentComplete = true;
        updateProduct();
    }
}

void QInAppProductQmlType::setIdentifier(const QString &identifier)
{
    if (m_identifier == identifier)
        return;

    m_identifier = identifier;
    if (m_componentComplete)
        updateProduct();
    emit identifierChanged();
}

void QInAppProductQmlType::updateProduct()
{
    if (m_store == 0 )
        return;

    Status oldStatus = m_status;
    QInAppProduct *product = 0;
    if (m_identifier.isEmpty()) {
        m_status = Unknown;
    } else {
        product = m_store->registeredProduct(m_identifier);
        if (product != 0 && product == m_product)
            return;

        if (product == 0) {
            m_status = PendingRegistration;
            m_store->registerProduct(m_requiredType, m_identifier);
        } else if (product->productType() != m_requiredType) {
            product = 0;
            m_status = Unknown;
        } else {
            m_status = Registered;
        }
    }

    setProduct(product);
    if (oldStatus != m_status)
        emit statusChanged();
}

QString QInAppProductQmlType::identifier() const
{
    return m_identifier;
}

QInAppProductQmlType::Status QInAppProductQmlType::status() const
{
    return m_status;
}

QString QInAppProductQmlType::price() const
{
    return m_product != 0 ? m_product->price() : QString();
}

void QInAppProductQmlType::setProduct(QInAppProduct *product)
{
    if (m_product == product)
        return;

    QString oldPrice = price();
    m_product = product;
    if (price() != oldPrice)
        emit priceChanged();
}

void QInAppProductQmlType::handleProductRegistered(QInAppProduct *product)
{
    if (product->identifier() == m_identifier && product->productType() == m_requiredType) {
        setProduct(product);
        if (m_status != Registered) {
            m_status = Registered;
            emit statusChanged();
        }
    } else if (product->identifier() == m_identifier) {
        setProduct(0);
        if (m_status != Unknown) {
            m_status = Unknown;
            emit statusChanged();
        }
    }
}

void QInAppProductQmlType::handleProductUnknown(QInAppProduct::ProductType, const QString &identifier)
{
    if (identifier == m_identifier) {
        setProduct(0);
        if (m_status != Unknown) {
            m_status = Unknown;
            emit statusChanged();
        }
    }
}

void QInAppProductQmlType::handleTransaction(QInAppTransaction *transaction)
{
    if (transaction->status() == QInAppTransaction::PurchaseApproved)
        emit purchaseSucceeded(transaction);
    else
        emit purchaseFailed(transaction);
}

void QInAppProductQmlType::purchase()
{
    if (m_product != 0)
        m_product->purchase();
    else
        qWarning("Attempted to purchase unregistered product");
}

QT_END_NAMESPACE
