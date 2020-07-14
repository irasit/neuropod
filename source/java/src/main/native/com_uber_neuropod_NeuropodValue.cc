#include "com_uber_neuropod_NeuropodValue.h"

#include "jclass_register.h"
#include "reference_manager.h"
#include "utils.h"

#include <neuropod/core/generic_tensor.hh>
#include <neuropod/neuropod.hh>

#include <string>

#include <jni.h>

using namespace neuropod::jni;

const int FLOAT  = 0;
const int DOUBLE = 1;
const int INT    = 2;
const int LONG   = 3;
const int STRING = 5;

JNIEXPORT jobject JNICALL Java_com_uber_neuropod_NeuropodValue_nativeGetBuffer(JNIEnv *env, jclass, jlong nativeHandle)
{
    try
    {
        ReferenceManager<neuropod::NeuropodValue>::check(nativeHandle);
        auto neuropodTensor = reinterpret_cast<neuropod::NeuropodValue *>(nativeHandle)->as_tensor();
        auto tensorType     = neuropodTensor->get_tensor_type();
        switch (tensorType)
        {
        case neuropod::FLOAT_TENSOR: {
            return createDirectBuffer<float>(env, neuropodTensor);
        }
        case neuropod::DOUBLE_TENSOR: {
            return createDirectBuffer<double>(env, neuropodTensor);
        }
        case neuropod::INT32_TENSOR: {
            return createDirectBuffer<int32_t>(env, neuropodTensor);
        }
        case neuropod::INT64_TENSOR: {
            return createDirectBuffer<int64_t>(env, neuropodTensor);
        }
        default:
            throw std::runtime_error(std::string("Unsupported tensor type:") + tensorTypeToString(tensorType));
        }
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return nullptr;
}

JNIEXPORT jobject JNICALL Java_com_uber_neuropod_NeuropodValue_nativeToStringList(JNIEnv *env, jclass, jlong nativeHandle)
{
    try
    {
        ReferenceManager<neuropod::NeuropodValue>::check(nativeHandle);
        auto neuropodTensor = reinterpret_cast<neuropod::NeuropodValue *>(nativeHandle)->as_tensor();
        auto size           = neuropodTensor->get_num_elements();
        auto tensorType     = neuropodTensor->get_tensor_type();

        switch (tensorType)
        {
        case neuropod::STRING_TENSOR: {
            // This function copy data twice
            jobject ret         = env->NewObject(java_util_ArrayList, java_util_ArrayList_, size);
            auto    typedTensor = neuropodTensor->as_typed_tensor<std::string>();
            auto    elementList = typedTensor->get_data_as_vector();
            for (auto const &ele : elementList)
            {
                jstring convertedEle = env->NewStringUTF(ele.c_str());
                env->CallBooleanMethod(ret, java_util_ArrayList_add, convertedEle);
                env->DeleteLocalRef(convertedEle);
            }
            return ret;
        }
        default:
            throw std::runtime_error(std::string("Unsupported tensor type:") + tensorTypeToString(tensorType));
        }
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return nullptr;
}

JNIEXPORT jlong JNICALL Java_com_uber_neuropod_NeuropodValue_nativeAllocate(
    JNIEnv *env, jclass, jlongArray dims, jint typeNumber, jlong modelHandle)
{
    try
    {
        std::shared_ptr<neuropod::NeuropodTensorAllocator> allocator;
        if (modelHandle != 0) // Use the allocator defined by the provide model

        {
            ReferenceManager<neuropod::Neuropod>::check(modelHandle);
            auto model = reinterpret_cast<neuropod::Neuropod *>(modelHandle);
            allocator  = model->get_tensor_allocator();
        }
        else // No model provided, use default allocator instead
        {
            allocator = neuropod::get_generic_tensor_allocator();
        }

        // Prepare shape
        jsize                size = env->GetArrayLength(dims);
        jlong *              arr  = env->GetLongArrayElements(dims, 0);
        std::vector<int64_t> shapes(arr, arr + size);
        env->ReleaseLongArrayElements(dims, arr, JNI_ABORT);

        switch (typeNumber)
        {
        case INT: {
            auto tensor = allocator->allocate_tensor<int32_t>(shapes);
            ReferenceManager<neuropod::NeuropodValue>::put(tensor);
            return reinterpret_cast<jlong>(tensor.get());
        }
        case LONG: {
            auto tensor = allocator->allocate_tensor<int64_t>(shapes);
            ReferenceManager<neuropod::NeuropodValue>::put(tensor);
            return reinterpret_cast<jlong>(tensor.get());
        }
        case FLOAT: {
            auto tensor = allocator->allocate_tensor<float>(shapes);
            ReferenceManager<neuropod::NeuropodValue>::put(tensor);
            return reinterpret_cast<jlong>(tensor.get());
        }
        case DOUBLE: {
            auto tensor = allocator->allocate_tensor<double>(shapes);
            ReferenceManager<neuropod::NeuropodValue>::put(tensor);
            return reinterpret_cast<jlong>(tensor.get());
        }
        default:
            throw std::runtime_error("Unsupported tensor type!");
        }
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return reinterpret_cast<jlong>(nullptr);
}

JNIEXPORT jlong JNICALL
Java_com_uber_neuropod_NeuropodValue_nativeCreateStringTensor(JNIEnv *env, jclass, jobject data, jlongArray dims, jlong modelHandle)
{
    try
    {
        std::shared_ptr<neuropod::NeuropodTensorAllocator> allocator;
        // Prepare allocator
        if (modelHandle != 0)
        {
            ReferenceManager<neuropod::Neuropod>::check(modelHandle);
            auto model = reinterpret_cast<neuropod::Neuropod *>(modelHandle);
            allocator  = model->get_tensor_allocator();
        }
        else
        {
            allocator = neuropod::get_generic_tensor_allocator();
        }

        // Prepare shape

        jsize                shapeSize = env->GetArrayLength(dims);
        jlong *              arr  = env->GetLongArrayElements(dims, 0);
        std::vector<int64_t> shapes(arr, arr + shapeSize);
        env->ReleaseLongArrayElements(dims, arr, JNI_ABORT);

        // This function copy data twice
        jlong                    size = env->CallIntMethod(data, java_util_ArrayList_size);
        std::vector<std::string> intermediate;
        for (jlong i = 0; i < size; i++)
        {
            jstring element = static_cast<jstring>(env->CallObjectMethod(data, java_util_ArrayList_get, i));
            intermediate.emplace_back(toString(env, element));
            env->DeleteLocalRef(element);
        }
        auto tensor = allocator->allocate_tensor<std::string>(shapes);
        tensor->copy_from(intermediate);
        ReferenceManager<neuropod::NeuropodValue>::put(tensor);
        return reinterpret_cast<jlong>(tensor.get());
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return reinterpret_cast<jlong>(nullptr);
}

JNIEXPORT jlongArray JNICALL Java_com_uber_neuropod_NeuropodValue_nativeGetDims(JNIEnv *env, jclass, jlong handle)
{
    try
    {
        ReferenceManager<neuropod::NeuropodValue>::check(handle);
        auto       tensor = reinterpret_cast<neuropod::NeuropodValue *>(handle);
        auto       dims   = tensor->as_tensor()->get_dims();
        jlongArray result = env->NewLongArray(dims.size());
        if (result == NULL)
        {
            throw std::runtime_error("Out of memory!");
        }
        env->SetLongArrayRegion(result, 0, dims.size(), reinterpret_cast<jlong *>(dims.data()));
        return result;
    }

    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return nullptr;
}

JNIEXPORT jlong JNICALL Java_com_uber_neuropod_NeuropodValue_nativeGetNumberOfElements(JNIEnv *env,
                                                                                       jclass,
                                                                                       jlong modelHandle)
{
    try
    {
        ReferenceManager<neuropod::NeuropodValue>::check(modelHandle);
        auto tensor = reinterpret_cast<neuropod::NeuropodValue *>(modelHandle);
        return tensor->as_tensor()->get_num_elements();
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return 0;
}

JNIEXPORT jobject JNICALL Java_com_uber_neuropod_NeuropodValue_nativeGetTensorType(JNIEnv *env, jclass, jlong handle)
{
    try
    {
        ReferenceManager<neuropod::NeuropodValue>::check(handle);
        auto tensor = reinterpret_cast<neuropod::NeuropodValue *>(handle);
        auto type   = tensor->as_tensor()->get_tensor_type();
        return getFieldObject(env, com_uber_neuropod_TensorType, tensorTypeToString(type).c_str());
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, e.what());
    }
    return nullptr;
}

JNIEXPORT void JNICALL Java_com_uber_neuropod_NeuropodValue_nativeDelete(JNIEnv *, jobject, jlong nativeHandle)
{
    ReferenceManager<neuropod::NeuropodValue>::remove(nativeHandle);
}
