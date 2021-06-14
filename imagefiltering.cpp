#include "imagefiltering.h"

ImageFiltering::ImageFiltering()
{
}

Mat ImageFiltering::sobelFiltering(Mat& img, const int ksize, const double scale, const double delta)
{
    Mat src_gray, out;
    int ddepth= CV_16S;

    GaussianBlur(img, src_gray, Size(3, 3), 0, 0, BORDER_DEFAULT);

    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
    Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);

    // converting back to CV_8U
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);

    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, out);

    return out;
}

void ImageFiltering::setSmoothing(const int _smoothing)
{
    if(_smoothing==0)
        smooth=GAUSSIAN;
    else if(_smoothing==1)
        smooth=BLUR;
    else if(_smoothing==2)
        smooth=MEDIAN;
}

Mat ImageFiltering::laplacianFiltering(Mat& img, const int sigma)
{
/*Equazione laplaciana
            g(x,y)= f(x,y) + c [  delta^2 f((x,y)  )]
            va sommata l'immagine di partenza per ovviare algli effetti
            delle derivate seconde sulle zone omogenee */

    int smoothType = smooth;

    Mat smoothed, laplace, out;
    int ksize = (sigma*5)|1;

    if(smoothType == GAUSSIAN)
        GaussianBlur(img, smoothed, Size(ksize, ksize), sigma, sigma);
    else if(smoothType == BLUR)
        blur(img, smoothed, Size(ksize, ksize));
    else
        medianBlur(img, smoothed, ksize);

    Laplacian(smoothed, laplace, CV_16S, 5);
    convertScaleAbs(laplace, out, (sigma+1)*0.25);

    return out;
}

Mat ImageFiltering::blurFiltering(Mat& img, const int kernelLenght)
{
    Mat out;
    for (int i = 1; i < kernelLenght; i = i + 2)
        blur( img, out, Size( i, i ), Point(-1,-1) );

    return out;
}

Mat ImageFiltering::gaussianBlurFiltering(Mat& img, const int kernelLenght)
{
    Mat out;
    for ( int i = 1; i < kernelLenght; i = i + 2 )
        GaussianBlur( img, out, Size( i, i ), 0, 0 );

    return out;
}

Mat ImageFiltering::medianBlurFiltering(Mat& img, const int kernelLenght)
{
    Mat out;
    for ( int i = 1; i < kernelLenght; i = i + 2 )
        medianBlur ( img, out, i );

    return out;
}

Mat ImageFiltering::bilateralBlurFiltering(Mat& img, const int kernelLenght)
{
    Mat out;
    for (int i = 1; i < kernelLenght; i = i + 2)
        bilateralFilter ( img, out, i, i*2, i/2 );

    return out;
}

Mat ImageFiltering::powProcessing(Mat& img, const double exponent, const double coefficient)
{
    Mat out;
    Mat out1, img1;
    img.convertTo(img1, CV_32F);

    cv::pow(img1, exponent, out1);
    out1=coefficient*out1;
    //normalize(out1, out1, cv::NORM_MINMAX);
    out1.convertTo(out, CV_8U);
    return out;
}

Mat ImageFiltering::levelSlicing(Mat& img, bool binary, int lowLevel, int highLevel)
{
    //Mat out;
    Mat dst = img.clone();

    for(int i=0; i<dst.cols; i++)
    {
        for (int j=0; j<dst.rows; j++)
        {
            if((dst.at<uchar>(j,i)>highLevel)&&(dst.at<uchar>(j,i)<lowLevel))
                dst.at<uchar>(j,i)= 255;
            else
            {
                if(binary)
                    dst.at<uchar>(j,i)=0;
            }
        }
    }
    return dst;
}

Mat ImageFiltering::addSaltAndPepperNoise(Mat& img, int n)
{
    Mat dst;
    dst=img.clone();
    for (int k=0; k<n; k++)
    {
        // rand() is the MFC random number generator
        // try qrand() with Qt
        int i= rand()%dst.cols;
        int j= rand()%dst.rows;
        // gray-level image
        dst.at<uchar>(j,i)= 0;
    }
    return dst;
}

Mat ImageFiltering::addGaussianNoise(Mat& img, double mean, double variance)
{
    Mat dst;
    dst=img.clone();
    Mat result;
    cv::Mat noise = Mat(dst.size(),CV_64F);
    normalize(dst, result, 0.0, 1.0, CV_MINMAX, CV_64F);
    cv::randn(noise, mean, variance);
    result = result + noise;
    normalize(result, result, 0, 255, CV_MINMAX, CV_8U);

    return result;
}

Mat ImageFiltering::getBitPlaneSlices(Mat& img, QString level)
{
    Mat dst;
    dst=img.clone();
    int cols = dst.cols;
    int rows = dst.rows;
    uchar levelNumber=0;

    Mat out(rows, cols, CV_8U, Scalar(0));
    if(level=="2 livelli")
        levelNumber=2;
    else if(level=="4 livelli")
        levelNumber=4;
    else if(level=="8 livelli")
        levelNumber=8;
    else if(level=="16 livelli")
        levelNumber=16;
    else if(level=="32 livelli")
        levelNumber=32;
    else if(level=="64 livelli")
        levelNumber=64;
    else if(level=="128 livelli")
        levelNumber=128;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            out.at<uchar>(i, j) = (dst.at<uchar>(i, j) & levelNumber) ? uchar(255) : uchar(0);
    }
    return out;
}

Mat ImageFiltering::findEdgeCanny(Mat& img, const int kernel_size, const int ratio)
{
    Mat src;
    src=img.clone();
    Mat dst, detected_edges;

    int lowThreshold = 0;

    blur( src, detected_edges, Size(3,3) );
    Canny( detected_edges, detected_edges, lowThreshold, ratio, kernel_size );
    return detected_edges;
}

Mat ImageFiltering::colorReduce(Mat& img, const int div)
{
    Mat dst;
    Mat out;
    dst=img.clone();
     // obtain iterator at initial position
    cv::Mat_<uchar>::iterator it=dst.begin<uchar>();
    // obtain end position
    cv::Mat_<uchar>::iterator itend=dst.end<uchar>();
    // loop over all pixels
    for ( ; it!= itend; ++it)
    {
        // process each pixel --------------------
        (*it)= (*it)/div*div + div/2;
        // end of pixel processing ---------------
         out=dst;
    }
   return out;
}

Mat ImageFiltering::erodeImage(Mat& img, const int iterations)
{
    Mat dst;
    Mat out;
    dst=img.clone();
    cv::erode(dst,out,cv::Mat(),cv::Point(-1,-1),iterations);
    return out;
}

Mat ImageFiltering::dilateImage(Mat& img, const int iterations)
{
    Mat dst;
    Mat out;
    dst=img.clone();
    cv::dilate(dst,out,cv::Mat(),cv::Point(-1,-1),iterations);
    return out;
}

Mat ImageFiltering::openingImage(Mat& img, const int openIterations)
{
    Mat dst;
    Mat out;
    dst=img.clone();

    cv::erode(dst,dst,cv::Mat(),cv::Point(-1,-1),openIterations);
    cv::dilate(dst,out,cv::Mat(),cv::Point(-1,-1),openIterations);

    return out;
}

Mat ImageFiltering::closingImage(Mat& img, const int closeIterations)
{
    Mat dst;
    Mat out;
    dst=img.clone();

    cv::dilate(dst,dst,cv::Mat(),cv::Point(-1,-1),closeIterations);
    cv::erode(dst,out,cv::Mat(),cv::Point(-1,-1),closeIterations);

    return out;
}

ImageFiltering::~ImageFiltering()
{

}
