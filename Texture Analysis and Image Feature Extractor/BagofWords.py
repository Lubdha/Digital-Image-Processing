#Lubdha Pimpale
#EE 569 Homework 4
#19 March 2019

import numpy as np
import cv2 
from matplotlib import pyplot as plt

def read_image(file, width, height):
    raw_input = open(file,'rb').read()
    raw_input = np.frombuffer(raw_input, np.uint8).copy()
    image = np.reshape(raw_input, (height,width))
    return image

one_1 = read_image('one_1.raw',28,28)
one_2 = read_image('one_2.raw',28,28)
one_3 = read_image('one_3.raw',28,28)
one_4 = read_image('one_4.raw',28,28)
one_5 = read_image('one_5.raw',28,28)

zero_1 = read_image('zero_1.raw',28,28)
zero_2 = read_image('zero_2.raw',28,28)
zero_3 = read_image('zero_3.raw',28,28)
zero_4 = read_image('zero_4.raw',28,28)
zero_5 = read_image('zero_5.raw',28,28)

eight = read_image('eight.raw',28,28)

sift = cv2.xfeatures2d.SIFT_create()
keypoint1, descriptor1 = sift.detectAndCompute(one_1,None)
keypoint2, descriptor2 = sift.detectAndCompute(one_2,None)
keypoint3, descriptor3 = sift.detectAndCompute(one_3,None)
keypoint4, descriptor4 = sift.detectAndCompute(one_4,None)
keypoint5, descriptor5 = sift.detectAndCompute(one_5,None)

keypoint6, descriptor6 = sift.detectAndCompute(zero_1,None)
keypoint7, descriptor7 = sift.detectAndCompute(zero_2,None)
keypoint8, descriptor8 = sift.detectAndCompute(zero_3,None)
keypoint9, descriptor9 = sift.detectAndCompute(zero_4,None)
keypoint10, descriptor10 = sift.detectAndCompute(zero_5,None)

keypoint11, descriptor11 = sift.detectAndCompute(eight,None)
   
train_des= np.zeros((1,128)) 
for des in [descriptor1, descriptor2, descriptor3, descriptor4, descriptor5,descriptor6, descriptor7, descriptor8, descriptor9, descriptor10]:
    if des is not None:
        train_des = np.concatenate((train_des,des),axis=0)
train_des= np.delete(train_des,(0), axis=0)
        
train_des_zero= np.zeros((1,128)) 
for des in [descriptor6, descriptor7, descriptor8, descriptor9, descriptor10]:
    if des is not None:
        train_des_zero = np.concatenate((train_des_zero,des),axis=0)
train_des_zero= np.delete(train_des_zero,(0), axis=0)

train_des_one= np.zeros((1,128)) 
for des in [descriptor1, descriptor2, descriptor3, descriptor4, descriptor5]:
    if des is not None:
        train_des_one = np.concatenate((train_des_one,des),axis=0)
train_des_one= np.delete(train_des_one,(0), axis=0)


#k-means clustering
from sklearn.cluster import KMeans
kmeans = KMeans(n_clusters=2).fit(train_des)
labels = kmeans.labels_

hist_one=[0,0]
for des in train_des_one:
    des = des.reshape(128,1)
    des = des.T
    if kmeans.predict(des)==0:
        hist_one[0] +=1
    elif kmeans.predict(des)==1:
        hist_one[1] +=1

fig = plt.figure(1)
plt.bar([1,0],[hist_one[1], hist_one[0]])
fig.show

#hist
hist_zero=[0,0]
for des in train_des_zero:
    des = des.reshape(128,1)
    des = des.T
    if kmeans.predict(des)==0:
        hist_zero[0] +=1
    elif kmeans.predict(des)==1:
        hist_zero[1] +=1

fig = plt.figure(2)
plt.bar([1,0],[hist_zero[1], hist_zero[0]])
fig.show

hist_eight=[0,0]
for des in descriptor11:
    des = des.reshape(128,1)
    des = des.T
    if kmeans.predict(des)==0:
        hist_eight[0] +=1
    elif kmeans.predict(des)==1:
        hist_eight[1] +=1        
    
fig = plt.figure(3)
plt.bar([1,0],[hist_eight[1], hist_eight[0]])
fig.show


#Intersection
intersection_zeros = min(hist_zero[0],hist_eight[0])/ max(hist_zero[0],hist_eight[0]) + min(hist_zero[1],hist_eight[1])/ max(hist_zero[1],hist_eight[1])
intersection_one = min(hist_eight[0],hist_one[0])/ max(hist_eight[0],hist_one[0]) + min(hist_eight[1],hist_one[1])/ max(hist_eight[1],hist_one[1])


print(intersection_zeros,intersection_one)