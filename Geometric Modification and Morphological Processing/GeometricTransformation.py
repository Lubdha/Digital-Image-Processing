#Lubdha Pimpale
#email- lpimpale@usc.edu
#date- 3 Mar 2019

import numpy as np
from matplotlib import pyplot as plt
import math
from numpy.linalg import pinv

def read_image(file, width, height):
    raw_input = open(file,'rb').read()
    raw_input = np.frombuffer(raw_input, np.uint8).copy()
    image = np.reshape(raw_input, (height,width))
    return image



def get_cartesian_coordinate(u,v):
    x= u-127
    y= 127- v
    return x,y

def get_image_coordinate(x,y):
    u= x+127
    v= 127- y
    return u,v

#lighthouse

lighthouse = read_image("lighthouse.raw", 512,512)
    #hole2 upper coordinate
def get_start_coordinate(start_x, start_y,end_x, end_y):
    for or_y in range(start_y,end_y):
        for or_x in range(start_x, end_x):
            if(lighthouse[or_y][or_x] == 255):
                #print( or_x, or_y)
                return or_x, or_y
        
#hole2 upper coordinate
def get_end_coordinate(start_x, start_y):
    for or_y in range(start_y,512):
        for or_x in range(start_x,512):
            if(lighthouse[or_y][or_x] != 255):
                #print( or_x-1, or_y)
                return or_x-1, or_y
        
hole2_x1, hole2_y1 = get_start_coordinate(0,0,512,512)
hole2_x2, hole2_y2 =get_end_coordinate(hole2_x1, hole2_y1)
hole1_x1, hole1_y1 = get_start_coordinate(0,0,hole2_x1, 512)
hole1_x2, hole1_y2 =get_end_coordinate(hole1_x1, hole1_y1)

#hole2 upper coordinate
def get_down_coordinate(start_x, start_y):
    for or_y in range(start_y,512):
        if(lighthouse[or_y][start_x] != 255):
            #print( start_x, or_y)
            return start_x, or_y

hole1_x3, hole1_y3 = get_down_coordinate(hole1_x1, hole1_y1)
hole1_x4, hole1_y4 = get_down_coordinate(hole1_x2, hole1_y2)

hole2_x3, hole2_y3 = get_down_coordinate(hole2_x1, hole2_y1)
hole2_x4, hole2_y4 = get_down_coordinate(hole2_x2, hole2_y2)

hole3_x1, hole3_y1 = get_start_coordinate(hole1_x4+1,hole1_y4+1,512,512)
hole3_x2, hole3_y2 =get_end_coordinate(hole3_x1, hole3_y1)

hole3_x3, hole3_y3 = get_down_coordinate(hole3_x1, hole3_y1)
hole3_x4, hole3_y4 = get_down_coordinate(hole3_x2, hole3_y2)

def manipulating_piece(piece, t,temp_x1, temp_x2,temp_y2, temp_y1, temp_y3):  
    lighthouse1 = piece 
    # finding (x1,y1)
    flag= 1
    v1=0;
    for i in range(256):
        v1 += 1
        u1=0
        for j in range(256):
            u1 +=1
            if(lighthouse1[i][j] != 255):
#                print(u1,v1)
                flag=0
                break
        if (flag==0): break
        
    x1,y1= get_cartesian_coordinate(u1,v1)
    
    # finding (x2,y2)
    flag= 1
    v2=255;
    for i in range(255,-1,-1):
        v2 -= 1
        u2=255
        for j in range(255,-1,-1):
            u2 -=1
            if(lighthouse1[i][j] != 255):
                #print (u2,v2)
                flag=0
                break
        if (flag==0): break
        
    x2,y2= get_cartesian_coordinate(u2,v2)
    
    # finding (x3,y3)
    flag= 1
    u3=0;
    for i in range(256):
        u3 += 1
        v3=0
        for j in range(256):
            v3 +=1
            if(lighthouse1[j][i] != 255):
                #print (u3,v3)
                flag=0
                break
        if (flag==0): break
        
    x3,y3= get_cartesian_coordinate(u3,v3)
    
    # finding (x4,y4)
    flag= 1
    u4=255;
    for i in range(255,-1,-1):
        u4 -= 1
        v4=255
        for j in range(255,-1,-1):
            v4 -=1
            if(lighthouse1[j][i] != 255):
                
                #print (u4,v4)
                flag=0
                break
        if (flag==0): break
        
    x4,y4= get_cartesian_coordinate(u4,v4)
    
    im_width= math.sqrt((x1-x3)*(x1-x3) + (y1-y3)*(y1-y3))
    im_height= math.sqrt((x2-x4)*(x2-x4) + (y2-y4)*(y2-y4))
    im_theta = math.atan((y2-y3)/(x2-x3))
    im_theta = im_theta+ t
    im_centre_point_x = (x3 + x4)/2
    im_centre_point_y = (y3 + y4)/2
    
    A= np.array([[1,0,im_centre_point_x],[0,1,im_centre_point_y],[0,0,1]])
    B= np.array([[np.cos(im_theta),-np.sin(im_theta),0],[np.sin(im_theta),np.cos(im_theta),0],[0,0,1]])
    C= np.array([[1,0,-im_centre_point_x],[0,1,-im_centre_point_y],[0,0,1]])
    
    D= np.matmul(A, B)
    E= np.matmul(D, C)
    inverse= pinv(E)
    
    lighthouse1_new= np.zeros((256,256))
    
    for i in range(256):
        for j in range(256):
            car_x, car_y = get_cartesian_coordinate(i,j) 
            F= np.array([[car_x],[car_y],[1]])
            G= np.matmul(inverse, F)
            u,v=get_image_coordinate(G[0][0],G[1][0])
            if (u>255 or v>255 or u<0 or v<0):
                lighthouse1_new[i][j]= 255
            else:
                lighthouse1_new[i][j] = lighthouse1[math.floor(u)][math.floor(v)]
    
    # finding (u1,v1)
    flag= 1
    v1=0;
    for i in range(256):
        v1 += 1
        u1=0
        for j in range(256):
            u1 +=1
            if(lighthouse1_new[j][i] != 255):
                #print(u1,v1)
                flag=0
                break
        if (flag==0): break
        
        
    # finding (u3,v3)
