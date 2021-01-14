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
#include <QtCore/qdatetime.h>
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

    void queryProducts(const QList<Product> &products);
    void queryProduct(QInAppProduct::ProductType productType, const QString &identifier);
    void restorePurchases();

    void setPlatformProperty(const QString &propertyName, const QString &value);

    void purchaseProduct(QAndroidInAppProduct *product);

    void consumeTransaction(const QString &purchaseToken);
    void registerFinalizedUnlockable(const QString &identifier);

    // Callbacks from Java
    Q_INVOKABLE void registerQueryFailure(const QString &productId);
    Q_INVOKABLE void registerProduct(const QString &productId,
                                     const QString &price,
                                     const QString &title,
                                     const QString &description);
    Q_INVOKABLE void registerPurchased(const QString &identifier,
                                       const QString &signature,
                                       const QString &data,
                                       const QString &purchaseToken,
                                       const QString &orderId,
                                       const QDateTime &timestamp);
    Q_INVOKABLE void purchaseSucceeded(int requestCode,
                                       const QString &signature,
                                       const QString &data,
                                       const QString &purchaseToken,
                                       const QString &orderId,
                                       const QDateTime &timestamp);
    Q_INVOKABLE void purchaseFailed(int requestCode,
                                    int failureReason,
                                    const QString &errorString);
    Q_INVOKABLE void registerReady();

    void handleActivityResult(int requestCode, int resultCode, const QAndroidJniObject &data);

    QString finalizedUnlockableFileName() const;

private:
    void checkFinalizationStatus(QInAppProduct *product,
                                 QInAppTransaction::TransactionStatus status = QInAppTransaction::PurchaseApproved);
    bool transactionFinalizedForProduct(QInAppProduct *product);
    void purchaseFailed(QInAppProduct *product,
                        int failureReason,
                        const QString &errorString);

    struct PurchaseInfo
    {
        PurchaseInfo(const QString &signature_, const QString &data_, const QString &purchaseToken_, const QString &orderId_, const QDateTime &timestamp_)
            : signature(signature_)
            , data(data_)
            , purchaseToken(purchaseToken_)
            , orderId(orderId_)
            , timestamp(timestamp_)
        {
        }

        QString signature;
        QString data;
        QString purchaseToken;
        QString orderId;
        QDateTime timestamp;
    };

    mutable QRecursiveMutex m_mutex;
    bool m_isReady;
    QAndroidJniObject m_javaObject;
    QHash<QString, QInAppProduct::ProductType> m_productTypeForPendingId;
    QHash<QString, PurchaseInfo> m_infoForPurchase;
    QSet<QString> m_finalizedUnlockableProducts;
    QHash<int, QInAppProduct *> m_activePurchaseRequests;
};

QT_END_NAMESPACE

#endif // QANDROIDINAPPPURCHASEBACKEND_P_H
