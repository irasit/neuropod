#include "com_uber_neuropod_NeuropodValueMap.h"

#include "jclass_register.h"
#include "reference_manager.h"
#include "utils.h"

#include <neuropod/neuropod.hh>

#include <string>

#include <jni.h>

using namespace neuropod::jni;

JNIEXPORT void JNICALL Java_com_uber_neuropod_NeuropodValueMap_nativeDelete(JNIEnv *, jobject, jlong inputHandle)
{
    if (!ReferenceManager<neuropod::NeuropodValueMap>::contains(inputHandle))
    {
        return;
    }
    auto input = reinterpret_cast<neuropod::NeuropodValueMap *>(inputHandle);
    for (auto &entry : *input)
    {
        entry.second.reset();
    }
    ReferenceManager<neuropod::NeuropodValueMap>::remove(inputHandle);
}

JNIEXPORT jlong JNICALL Java_com_uber_neuropod_NeuropodValueMap_nativeNew(JNIEnv *, jclass)
{
    auto ret = std::make_shared<neuropod::NeuropodValueMap>();
    ReferenceManager<neuropod::NeuropodValueMap>::put(ret);
    return reinterpret_cast<long>(ret.get());
}

JNIEXPORT jlong JNICALL Java_com_uber_neuropod_NeuropodValueMap_nativeGetValue(JNIEnv *env,
                                                                          jclass,
                                                                          jstring key,
                                                                          jlong   nativeHandle)
{
    try
    {
        ReferenceManager<neuropod::NeuropodValueMap>::check(nativeHandle);
        auto neuropodValueMap = reinterpret_cast<neuropod::NeuropodValueMap *>(nativeHandle);
        auto ret              = (*neuropodValueMap)[neuropod::jni::toString(env, key)];
        ReferenceManager<neuropod::NeuropodValue>::put(ret);
        return reinterpret_cast<long>(ret.get());
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return reinterpret_cast<long>(nullptr);
}

JNIEXPORT void JNICALL Java_com_uber_neuropod_NeuropodValueMap_nativeAddEntry
        (JNIEnv * env, jclass, jstring key, jlong tensorHandle, jlong mapHandle) {
    try
    {
        ReferenceManager<neuropod::NeuropodValueMap>::check(mapHandle);
        auto neuropodValueMap = reinterpret_cast<neuropod::NeuropodValueMap *>(mapHandle);
        ReferenceManager<neuropod::NeuropodValue>::check(tensorHandle);
        (*neuropodValueMap)[toString(env, key)] = ReferenceManager<neuropod::NeuropodValue>::get(tensorHandle);
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }

}

JNIEXPORT jobject JNICALL Java_com_uber_neuropod_NeuropodValueMap_nativeGetKeyList(JNIEnv *env, jclass, jlong nativeHandle)
{
    try
    {
        ReferenceManager<neuropod::NeuropodValueMap>::check(nativeHandle);
        auto    neuropodValueMap = reinterpret_cast<neuropod::NeuropodValueMap *>(nativeHandle);
        jobject ret              = env->NewObject(java_util_ArrayList, java_util_ArrayList_, neuropodValueMap->size());
        for (const auto &entry : *neuropodValueMap)
        {
            jstring key = env->NewStringUTF(entry.first.c_str());
            env->CallBooleanMethod(ret, java_util_ArrayList_add, key);
            // Normally we do not need to delete local ref manually. But as Java jni only have limited number of local
            // ref support, in a long loop we need to do so.
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
