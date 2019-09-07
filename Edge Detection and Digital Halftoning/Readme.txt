Edge Detection and Digital Half-toning



Requirements:

1. Mac/Windows/Ubuntu
2. Vscode
3. C++ (g++ compiler)
4. MATLAB 2016b+

Usage: 

Use below commands to run all .cpp files

g++ filename.cpp
./filename InputImageName OutputImageName BytesPerPixel Width Height


For Structured Edge detection
Run edgesDemo.m
>> I = imread('filename.jpg');
>> E = edgesDetect(I, model);
>> imshow(E)
