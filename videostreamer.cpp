#include "videostreamer.h"
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core/core_c.h>
#include <stdio.h>
#include <iostream>
#include<math.h>
#include <fstream>



using namespace std;
using namespace cv;


//Necessary definings.
Mat frame,minAR,minAR1,output1;
Mat cannyOutput,medianImg,output;
//lambda1 matrix for apply perspective seperately from function.
Mat lambda1( 2, 4, CV_32FC1 );
//Arrays for store the distance between corners.
int difference_y[266],difference_x[270];
short int situation;
//Define values for perspective measurements.
int a=0,b=0,c=-1,d=0,e=-1,f=-1,g=0,h=-1;

cv::RNG rng(12345);
//Define values for minAreRect measurement.
int x,y,z;
vector<std::vector<cv::Point> > contours;
vector<cv::Vec4i> hierarchy;
int count1 = 0;

int counta=0,countf=0,countd=0,countw=0;
string txtpathload;
string txtpathsave;
int getstat1;
int countaa=0,counts=0,countload=0;
int returnvalue;
int button;
int Lessthan = 110;
int Biggerthan =75;
int CannyTH=80;



//Chessboard corner points.
vector<Point2f> corner_pts;
// Input Quadilateral or Image plane coordinates
Point2f inputQuad[4];
// Output Quadilateral or World plane coordinates
Point2f outputQuad[4];


VideoStreamer::VideoStreamer()
{
    connect(&tUpdate,&QTimer::timeout,this,&VideoStreamer::streamVideo);

}

VideoStreamer::~VideoStreamer()
{
    cap.release();
    tUpdate.stop();
    threadStreamer->requestInterruption();
}

int VideoStreamer::GetDimensions(Mat &input, vector<Point2f> &corner_pts, int difference_y[], int difference_x[],int stat)
{
    //int CHECKERBOARD[2]{15,19},a,i=0;
    //int CHECKERBOARD[2]{6,9},a,i=0;
     int x1=0;
     int x2=0;
    if(stat == 1){
     x1=6;
     x2=9;
    }
    if(stat == 2){
     x1=15;
     x2=19;
    }

    int CHECKERBOARD[2]{x1,x2},a,i=0;
    Mat gray;
    bool success;
        cvtColor(input,gray,COLOR_BGR2GRAY);


        success = findChessboardCorners(gray,Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, CALIB_CB_ADAPTIVE_THRESH  | CALIB_CB_FILTER_QUADS);
        TermCriteria criteria( TermCriteria::EPS | TermCriteria::MAX_ITER, 30, 0.001);
        if(success ==1){
            cout<<"Chessboard successfully found."<<endl;
        // refining pixel coordinates for given 2d points.
        cornerSubPix(gray,corner_pts,Size(11,11), Size(-1,-1),criteria);

        // Displaying the detected corner points on the checker board
        //drawChessboardCorners(input, Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, success);
}
        else {
            cout<<"Could not find the chessboard."<<endl;
            return -1;
        }
        string string1;
if(stat == 2){
        int b=0,c=0;
        a = corner_pts.size();
        for(i; i < (a-1);i++){
            //Eliminate the cross lines.
            if(i==14 || i==29 || i==44 || i== 59 || i== 74 || i== 89 || i==104 || i==119|| i==134|| i==149|| i==164|| i==179|| i==194|| i==209|| i==224|| i==239|| i==254|| i==269|| i==284|| i==299 || i ==314||i==329){
                continue;
            }
            difference_y[b] = corner_pts[i].y - corner_pts[i+1].y;  //Find the difference between two corners on y-axis.
            string1 = to_string(difference_y[b]);
            //putText(input,string1,Point((corner_pts[i].x + corner_pts[i+1].x)/2 ,(corner_pts[i].y + corner_pts[i+1].y)/2),FONT_HERSHEY_DUPLEX,1.2,CV_RGB(125,125,125),1);
            b++;
        }


        i = 0;
        for(i; i < (a-15);i++){
            difference_x[c] = corner_pts[i+15].x - corner_pts[i].x;  //Find the difference between two corners on x-axis.
            string1 = to_string(difference_x[c]);
            //putText(input,string1,Point((corner_pts[i+6].x + corner_pts[i].x)/2 ,(corner_pts[i+6].y + corner_pts[i].y)/2),FONT_HERSHEY_DUPLEX,1.2,CV_RGB(125,125,125),1);
            c++;
        }
}

if(stat == 1){
        int b=0,c=0;
        a = corner_pts.size();
        for(i; i < (a-1);i++){
            //Eliminate the cross lines.
            if(i==5 || i==11 || i==17 || i== 23 || i== 29 || i== 35 || i==41 || i==47){
                continue;
            }
            difference_y[b] = corner_pts[i].y - corner_pts[i+1].y;  //Find the difference between two corners on y-axis.
            string1 = to_string(difference_y[b]);
            putText(input,string1,Point((corner_pts[i].x + corner_pts[i+1].x)/2 ,(corner_pts[i].y + corner_pts[i+1].y)/2),FONT_HERSHEY_DUPLEX,0.7,CV_RGB(125,125,125),1);
            b++;
        }


        i = 0;
        for(i; i < (a-6);i++){
            difference_x[c] = corner_pts[i+6].x - corner_pts[i].x;  //Find the difference between two corners on x-axis.
            string1 = to_string(difference_x[c]);
            putText(input,string1,Point((corner_pts[i+6].x + corner_pts[i].x)/2 ,(corner_pts[i+6].y + corner_pts[i].y)/2),FONT_HERSHEY_DUPLEX,0.7,CV_RGB(125,125,125),1);
            c++;
        }
}
        return 1;
}

