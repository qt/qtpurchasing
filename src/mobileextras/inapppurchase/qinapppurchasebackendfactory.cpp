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

#include "qinapppurchasebackendfactory_p.h"

#if defined(Q_OS_ANDROID)
#  include "qandroidinapppurchasebackend_p.h"
#elif defined(Q_OS_IOS)
#  include "qiosinapppurchasebackend_p.h"
#else
#  include "qinapppurchasebackend_p.h"
#endif

QT_BEGIN_NAMESPACE

QInAppPurchaseBackend *QInAppPurchaseBackendFactory::create()
{
#if defined(Q_OS_ANDROID)
    return new QAndroidInAppPurchaseBackend;
#elif defined (Q_OS_IOS)
    return new QIosInAppPurchaseBackend;
#else
    return new QInAppPurchaseBackend;
#endif
}

QT_END_NAMESPACE
