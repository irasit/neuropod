# To build this neuropod java api test under OS X environment:
1. Build the cpp neuropod
2. Make sure the g++ can find the header and library of neuropod, for example copy them to /usr/local/include/ and /usr/local/lib/ respectively, and change the NEUROPOD_PATH and JAVA_HOME to the correct path.
3. To run the simple test:
```
make run
```
4. To build the jar file:
```
make jar
```
5. To clean up:
```
make clean
```

# The java api interface:
1. To load a model:
```
Neuropod neuropod = new Neuropod(modelPath);
```
2. To prepare the input feature:
```
NeuropodValueMap inputs = new NeuropodValueMap();
NeuropodValue tensor1 = NeuropodValue.create(new float[]{1.0f, 3.0f}, Arrays.asList(2L, 1L), neuropod);
inputs.addEntry("request_location_latitude",tensor1);
tensor1.close();
NeuropodValue tensor2 = NeuropodValue.create(new float[]{2.0f, 5.0f}, Arrays.asList(2L, 1L), neuropod);
inputs.addEntry("request_location_longitude",tensor2);
tensor2.close();
...
```
3. To do the inference job:
```
NeuropodValueMap valueMap = neuropod.infer(inputs);
```

4. To retrieve the results:
```
Map<String, Object> outputs = valueMap.toJavaMap();
```

5. Don't forget to clean up the memory:
```
valueMap.close();
neuropod.close();
```
