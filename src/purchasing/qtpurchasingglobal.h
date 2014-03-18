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

#ifndef QTPURCHASINGGLOBAL_H
#define QTPURCHASINGGLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_PURCHASING_LIB)
#    define Q_PURCHASING_EXPORT Q_DECL_EXPORT
#  else
#    define Q_PURCHASING_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_PURCHASING_EXPORT
#endif

QT_END_NAMESPACE

#endif // QTPURCHASINGGLOBAL_H
