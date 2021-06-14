#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <string>
#include "Display.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class ImageProcessing
{
public:

  // Constructors/Destructors

  ImageProcessing (Display *_imageDisplay);

  virtual ~ImageProcessing ();

  void cannyProcessing(Mat&, Mat&);

  void powProcessing(Mat&, Mat&);
  void sqrtProcessing(Mat&, Mat&);
  void logProcessing(Mat&, Mat&);
  void invProcessing(Mat&, Mat&);
  void negativeProcessing(Mat&, Mat&);
  void fourierTransform(Mat&);
  void setDisplay (Display *);
  Display* getDisplay ();


private:
Mat inputImage;
Mat outputImage;
Display *imageDisplay;
};

#endif // IMAGEPROCESSING_H
