//Lubdha Pimpale
//email- lpimpale@usc.edu
//date- 22 Jan 2019

// This sample code reads in image data from a RAW image file and
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel;
    int width = 390;
    int height = 300;

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
            height = atoi(argv[4]);
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

    unsigned char ImageBounddata[height+4][width+4];
    int i;
    int j;
    //copying matrix without boundary
    for(i=2; i<height+2; i++) {
        for (j = 2; j < width+2; j++) {
            ImageBounddata[i][j] = Imagedata[i - 2][j - 2][0];
        }
    }
    //adding boundary columns
    for(int i=0; i<height; i++) {
        ImageBounddata[i + 2][0] = Imagedata[i][2][0];
        ImageBounddata[i + 2][1] = Imagedata[i][1][0];
        ImageBounddata[i + 2][width + 2] = Imagedata[i][width-2][0];
        ImageBounddata[i + 2][width + 3] = Imagedata[i][width-3][0];
    }
    //adding boundary rows
    for(int j=0; j<width; j++) {
        ImageBounddata[0][j] = ImageBounddata[4][j];
        ImageBounddata[1][j] = ImageBounddata[3][j];
        ImageBounddata[height + 2][j] = ImageBounddata[height][j];
        ImageBounddata[height + 3][j] = ImageBounddata[height - 1][j];
    }

    unsigned char ImageOutdata[height][width][3];

    for( i=2; i<height+2; i++){
        int rem_i = i%2;
        for( j=2; j< width+2; j++){
            int rem_j = j%2;
            if(rem_i == 0){
            //even rows
            // even columns
            if(rem_j == 0){
                ImageOutdata[i-2][j-2][0]= (5*ImageBounddata[i][j] + 4*ImageBounddata[i][j-1] + 4*ImageBounddata[i][j+1] - ImageBounddata[i][j-2]
                                            -ImageBounddata[i][j+2]- ImageBounddata[i-1][j-1]- ImageBounddata[i-1][j+1]- ImageBounddata[i+1][j-1]- ImageBounddata[i+1][j+1]
                                            +0.5*ImageBounddata[i-2][j]+ 0.5*ImageBounddata[i+2][j])/8 ;
                ImageOutdata[i-2][j-2][1]= ImageBounddata[i][j];
                ImageOutdata[i-2][j-2][2]=  (5*ImageBounddata[i][j] + 4*ImageBounddata[i-1][j] + 4*ImageBounddata[i+1][j] + 0.5*ImageBounddata[i][j-2]
                                             + 0.5*ImageBounddata[i][j+2]- ImageBounddata[i-1][j-1]- ImageBounddata[i-1][j+1]- ImageBounddata[i+1][j-1]- ImageBounddata[i+1][j+1]
                                             - ImageBounddata[i][j-2]- ImageBounddata[i][j+2])/8 ;
            }
            else{
                // odd columns
                ImageOutdata[i-2][j-2][0]= ImageBounddata[i][j] ;
                ImageOutdata[i-2][j-2][1]= (4*ImageBounddata[i][j] + 2*ImageBounddata[i-1][j] + 2*ImageBounddata[i+1][j] + 2*ImageBounddata[i][j-1]
                                            + 2*ImageBounddata[i][j+1] - ImageBounddata[i-2][j] - ImageBounddata[i+2][j] - ImageBounddata[i][j-2] - ImageBounddata[i][j+2])/8;
                ImageOutdata[i-2][j-2][2]= (6*ImageBounddata[i][j] + 2*ImageBounddata[i-1][j-1] + 2*ImageBounddata[i-1][j+1] + 2*ImageBounddata[i+1][j-1]
                                            + 2*ImageBounddata[i+1][j+1]- 1.5*ImageBounddata[i][j-2]- 1.5*ImageBounddata[i][j+2]- 1.5*ImageBounddata[i-2][j]
                                            - 1.5*ImageBounddata[i+2][j])/8 ;
            }
        }
        else{
            // odd rows
            if(rem_j == 0){
                // even columns
                ImageOutdata[i-2][j-2][0]= (6*ImageBounddata[i][j] + 2*ImageBounddata[i-1][j-1] + 2*ImageBounddata[i-1][j+1] + 2*ImageBounddata[i+1][j-1]
                                            + 2*ImageBounddata[i+1][j+1]- 1.5*ImageBounddata[i][j-2]- 1.5*ImageBounddata[i][j+2]- 1.5*ImageBounddata[i-2][j]
                                            - 1.5*ImageBounddata[i+2][j])/8 ;
                ImageOutdata[i-2][j-2][1]= (4*ImageBounddata[i][j] + 2*ImageBounddata[i-1][j] + 2*ImageBounddata[i+1][j] + 2*ImageBounddata[i][j-1]
                                            + 2*ImageBounddata[i][j+1] - ImageBounddata[i-2][j] - ImageBounddata[i+2][j] - ImageBounddata[i][j-2] - ImageBounddata[i][j+2])/8;
                ImageOutdata[i-2][j-2][2]= ImageBounddata[i][j];
            }
            else{
                // odd columns
                ImageOutdata[i-2][j-2][0]= (5*ImageBounddata[i][j] + 4*ImageBounddata[i-1][j] + 4*ImageBounddata[i+1][j] + 0.5*ImageBounddata[i][j-2]
                                            + 0.5*ImageBounddata[i][j+2]- ImageBounddata[i-1][j-1]- ImageBounddata[i-1][j+1]- ImageBounddata[i+1][j-1]- ImageBounddata[i+1][j+1]
                                            - ImageBounddata[i][j-2]- ImageBounddata[i][j+2])/8 ;
                ImageOutdata[i-2][j-2][1]= ImageBounddata[i][j];
                ImageOutdata[i-2][j-2][2]= (5*ImageBounddata[i][j] + 4*ImageBounddata[i][j-1] + 4*ImageBounddata[i][j+1] - ImageBounddata[i][j-2]
                                            - ImageBounddata[i][j+2]- ImageBounddata[i-1][j-1]- ImageBounddata[i-1][j+1]- ImageBounddata[i+1][j-1]- ImageBounddata[i+1][j+1]
                                            + 0.5*ImageBounddata[i][j-2]+ 0.5*ImageBounddata[i][j+2])/8 ;
            }
        }

    }

}

///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

// Write image data (filename specified by second argument) from image data matrix

if (!(file=fopen(argv[2],"wb"))) {
cout << "Cannot open file: " << argv[2] << endl;
exit(1);
}
fwrite(ImageOutdata, sizeof(unsigned char), width*height*3, file);
fclose(file);

return 0;
}
