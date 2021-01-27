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

#ifndef QMACINAPPTRANSACTION_P_H
#define QMACINAPPTRANSACTION_P_H

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

#include "qinapptransaction.h"
#include <QtCore/QString>

@class SKPaymentTransaction;

QT_BEGIN_NAMESPACE

class QMacInAppPurchaseBackend;

class QMacInAppPurchaseTransaction : public QInAppTransaction
{
    Q_OBJECT
public:
    QMacInAppPurchaseTransaction(SKPaymentTransaction *transaction,
                                 const TransactionStatus status,
                                 QInAppProduct *product,
                                 QMacInAppPurchaseBackend *backend = 0);

    void finalize();
    QString orderId() const;
    FailureReason failureReason() const;
    QString errorString() const;
    QDateTime timestamp() const;

private:
    SKPaymentTransaction *m_nativeTransaction;
    QString m_errorString;
    FailureReason m_failureReason;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QMacInAppPurchaseTransaction*)

#endif // QMACINAPPTRANSACTION_P_H
