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

#include "qinappproductconsumable_p.h"
#include <QtMobileExtras/qinappstore.h>

QT_BEGIN_NAMESPACE

/*!
    \qmltype ConsumableProduct
    \inherits Product
    \inqmlmodule QtPurchasing
    \since QtPurchasing 1.0
    \ingroup qtpurchasing
    \brief Specifies a consumable product for in-app purchasing.

    A ConsumableProduct is created as a child of a \l Store component. Creating a
    consumable product will query its identifier from the platform's market place.
    If a consumable product with the given identifier is available in the market
    place, the component will be updated with information collected from the
    market place.

    For example, the following example will register the product named "myConsumableProduct"
    with the platform store and when the registration is successfully completed, it will
    show its price in a label:

    \qml
    Store {
        id: store
        ConsumableProduct {
            id: myConsumableProduct
            identifier: "myConsumableProduct"

            // ...
        }
    }

    Text {
        id: label
        text: myConsumableProduct.status === Product.Registered
              ? "The price of the consumable product is " + myConsumableProduct.price
              : "Consumable product is pending registration"
    }
    \endqml

    A ConsumableProduct is a product which can be purchased by the same user more than
    once.
*/

QInAppProductConsumable::QInAppProductConsumable(QObject *parent)
    : QInAppProductQmlType(QInAppProduct::Consumable, parent)
{
}

QT_END_NAMESPACE
