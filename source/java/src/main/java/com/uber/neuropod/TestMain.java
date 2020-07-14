package com.uber.neuropod;

import java.awt.*;
import java.nio.FloatBuffer;
import java.util.*;
import java.util.List;

public class TestMain {

    public static void main(String[] args) throws Exception {
        System.out.println("Start working......");

        String modelPath = "src/test/resources/test.zip";
        System.out.println("Loading model = " + System.getProperty("user.dir") + modelPath);
        // Load a neuropod model
        Neuropod neuropod = new Neuropod(modelPath);

        System.out.println("The input spec is:");
        System.out.println(neuropod.getInputs().get(0).getDims());
        System.out.println("The output spec is:");
        System.out.println(neuropod.getOutpus());

        System.out.println();
        System.out.println("Normal procedure:");

        NeuropodValueMap inputs = new NeuropodValueMap();
        NeuropodValue tensor1 = NeuropodValue.create(FloatBuffer.wrap(new float[] { 1.0f, 3.0f }),
                new long[]{2L, 1L}, neuropod);
        inputs.addEntry("request_location_latitude",tensor1);
        tensor1.close();

        NeuropodValue tensor2 = NeuropodValue.create(FloatBuffer.wrap(new float[] { 2.0f, 5.0f }),
                new long[]{2L, 1L}, neuropod);
        inputs.addEntry("request_location_longitude",tensor2);
        tensor2.close();

        System.out.println("The input is:");
        System.out.println(inputs);

        NeuropodValueMap valueMap = neuropod.infer(inputs);

        System.out.println("The output is:");
        System.out.println(valueMap);
        System.out.println();

        System.out.println("Try to infer tensors with wrong shape:");
        Map<String, Object> example2 = new HashMap<>();
        example2.put("request_location_latitude", FloatBuffer.wrap(new float[]{1.0f, 3.0f}));
        example2.put("request_location_longitude", FloatBuffer.wrap(new float[]{1.0f, 3.0f}));
        Map<String, long[]> dims2 = new HashMap<>();
        dims2.put("request_location_latitude", new long[]{1L, 2L});
        dims2.put("request_location_longitude", new long[]{1L, 2L});
        try {
            neuropod.infer(example2, dims2);
        } catch (Exception e) {
            System.out.println(e);
        }
        System.out.println();

        System.out.println("Try to infer on closed tensor:");
        inputs.close();
        try {
            neuropod.infer(inputs);
        } catch (Exception e) {
            System.out.println(e);
        }


        neuropod.close();
        // Double delete test
        neuropod.close();

        valueMap.close();

        System.out.println("Safely quit!");
    }

}
