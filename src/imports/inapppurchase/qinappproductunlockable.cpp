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

#include "qinappproductunlockable_p.h"

QT_BEGIN_NAMESPACE

/*!
    \qmltype UnlockableProduct
    \inherits Product
    \inqmlmodule QtPurchasing
    \since QtPurchasing 1.0
    \ingroup qtpurchasing
    \brief Specifies an unlockable product for in-app purchasing.

    An UnlockableProduct is created as a child of a \l Store component. Creating an
    unlockable product will query its identifier from the platform's market place.
    If an unlockable product with the given identifier is available in the market
    place, the component will be updated with information collected from the
    market place.

    For example, the following example will register the product named "myUnlockableProduct"
    with the platform store and when the registration is successfully completed, it will
    show its price in a label:

    \qml
    Store {
        id: store
        UnlockableProduct {
            id: myUnlockableProduct
            identifier: "myUnlockableProduct"
        }
    }

    Text {
        id: label
        text: myUnlockableProduct.status === Product.Registered
              ? "The price of the unlockable product is " + myUnlockableProduct.price
              : "Unlockable product is pending registration"
    }
    \endqml

    An UnlockableProduct is a product which can only be purchased once per user.
*/

QInAppProductUnlockable::QInAppProductUnlockable(QObject *parent)
    : QInAppProductQmlType(QInAppProduct::Unlockable, parent)
{
}

QT_END_NAMESPACE
