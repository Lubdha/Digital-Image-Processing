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
#include <vector>
#include <fstream>

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
    //unsigned char EquilizedImage[height][width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), width * height * BytesPerPixel, file);

    fclose(file);


    float hist[2][256];
    for (int i = 0; i < 256; i++) {
        hist[0][i] = i;
        hist[1][i] = 0;
    }
    //histogram
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for ( int k = 0; k < 256; k++) {
                if (Imagedata[i][j][0] == hist[0][k]) {
                    hist[1][k] = hist[1][k] + 1;
                    break;
                }
            }
        }
    }

    //Dividing by pixels
    int total_pixels= height*width;
    for (int k = 0; k < 256; k++) {
        hist[1][k]= hist[1][k]/total_pixels;
    }


    //cdf
    for (int k = 1; k < 256; k++) {
        hist[1][k] = (hist[1][k] + hist[1][k - 1]);

    }


    //plotting transfer function
    ofstream transfer_func("cdf_hist.csv");
    if (transfer_func.is_open()){
        for(int j=0; j<256; j++){
            transfer_func<< hist[1][j] << endl;
        }
        transfer_func.close();
    }


    //typedef std::vector<int> int_vector;
    static std::vector<int> vect(total_pixels);
    int k;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            vect[i*width +j]= Imagedata[i][j][0];
        }
    }

    //sort vector
    std::sort (vect.begin(),vect.end());

    //no of pixels in each bucket
    int pixel_in_bucket = total_pixels / 256;

    //to keep track of a number of times a particular no occurred
    int new_array[256]={0};

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int temp=0;
            for( int k=0;k<total_pixels;k++){
                if(Imagedata[i][j][0] == vect[k]){
                    if(Imagedata[i][j][0] != (new_array[vect[k]]+k)/ pixel_in_bucket){
                        Imagedata[i][j][0] = (new_array[vect[k]]+k)/ pixel_in_bucket;
                        new_array[vect[k]] += 1;
                        break;
                    }
                }
            }
        }
    }

    for (int i = 0; i < 256; i++) {
        hist[0][i] = i;
        hist[1][i] = 0;
    }
    //equilized histogram
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 256; k++) {
                if (Imagedata[i][j][0] == hist[0][k]) {
                    hist[1][k] = hist[1][k] + 1;
                    break;
                }
            }
        }
    }
    ofstream equ_transfer_hist("equ_transfer_hist.csv");
    if (equ_transfer_hist.is_open()){
        for(int j=0; j<256; j++){
            equ_transfer_hist<< hist[1][j] << endl;
        }
        equ_transfer_hist.close();
    }
/*    //Dividing by pixels
    for (int k = 0; k < 256; k++) {
        hist[1][k]= hist[1][k]/total_pixels;
    }


    //cdf
    for (int k = 1; k < 256; k++) {
        hist[1][k] = (hist[1][k] + hist[1][k - 1]);
    }

    //plotting transfer function
    ofstream equ_cdf("cdf_hist_equ.csv");
    if (equ_cdf.is_open()){
        for(int j=0; j<256; j++){
            equ_cdf<< hist[1][j] << endl;
        }
        equ_cdf.close();
    }
*/
    if (!(file = fopen(argv[2], "wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Imagedata, sizeof(unsigned char), width * height *BytesPerPixel, file);
    fclose(file);
}