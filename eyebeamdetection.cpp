#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <stdlib.h>
#include <time.h>

using namespace cv;
using namespace std;

const int beamRadNum = 10;
const int colorNum = 5;
const double lineLength = 500;
class EyeUpper {
public:
  bool operator()(const Rect& a, const Rect& b) const {
    return a.y < b.y;
  }
};

int randomColor()
{
  double r = rand();
  return (int)(r / RAND_MAX * colorNum);
}

int randomRad()
{
  double r = rand();
  return (int)(r / RAND_MAX * beamRadNum );
}


int main(void)
{
  srand((unsigned int)time(NULL));
  std::string faceCascadeName
    = "/opt/local/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml";
  CascadeClassifier faceClassfier;
  if (!faceClassfier.load(faceCascadeName))
    return -1;
  
  std::string eyeCascadeName
    = "/opt/local/share/OpenCV/haarcascades/haarcascade_eye.xml";
  CascadeClassifier eyeClassfier;
  if (!eyeClassfier.load(eyeCascadeName))
    return -1;
  
  VideoCapture cap(0);
  if(!cap.isOpened())
    return -1;
  
  std::vector<double> beamRad;
  for (int i = 0; i < beamRadNum; i++) {
    beamRad.push_back(2 * M_PI / beamRadNum * i);
  }
  std::vector<Scalar> colors; // bgr
  colors.push_back(Scalar(0xb6, 0x43, 0x7a));
  colors.push_back(Scalar(0x1d, 0x26, 0x9d));
  colors.push_back(Scalar(0x0d, 0xc4, 0xff));
  colors.push_back(Scalar(0x5f, 0x32, 0xc3));
  colors.push_back(Scalar(0x46, 0xa5, 0x46));
  
  namedWindow("cam",1);
  for(;;)
  {
    Mat frame;
    Mat result;
    cap >> frame;
    Mat grayImg;
    cvtColor(frame, grayImg, CV_BGR2GRAY);
    std::vector<Rect> faces;
    faceClassfier.detectMultiScale(grayImg, faces,
                                   1.1, 3,
                                   CV_HAAR_SCALE_IMAGE, 
                                   cv::Size(50,50));
    if (faces.size() > 0) {
      std::vector<Rect> eyes;
      Mat faceImg = Mat(grayImg, faces[0]);
      eyeClassfier.detectMultiScale(faceImg, eyes,
                                    1.1, 5,
                                    CV_HAAR_SCALE_IMAGE, 
                                    cv::Size(10,10));
      if (eyes.size() >= 2) {
        std::sort(eyes.begin(), eyes.end(), EyeUpper());
        Rect upperEye = eyes[0];
        Rect lowerEye = eyes[1];
        
        double ang = beamRad[randomRad()];
        Scalar color = colors[randomColor()];
        cout << randomRad() << endl;
        cout << ang << endl;
        vector<Point> upperLine(2);
        vector<Point> lowerLine(2);
        
        upperLine[0] = Point(faces[0].x + upperEye.x + upperEye.width / 2.0,
                             faces[0].y + upperEye.y + upperEye.height / 2.0);
        upperLine[1] = Point(faces[0].x + upperEye.x + upperEye.width / 2.0
                             + lineLength * cos(ang),
                             faces[0].y + upperEye.y + upperEye.height / 2.0
                             + lineLength * sin(ang));
        lowerLine[0] = Point(faces[0].x + lowerEye.x + lowerEye.width / 2.0,
                             faces[0].y + lowerEye.y + lowerEye.height / 2.0);
        lowerLine[1] = Point(faces[0].x + lowerEye.x + lowerEye.width / 2.0
                             + lineLength * cos(ang),
                             faces[0].y + lowerEye.y + lowerEye.height / 2.0
                             + lineLength * sin(ang));
        line(frame, upperLine[0], upperLine[1], color, 5, 5);
        line(frame, lowerLine[0], lowerLine[1], color, 5, 5);
      }
    }
    imshow("cam", frame);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
