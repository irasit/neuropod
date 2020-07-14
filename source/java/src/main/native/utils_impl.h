namespace neuropod
{
namespace jni
{

template <typename T>
jobject createDirectBuffer(JNIEnv *env, NeuropodTensor *tensor)
{
    auto         typedTensor = tensor->as_typed_tensor<T>();
    auto         rawPtr      = typedTensor->get_raw_data_ptr();
    const size_t memSize     = sizeof(T) * tensor->get_num_elements();
    return env->NewDirectByteBuffer(rawPtr, static_cast<jlong>(memSize));
}

}
}
