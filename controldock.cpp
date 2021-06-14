#include "controldock.h"
#include <QDebug>

ControlDock::ControlDock()
{
    controlDockWidget = new QWidget();
    controlDockWidget->setObjectName(QStringLiteral("controlDockWidget"));
    verticalLayout = new QVBoxLayout(controlDockWidget);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    controlDockLabel = new QLabel(controlDockWidget);
    controlDockLabel->setObjectName(QStringLiteral("controlDockLabel"));
    controlDockLabel->setMinimumSize(QSize(320, 240));
    controlDockLabel->setStyleSheet("background-color: rgb(200, 200, 200);");

    histogramDockLabel = new QLabel(controlDockWidget);
    histogramDockLabel->setObjectName(QStringLiteral("histogramDockLabel"));
    histogramDockLabel->setMaximumSize(QSize(320, 80));
    histogramDockLabel->setFrameShape(QFrame::Box);

    mainPanelTextEdit = new QTextEdit(controlDockWidget);

    mainPanelTextEdit->setObjectName(QStringLiteral("mainPanelTextEdit"));
    QSizePolicy textEditSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainPanelTextEdit->setSizePolicy(textEditSizePolicy);
    mainPanelTextEdit->setStyleSheet("background-color: rgb(200, 200, 200);");

    verticalLayout->addWidget(controlDockLabel);

    imageDetails = new QLabel(controlDockWidget);
    imageDetails->setObjectName(QStringLiteral("imageDetailsLabel"));

    verticalLayout->addWidget(imageDetails);
    verticalLayout->addWidget(histogramDockLabel);
    verticalLayout->addWidget(mainPanelTextEdit);

//sobelGroupBox controls
    sobelGroupBox = new QGroupBox(controlDockWidget);
    sobelGroupBox->setObjectName(QStringLiteral("sobelGroupBox"));
    sobelGroupBox->setEnabled(true);
    sobelGroupBox->setAutoFillBackground(false);
    sobelGridLayout = new QGridLayout(sobelGroupBox);
    sobelGridLayout->setObjectName(QStringLiteral("sobelGridLayout"));
    deltaSpinBox = new QDoubleSpinBox(sobelGroupBox);
    deltaSpinBox->setObjectName(QStringLiteral("deltaSpinBox"));
    deltaSpinBox->setMaximum(12);

    sobelGridLayout->addWidget(deltaSpinBox, 2, 4, 1, 1);
    scaleSpinBox = new QDoubleSpinBox(sobelGroupBox);
    scaleSpinBox->setObjectName(QStringLiteral("scaleSpinBox"));
    scaleSpinBox->setMinimum(1);
    scaleSpinBox->setMaximum(11);

    sobelGridLayout->addWidget(scaleSpinBox, 2, 3, 1, 1);

    kSizeSpinBox = new QSpinBox(sobelGroupBox);
    kSizeSpinBox->setObjectName(QStringLiteral("kSizeSpinBox"));
    kSizeSpinBox->setMinimum(-1);
    kSizeSpinBox->setMaximum(29);
    kSizeSpinBox->setSingleStep(2);
    kSizeSpinBox->setValue(-1);

    sobelGridLayout->addWidget(kSizeSpinBox, 2, 2, 1, 1);

    deltaLabel = new QLabel(sobelGroupBox);
    deltaLabel->setObjectName(QStringLiteral("deltaLabel"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(deltaLabel->sizePolicy().hasHeightForWidth());
    deltaLabel->setSizePolicy(sizePolicy);
    deltaLabel->setAlignment(Qt::AlignCenter);

    sobelGridLayout->addWidget(deltaLabel, 1, 4, 1, 1);

    ScaleLabel = new QLabel(sobelGroupBox);
    ScaleLabel->setObjectName(QStringLiteral("ScaleLabel"));
    sizePolicy.setHeightForWidth(ScaleLabel->sizePolicy().hasHeightForWidth());
    ScaleLabel->setSizePolicy(sizePolicy);
    ScaleLabel->setAlignment(Qt::AlignCenter);

    sobelGridLayout->addWidget(ScaleLabel, 1, 3, 1, 1);

    KSizeLabel = new QLabel(sobelGroupBox);
    KSizeLabel->setObjectName(QStringLiteral("KSizeLabel"));
    sizePolicy.setHeightForWidth(KSizeLabel->sizePolicy().hasHeightForWidth());
    KSizeLabel->setSizePolicy(sizePolicy);
    KSizeLabel->setAlignment(Qt::AlignCenter);

    sobelGridLayout->addWidget(KSizeLabel, 1, 2, 1, 1);

    verticalLayout->addWidget(sobelGroupBox);

    sobelGroupBox->setTitle(tr("Operatore di sobel"));
    KSizeLabel->setText(tr("ksize"));
    ScaleLabel->setText(tr("scale"));
    deltaLabel->setText(tr("delta"));

//cannyGroupBox controls
    cannyGroupBox = new QGroupBox(controlDockWidget);
    cannyGroupBox->setObjectName(QStringLiteral("cannyGroupBox"));
    cannyGroupBox->setEnabled(true);
    cannyGroupBox->setAutoFillBackground(false);
    cannyGridLayout = new QGridLayout(cannyGroupBox);
    cannyGridLayout->setObjectName(QStringLiteral("cannyGridLayout"));

    kernelSizeLabel = new QLabel(cannyGroupBox);
    kernelSizeLabel->setObjectName(QStringLiteral("kernelSizeLabel"));
    kernelSizeLabel->setAlignment(Qt::AlignCenter);

    cannyGridLayout->addWidget(kernelSizeLabel, 1, 1, 1, 1);

    kernelSizeSpinBox = new QSpinBox(cannyGroupBox);
    kernelSizeSpinBox->setObjectName(QStringLiteral("kernelSizeSpinBox"));
    kernelSizeSpinBox->setMinimum(3);
    kernelSizeSpinBox->setMaximum(7);
    kernelSizeSpinBox->setSingleStep(2);
    kernelSizeSpinBox->setValue(3);

    cannyGridLayout->addWidget(kernelSizeSpinBox, 1, 2, 1, 1);

    ratioLabel = new QLabel(cannyGroupBox);
    ratioLabel->setObjectName(QStringLiteral("ratioLabel"));
    ratioLabel->setAlignment(Qt::AlignCenter);

    cannyGridLayout->addWidget(ratioLabel, 1, 3, 1, 1);

    ratioSpinBox = new QSpinBox(cannyGroupBox);
    ratioSpinBox->setObjectName(QStringLiteral("ratioSpinBox"));
    ratioSpinBox->setMinimum(1);
    ratioSpinBox->setMaximum(100);
    ratioSpinBox->setSingleStep(10);
    ratioSpinBox->setValue(50);

    cannyGridLayout->addWidget(ratioSpinBox, 1, 4, 1, 1);

    verticalLayout->addWidget(cannyGroupBox);

    cannyGroupBox->setTitle(tr("Funzione di Canny"));
    kernelSizeLabel->setText(tr("ksize"));
    ratioLabel->setText(tr("soglia"));

//laplacian GroupBox controls
    laplacianGroupBox = new QGroupBox(controlDockWidget);
    laplacianGroupBox->setObjectName(QStringLiteral("laplacianGroupBox"));
    laplacianGroupBox->setEnabled(true);
    laplacianGroupBox->setAutoFillBackground(false);
    laplacianGridLayout = new QGridLayout(laplacianGroupBox);
    laplacianGridLayout->setObjectName(QStringLiteral("laplacianGridLayout"));
    smoothingLabel = new QLabel(laplacianGroupBox);
    smoothingLabel->setObjectName(QStringLiteral("smoothingLabel"));
    QSizePolicy laplacianSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    laplacianSizePolicy.setHorizontalStretch(1);
    laplacianSizePolicy.setVerticalStretch(0);
    laplacianSizePolicy.setHeightForWidth(smoothingLabel->sizePolicy().hasHeightForWidth());
    smoothingLabel->setSizePolicy(laplacianSizePolicy);
    smoothingLabel->setAlignment(Qt::AlignCenter);

    laplacianGridLayout->addWidget(smoothingLabel, 1, 3, 1, 1);

    sigmaLabel = new QLabel(laplacianGroupBox);
    sigmaLabel->setObjectName(QStringLiteral("sigmaLabel"));
    QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(sigmaLabel->sizePolicy().hasHeightForWidth());
    sigmaLabel->setSizePolicy(sizePolicy2);
    sigmaLabel->setAlignment(Qt::AlignCenter);

    laplacianGridLayout->addWidget(sigmaLabel, 1, 2, 1, 1);

    sigmaSpinBox = new QSpinBox(laplacianGroupBox);
    sigmaSpinBox->setObjectName(QStringLiteral("sigmaSpinBox"));
    QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(sigmaSpinBox->sizePolicy().hasHeightForWidth());
    sigmaSpinBox->setSizePolicy(sizePolicy3);
    sigmaSpinBox->setMinimum(0);
    sigmaSpinBox->setMaximum(15);
    sigmaSpinBox->setSingleStep(1);
    sigmaSpinBox->setValue(0);
    laplacianGridLayout->addWidget(sigmaSpinBox, 2, 2, 1, 1);
    smoothingComboBox = new QComboBox(laplacianGroupBox);
    smoothingComboBox->setObjectName(QStringLiteral("smoothingComboBox"));
    laplacianGridLayout->addWidget(smoothingComboBox, 2, 3, 1, 1);

    verticalLayout->addWidget(laplacianGroupBox);

    laplacianGroupBox->setTitle(tr("Operatore di Laplace"));
    smoothingLabel->setText(tr("passa basso"));
    sigmaLabel->setText(tr("sigma"));

    smoothingComboBox->clear();
    smoothingComboBox->insertItems(0, QStringList()
                        << tr("Gaussiano")
                        << tr("Semplice")
                        << tr("Mediano")
                        );
//other filters
    otherFiltersGroupBox = new QGroupBox(controlDockWidget);
    otherFiltersGroupBox->setObjectName(QStringLiteral("otherFiltersGroupBox"));
    otherFiltersGroupBox->setEnabled(true);
    otherFiltersGroupBox->setAutoFillBackground(false);

    otherFiltersGridLayout = new QGridLayout(otherFiltersGroupBox);
    otherFiltersGridLayout->setObjectName(QStringLiteral("gridLayout"));

    scrollLabel = new QLabel(otherFiltersGroupBox);
    scrollLabel->setObjectName(QStringLiteral("scrollLabel"));
    scrollLabel->setAlignment(Qt::AlignCenter);

    scrollDisplayLabel = new QLabel(otherFiltersGroupBox);
    scrollDisplayLabel->setObjectName(QStringLiteral("scrollDisplayLabel"));
    scrollDisplayLabel->setAutoFillBackground(false);
    scrollDisplayLabel->setScaledContents(true);
    scrollDisplayLabel->setWordWrap(false);

    otherFiltersScrollBar = new QScrollBar(otherFiltersGroupBox);
    otherFiltersScrollBar->setObjectName(QStringLiteral("otherFiltersScrollBar"));
    QSizePolicy otherFiltersSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    otherFiltersSizePolicy.setHorizontalStretch(0);
    otherFiltersSizePolicy.setVerticalStretch(0);
    otherFiltersSizePolicy.setHeightForWidth(otherFiltersScrollBar->sizePolicy().hasHeightForWidth());
    otherFiltersScrollBar->setSizePolicy(otherFiltersSizePolicy);
    otherFiltersScrollBar->setMinimum(2);
    otherFiltersScrollBar->setMaximum(31);
    otherFiltersScrollBar->setSingleStep(1);
    otherFiltersScrollBar->setValue(2);
    otherFiltersScrollBar->setSliderPosition(0);
    otherFiltersScrollBar->setOrientation(Qt::Horizontal);

    otherFiltersGridLayout->addWidget(otherFiltersScrollBar, 0, 1, 1, 10);

    scrollLabel = new QLabel(otherFiltersGroupBox);
    scrollLabel->setObjectName(QStringLiteral("scrollLabel"));
    scrollLabel->setAlignment(Qt::AlignCenter);
    otherFiltersGridLayout->addWidget(scrollLabel, 1, 0, 1, 11);

    otherFiltersGridLayout->addWidget(scrollDisplayLabel, 0, 0, 1, 1);
    verticalLayout->addWidget(otherFiltersGroupBox);

    scrollLabel->setText(tr("ksize"));
    scrollDisplayLabel->setText(tr("2"));

//powerGroupBox control
    exponent=1;
    coefficient=1;
    powerGroupBox = new QGroupBox(controlDockWidget);
    powerGroupBox->setObjectName(QStringLiteral("powerGroupBox"));
    powerGroupBox->setEnabled(true);
    powerGroupBox->setAutoFillBackground(false);
    powerGridLayout = new QGridLayout(powerGroupBox);
    powerGridLayout->setObjectName(QStringLiteral("powerGridLayout"));
    exponentLabel = new QLabel(powerGroupBox);
    exponentLabel->setObjectName(QStringLiteral("exponentLabel"));
    QSizePolicy powerSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    powerSizePolicy.setHorizontalStretch(1);
    powerSizePolicy.setVerticalStretch(0);
    powerSizePolicy.setHeightForWidth(exponentLabel->sizePolicy().hasHeightForWidth());
    exponentLabel->setSizePolicy(powerSizePolicy);
    exponentLabel->setAlignment(Qt::AlignCenter);

    powerGridLayout->addWidget(exponentLabel, 0, 0, 1, 1);

    exponentSpinBox = new QDoubleSpinBox(powerGroupBox);
    exponentSpinBox->setObjectName(QStringLiteral("exponentSpinBox"));
    QSizePolicy valuesSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    valuesSizePolicy.setHorizontalStretch(0);
    valuesSizePolicy.setVerticalStretch(0);
    valuesSizePolicy.setHeightForWidth(exponentSpinBox->sizePolicy().hasHeightForWidth());
    exponentSpinBox->setSizePolicy(valuesSizePolicy);
    exponentSpinBox->setMinimum(0);
    exponentSpinBox->setMaximum(2);
    exponentSpinBox->setSingleStep(0.1);
    exponentSpinBox->setValue(1);

    powerGridLayout->addWidget(exponentSpinBox, 0, 1, 1, 1);

    coefficientLabel = new QLabel(powerGroupBox);
    coefficientLabel->setObjectName(QStringLiteral("coefficientLabel"));
    powerSizePolicy.setHeightForWidth(coefficientLabel->sizePolicy().hasHeightForWidth());
    coefficientLabel->setSizePolicy(powerSizePolicy);
    coefficientLabel->setAlignment(Qt::AlignCenter);

    powerGridLayout->addWidget(coefficientLabel, 0, 2, 1, 1);

    coefficientSpinBox = new QDoubleSpinBox(powerGroupBox);
    coefficientSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
    valuesSizePolicy.setHeightForWidth(coefficientSpinBox->sizePolicy().hasHeightForWidth());
    coefficientSpinBox->setSizePolicy(valuesSizePolicy);
    coefficientSpinBox->setMinimum(0);
    coefficientSpinBox->setMaximum(10);
    coefficientSpinBox->setSingleStep(0.1);
    coefficientSpinBox->setValue(1);

    powerGridLayout->addWidget(coefficientSpinBox, 0, 3, 1, 1);

    verticalLayout->addWidget(powerGroupBox);

    exponentLabel->setText(tr("Esponente"));
    coefficientLabel->setText(tr("Coefficiente"));
    powerGroupBox->setTitle(tr("Potenza"));

    //slicing level controls
    slicingGroupBox = new QGroupBox(controlDockWidget);
    slicingGroupBox->setObjectName(QStringLiteral("slicingGroupBox"));
    slicingGroupBox->setEnabled(true);
    slicingGroupBox->setAutoFillBackground(false);
    slicingGridLayout = new QGridLayout(slicingGroupBox);
    slicingGridLayout->setObjectName(QStringLiteral("slicingGridLayout"));
    lowLevelLabel = new QLabel(slicingGroupBox);
    lowLevelLabel->setObjectName(QStringLiteral("lowLevelLabel"));
    lowLevelLabel->setAlignment(Qt::AlignCenter);

    slicingGridLayout->addWidget(lowLevelLabel, 0, 1, 1, 1);

    highLevelLabel = new QLabel(slicingGroupBox);
    highLevelLabel->setObjectName(QStringLiteral("highLevelLabel"));
    highLevelLabel->setAlignment(Qt::AlignCenter);

    slicingGridLayout->addWidget(highLevelLabel, 0, 2, 1, 1);

    lowSpinBox = new QSpinBox(slicingGroupBox);
    lowSpinBox->setObjectName(QStringLiteral("lowSpinBox"));
    lowSpinBox->setMaximum(255);
    lowSpinBox->setSingleStep(1);
    lowSpinBox->setValue(100);

    slicingGridLayout->addWidget(lowSpinBox, 1, 1, 1, 1);

    highSpinBox = new QSpinBox(slicingGroupBox);
    highSpinBox->setObjectName(QStringLiteral("highSpinBox"));
    highSpinBox->setMaximum(255);
    highSpinBox->setSingleStep(1);
    highSpinBox->setValue(150);

    slicingGridLayout->addWidget(highSpinBox, 1, 2, 1, 1);

    binaryCheckBox = new QCheckBox(slicingGroupBox);
    binaryCheckBox->setObjectName(QStringLiteral("binaryCheckBox"));

    slicingGridLayout->addWidget(binaryCheckBox, 1, 0, 1, 1);

    verticalLayout->addWidget(slicingGroupBox);

    slicingGroupBox->setTitle(tr("Slicing"));
    binaryCheckBox->setText(tr("Slicing binario"));
    lowLevelLabel->setText(tr("Soglia inferiore"));;
    highLevelLabel->setText(tr("Soglia superiore"));

    //dilate erode groupbox
    morphoGroupBox = new QGroupBox(controlDockWidget);
    morphoGroupBox->setObjectName(QStringLiteral("morphoGroupBox"));
    morphoGroupBox->setEnabled(true);
    morphoGroupBox->setAutoFillBackground(false);

    morphoGridLayout = new QGridLayout(morphoGroupBox);
    morphoGridLayout->setObjectName(QStringLiteral("morphoGridLayout"));

    morphoLabel = new QLabel(morphoGroupBox);
    morphoLabel->setObjectName(QStringLiteral("morphoGreyLabel"));
    morphoLabel->setAlignment(Qt::AlignCenter);

    morphoGridLayout->addWidget(morphoLabel, 0, 0, 1, 9);
    morphoScrollBar = new QScrollBar(morphoGroupBox);
    morphoScrollBar->setObjectName(QStringLiteral("morphoScrollBar"));
    morphoScrollBar->setMinimum(1);
    morphoScrollBar->setMaximum(10);
    morphoScrollBar->setSingleStep(1);
    morphoScrollBar->setValue(1);
    morphoScrollBar->setSliderPosition(1);
    morphoScrollBar->setOrientation(Qt::Horizontal);

    morphoGridLayout->addWidget(morphoScrollBar, 1, 1, 1, 8);

    morphoDisplayLabel = new QLabel(morphoGroupBox);
    morphoDisplayLabel->setObjectName(QStringLiteral("morphoDisplayLabel"));
    morphoDisplayLabel->setAlignment(Qt::AlignCenter);
    morphoGridLayout->addWidget(morphoDisplayLabel, 1, 0, 1, 1);

    verticalLayout->addWidget(morphoGroupBox);

    morphoLabel->setText(tr("Iterations"));
    morphoDisplayLabel->setText(tr("1"));

    //open close GroupBox control
    openCloseGroupBox = new QGroupBox(controlDockWidget);
    openCloseGroupBox->setObjectName(QStringLiteral("openCloseGroupBox"));
    openCloseGroupBox->setEnabled(true);
    openCloseGroupBox->setAutoFillBackground(false);
    openCloseGridLayout = new QGridLayout(openCloseGroupBox);
    openCloseGridLayout->setObjectName(QStringLiteral("openCloseGridLayout"));
    openLabel = new QLabel(openCloseGroupBox);
    openLabel->setObjectName(QStringLiteral("exponentLabel"));
    openLabel->setAlignment(Qt::AlignCenter);

    openCloseGridLayout->addWidget(openLabel, 1, 1, 1, 1);

    openSpinBox = new QSpinBox(openCloseGroupBox);
    openSpinBox->setObjectName(QStringLiteral("openSpinBox"));
    openSpinBox->setMinimum(1);
    openSpinBox->setMaximum(10);
    openSpinBox->setSingleStep(1);
    openSpinBox->setValue(1);

    openCloseGridLayout->addWidget(openSpinBox, 1, 2, 1, 1);

    openingClosingCombo = new QComboBox(openCloseGroupBox);
    openingClosingCombo->setObjectName(QStringLiteral("openingClosingCombo"));

    openCloseGridLayout->addWidget(openingClosingCombo, 1, 0, 1, 1);

    closeLabel = new QLabel(openCloseGroupBox);
    closeLabel->setObjectName(QStringLiteral("closeLabel"));
    closeLabel->setAlignment(Qt::AlignCenter);
    closeLabel->setVisible(false);


    openCloseGridLayout->addWidget(closeLabel, 1, 1, 1, 1);

    closeSpinBox = new QSpinBox(openCloseGroupBox);
    closeSpinBox->setObjectName(QStringLiteral("closeSpinBox"));
    closeSpinBox->setMinimum(1);
    closeSpinBox->setMaximum(10);
    closeSpinBox->setSingleStep(1);
    closeSpinBox->setValue(1);
    closeSpinBox->setVisible(false);

    openCloseGridLayout->addWidget(closeSpinBox, 1, 2, 1, 1);

    verticalLayout->addWidget(openCloseGroupBox);

    openLabel->setText(tr("Imposta open"));
    closeLabel->setText(tr("Imposta close"));
    openingClosingCombo->clear();
    openingClosingCombo->insertItems(0, QStringList()
            << tr("Opening")
            << tr("Closing")
                   );
    openCloseGroupBox->setTitle(tr("Apertura/chiusura"));

    //push buttons
    buttonsWidget = new QWidget(controlDockWidget);
    buttonsWidget->setObjectName(QStringLiteral("buttonsWidget"));
    buttonsGridLayout = new QGridLayout(buttonsWidget);
    buttonsGridLayout->setObjectName(QStringLiteral("buttonsGridLayout"));
    rejectButton = new QPushButton(buttonsWidget);
    rejectButton->setObjectName(QStringLiteral("rejectButton"));

    QSizePolicy buttonSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    buttonSizePolicy.setHeightForWidth(rejectButton->sizePolicy().hasHeightForWidth());
    rejectButton->setSizePolicy(buttonSizePolicy);

    QIcon rejectIcon;
    rejectIcon.addFile(QStringLiteral(":/images/reject.png"), QSize(), QIcon::Normal, QIcon::Off);
    rejectButton->setIcon(rejectIcon);

    buttonsGridLayout->addWidget(rejectButton, 0, 0, 1, 1);

    acceptButton = new QPushButton(buttonsWidget);
    acceptButton->setObjectName(QStringLiteral("acceptButton"));
    QIcon acceptIcon;
    acceptIcon.addFile(QStringLiteral(":/images/accept.png"), QSize(), QIcon::Normal, QIcon::Off);
    acceptButton->setIcon(acceptIcon);

    buttonsGridLayout->addWidget(acceptButton, 0, 1, 1, 1);
    verticalLayout->addWidget(buttonsWidget);

    rejectButton->setText(tr("Rifiuta"));
    acceptButton->setText(tr("Appica"));

    //cancel button
    cancelWidget = new QWidget(controlDockWidget);
    cancelWidget->setObjectName(QStringLiteral("cancelWidget"));
    cancelGridLayout = new QGridLayout(cancelWidget);
    cancelGridLayout->setObjectName(QStringLiteral("cancelGridLayout"));
    cancelButton = new QPushButton(cancelWidget);
    cancelButton->setObjectName(QStringLiteral("cancelButton"));
    QSizePolicy cancelSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    cancelSizePolicy.setHeightForWidth(cancelButton->sizePolicy().hasHeightForWidth());
    cancelButton->setSizePolicy(cancelSizePolicy);
    cancelButton->setMaximumSize(QSize(100, 16777215));
    QIcon cancelIcon;
    cancelIcon.addFile(QStringLiteral(":/images/reject.png"), QSize(), QIcon::Normal, QIcon::Off);
    cancelButton->setIcon(cancelIcon);

    cancelGridLayout->addWidget(cancelButton, 0, 0, 1, 1);
    verticalLayout ->addWidget(cancelWidget);

    cancelButton->setText(tr("Annulla"));

    //set the dockwidget
    setWidget(controlDockWidget);
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setAllControlsEnabled(false);

    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(kSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_kSizeSpinBox_valueChanged(int)));
    connect(scaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_scaleSpinBox_valueChanged(double)));
    connect(deltaSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_deltaSpinBox_valueChanged(double)));
    connect(kernelSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_kernelSizeSpinBox_valueChanged(int)));
    connect(ratioSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_ratioSpinBox_valueChanged(int)));
    connect(sigmaSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_sigmaSpinBox_valueChanged(int)));
    connect(smoothingComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_smoothingComboBox_currentIndexChanged(int)));
    connect(otherFiltersScrollBar, SIGNAL(valueChanged(int)), this, SLOT(on_otherFiltersScrollBar_valueChanged(int)));
    connect(morphoScrollBar, SIGNAL(valueChanged(int)), this, SLOT(on_morphoScrollBar_valueChanged(int)));
    connect(openSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_openSpinBox_valueChanged(int)));
    connect(closeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_closeSpinBox_valueChanged(int)));
    connect(openingClosingCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(on_openingClosigCombo_currentIndexChanged(int)));
    connect(lowSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_lowSpinBox_valueChanged(int)));
    connect(highSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_highSpinBox_valueChanged(int)));
    connect(binaryCheckBox, SIGNAL(clicked()), this, SLOT(on_binaryCheckBox_clicked()));
    connect(exponentSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_exponentSpinBox_valueChanged(double)));
    connect(coefficientSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_coefficientSpinBox_valueChanged(double)));
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(on_acceptButton_clicked()));
    connect(rejectButton, SIGNAL(clicked()), this, SLOT(on_rejectButton_clicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
}

void ControlDock::setOutputPix(QPixmap& img_pix)
{
    outputImage=img_pix;
    controlDockLabel->setPixmap(outputImage.scaled(QSize(320,240), Qt::KeepAspectRatio));
}

void ControlDock::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::ContentsRectChange:
        if(!controlDockLabel->pixmap(Qt::ReturnByValue))
            controlDockLabel->hasScaledContents();
        else
        {
            QPixmap img_pix=controlDockLabel->pixmap(Qt::ReturnByValue);
            controlDockLabel->setPixmap(img_pix.scaled(controlDockLabel->size(), Qt::KeepAspectRatio));
        }
    break;
    default:
    break;
    }
}

