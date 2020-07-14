#include "jclass_register.h"

#include "reference_manager.h"
#include "utils.h"

#include <exception>
#include <iostream>

#include <jni.h>

using namespace neuropod::jni;

jclass    java_util_Map;
jmethodID java_util_Map_get;

jclass    java_util_ArrayList;
jmethodID java_util_ArrayList_;
jmethodID java_util_ArrayList_add;
jmethodID java_util_ArrayList_get;
jmethodID java_util_ArrayList_size;

jclass    java_lang_Float;
jmethodID java_lang_Float_valueOf;
jmethodID java_lang_Float_floatValue;

jclass    java_lang_Double;
jmethodID java_lang_Double_valueOf;
jmethodID java_lang_Double_doubleValue;

jclass    java_lang_Integer;
jmethodID java_lang_Integer_valueOf;
jmethodID java_lang_Integer_intValue;

jclass    java_lang_Long;
jmethodID java_lang_Long_valueOf;
jmethodID java_lang_Long_longValue;

jclass java_nio_IntBuffer;
jmethodID java_nio_IntBuffer_allocate;
jmethodID java_nio_IntBuffer_array;

jclass java_nio_LongBuffer;
jmethodID java_nio_LongBuffer_allocate;
jmethodID java_nio_LongBuffer_array;

jclass java_nio_FloatBuffer;
jmethodID java_nio_FloatBuffer_allocate;
jmethodID java_nio_FloatBuffer_array;

jclass java_nio_DoubleBuffer;
jmethodID java_nio_DoubleBuffer_allocate;
jmethodID java_nio_DoubleBuffer_array;

jclass com_uber_neuropod_TensorSpec;
jmethodID com_uber_neuropod_TensorSpec_;

jclass com_uber_neuropod_Dimension;
jmethodID com_uber_neuropod_Dimension_value_;
jmethodID com_uber_neuropod_Dimension_symbol_;

jclass com_uber_neuropod_NeuropodJNIException;

jclass com_uber_neuropod_TensorType;

jint JNI_VERSION = JNI_VERSION_1_8;

