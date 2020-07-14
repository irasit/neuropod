export JAVA_HOME=/Library/Java/JavaVirtualMachines/adoptopenjdk-8.jdk/Contents/Home
export MY_JNI_HEADERS=${PWD}/src/main/native
export PROJECT_PACKAGE=${PWD}/src/main/java/com/uber/neuropod
export CPP_FOLDER=${PWD}/src/main/native
export NEUROPOD_PATH = /usr/local/lib/libneuropod.so

export OUT_DIR=${PWD}/out

javalib:
	mkdir -p ${OUT_DIR}
	javac -source 1.8 -h ${MY_JNI_HEADERS} -d ${OUT_DIR} ${PROJECT_PACKAGE}/LibraryLoader.java ${PROJECT_PACKAGE}/Dimension.java ${PROJECT_PACKAGE}/NeuropodDevice.java ${PROJECT_PACKAGE}/NeuropodValue.java ${PROJECT_PACKAGE}/TensorSpec.java ${PROJECT_PACKAGE}/NeuropodValueMap.java ${PROJECT_PACKAGE}/NativeClass.java ${PROJECT_PACKAGE}/Neuropod.java ${PROJECT_PACKAGE}/RuntimeOptions.java ${PROJECT_PACKAGE}/TensorType.java ${PROJECT_PACKAGE}/NeuropodJNIException.java

javamain:
	javac -source 1.8 -h ${MY_JNI_HEADERS} -d ${OUT_DIR} ${PROJECT_PACKAGE}/LibraryLoader.java ${PROJECT_PACKAGE}/Dimension.java ${PROJECT_PACKAGE}/NeuropodDevice.java ${PROJECT_PACKAGE}/NeuropodValue.java ${PROJECT_PACKAGE}/TensorSpec.java ${PROJECT_PACKAGE}/NeuropodValueMap.java ${PROJECT_PACKAGE}/NativeClass.java ${PROJECT_PACKAGE}/Neuropod.java ${PROJECT_PACKAGE}/RuntimeOptions.java ${PROJECT_PACKAGE}/TensorType.java ${PROJECT_PACKAGE}/NeuropodJNIException.java ${PROJECT_PACKAGE}/TestMain.java

jclass_register.o: javalib
	g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/darwin -I${MY_JNI_HEADERS} ${CPP_FOLDER}/jclass_register.cc -o ${OUT_DIR}/jclass_register.o -std=c++14

utils.o: javalib
	mkdir -p ${OUT_DIR}
	g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/darwin -I${MY_JNI_HEADERS} ${CPP_FOLDER}/utils.cc -o ${OUT_DIR}/utils.o -std=c++14

com_uber_neuropod_LibraryLoader.o: javalib
	mkdir -p ${OUT_DIR}
	g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/darwin -I${MY_JNI_HEADERS} ${CPP_FOLDER}/com_uber_neuropod_LibraryLoader.cc -o ${OUT_DIR}/com_uber_neuropod_LibraryLoader.o -std=c++14

com_uber_neuropod_Neuropod.o: javalib
	mkdir -p ${OUT_DIR}
	g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/darwin -I${MY_JNI_HEADERS} ${CPP_FOLDER}/com_uber_neuropod_Neuropod.cc -o ${OUT_DIR}/com_uber_neuropod_Neuropod.o -std=c++14

com_uber_neuropod_NeuropodValue.o: javalib
	mkdir -p ${OUT_DIR}
	g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/darwin -I${MY_JNI_HEADERS} ${CPP_FOLDER}/com_uber_neuropod_NeuropodValue.cc -o ${OUT_DIR}/com_uber_neuropod_NeuropodValue.o -std=c++14

com_uber_neuropod_NeuropodValueMap.o: javalib
	mkdir -p ${OUT_DIR}
	g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/darwin -I${MY_JNI_HEADERS} ${CPP_FOLDER}/com_uber_neuropod_NeuropodValueMap.cc -o ${OUT_DIR}/com_uber_neuropod_NeuropodValueMap.o -std=c++14

com_uber_neuropod_RuntimeOptionsNative.o: javalib
	mkdir -p ${OUT_DIR}
	g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/darwin -I${MY_JNI_HEADERS} ${CPP_FOLDER}/com_uber_neuropod_RuntimeOptions_RuntimeOptionsNative.cc -o ${OUT_DIR}/com_uber_neuropod_RuntimeOptions_RuntimeOptionsNative.o -std=c++14

libneuropod_jni.dylib: com_uber_neuropod_Neuropod.o com_uber_neuropod_NeuropodValue.o com_uber_neuropod_NeuropodValueMap.o com_uber_neuropod_LibraryLoader.o utils.o jclass_register.o com_uber_neuropod_RuntimeOptionsNative.o
	g++ -dynamiclib -o ${OUT_DIR}/libneuropod_jni.dylib ${OUT_DIR}/com_uber_neuropod_LibraryLoader.o ${OUT_DIR}/com_uber_neuropod_Neuropod.o ${OUT_DIR}/com_uber_neuropod_NeuropodValue.o ${OUT_DIR}/com_uber_neuropod_RuntimeOptions_RuntimeOptionsNative.o ${OUT_DIR}/com_uber_neuropod_NeuropodValueMap.o ${OUT_DIR}/utils.o ${OUT_DIR}/jclass_register.o ${NEUROPOD_PATH} -framework JavaVM
	install_name_tool -change bazel-out/darwin-fastbuild/bin/neuropod/libneuropod.so ${NEUROPOD_PATH} ${OUT_DIR}/libneuropod_jni.dylib


run: libneuropod_jni.dylib javamain
	java -cp ${OUT_DIR} -Djava.library.path=${OUT_DIR} com.uber.neuropod.TestMain

jar: libneuropod_jni.dylib
	rm ${OUT_DIR}/*.o
	jar cvfm test.jar ${PWD}/src/main/resources/META-INF/MANIFEST.MF -C ./out/ .

clean:
	rm -rf ${OUT_DIR}