void ControlDock::setContenentsImage()
{
    QPixmap img_pix=controlDockLabel->pixmap(Qt::ReturnByValue);
    controlDockLabel->setPixmap(img_pix.scaled(QSize(320,240), Qt::KeepAspectRatio));
}

//sobel controls
void ControlDock::on_kSizeSpinBox_valueChanged(int value)
{
    if(value%2==0)
        --value;
//DA VERIFICARE
    kSizeSpinBox->setValue(value);
    emit sobelChanged();
}

void ControlDock::on_scaleSpinBox_valueChanged(double _value)
{
    Q_UNUSED(_value);
    emit sobelChanged();
}

void ControlDock::on_deltaSpinBox_valueChanged(double _value)
{
    Q_UNUSED(_value);
    emit sobelChanged();
}

int ControlDock::getKSize()
{
    return kSizeSpinBox->value();
}

double ControlDock::getScale()
{
    return scaleSpinBox->value();
}

double ControlDock::getDelta()
{
    return deltaSpinBox->value();
}

void ControlDock::setSobelGroupBoxVisible(bool _visible)
{
    sobelGroupBox->setVisible(_visible);
}

//canny controls
void ControlDock::on_kernelSizeSpinBox_valueChanged(int value)
{
    if(value%2==0)
        --value;
    kernelSizeSpinBox->setValue(value);
    emit cannyChanged();
}