// This function is called when the JNI is loaded.
jint JNI_OnLoad(JavaVM *vm, void *reserved)
{

    // Obtain the JNIEnv from the VM and confirm JNI_VERSION
    JNIEnv *env;
    std::cout << "Reading" << std::endl;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION) != JNI_OK)
    {
        return JNI_ERR;
    }

    // Move this exception class out of try catch block to avoid unexpected error when throw a java exception and the
    // exception type is wrong
    com_uber_neuropod_NeuropodJNIException =
        static_cast<jclass>(env->NewGlobalRef(findClass(env, "com/uber/neuropod/NeuropodJNIException")));
    try
    {
        java_util_Map     = static_cast<jclass>(env->NewGlobalRef(findClass(env, "java/util/Map")));
        java_util_Map_get = getMethodID(env, java_util_Map, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");

        java_util_ArrayList        = static_cast<jclass>(env->NewGlobalRef(findClass(env, "java/util/ArrayList")));
        java_util_ArrayList_       = getMethodID(env, java_util_ArrayList, "<init>", "(I)V");
        java_util_ArrayList_add    = getMethodID(env, java_util_ArrayList, "add", "(Ljava/lang/Object;)Z");
        java_util_ArrayList_get    = getMethodID(env, java_util_ArrayList, "get", "(I)Ljava/lang/Object;");
        java_util_ArrayList_size   = getMethodID(env, java_util_ArrayList, "size", "()I");
        java_lang_Float            = static_cast<jclass>(env->NewGlobalRef(findClass(env, "java/lang/Float")));
        java_lang_Float_valueOf    = getStaticMethodID(env, java_lang_Float, "valueOf", "(F)Ljava/lang/Float;");
        java_lang_Float_floatValue = getMethodID(env, java_lang_Float, "floatValue", "()F");

        java_lang_Double             = static_cast<jclass>(env->NewGlobalRef(findClass(env, "java/lang/Double")));
        java_lang_Double_valueOf     = getStaticMethodID(env, java_lang_Double, "valueOf", "(D)Ljava/lang/Double;");
        java_lang_Double_doubleValue = getMethodID(env, java_lang_Double, "doubleValue", "()D");

        java_lang_Integer          = static_cast<jclass>(env->NewGlobalRef(findClass(env, "java/lang/Integer")));
        java_lang_Integer_valueOf  = getStaticMethodID(env, java_lang_Integer, "valueOf", "(I)Ljava/lang/Integer;");
        java_lang_Integer_intValue = getMethodID(env, java_lang_Integer, "intValue", "()I");

        java_lang_Long           = static_cast<jclass>(env->NewGlobalRef(findClass(env, "java/lang/Long")));
        java_lang_Long_valueOf   = getStaticMethodID(env, java_lang_Long, "valueOf", "(J)Ljava/lang/Long;");
        java_lang_Long_longValue = getMethodID(env, java_lang_Long, "longValue", "()J");

        java_nio_IntBuffer = static_cast<jclass>(env->NewGlobalRef(findClass(env, "java/nio/IntBuffer")));
        java_nio_IntBuffer_array = getMethodID(env, java_nio_IntBuffer, "array", "()Ljava/lang/Object;");
        java_nio_IntBuffer_allocate = getStaticMethodID(env,java_nio_IntBuffer, "allocate","(I)Ljava/nio/IntBuffer;");

        java_nio_FloatBuffer = static_cast<jclass>(env->NewGlobalRef(findClass(env, "java/nio/FloatBuffer")));
        java_nio_FloatBuffer_array = getMethodID(env, java_nio_FloatBuffer, "array", "()Ljava/lang/Object;");
        java_nio_FloatBuffer_allocate = getStaticMethodID(env,java_nio_FloatBuffer, "allocate","(I)Ljava/nio/FloatBuffer;");

        java_nio_LongBuffer = static_cast<jclass>(env->NewGlobalRef(findClass(env, "java/nio/LongBuffer")));
        java_nio_LongBuffer_array = getMethodID(env, java_nio_LongBuffer, "array", "()Ljava/lang/Object;");
        java_nio_LongBuffer_allocate = getStaticMethodID(env,java_nio_LongBuffer, "allocate","(I)Ljava/nio/LongBuffer;");

        java_nio_DoubleBuffer = static_cast<jclass>(env->NewGlobalRef(findClass(env, "java/nio/DoubleBuffer")));
        java_nio_DoubleBuffer_array = getMethodID(env, java_nio_DoubleBuffer, "array", "()Ljava/lang/Object;");
        java_nio_DoubleBuffer_allocate = getStaticMethodID(env, java_nio_DoubleBuffer, "allocate", "(I)Ljava/nio/DoubleBuffer;");

        com_uber_neuropod_TensorSpec = static_cast<jclass>(env->NewGlobalRef(findClass(env, "com/uber/neuropod/TensorSpec")));
        com_uber_neuropod_TensorSpec_ = getMethodID(env, com_uber_neuropod_TensorSpec, "<init>", "(Ljava/lang/String;Lcom/uber/neuropod/TensorType;Ljava/util/List;)V");

        com_uber_neuropod_Dimension = static_cast<jclass>(env->NewGlobalRef(findClass(env, "com/uber/neuropod/Dimension")));;
        com_uber_neuropod_Dimension_value_ = getMethodID(env, com_uber_neuropod_Dimension, "<init>", "(J)V");
        com_uber_neuropod_Dimension_symbol_ = getMethodID(env, com_uber_neuropod_Dimension, "<init>", "(Ljava/lang/String;)V");

        com_uber_neuropod_TensorType = static_cast<jclass>(env->NewGlobalRef(findClass(env, "com/uber/neuropod/TensorType")));
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    // Return the JNI Version as required by method
    return JNI_VERSION;
}

// This function is called when the JNI is unloaded.
void JNI_OnUnload(JavaVM *vm, void *reserved)
{

    // Obtain the JNIEnv from the VM
    JNIEnv *env;
    vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION);
    // Destroy the global references
    env->DeleteGlobalRef(java_util_Map);
    env->DeleteGlobalRef(java_util_ArrayList);
    env->DeleteGlobalRef(java_lang_Float);
    env->DeleteGlobalRef(java_lang_Double);
    env->DeleteGlobalRef(java_lang_Integer);
    env->DeleteGlobalRef(java_lang_Long);

    env->DeleteGlobalRef(java_nio_IntBuffer);
    env->DeleteGlobalRef(java_nio_FloatBuffer);
    env->DeleteGlobalRef(java_nio_LongBuffer);
    env->DeleteGlobalRef(java_nio_DoubleBuffer);


    env->DeleteGlobalRef(com_uber_neuropod_NeuropodJNIException);
    env->DeleteGlobalRef(com_uber_neuropod_TensorType);
    neuropod::jni::ReferenceManager<neuropod::Neuropod>::reset();
    neuropod::jni::ReferenceManager<neuropod::NeuropodValue>::reset();
    neuropod::jni::ReferenceManager<neuropod::NeuropodValueMap>::reset();
}
