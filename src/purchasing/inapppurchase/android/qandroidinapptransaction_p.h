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


#ifndef QANDROIDINAPPTRANSACTION_P_H
#define QANDROIDINAPPTRANSACTION_P_H

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

#include <QtCore/qobject.h>
#include "qinapptransaction.h"

QT_BEGIN_NAMESPACE

class QAndroidInAppTransaction : public QInAppTransaction
{
    Q_OBJECT
public:
    explicit QAndroidInAppTransaction(const QString &signature,
                                      const QString &data,
                                      const QString &purchaseToken,
                                      const QString &orderId,
                                      TransactionStatus status,
                                      QInAppProduct *product,
                                      QObject *parent = 0);

    void finalize();

    QString orderId() const;
    QString platformProperty(const QString &propertyName) const;

private:
    QString m_signature;
    QString m_data;
    QString m_purchaseToken;
    QString m_orderId;
};

QT_END_NAMESPACE

#endif // QANDROIDINAPPTRANSACTION_P_H