void ControlDock::on_ratioSpinBox_valueChanged(int _value)
{
    Q_UNUSED(_value);
    emit cannyChanged();
}

int ControlDock::getKernelSize()
{
    return kernelSizeSpinBox->value();
}

int ControlDock::getRatio()
{
    return ratioSpinBox->value();
}

void ControlDock::setCannyGroupBoxVisible(bool _visible)
{
    cannyGroupBox->setVisible(_visible);
}

//laplacian member functions
int ControlDock::getSmoothing()
{
    return currentIndex;
}

void ControlDock::setLaplacianGroupBoxVisible(bool _visible)
{
    laplacianGroupBox->setVisible(_visible);
}

void ControlDock::on_sigmaSpinBox_valueChanged(int value)
{
    Q_UNUSED(value);
    emit laplacianChanged();
}

int ControlDock::getSigma()
{
    return sigmaSpinBox->value();
}

void ControlDock::on_smoothingComboBox_currentIndexChanged(int value)
{
    currentIndex=value;
    emit laplacianChanged();
}

//level slicing

void ControlDock::on_lowSpinBox_valueChanged(int value)
{
    Q_UNUSED(value);
    emit sliceChanged();
}

void ControlDock::on_highSpinBox_valueChanged(int value)
{
    Q_UNUSED(value);
    emit sliceChanged();
}

