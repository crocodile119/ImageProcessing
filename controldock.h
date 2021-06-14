#ifndef CONTROLDOCK_H
#define CONTROLDOCK_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QImage>
#include <QPixmap>
#include <QWidget>
#include <QDockWidget>
#include <QGridLayout>
#include <QLabel>
#include <QEvent>
#include <QGroupBox>
#include <QScrollBar>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QTextEdit>
#include <QRadioButton>

class ControlDock: public QDockWidget

{
        Q_OBJECT

public:
    explicit ControlDock();
    ~ControlDock();
    void setOutputPix(QPixmap &);
    void changeEvent(QEvent *);
    void setContenentsImage();
    void setImageDetails(QString);
    void setAllControlsEnabled(bool);
    //sobel member functions
    int getKSize();
    double getScale();
    double getDelta();
    void setSobelGroupBoxVisible(bool);
    //laplacian member functions
    int getSigma();
    int getSmoothing();
    void setLaplacianGroupBoxVisible(bool);
    //other filters
    int getOtherFiltersParameter();
    void setOtherFiltersGroupBoxVisible(bool);
    void setGroupBoxTitle(QString);
    //slice transform slots
    bool getBinaryCheck();
    int getHighLevel();
    int getLowLevel();
    void setSliceGroupBoxVisible(bool);
    void activateControls(bool);
    //power transform
    void setPowerGroupBoxVisible(bool);
    double getPowerCoefficient();
    double getPowerExponent();
    //erode dilate
    int getIterationsParameter();
    void setMorphoGroupBoxVisible(bool);
    void setMorphoGroupBoxTitle(QString);
    //opening closing
    void setOpenCLoseBoxVisible(bool _visible);
    int getOpenParameter();
    int getCloseParameter();
    int getOpeningClosingComboOption();

    //cancel button
    void setCancelButtonVisible(bool);
    //histogram
    void setHistogramVisible();
    void setHistogram(QPixmap);
    // history
    void setTextHistory(QString);
    void seTextHistoryVisible(bool);
    bool textHistoryVisible();

    QPushButton *rejectButton;
    QPushButton *acceptButton;

signals:
    void sobelChanged();
    void cannyChanged();
    void laplacianChanged();
    void otherFiltersChanged();
    void powerTransformChanged();
    void sliceChanged();
    void morphoChanged();
    void openCloseChanged();
    void rejected();
    void acceped();
    void cancel();

public slots:

    //buttons slots
    void on_acceptButton_clicked();
    void on_rejectButton_clicked();
    //cancel button
    void on_cancelButton_clicked();
    //sobel slots
    void on_kSizeSpinBox_valueChanged(int);
    void on_scaleSpinBox_valueChanged(double);
    void on_deltaSpinBox_valueChanged(double);
    //canny slots
    void on_kernelSizeSpinBox_valueChanged(int value);
    void on_ratioSpinBox_valueChanged(int _value);
    int getKernelSize();
    int getRatio();
    void setCannyGroupBoxVisible(bool _visible);
    //laplacian slots
    void on_sigmaSpinBox_valueChanged(int value);
    void on_smoothingComboBox_currentIndexChanged(int value);
    //other filters
    void on_otherFiltersScrollBar_valueChanged(int value);
    //slicing level
    void on_lowSpinBox_valueChanged(int value);
    void on_highSpinBox_valueChanged(int value);
    void on_binaryCheckBox_clicked();
    //erode dilate
    void on_morphoScrollBar_valueChanged(int value);
    //power transform slots
    void on_exponentSpinBox_valueChanged(double);
    void on_coefficientSpinBox_valueChanged(double);
    //opening closing
    void on_openSpinBox_valueChanged(int value);
    void on_closeSpinBox_valueChanged(int value);
    void on_openingClosigCombo_currentIndexChanged(int value);

private:
    //private global variables
    double exponent;
    double coefficient;

    //dockwidget
    QWidget *controlDockWidget;
    QTextEdit *mainPanelTextEdit;
    QPixmap outputImage;
    QGridLayout *gridLayoutDock;
    QGridLayout *textEditGridLayout;
    QLabel *controlDockLabel;
    QLabel *histogramDockLabel;
    QLabel *imageDetails;
    QVBoxLayout *verticalLayout;
    int currentIndex;

    //sobel controls
    QGridLayout *sobelGridLayout;
    QGroupBox *sobelGroupBox;
    QLabel *KSizeLabel;
    QSpinBox *kSizeSpinBox;
    QLabel *ScaleLabel;
    QDoubleSpinBox *scaleSpinBox;
    QLabel *deltaLabel;
    QDoubleSpinBox *deltaSpinBox;

    //canny controls
    QGridLayout *cannyGridLayout;
    QGroupBox *cannyGroupBox;
    QLabel *kernelSizeLabel;
    QSpinBox *kernelSizeSpinBox;
    QLabel *ratioLabel;
    QSpinBox *ratioSpinBox;

    //laplacian controls
    QGroupBox *laplacianGroupBox;
    QGridLayout *laplacianGridLayout;
    QLabel *sigmaLabel;
    QHBoxLayout *sigmaLayout;
    QSpinBox *sigmaSpinBox;
    QComboBox *smoothingComboBox;
    QHBoxLayout *comboBoxLayout;
    QLabel *smoothingLabel;

    //other filter controls
    QGroupBox *otherFiltersGroupBox;
    QGridLayout *otherFiltersGridLayout;
    QLabel *scrollLabel;
    QHBoxLayout *scrollLayout;
    QLabel *scrollDisplayLabel;
    QScrollBar *otherFiltersScrollBar;

    //power transform control
    QGroupBox *powerGroupBox;
    QGridLayout *powerGridLayout;
    QLabel *exponentLabel;
    QDoubleSpinBox *exponentSpinBox;
    QLabel *coefficientLabel;
    QDoubleSpinBox *coefficientSpinBox;

    //reduce grey levels
    QGroupBox *morphoGroupBox;
    QGridLayout *morphoGridLayout;
    QLabel *morphoLabel;
    QLabel *morphoDisplayLabel;
    QScrollBar *morphoScrollBar;

    //opening closing controls
    QGroupBox *openCloseGroupBox;
    QGridLayout *openCloseGridLayout;
    QLabel *openLabel;
    QSpinBox *openSpinBox;
    QLabel *closeLabel;
    QSpinBox *closeSpinBox;
    QComboBox *openingClosingCombo;

    //push buttons
    QSpacerItem *leftSpacer;
    QSpacerItem *rightSpacer;
    QGridLayout *buttonsGridLayout;

    //cancel button
    QWidget *cancelWidget;
    QGridLayout *cancelGridLayout;
    QPushButton *cancelButton;

    //level slicing
    QWidget *buttonsWidget;
    QGroupBox *slicingGroupBox;
    QGridLayout *slicingGridLayout;
    QCheckBox *binaryCheckBox;
    QLabel *lowLevelLabel;
    QHBoxLayout *lowLevelLayout;
    QSpinBox *lowSpinBox;
    QLabel *highLevelLabel;
    QSpinBox *highSpinBox;

    //text history
};

#endif // CONTROLDOCK_H
