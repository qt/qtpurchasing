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

#include "qinappproduct.h"

QT_BEGIN_NAMESPACE

struct QInAppProductPrivate
{
    QInAppProductPrivate(const QString &price, QInAppProduct::ProductType type, const QString &id)
        : localPrice(price)
        , productType(type)
        , identifier(id)
    {
    }

    QString localPrice;
    QInAppProduct::ProductType productType;
    QString identifier;
};

/*!
 *  \class QInAppProduct
 *  \brief A product registered in the store
 *
 *  QInAppProduct encapsulates a product in the external store after it has been registered in \c QInAppStore
 *  and confirmed to exist. It has an identifier which matches the identifier of the product in the external
 *  store, it has a price which is retrieved from the external store, and it has a product type.
 *
 *  The product type can be either \c Consumable or \c Unlockable. The former type of products can be purchased
 *  any number of times as long as each transaction is finalized explicitly by the application. The latter type
 *  can only be purchased once.
 */

/*!
 * \internal
 */\
QInAppProduct::QInAppProduct(const QString &price, ProductType productType, const QString &identifier, QObject *parent)
    : QObject(parent)
{
    d = QSharedPointer<QInAppProductPrivate>(new QInAppProductPrivate(price, productType, identifier));
}

/*!
 * \internal
 */\
QInAppProduct::~QInAppProduct()
{
}

/*!
 * Returns the price of the product as reported by the external store. This is usually the price in the
 * locale of the current user.
 */
QString QInAppProduct::price() const
{
    return d->localPrice;
}

/*!
 * Returns the identifier of the product. This matches the identifier of the product which is registered
 * in the external store.
 */
QString QInAppProduct::identifier() const
{
    return d->identifier;
}

/*!
 * Returns the type of the product. This can either be \c Consumable or \c Unlockable. The former are products
 * which can be purchased any number of times (granted that each transaction is explicitly finalized by the
 * application first) and the latter are products which can only be purchased once per user.
 */
QInAppProduct::ProductType QInAppProduct::productType() const
{
    return d->productType;
}

/*!
 * \fn void QInAppProduct::purchase()
 *
 * Launches the purchase flow for this product. The purchase is done asynchronously. When the purchase has
 * either been completed successfully or failed for some reason, the QInAppStore instance containing
 * this product will emit a QInAppStore::transactionReady() signal with information about the transaction.
 *
 * \sa QInAppTransaction
 */

QT_END_NAMESPACE
