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


#ifndef QANDROIDINAPPPURCHASEBACKEND_P_H
#define QANDROIDINAPPPURCHASEBACKEND_P_H

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

#include <QtCore/qmutex.h>
#include <QtCore/qset.h>
#include <QtAndroidExtras/qandroidjniobject.h>
#include <QtAndroidExtras/qandroidactivityresultreceiver.h>

QT_BEGIN_NAMESPACE

class QAndroidInAppProduct;
class QAndroidInAppPurchaseBackend : public QInAppPurchaseBackend, public QAndroidActivityResultReceiver
{
    Q_OBJECT
public:
    explicit QAndroidInAppPurchaseBackend(QObject *parent = 0);

    void initialize();
    bool isReady() const;

    void queryProduct(QInAppProduct::ProductType productType, const QString &identifier);
    void restorePurchases();

    void setPlatformProperty(const QString &propertyName, const QString &value);

    void purchaseProduct(QAndroidInAppProduct *product);

    // Callbacks from Java
    void registerQueryFailure(const QString &productId);
    void registerProduct(const QString &productId, const QString &price);
    void registerPurchased(const QString &productId, const QString &signature, const QString &data);
    void registerReady();

    void handleActivityResult(int requestCode, int resultCode, const QAndroidJniObject &data);

private:
    void checkFinalizationStatus(QInAppProduct *product);
    void createTransactionForProduct(QInAppTransaction::TransactionStatus status,
                                     QInAppProduct *product,
                                     const QPair<QString, QString> &signature);
    bool transactionFinalizedForProduct(QInAppProduct *product);

    void purchaseFailed(const QString &identifier);
    void purchaseSucceeded(const QString &identifier);

    mutable QMutex m_mutex;
    bool m_isReady;
    QAndroidJniObject m_javaObject;
    QHash<QString, QInAppProduct::ProductType> m_productTypeForPendingId;
    QHash<QString, QPair<QString, QString> > m_signatureAndDataForPurchase;
    QSet<QString> m_finalizedUnlockableProducts;
    QHash<QString, QInAppProduct *> m_pendingPurchaseForIdentifier;
};

QT_END_NAMESPACE

#endif // QANDROIDINAPPPURCHASEBACKEND_P_H
