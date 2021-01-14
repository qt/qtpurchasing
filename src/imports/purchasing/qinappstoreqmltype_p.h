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

#ifndef QINAPPSTOREQMLTYPE_P_H
#define QINAPPSTOREQMLTYPE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

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
