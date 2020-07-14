#pragma once

#include <jni.h>

extern jclass    java_util_Map;
extern jmethodID java_util_Map_get;

extern jclass    java_util_ArrayList;
extern jmethodID java_util_ArrayList_;
extern jmethodID java_util_ArrayList_add;
extern jmethodID java_util_ArrayList_get;
extern jmethodID java_util_ArrayList_size;

extern jclass    java_lang_Float;
extern jmethodID java_lang_Float_valueOf;
extern jmethodID java_lang_Float_floatValue;

extern jclass    java_lang_Double;
extern jmethodID java_lang_Double_valueOf;
extern jmethodID java_lang_Double_doubleValue;

extern jclass    java_lang_Integer;
extern jmethodID java_lang_Integer_valueOf;
extern jmethodID java_lang_Integer_intValue;

extern jclass    java_lang_Long;
extern jmethodID java_lang_Long_valueOf;
extern jmethodID java_lang_Long_longValue;

extern jclass java_nio_IntBuffer;
extern jmethodID java_nio_IntBuffer_allocate;
extern jmethodID java_nio_IntBuffer_array;

extern jclass java_nio_LongBuffer;
extern jmethodID java_nio_LongBuffer_allocate;
extern jmethodID java_nio_LongBuffer_array;

extern jclass java_nio_FloatBuffer;
extern jmethodID java_nio_FloatBuffer_allocate;
extern jmethodID java_nio_FloatBuffer_array;

extern jclass java_nio_DoubleBuffer;
extern jmethodID java_nio_DoubleBuffer_allocate;
extern jmethodID java_nio_DoubleBuffer_array;

extern jclass com_uber_neuropod_TensorSpec;
extern jmethodID com_uber_neuropod_TensorSpec_;

extern jclass com_uber_neuropod_Dimension;
extern jmethodID com_uber_neuropod_Dimension_value_;
extern jmethodID com_uber_neuropod_Dimension_symbol_;

extern jclass com_uber_neuropod_NeuropodJNIException;

extern jclass com_uber_neuropod_TensorType;