void VideoStreamer::GetPers(Mat &input, Mat &output, short situation)
{
    // Input Quadilateral or Image plane coordinates
    Point2f inputQuad[4];
    // Output Quadilateral or World plane coordinates
    Point2f outputQuad[4];

    // Lambda Matrix
    Mat lambda( 2, 4, CV_32FC1 );
    static int a=0,b=0,c=-1,d=0,e=-1,f=-1,g=0,h=-1;
    // Set the lambda matrix the same type and size as input
    lambda = Mat::zeros( input.rows, input.cols, input.type() );

    switch(situation){
        case 1:
            e += 5;
            g -= 5;
            break;
        case 2:
            a -= 5;
            c += 5;
            break;
        case 3:
            b -=5;
            h +=5;
            break;
        case 4:
            d -= 5;
            f += 5;
            break;
        case 5:
             a=0;
             b=0;
             c=-1;
             d=0;
             e=-1;
             f=-1;
             g=0;
             h=-1;
             break;

    }

    // The 4 points that select quadilateral on the input , from top-left in clockwise order

    inputQuad[0] = Point2f( a,b );
    inputQuad[1] = Point2f( input.cols+c,d);
    inputQuad[2] = Point2f( input.cols+e,input.rows+f);
    inputQuad[3] = Point2f( g,input.rows+h);


    // The 4 points where the mapping is to be done , from top-left in clockwise order
    outputQuad[0] = Point2f( 0,0 );
    outputQuad[1] = Point2f( input.cols-1,0);
    outputQuad[2] = Point2f( input.cols-1,input.rows-1);
    outputQuad[3] = Point2f( 0,input.rows-1  );


    // Get the Perspective Transform Matrix i.e. lambda
    lambda = getPerspectiveTransform( inputQuad, outputQuad );
    // Apply the Perspective Transform just found to the src image
    warpPerspective(input,output,lambda,output.size() );
}

int VideoStreamer::CalcHypotenuse(int x, int y)
{
    int z;
    x *= x;
    y *= y;
    z = sqrt(x+y);
    return z;
}

void VideoStreamer::DrawDimensions(Mat &input, vector<Point2f> corner_pts, int difference_y[], int difference_x[],int stat)
{
    //int CHECKERBOARD[2]{15,19};
    //int CHECKERBOARD[2]{6,9};
    int x1=0;
    int x2=0;
   if(stat == 1){
    x1=6;
    x2=9;
   }
   if(stat == 2){
    x1=15;
    x2=19;
   }

   int CHECKERBOARD[2]{x1,x2};
    Mat gray;
    bool success1;
        cvtColor(input,gray,COLOR_BGR2GRAY);


        success1 = findChessboardCorners(gray,Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, CALIB_CB_ADAPTIVE_THRESH  | CALIB_CB_FILTER_QUADS);
        TermCriteria criteria( TermCriteria::EPS | TermCriteria::MAX_ITER, 30, 0.001);
        if(success1 ==1){
        // refining pixel coordinates for given 2d points.
        cornerSubPix(gray,corner_pts,Size(11,11), Size(-1,-1),criteria);

        // Displaying the detected corner points on the checker board
        drawChessboardCorners(input, Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, success1);
        }
}

void VideoStreamer::findCValues()    // c
{

    counta = !counta;
}
void VideoStreamer::apply()
{

    countaa = !countaa;
}

void VideoStreamer::cannyT(QString path3)
{
    CannyTH = path3.toInt();
}

void VideoStreamer::save(QString path5)
{
    counts = !counts;
    txtpathsave = path5.toStdString();
}

void VideoStreamer::load(QString path4)
{
    countload = !countload;
    txtpathload = path4.toStdString();
}

void VideoStreamer::getstat(QString path6)
{
    getstat1 = path6.toInt();
}


