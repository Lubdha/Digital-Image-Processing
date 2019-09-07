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
#include <fstream>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel;
    int width ;
    int height ;

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

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);

    fclose(file);


    unsigned char Originaldata[height][width][BytesPerPixel];
    if (!(file=fopen(argv[6],"rb"))) {
        cout << "Cannot open file: " << argv[6] <<endl;
        exit(1);
    }
    fread(Originaldata, sizeof(unsigned char), width*height, file);

    fclose(file);

    unsigned char ImageOutdata[height][width];
    float PSNR[2]={0};
    int p=0;
    for(int window=3; window<6; window +=2) {
        unsigned int filter[window][window];
        for (int i = 0; i < window; i++) {
            for (int j = 0; j < window; j++) {
                filter[i][j] = 1;
            }
        }

        unsigned char ImageBounddata[height + window - 1][width + window - 1];

        int i;
        int j;
        int m = (window - 1) / 2;

        //copying matrix without boundary
        for (i = m; i < height + m; i++) {
            for (j = m; j < width + m; j++) {
                ImageBounddata[i][j] = Imagedata[i - m][j - m][0];
            }
        }
        //adding boundary columns
        for (int i = 0; i < height; i++) {
            for (j = 1; j < m + 1; j++) {
                ImageBounddata[i + m][m - j] = Imagedata[i][j][0];
                ImageBounddata[i + m][width + m + j - 1] = Imagedata[i][width - j - 1][0];
            }
        }
        //adding boundary rows
        for (int j = 0; j < width; j++) {
            for (i = 0; i < m; i++) {
                ImageBounddata[i][j] = ImageBounddata[i + m + 1][j];
                ImageBounddata[height + m + i][j] = ImageBounddata[height + m - 2 - i][j];
            }
        }


        int a = 1;
        float weight;

        for (int i = m; i < height + m; i++) {
            for (int j = m; j < width + m; j++) {
                int intensity_after_filtering = 0;
                weight = 0.0;
                float g = 0.0;
                for (int k = 0; k < window; k++) {
                    for (int l = 0; l < window; l++) {
                        g = (1 / (sqrt(2 * M_PI) * a)) * exp(-(k * k) - (l * l)) / (2 * (a * a));
                        intensity_after_filtering += g * ImageBounddata[i - m + k][j - m + l];
                        weight += g;
                    }
                }
                ImageOutdata[i - m][j - m] = (int)(intensity_after_filtering / weight);
            }
        }
        float mse=0.0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                mse += pow((Originaldata[i][j][0] - ImageOutdata[i][j]), 2);
            }
        }
        mse = mse / (height * width);
        PSNR[p] = 10 * log10(255 * 255 / mse);
        p +=1;
    }

    ofstream psnr("psnr.csv");
    if (psnr.is_open()){
        for(int j=0; j<22; j++){
            psnr<< PSNR[j] << endl;
        }
        psnr.close();
    }


    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(ImageOutdata, sizeof(unsigned char), width*height*1, file);
    fclose(file);

    return 0;
}