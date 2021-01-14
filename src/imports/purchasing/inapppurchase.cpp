/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Purchasing module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
****************************************************************************/

#include "qinappstoreqmltype_p.h"

#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>

#include <QtPurchasing/qinappproduct.h>
#include <QtPurchasing/qinapptransaction.h>

QT_BEGIN_NAMESPACE

class QInAppPurchaseModule : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    QInAppPurchaseModule(QObject *parent = 0) : QQmlExtensionPlugin(parent) { }
    void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtPurchasing"));

        qmlRegisterType<QInAppStoreQmlType>(uri,
                                            1, 0,
                                            "Store");
        qmlRegisterType<QInAppProductQmlType>(uri,
                                              1, 0,
                                              "Product");
        qmlRegisterUncreatableType<QInAppTransaction>(uri,
                                                      1, 0,
                                                      "Transaction",
                                                      tr("Transaction is provided by InAppStore"));

        // Register the latest Qt version as QML type version
        qmlRegisterModule(uri, 1, QT_VERSION_MINOR);
    }

    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(uri);
        Q_UNUSED(engine);
    }
};

QT_END_NAMESPACE

#include "inapppurchase.moc"



