//Lubdha Pimpale
//email- lpimpale@usc.edu
//date- 22 Jan 2019

// This sample code reads in image data from a RAW image file and
// writes it into another file

// NOTE:   The code assumes that the image is of size 256 x 256 and is in the
//       RAW format. You will need to make corresponding changes to
//       accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel;
    int width;
    int height;

    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
        return 0;
    }

    // Check if image is grayscale or color
    if (argc < 4){
        BytesPerPixel = 1; // default is grey image
    }
    else {
        BytesPerPixel = atoi(argv[3]);
        // Check if size is specified
        if (argc >= 5){
            width = atoi(argv[4]);
            height = atoi(argv[5]);
        }
    }

    // Allocate image data array
    unsigned char Imagedata[height][width][BytesPerPixel];
    unsigned char ImageOutdata[height][width];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
    fclose(file);

    int window_small=5;
    int window_big= 25;
    int m= (window_small-1)/2;
    int n= (window_big-1)/2;

    unsigned char ImageBounddata[height+24][width+24];
    int i;
    int j;
    //copying matrix without boundary
    for (i = n; i < height + n; i++) {
        for (j = n; j < width + n; j++) {
            ImageBounddata[i][j] = Imagedata[i - n][j - n][0];
        }
    }
    //adding boundary columns
    for (int i = 0; i < height; i++) {
        for (j = 1; j < n + 1; j++) {
            ImageBounddata[i + n][n - j] = Imagedata[i][j][0];
            ImageBounddata[i + n][width + n + j - 1] = Imagedata[i][width - j - 1][0];
        }
    }
    //adding boundary rows
    for (int j = 0; j < width; j++) {
        for (i = 0; i < n; i++) {
            ImageBounddata[i][j] = ImageBounddata[i + n + 1][j];
            ImageBounddata[height + n + i][j] = ImageBounddata[height + n - 2 - i][j];
        }
    }
/*
    //copying matrix without boundary
    for(i=12; i<height+12; i++) {
        for (j = 12; j < width+12; j++) {
            ImageBounddata[i][j] = Imagedata[i - 12][j - 12][0];
        }
    }
    //adding boundary columns
    for(int i=0; i<height; i++) {
        for(j=1; j<13; j++) {
            ImageBounddata[i + 12][12 - j] = Imagedata[i][j][0];
            ImageBounddata[i + 12][width + 12 + j - 1] = Imagedata[i][width - j - 1][0];
        }
    }

    //adding boundary rows
    for(int j=0; j<width; j++) {
        for(i=0; i<12; i++) {
            ImageBounddata[i][j + 12] = ImageBounddata[24 - i][j];
            ImageBounddata[height + 12 + i][j + 2] = ImageBounddata[height + 10 - i][j];
        }
    }
*/
    float g;
    int a=1;
    int h=100;
    float intensity_after_filtering;
    float weight;
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            intensity_after_filtering = 0.0;
            weight = 0.0;
            for(int x= -n;x< n+1;x++) {
                for (int y = -n; y < n +1; y++) {
                    g= 0.0;
                    for (int k = -m; k < m + 1; k++) {
                        for (int l = -m; l < m + 1; l++) {
                            g += (1 / (sqrt(2 * M_PI) * a)) * exp((-(k * k) - (l * l)) / (2 * (a * a))) *
                                 pow((ImageBounddata[k+m + i-2][l+m + j-2] - ImageBounddata[x+n + k +m ][y+n + l+m]), 2);
                        }
                    }
                    intensity_after_filtering += ImageBounddata[x+n][y+n] * exp(-g / (h * h));
                    weight += exp(-g / (h * h));
                }
            }
            ImageOutdata[i][j] = (int)intensity_after_filtering/weight;
        }
    }


    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(ImageOutdata, sizeof(unsigned char), width*height*1, file);
    fclose(file);

    return 0;
}