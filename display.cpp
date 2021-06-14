#include "display.h"

// Constructors/Destructors
//  

Display::Display()
{
}

void Display::setQtImage (Mat cvMatInput)
{

    QtImage= QImage((const unsigned char*)cvMatInput.data,static_cast<int>(cvMatInput.cols), static_cast<int>(cvMatInput.rows), static_cast<int>(cvMatInput.step), QImage::Format_RGB888);

    //For Binary Images
    if (QtImage.isNull())
    {
        //ColorTable for Binary Images
        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; i++)
            colorTable.push_back(qRgb(i, i, i));

        QtImage = QImage((const unsigned char*)cvMatInput.data,cvMatInput.cols, cvMatInput.rows, QImage::Format_Indexed8);
        QtImage.setColorTable(colorTable);
    }
}


QImage Display::getQtImage ()
{
    return QtImage;
}


void Display::setCvMat (Mat _CvMatImage)
{
    CvMatImage = _CvMatImage;
}

Mat Display::getCvMat()
{
  return CvMatImage;
}

QPixmap Display::setOutputPixmap()
{
    outputPixmap=QPixmap::fromImage(QtImage.rgbSwapped());     //BGR to RGB
    return outputPixmap;
}

QSize Display::getImageDimension()
{
    return QtImage.size();
}


Display::~Display()
{
}
