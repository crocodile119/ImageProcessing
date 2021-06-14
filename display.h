#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QImage>
#include <QPixmap>

using namespace std;
using namespace cv;

class Display
{
public:

  explicit Display ();
  virtual ~Display ();
  void setQtImage (Mat);
  QImage getQtImage ();
  void setCvMat (Mat);
  Mat getCvMat();
  QPixmap setOutputPixmap();
  QSize getImageDimension();

private:
  QImage QtImage;
  Mat CvMatImage;
  QPixmap outputPixmap;
};

#endif // DISPLAY_H
