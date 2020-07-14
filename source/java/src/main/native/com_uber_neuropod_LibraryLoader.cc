#include "com_uber_neuropod_LibraryLoader.h"

#include <jni.h>

JNIEXPORT jboolean JNICALL Java_com_uber_neuropod_LibraryLoader_nativeIsLoaded(JNIEnv *, jclass)
{
    return JNI_TRUE;
}
