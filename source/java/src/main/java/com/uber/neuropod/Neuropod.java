package com.uber.neuropod;

import java.nio.*;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

/**
 * This class is an one to one mapping to cpp NeuropodValue type. Need to manually call close
 * Method after using this class
 */
public class Neuropod extends NativeClass {

    /**
     * Wrap a native handle.
     *
     * @param nativeHandle the native handle
     */
    protected Neuropod(long nativeHandle) {
        super(nativeHandle);
    }

    /**
     * Load a model from the file path and use default options
     *
     * @param neuropodPath the neuropod path
     */
    public Neuropod(String neuropodPath) {
        super.setNativeHandle(nativeNew(neuropodPath));
    }

    /**
     * Load a model from the file path and use provided options
     *
     * @param neuropodPath the neuropod path
     * @param options      the options
     * @throws Exception the exception
     */
    public Neuropod(String neuropodPath, RuntimeOptions options) throws Exception {
        RuntimeOptions.RuntimeOptionsNative nativeOptions = options.toNative();
        super.setNativeHandle(nativeNew(neuropodPath, nativeOptions.getNativeHandle()));
        nativeOptions.close();
    }


    /**
     * Perform the inference calculation on the input data
     *
     * @param inputs the input data
     * @param shape  the shape of the input data
     * @return the inference result
     * @throws Exception the exception
     */
    public NeuropodValueMap infer(Map<String, Object> inputs, Map<String, long[]> shape) throws Exception {
        checkInputSpec(inputs);
        NeuropodValueMap valueMap = new NeuropodValueMap();
        for (String key:inputs.keySet()) {
            long[] curDim = shape.get(key);
            NeuropodValue value = NeuropodValue.create(inputs.get(key), shape.get(key), this);
            valueMap.addEntry(key, value);
            value.close();
        }
        NeuropodValueMap ret = infer(valueMap);
        valueMap.close();
        return ret;
    }

    /**
     * Gets name.
     *
     * @return the name
     */
    public String getName() {
        return nativeGetName(super.getNativeHandle());
    }

    /**
     * Gets platform.
     *
     * @return the platform
     */
    public String getPlatform() {
        return nativeGetPlatform(super.getNativeHandle());
    }

    /**
     * Perform the inference calculation on the input data
     *
     * @param inputs the inputs
     * @return the inference result
     */
    public synchronized NeuropodValueMap infer(NeuropodValueMap inputs) {
        return new NeuropodValueMap(nativeInfer(inputs.getNativeHandle(), super.getNativeHandle()));
    }

    /**
     * Gets input tensor specs
     *
     * @return the inputs
     */
    public List<TensorSpec> getInputs() {
        return nativeGetInputs(super.getNativeHandle());
    }

    /**
     * Gets outpus tnesor specs
     *
     * @return the outpus
     */
    public List<TensorSpec> getOutpus() {
        return nativeGetOutputs(super.getNativeHandle());
    }

    /**
     * Load model.
     */
    public void loadModel() {
        nativeLoadModel(super.getNativeHandle());
    }

    // Check whether the input tensor names are the same with the model's requirement
    private void checkInputSpec(Map<String, Object> inputs) throws NeuropodJNIException {
        List<String> inputFeatureKeyList = nativeGetInputFeatureKeys(super.getNativeHandle());
        List<TensorType> inputFeatureTensorTypes = nativeGetInputFeatureDataTypes(super.getNativeHandle());
        Iterator<String> keyIt = inputFeatureKeyList.iterator();
        Iterator<TensorType> typeIt = inputFeatureTensorTypes.iterator();
        while (keyIt.hasNext() && typeIt.hasNext()) {
            String currentKey = keyIt.next();
            if (!inputs.containsKey(currentKey)) {
                throw new NeuropodJNIException(String.format("Neuropod Error: Tensor name(s) '{%s}' are not found in the input spec", currentKey));
            }
            TensorType type = typeIt.next();
            verifyType(inputs.get(currentKey), type, currentKey);
        }
    }

    // Check whether the input tensor has the same data type with the model's reuqirement
    private void verifyType(Object obj, TensorType type, String tensorKey) {
        switch (type) {
            case FLOAT_TENSOR:
                if (!(obj instanceof FloatBuffer)) {
                    System.out.println(obj.getClass().getSimpleName());
                    throw new NeuropodJNIException(String.format("Neuropod Error: Tensor name(s) '{%s}' should have a float buffer as its input data", tensorKey));
                }
                break;
            case DOUBLE_TENSOR:
                if (!(obj instanceof DoubleBuffer)) {
                    throw new NeuropodJNIException(String.format("Neuropod Error: Tensor name(s) '{%s}' should have a double buffer as its input data", tensorKey));
                }
                break;
            case INT32_TENSOR:
                if (!(obj instanceof IntBuffer)) {
                    throw new NeuropodJNIException(String.format("Neuropod Error: Tensor name(s) '{%s}' should have a int32 buffer as its input data", tensorKey));
                }
                break;
            case INT64_TENSOR:
                if (!(obj instanceof LongBuffer)) {
                    throw new NeuropodJNIException(String.format("Neuropod Error: Tensor name(s) '{%s}' should have a int64 buffer as its input data", tensorKey));
                }
                break;
            case STRING_TENSOR:
                if (!(obj instanceof ByteBuffer)) {
                    throw new NeuropodJNIException(String.format("Neuropod Error: Tensor name(s) '{%s}' should have a byte buffer as its input data", tensorKey));
                }
                break;
            default:
                throw new NeuropodJNIException(String.format("Neuropod Error: Tensor name(s) '{%s}' has a unsupported data type", tensorKey));
        }
    }

    static private native List<String> nativeGetInputFeatureKeys(long handle);

    static private native List<TensorType> nativeGetInputFeatureDataTypes(long handle);

    static private native long nativeNew(String filePath);

    static private native long nativeNew(String filePath, long optionHandle);

    static private native long nativeInfer(long inputHanlde, long modelHandle);

    static private native List<TensorSpec> nativeGetInputs(long modelHandle);

    static private native List<TensorSpec> nativeGetOutputs(long modelHandle);

    static private native void nativeLoadModel(long modelHandle);

    static private native String nativeGetName(long modelHandle);

    static private native String nativeGetPlatform(long modelHandle);

    /**
     * Native delete.
     *
     * @param handle the handle
     */
    @Override
    protected native void nativeDelete(long handle);
}
