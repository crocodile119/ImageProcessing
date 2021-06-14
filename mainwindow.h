#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QFileDialog>
#include <QColorDialog>
#include <QUndoStack>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QComboBox>
#include <ImageProcessing.h>
#include <Display.h>
#include <QSplitter>
#include <QHelpEngine>
#include <QHelpContentWidget>
#include <QHelpIndexWidget>
#include <QHelpSearchQueryWidget>
#include "commands.h"
#include "imagepan.h"
#include <showhistogram.h>
#include <ui_showhistogram.h>
#include <QMouseEvent>
#include <QTextEdit>
#include "informationpanel.h"
#include "controldock.h"
#include "imagefiltering.h"
#include "helpbrowser.h"

//OpenCV
#include <opencv/cv.h>
#include <opencv/highgui.h>


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    enum processAlg {NO_FILTERING, SOBEL_FILTERING, CANNY_EDGE, LAPLACIAN_FILTERING, BLUR_FILTERING,
                    GAUSSIANBLUR_FILTERING, MEDIANBLUR_FILTERING, BILATERALBLUR_FILTERING,
                     POWER_TRANSFORM, SLICE_TRANSFORM, SALT_AND_PEPPER_NOISE, ERODE_MORPHO, DILATE_MORPHO,
                     OPENCLOSE_MORPHO};
public:
    explicit MainWindow();
    ~MainWindow();
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    bool eventFilter (QObject *, QEvent *);
    void activateChoose(bool);
    void setDockHistogram();

signals:
    void imageForMouseChanged();

private slots:
    bool save();
    bool saveAs();
    void open();
    bool loadFile(const QString &);
    void openRecentFile();
    void about();
    void imageModified();
    void undo();
    void redo();
    void showHistogramDiagram();
    void equalize();
    void zoomIn();
    void zoomOut();
    void adapt();
    void normalize();
    void createHelpWindow();
    void setImageView();
    void setOffImageView();
    void dragMode();
    void negativeTransform();
    void logTransform();
    void invTransform();
    void fourierTransform();
    void previewAccept();
    void previewReject();
    void sobelFilter();
    void cannyFilter();
    void blurFilter();
    void laplacianFilter();
    void gaussianFilter();
    void medianFilter();
    void morphoOperation();
    void otherFilters();
    void bilateralFilter();
    void on_SobelChanged();
    void on_cannyChanged();
    void on_laplacianChanged();
    void on_otherFiltersChanged();
    void on_powerTransformChanged();
    void on_sliceLevelChanged();
    void on_morphoOperationChanged();
    void erodeOperation();
    void dilateOperation();
    void on_openCloseOperationChanged();
    void openCloseOperation();
    void sliceLevelTransform();
    void powerTransform();
    void filterAccepted();
    void filterRejected();
    void selectROI();
    void extractROI();
    void setRoiExtractEnabled();
    void canceltROI();
    void updateHistogram();

    //informationPanel
    void on_saltAndPepperNoise();
    void on_gaussianNoise();
    void on_reduceLevel();
    void processingAccepted();
    void processingRejected();
    void on_reduceGreyLevels();

private:
    enum { MaxRecentFiles = 8 };
    QString imageName; //the name of image from QFileDialog
    QString curFile; //the name of the imma per the app (before saveAs is an empty string)
    bool modified;

    void setCurrentFile(const QString &);
    QString strippedName(const QString &); //get the filename stripped (only the file name without the complete path)
    QUndoStack *undoStack;
    QComboBox *comboBox;
    ImageProcessing *ImgExample;
    Display *MyDisplay;
    Mat img_mat;
    QPixmap img_pix;

    QStringList recentFiles;
    //the central widget
    ImagePan *imagePan;
    int currentIndex;
    Mat openClosePreview;

    //private functions
    bool okToContinue();
    void createActions();
    void createContextMenu();
    void createStatusBar();
    int filteringImage(enum processAlg);
    QString getImageDetails(bool);
    void filterButtonsEnabled(bool);
    void filterButtonsChecked(bool);
    void updateRecentFileActions();
    void readSettings();
    void writeSettings();

    processAlg filter;

    QTextEdit *dockTextEdit; //textedit widget for the command History dockwidget
    //Action for menu
    QAction *openAct;
    QAction *saveAsAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;
    QAction *negativeAct;
    QAction *logAct;
    QAction *invLogAct;
    QAction *fourierTransformAct;
    QAction *erodeAct;
    QAction *dilateAct;
    QAction *openCloseAct;
    QAction *histogramAct;
    QAction *undoAction;
    QAction *redoAction;
    QAction *equalizeAct;
    QAction *dockInformationAct;
    QAction *mainPanelDockAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalizeAct;
    QAction *dragAct;
    QAction *cannyAct;
    QAction *laplacianAct;
    QAction *blurAct;
    QAction *gaussianBlurAct;
    QAction *medianBlurAct;
    QAction *bilateralBlurAct;
    QAction *sobelAct;
    QAction *adaptAct;
    QAction *powerTransformAct;
    QAction *sliceTransformAct;
    QAction *applyFilterAct;
    QAction *rejectFilterAct;
    QAction *roiSelectAct;
    QAction *roiExtractAct;
    QAction *imageViewAct;
    QAction *helpAction;


    QMouseEvent *mouseEvent;
//I treat ControlDock object as pubblic because It is a part of MainWindow widget
    ControlDock *mainPanelDock;
    InformationPanel *informationPanel;
    ImageFiltering *myImageFiltering;

// help

    QDockWidget *helpWindow;
};

#endif // MAINWINDOW_H
