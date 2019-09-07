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

    unsigned char ImageBounddata[height+4][width+4][BytesPerPixel];
    int i;
    int j;
    //copying matrix without boundary
    for(int q=0; q<BytesPerPixel; q++) {
        for (i = 2; i < height + 2; i++) {
            for (j = 2; j < width + 2; j++) {
                ImageBounddata[i][j][q] = Imagedata[i - 2][j - 2][q];
            }
        }
    }
    //adding boundary columns
    for(int q=0; q<BytesPerPixel; q++) {
        for (int i = 0; i < height; i++) {
            ImageBounddata[i + 2][0][q] = Imagedata[i][2][q];
            ImageBounddata[i + 2][1][q] = Imagedata[i][1][q];
            ImageBounddata[i + 2][width + 2][q] = Imagedata[i][width - 2][q];
            ImageBounddata[i + 2][width + 3][q] = Imagedata[i][width - 3][q];
        }
    }
    //adding boundary rows
    for(int q=0; q<BytesPerPixel; q++) {
        for (int j = 0; j < width; j++) {
            ImageBounddata[0][j][q] = ImageBounddata[4][j][q];
            ImageBounddata[1][j][q] = ImageBounddata[3][j][q];
            ImageBounddata[height + 2][j][q] = ImageBounddata[height][j][q];
            ImageBounddata[height + 3][j][q] = ImageBounddata[height - 1][j][q];
        }
    }

    int m=2;
    float Error[height+4][width+4];

    unsigned int filterl2r[5][5]={{0,0,0,0,0},{0,0,0,0,0},{0,0,0,7,5},{3,5,7,5,3},{1,3,5,3,1}};
    unsigned int filterr2l[5][5]={{0,0,0,0,0},{0,0,0,0,0},{5,7,0,0,0},{3,5,7,5,3},{1,3,5,3,1}};

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
                            ImageBounddata[i + k][j + l][q] += (filterl2r[k + m][l + m] * Error[i][j] / 48);

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
                            ImageBounddata[i + k][j + l][q] += (filterr2l[k + m][l + m] * Error[i][j] / 48);
                        }
                    }
                }
            }

        }
    }

    for(int q=0; q<BytesPerPixel; q++) {
        for (i = 2; i < height + 2; i++) {
            for (j = 2; j < width + 2; j++) {
                ImageOutdata[i - 2][j - 2][q] = ImageBounddata[i][j][q];
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
