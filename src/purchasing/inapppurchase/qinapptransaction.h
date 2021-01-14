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

#ifndef QINAPPTRANSACTION_H
#define QINAPPTRANSACTION_H

#include <QtCore/qobject.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qdatetime.h>
#include <QtPurchasing/qtpurchasingglobal.h>

QT_BEGIN_NAMESPACE

class QInAppProduct;
class QInAppTransactionPrivate;
class Q_PURCHASING_EXPORT QInAppTransaction: public QObject
{
    Q_OBJECT
    Q_ENUMS(TransactionStatus FailureReason)
    Q_PROPERTY(TransactionStatus status READ status CONSTANT)
    Q_PROPERTY(QInAppProduct* product READ product CONSTANT)
    Q_PROPERTY(QString orderId READ orderId CONSTANT)
    Q_PROPERTY(FailureReason failureReason READ failureReason CONSTANT)
    Q_PROPERTY(QString errorString READ errorString CONSTANT)
    Q_PROPERTY(QDateTime timestamp READ timestamp CONSTANT)
public:
    enum TransactionStatus {
        Unknown,
        PurchaseApproved,
        PurchaseFailed,
        PurchaseRestored
    };

    enum FailureReason {
        NoFailure,
        CanceledByUser,
        ErrorOccurred
    };

    ~QInAppTransaction();

    QInAppProduct *product() const;

    virtual QString orderId() const;
    virtual FailureReason failureReason() const;
    virtual QString errorString() const;
    virtual QDateTime timestamp() const;

    Q_INVOKABLE virtual void finalize() = 0;
    Q_INVOKABLE virtual QString platformProperty(const QString &propertyName) const;

    TransactionStatus status() const;

protected:
    explicit QInAppTransaction(TransactionStatus status,
                               QInAppProduct *product,
                               QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(QInAppTransaction)
    QSharedPointer<QInAppTransactionPrivate> d;
};

QT_END_NAMESPACE

#endif // QINAPPTRANSACTION_H
