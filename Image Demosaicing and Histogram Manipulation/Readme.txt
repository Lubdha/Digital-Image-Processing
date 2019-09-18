Image Demosaicing, Histogram Equalization and Denoising

Description:

The aim of a demosaicing algorithm is to reconstruct a full color image (i.e. a full set of color triples) from the spatially under sampled color channels output from the CFA. Used Bilinear Demosaicing, Malvar-He-Cutler Demosaicing. Used Histogram Equilization to improve color contrast. 

Used different Denoising techniques for grayscale and color images.

Requirements:

1.	Mac/Windows
2.	Vscode
3.	C++ (g++ compiler)

Usage Information:

Use below command to run each file:

g++ filename.cpp
./filename InputImageName OutputImageName BytesPerPixel Width Height
