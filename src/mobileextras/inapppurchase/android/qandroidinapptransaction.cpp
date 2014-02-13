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

#include "qandroidinapptransaction_p.h"

QT_BEGIN_NAMESPACE

QAndroidInAppTransaction::QAndroidInAppTransaction(const QString &signature,
                                                   const QString &data,
                                                   TransactionStatus status,
                                                   QInAppProduct *product,
                                                   QObject *parent)
    : QInAppTransaction(status, product, parent)
    , m_signature(signature)
    , m_data(data)
{
}

QString QAndroidInAppTransaction::platformProperty(const QString &propertyName) const
{
    if (propertyName.compare(QStringLiteral("AndroidSignature"), Qt::CaseInsensitive) == 0)
        return m_signature;
    else if (propertyName.compare(QStringLiteral("AndroidPurchaseData"), Qt::CaseInsensitive) == 0)
        return m_data;
    else
        return QInAppTransaction::platformProperty(propertyName);
}

void QAndroidInAppTransaction::finalize()
{
    // ### consume consumable or store finalized data for unlockable
#warning Unimplemented
}

QT_END_NAMESPACE
