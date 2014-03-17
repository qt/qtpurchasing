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

/*!
    \qmltype Product
    \inqmlmodule QtPurchasing
    \since QtPurchasing 1.0
    \ingroup qtpurchasing
    \brief Base of products for in-app purchasing.

    Product is the base of the different product types in the Qt Purchasing
    API. It cannot be created directly, but must be created via one of its
    subtypes.

    For products that should only be purchased once per user, use \l UnlockableProduct, and for
    products that can be purchased any number of times, use \l ConsumableProduct
*/

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

/*!
  \qmlproperty string QtPurchasing::Product::identifier
  This property holds the identifier of the product in the external market place. It must match the
  identifier used to register the product externally before-hand.

  When the identifier is set, the product is queried from the external market place, and its other
  properties are updated asynchronously.

  The following example queries an unlockable product named "myUnlockableProduct" from the external
  market place.
  \qml
  Store {
      UnlockableProduct {
        identifier: "myUnlockableProduct"

        // ...
      }
  }
*/
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
    if (m_store == 0)
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

/*!
  \qmlproperty enumeration QtPurchasing::Product::status
  This property holds the current status of the product in the registration sequence.

  \list
  \li Product.Uninitialized - This is initial status, before the identifier property has been set.
  \li Product.PendingRegistration - Indicates that the product is currently being queried from the
  external market place. The product gets this status when its identifier is set.
  \li Product.Registered - Indicates that the product was successfully found in the external market
  place. Its price can now be queried and the product can be purchased.
  \li Product.Unknown - The product could not be found in the external market place. This could
  for example be due to misspelling the product identifier.
  \endlist

  \qml
  Store {
      ConsumableProduct {
          identifier: "myConsumableProduct"
          onStatusChanged: {
              switch (status) {
              case Product.PendingRegistration: console.debug("Registering " + identifier); break
              case Product.Registered: console.debug(identifier + " registered with price " + price); break
              case Product.Unknown: console.debug(identifier + " was not found in the market place"); break
              }
          }

          // ...
      }
  }
  \endqml
*/
QInAppProductQmlType::Status QInAppProductQmlType::status() const
{
    return m_status;
}

/*!
  \qmlproperty string QtPurchasing::Product::price
  This property holds the price of the product once it has been successfully queried from the
  external market place. The price is a string consisting of both currency and value, and is
  usually localized to the current user.

  For example, the following example displays the price of the unlockable product named
  "myUnlockableProduct":
  \qml
  Store {
      UnlockableProduct {
          id: myUnlockableProduct
          identifier: "myUnlockableProduct"

          // ...
      }
  }

  Text {
      text: myUnlockableProduct.status === Product.Registered
            ? "Price is " + myUnlockableProduct.price
            : "Price unknown at the moment"
  }
  \endqml

  When run in a Norwegian locale, this code could for instance display "Price is kr 6,00" for a one-dollar product.
*/
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
    if (transaction->product()->identifier() != m_identifier)
        return;

    if (transaction->status() == QInAppTransaction::PurchaseApproved)
        emit purchaseSucceeded(transaction);
    else if (transaction->status() == QInAppTransaction::PurchaseRestored)
        emit purchaseRestored(transaction);
    else
        emit purchaseFailed(transaction);
}

/*!
  \qmlmethod QtPurchasing::Product::purchase()

  Launches the purchasing process for this product. The purchasing process is asynchronous.
  When it completes, either the \l onPurchaseSucceeded or the \l onPurchaseFailed handler
  in the object will be called with the resulting transaction.
*/
void QInAppProductQmlType::purchase()
{
    if (m_product != 0 && m_status == Registered)
        m_product->purchase();
    else
        qWarning("Attempted to purchase unregistered product");
}

/*!
  \qmlsignal QtPurchasing::Product::onPurchaseSucceeded(object transaction)

  This handler is called when a product has been purchased successfully. It is triggered
  when the application has called purchase() on the product and the user has subsequently
  confirmed the purchase, for example by entering their password.

  All products should have a handler for onPurchaseSucceeded. This handler should in turn
  save information about the purchased product and when the information has been stored
  and verified, it should call finalize() on the \a transaction object.

  The handler should support being called multiple times for the same purchase. For example,
  the application execution might by accident be interrupted after saving the purchase
  information, but before finalizing the transaction. In this case, the handler should
  verify that the information is already stored in the persistent storage and then finalize
  the transaction.

  The following example attempts to store the purchase state of a consumable
  product using a custom made function. It only finalizes the transaction if saving the
  data was successful. Otherwise, it calls another custom function to display an error
  message to the user.

  \qml
  Store {
      ConsumableProduct {
          id: myConsumableProduct
          identifier: "myConsumableProduct"

          onPurchaseSucceeded: {
              if (myStorage.savePurchaseInformation(identifier)) {
                  transaction.finalize()
              } else {
                  myDisplayHelper.message("Failed to store purchase information. Is there available storage?")
              }
          }

          // ...
      }
  }
  \endqml

  If the transaction is not finalized, the onPurchaseSucceeded handler will be called again
  the next time the product is registered (on application startup.) This means that if saving
  the information failed, the user will have the opportunity of rectifying the problem (for
  example by deleting something else to make space for the data) and the transaction will
  be completed once they restart the application and the problem has been solved.

  \note A purchased, consumable product can not be purchased again until its previous transaction
  is finalized.
*/

/*!
  \qmlsignal QtPurchasing::Product::onPurchaseFailed(object transaction)

  This handler is called when a purchase was requested for a given product, but the purchase
  failed. This will typically happen if the application calls purchase() on a product, and
  the user subsequently cancels the purchase. It could also happen under other circumstances,
  for example if there is no suitable network connection.

  All products should have an \c onPurchaseFailed handler.

  After a proper reaction is taken, the finalize() function should be called on the \a transaction
  object. If this is not done, the handler may be called again the next time the product is registered.

  The following example reacts to a failed purchase attempt by calling a custom function to display a
  message to the user.
  \qml
  Store {
      ConsumableProduct {
          id: myConsumableProduct
          identifier: "myConsumableProduct"

          onPurchaseFailed: {
              myDisplayHelper.message("Product was not purchased. You have not been charged.")
              transaction.finalize()
          }

          // ...
      }
  }
  \endqml
*/

/*!
  \qmlsignal QtPurchasing::Product::onPurchaseRestored(object transaction)

  This handler is called when a previously purchased unlockable product is restored. This
  can happen when the \l restorePurchases() function in the current \l Store is called.
  The \c onPurchaseRestored handler will then be called for each unlockable product which
  has previously been purchased by the user.

  Applications which uses the \l restorePurchases() function should include this handler
  in all unlockable products. In the handler, the application should make sure information
  about the purchase is stored and call \l finalize() on the \a transaction object if
  the information has been successfully stored (or has been verified to already be stored).

  The following example calls a custom function which either saves the information about
  the purchase or verifies that it is already saved. When the data has been verified, it
  finalizes the transaction. If it could not be verified, it calls another custom function
  to display an error message to the user. If the transaction is not finalized, the handler
  will be called again for the same transaction the next time the product is registered
  (on application start-up).

  \qml
  Store {
      UnlockableProduct {
          id: myUnlockableProduct
          identifier: "myUnlockableProduct"

          onPurchaseRestored: {
              if (myStorage.savePurchaseInformation(identifier)) {
                  transaction.finalize()
              } else {
                  myDisplayHelper.message("Failed to store purchase information. Is there available storage?")
              }
          }

          // ...
      }
  }
  \endqml
*/

QT_END_NAMESPACE
