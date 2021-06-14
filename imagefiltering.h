#ifndef IMAGEFILTERING_H
#define IMAGEFILTERING_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ImageProcessing.h"

using namespace cv;
class ImageFiltering
{  
public:
    enum smoothing {GAUSSIAN, BLUR, MEDIAN};

public:
    explicit ImageFiltering();
    ~ImageFiltering();
    Mat sobelFiltering(Mat &, const int, const double, const double);
    void setSmoothing(const int);
    Mat laplacianFiltering(Mat &, const int);
    Mat findEdgeCanny(Mat& img, const int, const int);
    Mat blurFiltering(Mat&, const int);
    Mat gaussianBlurFiltering(Mat&, const int);
    Mat medianBlurFiltering(Mat&, const int);
    Mat bilateralBlurFiltering(Mat&, const int); 
    Mat levelSlicing(Mat&, bool, int, int);

    //In relatà si tratta di una trasformazione puntuale che per adesso piazzo qui.
    Mat powProcessing(Mat&,  const double, const double);
    Mat addSaltAndPepperNoise(Mat& img, int n);
    Mat addGaussianNoise(Mat&, double, double);
    Mat getBitPlaneSlices(Mat&, QString);
    Mat colorReduce(Mat& img, const int);
    Mat erodeImage(Mat&, const int);
    Mat dilateImage(Mat&, const int);
    Mat openingImage(Mat&, const int);
    Mat closingImage(Mat&, const int);


private:
    int MAX_KERNEL_LENGTH = 31;
    smoothing smooth;
};

#endif // IMAGEFILTERING_H
