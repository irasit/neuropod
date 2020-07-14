package com.uber.neuropod;

import java.io.Serializable;
import java.nio.*;
import java.util.List;

/**
 * This class is an one to one mapping to cpp NeuropodValue type.
 * Should not call the close function if this class does not have the ownership of the underlying native class.
 */
public class NeuropodValue extends NativeClass implements Serializable {

    /**
     * Instantiates a new Neuropod value map from existing cpp handle.
     * The native object the nativeHandle points to must have been created by the neuropod jni before, otherwise
     * it will be treated as a wild pointer and a NeuropodJNIException will be thrown in the java side if any calls
     * invoking native functions happens.
     *
     * @param nativeHandle the native handle
     */
    protected NeuropodValue(long nativeHandle) {
        super(nativeHandle);
    }

    /**
     * Gets dims.
     *
     * @return the dims
     */
    public long[] getDims() {
        return nativeGetDims(super.getNativeHandle());
    }

    public Long getNumberOfElements() {
        return nativeGetNumberOfElements(super.getNativeHandle());
    }

    static NeuropodValue create(Object data,
                                long[] shape,
                                Neuropod model) {
        if (data instanceof IntBuffer) {
            return create((IntBuffer) data, shape, model);
        } else if (data instanceof LongBuffer) {
            return create((LongBuffer) data, shape, model);

        } else if (data instanceof DoubleBuffer) {
            return create((DoubleBuffer) data, shape, model);

        } else if (data instanceof FloatBuffer) {
            return create((FloatBuffer) data, shape, model);

        } else if (data instanceof List<?>) {
            // For String tensors only
            List<?> curData = (List<?>) data;
            if (!curData.isEmpty()) {
                Object obj = curData.get(0);
                if (!(obj instanceof String)) {
                    throw new NeuropodJNIException("Neuropod Exception: Unsupported input type!");
                }
            }

            return create((List<String>) data, shape, model);
        }
        throw new NeuropodJNIException("Neuropod Exception: Unsupported input type!");
    }

    /**
     * Allocate a neuropod tensor and copy java data to it based on the allocator of certain model.
     *
     * @param data  the data
     * @param shape the shape
     * @param model the neuropod model to allcoate the NeuropodValue in the cpp side
     * @return the allocated neuropod tensor
     */
    static public NeuropodValue create(IntBuffer data,
                                       long[] shape,
                                       Neuropod model) {
        NeuropodValue ret = allocate(shape, TensorType.INT32_TENSOR, model);
        ret.getBuffer().asIntBuffer().put(data);
        return ret;
    }

    /**
     * Allocate a neuropod tensor and copy java data to it based on the allocator of certain model.
     *
     * @param data  the data
     * @param shape the shape
     * @param model the neuropod model to allcoate the NeuropodValue in the cpp side
     * @return the allocated neuropod tensor
     */
    static public NeuropodValue create(LongBuffer data,
                                       long[] shape,
                                       Neuropod model) {
        NeuropodValue ret = allocate(shape, TensorType.INT64_TENSOR, model);
        ret.getBuffer().asLongBuffer().put(data);
        return ret;
    }

    /**
     * Allocate a neuropod tensor and copy java data to it based on the allocator of certain model.
     *
     * @param data  the data
     * @param shape the shape
     * @param model the neuropod model to allcoate the NeuropodValue in the cpp side
     * @return the allocated neuropod tensor
     */
    static public NeuropodValue create(FloatBuffer data,
                                       long[] shape,
                                       Neuropod model) {
        NeuropodValue ret = allocate(shape, TensorType.FLOAT_TENSOR, model);
        ret.getBuffer().asFloatBuffer().put(data);
        return ret;
    }

    /**
     * Allocate a neuropod tensor and copy java data to it based on the allocator of certain model.
     *
     * @param data  the data
     * @param shape the shape
     * @param model the neuropod model to allcoate the NeuropodValue in the cpp side
     * @return the allocated neuropod tensor
     */
    static public NeuropodValue create(DoubleBuffer data,
                                       long[] shape,
                                       Neuropod model) {

        NeuropodValue ret = allocate(shape, TensorType.DOUBLE_TENSOR, model);
        ret.getBuffer().asDoubleBuffer().put(data);
        return ret;
    }

