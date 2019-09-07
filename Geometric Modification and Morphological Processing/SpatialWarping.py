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
    x= u-255
    y= 255 - v
    return x,y

def get_image_coordinate(x,y):
    u= x+255
    v= 255- y
    return u,v


hat= read_image("hat.raw", 512, 512).tolist()

hat_width= 512


#t=1
#u_1=u_2= u_3= v_1 = 0
#v_3 = 511
#
#v_2= 255
#u_4 = v_4=u_5= 127
#v_5 = 383
#u_6 = v_6 = 255

##t=2
#
# u2_1= v2_1 =v2_3=v2_2= 0
# u2_2= 255
# u2_3=511
# u2_4= v2_4=v2_5= 127
# u2_5= 383
# u2_6 = v2_6 = 255

##t=3
#u3_1= 0 
#v3_1 =511
#u3_2= 255
#v3_2= 511
#u3_3= 511
#v3_3= 511
#u3_4= 127
#v3_4= 383
#u3_5= 383
#v3_5= 383
#u3_6 = v3_6 = 255

##t=4
# u4_1= 511
# v4_1= 0
# u4_2= 511
# v4_2= 255
# u4_3= 511
# v4_3= 511
# u4_4 =383
# v4_4= 127
# u4_5= 383
# v4_5= 383
# u4_6 = v4_6 = 255

def get_output(t,u_1,v_1,u_2,v_2,u_3,v_3,u_4,v_4,u_5,v_5,u_6,v_6) :   
    [(x_1,y_1),(x_2,y_2),(x_3,y_3),(x_4,y_4),(x_5,y_5),(x_6,y_6)]=  [get_cartesian_coordinate(u_1,v_1),
                                                                     get_cartesian_coordinate(u_2,v_2),
                                                                     get_cartesian_coordinate(u_3,v_3),
                                                                     get_cartesian_coordinate(u_4,v_4),
                                                                     get_cartesian_coordinate(u_5,v_5),
                                                                     get_cartesian_coordinate(u_6,v_6)]
    
    if(t==1):
        [(u_1,v_1),(u_2,v_2),(u_3,v_3),(u_4,v_4),(u_5,v_5),(u_6,v_6)]= [(x_1,y_1),(x_2+128,y_2),(x_3,y_3),(x_4,y_4),(x_5,y_5),(x_6,y_6)]
    
    if(t==2):
        [(u_1,v_1),(u_2,v_2),(u_3,v_3),(u_4,v_4),(u_5,v_5),(u_6,v_6)]= [(x_1,y_1),(x_2,y_2-128),(x_3,y_3),(x_4,y_4),(x_5,y_5),(x_6,y_6)]
    
    if(t==3):
        [(u_1,v_1),(u_2,v_2),(u_3,v_3),(u_4,v_4),(u_5,v_5),(u_6,v_6)]= [(x_1,y_1),(x_2,y_2+128),(x_3,y_3),(x_4,y_4),(x_5,y_5),(x_6,y_6)]
    
    if(t==4):
        [(u_1,v_1),(u_2,v_2),(u_3,v_3),(u_4,v_4),(u_5,v_5),(u_6,v_6)]= [(x_1,y_1),(x_2-128,y_2),(x_3,y_3),(x_4,y_4),(x_5,y_5),(x_6,y_6)]
    
    A= [[u_1,u_2,u_3,u_4,u_5,u_6],[v_1,v_2,v_3, v_4,v_5,v_6]]
    
    B= [[1,1,1,1,1,1],[x_1, x_2, x_3, x_4,x_5,x_6], [y_1, y_2, y_3, y_4,y_5,y_6],[x_1*x_1, x_2*x_2, x_3*x_3, x_4*x_4,x_5*x_5,x_6*x_6],
        [x_1*y_1, x_2*y_2, x_3*y_3, x_4*y_4, x_5*y_5, x_6*y_6],[y_1*y_1, y_2*y_2, y_3*y_3, y_4*y_4,y_5*y_5,y_6*y_6]]
    inv_B= pinv(B)
    
    C=[[a_1,a_2,a_3,a_4,a_5,a_6],[b_1,b_2,b_3,b_4,b_5,b_6]] = np.matmul(A, inv_B)
    
    
    
    # t=1, (u_1,u_6+1),(i,512-i)
    # t=4, (u_1,u_6+1,-1),(511-i,i+1)
    
    # t=2, enumerate(range(j,512-j)),(v_1,v_6+1)
    # t=3, (255-i,i+256+1)
    u_1,v_1= get_image_coordinate(x_1,y_1)
    u_6,v_6= get_image_coordinate(x_6,y_6)
    #print(u_1)
    
    if(t==1):
        diff=1 
    
    
    if(t==4):
        diff=-1 
    
    if(t==1 or t==4):
         for i in range(u_1,u_6+1,diff):
            if(t==1):
                j_start= i
                j_end= 512-i
                
            if(t==4): 
                j_start= 511-i
                j_end= i+1
                
            for j in enumerate(range(j_start,j_end)):
                 #print(j,'j')
                 #print(type(j[1]))
                 #print(i,'i')
                 #print(j[1],'j')
                 x,y= get_cartesian_coordinate(i,j[1])
                 [[u],[v]]= np.matmul(C,[[1],[x],[y],[x*x],[x*y],[y*y]])
                 u_warped,v_warped= get_image_coordinate(u,v)
                 #print(type(((u_warped))))
                 #print(int(u_warped))
                 #print(int(v_warped))
                 #print(hat[i][j[1]],'hat')
                 warped_image[int(u_warped)][int(v_warped)]= hat[i][j[1]]
    
    if(t==2):
        diff=1 
    
    if(t==3):
        diff=-1 
        
    if(t==2 or t==3):
        for j in range(v_1,v_6+1,diff):
            if(t==2):
                i_start= j
                i_end= 512-j
                
            if(t==3): 
                i_start= 511-j
                i_end= j+1
            #print(i,'i')
            for i in enumerate(range(i_start,i_end)):
                #print(j,'j')
                #print(type(j[1]))
                #print(i,'i')
                #print(j[1],'j')
                x,y= get_cartesian_coordinate(i[1],j)
                [[u],[v]]= np.matmul(C,[[1],[x],[y],[x*x],[x*y],[y*y]])
                u_warped,v_warped= get_image_coordinate(u,v)
                #print(type(((u_warped))))
                #print(int(u_warped))
                #print(int(v_warped))
                #print(hat[i][j[1]],'hat')
                warped_image[int(u_warped)][int(v_warped)]= hat[i[1]][j]
            
            
    plt.matshow(warped_image, cmap='gray')

warped_image= np.zeros((512,512))

get_output(1,0,0,0,255,0,511,127,127,127,383,255,255)
get_output(2,0,0, 255,0, 511,0, 127,127, 383,127, 255,255)
get_output(3,0,511, 255,511, 511,511, 127,383, 383,383, 255,255)
get_output(4,511,0, 511,255, 511,511, 383,127, 383,383,255,255)

warped_image.astype('int8').tofile('warped_hat.raw')