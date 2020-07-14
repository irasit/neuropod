#include "utils.h"

#include "jclass_register.h"

#include <neuropod/neuropod.hh>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <jni.h>

namespace neuropod
{
namespace jni
{

const std::string TENSOR_TYPE = "Lcom/uber/neuropod/TensorType;";

std::string toString(JNIEnv *env, jstring target)
{
    const char *raw = env->GetStringUTFChars(target, NULL);
    std::string res(raw);
    env->ReleaseStringUTFChars(target, raw);
    return res;
}

std::vector<int64_t> getDefaultInputDim(const TensorSpec &tensorSpec)
{
    std::vector<int64_t> ret;
    for (auto &dim : tensorSpec.dims)
    {
        if (dim.value == -1 || dim.value == -2)
        {
            ret.push_back(1);
        }
        else
        {
            ret.push_back(dim.value);
        }
    }
    return ret;
}

jclass findClass(JNIEnv *env, const char *name)
{
    jclass ret = env->FindClass(name);
    if (reinterpret_cast<jlong>(ret) == 0)
    {
        throw std::runtime_error(std::string("Class not found: ") + name);
    }
    return ret;
}

std::string getJclassName(JNIEnv *env, jclass clazz)
{
    // TODO: get class name from jclass by weijiad
    return "";
}

jmethodID getMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig)
{
    jmethodID ret = env->GetMethodID(clazz, name, sig);
    if (reinterpret_cast<jlong>(ret) == 0)
    {
        throw std::runtime_error(std::string("Method ID not found: ") + getJclassName(env, clazz) + name + sig);
    }
    return ret;
}

jmethodID getStaticMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig)
{
    jmethodID ret = env->GetStaticMethodID(clazz, name, sig);
    if (reinterpret_cast<jlong>(ret) == 0)
    {
        throw std::runtime_error(std::string("Method ID not found: ") + getJclassName(env, clazz) + name + sig);
    }
    return ret;
}

jobject getFieldObject(JNIEnv *env, jclass dataTypes, std::string fieldName)
{
    jfieldID field = env->GetStaticFieldID(dataTypes, fieldName.c_str(), TENSOR_TYPE.c_str());
    if (reinterpret_cast<jlong>(field) == 0)
    {
        throw std::runtime_error(std::string("Field not found: ") + getJclassName(env, dataTypes) + fieldName);
    }
    return env->GetStaticObjectField(dataTypes, field);
}

std::string tensorTypeToString(TensorType type)
{
    std::string       typeString;
    std::stringstream ss;
    ss << type;
    ss >> typeString;
    return typeString;
}

void throwJavaException(JNIEnv *env, const char* message)
{
    env->ThrowNew(com_uber_neuropod_NeuropodJNIException, message);
}

jobject toJavaTensorSpecList(JNIEnv *env, const std::vector<TensorSpec> &specs) {
    jobject ret       = env->NewObject(java_util_ArrayList, java_util_ArrayList_, specs.size());
    for (const auto &tensorSpec : specs)
    {
        auto type = getFieldObject(env, com_uber_neuropod_TensorType, tensorTypeToString(tensorSpec.type).c_str());
        jstring name = env->NewStringUTF(tensorSpec.name.c_str());
        jobject dims       = env->NewObject(java_util_ArrayList, java_util_ArrayList_, tensorSpec.dims.size());
        for (const auto& dim:tensorSpec.dims) {
            // Dim is symbol
            if (dim.value == -2) {
                jstring symbol = env->NewStringUTF(dim.symbol.c_str());
                jobject javaDim       = env->NewObject(com_uber_neuropod_Dimension, com_uber_neuropod_Dimension_symbol_, symbol);
                env->CallBooleanMethod(
                        dims,
                        java_util_ArrayList_add,
                        javaDim);
                env->DeleteLocalRef(javaDim);
                env->DeleteLocalRef(symbol);
            } else {
                jobject javaDim       = env->NewObject(com_uber_neuropod_Dimension, com_uber_neuropod_Dimension_value_, dim.value);
                env->CallBooleanMethod(
                        dims,
                        java_util_ArrayList_add,
                        javaDim);
                env->DeleteLocalRef(javaDim);
            }
        }
        jobject javaTensorSpec       = env->NewObject(com_uber_neuropod_TensorSpec, com_uber_neuropod_TensorSpec_, name, type, dims);
        env->CallBooleanMethod(ret, java_util_ArrayList_add, javaTensorSpec);
        env->DeleteLocalRef(name);
        env->DeleteLocalRef(dims);
        env->DeleteLocalRef(type);

    }
    return ret;
}

} // namespace jni
} // namespace neuropod
