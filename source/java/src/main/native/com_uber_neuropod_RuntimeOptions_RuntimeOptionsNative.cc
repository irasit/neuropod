#include "com_uber_neuropod_RuntimeOptions_RuntimeOptionsNative.h"

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


JNIEXPORT jlong JNICALL Java_com_uber_neuropod_RuntimeOptions_00024RuntimeOptionsNative_nativeCreate
        (JNIEnv *env, jclass, jboolean freeMemoryEveryCycle, jstring jControlQueueName, jint visibleDevice,
         jboolean loadModelAtConstruction, jboolean disableShapeAndTypeChecking) {
    try {
        std::string controlQueueName = toString(env, jControlQueueName);
        auto opts = std::make_shared<neuropod::RuntimeOptions>();
        opts->ope_options.free_memory_every_cycle = (freeMemoryEveryCycle == JNI_TRUE);
        opts->ope_options.control_queue_name = controlQueueName;
        opts->visible_device = static_cast<int32_t>(visibleDevice);
        opts->load_model_at_construction = (loadModelAtConstruction == JNI_TRUE);
        opts->disable_shape_and_type_checking = (disableShapeAndTypeChecking == JNI_TRUE);
        ReferenceManager<neuropod::RuntimeOptions>::put(opts);
        return reinterpret_cast<jlong>(opts.get());
    } catch (const std::exception &e) {
        throwJavaException(env, e.what());
    }
    return reinterpret_cast<jlong>(nullptr);

}

JNIEXPORT void JNICALL Java_com_uber_neuropod_RuntimeOptions_00024RuntimeOptionsNative_nativeDelete
        (JNIEnv *, jobject, jlong handle) {
    ReferenceManager<neuropod::RuntimeOptions>::remove(handle);
}
