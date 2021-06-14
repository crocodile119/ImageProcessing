#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QDockWidget>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPolygonF>
#include <QMouseEvent>
#include <QRect>
#include <QPaintEngine>
#include <QProgressDialog>
#include "imagepan.h"
#include "ui_imagepan.h"
#include "roidialog.h"
#include "ui_roidialog.h"
#include "ImageProcessing.h"
#include "scene.h"

ImagePan::ImagePan(QWidget *parent) :
    QWidget(parent), ui(new Ui::ImagePan), scaleFactor(1)
{
    ui->setupUi(this);

    MyDisplay=new Display();
    ImgExample=new ImageProcessing(MyDisplay);
    MyHistogram = new Histogram1D();
    undoStack = new QUndoStack(this);
    scene = new Scene(this);

    scene->rubberBandDragEnabled(false);
    ui->graphicsView->setScene(scene);
    myGraphicsView = new QGraphicsView();
    myGraphicsView=ui->graphicsView;
    myGraphicsView->setMouseTracking(true);

    myGraphicsView->setBackgroundRole(QPalette::Base);
    myGraphicsView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    qApp->installEventFilter(this);

      connect(scene, SIGNAL(selected()),this, SLOT(setMoveCursor()));
      connect(this, SIGNAL(changeDisplay()), this, SLOT(updateQImageToDispay()));
}

ImagePan::~ImagePan()
{
    delete ui;
    delete MyDisplay;
    delete ImgExample;
    delete myGraphicsView;
    delete MyHistogram;
    delete undoStack;
    delete scene;
}

bool ImagePan::setImage(const QString &fileName)
{
    Mat img_mat = imread(fileName.toStdString(),0); //0 for grayscale
    if(img_mat.data == NULL)
        return false;

    undoStack->clear();
    equalized = 0;

    MyDisplay->setCvMat(img_mat);
    MyDisplay->setQtImage(img_mat);

    img_pix = MyDisplay->setOutputPixmap();
    ImgExample->setDisplay(MyDisplay);
    scene->clear();
    scene->addPixmap(img_pix);
    scene->update();
    return true;
}

bool ImagePan::saveFile(QString &fileName)
{
    QImage QtImage=MyDisplay->getQtImage();
    QtImage=QtImage.rgbSwapped();

    if(QtImage.save(fileName, 0,-1))
        return true;
    else
        return false;
}

Mat ImagePan::getHistogramMat(const Mat _image)
{
    Mat result;
    result=MyHistogram->getHistogramImage(_image);
    return result;
}

void ImagePan::showHistogramDiagram(const Mat _inputImage)
{
Mat image = getHistogramMat(_inputImage);

QImage qt_hist = QImage((const unsigned char*)image.data,static_cast<int>(image.cols), static_cast<int>(image.rows), static_cast<int>(image.step), QImage::Format_Indexed8);
pix_hist=QPixmap::fromImage(qt_hist);
}

bool ImagePan::setPixmapView(bool preview)
{
    Mat imageToDisplay;
    if(previewOut.data==NULL)
        return false;

    if(preview)
        imageToDisplay=previewOut;
    else
        imageToDisplay=MyDisplay->getCvMat();

    QImage displayQtImage = QImage((const unsigned char*)imageToDisplay.data, static_cast<int>(imageToDisplay.cols),
                                   static_cast<int>(imageToDisplay.rows), static_cast<int>(imageToDisplay.step), QImage::Format_Indexed8);
    img_pix=QPixmap::fromImage(displayQtImage);
    scene->clear();
    scene->addPixmap(img_pix);
    scene->update();
    emit changeDisplay();
    return true;
}

QPixmap ImagePan::getHistogramPixmap()
{
    return pix_hist;
}

bool ImagePan::equalizeImage(){

    if((equalized==0)||(undoStack->index()==0))
    {
        ++equalized;
        qDebug()<< undoStack->index();
        emit modified();
        undoStack->push(new ImageEqualizerCommand(ImgExample, scene));
        showHistogramDiagram(MyDisplay->getCvMat());
        return true;
    }
    else
    return false;
}

bool ImagePan::processImage(const int currentIndex)
{
    equalized=0;
    undoStack->push(new ImageProcessingCommand(ImgExample, currentIndex, scene));
    emit modified();
    return true;
}

