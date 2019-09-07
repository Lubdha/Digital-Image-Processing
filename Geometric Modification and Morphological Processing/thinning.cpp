//Lubdha Pimpale
//email- lpimpale@usc.edu
//date- 3rd March 2019

#include <stdio.h>
#include <iostream>
#include <stdlib.h>


using namespace std;

int main(int argc, char *argv[]) {
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel;
    int width;
    int height;

    // Check for proper syntax
    if (argc < 3) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
        return 0;
    }

    // Check if image is grayscale or color
    if (argc < 4) {
        BytesPerPixel = 1; // default is grey image
    } else {
        BytesPerPixel = atoi(argv[3]);
        // Check if size is specified
        if (argc >= 5) {
            width = atoi(argv[4]);
            height = atoi(argv[5]);
        }
    }

    // Allocate image data array
    unsigned char Imagedata[height][width][BytesPerPixel];
    unsigned char Out[height][width][BytesPerPixel];
    int ImageBounddata[height+2][width+2][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), width * height * BytesPerPixel, file);

    fclose(file);



    //unconditional shrinking AND THINNING mask
    int UT[332][3][3] = {
            {0, 0, 1, 0, 1, 0, 0, 0, 0},
            {1, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 0, 0, 1, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 0},
            {0, 0, 1, 0, 1, 1, 0, 0, 0},
            {0, 1, 1, 0, 1, 0, 0, 0, 0},
            {1, 1, 0, 0, 1, 0, 0, 0, 0},
            {1, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 0, 0, 0, 0 },
            {0, 0, 0, 0, 1, 0, 1, 1, 0 },
            {0, 0, 0, 0, 1, 0, 0, 1, 1 },
            {0, 0, 0, 0, 1, 1, 0, 0, 1 },
            {0, 1, 1, 1, 1, 0, 0, 0, 0 },
            {1, 1, 0, 0, 1, 1, 0, 0, 0 },
            {0, 1, 0, 0, 1, 1, 0, 0, 1 },
            {0, 0, 1, 0, 1, 1, 0, 1, 0 },
            {0, 0, 1, 0, 1, 1, 1, 0, 0 },
            {1, 1, 0, 0, 1, 0, 0, 0, 1 },
            {0, 0, 1, 0, 1, 0, 1, 1, 0 },
            {1, 0, 0, 0, 1, 1, 0, 0, 1 },
            {0, 1, 1, 0, 1, 0, 1, 0, 0 },
            {1, 0, 0, 1, 1, 0, 0, 0, 1 },
            {0, 0, 1, 1, 1, 0, 1, 0, 0 },
            {1, 0, 0, 0, 1, 0, 0, 1, 1 },
            {0, 1, 1, 0, 1, 1, 1, 0, 0 },
            {1, 1, 0, 1, 1, 0, 0, 0, 1 },
            {0, 0, 1, 1, 1, 0, 1, 1, 0 },
            {1, 0, 0, 0, 1, 1, 0, 1, 1 },
            {0, 1, 0, 1, 1, 1, 0, 0, 0 },
            {0, 1, 0, 1, 1, 1, 0, 0, 0 },
            {0, 0, 0, 1, 1, 1, 0, 1, 0 },
            {0, 0, 0, 1, 1, 1, 0, 1, 0 },
            {0, 1, 0, 1, 1, 0, 0, 1, 0 },
            {0, 1, 0, 1, 1, 0, 0, 1, 0 },
            {0, 1, 0, 0, 1, 1, 0, 1, 0 },
            {0, 1, 0, 0, 1, 1, 0, 1, 0 },
            {0, 1, 0, 0, 1, 1, 1, 0, 0 },
            {0, 1, 0, 1, 1, 0, 0, 0, 1 },
            {0, 0, 1, 1, 1, 0, 0, 1, 0 },
            {1, 0, 0, 0, 1, 1, 0, 1, 0 },
            {1, 1, 0, 1, 1, 0, 0, 0, 0 },
            {1, 1, 0, 1, 1, 0, 0, 0, 1 },
            {1, 1, 0, 1, 1, 0, 0, 1, 0 },
            {1, 1, 0, 1, 1, 0, 0, 1, 1 },
            {1, 1, 0, 1, 1, 0, 1, 0, 0 },
            {1, 1, 0, 1, 1, 0, 1, 0, 1 },
            {1, 1, 0, 1, 1, 0, 1, 1, 0 },
            {1, 1, 0, 1, 1, 0, 1, 1, 1 },
            {0, 1, 0, 1, 1, 1, 1, 0, 0 },
            {0, 1, 0, 1, 1, 1, 0, 0, 1 },
            {0, 0, 0, 1, 1, 1, 0, 1, 1 },
            {0, 0, 0, 1, 1, 1, 1, 1, 0 },
            {0, 1, 1, 1, 1, 0, 0, 1, 0 },
            {0, 1, 0, 1, 1, 0, 0, 1, 1 },
            {0, 1, 0, 0, 1, 1, 0, 1, 1 },
            {0, 1, 1, 0, 1, 1, 0, 1, 0 },
            {0, 1, 0, 0, 1, 1, 1, 0, 1 },
            {0, 1, 0, 1, 1, 0, 1, 0, 1 },
            {0, 0, 1, 1, 1, 0, 0, 1, 1 },
            {1, 0, 0, 0, 1, 1, 1, 1, 0 },
            {1, 1, 0, 1, 1, 1, 0, 0, 0 },
            {1, 1, 0, 1, 1, 1, 0, 0, 1 },
            {1, 1, 0, 1, 1, 1, 0, 1, 0 },
            {1, 1, 0, 1, 1, 1, 0, 1, 1 },
            {1, 1, 0, 1, 1, 1, 1, 0, 0 },
            {1, 1, 0, 1, 1, 1, 1, 0, 1 },
            {1, 1, 0, 1, 1, 1, 1, 1, 0 },
            {1, 1, 0, 1, 1, 1, 1, 1, 1 },
            {1, 1, 0, 1, 1, 1, 0, 0, 0 },
            {0, 1, 1, 1, 1, 1, 0, 0, 0 },
            {0, 0, 1, 1, 1, 1, 0, 1, 0 },
            {1, 0, 0, 1, 1, 1, 0, 1, 0 },
            {1, 1, 0, 1, 1, 0, 0, 1, 0 },
            {0, 1, 0, 1, 1, 0, 1, 1, 0 },
            {0, 1, 0, 0, 1, 1, 1, 1, 0 },
            {1, 1, 0, 0, 1, 1, 0, 1, 0 },
            {1, 1, 0, 0, 1, 1, 1, 0, 0 },
            {0, 1, 1, 1, 1, 0, 0, 0, 1 },
            {1, 0, 1, 1, 1, 0, 0, 1, 0 },
            {1, 0, 1, 0, 1, 1, 0, 1, 0 },
            {1, 1, 1, 1, 1, 0, 0, 0, 0 },
            {1, 1, 1, 1, 1, 0, 0, 0, 1 },
            {1, 1, 1, 1, 1, 0, 0, 1, 0 },
            {1, 1, 1, 1, 1, 0, 0, 1, 1 },
            {1, 1, 1, 1, 1, 0, 1, 0, 0 },
            {1, 1, 1, 1, 1, 0, 1, 0, 1 },
            {1, 1, 1, 1, 1, 0, 1, 1, 0 },
            {1, 1, 1, 1, 1, 0, 1, 1, 1 },
            {1, 1, 0, 1, 1, 1, 1, 0, 0 },
            {0, 1, 1, 1, 1, 1, 0, 0, 1 },
            {0, 0, 1, 1, 1, 1, 0, 1, 1 },
            {1, 0, 0, 1, 1, 1, 1, 1, 0 },
            {1, 1, 1, 1, 1, 0, 0, 1, 0 },
            {0, 1, 0, 1, 1, 0, 1, 1, 1 },
            {0, 1, 0, 0, 1, 1, 1, 1, 1 },
            {1, 1, 1, 0, 1, 1, 0, 1, 0 },
            {1, 1, 0, 0, 1, 1, 1, 0, 1 },
            {0, 1, 1, 1, 1, 0, 1, 0, 1 },
            {1, 0, 1, 1, 1, 0, 0, 1, 1 },
            {1, 0, 1, 0, 1, 1, 1, 1, 0 },
            {1, 1, 1, 1, 1, 1, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 0, 1, 0},
            {1, 1, 1, 1, 1, 1, 0, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 0, 0},
            {1, 1, 1, 1, 1, 1, 1, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 0},
            {1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 1, 0, 1, 0, 0, 0, 1},
            {1, 0, 1, 0, 1, 0, 1, 0, 0},
            {1, 0, 0, 0, 1, 0, 1, 0, 1},
            {0, 0, 1, 0, 1, 0, 1, 0, 1},
            {1, 0, 1, 0, 1, 1, 0, 0, 1},
            {1, 0, 1, 0, 1, 0, 1, 1, 0},
            {1, 0, 0, 0, 1, 0, 1, 1, 1},
            {0, 0, 1, 0, 1, 0, 1, 1, 1},
            {1, 0, 1, 1, 1, 0, 0, 0, 1},
            {1, 0, 1, 1, 1, 0, 1, 0, 0},
            {1, 0, 0, 0, 1, 1, 1, 0, 1},
            {0, 0, 1, 0, 1, 1, 1, 0, 1},
            {1, 0, 1, 1, 1, 1, 0, 0, 1},
            {1, 0, 1, 1, 1, 0, 1, 1, 0},
            {1, 0, 0, 0, 1, 1, 1, 1, 1},
            {0, 0, 1, 0, 1, 1, 1, 1, 1},
            {1, 1, 1, 0, 1, 0, 0, 0, 1},
            {1, 1, 1, 0, 1, 0, 1, 0, 0},
            {1, 0, 0, 1, 1, 0, 1, 0, 1},
            {0, 1, 1, 0, 1, 0, 1, 0, 1},
            {1, 1, 1, 0, 1, 1, 0, 0, 1},
            {1, 1, 1, 0, 1, 0, 1, 1, 0},
            {1, 0, 0, 1, 1, 0, 1, 1, 1},
            {0, 1, 1, 0, 1, 0, 1, 1, 1},
            {1, 1, 1, 1, 1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 0, 1, 0, 0},
            {1, 0, 0, 1, 1, 1, 1, 0, 1},
            {0, 1, 1, 0, 1, 1, 1, 0, 1},
            {1, 1, 1, 1, 1, 1, 0, 0, 1},
            {1, 1, 1, 1, 1, 0, 1, 1, 0},
            {1, 0, 0, 1, 1, 1, 1, 1, 1},
            {0, 1, 1, 0, 1, 1, 1, 1, 1},
            {1, 0, 1, 0, 1, 0, 0, 1, 0},
            {1, 0, 0, 0, 1, 1, 1, 0, 0},
            {0, 1, 0, 0, 1, 0, 1, 0, 1},
            {0, 0, 1, 1, 1, 0, 0, 0, 1},
            {1, 0, 1, 0, 1, 1, 0, 1, 0},
            {1, 0, 0, 0, 1, 1, 1, 1, 0, },
            {0, 1, 0, 0, 1, 0, 1, 1, 1, },
            {0, 0, 1, 1, 1, 0, 0, 1, 1, },
            {1, 0, 1, 1, 1, 0, 0, 1, 0, },
            {1, 0, 0, 1, 1, 1, 1, 0, 0, },
            {0, 1, 0, 0, 1, 1, 1, 0, 1, },
            {0, 0, 1, 1, 1, 1, 0, 0, 1, },
            {1, 0, 1, 1, 1, 1, 0, 1, 0, },
            {1, 0, 0, 1, 1, 1, 1, 1, 0, },
            {0, 1, 0, 0, 1, 1, 1, 1, 1, },
            {0, 0, 1, 1, 1, 1, 0, 1, 1, },
            {1, 1, 1, 0, 1, 0, 0, 1, 0, },
            {1, 1, 0, 0, 1, 1, 1, 0, 0, },
            {0, 1, 0, 1, 1, 0, 1, 0, 1, },
            {0, 1, 1, 1, 1, 0, 0, 0, 1, },
            {1, 1, 1, 0, 1, 1, 0, 1, 0, },
            {1, 1, 0, 0, 1, 1, 1, 1, 0, },
            {0, 1, 0, 1, 1, 0, 1, 1, 1, },
            {0, 1, 1, 1, 1, 0, 0, 1, 1, },
            {1, 1, 1, 1, 1, 0, 0, 1, 0, },
            {1, 1, 0, 1, 1, 1, 1, 0, 0, },
            {0, 1, 0, 1, 1, 1, 1, 0, 1, },
            {0, 1, 1, 1, 1, 1, 0, 0, 1, },
            {1, 1, 1, 1, 1, 1, 0, 1, 0, },
            {1, 1, 0, 1, 1, 1, 1, 1, 0, },
            {0, 1, 0, 1, 1, 1, 1, 1, 1, },
            {0, 1, 1, 1, 1, 1, 0, 1, 1, },
            {1, 0, 1, 0, 1, 0, 0, 1, 1, },
            {1, 0, 1, 0, 1, 1, 1, 0, 0, },
            {1, 1, 0, 0, 1, 0, 1, 0, 1, },
            {0, 0, 1, 1, 1, 0, 1, 0, 1, },
            {1, 0, 1, 0, 1, 1, 0, 1, 1, },
            {1, 0, 1, 0, 1, 1, 1, 1, 0, },
            {1, 1, 0, 0, 1, 0, 1, 1, 1, },
            {0, 0, 1, 1, 1, 0, 1, 1, 1, },
            {1, 0, 1, 1, 1, 0, 0, 1, 1, },
            {1, 0, 1, 1, 1, 1, 1, 0, 0, },
            {1, 1, 0, 0, 1, 1, 1, 0, 1, },
            {0, 0, 1, 1, 1, 1, 1, 0, 1, },
            {1, 0, 1, 1, 1, 1, 0, 1, 1, },
            {1, 0, 1, 1, 1, 1, 1, 1, 0, },
            {1, 1, 0, 0, 1, 1, 1, 1, 1, },
            {0, 0, 1, 1, 1, 1, 1, 1, 1, },
            {1, 1, 1, 0, 1, 0, 0, 1, 1, },
            {1, 1, 1, 0, 1, 1, 1, 0, 0, },
            {1, 1, 0, 1, 1, 0, 1, 0, 1, },
            {0, 1, 1, 1, 1, 0, 1, 0, 1, },
            {1, 1, 1, 0, 1, 1, 0, 1, 1, },
            {1, 1, 1, 0, 1, 1, 1, 1, 0, },
            {1, 1, 0, 1, 1, 0, 1, 1, 1, },
            {0, 1, 1, 1, 1, 0, 1, 1, 1, },
            {1, 1, 1, 1, 1, 0, 0, 1, 1, },
            {1, 1, 1, 1, 1, 1, 1, 0, 0, },
            {1, 1, 0, 1, 1, 1, 1, 0, 1, },
            {0, 1, 1, 1, 1, 1, 1, 0, 1, },
            {1, 1, 1, 1, 1, 1, 0, 1, 1, },
            {1, 1, 1, 1, 1, 1, 1, 1, 0, },
            {1, 1, 0, 1, 1, 1, 1, 1, 1, },
            {0, 1, 1, 1, 1, 1, 1, 1, 1, },
            {1, 0, 1, 0, 1, 0, 1, 0, 0, },
            {1, 0, 0, 0, 1, 0, 1, 0, 1, },
            {0, 0, 1, 0, 1, 0, 1, 0, 1, },
            {1, 0, 1, 0, 1, 0, 0, 0, 1, },
            {1, 0, 1, 0, 1, 1, 1, 0, 0, },
            {1, 0, 0, 0, 1, 0, 1, 1, 1, },
            {0, 0, 1, 0, 1, 0, 1, 1, 1, },
            {1, 0, 1, 0, 1, 0, 0, 1, 1, },
            {1, 0, 1, 1, 1, 0, 1, 0, 0, },
            {1, 0, 0, 1, 1, 0, 1, 0, 1, },
            {0, 0, 1, 0, 1, 1, 1, 0, 1, },
            {1, 0, 1, 0, 1, 1, 0, 0, 1, },
            {1, 0, 1, 1, 1, 1, 1, 0, 0, },
            {1, 0, 0, 1, 1, 0, 1, 1, 1, },
            {0, 0, 1, 0, 1, 1, 1, 1, 1, },
            {1, 0, 1, 0, 1, 1, 0, 1, 1, },
            {1, 1, 1, 0, 1, 0, 1, 0, 0, },
            {1, 1, 0, 0, 1, 0, 1, 0, 1, },
            {0, 0, 1, 1, 1, 0, 1, 0, 1, },
            {1, 1, 1, 0, 1, 0, 0, 0, 1, },
            {1, 1, 1, 0, 1, 1, 1, 0, 0, },
            {1, 1, 0, 0, 1, 0, 1, 1, 1, },
            {0, 0, 1, 1, 1, 0, 1, 1, 1, },
            {1, 1, 1, 0, 1, 0, 0, 1, 1, },
            {1, 1, 1, 1, 1, 0, 1, 0, 0, },
            {1, 1, 0, 1, 1, 0, 1, 0, 1, },
            {0, 0, 1, 1, 1, 1, 1, 0, 1, },
            {1, 1, 1, 0, 1, 1, 0, 0, 1, },
            {1, 1, 1, 1, 1, 1, 1, 0, 0, },
            {1, 1, 0, 1, 1, 0, 1, 1, 1, },
            {0, 0, 1, 1, 1, 1, 1, 1, 1, },
            {1, 1, 1, 0, 1, 1, 0, 1, 1, },
            {1, 0, 1, 0, 1, 0, 1, 0, 1, },
            {1, 0, 1, 0, 1, 0, 1, 0, 1, },
            {1, 0, 1, 0, 1, 0, 1, 0, 1, },
            {1, 0, 1, 0, 1, 0, 1, 0, 1, },
            {1, 0, 1, 0, 1, 1, 1, 0, 1, },
            {1, 0, 1, 0, 1, 0, 1, 1, 1, },
            {1, 0, 1, 0, 1, 0, 1, 1, 1, },
            {1, 0, 1, 0, 1, 0, 1, 1, 1, },
            {1, 0, 1, 1, 1, 0, 1, 0, 1, },
            {1, 0, 1, 1, 1, 0, 1, 0, 1, },
            {1, 0, 1, 0, 1, 1, 1, 0, 1, },
            {1, 0, 1, 0, 1, 1, 1, 0, 1, },
            {1, 0, 1, 1, 1, 1, 1, 0, 1, },
            {1, 0, 1, 1, 1, 0, 1, 1, 1, },
            {1, 0, 1, 0, 1, 1, 1, 1, 1, },
            {1, 0, 1, 0, 1, 1, 1, 1, 1, },
            {1, 1, 1, 0, 1, 0, 1, 0, 1, },
            {1, 1, 1, 0, 1, 0, 1, 0, 1, },
            {1, 0, 1, 1, 1, 0, 1, 0, 1, },
            {1, 1, 1, 0, 1, 0, 1, 0, 1, },
            {1, 1, 1, 0, 1, 1, 1, 0, 1, },
            {1, 1, 1, 0, 1, 0, 1, 1, 1, },
            {1, 0, 1, 1, 1, 0, 1, 1, 1, },
            {1, 1, 1, 0, 1, 0, 1, 1, 1, },
            {1, 1, 1, 1, 1, 0, 1, 0, 1, },
            {1, 1, 1, 1, 1, 0, 1, 0, 1, },
            {1, 0, 1, 1, 1, 1, 1, 0, 1, },
            {1, 1, 1, 0, 1, 1, 1, 0, 1, },
            {1, 1, 1, 1, 1, 1, 1, 0, 1, },
            {1, 1, 1, 1, 1, 0, 1, 1, 1, },
            {1, 0, 1, 1, 1, 1, 1, 1, 1, },
            {1, 1, 1, 0, 1, 1, 1, 1, 1, },
            {1, 0, 1, 0, 1, 0, 1, 1, 0, },
            {1, 0, 0, 0, 1, 1, 1, 0, 1, },
            {0, 1, 1, 0, 1, 0, 1, 0, 1, },
            {1, 0, 1, 1, 1, 0, 0, 0, 1, },
            {1, 0, 1, 0, 1, 1, 1, 1, 0, },
            {1, 0, 0, 0, 1, 1, 1, 1, 1, },
            {0, 1, 1, 0, 1, 0, 1, 1, 1, },
            {1, 0, 1, 1, 1, 0, 0, 1, 1, },
            {1, 0, 1, 1, 1, 0, 1, 1, 0, },
            {1, 0, 0, 1, 1, 1, 1, 0, 1, },
            {0, 1, 1, 0, 1, 1, 1, 0, 1, },
            {1, 0, 1, 1, 1, 1, 0, 0, 1, },
            {1, 0, 1, 1, 1, 1, 1, 1, 0, },
            {1, 0, 0, 1, 1, 1, 1, 1, 1, },
            {0, 1, 1, 0, 1, 1, 1, 1, 1, },
            {1, 0, 1, 1, 1, 1, 0, 1, 1, },
            {1, 1, 1, 0, 1, 0, 1, 1, 0, },
            {1, 1, 0, 0, 1, 1, 1, 0, 1, },
            {0, 1, 1, 1, 1, 0, 1, 0, 1, },
            {1, 1, 1, 1, 1, 0, 0, 0, 1, },
            {1, 1, 1, 0, 1, 1, 1, 1, 0, },
            {1, 1, 0, 0, 1, 1, 1, 1, 1, },
            {0, 1, 1, 1, 1, 0, 1, 1, 1, },
            {1, 1, 1, 1, 1, 0, 0, 1, 1, },
            {1, 1, 1, 1, 1, 0, 1, 1, 0, },
            {1, 1, 0, 1, 1, 1, 1, 0, 1, },
            {0, 1, 1, 1, 1, 1, 1, 0, 1, },
            {1, 1, 1, 1, 1, 1, 0, 0, 1, },
            {1, 1, 1, 1, 1, 1, 1, 1, 0, },
            {1, 1, 0, 1, 1, 1, 1, 1, 1, },
            {0, 1, 1, 1, 1, 1, 1, 1, 1, },
            {1, 1, 1, 1, 1, 1, 0, 1, 1, },
            {1, 0, 1, 0, 1, 0, 1, 1, 1, },
            {1, 0, 1, 0, 1, 1, 1, 0, 1, },
            {1, 1, 1, 0, 1, 0, 1, 0, 1, },
            {1, 0, 1, 1, 1, 0, 1, 0, 1, },
            {1, 0, 1, 0, 1, 1, 1, 1, 1, },
            {1, 0, 1, 0, 1, 1, 1, 1, 1, },
            {1, 1, 1, 0, 1, 0, 1, 1, 1, },
            {1, 0, 1, 1, 1, 0, 1, 1, 1, },
            {1, 0, 1, 1, 1, 0, 1, 1, 1, },
            {1, 0, 1, 1, 1, 1, 1, 0, 1, },
            {1, 1, 1, 0, 1, 1, 1, 0, 1, },
            {1, 0, 1, 1, 1, 1, 1, 0, 1, },
            {1, 0, 1, 1, 1, 1, 1, 1, 1, },
            {1, 0, 1, 1, 1, 1, 1, 1, 1, },
            {1, 1, 1, 0, 1, 1, 1, 1, 1, },
            {1, 0, 1, 1, 1, 1, 1, 1, 1, },
            {1, 1, 1, 0, 1, 0, 1, 1, 1, },
            {1, 1, 1, 0, 1, 1, 1, 0, 1, },
            {1, 1, 1, 1, 1, 0, 1, 0, 1, },
            {1, 1, 1, 1, 1, 0, 1, 0, 1, },
            {1, 1, 1, 0, 1, 1, 1, 1, 1, },
            {1, 1, 1, 0, 1, 1, 1, 1, 1, },
            {1, 1, 1, 1, 1, 0, 1, 1, 1, },
            {1, 1, 1, 1, 1, 0, 1, 1, 1, },
            {1, 1, 1, 1, 1, 0, 1, 1, 1, },
            {1, 1, 1, 1, 1, 1, 1, 0, 1, },
            {1, 1, 1, 1, 1, 1, 1, 0, 1, },
            {1, 1, 1, 1, 1, 1, 1, 0, 1, },
            {1, 1, 1, 1, 1, 1, 1, 1, 1, },
            {1, 1, 1, 1, 1, 1, 1, 1, 1, },
            {1, 1, 1, 1, 1, 1, 1, 1, 1, },
            {1, 1, 1, 1, 1, 1, 1, 1, 1, }
    };
    //conditional thinning
    int CT[46][3][3]={
            {0, 1, 0, 0, 1, 1, 0, 0, 0},
            {0, 1, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 0, 0, 1, 0},
            {0, 0, 0, 0, 1, 1, 0, 1, 0},
            {0, 0, 1, 0, 1, 1, 0, 0, 1},
            {1, 1, 1, 0, 1, 0, 0, 0, 0},
            {1, 0, 0, 1, 1, 0, 1, 0, 0},
            {0, 0, 0, 0, 1, 0, 1, 1, 1},
            {1, 1, 0, 0, 1, 1, 0, 0, 0},
            {0, 1, 0, 0, 1, 1, 0, 0, 1},
            {0, 1, 1, 1, 1, 0, 0, 0, 0},
            {0, 0, 1, 0, 1, 1, 0, 1, 0},
            {0, 1, 1, 0, 1, 1, 0, 0, 0},
            {1, 1, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 0, 1, 1, 0},
            {0, 0, 0, 0, 1, 1, 0, 1, 1},
            {1, 1, 0, 0, 1, 1, 0, 0, 1},
            {0, 1, 1, 1, 1, 0, 1, 0, 0},
            {1, 1, 1, 0, 1, 1, 0, 0, 0},
            {0, 1, 1, 0, 1, 1, 0, 0, 1},
            {1, 1, 1, 1, 1, 0, 0, 0, 0},
            {1, 1, 0, 1, 1, 0, 1, 0, 0},
            {1, 0, 0, 1, 1, 0, 1, 1, 0},
            {0, 0, 0, 1, 1, 0, 1, 1, 1},
            {0, 0, 0, 0, 1, 1, 1, 1, 1},
            {0, 0, 1, 0, 1, 1, 0, 1, 1},
            {1, 1, 1, 0, 1, 1, 0, 0, 1},
            {1, 1, 1, 1, 1, 0, 1, 0, 0},
            {1, 0, 0, 1, 1, 0, 1, 1, 1},
            {0, 0, 1, 0, 1, 1, 1, 1, 1},
            {0, 1, 1, 0, 1, 1, 0, 1, 1},
            {1, 1, 1, 1, 1, 1, 0, 0, 0},
            {1, 1, 0, 1, 1, 0, 1, 1, 0},
            {0, 0, 0, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 0, 1, 1, 0, 1, 1},
            {0, 1, 1, 0, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 0, 0},
            {1, 1, 1, 1, 1, 1, 0, 0, 1},
            {1, 1, 1, 1, 1, 0, 1, 1, 0},
            {1, 1, 0, 1, 1, 0, 1, 1, 1},
            {1, 0, 0, 1, 1, 1, 1, 1, 1},
            {0, 0, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 0, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 0, 1},
            {1, 1, 1, 1, 1, 0, 1, 1, 1},
            {1, 0, 1, 1, 1, 1, 1, 1, 1}
    };

    int M3[height][width][1];
    int M2[height + 2][width + 2][1];
    int M1[height][width][1];

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (Imagedata[i][j][0] > 128) {
                Imagedata[i][j][0] = (int) 1;
            } else {
                Imagedata[i][j][0] = (int) 0;
            }
        }
    }

    /*for (int i = 1; i < height ; i++) {
        for (int j = 1; j < width ; j++) {
            cout<< (unsigned int)Imagedata[i][j][0] << " ";
        }
    }*/

    int G[height][width][1];

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            G[i][j][0]= (int) Imagedata[i][j][0];
        }
    }
    /*for (int i = 1; i < height ; i++) {
        for (int j = 1; j < width ; j++) {
            cout<< (int) G[i][j][0] << " ";
        }
    }*/

    int f=0;
    while(f==0) {

        //copying matrix without boundary
        for (int k = 0; k < BytesPerPixel; k++) {
            for (int i = 1; i < height + 1; i++) {
                for (int j = 1; j < width + 1; j++) {
                    ImageBounddata[i][j][k] =  (G[i - 1][j - 1][k]);
                }
            }
        }
        //adding boundary columns
        for (int k = 0; k < BytesPerPixel; k++) {
            for (int i = 0; i < height; i++) {
                ImageBounddata[i + 1][0][k] =  (G[i][1][k]);
                ImageBounddata[i + 1][width + 1][k] =  (G[i][width - 2][k]);
            }
        }
        //adding boundary rows
        for (int k = 0; k < BytesPerPixel; k++) {
            for (int j = 0; j < width + 2; j++) {
                ImageBounddata[0][j][k] = ImageBounddata[2][j][k];
                ImageBounddata[height + 1][j][k] = ImageBounddata[height - 1][j][k];
            }
        }


        int m = 1;
        int window = 3;

        //unsigned int M1[height][width][0];


        for (int i = m; i < height + m; i++) {
            for (int j = m; j < width + m; j++) {
                int flag1 = 1;
                for (int q = 0; q < 46; q++) {
                    int flag2 = 1;
                    for (int k = 0; k < window; k++) {
                        for (int l = 0; l < window; l++) {
                            if (CT[q][k][l] == ImageBounddata[i - m + k][j - m + l][0]) {
                                if (k == 2 && l == 2) {
                                    M1[i - m][j - m][0] = 1;
                                    //cout<< M1[i - m][j - m][0]<<" "<< q<< endl;
                                    flag1=0;
                                    break;}
                            } else {
                                M1[i - m][j - m][0] = 0;
                                //cout<< S[q][k][l]<<" S"<< ImageBounddata[i - m + k][j - m + l][0]<< " I ";
                                //cout<< l<< " l" << k << "k" << " ";
                                flag2 = 0;
                                break;

                            }
                        }
                        if (flag2 == 0) break;
                        if (flag1 == 0) break;
                    }
                    if (flag1 == 0) break;
                }

            }

        }

        /*for (int i = m; i < height + m; i++) {
            for (int j = m; j < width + m; j++) {
                cout<< M1[i - m][j - m][0]<<" ";
            }
        }*/


        //copying matrix without boundary
        for (int k = 0; k < BytesPerPixel; k++) {
            for (int i = 1; i < height + 1; i++) {
                for (int j = 1; j < width + 1; j++) {
                    M2[i][j][k] = M1[i - 1][j - 1][k];
                    //cout<< M1[i - 1][j - 1][k]<< " " ;
                }
            }
        }
        //adding boundary columns
        for (int k = 0; k < BytesPerPixel; k++) {
            for (int i = 0; i < height; i++) {
                M2[i + 1][0][k] = M1[i][1][k];
                M2[i + 1][width + 1][k] = M1[i][width - 2][k];
            }
        }
        //adding boundary rows
        for (int k = 0; k < BytesPerPixel; k++) {
            for (int j = 0; j < width + 2; j++) {
                M2[0][j][k] = M2[2][j][k];
                M2[height + 1][j][k] = M2[height - 1][j][k];
            }
        }





        for (int i = m; i < height + m; i++) {
            for (int j = m; j < width + m; j++) {
                int flag1 = 1;
                for (int q = 0; q < 332; q++) {
                    int flag2 = 1;
                    for (int k = 0; k < window; k++) {
                        for (int l = 0; l < window; l++) {
                            if (UT[q][k][l] == M2[i - m + k][j - m + l][0]) {
                                if (k == 2 && l == 2) {
                                    M3[i - m][j - m][0] = 1;
                                    //cout<< M3[i - m][j - m][0]<<" "<< q<< endl;
                                    flag1 = 0;
                                    break;
                                }
                            }
                            else {
                                M3[i - m][j - m][0] = 0;
                                //cout<< S[q][k][l]<<" S"<< ImageBounddata[i - m + k][j - m + l][0]<< " I ";
                                //cout<< l<< " l" << k << "k" << " ";
                                flag2 = 0;
                                break;
                            }
                        }

                        if (flag2 == 0) break;
                        if (flag1 == 0) break;
                    }
                    if (flag1 == 0) break;
                }

            }

        }

        //unsigned int G[height][width][0] ;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                G[i][j][0] = (G[i][j][0] and ((not(M1[i][j][0])) or M3[i][j][0]));
                //cout<< M3[i][j][0]<< endl;
                //cout<< (ImageBounddata[i][j][0] && ((!(M1[i][j][0])) || M3[i][j][0]))<< " Op "<< ImageBounddata[i][j][0]<<" I "<<M1[i][j][0]<<" M1 "<<M3[i][j][0]<<" M3  "<< endl;
            }

        }
        int flag1 = 1;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (G[i][j][0] == ImageBounddata[i+1][j+1][0]) {
                    if (i == (height - 1) && j == (width - 1)) {
                        f = 1;
                    }
                }
                else{
                    flag1 = 0;
                    break;
                }

            }
            if (flag1 == 0) break;
        }


    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if(G[i][j][0]==1){
                Out[i][j][0]=(unsigned char) (255);
            }
            if(G[i][j][0]==0){
                Out[i][j][0]=(unsigned char) (0);
            }
        }
    }


    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Out, sizeof(unsigned char), (width)*(height)*1, file);
    fclose(file);
    return 0;
}
