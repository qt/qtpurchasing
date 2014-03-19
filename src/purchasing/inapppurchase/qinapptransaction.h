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

#ifndef QINAPPTRANSACTION_H
#define QINAPPTRANSACTION_H

#include <QtCore/qobject.h>
#include <QtCore/qsharedpointer.h>
#include <QtPurchasing/qtpurchasingglobal.h>

QT_BEGIN_NAMESPACE

class QInAppProduct;
class QInAppTransactionPrivate;
class Q_PURCHASING_EXPORT QInAppTransaction: public QObject
{
    Q_OBJECT
    Q_ENUMS(TransactionStatus)
    Q_PROPERTY(TransactionStatus status READ status CONSTANT)
    Q_PROPERTY(QInAppProduct * product READ product CONSTANT)
    Q_PROPERTY(QString orderId READ orderId CONSTANT)
public:
    enum TransactionStatus {
        Unknown,
        PurchaseApproved,
        PurchaseFailed,
        PurchaseRestored
    };

    ~QInAppTransaction();

    QInAppProduct *product() const;

    virtual QString orderId() const;

    Q_INVOKABLE virtual void finalize() = 0;
    Q_INVOKABLE virtual QString platformProperty(const QString &propertyName) const;

    TransactionStatus status() const;

protected:
    explicit QInAppTransaction(TransactionStatus status, QInAppProduct *product, QObject *parent = 0);

private:
    Q_DISABLE_COPY(QInAppTransaction)
    QSharedPointer<QInAppTransactionPrivate> d;
};

QT_END_NAMESPACE

#endif // QINAPPTRANSACTION_H
