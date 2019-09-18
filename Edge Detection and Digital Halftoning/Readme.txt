Edge Detection and Digital Half-toning

Description:

Edge detection is a process of finding boundaries of image within images. Used different edge detection methods like Sobel Edge Detector, Canny Edge Detector, Structured Edge, tuned the parameters and compared their performances.

Halftoning is a method for creating the illusion of continuous tone output with a binary device. Used different dithering techniques and error diffusion techniques for Digital Halfotoning for grayscale and color images.

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
