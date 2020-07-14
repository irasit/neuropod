package com.uber.neuropod;

/**
 * An One to One mapping to RuntimeOptions native class.
 */
public class RuntimeOptions {

    /**
     * The Free memory every cycle.
     * Internally, OPE(out-of-process execution) uses a shared memory allocator that reuses
     * blocks of memory if possible. Therefore memory isn't necessarily allocated during each
     * inference cycle as blocks may be reused.
     * If freeMemoryEveryCycle is set, then unused shared memory will be freed every cycle
     * This is useful for simple inference, but for code that is pipelined
     * (e.g. generating inputs for cycle t + 1 during the inference of cycle t), this may not
     * be desirable.
     * If freeMemoryEveryCycle is false, the user is responsible for periodically calling
     * neuropod.freeUnusedShmBlocks()
     */
    private boolean freeMemoryEveryCycle = true;
    /**
     * The Control queue name.
     * This option can be used to run the neuropod in an existing worker process
     * If this string is empty, a new worker will be started.
     */
    private String controlQueueName = "";
    /**
     * The device to run this Neuropod on.
     * Some devices are defined in the namespace above. For machines with more
     * than 8 GPUs, passing in an index will also work (e.g. `9` for `GPU9`).
     * To attempt to run the model on CPU, set this to `NeuropodDevice.CPU`
     */
    private int visibleDevice = NeuropodDevice.GPU0;
    /**
     * The Load model at construction.
     * Sometimes, it's important to be able to instantiate a Neuropod without
     * immediately loading the model. If this is set to `false`, the model will
     * not be loaded until the `loadModel` method is called on the Neuropod.
     */
    private boolean loadModelAtConstruction = true;
    /**
     * The Disable shape and type checking.
     * Whether or not to disable shape and type checking when running inference.
     */
    private boolean disableShapeAndTypeChecking = false;

    /**
     * Instantiates a new Runtime options.
     */
    public RuntimeOptions() {
    }

    /**
     * To native runtime options native.
     *
     * @return the runtime options native
     */
    RuntimeOptionsNative toNative() {
        return new RuntimeOptionsNative(freeMemoryEveryCycle, controlQueueName, visibleDevice, loadModelAtConstruction, disableShapeAndTypeChecking);
    }

    /**
     * Is free memory every cycle boolean.
     *
     * @return the boolean
     */
    public boolean isFreeMemoryEveryCycle() {
        return freeMemoryEveryCycle;
    }

    /**
     * Sets free memory every cycle.
     *
     * @param freeMemoryEveryCycle the free memory every cycle
     */
    public void setFreeMemoryEveryCycle(boolean freeMemoryEveryCycle) {
        this.freeMemoryEveryCycle = freeMemoryEveryCycle;
    }

    /**
     * Gets control queue name.
     *
     * @return the control queue name
     */
    public String getControlQueueName() {
        return controlQueueName;
    }

    /**
     * Sets control queue name.
     *
     * @param controlQueueName the control queue name
     */
    public void setControlQueueName(String controlQueueName) {
        this.controlQueueName = controlQueueName;
    }

    /**
     * Gets visible device.
     *
     * @return the visible device
     */
    public int getVisibleDevice() {
        return visibleDevice;
    }

    /**
     * Sets visible device.
     *
     * @param visibleDevice the visible device
     */
    public void setVisibleDevice(int visibleDevice) {
        this.visibleDevice = visibleDevice;
    }

    /**
     * Is load model at construction boolean.
     *
     * @return the boolean
     */
    public boolean isLoadModelAtConstruction() {
        return loadModelAtConstruction;
    }

    /**
     * Sets load model at construction.
     *
     * @param loadModelAtConstruction the load model at construction
     */
    public void setLoadModelAtConstruction(boolean loadModelAtConstruction) {
        this.loadModelAtConstruction = loadModelAtConstruction;
    }

    /**
     * Is disable shape and type checking boolean.
     *
     * @return the boolean
     */
    public boolean isDisableShapeAndTypeChecking() {
        return disableShapeAndTypeChecking;
    }

    /**
     * Sets disable shape and type checking.
     *
     * @param disableShapeAndTypeChecking the disable shape and type checking
     */
    public void setDisableShapeAndTypeChecking(boolean disableShapeAndTypeChecking) {
        this.disableShapeAndTypeChecking = disableShapeAndTypeChecking;
    }

    /**
     * The corresponding native type for RuntimeOptions. Is not exposed to user, only used inside other public
     * method.
     */
    static class RuntimeOptionsNative extends NativeClass {
        static {
            LibraryLoader.load();
        }

        /**
         * Instantiates a new Runtime options native.
         *
         * @param freeMemoryEveryCycle        the free memory every cycle
         * @param controlQueueName            the control queue name
         * @param visibleDevice               the visible device
         * @param loadModelAtConstruction     the load model at construction
         * @param disableShapeAndTypeChecking the disable shape and type checking
         */
        RuntimeOptionsNative(boolean freeMemoryEveryCycle, String controlQueueName, int visibleDevice, boolean loadModelAtConstruction, boolean disableShapeAndTypeChecking) {
            super(nativeCreate(freeMemoryEveryCycle, controlQueueName, visibleDevice, loadModelAtConstruction, disableShapeAndTypeChecking));
        }

        static private native long nativeCreate(boolean freeMemoeryEverySycle, String controlQueueName, int visibleDevice, boolean loadModelAtConstruction, boolean disableShapeAndTypeChecking);

        @Override
        protected native void nativeDelete(long handle);
    }
}
