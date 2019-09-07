#Lubdha Pimpale
#EE 569 Homework 4
#19 March 2019

import numpy as np
import cv2 

def read_image(file, width, height):
    raw_input = open(file,'rb').read()
    raw_input = np.frombuffer(raw_input, np.uint8).copy()
    image = np.reshape(raw_input, (height,width,3))
    return image

image1 = read_image('river1.raw',768,1024)
image2 = read_image('river2.raw',768,1024)


sift = cv2.xfeatures2d.SIFT_create()
keypoint1, descriptor1 = sift.detectAndCompute(image1,None)
keypoint2, descriptor2 = sift.detectAndCompute(image2,None)


norm1 = np.empty((descriptor1.shape[0]))
for i in range(descriptor1.shape[0]):
    norm1[i]= np.linalg.norm(descriptor1[i,:])

norm2 = np.empty((descriptor2.shape[0]))
for i in range(descriptor2.shape[0]):
    norm2[i]= np.linalg.norm(descriptor2[i,:])
    
# BFMatch
brute = cv2.BFMatcher()
index = np.argmax(norm1)
matches = brute.knnMatch(np.transpose(descriptor1[index,:].reshape((128,1))),descriptor2, k=2)

# Ratio test
ratio_test = []
for a,b in matches:
    ratio_test.append([a])

print('Ori of river1', keypoint1[index].angle)
print('Ori of river2', keypoint2[ratio_test[0][0].trainIdx].angle)

final = cv2.drawMatchesKnn(image1,[keypoint1[index]],image2,keypoint2,ratio_test,None,flags=2)

cv2.imwrite('match.jpg', cv2.cvtColor(final, cv2.COLOR_RGB2BGR)) 
