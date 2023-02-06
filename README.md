# InterIIT JLR Open CV
#### This repository contains all the files involved in solving the Computer Vision Part of the JLR Problem Statement in InterIIT Techmeet 11.0.

## Charging Port Detection
##### This folder contains the Yolo model and dataset used for charging port detection. 

#### Yolov5 has been used to detect the CCS2 charging port. Yolo model detects the port and a bounding box is generated. The coordinates of bounding box is used to get the approximate center of the charging port. 

### Libraries Used
##### Pytorch
### Dataset creation
##### The images of the CCS2 charging port were taken from various angles and at different distances from the camera, for the purpose of capturing a comprehensive representation of the port's design and appearance. 

### Data Augumentions
##### Adaptive Equalization
##### Filter null (8%)
##### Grayscaling
##### Vertical and horizontal flip
##### Rotation ( -25 to +25 degrees )
##### Vertican shear ( -13 to +13 degress )
##### Horizontal shear ( -9 to +9 degrees )
##### Hue ( -25 to 25 degrees )
##### Exposure ( -12% to 12% )
##### Cutout ( 4 boxes with 30% size each )

### Dataset split
##### Training dataset - 99%
##### Validation dataset - 0.5%
##### Test dataset - 0.5%

### Accuracy - 92%+
##### Maximum mAP50-90 = 0.995



## Charging Port Orientation
##### This folder contains the Custom CNN Regression model and dataset made by us for estimating the orientation of the Charging Port.

### Libraries Used  
##### TensorFlow Keras, Pandas, Numpy

### Dataset Creation and augumentation
##### CCS2 port images have been taken from a distance of 20cm from the camera at various yaw angles (-30 to 30 with a difference of 0.5 degrees each), pitch angles (0, 2, 4 and -10 degrees) and with zero roll (because other than 0 degree, other angles for roll not possible). Images have been augumented with a rotation angle of range 3 degrees, width and height shift range of 0.05 and zoom range from 75% to 100%.

### Dataset Split
##### Training Dataset - 99.9%
##### Validation Dataset - 0.05%
##### Test Dataset - 0.05%

### Model Architecture
##### Conv2D layer with 16 filters, 15 kernel size and relu activation function.
##### Conv2D layer with 32 filters, 12 kernel size and relu activation function.
##### Conv2D layer with 64 filters, 9 kernel size and relu activation function.
##### Conv2D layer with 60 filters, 6 kernel size and relu activation function.
##### Conv2D layer with 16 filters, 15 kernel size and relu activation function.
##### Dense layer with 128 neurons and relu activation function.
##### Dense layer with 64 neurons and relu activation function.
##### Dense layer with 500 neurons, relu activation function and l2 regularization with lambda = 0.01.
##### Dense output layer with 3 neurons and linear activation function.
##### 4 batch normalization layer and MaxPooling2D((2,2)) layer.
##### 2 Dropout layers

### Optimizer Used
##### Adam with learning rate = 0.000005

### Loss function
##### Mean Sqaured Error

### Error
##### Mean Yaw Error ~ 0.61 degree
##### Mean Pitch Error ~ 0.66 degree
##### Mean Roll Error ~ 0.11 degree
##### Mean Error ~ 0.46 degree


## Charging Port Gazebo
##### This folder contains all the files required for simulating the Charging port in gazebo environment.
