package com.uber.neuropod;

import java.io.*;
import java.net.URL;

/**
 * This class is responsible for loading jni library
 */
class LibraryLoader {
    private static final String LIBNAME = "neuropod_jni";

    /**
     * Load jni library.
     */
    public static void load() {
        if (isLoaded() || loadSharedLibrary() || loadEmbeddedLibrary()) {
            return;
        }
        throw new UnsatisfiedLinkError("Load neuropod jni library failed");
    }

    private static boolean loadSharedLibrary() {
        try {
            System.loadLibrary(LIBNAME);
            System.out.println("java.library.path: " + System.getProperty("java.library.path"));
            return true;
        } catch (UnsatisfiedLinkError e) {
            System.out.println("tryLoadLibraryFailed: " + e.getMessage());
            return false;
        }
    }

    private static boolean loadEmbeddedLibrary() {
        // TODO: Contains os arch and os name info in path, compile binaries for multiple platforms
        // TODO: Also Include neuropod library and corresponding backend library (like tensorflow or pytorch)
        // in jar file or static link it into neuropod jni library
        final String jniLibName = System.mapLibraryName(LIBNAME);
        String path = "/";

        URL nativeLibraryUrl = LibraryLoader.class.getResource(path + jniLibName);

        try {
            final File libFile = File.createTempFile(LIBNAME, ".lib");
            libFile.deleteOnExit(); // just in case
            System.out.println(nativeLibraryUrl);
            System.out.println(jniLibName);
            final InputStream in = nativeLibraryUrl.openStream();
            final OutputStream out = new BufferedOutputStream(new FileOutputStream(libFile));
            // Copy library file in jar to temporary file
            int len = 0;
            byte[] buffer = new byte[16384];
            while ((len = in.read(buffer)) > -1)
                out.write(buffer, 0, len);
            out.close();
            in.close();
            System.load(libFile.getAbsolutePath());

        } catch (IOException x) {
            System.out.println(x.getMessage());
            return false;
        }

        return true;
    }


    private static boolean isLoaded() {
        try {
            return nativeIsLoaded();
        } catch (UnsatisfiedLinkError e) {
            return false;
        }
    }

    private static native boolean nativeIsLoaded();
}

