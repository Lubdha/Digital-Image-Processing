//Lubdha Pimpale
//lpimpale@usc.edu
//Date: 12 Feb 2019

#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <math.h>

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
    float ImageOutdata[height][width];
    unsigned char FinalImage[height][width];
    unsigned char x_grad_edge[height][width];
    unsigned char y_grad_edge[height][width];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
    fclose(file);

    float ImageBounddata[height+2][width+2];

    //converting RGB to graycscale and copying matrix without boundary
    for (int i = 1; i < height + 1; i++) {
        for (int j = 1; j < width + 1; j++) {
            ImageBounddata[i][j] = (float)(0.3*Imagedata[i - 1][j - 1][0])+(0.59*Imagedata[i - 1][j - 1][1])+(0.11*Imagedata[i - 1][j - 1][2]);
        }
    }

    //adding boundary columns
    for (int i = 1; i < height+1; i++) {
        ImageBounddata[i][0] = ImageBounddata[i][2];
        ImageBounddata[i][width + 1] = ImageBounddata[i][width - 1];
    }

    //adding boundary rows
    for (int j = 0; j < width + 2; j++) {
        ImageBounddata[0][j] = ImageBounddata[2][j];
        ImageBounddata[height + 1][j] = ImageBounddata[height - 1][j];
    }

    int total_pixels= height*width;
    int m=1;
    int gx[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
    int gy[3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};
    float g[total_pixels];
    float x_grad[total_pixels];
    float y_grad[total_pixels];


    for (int i = m; i < height + m; i++) {
        for (int j = m; j < width + m; j++) {
            float temp_gx=0.0;
            float temp_gy=0.0;
            for (int k = -m; k < m + 1; k++) {
                for (int l = -m; l < m + 1; l++) {
                    temp_gx += gx[k + m][l + m] * ImageBounddata[i+k][j+l];
                    temp_gy += gy[k + m][l + m] * ImageBounddata[i+k][j+l];
                }
            }
            ImageOutdata[i-m][j-m] = (int)((pow((pow((temp_gx),2)+ pow((temp_gy),2)),.5)));
            g[(i-m)*width+j-m] = ImageOutdata[i-m][j-m];
            x_grad[(i-m)*width+j-m] = temp_gx;
            y_grad[(i-m)*width+j-m] = temp_gy;

        }

    }


    sort(g, g+ total_pixels);
    float x_grad_min=200;
    float x_grad_max=0;
    float y_grad_min=200;
    float y_grad_max=0;
    int g_min=200;
    int g_max=0;

    for(int i=0; i< total_pixels; i++){
        if(x_grad[i]< x_grad_min) {
            x_grad_min= x_grad[i];
        }
        if(x_grad[i]> x_grad_max) {
            x_grad_max= x_grad[i];
        }
    }


    for(int i=0; i< total_pixels; i++){
        if(y_grad[i]< y_grad_min) {
            y_grad_min= y_grad[i];
        }
        if(y_grad[i]> y_grad_max) {
            y_grad_max= y_grad[i];
        }
    }

    for(int i=0; i< total_pixels; i++){
        if(g[i]< g_min) {
            g_min= g[i];
        }
        if(g[i]> g_max) {
            g_max= g[i];
        }
    }
    //cout << g_max<< endl;
    //cout << g_min<< endl;


    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            x_grad_edge[i-m][j-m] = (unsigned char)(((x_grad[(i-m)*width+j-m] - x_grad_min)/(x_grad_max - x_grad_min))*255.0) ;
            y_grad_edge[i-m][j-m] = (unsigned char)(((y_grad[(i-m)*width+j-m] - y_grad_min)/(y_grad_max - y_grad_min))*255.0) ;
        }
    }



    float hist[2][g_max];
    for (int i = 0; i < g_max+1; i++) {
        hist[0][i] = i;
        hist[1][i] = 0;
    }

    //histogram
    for (int i = 0; i < total_pixels; i++) {
        for ( int k = 0; k < g_max+1; k++) {
            if (g[i] == hist[0][k]) {
                hist[1][k] = hist[1][k] + 1;
                break;
            }
        }

    }


    //Dividing by pixels
    for (int k = 0; k < g_max+1; k++) {
        hist[1][k]= (float) (hist[1][k]/total_pixels);
    }


    //cdf
    for (int k = 1; k < g_max+1; k++) {
        hist[1][k] = (hist[1][k] + hist[1][k - 1]);
    }


 /*   for(int i=0; i<g_max+1; i++) {
      cout << hist[1][i]<< endl;
    }*/


    float threshold= 0.8;
    float g_threshold;

    for (int k = 0; k< g_max; k++) {
        if(hist[1][k] >= threshold){
            g_threshold = k;
            break;
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (ImageOutdata[i][j] > g_threshold) {
                FinalImage[i][j] = 0;
            } else {
                FinalImage[i][j] = 255;
            }
        }
    }

    if (!(file=fopen(argv[6],"wb"))) {
        cout << "Cannot open file: " << argv[6] << endl;
        exit(1);
    }
    fwrite(x_grad_edge, sizeof(unsigned char), (height)*(width), file);
    fclose(file);

    if (!(file=fopen(argv[7],"wb"))) {
        cout << "Cannot open file: " << argv[7] << endl;
        exit(1);
    }
    fwrite(y_grad_edge, sizeof(unsigned char), (height)*(width), file);
    fclose(file);

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(FinalImage, sizeof(unsigned char), (height)*(width), file);
    fclose(file);
    return 0;
}
