#Lubdha Pimpale
#lpimpale@usc.edu
#Date: 28 April 2019

import pickle
import data
import numpy as np

#Load features for train

fr=open('llsr_feature55.pkl','rb')  
feat1 =pickle.load(fr, encoding='latin1')
fr.close()

fr=open('llsr_feature53.pkl','rb')  
feat2 =pickle.load(fr, encoding='latin1')
fr.close()

fr=open('llsr_feature35.pkl','rb')  
feat3 =pickle.load(fr, encoding='latin1')
fr.close()

fr=open('llsr_feature33.pkl','rb')  
feat4 =pickle.load(fr, encoding='latin1')
fr.close()


#Load test data

fr=open('feat_test55.pkl','rb')  
test_feat1 =pickle.load(fr, encoding='latin1')
fr.close()

fr=open('feat_test53.pkl','rb')  
test_feat2 =pickle.load(fr, encoding='latin1')
fr.close()

fr=open('feat_test35.pkl','rb')  
test_feat3 =pickle.load(fr, encoding='latin1')
fr.close()

fr=open('feat_test33.pkl','rb')  
test_feat4 =pickle.load(fr, encoding='latin1')
fr.close()

#concat
train= np.concatenate((feat1, feat2, feat3, feat4), axis=1)
test= np.concatenate((test_feat1, test_feat2, test_feat3, test_feat4), axis=1)

#PCA
from sklearn.decomposition import PCA
pca = PCA(n_components=10)
train_afterPCA = pca.fit_transform(train)
test_afterPCA = pca.transform(test)

#SVC
from sklearn.svm import SVC
train_images, train_labels, test_images, test_labels, class_list = data.import_data("0-9")
classifier = SVC(gamma='auto')
classifier.fit(train_afterPCA, train_labels) 
pred_test = classifier.predict(test_afterPCA)
pred_train = classifier.predict(train_afterPCA)

#accuracy
from sklearn.metrics import accuracy_score
train_acc = accuracy_score(train_labels, pred_train)
test_acc = accuracy_score(test_labels, pred_test)
print('train_acc',train_acc)
print('test_acc',test_acc)

#confusion matrix
from mlxtend.evaluate import confusion_matrix
import matplotlib.pyplot as plt
from mlxtend.plotting import plot_confusion_matrix

confusion = confusion_matrix(y_target= test_labels, y_predicted=pred_test, binary=False)
fig, ax = plot_confusion_matrix(conf_mat=confusion)
plt.show()

confusion = confusion_matrix(y_target= train_labels, y_predicted=pred_train, binary=False)
fig, ax = plot_confusion_matrix(conf_mat=confusion)
plt.show()