    /**
     * Allocate a neuropod tensor and copy java data to it based on the allocator of certain model.
     *
     * @param data  the data
     * @param shape the shape
     * @param model the neuropod model to allcoate the NeuropodValue in the cpp side
     * @return the allocated neuropod tensor
     */
    static public NeuropodValue create(List<String> data,
                                       long[] shape,
                                       Neuropod model) {
        long retHandle = nativeCreateStringTensor(data, shape, checkHandle(model));
        return new NeuropodValue(retHandle);
    }

    /**
     * Allocate a neuropod tensor and copy java data to it based on default allocator.
     *
     * @param data  the data
     * @param shape the shape
     * @return the allocated neuropod tensor
     */
    static public NeuropodValue create(IntBuffer data,
                                       long[] shape) {
        return create(data, shape, null);
    }

    /**
     * Allocate a neuropod tensor and copy java data to it based on default allocator.
     *
     * @param data  the data
     * @param shape the shape
     * @return the allocated neuropod tensor
     */
    static public NeuropodValue create(FloatBuffer data,
                                       long[] shape) {
        return create(data, shape, null);
    }

    /**
     * Allocate a neuropod tensor and copy java data to it based on default allocator.
     *
     * @param data  the data
     * @param shape the shape
     * @return the allocated neuropod tensor
     */
    static public NeuropodValue create(DoubleBuffer data,
                                       long[] shape) {
        return create(data, shape, null);
    }

    /**
     * Allocate a neuropod tensor and copy java data to it based on default allocator.
     *
     * @param data  the data
     * @param shape the shape
     * @return the allocated neuropod tensor
     */
    static public NeuropodValue create(LongBuffer data,
                                       long[] shape) {
        return create(data, shape, null);
    }

    /**
     * Allocate a neuropod tensor and copy java data to it based on default allocator.
     *
     * @param data  the data
     * @param shape the shape
     * @return the allocated neuropod tensor
     */
    static public NeuropodValue create(List<String> data,
                                       long[] shape) {
        return create(data, shape, null);
    }

    /**
     * Gets tensor type.
     *
     * @return the tensor type
     */
    public TensorType getTensorType() {
        return nativeGetTensorType(super.getNativeHandle());
    }

    static private long checkHandle(Neuropod model) {
        long handle = 0;
        if (model != null) {
            handle = model.getNativeHandle();
        }
        return handle;

    }

    private ByteBuffer getBuffer() {
        return nativeGetBuffer(getNativeHandle()).order(ByteOrder.nativeOrder());
    }

    static private NeuropodValue allocate(long[] shape, TensorType type, Neuropod model) {
        return new NeuropodValue(nativeAllocate(shape, type.getValue(), model.getNativeHandle()));
    }

    /**
     * Copy all the data of the NeuropodValue and flatten it to a java object
     * The java type of the jave object is based on the tensor type.
     * It is [Type]Buffer for non string tensors, List[String] for string tensors.
     *
     * @return the list
     */
    public Object toList() {
        switch (this.getTensorType()) {
            case INT32_TENSOR : return toIntList();
            case INT64_TENSOR : return toLongList();
            case FLOAT_TENSOR : return toFloatList();
            case DOUBLE_TENSOR : return toDoubleList();
            case STRING_TENSOR : return toStringList();
            default : throw new NeuropodJNIException("Unsupported tensor type" + this.getTensorType());
        }
    }

    /**
     * Copy all the data of the NeuropodValue and flatten it to an int buffer.
     *
     * @return the int buffer
     */
    public IntBuffer toIntList() {
        TensorType type = getTensorType();
        if (type != TensorType.INT32_TENSOR) {
            throw new NeuropodJNIException("Only a int tensor can return a int list!");
        }
        return IntBuffer.allocate(this.getNumberOfElements().intValue()).put(this.getBuffer().asIntBuffer());
    }

