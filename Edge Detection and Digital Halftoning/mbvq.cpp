//Lubdha Pimpale
//lpimpale@usc.edu
//Date: 12 Feb 2019

#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <vector>
#include <string>
#include <initializer_list>

using namespace std;

string getMBVQ(int R,int G,int B);
string getNearestVertex(string mbvq, int R,int G,int B);
vector<int> GiveVector(string vertex);

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

    float ImageBounddata[height+2][width+2][BytesPerPixel];
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
    float Error[height+2][width+2][BytesPerPixel];

    unsigned int filterl2r[3][3]={{0,0,0},{0,0,7},{3,5,1}};
    unsigned int filterr2l[3][3]={{0,0,0},{7,0,0},{1,5,3}};
    std::vector<int> v;

    for (int i = m; i < height + m; i++) {
        if (i % 2 == 0) {
            for (int j = m; j < width + m; j++) {

                string mbvq = getMBVQ(Imagedata[i-m][j-m][0],Imagedata[i-m][j-m][1],Imagedata[i-m][j-m][2]);
                string vertex = getNearestVertex(mbvq, ImageBounddata[i][j][0],ImageBounddata[i][j][1],ImageBounddata[i][j][2]);
                v = GiveVector(vertex);

                for(int q=0; q<BytesPerPixel; q++) {
                    Error[i][j][q]= ImageBounddata[i][j][q] - v[q];
                    ImageOutdata[i - m][j - m][q] = v[q];
                    for (int k = -m; k < m + 1; k++) {
                        for (int l = -m; l < m + 1; l++) {
                            ImageBounddata[i + k][j + l][q] += (filterl2r[k + m][l + m] * Error[i][j][q] / 16);

                        }
                    }
                }
            }
        }
        else {
            for (int j = width + m - 1; j > m - 1; j--) {

                string mbvq = getMBVQ(Imagedata[i-m][j-m][0],Imagedata[i-m][j-m][1],Imagedata[i-m][j-m][2]);
                string vertex = getNearestVertex(mbvq, ImageBounddata[i][j][0],ImageBounddata[i][j][1],ImageBounddata[i][j][2]);
                v = GiveVector(vertex);

                for(int q=0; q<BytesPerPixel; q++) {
                    Error[i][j][q]= ImageBounddata[i][j][q] - v[q];
                    ImageOutdata[i - m][j - m][q] = v[q];
                    for (int k = -m; k < m + 1; k++) {
                        for (int l = -m; l < m + 1; l++) {
                            ImageBounddata[i + k][j + l][q] += (filterr2l[k + m][l + m] * Error[i][j][q] / 16);
                        }
                    }
                }
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

string getMBVQ(int R,int G,int B) {
    if ((R + G) > 255) {
        if ((G + B) > 255) {
            if ((R + G + B) > 510) {
                return "CMYW";
            } else {
                return "MYGC";
            }
        } else {
            return "RGMY";
        }
    } else {
        if (!((G + B) > 255)) {
            if (!((R + G + B) > 255)) {
                return "KRGB";
            } else {
                return "RGBM";
            }
        } else {
            return "CMGB";
        }

    }
}

vector<int> GiveVector(string vertex){
    vector<int> v;
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    if(vertex == "black"){
        v[0]=0;
        v[1]=0;
        v[2]=0;
    }
    else if(vertex == "blue"){
        v[0]=0;
        v[1]=0;
        v[2]=255;
    }
    else if(vertex == "green"){
        v[0]=0;
        v[1]=255;
        v[2]=0;
    }
    else if(vertex == "red"){
        v[0]=255;
        v[1]=0;
        v[2]=0;
    }
    else if(vertex == "cyan"){
        v[0]=0;
        v[1]=255;
        v[2]=255;
    }
    else if(vertex == "magenta"){
        v[0]=255;
        v[1]=0;
        v[2]=255;
    }
    else if(vertex == "yellow"){
        v[0]=255;
        v[1]=255;
        v[2]=0;
    }
    else if(vertex == "white"){
        v[0]=255;
        v[1]=255;
        v[2]=255;
    }
    return v;

}


string getNearestVertex(string mbvq,int R,int G,int B){
    string vertex;
    // No.1 for CMYW
    if (mbvq == "CMYW"){
        vertex = "white";
        if (B < 128){
            if (B <= R){
                if (B <= G){
                    vertex = "yellow";
                }
            }
        }

        if (G < 128){
            if (G <= B){
                if (G <= R){
                    vertex = "magenta";
                }
            }
        }

        if (R < 128){
            if (R <= B){
                if (R <= G){
                    vertex = "cyan";
                }
            }
        }
        return vertex;
    }

    // No.2 for MYGC
    if (mbvq == "MYGC") {
        vertex = "magenta";
        if (G >= B) {
            if (R >= B) {
                if (R >= 128) {
                    vertex = "yellow";
                }
                else {
                    vertex = "green";
                }
            }
        }

        if (G >= R) {
            if (B >= R) {
                if (B >= 128) {
                    vertex = "cyan";
                }
                else {
                    vertex = "green";
                }
            }
        }
        return vertex;
    }


    // No.3 for RGMY
    if (mbvq == "RGMY"){
        if (B > 128) {
            if (R > 128) {
                if (B >= G) {
                    vertex = "magenta";
                }
                else {
                    vertex = "yellow";
                }
            } else {
                if (G > B + R) {
                    vertex = "green";
                }
                else {
                    vertex = "magenta";
                }
            }
        }
        else {
            if (R >= 128) {
                if (G >= 128) {
                    vertex = "yellow";
                }
                else {
                    vertex = "red";
                }
            } else {
                if (R >= G) {
                    vertex = "red";
                }
                else {
                    vertex = "green";
                }
            }
        }
        return vertex;
    }


    // No.4 for KRGB
    if (mbvq == "KRGB"){
        vertex = "black";
        if (B > 128){
            if (B >= R){
                if (B >= G){
                    vertex = "blue";
                }
            }
        }
        if (G > 128){
            if (G >= B){
                if (G >= R){
                    vertex = "green";
                }
            }
        }
        if (R > 128){
            if (R >= B){
                if (R >= G){
                    vertex = "red";
                }
            }
        }
        return vertex;
    }

    // No.5 for RGBM
    if (mbvq == "RGBM"){
        vertex = "green";
        if (R > G){
            if (R >= B){
                if (B < 128){
                    vertex = "red";
                }
                else{
                    vertex = "magenta";
                }
            }
        }
        if (B > G){
            if (B >= R){
                if (R < 128){
                    vertex = "blue";
                }
                else{
                    vertex = "magenta";
                }
            }
        }
        return vertex;
    }


    // No.6 for CMGB
    if (mbvq == "CMGB"){
        if (B > 128) {
            if (R > 128) {
                if (G >= R) {
                    vertex = "cyan";
                } else {
                    vertex = "magenta";
                }
            }
            else{
                if (G > 128) {
                    vertex = "cyan";
                }
                else {
                    vertex = "blue";

                }
            }

        }
        else{
            if ( R > 128) {
                if (R - G + B >= 128) {
                    vertex = "magenta";
                }
                else {
                    vertex = "green";
                }
            }
            else{
                if (G >= B){
                    vertex = "green";
                }
                else{
                    vertex = "blue";
                }
            }
        }
        return vertex;
    }
}
