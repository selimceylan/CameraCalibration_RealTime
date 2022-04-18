#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <QObject>
#include <QTimer>
#include <opencv2/highgui.hpp>
#include <QImage>
#include <iostream>
#include <QThread>

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core/core_c.h>
#include <stdio.h>
#include <iostream>
#include<math.h>

using namespace std;
using namespace cv;

static cv::VideoCapture cap;



class VideoStreamer: public QObject
{
    Q_OBJECT
public:
    VideoStreamer();
    ~VideoStreamer();

public:


    void streamVideo();
    QThread* threadStreamer = new QThread();
    void catchFrame(cv::Mat emittedFrame);

public slots:
    void openVideoCamera(QString path);

    int GetDimensions(Mat &input,vector<Point2f> &corner_pts,int difference_y[266],int difference_x[270],int stat);
    void GetPers(Mat &input, Mat &output,short int situation);
    int CalcHypotenuse(int x, int y);
    void DrawDimensions(Mat &input,vector<Point2f> corner_pts,int difference_y[266],int difference_x[270],int stat);

    void findCValues();
    void draw();
    void obDet();
    void less(QString path1);
    void bigger(QString path2);
    void apply();
    void cannyT(QString path3);
    void save(QString path5);
    void load(QString path4);
    void getstat(QString path6);

    void streamerThreadSlot();

private:
    cv::Mat frame;
    QTimer tUpdate;

signals:
    void newImage(QImage &);

    void emitThreadImage(cv::Mat frameThread);
};

#endif // VIDEOSTREAMER_H