void ControlDock::on_binaryCheckBox_clicked()
{
    emit sliceChanged();
}

bool ControlDock::getBinaryCheck()
{
    if(binaryCheckBox->isChecked())
        return true;
    else
        return false;
}

int ControlDock::getHighLevel()
{
    return highSpinBox->value();
}

int ControlDock::getLowLevel()
{
    return lowSpinBox->value();
}

void ControlDock::setSliceGroupBoxVisible(bool _visible)
{
    slicingGroupBox->setVisible(_visible);
}

//other filter
int ControlDock::getOtherFiltersParameter()
{
    return scrollDisplayLabel->text().toDouble();
}

void ControlDock::on_otherFiltersScrollBar_valueChanged(int value)
{
    QString otherFiltersString (QString::number(value));
    scrollDisplayLabel->setText(otherFiltersString);
    emit otherFiltersChanged();
}

void ControlDock::setOtherFiltersGroupBoxVisible(bool _visible)
{
    otherFiltersGroupBox->setVisible(_visible);
}

void ControlDock::setGroupBoxTitle(QString title)
{
    otherFiltersGroupBox->setTitle(title);
}

//erode dilate

int ControlDock::getIterationsParameter()
{
    return morphoDisplayLabel->text().toInt();
}

void ControlDock::on_morphoScrollBar_valueChanged(int value)
{
    QString morphoString (QString::number(value));
    morphoDisplayLabel->setText(morphoString);
    emit morphoChanged();
}

