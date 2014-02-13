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

#ifndef QINAPPPRODUCTUNLOCKABLE_P_H
#define QINAPPPRODUCTUNLOCKABLE_P_H

#include "qinappproductqmltype_p.h"

class QInAppProductUnlockable : public QInAppProductQmlType
{
    Q_OBJECT
public:
    explicit QInAppProductUnlockable(QObject *parent = 0);
};

#endif // QINAPPPRODUCTUNLOCKABLE_P_H
