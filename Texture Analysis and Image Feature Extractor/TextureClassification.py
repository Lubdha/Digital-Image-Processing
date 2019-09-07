#Lubdha Pimpale
#EE 569 Homework 4
#19 March 2019

import numpy as np
from matplotlib import pyplot as plt

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

def find_energy(im):
    image = read_image(im, 128,128)
    
    avg= np.average(image)
    image = image - avg
    
    m=2
    height=width=128
    
    extended_image = np.pad(image,m,'reflect')
    filtered_images= np.empty((25,128,128))
    
    
    for k in  range(25):
        for i in range(m, height + m):
            for j in range(m, width + m):
                filtered_images[k,i-m,j-m ]= np.average(np.multiply(filter_bank[k, :, :],extended_image[i-m : i+m+1,j-m : j+m+1]))
     
    energy = np.empty((25))       
    for i in range(25):
        energy[i]=np.average(np.absolute(filtered_images[i,:,:]))
        
    return energy

energy_all_images=np.empty((12,25))
energy_all_images[0,:]=find_energy('texture1.raw')
energy_all_images[1,:]=find_energy('texture2.raw')
energy_all_images[2,:]=find_energy('texture3.raw')
energy_all_images[3,:]=find_energy('texture4.raw')
energy_all_images[4,:]=find_energy('texture5.raw')
energy_all_images[5,:]=find_energy('texture6.raw')
energy_all_images[6,:]=find_energy('texture7.raw')
energy_all_images[7,:]=find_energy('texture8.raw')
energy_all_images[8,:]=find_energy('texture9.raw')
energy_all_images[9,:]=find_energy('texture10.raw')
energy_all_images[10,:]=find_energy('texture11.raw')
energy_all_images[11,:]=find_energy('texture12.raw')

#To find discriminant powers of features
var=[]
for i in range(25):
    var.append(np.var(energy_all_images[:,i]))
weakest_discriminant_power = np.argmax(var)
strongest_discriminant_power = np.argmin(var)

#Normalizing features
for i in range(12):
    energy_all_images[i,:] = (energy_all_images[i,:]- np.average(energy_all_images[i,:]))/np.std(energy_all_images[i,:])
  


#dimension reduction
from sklearn.decomposition import PCA
pca = PCA(n_components=12)
after_dim_red = pca.fit_transform(energy_all_images)
weight = pca.explained_variance_ratio_

# 3D plot 
figure = plt.figure()
ax = plt.axes(projection='3d')
ax.scatter3D(after_dim_red[:,0], after_dim_red[:,1], after_dim_red[:,2], edgecolor='c')

#k-means clustering
from sklearn.cluster import KMeans
kmeans = KMeans(n_clusters=4).fit(after_dim_red)
labels= kmeans.labels_

print(labels)