void VideoStreamer::draw()
{
    countd = !countd;
}

void VideoStreamer::obDet()
{
    countf = !countf;
}

void VideoStreamer::less(QString path1)
{
    Lessthan = path1.toInt();

}

void VideoStreamer::bigger(QString path2)
{
    Biggerthan = path2.toInt();
}




void VideoStreamer::streamVideo()
{

    if(frame.data)
    {
        QImage img = QImage(frame.data,frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped();
        emit newImage(img);
    }
}

void VideoStreamer::catchFrame(cv::Mat emittedFrame)
{
    frame = emittedFrame;
}

void VideoStreamer::openVideoCamera(QString path)
{
//    if(path.length() == 1)
//    cap.open(path.toInt());
//    else
//    cap.open(path.toStdString());
    cap.open(-1);

    VideoStreamer* worker = new VideoStreamer();
    worker->moveToThread(threadStreamer);
    QObject::connect(threadStreamer,SIGNAL(started()),worker,SLOT(streamerThreadSlot()));
    QObject::connect(worker,&VideoStreamer::emitThreadImage,this,&VideoStreamer::catchFrame);
    threadStreamer->start();

    double fps = cap.get(cv::CAP_PROP_FPS);
    tUpdate.start(1);
}

void VideoStreamer::streamerThreadSlot()
{
    ofstream file;

    while (1) {
        cap>>frame;


        if(counta){
            cout<<"girdia"<<endl;
            counta=0;
            //Define a matrix for use it here.
            GetPers(frame,frame,5);
            a=0;
            b=0;
            c=-1;
            d=0;
            e=-1;
            f=-1;
            g=0;
            h=-1;

            //Take the measurement.
            returnvalue = GetDimensions(frame,corner_pts,difference_y,difference_x,getstat1);

            //Check the measurement for detect which edges needs to compress.
            //Do it same process for 4 situation(right, left, up and down).

            //If There is a difference, start the process.
        if(returnvalue == 1){
            if((difference_y[0]-difference_y[13])>1 || (difference_x[0]-difference_x[14])>1){
                situation = 1;
                while(1){
                        //Apply the perspective process until the measurements get equal.
                        GetPers(frame,frame,situation);
                        //Take feedbacks.
                        e+=5;
                        g-=5;
                        cout<<"e"<<e<<endl;
                        cout<<"g"<<g<<endl;
                        //Take dimensions for check again.
                        returnvalue=GetDimensions(frame,corner_pts,difference_y,difference_x,getstat1);
                        if(returnvalue == -1)
                            break;
                        //Finish the process if dimensions are equal.
                        if((difference_y[0]-difference_y[13])<2 || (difference_x[0]-difference_x[14])<2)
                            break;
                }
            }

            if((difference_y[0]-difference_y[13])<(-1) || (difference_x[0]-difference_x[14])<(-1)){
                situation = 2;
                while(1){
                        GetPers(frame,frame,situation);
                        a -= 5;
                        c += 5;
                        cout<<"a"<<a<<endl;
                        cout<<"c"<<c<<endl;
                        returnvalue=GetDimensions(frame,corner_pts,difference_y,difference_x,getstat1);
                        if(returnvalue == -1)
                            break;
                        if((difference_y[0]-difference_y[13])>(-2) || (difference_x[0]-difference_x[14])>(-2))
                            break;
                }
            }

            if((difference_y[7]-difference_y[258])>1 || (difference_x[7]-difference_x[261])>1){
                situation = 3;
                while(1){
                        GetPers(frame,frame,situation);
                        b -=5;
                        h +=5;
                        cout<<"b"<<b<<endl;
                        cout<<"h"<<h<<endl;
                        returnvalue=GetDimensions(frame,corner_pts,difference_y,difference_x,getstat1);
                        if(returnvalue == -1)
                            break;
                        if((difference_y[7]-difference_y[258])<2 || (difference_x[7]-difference_x[261])<2)
                            break;
                }
            }

            if((difference_y[7]-difference_y[258])<(-1) || (difference_x[7]-difference_x[261])<(-1)){
                situation = 4;
                while(1){

                    GetPers(frame,frame,situation);
                    d -= 5;
                    f += 5;
                    cout<<"d"<<d<<endl;
                    cout<<"f"<<f<<endl;
                    returnvalue=GetDimensions(frame,corner_pts,difference_y,difference_x,getstat1);
                    if(returnvalue == -1)
                        break;
                    if((difference_y[7]-difference_y[258])>(-2) || (difference_x[7]-difference_x[261])>(-2))
                        break;
                }
            }

            //Normalizing perspective values.
            if(a != 0)
                a *= 5;
            if(b != 0)
                b *= 5;
            if(c != -1)
                c *= 5;
            if(d != 0)
                d *= 5;
            if(e != -1)
                e *= 5;
            if(f != -1)
                f *= 5;
            if(g != 0)
                g *= 5;
            if(h != -1)
                h *= 5;

        }

        }

        //WarpPerspective part.
        if(countaa){
         cout<<"girdiaaaa"<<endl;
        lambda1 = Mat::zeros( frame.rows, frame.cols, frame.type() );

        //Input points values for perspective.
        inputQuad[0] = Point2f( a,b );
        inputQuad[1] = Point2f( frame.cols+c,d);
        inputQuad[2] = Point2f( frame.cols+e,frame.rows+f);
        inputQuad[3] = Point2f( g,frame.rows+h);

        //Output points values perspective.
        outputQuad[0] = Point2f( 0,0 );
        outputQuad[1] = Point2f( frame.cols-1,0);
        outputQuad[2] = Point2f( frame.cols-1,frame.rows-1);
        outputQuad[3] = Point2f( 0,frame.rows-1  );

        //Calculate lambda1 matrix.
        lambda1 = getPerspectiveTransform( inputQuad, outputQuad );

        //Finally, apply perspective with using WarpPerspective.
        warpPerspective(frame,frame,lambda1,frame.size() );


     }

        //Save Part.
        if(counts){
            counts = 0;
            cout<<"saved."<<endl;
            file.open (txtpathsave + "/saved.txt");  ///home/selim/Documents/saved.txt
            file<<a<<endl;
            file<<b<<endl;
            file<<c<<endl;
            file<<d<<endl;
            file<<e<<endl;
            file<<f<<endl;
            file<<g<<endl;
            file<<h<<endl;
            file.close();
        }
        //Load part.
        if(countload){
            countload = 0;
            cout<<"load."<<endl;
            ifstream input_file(txtpathload);
            input_file >>a>>b>>c>>d>>e>>f>>g>>h;

            cout<<a<<endl;
            cout<<b<<endl;
            cout<<c<<endl;
            cout<<d<<endl;
            cout<<e<<endl;
            cout<<f<<endl;
            cout<<g<<endl;
            cout<<h<<endl;
        }

        if(countd){
            cout<<"girdid"<<endl;
            DrawDimensions(frame,corner_pts,difference_y,difference_x,getstat1);

        }

        if(countf){
            cout<<"girdif"<<endl;
            //Begin minAreaRect process.
            //cropped = frame(Range(corner_pts[14].y,corner_pts[255].y),Range(corner_pts[14].x,corner_pts[269].x));
            minAR = frame(Range(corner_pts[14].y,corner_pts[255].y),Range(corner_pts[14].x,corner_pts[269].x));
            frame = minAR.clone();
            cvtColor(minAR,minAR,cv::COLOR_BGR2GRAY);
            //Median Filter.
            medianBlur(minAR,medianImg,1);
            //Canny Process.
            //Second threshold should be 3 times of first threshold.
            Canny(medianImg,cannyOutput,CannyTH,3*CannyTH,3,0);
            //imshow("Canny",cannyOutput);
            //Find contours of objects.
            findContours(cannyOutput,contours,hierarchy,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
            vector<cv::RotatedRect> minRect( contours.size() );


            for( size_t i = 0; i< contours.size(); i++ )
            {
                //Load the corners points of objects.
                minRect[i] = cv::minAreaRect( contours[i] );
                cv::Point2f rect_points[4];
                minRect[i].points( rect_points );

                //If the contour length bigger than 90 apply sign process.
                if(contours[i].size()>90)
                {
                    //Find middle of objects.
                    int centerX = (rect_points[0].x + rect_points[2].x)/2;
                    int centerY = (rect_points[0].y + rect_points[2].y)/2;
                    //Find x and y length of first and second points of objects.
                    x = rect_points[1].x - rect_points[0].x;
                    y = rect_points[1].y - rect_points[0].y;
                    //Calculate hypotenuse.
                    z = CalcHypotenuse(x,y);
                    //Write the distance on image.
                    if(z <Lessthan  && z > Biggerthan){
                    putText(frame,to_string(z),Point(centerX,centerY),FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),2);


                    //Draw rectangles around of objects.
                    for ( int j = 0; j < 4; j++ )
                    {
                        line( frame, rect_points[j], rect_points[(j+1)%4], cv::Scalar(0,0,255),3 );
                    }
                    count1++;
                    }
                }
            }
            //imshow("minAreaRect",output);
            cout<<count1<<" object signed."<<endl;
            count1 = 0;
        }






        if(frame.data)
            emit emitThreadImage(frame);

        if(QThread::currentThread()->isInterruptionRequested())
        {
            cap.release();
            return;
        }

    }
}
