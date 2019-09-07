#Lubdha Pimpale
#EE 569 Homework 4
#19 March 2019

import numpy as np

#Law's filters
L5= np.array([1,4,6,4,1]).reshape((1,5))
E5= np.array([1,-2,0,2,1]).reshape((1,5))
S5= np.array([-1,0,2,0,-1]).reshape((1,5))
W5= np.array([-1,2,0,-2,1]).reshape((1,5))
R5= np.array([1,-4,6,-4,1]).reshape((1,5))

filter_bank= np.empty((25,5,5))
i=0
for a in [L5,E5,S5,W5,R5]:
    for b in [L5,E5,S5,W5,R5]:
        filter_bank[i,:,:] = np.multiply(np.transpose(a),b)
        i +=1
        
#read image
def read_image(file, width, height):
    raw_input = open(file,'rb').read()
    raw_input = np.frombuffer(raw_input, np.uint8).copy()
    image = np.reshape(raw_input, (height,width))
    return image

height=width=510
image = read_image('comb.raw', height,width)

extended_image = np.pad(image,2,'reflect')
filtered_images= np.empty((25,height,width))
energy_filtered_images= np.empty((25,height,width))



m = 2
for k in  range(25):
    for i in range(m, height + m):
        for j in range(m, width + m):
            filtered_images[k,i-m,j-m ]= np.average(np.multiply(filter_bank[k, :, :],extended_image[i-m : i+m+1,j-m : j+m+1]))

#filtered_images= np.square(filtered_images)
    
window = 35 
m = int ((window-1)/2)
filtered_images = np.pad(filtered_images,m,'reflect')

for k in  range(25):
    for i in range(m, height + m):
        for j in range(m, width + m): 
            if k==0:
                energy_filtered_images[k,i-m,j-m ] = 0  
                nor_factor = np.average(np.absolute(filtered_images[k,i-m:i+m+1,j-m: j+m+1 ]))
            else:
                energy_filtered_images[k,i-m,j-m ] = (np.average(np.absolute(filtered_images[k,i-m:i+m+1,j-m: j+m+1 ])))/nor_factor      

changing_dim_energy = np.empty((height*width,25))
for k in  range(25):
    for i in range(height):
        for j in range(width):
            changing_dim_energy[i*height + j,k] = energy_filtered_images[k,i,j]


#k-means clustering
from sklearn.cluster import KMeans
kmeans = KMeans(n_clusters=7).fit(changing_dim_energy)
labels = kmeans.labels_

Out_image= np.empty((height,width))
for i in range(height):
    Out_image[i, :]= labels[i*height: i*height+height]

for i in range(height):
    for j in range(width):
        if Out_image[i,j]==1:
            Out_image[i,j] = 42
        if Out_image[i,j]==2:
            Out_image[i,j] = 84
        if Out_image[i,j]==3:
            Out_image[i,j] = 126
        if Out_image[i,j]==4:
            Out_image[i,j] = 168
        if Out_image[i,j]==5:
            Out_image[i,j] = 210
        if Out_image[i,j]==6:
            Out_image[i,j] = 255
            
import cv2
cv2.imwrite('segmentation.jpg', Out_image )