#Lubdha Pimpale
#lpimpale@usc.edu
#Date: 28 April 2019

import pickle
import numpy as np
import data
import saab3

def laws(im):
    #Law's filters
    L5= np.array([1,4,6,4,1]).reshape((1,5))
    E5= np.array([1,-2,0,2,1]).reshape((1,5))
    S5= np.array([-1,0,2,0,-1]).reshape((1,5))
    W5= np.array([-1,2,0,-2,1]).reshape((1,5))
    R5= np.array([1,-4,6,-4,1]).reshape((1,5))
    
    m=2
    height=width=32
    
    filter_bank= np.empty((25,5,5))
    i=0
    for a in [L5,E5,S5,W5,R5]:
        for b in [L5,E5,S5,W5,R5]:
            filter_bank[i,:,:] = np.multiply(np.transpose(a),b)
            i +=1
    
    im = np.pad(im,m,'reflect')
    filtered_images= np.empty((5,32,32))
    
    for k in  range(5):
        for i in range(m, height + m):
            for j in range(m, width + m):
                filtered_images[k,i-m,j-m ]= np.average(np.multiply(filter_bank[k, :, :],im[i-m : i+m+1,j-m : j+m+1]))

    return filtered_images

def main():
    # load data
    fr=open('pca_params55.pkl','rb')  
    pca_params=pickle.load(fr)
    fr.close()
      
    
    # read data
    train_images, train_labels, test_images, test_labels, class_list = data.import_data("0-9")
    print('Training image size:', train_images.shape)
    print('Testing_image size:', test_images.shape)
    #print('pca_params:',pca_params)
	
    train=np.zeros((10000,32,32,1))
    t=[]
    #Apply laws filter
    for im in train_images[0:10000]:
        t.append(laws(im))
    
    t = np.vstack(t)
    train=t
    train = train.reshape(10000,32,32,1)
    feature=[]
    for i in range(0,10000,200):
        print('i',i)
        print('--------Training--------')
        
        print('shape',train.shape)
        f=saab3.initialize(train[i:i+200], pca_params) 
        f=f.reshape(f.shape[0],-1)
        feature.append(f)
        
        
    feature= np.vstack(feature)
    #print("S4 shape:", feature.shape)
    print('--------Finish Feature Extraction subnet--------')
    feat={}
    feat['feature']=feature
	
    # save data
    fw=open('feat33.pkl','wb')    
    pickle.dump(feat, fw)    
    fw.close()

if __name__ == '__main__':
	main()
