#ifndef IMAGEPAN_H
#define IMAGEPAN_H

#include <QMainWindow>
#include <QFileDialog>
#include <QColorDialog>
#include <QUndoStack>
#include <QToolBar>
#include <QRubberBand>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "ImageProcessing.h"
#include "display.h"
#include "imagefiltering.h"
#include "commands.h"
#include "equalizer.h"
#include "previewcommands.h"
#include "histogram.h"
#include "scene.h"
#include "mousewindowscene.h"

//OpenCV
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;

namespace Ui {
class ImagePan;
}

class ImagePan : public QWidget
{
    Q_OBJECT

public:
    explicit ImagePan(QWidget *parent = 0);
    ~ImagePan();
    Ui::ImagePan *ui;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    Display *MyDisplay;
    bool setImage(const QString &);
    bool saveFile(QString &);
    void zoomIn();
    void zoomOut();
    void scaleImage(double);
    void adaptImage();
    void normalize();
    void setScaleFactor(double);
    bool setPixmapView(bool);
    QPixmap getHistogramPixmap();
    void setPreviewOut(Mat);
    Mat getPreviewOut();
    Mat getPreviewIn();
    double getScaleFactor();
    QUndoStack *undoStack;
    Mat getHistogramMat(const Mat);
    bool eventFilter(QObject *obj, QEvent *event);
    QStringList getPixelValues();
    bool visualizePixelStatus();
    void showHistogramDiagram(const Mat);
    void previewAccept();
    Scene *scene;
    QGraphicsView *myGraphicsView;
    void cancelROI();

public slots:
    bool processImage(const int);
    bool equalizeImage();
    QImage updateQImageToDispay();
    void setMoveCursor();
    void selectROI();
    void extractROI();

signals:
    void modified();
    void changeDisplay();

protected:


private slots:

private:
    Mat imageROI;
    double scaleFactor;
    ImageProcessing *ImgExample;
    Histogram1D *MyHistogram;
    Mat img_mat;
    QAction *undoAction;
    QAction *redoAction;
    QToolBar* editToolBar;
    QPixmap img_pix;
    QPixmap pix_hist;
    int equalized;
    Mat previewIn;
    Mat previewOut;
    QGraphicsItem* item;
    QStringList pixelValues;
    bool visualize;
    QRubberBand *rubberBand;
    QPoint origin;
    QTableWidgetItem *tableWidgetItem;
};

#endif // IMAGEPAN_H