    /**
     * Copy all the data of the NeuropodValue and flatten it to a long buffer.
     *
     * @return the long buffer
     */
    public LongBuffer toLongList() {
        TensorType type = getTensorType();
        if (type != TensorType.INT64_TENSOR) {
            throw new NeuropodJNIException("Only a long tensor can return a long list!");
        }
        return LongBuffer.allocate(this.getNumberOfElements().intValue()).put(this.getBuffer().asLongBuffer());
    }

    /**
     * Copy all the data of the NeuropodValue and flatten it to a float buffer.
     *
     * @return the float buffer
     */
    public FloatBuffer toFloatList() {
        TensorType type = getTensorType();
        if (type != TensorType.FLOAT_TENSOR) {
            throw new NeuropodJNIException("Only a float tensor can return a float list!");
        }
        return FloatBuffer.allocate(this.getNumberOfElements().intValue()).put(this.getBuffer().asFloatBuffer());
    }

    /**
     * Copy all the data of the NeuropodValue and flatten it to a double buffer.
     *
     * @return the double buffer
     */
    public DoubleBuffer toDoubleList() {
        TensorType type = getTensorType();
        if (type != TensorType.DOUBLE_TENSOR) {
            throw new NeuropodJNIException("Only a double tensor can return a double list!");
        }
        return DoubleBuffer.allocate(this.getNumberOfElements().intValue()).put(this.getBuffer().asDoubleBuffer());
    }

    @java.lang.Override
    public java.lang.String toString() {
        Object list = toList();
        TensorType type = getTensorType();
        StringBuilder values = new StringBuilder();
        StringBuilder dims = new StringBuilder();
        long[] dimsArray = getDims();
        dims.append("[");
        for (long l : dimsArray) {
            dims.append(l);
            dims.append(",");
        }
        dims.append("]");
        switch (type) {
            case FLOAT_TENSOR: {
                float[] array = ((FloatBuffer) list).array();
                values.append("[");
                for (float v : array) {
                    values.append(v);
                    values.append(",");
                }
                values.append("]");
                break;
            }
            case DOUBLE_TENSOR: {
                double[] array = ((DoubleBuffer) list).array();
                values.append("[");
                for (double v : array) {
                    values.append(v);
                    values.append(",");
                }
                values.append("]");
                break;
            }
            case INT32_TENSOR: {
                int[] array = ((IntBuffer) list).array();
                values.append("[");
                for (int v : array) {
                    values.append(v);
                    values.append(",");
                }
                values.append("]");
                break;
            }
            case INT64_TENSOR: {
                long[] array = ((LongBuffer) list).array();
                values.append("[");
                for (long v : array) {
                    values.append(v);
                    values.append(",");
                }
                values.append("]");
                break;
            }
            case STRING_TENSOR:
                values.append(list);
            default:
                return "NeuropodValue{unsupported tensor type}";
        }
        return String.format("NeuropodValue{value: %s, dims: %s}", values.toString(), dims.toString());
    }

    /**
     * Copy all the data of the NeuropodValue and flatten it to a list of string.
     *
     * @return the list
     */
    public List<String> toStringList() {
        TensorType type = getTensorType();
        if (type != TensorType.DOUBLE_TENSOR) {
            throw new NeuropodJNIException("Only a double tensor can return a double list!");
        }
        return (List<String>) nativeToStringList(super.getNativeHandle());
    }

    static native private ByteBuffer nativeGetBuffer(long nativeHandle);

    static native private Object nativeToStringList(long handle) throws NeuropodJNIException;

    static native private long nativeCreateStringTensor(List<String> data, long[] dims, long modelHandle) throws NeuropodJNIException;

    static native private long nativeAllocate(long[] dims, int typeNumber, long modelHandle) throws NeuropodJNIException;

    static native private long[] nativeGetDims(long nativeHandle) throws NeuropodJNIException;

    static native private TensorType nativeGetTensorType(long nativeHandle) throws NeuropodJNIException;

    static native private long nativeGetNumberOfElements(long nativeHandle) throws NeuropodJNIException;

    @Override
    protected native void nativeDelete(long handle);
}
