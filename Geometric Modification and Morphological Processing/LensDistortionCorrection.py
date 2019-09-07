#Lubdha Pimpale
#email- lpimpale@usc.edu
#date- 3 Mar 2019


import numpy as np
from matplotlib import pyplot as plt
import math

def read_image(file, width, height):
    raw_input = open(file,'rb').read()
    raw_input = np.frombuffer(raw_input, np.uint8).copy()
    image = np.reshape(raw_input, (height,width))
    return image

im_width= 1072
im_height= 712

def get_camera_coordinate(u,v):
    x= (u-im_height/2 +1)/600
    y= (v-im_width/2 +1)/600
    return x,y

def get_image_coordinate(x,y):
    u= x*600 + im_height/2 -1
    v= y*600 + im_width/2 -1
    return u,v

classroom = read_image('classroom.raw', 1072, 712)
classroom = np.pad(classroom,50,'edge')
pad = 50

k1= -0.3536
k2= 0.1730
k3= 0


U= np.arange(712+2*pad)
V= np.arange(1072+ 2*pad)

X,Y= get_camera_coordinate(U,V)

XX= np.repeat(np.transpose([X]), 1072 + 2*pad, axis=1)
YY= np.repeat([Y], 712 + 2*pad, axis=0)

r_sq= np.square(XX) + np.square(YY)


X_d= (XX*(1+ k1*r_sq + k2*np.square(r_sq) + k3*np.power(r_sq,3)))
Y_d= (YY*(1+ k1*r_sq + k2*np.square(r_sq) + k3*np.power(r_sq,3)))

U_d, V_d= get_image_coordinate(X_d,Y_d)

chess= np.zeros((712 + 2*pad,1072 + 2*pad))
for u in range(712 +2*pad):
    for v in range(1072 + 2*pad):
        #print(u.index)
        #print(u,v)
        a= U_d[u,v]- math.floor(U_d[u,v])
        b= V_d[u,v]- math.floor(V_d[u,v])
#        print (U_d[u,v])
#        print (V_d[u,v])
        if U_d[u,v]>0 and V_d[u,v]>0 and U_d[u,v]<(712 +2*pad-1) and V_d[u,v]<(1071 +2*pad-1):
            chess[u,v]= (classroom[math.floor(U_d[u,v])][math.floor(V_d[u,v])])*(1-a)*(1-b)+ (classroom[math.floor(U_d[u,v])][math.ceil(V_d[u,v])])*(1-a)*(b)+ (classroom[math.ceil(U_d[u,v])][math.floor(V_d[u,v])])*(a)*(1-b)+ (classroom[math.ceil(U_d[u,v])][math.ceil(V_d[u,v])])*(a)*(b)

            
plt.matshow(chess, cmap='gray')            

chess.astype('int8').tofile('chess_undistorted.raw')