#if !defined HISTOGRAM
#define HISTOGRAM

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

class Histogram1D {

  private:

    int histSize[1];
    float hranges[2];
    const float* ranges[1];
    int channels[1];

  public:

    Histogram1D();
    virtual ~Histogram1D ();

    // Sets the channel on which histogram will be calculated.
    // By default it is channel 0.
    void setChannel(int);

    // Gets the channel used.
    int getChannel();

    // Sets the range for the pixel values.
    // By default it is [0,255]
    void setRange(float, float);

    // Gets the min pixel value.
    float getMinValue();

    // Gets the max pixel value.
    float getMaxValue();

    // Sets the number of bins in histogram.
    // By default it is 256.
    void setNBins(int);

    // Gets the number of bins in histogram.
    int getNBins();

    // Computes the 1D histogram.
    Mat getHistogram(const Mat &);

    // Computes the 1D histogram and returns an image of it.
    Mat getHistogramImage(const Mat &);

    // Equalizes the source image.
    Mat equalize(const Mat &);

    // Stretches the source image.
    Mat stretch(const Mat &, int );

    // Applies a lookup table transforming an input image into a 1-channel image
    Mat applyLookUp(const Mat& image, const Mat& lookup);
};


#endif
