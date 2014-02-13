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

#include "qandroidinapppurchasebackend_p.h"

#include <QtAndroidExtras/qandroidjniobject.h>
#include <jni.h>

QT_USE_NAMESPACE

static void queryFailed(jclass, jlong nativePointer, jstring productId)
{
    QAndroidInAppPurchaseBackend *backend = reinterpret_cast<QAndroidInAppPurchaseBackend *>(nativePointer);
    backend->registerQueryFailure(QAndroidJniObject(productId).toString());
}

static void purchasedProductsQueried(jclass, jlong nativePointer)
{
    QAndroidInAppPurchaseBackend *backend = reinterpret_cast<QAndroidInAppPurchaseBackend *>(nativePointer);
    backend->registerReady();
}

static void registerProduct(jclass, jlong nativePointer, jstring productId, jstring price)
{
    QAndroidInAppPurchaseBackend *backend = reinterpret_cast<QAndroidInAppPurchaseBackend *>(nativePointer);
    backend->registerProduct(QAndroidJniObject(productId).toString(),
                             QAndroidJniObject(price).toString());
}

static void registerPurchased(jclass, jlong nativePointer, jstring productId, jstring signature, jstring data)
{
    QAndroidInAppPurchaseBackend *backend = reinterpret_cast<QAndroidInAppPurchaseBackend *>(nativePointer);
    backend->registerPurchased(QAndroidJniObject(productId).toString(),
                               QAndroidJniObject(signature).toString(),
                               QAndroidJniObject(data).toString());

}

static JNINativeMethod methods[] = {
    {"queryFailed", "(JLjava/lang/String;)V", (void *)queryFailed},
    {"purchasedProductsQueried", "(J)V", (void *)purchasedProductsQueried},
    {"registerProduct", "(JLjava/lang/String;Ljava/lang/String;)V", (void *)registerProduct},
    {"registerPurchased", "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", (void *)registerPurchased}
};

jint JNICALL JNI_OnLoad(JavaVM *vm, void *)
{
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4) != JNI_OK)
        return JNI_FALSE;

    jclass clazz = env->FindClass("com/digia/qt5/android/mobileextras/QtInAppPurchase");
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0)
        return JNI_FALSE;

    return JNI_VERSION_1_4;
}
