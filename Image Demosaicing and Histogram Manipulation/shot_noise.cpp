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
#include <vector>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel;
    int width ;
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
    BytesPerPixel=1;
    // Allocate image data array
    unsigned char Imagedata[height][width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);

    fclose(file);

    int window=5;
    unsigned int filter[5][5]= {{1,4,7,4,1},{4,16,26,16,4},{7,26,41,26,7},{4,16,26,16,4},{1,4,7,4,1}};

    unsigned char ImageBounddata[height+4][width+4];
    unsigned char ImageOutdata[height][width];
    int i;
    int j;

    //Anscombe root transformation

    for (i = 2; i < height ; i++) {
        for (j = 2; j < width ; j++) {
            Imagedata[i-2][j-2][0] = 2*sqrt(Imagedata[i][j][0] +3/8);
        }
    }

    //copying matrix without boundary

    for (i = 2; i < height + 2; i++) {
        for (j = 2; j < width + 2; j++) {
            ImageBounddata[i][j] = Imagedata[i - 2][j - 2][0];
        }
    }

    //adding boundary columns

    for (int i = 0; i < height; i++) {
        ImageBounddata[i + 2][0] = Imagedata[i][2][0];
        ImageBounddata[i + 2][1] = Imagedata[i][1][0];
        ImageBounddata[i + 2][width + 2] = Imagedata[i][width - 2][0];
        ImageBounddata[i + 2][width + 3] = Imagedata[i][width - 3][0];
    }

    //adding boundary rows

    for (int j = 0; j < width; j++) {
        ImageBounddata[0][j] = ImageBounddata[4][j];
        ImageBounddata[1][j] = ImageBounddata[3][j];
        ImageBounddata[height + 2][j] = ImageBounddata[height][j];
        ImageBounddata[height + 3][j] = ImageBounddata[height - 1][j];
    }


    int m= (window-1)/2;


    for (int i = 2; i < height + 2; i++) {
        for (int j = 2; j < width + 2; j++) {
            int intensity_after_filtering = 0;
            for (int k = 0; k < window; k++) {
                for (int l = 0; l < window; l++) {
                    intensity_after_filtering =
                            intensity_after_filtering + (filter[k][l] * ImageBounddata[i - m + k][j - m + l]);
                }
            }
            ImageOutdata[i - 2][j - 2] = intensity_after_filtering / 273;
        }
    }


    //inverse transform
    for (i = 2; i < height ; i++) {
        for (j = 2; j < width ; j++) {
            ImageOutdata[i-2][j-2] = pow(ImageOutdata[i][j],2)/4 -(3/8);
        }
    }

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(ImageOutdata, sizeof(unsigned char), width*height*BytesPerPixel, file);
    fclose(file);

    return 0;
}