void ImagePan::previewAccept()
{
    equalized=0;
    undoStack->push(new PreviewCommand(ImgExample, previewOut, scene));
    emit modified();
}

void ImagePan::zoomIn()
{
    scaleImage(1.25);
}

void ImagePan::zoomOut()
{
    scaleImage(0.8);
}

void ImagePan::scaleImage(double factor)
{
    scaleFactor *= factor;
    myGraphicsView->scale(factor,factor);
}

void ImagePan::adaptImage()
{
    normalize();
    QSize myGraphics=myGraphicsView->size();
    int x1=myGraphics.height();
    int y1=myGraphics.width();
    int x2=img_pix.height();
    int y2=img_pix.width();

    double factor;

    double xFactor= (double) x1/x2;
    double yFactor= (double) y1/y2;

    if(xFactor>yFactor)
        factor=yFactor;
    else
        factor=xFactor;

    myGraphicsView->scale(factor,factor);
    scaleFactor *= factor;
}

void ImagePan::normalize()
{
    scaleImage(1/scaleFactor);
}

double ImagePan::getScaleFactor()
{
    return scaleFactor;
}

void ImagePan::setScaleFactor(double _factor)
{
    scaleFactor=_factor;
}

QImage ImagePan::updateQImageToDispay()
{
    return img_pix.toImage();
}

bool ImagePan::eventFilter(QObject *obj,QEvent *event)
{
    if (event->type() ==  QEvent::GraphicsSceneMouseMove )
    {
        QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
        QPointF img_coord_pt = mouseEvent->scenePos();

        int x = img_coord_pt.x();
        int y = img_coord_pt.y();
        QImage img_q=updateQImageToDispay();

        QColor color = QColor(img_q.pixel(x,y));
        int average = (color.red()+color.green()+color.blue())/3;

        QSize img_pixSize=img_pix.size();
        int xSide = img_pixSize.width();
        int ySide = img_pixSize.height();

        QString X;
        QString Y;
        QString level;

        if((x<=xSide) && (y<=ySide))
        {
            X=QString::number(x);
            Y=QString::number(y);
            level =QString::number(average);
            visualize = true;
        }
        else
        {
            X="";
            Y="";
            level="";
            visualize = false;
        }

    pixelValues= QStringList() << "Coordinate: ("+X+", " << Y+") Livello di grigio: " <<level;
    return QObject::eventFilter(obj, event);
    }
    else
        return QObject::eventFilter(obj, event);
}

QStringList ImagePan::getPixelValues()
{
    return pixelValues;
}

bool ImagePan::visualizePixelStatus()
{
    return visualize;
}


void ImagePan::setPreviewOut(Mat _previewOut)
{
    previewOut= _previewOut;
}

Mat ImagePan::getPreviewIn()
{
    return previewIn;
}

Mat ImagePan::getPreviewOut()
{
    return previewOut;
}

void ImagePan::selectROI()
{
    scene->rubberBandDragEnabled(true);
    myGraphicsView->setCursor(Qt::CrossCursor);
    scene->update();
}