void ControlDock::setMorphoGroupBoxVisible(bool _visible)
{
    morphoGroupBox->setVisible(_visible);
}

void ControlDock::setMorphoGroupBoxTitle(QString title)
{
    morphoGroupBox->setTitle(title);
}

//opening closing

void ControlDock::setOpenCLoseBoxVisible(bool _visible)
{
    openCloseGroupBox->setVisible(_visible);
}

int ControlDock::getOpenParameter()
{
    return openSpinBox->value();
}

int ControlDock::getCloseParameter()
{
    return closeSpinBox->value();
}

void ControlDock::on_openSpinBox_valueChanged(int value)
{
    Q_UNUSED(value);
    emit openCloseChanged();
}

void ControlDock::on_closeSpinBox_valueChanged(int value)
{
    Q_UNUSED(value);
    emit openCloseChanged();
}

void ControlDock::on_openingClosigCombo_currentIndexChanged(int value)
{
    qDebug()<< "current index: " << value;
    if(value==0)
    {
        openSpinBox->setVisible(true);
        openLabel->setVisible(true);
        closeSpinBox->setVisible(false);
        closeLabel->setVisible(false);
    }
    else if(value==1)
    {
        openSpinBox->setVisible(false);
        openLabel->setVisible(false);
        closeSpinBox->setVisible(true);
        closeLabel->setVisible(true);
    }
    emit openCloseChanged();
}

