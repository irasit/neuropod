#include "com_uber_neuropod_Neuropod.h"

#include "jclass_register.h"
#include "reference_manager.h"
#include "utils.h"

#include <neuropod/neuropod.hh>

#include <exception>
#include <memory>
#include <sstream>
#include <string>

#include <jni.h>

using namespace neuropod::jni;

JNIEXPORT jlong JNICALL Java_com_uber_neuropod_Neuropod_nativeNew__Ljava_lang_String_2(JNIEnv *env, jclass, jstring path)
{
    neuropod::RuntimeOptions opts;
    opts.use_ope = true;
    try
    {
        auto convertedPath = toString(env, path);
        auto ret           = std::make_shared<neuropod::Neuropod>(convertedPath, opts);
        ReferenceManager<neuropod::Neuropod>::put(ret);
        return reinterpret_cast<jlong>(ret.get());
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return reinterpret_cast<jlong>(nullptr);
}

JNIEXPORT jlong JNICALL Java_com_uber_neuropod_Neuropod_nativeNew__Ljava_lang_String_2J(JNIEnv *env,
                                                                                   jclass,
                                                                                   jstring path,
                                                                                   jlong   optHandle)
{
    auto opts = reinterpret_cast<neuropod::RuntimeOptions *>(optHandle);
    try
    {
        auto convertedPath = toString(env, path);
        auto ret           = std::make_shared<neuropod::Neuropod>(convertedPath, *opts);
        ReferenceManager<neuropod::Neuropod>::put(ret);
        return reinterpret_cast<jlong>(ret.get());
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return reinterpret_cast<jlong>(nullptr);
}

JNIEXPORT jlong JNICALL Java_com_uber_neuropod_Neuropod_nativeInfer(JNIEnv *env,
                                                               jclass,
                                                               jlong inputHandle,
                                                               jlong modelHandle)
{

    try
    {
        ReferenceManager<neuropod::Neuropod>::check(modelHandle);
        auto model = reinterpret_cast<neuropod::Neuropod *>(modelHandle);
        ReferenceManager<neuropod::NeuropodValueMap>::check(inputHandle);
        auto input = reinterpret_cast<neuropod::NeuropodValueMap *>(inputHandle);

        std::shared_ptr<neuropod::NeuropodValueMap> ret = model->infer(*input);
        ReferenceManager<neuropod::NeuropodValueMap>::put(ret);
        return reinterpret_cast<jlong>(ret.get());
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return reinterpret_cast<jlong>(nullptr);
}

JNIEXPORT void JNICALL Java_com_uber_neuropod_Neuropod_nativeDelete(JNIEnv *, jobject obj, jlong handle)
{
    ReferenceManager<neuropod::Neuropod>::remove(handle);
}

JNIEXPORT jobject JNICALL Java_com_uber_neuropod_Neuropod_nativeGetInputFeatureKeys(JNIEnv *env, jclass, jlong handle)
{
    try
    {
        auto    model     = reinterpret_cast<neuropod::Neuropod *>(handle);
        auto    inputSpec = model->get_inputs();
        jobject ret       = env->NewObject(java_util_ArrayList, java_util_ArrayList_, inputSpec.size());
        for (const auto &tensorSpec : inputSpec)
        {
            jstring key = env->NewStringUTF(tensorSpec.name.c_str());
            env->CallBooleanMethod(ret, java_util_ArrayList_add, key);
            env->DeleteLocalRef(key);
        }
        return ret;
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return nullptr;
}

JNIEXPORT jobject JNICALL Java_com_uber_neuropod_Neuropod_nativeGetInputFeatureDataTypes(JNIEnv *env, jclass, jlong handle)
{
    try
    {
        ReferenceManager<neuropod::Neuropod>::check(handle);
        auto    model     = reinterpret_cast<neuropod::Neuropod *>(handle);
        auto    inputSpec = model->get_inputs();
        jobject ret       = env->NewObject(java_util_ArrayList, java_util_ArrayList_, inputSpec.size());
        for (const auto &tensorSpec : inputSpec)
        {

            env->CallBooleanMethod(
                ret,
                java_util_ArrayList_add,
                getFieldObject(env, com_uber_neuropod_TensorType, tensorTypeToString(tensorSpec.type).c_str()));
        }
        return ret;
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return nullptr;
}

JNIEXPORT jobject JNICALL Java_com_uber_neuropod_Neuropod_nativeGetInputs(JNIEnv *env, jclass, jlong handle)
{
    try
    {
        ReferenceManager<neuropod::Neuropod>::check(handle);
        auto    model     = reinterpret_cast<neuropod::Neuropod *>(handle);
        auto    inputSpec = model->get_inputs();
        return toJavaTensorSpecList(env, inputSpec);
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return nullptr;
}

JNIEXPORT jobject JNICALL Java_com_uber_neuropod_Neuropod_nativeGetOutputs(JNIEnv *env, jclass, jlong handle)
{
    try
    {
        ReferenceManager<neuropod::Neuropod>::check(handle);
        auto    model     = reinterpret_cast<neuropod::Neuropod *>(handle);
        auto    outputs = model->get_outputs();
        return toJavaTensorSpecList(env, outputs);
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return nullptr;
}

JNIEXPORT void JNICALL Java_com_uber_neuropod_Neuropod_nativeLoadModel(JNIEnv * env, jclass, jlong handle)
{
    try
    {
        ReferenceManager<neuropod::Neuropod>::check(handle);
        auto    model     = reinterpret_cast<neuropod::Neuropod *>(handle);
        model->load_model();
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
}

JNIEXPORT jstring JNICALL Java_com_uber_neuropod_Neuropod_nativeGetName(JNIEnv * env, jclass, jlong handle)
{
    try
    {
        ReferenceManager<neuropod::Neuropod>::check(handle);
        auto    model     = reinterpret_cast<neuropod::Neuropod *>(handle);
        return env->NewStringUTF(model->get_name().c_str());
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return nullptr;
}

JNIEXPORT jstring JNICALL Java_com_uber_neuropod_Neuropod_nativeGetPlatform(JNIEnv * env, jclass, jlong handle)
{
    try
    {
        ReferenceManager<neuropod::Neuropod>::check(handle);
        auto    model     = reinterpret_cast<neuropod::Neuropod *>(handle);
        return env->NewStringUTF(model->get_platform().c_str());
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return nullptr;
}
