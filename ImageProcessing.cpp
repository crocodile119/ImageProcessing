#include "ImageProcessing.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <QDebug>

using namespace cv;
using namespace std;

// Constructors/Destructors
//  

ImageProcessing::ImageProcessing (Display* _imageDisplay)
{
    imageDisplay =_imageDisplay;
}

ImageProcessing::~ImageProcessing ()
{
}

void ImageProcessing::cannyProcessing(Mat& img, Mat& out)
{
    Q_UNUSED(img);
    Q_UNUSED(out);
}

void ImageProcessing::powProcessing(Mat& img, Mat& out)
{
    Mat out1, img1;
    img.convertTo(img1, CV_32F);
    cv::pow(img1, 1.5, out1);
    out1.convertTo(out, CV_8U);
}

void ImageProcessing::sqrtProcessing(Mat& img, Mat& out)
{
    Mat out1, img1;
    img.convertTo(img1, CV_32F);
    cv::pow(img1, 0.8, out1);
    out1.convertTo(out, CV_8U);
}

void ImageProcessing::logProcessing(Mat& img, Mat& out)
{
    const double logNormalize = 255/(std::log(255));
    Mat img1=img.clone();
    Mat fg;
    img1.convertTo(fg,CV_32F);
    fg = fg + 1;

    cv::Mat_<float>::iterator it=fg.begin<float>();
    cv::Mat_<float>::iterator itend=fg.end<float>();

    for ( ; it!= itend; ++it)
        (*it)= std::log(*it)*logNormalize;

    fg.convertTo(out, CV_8U);
}

void ImageProcessing::invProcessing(Mat& img, Mat& out)
{
    const double logNormalize = 255/(std::log(255));
    Mat img1=img.clone();
    Mat fg;
    img1.convertTo(fg,CV_32F);
    fg = fg + 1;

    cv::Mat_<float>::iterator it=fg.begin<float>();
    cv::Mat_<float>::iterator itend=fg.end<float>();

    for ( ; it!= itend; ++it)
    {
        (*it)=255-(*it);
        (*it)=std::log(1/(*it)+1)*logNormalize+ 254;
    }
    normalize(fg, fg, 0, 255, NORM_MINMAX);
    fg.convertTo(out, CV_8U);
}

void ImageProcessing::negativeProcessing(Mat& img, Mat& out)
{
    Mat dst = img.clone();
    cv::Mat_<uchar>::iterator it=dst.begin<uchar>();

    cv::Mat_<uchar>::iterator itend=dst.end<uchar>();

    for ( ; it!= itend; ++it)
        (*it)= 255-(*it);

    out=dst;
}

void ImageProcessing::fourierTransform(Mat& img)
    {
    Mat src = img.clone();

    // get optimal size of DFT
    int optimalRows = getOptimalDFTSize(src.rows);
    int optimalCols = getOptimalDFTSize(src.cols);
    Scalar s0 = cv::Scalar::all(0);
    Mat padded;
    copyMakeBorder(src, padded, 0, optimalRows - src.rows, 0,
                      optimalCols - src.cols, BORDER_CONSTANT, s0);

    // use cv.MatVector to distribute space for real part and imaginary part
    Mat plane0;
    padded.convertTo(plane0, CV_32F);
    vector<cv::Mat> planes;
    Mat complexI;
    Mat plane1 = cv::Mat::zeros(padded.rows, padded.cols, CV_32F);
    planes.push_back(plane0);
    planes.push_back(plane1);
    merge(planes, complexI);

    // in-place dft transform
    dft(complexI, complexI);

    // compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))
    split(complexI, planes);

    magnitude(planes.at(0), planes.at(1), planes.at(0));
    Mat mag = planes.at(0);
    Mat m1 = cv::Mat::ones(mag.rows, mag.cols, mag.type());
    add(mag, m1, mag);
    log(mag, mag);

    // crop the spectrum, if it has an odd number of rows or columns
    if(mag.cols%2!=0)
        mag.cols=mag.cols-1;

    if(mag.rows%2!=0)
        mag.rows=mag.rows-1;

    cv::Rect rect = Rect(0, 0, mag.cols, mag.rows);
    mag = mag(rect);

    // rearrange the quadrants of Fourier image
    // so that the origin is at the image center
    int cx = mag.cols / 2;
    int cy = mag.rows / 2;
    Mat tmp;

    cv::Rect rect0 = Rect(0, 0, cx, cy);
    cv:: Rect rect1 = Rect(cx, 0, cx, cy);
    cv::Rect rect2 = Rect(0, cy, cx, cy);
    cv::Rect rect3 = Rect(cx, cy, cx, cy);

    Mat q0 = mag(rect0);
    Mat q1 = mag(rect1);
    Mat q2 = mag(rect2);
    Mat q3 = mag(rect3);

    // exchange 1 and 4 quadrants
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    // exchange 2 and 3 quadrants
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    qDebug() << "Size of the transformed image:"  << "(" << mag.rows << ", " <<
                 mag.cols << ") ";
    // The pixel value of CV_32S type image ranges from 0 to 1.
    normalize(mag, mag, 0, 1, NORM_MINMAX);

    imshow("Trasformata di Fourier", mag);


    // show the image
}

void ImageProcessing::setDisplay (Display* _Display)
{
    imageDisplay=_Display;
}

Display* ImageProcessing::getDisplay ()
{
    return imageDisplay;
}
