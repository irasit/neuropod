package com.uber.neuropod;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * This is a base class for all class with a binding to native class.
 * Need to call close() method after usage to free memory in C++ side.
 */
abstract class NativeClass implements AutoCloseable {
    // Load native library
    static {
        LibraryLoader.load();
    }

    private static Map<Long, AtomicInteger> counter = new ConcurrentHashMap<Long, AtomicInteger>();

    // The pointer to the native object
    private Long nativeHandle;

    /**
     * Instantiates a new Native class.
     */
    public NativeClass() {
    }

    /**
     * Instantiates a new Native class.
     *
     * @param handle the native handle
     */
    protected NativeClass(long handle) {
        if (counter.containsKey(handle)) {
            counter.get(handle).incrementAndGet();
        } else {
            counter.put(handle, new AtomicInteger(1));
        }
        this.nativeHandle = handle;
    }

    /**
     * Gets native handle.
     *
     * @return the native handle
     */
    protected long getNativeHandle() {
        if (nativeHandle == null) {
            throw new NeuropodJNIException("Object is not allocated");
        }
        return nativeHandle;
    }

    /**
     * Sets native handle.
     *
     * @param handle the handle
     */
    protected void setNativeHandle(long handle) {
        decreaseThisCount();
        increaseCount(handle);
        this.nativeHandle = handle;
    }

    private void increaseCount(long handle) {
        if (counter.containsKey(handle)) {
            counter.get(handle).incrementAndGet();
        } else {
            counter.put(handle, new AtomicInteger(1));
        }
    }

    private void decreaseThisCount() {
        if (nativeHandle == null) {
            return;
        }
        // Check the reference counter to avoid delete early
        if (counter.containsKey(nativeHandle)) {
            int val = counter.get(nativeHandle).decrementAndGet();
            if (val == 0) {
                counter.remove(nativeHandle);
            }
        }
    }

    /**
     * Delete the underlying native class
     *
     * @param handle the handle
     */
    abstract protected void nativeDelete(long handle);

    @Override
    public void close() throws Exception {
        // Wrap the nativeDelete to close method so that the IDE will have a warning
        // if the object is not deleted, and will be auto deleted in a try catch block.
        decreaseThisCount();
        nativeHandle = null;
    }
}
