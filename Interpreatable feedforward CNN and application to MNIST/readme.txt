Interpreatable feedforward CNN and application to mnist




Usage: 

Image reconstructions from Saab coefficients

* Run Getkernel3.py. Uncomment non-overlapping patches. And change number of kernels for 4 different settings. 
* Run inverse_multisaab_transform.py


Handwritten digits recognition using ensembles of feedforward design

For first 4 settings,
* Run Getkernel3.py. Change training samples to 60,000. Uncomment overlapping patches 
* Run Getfeature3.py
* Run Getweight3.py
* Run SVC.py

For next 6 settings,
* Run Getkernel3.py. 
* Run GetfeatureLaws.py
* Run Getweight3.py
* Run SVC.py


References:

1. Kuo, C. C. J., Zhang, M., Li, S., Duan, J., & Chen, Y. (2019). Interpretable convolutional neural networks via feedforward design. Journal of Visual Communication and Image Representation.
2. Chen, Y., Yang, Y., Wang, W., & Kuo, C. C. J. (2019). Ensembles of feedforward-designed convolutional neural networks. arXiv preprint arXiv:1901.02154.
3. [MNIST] http://yann.lecun.com/exdb/mnist/
4. Source code adapted from https://github.com/davidsonic/Interpretable_CNNs_via_Feedforward_Design.

