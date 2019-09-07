#Lubdha Pimpale
#lpimpale@usc.edu
#Date: 28 April 2019

#inverse 
import pickle
import numpy as np
import data
import saab
import keras
import sklearn
import Getkernel
import Getfeature

# load data
fr=open('pca_params.pkl','rb')  
pca_params=pickle.load(fr, encoding='iso-8859-1')
fr.close()

# load data
fr=open('feat.pkl','rb')  
feat=pickle.load(fr)
fr.close()

test_images=[]

t1 = Image.open('1.png')
t2 = Image.open('2.png')
t3 = Image.open('3.png')
t4 = Image.open('4.png')

t1 = np.array(t1.getdata())
t1 = t1.reshape(-1,32,32,1)
t1 = t1/255.
test_images.append(t1)

t2 = np.array(t2.getdata())
t2 = t2.reshape(-1,32,32,1)
t2 = t2/255.
test_images.append(t2)

t3 = np.array(t3.getdata())
t3 = t3.reshape(-1,32,32,1)
t3 = t3/255.
test_images.append(t3)

t4 = np.array(t4.getdata())
t4 = t4.reshape(-1,32,32,1)
t4 = t4/255.
test_images.append(t4)

test_images= np.vstack(test_images)
test_labels = [9,0,5,3]
f = saab.initialize(test_images,pca_params)

num_layers=pca_params['num_layers']
kernel_sizes=pca_params['kernel_size']

trans= f
for i in range(num_layers-1,-1,-1):
    print('--------stage %d --------'%i)
    feature_expectation = pca_params['Layer_%d/feature_expectation'%i]
    kernels = pca_params['Layer_%d/kernel'%i]
    mean = pca_params['Layer_%d/pca_mean'%i]

    if i==0:
        # Transform to get data for the previous stage
        sample_patches_centered=np.matmul(trans, np.linalg.pinv(np.transpose(kernels))
    else:
        # Call bias term
        bias= pca_params['Layer_%d/bias'%i]

        # Add bias
        e=np.zeros((1, kernels.shape[0]))
        e[0,0]=1
        print('feat dim',feat['feature'].shape)
        print('dim(bias*e)',(bias*e).shape)
        trans +=bias*e

        # Transform to get data for the previous stage
        sample_patches_centered_w_bias=np.matmul(trans, np.linalg.pinv(np.transpose(kernels))

        # Subtract bias
        sample_patches_centered=sample_patches_centered_w_bias-np.sqrt(num_channels)*bias

    # Add feature mean (Set E(X)=0 for each dimension)
    sample_patches = sample_patches_centered + feature_expectation
