#ifndef INFORMATIONPANEL_H
#define INFORMATIONPANEL_H

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
#include <QTreeWidget>

using namespace cv;

class InformationPanel:public QDockWidget

{
        Q_OBJECT

public:
    explicit InformationPanel();
    ~InformationPanel();
    void setTreeWidgetVisible(bool status);
    void setSingleParameterGroupBoxVisible(bool);
    //void setInformationTextEditVisible(bool);
    int getSingleParameterValue();
    void setGroupBoxTitle(QString title);
    void setDialogButtonsVisible(bool status);
//gaussian noise group
    double getMean();
    double getVariance();
    void setGaussNoiseBoxVisible(bool);
//reduce grey levels
    void setreduceGreyGroupBoxVisible(bool);
    int getReduceGreyParameterValue();

    QPushButton *rejectButton;
    QPushButton *acceptButton;

//bit plane slices
    QString getLevel();

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *, int);
    void on_singleParameterScrollBar_valueChanged(int);

//buttons
    void on_acceptButton_clicked();
    void on_cancelButton_clicked();

//gaussian noise group
    void on_meanSpinBox_valueChanged(double);
    void on_varianceSpinBox_valueChanged(double);
// reduce grey levels group
    void on_reduceGreyScrollBar_valueChanged(int);

signals:
    void singleParameterChanged();
    void accept();
    void cancel();
    void addingGaussianNoise();
    void planeChanged();
    void redeuceGreyChanged();

private:
//dockwidget
    QWidget *informationDockWidget;
    //QTextEdit *informationTextEdit;
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;

//single parameter controls
    QGroupBox *singleParameterGroupBox;
    QGridLayout *singleParameterGridLayout;
    QLabel *scrollLabel;
    QHBoxLayout *scrollLayout;
    QLabel *scrollDisplayLabel;
    QScrollBar *singleParameterScrollBar;
    QSpacerItem *singleParameterVerticalSpacer;
//gaussian noise controls
    QGridLayout *gaussNoiseGridLayout;
    QGroupBox *gaussNoiseGroupBox;
    QLabel *meanLabel;
    QDoubleSpinBox *meanSpinBox;
    QLabel *varianceLabel;
    QDoubleSpinBox *varianceSpinBox;

//bit plane levels
    QString level;

//reduce grey levels
     QGroupBox *reduceGreyGroupBox;
     QGridLayout *reduceGreyGridLayout;
     QLabel *reduceGreyLabel;
     QLabel *reduceGreyDisplayLabel;
     QScrollBar *reduceGreyScrollBar;

//push buttons
    QWidget *buttonsWidget;
    QSpacerItem *leftSpacer;
    QSpacerItem *rightSpacer;
    QGridLayout *buttonsGridLayout;
    QSpacerItem *buttonsVerticalSpacer;
};

#endif // INFORMATIONPANEL_H
