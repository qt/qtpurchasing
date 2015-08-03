/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Purchasing module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QIOSINAPPPURCHASEBACKEND_P_H
#define QIOSINAPPPURCHASEBACKEND_P_H

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

#include "qinapppurchasebackend_p.h"
#include "qinappproduct.h"
#include "qinapptransaction.h"

#include <QtCore/QHash>

QT_BEGIN_NAMESPACE

class QIosInAppPurchaseProduct;
class QIosInAppPurchaseTransaction;

class QIosInAppPurchaseBackend : public QInAppPurchaseBackend
{
    Q_OBJECT
public:
    QIosInAppPurchaseBackend(QObject *parent = 0);
    ~QIosInAppPurchaseBackend();

    void initialize();
    bool isReady() const;
    void queryProduct(QInAppProduct::ProductType productType, const QString &identifier);
    void restorePurchases();
    void setPlatformProperty(const QString &propertyName, const QString &value);

    //Called by InAppPurchaseManager
    Q_INVOKABLE void registerProduct(QIosInAppPurchaseProduct *product);
    Q_INVOKABLE void registerQueryFailure(const QString &productId);
    Q_INVOKABLE void registerTransaction(QIosInAppPurchaseTransaction *transaction);
    QInAppProduct::ProductType productTypeForProductId(const QString &productId);
    QIosInAppPurchaseProduct *registeredProductForProductId(const QString &productId);

private:
    void *m_iapManager;
    QHash<QString, QInAppProduct::ProductType> m_productTypeForPendingId;
    QHash<QString, QIosInAppPurchaseProduct*> m_registeredProductForId;
};

#endif // QIOSINAPPPURCHASEBACKEND_P_H

QT_END_NAMESPACE
