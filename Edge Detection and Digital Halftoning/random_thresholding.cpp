//Lubdha Pimpale
//lpimpale@usc.edu
//Date: 12 Feb 2019

#include <iostream>
#include <stdlib.h>

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
    unsigned char F[height][width][BytesPerPixel];
    unsigned char G[height][width][BytesPerPixel];
    unsigned char RV[height][width][BytesPerPixel];


    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(F, sizeof(unsigned char), height*width*BytesPerPixel, file);
    fclose(file);

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            RV[i][j][0] = rand() % 256;
        }
    }


    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            if(F[i][j][0]< RV[i][j][0]){
                G[i][j][0]=0;
            }
            else{
                G[i][j][0]=255;
            }
        }
    }

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(G, sizeof(unsigned char), height*width*BytesPerPixel, file);
    fclose(file);

    return 0;
}
