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

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);

    fclose(file);

    unsigned char ImageBounddata[height+2][width+2];
    int i;
    int j;

    //copying matrix without boundary
    for(i=1; i<height+1; i++) {
        for (j = 1; j < width+1; j++) {
            ImageBounddata[i][j] = Imagedata[i - 1][j - 1][0];
        }
    }
    //adding boundary columns
    for( i=0; i<height; i++) {
        ImageBounddata[i + 1][0] = Imagedata[i][1][0];
        ImageBounddata[i + 1][width + 1] = Imagedata[i][width-2][0];
    }
    //adding boundary rows
    for( j=0; j<width+2; j++) {
        ImageBounddata[0][j] = ImageBounddata[2][j];
        ImageBounddata[height + 1][j] = ImageBounddata[height-1][j];
    }

    unsigned char ImageOutdata[height][width][3];

    for( i=1; i<height+1; i++){
        int rem_i = (i-1)%2;
        for( j=1; j< width+1; j++){
            int rem_j = (j-1)%2;
            if(rem_i == 0){
                if(rem_j == 0){
                    ImageOutdata[i-1][j-1][0]= (ImageBounddata[i][j-1] + ImageBounddata[i][j+1])/2 ;
                    ImageOutdata[i-1][j-1][1]= ImageBounddata[i][j] ;
                    ImageOutdata[i-1][j-1][2]= (ImageBounddata[i-1][j] + ImageBounddata[i+1][j])/2 ;
                }
                else{
                    ImageOutdata[i-1][j-1][0]= ImageBounddata[i][j] ;
                    ImageOutdata[i-1][j-1][1]= (ImageBounddata[i][j-1] + ImageBounddata[i-1][j] + ImageBounddata[i][j+1] + ImageBounddata[i+1][j])/4;
                    ImageOutdata[i-1][j-1][2]= (ImageBounddata[i-1][j-1] + ImageBounddata[i-1][j+1] +ImageBounddata[i+1][j-1] + ImageBounddata[i+1][j+1])/4;
                }
            }
            else{
                if(rem_j == 0){
                    ImageOutdata[i-1][j-1][0]= (ImageBounddata[i-1][j-1] + ImageBounddata[i+1][j-1] + ImageBounddata[i-1][j+1] + ImageBounddata[i+1][j+1])/4 ;
                    ImageOutdata[i-1][j-1][1]= (ImageBounddata[i][j-1] +ImageBounddata[i][j+1] + ImageBounddata[i-1][j] + ImageBounddata[i+1][j])/4 ;
                    ImageOutdata[i-1][j-1][2]= ImageBounddata[i][j] ;
                }
                else{
                    ImageOutdata[i-1][j-1][0]= (ImageBounddata[i-1][j]+ ImageBounddata[i+1][j])/2 ;
                    ImageOutdata[i-1][j-1][1]= ImageBounddata[i][j];
                    ImageOutdata[i-1][j-1][2]= (ImageBounddata[i][j-1] + ImageBounddata[i][j+1])/2;
                }
            }
        }
    }


    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(ImageOutdata, sizeof(unsigned char), width*height*3, file);
    fclose(file);

    return 0;
}