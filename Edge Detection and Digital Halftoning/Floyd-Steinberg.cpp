//Lubdha Pimpale
//lpimpale@usc.edu
//Date: 12 Feb 2019

#include <iostream>
#include <stdlib.h>
#include <cstdio>

using namespace std;

int main(int argc, char *argv[]) {

    FILE *file;
    int BytesPerPixel;
    int height;
    int width;

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
    unsigned char ImageOutdata[height][width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
    fclose(file);

    unsigned char ImageBounddata[height+2][width+2][BytesPerPixel];
    int i;
    int j;

    //copying matrix without boundary
    for(int k=0; k<BytesPerPixel; k++) {
        for (i = 1; i < height + 1; i++) {
            for (j = 1; j < width + 1; j++) {
                ImageBounddata[i][j][k] = Imagedata[i - 1][j - 1][k];
            }
        }
    }
    //adding boundary columns
    for(int k=0; k<BytesPerPixel; k++) {
        for (i = 0; i < height; i++) {
            ImageBounddata[i + 1][0][k] = Imagedata[i][1][k];
            ImageBounddata[i + 1][width + 1][k] = Imagedata[i][width - 2][k];
        }
    }
    //adding boundary rows
    for(int k=0; k<BytesPerPixel; k++) {
        for (j = 0; j < width + 2; j++) {
            ImageBounddata[0][j][k] = ImageBounddata[2][j][k];
            ImageBounddata[height + 1][j][k] = ImageBounddata[height - 1][j][k];
        }
    }


    int m=1;
    float Error[height+2][width+2];

    unsigned int filterl2r[3][3]={{0,0,0},{0,0,7},{3,5,1}};
    unsigned int filterr2l[3][3]={{0,0,0},{7,0,0},{1,5,3}};

    for(int q=0; q<BytesPerPixel; q++) {
        for (int i = m; i < height + m; i++) {
            if (i % 2 == 0) {
                for (int j = m; j < width + m; j++) {

                    if (ImageBounddata[i][j][q] < 128) {
                        Error[i][j] = ImageBounddata[i][j][q];
                        ImageBounddata[i][j][q] = 0;
                    } else {
                        Error[i][j] = ImageBounddata[i][j][q] - 255;
                        ImageBounddata[i][j][q] = 255;
                    }
                    for (int k = -m; k < m + 1; k++) {
                        for (int l = -m; l < m + 1; l++) {
                            ImageBounddata[i + k][j + l][q] += (filterl2r[k + m][l + m] * Error[i][j] / 16);

                        }
                    }
                }
            } else {
                for (int j = width + m - 1; j > m - 1; j--) {

                    if (ImageBounddata[i][j][q] < 128) {
                        Error[i][j] = ImageBounddata[i][j][q];
                        ImageBounddata[i][j][q] = 0;
                    } else {
                        Error[i][j] = ImageBounddata[i][j][q] - 255;
                        ImageBounddata[i][j][q] = 255;
                    }
                    for (int k = -m; k < m + 1; k++) {
                        for (int l = -m; l < m + 1; l++) {
                            ImageBounddata[i + k][j + l][q] += (filterr2l[k + m][l + m] * Error[i][j] / 16);
                        }
                    }
                }
            }

        }
    }

    for(int k=0; k<BytesPerPixel; k++) {
        for (i = m; i < height + m; i++) {
            for (j = m; j < width + m; j++) {
                ImageOutdata[i - m][j - m][k] = ImageBounddata[i][j][k];
            }
        }
    }

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(ImageOutdata, sizeof(unsigned char), height*width*BytesPerPixel, file);
    fclose(file);
    return 0;
}
