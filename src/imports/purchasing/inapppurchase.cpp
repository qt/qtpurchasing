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

#include "qinappproductconsumable_p.h"
#include "qinappproductunlockable_p.h"
#include "qinappstoreqmltype_p.h"

#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>

#include <QtPurchasing/qinappproduct.h>
#include <QtPurchasing/qinapptransaction.h>

QT_BEGIN_NAMESPACE

class QInAppPurchaseModule : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtPurchasing"));

        qmlRegisterType<QInAppStoreQmlType>(uri,
                                            1, 0,
                                            "Store");
        qmlRegisterUncreatableType<QInAppProductQmlType>(uri,
                                                         1, 0,
                                                         "Product",
                                                         trUtf8("Products are created using types ConsumableProduct or UnlockableProduct"));
        qmlRegisterType<QInAppProductConsumable>(uri,
                                          1, 0,
                                          "ConsumableProduct");
        qmlRegisterType<QInAppProductUnlockable>(uri,
                                                 1, 0,
                                                 "UnlockableProduct");
        qmlRegisterUncreatableType<QInAppTransaction>(uri,
                                                      1, 0,
                                                      "Transaction",
                                                      trUtf8("Transaction is provided by InAppStore"));
    }

    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(uri);
        Q_UNUSED(engine);
    }
};

QT_END_NAMESPACE

#include "inapppurchase.moc"



