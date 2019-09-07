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
    BytesPerPixel=3;
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

    unsigned char ImageBounddata[height+4][width+4][BytesPerPixel];
    unsigned char ImageOutdata[height][width][BytesPerPixel];
    int i;
    int j;

    //copying matrix without boundary
    for(int c=0;c<BytesPerPixel; c++) {
        for (i = 2; i < height + 2; i++) {
            for (j = 2; j < width + 2; j++) {
                ImageBounddata[i][j][c] = Imagedata[i - 2][j - 2][c];
            }
        }
    }
    //adding boundary columns
    for(int c=0;c<BytesPerPixel; c++) {
        for (int i = 0; i < height; i++) {
            ImageBounddata[i + 2][0][c] = Imagedata[i][2][c];
            ImageBounddata[i + 2][1][c] = Imagedata[i][1][c];
            ImageBounddata[i + 2][width + 2][c] = Imagedata[i][width - 2][c];
            ImageBounddata[i + 2][width + 3][c] = Imagedata[i][width - 3][c];
        }
    }
    //adding boundary rows
    for(int c=0;c<BytesPerPixel; c++) {
        for (int j = 0; j < width; j++) {
            ImageBounddata[0][j + 2][c] = ImageBounddata[4][j][c];
            ImageBounddata[1][j + 2][c] = ImageBounddata[3][j][c];
            ImageBounddata[height + 2][j + 2][c] = ImageBounddata[height][j][c];
            ImageBounddata[height + 3][j + 2][c] = ImageBounddata[height - 1][j][c];
        }
    }

    int m= (window-1)/2;

    for(int c=0;c<BytesPerPixel; c++) {
        for (int i = 2; i < height + 2; i++) {
            for (int j = 2; j < width + 2; j++) {
                int intensity_after_filtering = 0;
                for (int k = 0; k < window; k++) {
                    for (int l = 0; l < window; l++) {
                        intensity_after_filtering =
                                intensity_after_filtering + (filter[k][l] * ImageBounddata[i - m + k][j - m + l][c]);
                    }
                }
                ImageOutdata[i - 2][j - 2][c] = intensity_after_filtering / 273;
            }
        }
    }
    //copying matrix without boundary
    for(int c=0;c<BytesPerPixel; c++) {
        for (i = 2; i < height + 2; i++) {
            for (j = 2; j < width + 2; j++) {
                ImageBounddata[i][j][c] = ImageOutdata[i - 2][j - 2][c];
            }
        }
    }
    //adding boundary columns
    for(int c=0;c<BytesPerPixel; c++) {
        for (int i = 0; i < height; i++) {
            ImageBounddata[i + 2][0][c] = ImageOutdata[i][2][c];
            ImageBounddata[i + 2][1][c] = ImageOutdata[i][1][c];
            ImageBounddata[i + 2][width + 2][c] = ImageOutdata[i][width - 2][c];
            ImageBounddata[i + 2][width + 3][c] = ImageOutdata[i][width - 3][c];
        }
    }
    //adding boundary rows
    for(int c=0;c<BytesPerPixel; c++) {
        for (int j = 0; j < width; j++) {
            ImageBounddata[0][j + 2][c] = ImageBounddata[4][j][c];
            ImageBounddata[1][j + 2][c] = ImageBounddata[3][j][c];
            ImageBounddata[height + 2][j + 2][c] = ImageBounddata[height][j][c];
            ImageBounddata[height + 3][j + 2][c] = ImageBounddata[height - 1][j][c];
        }
    }

    static std::vector<int> median_array(window*window);
    for(int c=0;c<BytesPerPixel; c++) {
        for (int i = 2; i < height + 2; i++) {
            for (int j = 2; j < width + 2; j++) {
                for (int k = 0; k < window; k++) {
                    for (int l = 0; l < window; l++) {
                        median_array[k * window + l] = ImageBounddata[i - m + k][j - m + l][c];
                    }
                }
                std::sort(median_array.begin(), median_array.end());
                ImageOutdata[i - 2][j - 2][c] = median_array[m];
            }
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