void ImagePan::extractROI()
{
    myGraphicsView->setCursor(Qt::ArrowCursor);
    //double pixWidth =double(img_pix.width());
    QRectF rectangleCoordinates =scene->getRectangleCoordinates();
    QRectF pictureRectangle=QRectF(0, 0, img_pix.width(), img_pix.height());
    qDebug() << "Rettangolo immagine: " << pictureRectangle;
    qDebug() << "I rettangoli si intersecano: "<< pictureRectangle.intersects(rectangleCoordinates);
    rectangleCoordinates=pictureRectangle.intersected(rectangleCoordinates);
    qDebug() << "Rettangolo intersezione: " << rectangleCoordinates;

    int rectangleHeight = rectangleCoordinates.height();
    int rectangleWidth = rectangleCoordinates.width();
    QPointF rectangleTopLeft = rectangleCoordinates.topLeft();
    int x = rectangleTopLeft.x();
    int y = rectangleTopLeft.y();

    Mat image=MyDisplay->getCvMat();
    imageROI= image(cv::Rect(x,y,rectangleWidth,rectangleHeight));

    QProgressDialog progress(this);
    progress.setLabelText(tr("Processing..."));
    progress.setRange(0, rectangleHeight-1);
    progress.setModal(true);
    Mat histROI = getHistogramMat(imageROI);
    QImage qt_hist = QImage((const unsigned char*)histROI.data,static_cast<int>(histROI.cols), static_cast<int>(histROI.rows), static_cast<int>(histROI.step), QImage::Format_Indexed8);
    QPixmap hist_Roi=QPixmap::fromImage(qt_hist);

    RoiDialog dialog(this);
    dialog.ui->tableWidget->setRowCount(rectangleHeight);
    dialog.ui->tableWidget->setColumnCount(rectangleWidth);
    dialog.ui->roiHistogramLabel->setPixmap(hist_Roi);
    dialog.ui->roiHistogramLabel->setScaledContents(true);
    dialog.ui->roiHistogramLabel->setFrameShape(QFrame::Box);

    for (int i=0; i<rectangleHeight; ++i)
    {
        progress.setValue(i);
        qApp->processEvents();

        if(progress.wasCanceled())
        {
            scene->deleteSelection();
            scene->reset();
            scene->update();
            return;
        }

        for (int j=0; j<rectangleWidth; ++j){
        /* to check the pixels of the image for debug
         * qDebug()<< "Image pixel: " << imageROI.at<uchar>(i,j);
        */
        tableWidgetItem= new QTableWidgetItem(QString::number(imageROI.at<uchar>(i,j)));
        dialog.ui->tableWidget->setItem(i,j,tableWidgetItem);
        // end of pixel processing ---------------
        }
    }
    QImage qtRoiImage = QImage((const unsigned char*)imageROI.data,static_cast<int>(imageROI.cols), static_cast<int>(imageROI.rows), static_cast<int>(imageROI.step), QImage::Format_Indexed8);

    QPixmap pixmapRoiImage =QPixmap::fromImage(qtRoiImage);
    dialog.ui->label->setPixmap(pixmapRoiImage.scaled(dialog.ui->label->size(), Qt::KeepAspectRatio));
    dialog.qtRoiPixmap=pixmapRoiImage;

    double minRoi;
    double maxRoi;
    Point minLoc;
    Point maxLoc;
    int imageX=imageROI.cols;
    int imageY= imageROI.rows;
    minMaxLoc(imageROI, &minRoi, &maxRoi, &minLoc, &maxLoc);

    QString roiDetailsString="<font size=4><b>Dimensioni immagine: </b></>";
    roiDetailsString.append("(");
    roiDetailsString.append(QString::number(imageX));
    roiDetailsString.append(", ");
    roiDetailsString.append(QString::number(imageY));
    roiDetailsString.append(tr(")<br>"));
    roiDetailsString.append("<font size=3>Massimo valore livello di grigio: ");
    roiDetailsString.append(QString::number(maxRoi));
    roiDetailsString.append(" @ (");
    roiDetailsString.append(QString::number(minLoc.x));
    roiDetailsString.append(", ");
    roiDetailsString.append(QString::number(minLoc.y));
    roiDetailsString.append(")");
    roiDetailsString.append("<br>");
    roiDetailsString.append("<font size=3>Minimo valore livello di grigio: ");
    roiDetailsString.append(QString::number(minRoi));
    roiDetailsString.append(" @ (");
    roiDetailsString.append(QString::number(maxLoc.x));
    roiDetailsString.append(", ");
    roiDetailsString.append(QString::number(maxLoc.y));
    roiDetailsString.append(")");

    dialog.ui->roiDetails->setText(roiDetailsString);

    if (dialog.exec())
    {
        scene->deleteSelection();
        scene->reset();
        scene->update();
    }
    scene->update();
}

void ImagePan::cancelROI()
{
    if(scene->checkSelection())
    {
        scene->deleteSelection();
        scene->update();
    }
    myGraphicsView->setCursor(Qt::ArrowCursor);
    scene->reset();
    scene->rubberBandDragEnabled(false);
}

void ImagePan::setMoveCursor()
{
    myGraphicsView->setCursor(Qt::SizeAllCursor);
}
