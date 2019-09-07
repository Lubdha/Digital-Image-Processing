//Lubdha Pimpale
//lpimpale@usc.edu
//Date: 12 Feb 2019

#include <iostream>
#include <stdlib.h>
#include <cstdio>

using namespace std;

int **recur(int**,int**,int,int);
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


    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(F, sizeof(unsigned char), height*width*BytesPerPixel, file);
    fclose(file);

    int m=2;
    int n=32;

    int A[2][2]= {{1,2},{3,0}};
    //create pointer of pointers
    int **I2;
    I2 = new int*[m];
    for(int row=0; row<m; row++){
        I2[row] = new int[m];
        for(int col=0; col<m; col++){
            I2[row][col] =  A[row][col];
        }
    }

    int** I = recur(I2,I2, m, n);

    int T[n][n];
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            T[i][j] = (I[i][j]+0.5)*255/(n*n);
        }
    }

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            if(F[i][j][0]<= T[i%n][j%n]){
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


int **recur(int **I,int **I2,int m,int n){
    if(n==m){
        return I;
    }
    else{
        int **G=0;
        G = new int*[2*m];
        for(int row=0; row<2*m; row++){
            G[row] = new int[2*m];
            for(int col=0; col<2*m; col++){
                G[row][col] =  0;
            }
        }
        for(int a=0; a<2; a++) {
            for (int b = 0; b < 2; b++) {
                for (int i = 0; i < m; i++) {
                    for (int j = 0; j < m; j++) {
                        G[ a*m + i][ b*m + j] = 4 * I[i][j] + I2[a][b];
                    }
                }
            }
        }
        m = 2*m;
        return recur(G,I2,m,n);
    }
}
