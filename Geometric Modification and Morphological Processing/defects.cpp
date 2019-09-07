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
    int ImageBounddata[height][width][BytesPerPixel];


    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), width * height * BytesPerPixel, file);

    fclose(file);

    //copying matrix without boundary

    for (int i = 0; i < height ; i++) {
        for (int j = 0; j < width ; j++) {
            ImageBounddata[i][j][0] =  (Imagedata[i - 1][j - 1][0]);
        }
    }

/*    //adding boundary columns
    for (int k = 0; k < BytesPerPixel; k++) {
        for (int i = 0; i < height; i++) {
            ImageBounddata[i + 1][0][k] =  (Imagedata[i][1][k]);
            ImageBounddata[i + 1][width + 1][k] =  (Imagedata[i][width - 2][k]);
        }
    }
    //adding boundary rows
    for (int k = 0; k < BytesPerPixel; k++) {
        for (int j = 0; j < width + 2; j++) {
            ImageBounddata[0][j][k] = ImageBounddata[2][j][k];
            ImageBounddata[height + 1][j][k] = ImageBounddata[height - 1][j][k];
        }
    }*/


    int mask[3][3] ={{255,255,255}, {255,0,255},{255,255,255}};
    int m=1;
    int window=3;
    int def=0;
    int flag1;

    for (int i = m; i < height-m; i++) {
        for (int j = m; j < width-m; j++) {
            flag1 = 1;
            for (int k = 0; k < window; k++) {
                for (int l = 0; l < window; l++) {
                    if (mask[k][l] == ImageBounddata[i -m+ k][j -m+ l][0]) {
                        if( (k == 2) and (l == 2)) {
                            def += 1;
                            ImageBounddata[i -m+ 1][j -m+ +1][0]= 255;
                        }
                    }
                    else {
                        flag1 = 0;
                        break;

                    }
                }
                if (flag1 == 0) break;
            }
        }

    }

    cout<< def<<" defects"<< endl;


    for (int i = 0; i < height ; i++) {
        for (int j = 0; j < width ; j++) {
            Out[i][j][0] =  (ImageBounddata[i - 1][j - 1][0]);
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
