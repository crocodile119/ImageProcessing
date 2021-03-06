#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include "histogram.h"

using namespace cv;
using namespace std;

Histogram1D::Histogram1D()
{
    // Prepare arguments for 1D histogram
    histSize[0]= 256;
    hranges[0]= 0.0;
    hranges[1]= 255.0;
    ranges[0]= hranges;
    channels[0]= 0; // by default, we look at channel 0
}

Histogram1D::~Histogram1D() {}

// Sets the channel on which histogram will be calculated.
// By default it is channel 0.

void Histogram1D::setChannel(int c)
{
    channels[0]= c;
}

// Gets the channel used.
int Histogram1D::getChannel()
{
    return channels[0];
}

/* Sets the range for the pixel values.
     By default it is [0,255]*/

void Histogram1D::setRange(float minValue, float maxValue)
{
    hranges[0]= minValue;
    hranges[1]= maxValue;
}

// Gets the min pixel value.
float Histogram1D::getMinValue()
{
    return hranges[0];
}

// Gets the max pixel value.
float Histogram1D::getMaxValue()
{
    return hranges[1];
}

/* Sets the number of bins in histogram.
     By default it is 256.*/

void Histogram1D::setNBins(int nbins)
{
    histSize[0]= nbins;
}

// Gets the number of bins in histogram.
int Histogram1D::getNBins()
{
    return histSize[0];
}

// Computes the 1D histogram.
Mat Histogram1D::getHistogram(const Mat &image)
{
    Mat hist;
// Compute histogram
    calcHist(&image,
            1,			// histogram of 1 image only
            channels,	// the channel used
            Mat(),	// no mask is used
            hist,		// the resulting histogram
            1,			// it is a 1D histogram
            histSize,	// number of bins
            ranges		// pixel value range
            );

    return hist;
}

// Computes the 1D histogram and returns an image of it.
Mat Histogram1D::getHistogramImage(const Mat &image)
{
// Compute histogram first
    Mat hist= getHistogram(image);
// Get min and max bin values
    double maxVal=0;
    double minVal=0;
    minMaxLoc(hist, &minVal, &maxVal, 0, 0);

// Image on which to display histogram
    Mat histImg(histSize[0], histSize[0], CV_8U,Scalar(255));

// set highest point at 90% of nbins
    int hpt = static_cast<int>(0.9*histSize[0]);

// Draw vertical line for each bin
    for(int h = 0; h < histSize[0]; h++)
    {
        float binVal = hist.at<float>(h);
        int intensity = static_cast<int>(binVal*hpt/maxVal);
        line(histImg, Point(h,histSize[0]), Point(h,histSize[0]-intensity), Scalar::all(0));
    }
    return histImg;
}

// Equalizes the source image.
    cv::Mat Histogram1D::equalize(const Mat &image) {

    Mat result;        equalizeHist(image,result);

        return result;
    }

    // Stretches the source image.
    Mat Histogram1D::stretch(const Mat &image, int minValue=0) {

        // Compute histogram first
        Mat hist= getHistogram(image);

        // find left extremity of the histogram
        int imin= 0;
        for( ; imin < histSize[0]; imin++ ) {
            //std::cout<<hist.at<float>(imin)<<std::endl;
            if (hist.at<float>(imin) > minValue)
                break;
        }

        // find right extremity of the histogram
        int imax= histSize[0]-1;
        for( ; imax >= 0; imax-- ) {

            if (hist.at<float>(imax) > minValue)
                break;
        }

        // Create lookup table
        int dims[1]={256};
        Mat lookup(1,dims,CV_8U);

        for (int i=0; i<256; i++) {

            if (i < imin) lookup.at<uchar>(i)= 0;
            else if (i > imax) lookup.at<uchar>(i)= 255;
            else lookup.at<uchar>(i)= static_cast<uchar>(255.0*(i-imin)/(imax-imin)+0.5);
        }

        // Apply lookup table
        Mat result;
        result= applyLookUp(image,lookup);

        return result;
    }

    // Applies a lookup table transforming an input image into a 1-channel image
    Mat Histogram1D::applyLookUp(const Mat& image, const Mat& lookup) {

        // Set output image (always 1-channel)
        Mat result(image.rows,image.cols,CV_8U);
        Mat_<uchar>::iterator itr= result.begin<uchar>();

        // Iterates over the input image
        Mat_<uchar>::const_iterator it= image.begin<uchar>();
        Mat_<uchar>::const_iterator itend= image.end<uchar>();

        // Applies lookup to each pixel
        for ( ; it!= itend; ++it, ++itr) {

            *itr= lookup.at<uchar>(*it);
        }

        return result;
    }

