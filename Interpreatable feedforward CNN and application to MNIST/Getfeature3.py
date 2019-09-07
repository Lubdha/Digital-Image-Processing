#Lubdha Pimpale
#lpimpale@usc.edu
#Date: 28 April 2019

import pickle
import numpy as np
import data
import saab3


def main():
    # load data
    fr=open('pca_params33.pkl','rb')  
    pca_params=pickle.load(fr)
    fr.close()

    # read data
    train_images, train_labels, test_images, test_labels, class_list = data.import_data("0-9")
    print('Training image size:', train_images.shape)
    print('Testing_image size:', test_images.shape)
    #print('pca_params:',pca_params)
	
    # Training

    f=[]
    feature=[]
    for i in range(0,60000,200):
        print('--------Training--------')
        f=saab3.initialize(train_images[i:i+200], pca_params) 
        f=f.reshape(f.shape[0],-1)
        feature.append(f)
        
    feature= np.vstack(feature)
    print("S4 shape:", feature.shape)
    print('--------Finish Feature Extraction subnet--------')
    feat={}
    feat['feature']=feature
	
    # save data
    fw=open('feat33.pkl','wb')    
    pickle.dump(feat, fw)    
    fw.close()

if __name__ == '__main__':
	main()
