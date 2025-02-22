/*
 * Copyright 2023 Google LLC
 * SPDX-License-Identifier: MIT
 */

#include "ANativeWindowAndroid.h"
#include "util/detect_os.h"

#if DETECT_OS_ANDROID
#include <android/native_window.h>
#include <system/window.h>
#endif  // DETECT_OS_ANDROID

namespace gfxstream {

bool ANativeWindowHelperAndroid::isValid(EGLNativeWindowType window) {
#if DETECT_OS_ANDROID
    auto* anw = reinterpret_cast<ANativeWindow*>(window);
    return anw->common.magic == ANDROID_NATIVE_WINDOW_MAGIC;
#else
    (void)window;
    return false;
#endif  // DETECT_OS_ANDROID
}

bool ANativeWindowHelperAndroid::isValid(EGLClientBuffer buffer) {
#if DETECT_OS_ANDROID
    auto* anwb = reinterpret_cast<ANativeWindowBuffer*>(buffer);
    if (anwb->common.magic != ANDROID_NATIVE_BUFFER_MAGIC) {
        return false;
    }
    if (anwb->common.version != sizeof(android_native_buffer_t)) {
        return false;
    }
    if (anwb->handle == nullptr) {
        return false;
    }
    return true;
#else
    (void)buffer;
    return false;
#endif  // DETECT_OS_ANDROID
}

void ANativeWindowHelperAndroid::acquire(EGLNativeWindowType window) {
#if DETECT_OS_ANDROID
    auto* anw = reinterpret_cast<ANativeWindow*>(window);
    ANativeWindow_acquire(anw);
#else
    (void)window;
#endif  // DETECT_OS_ANDROID
}

void ANativeWindowHelperAndroid::release(EGLNativeWindowType window) {
#if DETECT_OS_ANDROID
    auto* anw = reinterpret_cast<ANativeWindow*>(window);
    ANativeWindow_release(anw);
#else
    (void)window;
#endif  // DETECT_OS_ANDROID
}

void ANativeWindowHelperAndroid::acquire(EGLClientBuffer buffer) {
#if DETECT_OS_ANDROID
    auto* anwb = reinterpret_cast<ANativeWindowBuffer*>(buffer);
    anwb->incStrong(anwb);
#else
    (void)buffer;
#endif  // DETECT_OS_ANDROID
}

void ANativeWindowHelperAndroid::release(EGLClientBuffer buffer) {
#if DETECT_OS_ANDROID
    auto* anwb = reinterpret_cast<ANativeWindowBuffer*>(buffer);
    anwb->decStrong(anwb);
#else
    (void)buffer;
#endif  // DETECT_OS_ANDROID
}

int ANativeWindowHelperAndroid::getConsumerUsage(EGLNativeWindowType window, int* usage) {
#if DETECT_OS_ANDROID
    auto* anw = reinterpret_cast<ANativeWindow*>(window);
    return anw->query(anw, NATIVE_WINDOW_CONSUMER_USAGE_BITS, usage);
#else
    (void)window;
    (void)usage;
    return -1;
#endif  // DETECT_OS_ANDROID
}

void ANativeWindowHelperAndroid::setUsage(EGLNativeWindowType window, int usage) {
#if DETECT_OS_ANDROID
    auto* anw = reinterpret_cast<ANativeWindow*>(window);
    ANativeWindow_setUsage(anw, usage);
#else
    (void)window;
    (void)usage;
#endif  // DETECT_OS_ANDROID
}

int ANativeWindowHelperAndroid::getWidth(EGLNativeWindowType window) {
#if DETECT_OS_ANDROID
    auto* anw = reinterpret_cast<ANativeWindow*>(window);
    return ANativeWindow_getWidth(anw);
#else
    (void)window;
    return -1;
#endif  // DETECT_OS_ANDROID
}

int ANativeWindowHelperAndroid::getHeight(EGLNativeWindowType window) {
#if DETECT_OS_ANDROID
    auto* anw = reinterpret_cast<ANativeWindow*>(window);
    return ANativeWindow_getHeight(anw);
#else
    (void)window;
    return -1;
#endif  // DETECT_OS_ANDROID
}

int ANativeWindowHelperAndroid::getWidth(EGLClientBuffer buffer) {
#if DETECT_OS_ANDROID
    auto* anwb = reinterpret_cast<ANativeWindowBuffer*>(buffer);
    return anwb->width;
#else
    (void)buffer;
    return -1;
#endif  // DETECT_OS_ANDROID
}

int ANativeWindowHelperAndroid::getHeight(EGLClientBuffer buffer) {
#if DETECT_OS_ANDROID
    auto* anwb = reinterpret_cast<ANativeWindowBuffer*>(buffer);
    return anwb->height;
#else
    (void)buffer;
    return -1;
#endif  // DETECT_OS_ANDROID
}

int ANativeWindowHelperAndroid::getFormat(EGLClientBuffer buffer, Gralloc* gralloc) {
#if DETECT_OS_ANDROID
    auto* anb = reinterpret_cast<ANativeWindowBuffer*>(buffer);
    return gralloc->getFormat(anb->handle);
#else
    (void)buffer;
    (void)gralloc;
    return -1;
#endif  // DETECT_OS_ANDROID
}

void ANativeWindowHelperAndroid::setSwapInterval(EGLNativeWindowType window, int interval) {
#if DETECT_OS_ANDROID
    auto* anw = reinterpret_cast<ANativeWindow*>(window);
    anw->setSwapInterval(anw, interval);
#else
    (void)window;
    (void)interval;
#endif  // DETECT_OS_ANDROID
}

int ANativeWindowHelperAndroid::queueBuffer(EGLNativeWindowType window, EGLClientBuffer buffer,
                                            int fence) {
#if DETECT_OS_ANDROID
    auto* anw = reinterpret_cast<ANativeWindow*>(window);
    auto* anb = reinterpret_cast<ANativeWindowBuffer*>(buffer);
    return ANativeWindow_queueBuffer(anw, anb, fence);
#else
    (void)window;
    (void)buffer;
    (void)fence;
    return -1;
#endif  // DETECT_OS_ANDROID
}

int ANativeWindowHelperAndroid::dequeueBuffer(EGLNativeWindowType window, EGLClientBuffer* buffer,
                                              int* fence) {
#if DETECT_OS_ANDROID
    auto* anw = reinterpret_cast<ANativeWindow*>(window);
    auto* anb = reinterpret_cast<ANativeWindowBuffer**>(buffer);
    return ANativeWindow_dequeueBuffer(anw, anb, fence);
#else
    (void)window;
    (void)buffer;
    (void)fence;
    return -1;
#endif  // DETECT_OS_ANDROID
}

int ANativeWindowHelperAndroid::cancelBuffer(EGLNativeWindowType window, EGLClientBuffer buffer) {
#if DETECT_OS_ANDROID
    auto* anw = reinterpret_cast<ANativeWindow*>(window);
    auto* anb = reinterpret_cast<ANativeWindowBuffer*>(buffer);
    return ANativeWindow_cancelBuffer(anw, anb, -1);
#else
    (void)window;
    (void)buffer;
    return -1;
#endif  // DETECT_OS_ANDROID
}

int ANativeWindowHelperAndroid::getHostHandle(EGLClientBuffer buffer, Gralloc* gralloc) {
#if DETECT_OS_ANDROID
    auto* anb = reinterpret_cast<ANativeWindowBuffer*>(buffer);
    return gralloc->getHostHandle(anb->handle);
#else
    (void)buffer;
    (void)gralloc;
    return -1;
#endif  // DETECT_OS_ANDROID
}

ANativeWindowHelper* createPlatformANativeWindowHelper() {
    return new ANativeWindowHelperAndroid();
}

}  // namespace gfxstream