#    flag= 1
#    v3=255;
#    for i in range(255,-1,-1):
#        v3 -= 1
#        u3=0
#        for j in range(256):
#            u3 +=1
#            if(lighthouse1_new[j][i] != 255):
#                print(u3,v3)
#                flag=0
#                break
#        if (flag==0): break
        
        
    # finding (u2,v2)
#    flag= 1
#    v4=255;
#    for i in range(255,-1,-1):
#        v4 -= 1
#        u2=255
#        for j in range(255,-1,-1):
#            u2 -=1
#            if(lighthouse1_new[j][i] != 255):
#                print (u4,v4)
#                flag=0
#                break
#        if (flag==0): break
    
    lighthouse1_new2= lighthouse1_new[u1 :math.floor(u1+ im_width),v1:math.floor(v1+ im_height)]
    plt.matshow(lighthouse1_new2, cmap='gray')
    
    
    lighthouse1_new3= np.zeros((160,160))
    
    def bilinear_interpolation(image):
        row_ratio = im_width/160
        col_ratio = im_height/160
        for i in range(160):
            for j in range(160):
                new_r = i*row_ratio
                new_c = j*col_ratio
                r = int(math.floor(new_r))
                c = int(math.floor(new_c))
                error_r = new_r - r
                error_c = new_c - c
                if(r<math.floor(im_width)-1 and c<math.floor(im_height)-1):
                    lighthouse1_new3[i][j]= int(((image[r][c]*(1- error_r)*(1- error_c))+(image[r+1][c]*(error_r)*(1- error_c)) + (image[r][c+1]*(1- error_r)*(error_c)) + (image[r+1][c+1]*(error_r)*(error_c))))
    
    bilinear_interpolation(lighthouse1_new2)
    
    
    plt.matshow(lighthouse1_new3, cmap='gray')
    
    #lower boundary
    lighthouse1_new3[:][158]= lighthouse1_new3[:][156]
    lighthouse1_new3[:][159]= lighthouse1_new3[:][157]
    
    lighthouse1_new3[:][0]= lighthouse1_new3[:][1]
    
    
#    
#
#    lighthouse1_new3[158][:]= lighthouse1_new3[156][:]
#    lighthouse1_new3[159][:]= lighthouse1_new3[157][:]
###    
#    lighthouse1_new3[0][:]= lighthouse1_new3[4][:]
#    lighthouse1_new3[1][:]= lighthouse1_new3[5][:]
#    lighthouse1_new3[2][:]= lighthouse1_new3[6][:]
#    lighthouse1_new3[3][:]= lighthouse1_new3[7][:]


    plt.matshow(lighthouse1_new3, cmap='gray')


    
    for i in range(temp_x1, temp_x2+1,1):
        for j in range(temp_y1, temp_y3,1):
            lighthouse[j][i]= lighthouse1_new3[j- temp_y3][i- temp_x1]
    
    #right border
    lighthouse[temp_y1:temp_y3+1,temp_x2]=lighthouse[temp_y1:temp_y3+1,temp_x2+1]
    if(temp_x1== 62):
        lighthouse[temp_y1:temp_y3+1,temp_x2-1]=lighthouse[temp_y1:temp_y3+1,temp_x2-2]
    
    #left border
    #lighthouse[temp_y1:temp_y3+1,temp_x1-1]=lighthouse[temp_y1:temp_y3+1,temp_x1-2]
    lighthouse[temp_y1:temp_y3+1,temp_x1]=lighthouse[temp_y1:temp_y3+1,temp_x1-1]
    lighthouse[temp_y1:temp_y3+1,temp_x1+1]=lighthouse[temp_y1:temp_y3+1,temp_x1+2]
    
#    #upper border
#    lighthouse[temp_y1,temp_x1:temp_x2]=lighthouse[temp_y1+1,temp_y2:temp_x2]
#    lighthouse[temp_y1+1,temp_x1:temp_x2]=lighthouse[temp_y1+1,temp_y2:temp_x2]
#    
#    #lower border
#    lighthouse[temp_y3-1,temp_x1:temp_x2]=lighthouse[temp_y1-2,temp_y2:temp_x2]
#    lighthouse[temp_y3,temp_x1:temp_x2]=lighthouse[temp_y1+1,temp_y2:temp_x2]
            
    #lighthouse[:,temp_y2-1] = 0
#    lighthouse[:][temp_y2-1] = 0      
    plt.matshow(lighthouse, cmap='gray')
    
manipulating_piece(read_image("lighthouse1.raw", 256,256).tolist() , math.pi/2,hole1_x1, hole1_x2,hole1_y2, hole1_y1, hole1_y3)
manipulating_piece(read_image("lighthouse2.raw", 256,256).tolist() , math.pi ,hole2_x1, hole2_x2,hole2_y2, hole2_y1, hole2_y3)
manipulating_piece(read_image("lighthouse3.raw", 256,256).tolist() , 0,hole3_x1, hole3_x2,hole3_y2, hole3_y1, hole3_y3)

lighthouse.astype('int8').tofile('lighthouse_output.raw')