int ControlDock::getOpeningClosingComboOption()
{
    return openingClosingCombo->currentIndex();
}


//power transform
void ControlDock::setPowerGroupBoxVisible(bool _visible)
{
    powerGroupBox->setVisible(_visible);
}

double ControlDock::getPowerCoefficient()
{
    return coefficient;
}

double ControlDock::getPowerExponent()
{
    return exponent;
}

void ControlDock::on_exponentSpinBox_valueChanged(double arg1)
{
    exponent=arg1;
    emit powerTransformChanged();
}

void ControlDock::on_coefficientSpinBox_valueChanged(double arg2)
{
    coefficient=arg2;
    emit powerTransformChanged();
}

void ControlDock::setImageDetails(QString _imageDetails)
{
    imageDetails->setText(_imageDetails);
}

void ControlDock::on_acceptButton_clicked()
{
    emit acceped();
}

void ControlDock::on_rejectButton_clicked()
{
    emit rejected();
}

void ControlDock::on_cancelButton_clicked()
{
    emit cancel();
}

void ControlDock::activateControls(bool _status)
{
    buttonsWidget->setVisible(_status);
}

//cancel button
void ControlDock::setCancelButtonVisible(bool _visible)
{
    cancelButton->setVisible(_visible);
}
//histogram
void ControlDock::setHistogramVisible()
{
    histogramDockLabel->setVisible(true);
}

void ControlDock::setHistogram(QPixmap pix_hist)
{
    histogramDockLabel->setPixmap(pix_hist);
    histogramDockLabel->setScaledContents(true);
}

void ControlDock::setTextHistory(QString _textHistory)
{
    mainPanelTextEdit->append(_textHistory);
}

void ControlDock::seTextHistoryVisible(bool _textHistoryPan)
{
    mainPanelTextEdit->setVisible(_textHistoryPan);
}

bool ControlDock::textHistoryVisible()
{
    if(mainPanelTextEdit->isVisible())
        return true;
    else
        return false;
}

void ControlDock::setAllControlsEnabled(bool status)
{
   sobelGroupBox->setVisible(status);
   cannyGroupBox->setVisible(status);
   laplacianGroupBox->setVisible(status);
   otherFiltersGroupBox->setVisible(status);
   powerGroupBox->setVisible(status);
   buttonsWidget->setVisible(status);
   slicingGroupBox->setVisible(status);
   cancelButton->setVisible(status);
   histogramDockLabel->setVisible(status);
   mainPanelTextEdit->setVisible(status);
   morphoGroupBox->setVisible(status);
   openCloseGroupBox->setVisible(status);
}

ControlDock::~ControlDock()
{
}
