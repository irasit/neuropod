/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_uber_neuropod_Neuropod */

#ifndef _Included_com_uber_neuropod_Neuropod
#define _Included_com_uber_neuropod_Neuropod
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_uber_neuropod_Neuropod
 * Method:    nativeGetInputFeatureKeys
 * Signature: (J)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_com_uber_neuropod_Neuropod_nativeGetInputFeatureKeys
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_uber_neuropod_Neuropod
 * Method:    nativeGetInputFeatureDataTypes
 * Signature: (J)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_com_uber_neuropod_Neuropod_nativeGetInputFeatureDataTypes
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_uber_neuropod_Neuropod
 * Method:    nativeNew
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_uber_neuropod_Neuropod_nativeNew__Ljava_lang_String_2
  (JNIEnv *, jclass, jstring);

/*
 * Class:     com_uber_neuropod_Neuropod
 * Method:    nativeNew
 * Signature: (Ljava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_uber_neuropod_Neuropod_nativeNew__Ljava_lang_String_2J
  (JNIEnv *, jclass, jstring, jlong);

/*
 * Class:     com_uber_neuropod_Neuropod
 * Method:    nativeInfer
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_uber_neuropod_Neuropod_nativeInfer
  (JNIEnv *, jclass, jlong, jlong);

/*
 * Class:     com_uber_neuropod_Neuropod
 * Method:    nativeGetInputs
 * Signature: (J)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_com_uber_neuropod_Neuropod_nativeGetInputs
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_uber_neuropod_Neuropod
 * Method:    nativeGetOutputs
 * Signature: (J)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_com_uber_neuropod_Neuropod_nativeGetOutputs
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_uber_neuropod_Neuropod
 * Method:    nativeLoadModel
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_uber_neuropod_Neuropod_nativeLoadModel
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_uber_neuropod_Neuropod
 * Method:    nativeGetName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_uber_neuropod_Neuropod_nativeGetName
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_uber_neuropod_Neuropod
 * Method:    nativeGetPlatform
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_uber_neuropod_Neuropod_nativeGetPlatform
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_uber_neuropod_Neuropod
 * Method:    nativeDelete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_uber_neuropod_Neuropod_nativeDelete
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif