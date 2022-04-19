# CameraCalibration_RealTime
This is a software that applies real time calibration to cameras.
## Purpose
This software developed for calibration. 
Thanks to this, in the vision of camera all of the same objects has same pixel size.
The system can specify the exactly coordinates of the objects.
## Environment
This software developed with:
•	Linux operating system 
•	C++ programming language
•	Qt Creator IDE
•	OpenCV library
•	QML 
## Restrictions
User needs a chessboard.
Camera vision restriction: Chessboard must appear completely.
Number of chessboard corners and chessboard dimension should increase simultaneously with the dimensions of camera vision.
Number of x and y axis corners must be specified correct in code otherwise software does not work.
## Paths followed by the algorithm
1. Detect chessboard, if there is no chessboard warn the user.
2. Draw corners and measure the differences between the corners.
3. Figure out which side should be compressed.
4. Compress 5 pixel and measure again.
5. If still there are too differences, keep compressing.

## Computation and Application
Before the calibration:
![image](https://user-images.githubusercontent.com/86148100/163863073-3a4a3f02-72f8-4b4e-bba6-a7be99ce44e3.png)
 After the calibration:
![image](https://user-images.githubusercontent.com/86148100/163863128-7b941374-7725-4f4e-b973-f4661f239b85.png)

## Results with Hough
Before calibration (right image) the same circles have a difference of 7 pixels in radius.
After calibration (left image) there is only 1 pixel missing.
![Hough](https://user-images.githubusercontent.com/86148100/163863667-049460d0-acc2-43e9-99ab-f4a15f685e7b.png)

## Results with Canny Edge Detector and minAreaRect() function of OpenCV
This is another example showing before and after calibration.
![Canny](https://user-images.githubusercontent.com/86148100/163863966-8e3dc4e8-7856-4951-ac4c-b250c543f49a.png)


