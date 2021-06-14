#include "mainwindow.h"
#include "imagepan.h"
#include "ui_imagepan.h"
#include "imagefiltering.h"
#include <ImageProcessing.h>
#include <QDockWidget>
#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>

MainWindow::MainWindow():imagePan(new ImagePan)
{
    setCentralWidget(imagePan);

    setWindowTitle(tr("Image processing"));

    QIcon iconWin;
    iconWin.addFile(QStringLiteral(":/images/ico.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(iconWin);

    createActions();
    createHelpWindow();
    createContextMenu();
    filter = NO_FILTERING;
    modified=false;
    myImageFiltering = new ImageFiltering();

//dockTextEdit->setStyleSheet("background-color: rgb(200, 200, 200); color: rgb(255, 0, 0); font: 12pt;");

    qApp->installEventFilter(this);

    readSettings();
    connect(imagePan, SIGNAL(modified()),this, SLOT(imageModified()));
    connect(mainPanelDock->acceptButton, SIGNAL(clicked()), this ,SLOT(previewAccept()));
    connect(mainPanelDock->rejectButton, SIGNAL(clicked()), this ,SLOT(previewReject()));
    connect(imagePan->scene, SIGNAL(selected()),this, SLOT(setRoiExtractEnabled()));
    connect(mainPanelDock, SIGNAL(cancel()),this, SLOT(canceltROI()));
    connect(informationPanel, SIGNAL(singleParameterChanged()),this ,SLOT(on_saltAndPepperNoise()));
    connect(informationPanel, SIGNAL(addingGaussianNoise()),this ,SLOT(on_gaussianNoise()));
    connect(informationPanel, SIGNAL(redeuceGreyChanged()),this ,SLOT(on_reduceGreyLevels()));
    connect(informationPanel, SIGNAL(planeChanged()),this ,SLOT(on_reduceLevel()));
    connect(informationPanel, SIGNAL(accept()),this ,SLOT(processingAccepted()));
    connect(informationPanel, SIGNAL(cancel()),this ,SLOT(processingRejected()));
    connect(this, SIGNAL(imageForMouseChanged()), this, SLOT(setImageView()));
    connect(helpAction, SIGNAL(triggered()), helpWindow, SLOT(show()));
}

MainWindow::~MainWindow()
{
    delete myImageFiltering;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
        case QEvent::WindowStateChange:/*
        if(!imagePan->ui->label->pixmap())
        {
            imagePan->ui->label->hasScaledContents();
        }
        else
        {
            img_pix=*imagePan->ui->label->pixmap();
            imagePan->ui->label->setPixmap(img_pix.scaled(imagePan->ui->label->size(), Qt::KeepAspectRatio));
        }
*/
    default:
        break;
    }
}

void MainWindow::imageModified()
{
    setWindowModified(true);
}


void MainWindow::open()
{
    if (okToContinue())
    {
        QFileDialog::Options options;
        QString selectedFilter;
        QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open Image Files"),
                                    "",
                                    tr("Image files (*.jpg *.jpeg *.png *.gif *.bmp *.tif)"),
                                    &selectedFilter,
                                    options);
        if (!fileName.isEmpty())
        {
            loadFile(fileName);
            setCurrentFile(curFile);
        }
    }
}

bool MainWindow::loadFile(const QString & fileName)
{
   if (!imagePan->setImage(fileName))
   {
        QString myFile=strippedName(fileName);
        QString myFileWarning=("Caricamento immagine annullato:  ha un formato non riconosciuto o il file è danneggiato.");
        myFileWarning=myFileWarning.insert(32, myFile);
        if(mainPanelDock->textHistoryVisible())
        {
            QString myDollar = "$: ";
            mainPanelDock->setTextHistory(myDollar.append(myFileWarning));
        }
        statusBar()->showMessage(tr("Loading canceled"), 2000);
        QMessageBox::critical(this, tr("Image processing"),
                                          myFileWarning);
        return false;
    }
    imagePan->normalize();
    QString imageLoaded="$:  caricata con successo;";
    QString myFile = strippedName(fileName);
    imageLoaded.insert(3, myFile);
    statusBar()->showMessage(tr("Immagine caricata"), 2000);
    mainPanelDock->setTextHistory(imageLoaded);
    img_pix=imagePan->MyDisplay->setOutputPixmap();
    mainPanelDock->setOutputPix(img_pix);
    Mat inputImage=imagePan->MyDisplay->getCvMat();
    imagePan->setPreviewOut(inputImage);
    imagePan->showHistogramDiagram(inputImage);
    QPixmap hist_pix=imagePan->getHistogramPixmap();
    mainPanelDock->setHistogram(hist_pix);

    mainPanelDock->setImageDetails(getImageDetails(true));
    mainPanelDock->setAllControlsEnabled(false);
    curFile="";
    sobelAct->setEnabled(true);
    cannyAct->setEnabled(true);
    saveAct->setEnabled(true);
    saveAsAct->setEnabled(true);
    histogramAct->setEnabled(true);
    equalizeAct->setEnabled(true);
    zoomInAct->setEnabled(true);
    zoomOutAct->setEnabled(true);
    normalizeAct->setEnabled(true);
    adaptAct->setEnabled(true);
    negativeAct->setEnabled(true);
    logAct->setEnabled(true);
    invLogAct->setEnabled(true);
    laplacianAct->setEnabled(true);
    blurAct->setEnabled(true);
    gaussianBlurAct->setEnabled(true);
    medianBlurAct->setEnabled(true);
    bilateralBlurAct->setEnabled(true);
    roiSelectAct->setEnabled(true);
    dragAct->setEnabled(true);
    powerTransformAct->setEnabled(true);
    sliceTransformAct->setEnabled(true);
    erodeAct->setEnabled(true);
    dilateAct->setEnabled(true);
    openCloseAct->setEnabled(true);
    imageViewAct->setEnabled(true);
    fourierTransformAct->setEnabled(true);

    mainPanelDock->setHistogramVisible();
    mainPanelDock->seTextHistoryVisible(true);

//informatioPanel
    informationPanel->setTreeWidgetVisible(true);
//informationPanel->setInformationTextEditVisible(true);

    setCurrentFile(fileName);
    return true;
}

void MainWindow::openRecentFile()
{
    if (okToContinue())
    {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            loadFile(action->data().toString());
    }
}

QString MainWindow::getImageDetails(bool newImage)
{
    Mat inputImage;
    QSize imageDimension=imagePan->MyDisplay->getImageDimension();

    if(newImage)
        inputImage=imagePan->MyDisplay->getCvMat();
    else
        inputImage=imagePan->getPreviewOut();

    int width =imageDimension.width();
    int height = imageDimension.height();
    double minRoi;
    double maxRoi;
    Point minLoc;
    Point maxLoc;
    minMaxLoc(inputImage, &minRoi, &maxRoi, &minLoc, &maxLoc);

    QString imageDetails="<font size=4><b>Dimensioni immagine: </b></>";
    imageDetails.append("(");
    imageDetails.append(QString::number(width));
    imageDetails.append(", ");
    imageDetails.append(QString::number(height));
    imageDetails.append(tr(")<br>"));
    imageDetails.append("<font size=3>Massimo valore livello di grigio: ");
    imageDetails.append(QString::number(maxRoi));
    imageDetails.append(" @ (");
    imageDetails.append(QString::number(minLoc.x));
    imageDetails.append(", ");
    imageDetails.append(QString::number(minLoc.y));
    imageDetails.append(")");
    imageDetails.append("<br>");
    imageDetails.append("<font size=3>Minimo valore livello di grigio: ");
    imageDetails.append(QString::number(minRoi));
    imageDetails.append(" @ (");
    imageDetails.append(QString::number(maxLoc.x));
    imageDetails.append(", ");
    imageDetails.append(QString::number(maxLoc.y));
    imageDetails.append(")");

    return imageDetails;
}

bool MainWindow::okToContinue()
{
    if (isWindowModified())
    {
        int r = QMessageBox::warning(this, tr("Image processing"),
                            tr("L'immagine è stata modificata.\n"
                               "Vuoi salvare le modifiche?"),
                            QMessageBox::Yes | QMessageBox::No
                            | QMessageBox::Cancel);
        if (r == QMessageBox::Yes)
        {
            return save();
        }
        else if (r == QMessageBox::Cancel)
                return false;
    }
    return true;
}

void MainWindow::setCurrentFile(const QString &imageName)
{
    curFile = imageName;

    setWindowModified(false);

    QString shownName = tr("Senza nome");
    if (!curFile.isEmpty())
    {
        shownName = strippedName(curFile);
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        updateRecentFileActions();

    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                                   .arg(tr("Image processing")));
}

bool MainWindow::eventFilter (QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QStringList pixelValues = imagePan->getPixelValues();
        QString coordinates;
        if(imagePan->visualizePixelStatus())
        {
           coordinates =pixelValues.join(" ");
           statusBar()->showMessage(coordinates);
        }
      }
    return QObject::eventFilter(obj, event);
}

bool MainWindow::save()
{
    if (curFile.isEmpty())
        return saveAs();
    else
    {
        setCurrentFile(curFile);
        mainPanelDock->setTextHistory(tr("$: Salvataggio;"));
        return imagePan->saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString imageName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "../untitled.png",
                               tr("Images (*.png *.xpm *.jpg)"));

    if(imagePan->saveFile(imageName))
    {
        setCurrentFile(imageName);
        statusBar()->showMessage(tr("Immagine salvata"), 2000);
        mainPanelDock->setTextHistory(tr("$: Immagine salvata;"));
        return true;
    }
    else
    return false;
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("Informazioni su Image Processing"),
            tr("<h2>Image Processing</h2>"
               "<p>Prototipo in fase di sviluppo</p>"
               "<p>Applicazione di image processing"
               " con l'impiego di librerie opencv "
               "in ambiente di sviluppo Qt.Creator 4.6.2<br>"
               "(Based on Qt 5.11.1 (MSVC 2015, 32 bit))<br>"
               "https://opencv.org<br>"
               "https://qt.io</p>"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue())
    {
        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
        statusBar()->showMessage(tr("Abbandono immagine annullato"), 2000);
        mainPanelDock->setTextHistory(tr("$: Abbandono immagine annullato;"));
    }
}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while (i.hasNext())
    {
        if (!QFile::exists(i.next()))
            i.remove();
    }

    for (int j = 0; j < MaxRecentFiles; ++j)
    {
        if (j < recentFiles.count())
        {
            QString text = tr("&%1 %2")
                           .arg(j + 1)
                           .arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        }
        else
            recentFileActions[j]->setVisible(false);
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}

void MainWindow::createActions()
{
    //Crea il menu file
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    const QIcon openIcon = QIcon::fromTheme("Apri immagine", QIcon(":/images/open.png"));
    openAct = new QAction(openIcon, tr("&Apri..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Apre un'immagine"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("Salva immagine", QIcon(":/images/save.png"));
    saveAct = new QAction(saveIcon, tr("&Salva..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Salva l'immagine"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);
    saveAct->setEnabled(false);

    const QIcon saveAsIcon = QIcon::fromTheme("Salva con nome", QIcon(":/images/saveAs.png"));
    saveAsAct = new QAction(saveAsIcon, tr("Salva con nome..."), this);
    saveAsAct->setStatusTip(tr("Salva l'immagine con nome"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
    fileMenu->addAction(saveAsAct);
    saveAsAct->setEnabled(false);

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    separatorAction = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);

    fileMenu->addSeparator();

    exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    //Crea il meu modifica
    QMenu *editMenu = menuBar()->addMenu(tr("&Modifica"));
    QToolBar *editMenuToolBar = addToolBar(tr("Modifica"));


    undoAction = imagePan->undoStack->createUndoAction(this, tr("Annulla"));
    undoAction->setIcon(QIcon(":/images/undo.png"));
    connect(undoAction, &QAction::triggered, this, &MainWindow::imageModified);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);

    editMenu->addAction(undoAction);
    editMenuToolBar->addAction(undoAction);

    redoAction = imagePan->undoStack->createRedoAction(this, tr("Ripristina"));
    redoAction->setIcon(QIcon(":/images/redo.png"));
    connect(redoAction, &QAction::triggered, this, &MainWindow::imageModified);
    connect(redoAction, &QAction::triggered, this, &MainWindow::redo);
    editMenu->addAction(redoAction);
    editMenuToolBar->addAction(redoAction);

    //Crea il visualizza
    QMenu *viewMenu = menuBar()->addMenu(tr("&Visualizza"));
    QToolBar *viewMenuToolBar = addToolBar(tr("Visualizza"));

    const QIcon zoomOutIcon = QIcon::fromTheme("Riduci (25%)", QIcon(":/images/zoom-.png"));
    zoomOutAct = new QAction(zoomOutIcon, tr("&Riduci (25%)"), this);
    zoomOutAct->setStatusTip(tr("Riduci del 25%"));
    connect(zoomOutAct, &QAction::triggered, this, &MainWindow::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);
    viewMenu->addAction(zoomOutAct);
    viewMenuToolBar->addAction(zoomOutAct);

    const QIcon zoomInIcon = QIcon::fromTheme("Ingrandisci (25%)", QIcon(":/images/zoom+.png"));
    zoomInAct = new QAction(zoomInIcon, tr("Ingrandisci (25%)"), this);
    zoomInAct->setStatusTip(tr("Ingrandisce del 25%"));
    connect(zoomInAct, &QAction::triggered, this, &MainWindow::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);
    viewMenu->addAction(zoomInAct);
    viewMenuToolBar->addAction(zoomInAct);

    const QIcon normalizeIcon = QIcon::fromTheme("Dimensioni originali", QIcon(":/images/normalize.png"));
    normalizeAct = new QAction(normalizeIcon, tr("Dimensioni originali"), this);
    normalizeAct->setStatusTip(tr("Riporta l'immagine alle dimensioni originali"));
    connect(normalizeAct, &QAction::triggered, this, &MainWindow::normalize);
    normalizeAct->setEnabled(false);
    viewMenu->addAction(normalizeAct);
    viewMenuToolBar->addAction(normalizeAct);

    const QIcon adaptIcon = QIcon::fromTheme("Adatta", QIcon(":/images/zoomFit.png"));
    adaptAct = new QAction(adaptIcon, tr("Adatta"), this);
    adaptAct->setStatusTip(tr("Adatta l'immagine"));
    connect(adaptAct, &QAction::triggered, this, &MainWindow::adapt);
    adaptAct->setEnabled(false);
    viewMenu->addAction(adaptAct);
    viewMenuToolBar->addAction(adaptAct);

    const QIcon dragIcon = QIcon::fromTheme("Scrolla", QIcon(":/images/drag.png"));
    dragAct = new QAction(dragIcon, tr("Scrolla..."), this);
    dragAct->setStatusTip(tr("Scrolla l'immagine"));
    connect(dragAct, &QAction::triggered, this, &MainWindow::dragMode);
    dragAct->setCheckable(true);
    dragAct->setEnabled(false);
    viewMenu->addAction(dragAct);
    viewMenuToolBar->addAction(dragAct);

    viewMenu->addSeparator();

    imageViewAct = new QAction(tr("&Visualizza l'anteprima"), this);
    imageViewAct->setStatusTip(tr("&Visualizza l'anteprima"));
    imageViewAct->setCheckable(true);
    connect(imageViewAct, &QAction::triggered, this, &MainWindow::setImageView);
    imageViewAct->setEnabled(false);
    viewMenu->addAction(imageViewAct);

    mainPanelDock = new ControlDock();
    mainPanelDock->setWindowTitle(tr("Pannello principale"));
    mainPanelDock->setMinimumSize(QSize(320, 240));
    addDockWidget(Qt::LeftDockWidgetArea, mainPanelDock);

    mainPanelDockAct = new QAction(tr("Visualizza il pannello principale"), this);
    mainPanelDockAct->setStatusTip(tr("Visualizza il pannello principale"));
    mainPanelDockAct = mainPanelDock->toggleViewAction();
    viewMenu->addAction(mainPanelDockAct);
    viewMenuToolBar->addAction(mainPanelDockAct);

    informationPanel = new InformationPanel();
    informationPanel->setWindowTitle(tr("Panello di indagine"));
    informationPanel->setMinimumSize(QSize(320, 240));
    addDockWidget(Qt::RightDockWidgetArea, informationPanel);

    dockInformationAct =new QAction(tr("Pannello di indagine"), this);
    dockInformationAct = informationPanel->toggleViewAction();
    viewMenu->addAction(dockInformationAct);
    viewMenuToolBar->addAction(dockInformationAct);

    viewMenu->addSeparator();
    //Crea il menu Istogramma
    QMenu *histogramMenu = menuBar()->addMenu(tr("&Istogramma"));
    QToolBar *histogramMenuToolBar = addToolBar(tr("Istogramma"));

    const QIcon histogramIcon = QIcon::fromTheme(tr("Mostra"), QIcon(":/images/histogram.png"));
    histogramAct = new QAction(histogramIcon, tr("&Mostra"), this);
    histogramAct->setStatusTip(tr("Mostra l'istogramma dell'immagine"));
    connect(histogramAct, &QAction::triggered, this, &MainWindow::showHistogramDiagram);
    histogramMenu->addAction(histogramAct);
    histogramMenuToolBar->addAction(histogramAct);
    histogramAct->setEnabled(false);

    const QIcon equalizeIcon = QIcon::fromTheme(tr("Equalizza"), QIcon(":/images/equalize.png"));
    equalizeAct = new QAction(equalizeIcon, tr("&Equalizza"), this);
    equalizeAct->setStatusTip(tr("Equalizza limmagine"));
    connect(equalizeAct, &QAction::triggered, this, &MainWindow::equalize);
    histogramMenu->addAction(equalizeAct);
    histogramMenuToolBar->addAction(equalizeAct);
    equalizeAct->setEnabled(false);

    //Crea il menu ROI
    QMenu *selectModeMenu = menuBar()->addMenu(tr("&Regione di interesse"));
    QToolBar *selectModeMenuToolBar = addToolBar(tr("Regione di interesse"));

    const QIcon roiSelectIcon = QIcon::fromTheme(tr("Seleziona"), QIcon(":/images/selection.png"));
    roiSelectAct = new QAction(roiSelectIcon, tr("&Seleziona"), this);
    roiSelectAct->setStatusTip(tr("Seleziona la regione di interesse"));
    roiSelectAct->setCheckable(true);
    connect(roiSelectAct, &QAction::triggered, this, &MainWindow::selectROI);
    selectModeMenu->addAction(roiSelectAct);
    selectModeMenuToolBar->addAction(roiSelectAct);
    roiSelectAct->setEnabled(false);

    const QIcon roiIcon = QIcon::fromTheme(tr("Estrai"), QIcon(":/images/roi.png"));
    roiExtractAct = new QAction(roiIcon, tr("&Estrai"), this);
    roiExtractAct->setStatusTip(tr("Estrae la regione di interesse"));
    connect(roiExtractAct, &QAction::triggered, this, &MainWindow::extractROI);
    selectModeMenu->addAction(roiExtractAct);
    selectModeMenuToolBar->addAction(roiExtractAct);
    roiExtractAct->setEnabled(false);

    //crea il menu delle trasformazioni puntuali

    QMenu *pointTransformMenu = menuBar()->addMenu(tr("&Trasformazioni puntuali"));
    QToolBar *pointTransformToolBar = addToolBar(tr("Trasformazioni puntuali"));

    const QIcon negativeIcon = QIcon::fromTheme(tr("Negativo"), QIcon(":/images/negative.png"));
    negativeAct = new QAction(negativeIcon, tr("&Negativo"), this);
    negativeAct->setStatusTip(tr("Negativo dell'immagine"));
    connect(negativeAct, &QAction::triggered, this, &MainWindow::negativeTransform);
    pointTransformMenu->addAction(negativeAct);
    pointTransformToolBar->addAction(negativeAct);
    negativeAct->setEnabled(false);

    const QIcon logIcon = QIcon::fromTheme(tr("Logaritmica"), QIcon(":/images/log.png"));
    logAct = new QAction(logIcon, tr("&Logaritmica"), this);
    logAct->setStatusTip(tr("Logaritmica"));
    connect(logAct, &QAction::triggered, this, &MainWindow::logTransform);
    pointTransformMenu->addAction(logAct);
    pointTransformToolBar->addAction(logAct);
    logAct->setEnabled(false);

    const QIcon invLogIcon = QIcon::fromTheme(tr("Logaritmica inversa"), QIcon(":/images/inv.png"));
    invLogAct = new QAction(invLogIcon, tr("&Logaritmica inversa"), this);
    invLogAct->setStatusTip(tr("Logaritmica inversa"));
    connect(invLogAct, &QAction::triggered, this, &MainWindow::invTransform);
    pointTransformMenu->addAction(invLogAct);
    pointTransformToolBar->addAction(invLogAct);
    invLogAct->setEnabled(false);

    const QIcon powerTransformIcon = QIcon::fromTheme(tr("Potenza"), QIcon(":/images/pow.png"));
    powerTransformAct = new QAction(powerTransformIcon, tr("Potenza"), this);
    powerTransformAct->setStatusTip(tr("&Potenza"));
    powerTransformAct->setCheckable(true);
    connect(powerTransformAct, &QAction::triggered, this, &MainWindow::powerTransform);
    pointTransformMenu->addAction(powerTransformAct);
    pointTransformToolBar->addAction(powerTransformAct);
    powerTransformAct->setEnabled(false);

    const QIcon sliceTransformIcon = QIcon::fromTheme(tr("Slice"), QIcon(":/images/slice.png"));
    sliceTransformAct = new QAction(sliceTransformIcon, tr("&Slice"), this);
    sliceTransformAct->setStatusTip(tr("Slice"));
    sliceTransformAct->setCheckable(true);
    connect(sliceTransformAct, &QAction::triggered, this, &MainWindow::sliceLevelTransform);
    pointTransformMenu->addAction(sliceTransformAct);
    pointTransformToolBar->addAction(sliceTransformAct);
    sliceTransformAct->setEnabled(false);

    fourierTransformAct = new QAction(tr("Trasformata di Fourier"), this);
    fourierTransformAct->setStatusTip(tr("&Trasformata di Fourier"));
    connect(fourierTransformAct, &QAction::triggered, this, &MainWindow::fourierTransform);
    pointTransformMenu->addAction(fourierTransformAct);
    pointTransformToolBar->addAction(fourierTransformAct);
    fourierTransformAct->setEnabled(false);

    //Crea il menu filtri
    QMenu *filterMenu = menuBar()->addMenu(tr("&Filtri"));
    QMenu *derivativeFilterMenu  = new QMenu(tr("Basati su derivata"));
    QMenu *lowPassFilterMenu  = new QMenu(tr("Passa basso"));
    filterMenu->addMenu(derivativeFilterMenu);
    filterMenu->addMenu(lowPassFilterMenu);
    QToolBar *filtersMenuToolBar = addToolBar(tr("Filtri"));

    const QIcon sobelIcon = QIcon::fromTheme(tr("Operatore di Sobel"), QIcon(":/images/sobel.png"));
    sobelAct = new QAction(sobelIcon, tr("&Operatore di Sobel"), this);
    sobelAct->setStatusTip(tr("Applica l'operatore di Sobel"));
    sobelAct->setCheckable(true);
    connect(sobelAct, &QAction::triggered, this, &MainWindow::sobelFilter);

    derivativeFilterMenu->addAction(sobelAct);
    filtersMenuToolBar->addAction(sobelAct);
    sobelAct->setEnabled(false);

    const QIcon laplacianIcon = QIcon::fromTheme(tr("Operatore Laplaciano"), QIcon(":/images/laplacian.png"));
    laplacianAct = new QAction(laplacianIcon, tr("&Operatore Laplaciano"), this);
    laplacianAct->setStatusTip(tr("Applica l'operatore Laplaciano"));
    laplacianAct->setCheckable(true);
    connect(laplacianAct, &QAction::triggered, this, &MainWindow::laplacianFilter);
    derivativeFilterMenu->addAction(laplacianAct);
    filtersMenuToolBar->addAction(laplacianAct);
    laplacianAct->setEnabled(false);

    const QIcon blurIcon = QIcon::fromTheme(tr("Blur semplice"), QIcon(":/images/blur.png"));
    blurAct = new QAction(blurIcon, tr("&Blur semplice"), this);
    blurAct->setStatusTip(tr("Apply il filtro Blur semplice"));
    blurAct->setCheckable(true);
    connect(blurAct, &QAction::triggered, this, &MainWindow::blurFilter);
    lowPassFilterMenu->addAction(blurAct);
    filtersMenuToolBar->addAction(blurAct);
    blurAct->setEnabled(false);

    const QIcon gaussianBlurIcon = QIcon::fromTheme(tr("Gaussiano"), QIcon(":/images/gaussian.png"));
    gaussianBlurAct = new QAction(gaussianBlurIcon, tr("&Gaussiano"), this);
    gaussianBlurAct->setStatusTip(tr("Applyìica un filtro Blur di tipo Gaussiano"));
    gaussianBlurAct->setCheckable(true);
    connect(gaussianBlurAct, &QAction::triggered, this, &MainWindow::gaussianFilter);
    lowPassFilterMenu->addAction(gaussianBlurAct);
    filtersMenuToolBar->addAction(gaussianBlurAct);
    gaussianBlurAct->setEnabled(false);

    const QIcon medianBlurIcon = QIcon::fromTheme(tr("Mediano"), QIcon(":/images/median.png"));
    medianBlurAct = new QAction(medianBlurIcon, tr("&Mediano"), this);
    medianBlurAct->setStatusTip(tr("Applica un filtro Blur di tipo Mediano"));
    medianBlurAct->setCheckable(true);
    connect(medianBlurAct, &QAction::triggered, this, &MainWindow::medianFilter);
    lowPassFilterMenu->addAction(medianBlurAct);
    filtersMenuToolBar->addAction(medianBlurAct);
    medianBlurAct->setEnabled(false);

    const QIcon bilateralBlurIcon = QIcon::fromTheme(tr("Bilaterale"), QIcon(":/images/bilateral.png"));
    bilateralBlurAct = new QAction(bilateralBlurIcon, tr("&Bilaterale"), this);
    bilateralBlurAct->setStatusTip(tr("Applica un filtro Blur di tipo Bilaterale"));
    bilateralBlurAct->setCheckable(true);
    connect(bilateralBlurAct, &QAction::triggered, this, &MainWindow::bilateralFilter);
    lowPassFilterMenu->addAction(bilateralBlurAct);
    filtersMenuToolBar->addAction(bilateralBlurAct);
    bilateralBlurAct->setEnabled(false);

    const QIcon cannyIcon = QIcon::fromTheme(tr("Funzione di Canny"), QIcon(":/images/canny.png"));
    cannyAct = new QAction(cannyIcon, tr("Funzione di Canny"), this);
    cannyAct->setStatusTip(tr("Applica la funzione di Canny"));
    cannyAct->setCheckable(true);
    connect(cannyAct, &QAction::triggered, this, &MainWindow::cannyFilter);
    filterMenu->addAction(cannyAct);
    filtersMenuToolBar->addAction(cannyAct);
    cannyAct->setEnabled(false);

    //Crea il menu morfologia
    QMenu *morphoMenu = menuBar()->addMenu(tr("&Morfologia"));
    QToolBar *morphoMenuToolBar = addToolBar(tr("Morfologia"));

    const QIcon erodeIcon = QIcon::fromTheme(tr("Erodi"), QIcon(":/images/erode.png"));
    erodeAct = new QAction(erodeIcon, tr("&erodi"), this);
    erodeAct->setStatusTip(tr("erode l'immagine"));
    erodeAct->setCheckable(true);
    connect(erodeAct, &QAction::triggered, this, &MainWindow::erodeOperation);
    morphoMenu->addAction(erodeAct);
    morphoMenuToolBar->addAction(erodeAct);
    erodeAct->setEnabled(false);

    const QIcon dilateIcon = QIcon::fromTheme(tr("Dilata"), QIcon(":/images/dilate.png"));
    dilateAct = new QAction(dilateIcon, tr("&dilata"), this);
    dilateAct->setStatusTip(tr("dilata l'immagine"));
    dilateAct->setCheckable(true);
    connect(dilateAct, &QAction::triggered, this, &MainWindow::dilateOperation);
    morphoMenu->addAction(dilateAct);
    morphoMenuToolBar->addAction(dilateAct);
    dilateAct->setEnabled(false);

    const QIcon openCloseIcon = QIcon::fromTheme(tr("Apertura/Chiusura"), QIcon(":/images/close-open.png"));
    openCloseAct = new QAction(openCloseIcon, tr("&Apertura/Chiusura"), this);
    openCloseAct->setStatusTip(tr("Apertura o chiusura dell'immagine"));
    openCloseAct->setCheckable(true);
    connect(openCloseAct, &QAction::triggered, this, &MainWindow::openCloseOperation);
    morphoMenu->addAction(openCloseAct);
    morphoMenuToolBar->addAction(openCloseAct);
    openCloseAct->setEnabled(false);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Aiuto"));

    QAction *aboutAct = helpMenu->addAction(tr("&Informazioni"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Fornisce informazioni sull'applicazione"));

    helpAction = new QAction(tr("Guida in linea"), this);
    helpAction->setShortcut(QKeySequence::HelpContents);
    helpMenu->addAction(helpAction);
}

void MainWindow::createContextMenu()
{
    imagePan->addAction(imageViewAct);
    imagePan->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Pronto"), 2000);
    mainPanelDock->setTextHistory(tr("$: Pronto;"));
}

void MainWindow::undo()
{
    statusBar()->showMessage(tr("Ripristino immagine"), 2000);
    imagePan->setPreviewOut(imagePan->MyDisplay->getCvMat());
    img_pix=imagePan->MyDisplay->setOutputPixmap();
    mainPanelDock->setOutputPix(img_pix);
    mainPanelDock->setTextHistory(tr("$: Ripristino immagine;"));
    mainPanelDock->setImageDetails(getImageDetails(true));
    setDockHistogram();
    emit imageForMouseChanged();
}

void MainWindow::redo()
{
    statusBar()->showMessage(tr("Rielaborazione "), 2000);
    imagePan->setPreviewOut(imagePan->MyDisplay->getCvMat());
    img_pix=imagePan->MyDisplay->setOutputPixmap();
    mainPanelDock->setOutputPix(img_pix);
    mainPanelDock->setTextHistory(tr("$: Rielaborazione;"));
    mainPanelDock->setImageDetails(getImageDetails(true));
    setDockHistogram();
    setOffImageView();
    emit imageForMouseChanged();
}

void MainWindow::showHistogramDiagram()
{
    statusBar()->showMessage(tr("Finestra istogramma "), 2000);
    Mat inputImage=imagePan->MyDisplay->getCvMat();
    Mat image = imagePan->getHistogramMat(inputImage);

    ShowHistogram dialog(this);
    QImage qt_hist = QImage((const unsigned char*)image.data,static_cast<int>(image.cols), static_cast<int>(image.rows), static_cast<int>(image.step), QImage::Format_Indexed8);
    QPixmap pix_hist=QPixmap::fromImage(qt_hist);

    dialog.ui->label->setPixmap(pix_hist);

    if(dialog.exec())
    {
        statusBar()->showMessage(tr("Finestra istogramma generata"), 2000);
        mainPanelDock->setTextHistory(tr("$: Finestra istogramma generata;"));
    }
}

void MainWindow::equalize()
{
    if(imagePan->equalizeImage())
    {
        statusBar()->showMessage(tr("Immagine equalizzata"), 2000);
        mainPanelDock->setTextHistory(tr("$: Immagine equalizzata;"));
        img_pix=imagePan->MyDisplay->setOutputPixmap();
        mainPanelDock->setOutputPix(img_pix);
        setDockHistogram();
        mainPanelDock->setImageDetails(getImageDetails(true));
        imagePan->setPreviewOut(imagePan->MyDisplay->getCvMat());
        emit imageForMouseChanged();
    }
    else
        mainPanelDock->setTextHistory(tr("$: Non equalizzo due volte;"));
}

void MainWindow::zoomIn()
{
    imagePan->zoomIn();
    statusBar()->showMessage(tr("ZoomIn"), 2000);
    mainPanelDock->setTextHistory(tr("$: zoomIn;"));
}

void MainWindow::zoomOut()
{
    imagePan->zoomOut();
    statusBar()->showMessage(tr("zoomOut"), 2000);
    mainPanelDock->setTextHistory(tr("$: zoomOut;"));
}

void MainWindow::adapt()
{
    imagePan->adaptImage();
    statusBar()->showMessage(tr("Adatto alla finestra"), 2000);
    mainPanelDock->setTextHistory(tr("$: Immagine adattata alla finestra;"));
}

void MainWindow::setImageView()
{
    if(imageViewAct->isChecked())
    {
         if(imagePan->setPixmapView(true))
             statusBar()->showMessage(tr("Visualizzo l'anteprima"), 2000);
         else
             statusBar()->showMessage(tr("Anteprima impostata"), 2000);
    }
    else
        imagePan->setPixmapView(false);
}

void MainWindow::setOffImageView()
{
    imagePan->setPixmapView(false);
}

void MainWindow::normalize()
{
    imagePan->normalize();
    statusBar()->showMessage(tr("Dimensione 100%"), 2000);
    mainPanelDock->setTextHistory("$: Dimensione 100%;");
}

void MainWindow::negativeTransform()
{
    int currentIndex= 0;
    imagePan->processImage(currentIndex);
    img_pix=imagePan->MyDisplay->setOutputPixmap();
    mainPanelDock->setOutputPix(img_pix);
    setDockHistogram();
    mainPanelDock->setImageDetails(getImageDetails(true));
    statusBar()->showMessage(tr("Negative image"), 2000);
    mainPanelDock->setTextHistory(tr("$: Negativo;"));
    imagePan->setPreviewOut(imagePan->MyDisplay->getCvMat());
    emit imageForMouseChanged();
}

void MainWindow::logTransform()
{
    int currentIndex= 1;
    imagePan->processImage(currentIndex);
    img_pix=imagePan->MyDisplay->setOutputPixmap();
    mainPanelDock->setOutputPix(img_pix);
    setDockHistogram();
    mainPanelDock->setImageDetails(getImageDetails(true));
    statusBar()->showMessage(tr("Logarithm transform"), 2000);
    mainPanelDock->setTextHistory(tr("$: Funzione logaritmo;"));  
    imagePan->setPreviewOut(imagePan->MyDisplay->getCvMat());
    emit imageForMouseChanged();
}

void MainWindow::invTransform()
{
    int currentIndex= 2;
    imagePan->processImage(currentIndex);
    img_pix=imagePan->MyDisplay->setOutputPixmap();
    mainPanelDock->setOutputPix(img_pix);
    setDockHistogram();
    mainPanelDock->setImageDetails(getImageDetails(false));
    statusBar()->showMessage(tr("Inverse logarithm transform"), 2000);
    mainPanelDock->setTextHistory(tr("$: Effetto inverso funzione logaritmo;"));   
    imagePan->setPreviewOut(imagePan->MyDisplay->getCvMat());
    emit imageForMouseChanged();
}


void MainWindow::fourierTransform()
{
    Mat src = imagePan->MyDisplay->getCvMat();
    ImgExample->fourierTransform(src);

    statusBar()->showMessage(tr("Fourier transform"), 2000);
    mainPanelDock->setTextHistory(tr("$: Trasformata di fourier;"));
}

void MainWindow::on_SobelChanged()
{
    int ksize = mainPanelDock->getKSize();
    double scale = mainPanelDock->getScale();
    double delta = mainPanelDock->getDelta();
    Mat img = imagePan->MyDisplay->getCvMat();
    Mat preview =myImageFiltering->sobelFiltering(img, ksize, scale, delta);
    imagePan->setPreviewOut(preview);
    QImage QtPreview = QImage((const unsigned char*)preview.data,static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    updateHistogram();
    mainPanelDock->setImageDetails(getImageDetails(false));
    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    emit imageForMouseChanged();
}

void MainWindow::sobelFilter()
{
    on_SobelChanged();
    filter= SOBEL_FILTERING;
    activateChoose(true);
    mainPanelDock->setSobelGroupBoxVisible(true);
    filterButtonsEnabled(false);
    connect(mainPanelDock, SIGNAL(sobelChanged()),this ,SLOT(on_SobelChanged()));
}

void MainWindow::on_cannyChanged()
{
    int ksize = mainPanelDock->getKernelSize();
    int ratio = mainPanelDock->getRatio();
    Mat img = imagePan->MyDisplay->getCvMat();
    Mat preview =myImageFiltering->findEdgeCanny(img, ksize, ratio);
    imagePan->setPreviewOut(preview);
    QImage QtPreview = QImage((const unsigned char*)preview.data,static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    updateHistogram();
    mainPanelDock->setImageDetails(getImageDetails(false));
    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    emit imageForMouseChanged();
}

void MainWindow::cannyFilter()
{
    on_cannyChanged();
    filter= CANNY_EDGE;
    activateChoose(true);
    mainPanelDock->setCannyGroupBoxVisible(true);
    filterButtonsEnabled(false);
    connect(mainPanelDock, SIGNAL(cannyChanged()),this ,SLOT(on_cannyChanged()));
}

void MainWindow::on_powerTransformChanged()
{
    double exponent = mainPanelDock->getPowerExponent();
    double coefficient = mainPanelDock->getPowerCoefficient();
    qDebug() << "exponent: " << exponent << "coefficient: " << coefficient;

    Mat img = imagePan->MyDisplay->getCvMat();
    Mat preview =myImageFiltering->powProcessing(img, exponent, coefficient);
    imagePan->setPreviewOut(preview);
    QImage QtPreview = QImage((const unsigned char*)preview.data,static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    updateHistogram();
    mainPanelDock->setImageDetails(getImageDetails(false));
    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    emit imageForMouseChanged();
}

void MainWindow::powerTransform()
{
    on_powerTransformChanged();
    filter= POWER_TRANSFORM;

    activateChoose(true);
    filterButtonsEnabled(false);
    mainPanelDock->setPowerGroupBoxVisible(true);
    connect(mainPanelDock, SIGNAL(powerTransformChanged()),this ,SLOT(on_powerTransformChanged()));
}

void MainWindow::on_sliceLevelChanged()
{
    bool binary = mainPanelDock->getBinaryCheck();
    double highLevel = mainPanelDock->getHighLevel();
    double lowLevel = mainPanelDock->getLowLevel();
    Mat img = imagePan->MyDisplay->getCvMat();
    Mat preview =myImageFiltering->levelSlicing(img, binary, highLevel, lowLevel);
    imagePan->setPreviewOut(preview);
    QImage QtPreview = QImage((const unsigned char*)preview.data,static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    updateHistogram();
    mainPanelDock->setImageDetails(getImageDetails(false));
    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    emit imageForMouseChanged();
}

void MainWindow::sliceLevelTransform()
{
    on_sliceLevelChanged();
    filter= SLICE_TRANSFORM;

    activateChoose(true);
    mainPanelDock->setSliceGroupBoxVisible(true);
    filterButtonsEnabled(false);
    connect(mainPanelDock, SIGNAL(sliceChanged()),this ,SLOT(on_sliceLevelChanged()));
}

void MainWindow::blurFilter()
{
    filter=BLUR_FILTERING;
    otherFilters();
    activateChoose(true);
    mainPanelDock->setGroupBoxTitle("Blur semplice");
}

void MainWindow::gaussianFilter()
{
    filter=GAUSSIANBLUR_FILTERING;
    otherFilters();
    activateChoose(true);
    mainPanelDock->setGroupBoxTitle("Gaussiano");
}

void MainWindow::medianFilter()
{
    filter=MEDIANBLUR_FILTERING;
    otherFilters();
    activateChoose(true);
    mainPanelDock->setGroupBoxTitle("Mediano");
}

void MainWindow::bilateralFilter()
{
    filter=BILATERALBLUR_FILTERING;
    otherFilters();
    activateChoose(true);
    mainPanelDock->setGroupBoxTitle("Bilaterale");
}

void MainWindow::on_otherFiltersChanged()
{
    int scroll = mainPanelDock->getOtherFiltersParameter();
    Mat img = imagePan->MyDisplay->getCvMat();
    Mat preview;
    if(filter==BLUR_FILTERING)
        preview =myImageFiltering->blurFiltering(img, scroll);
    else if(filter==GAUSSIANBLUR_FILTERING)
        preview =myImageFiltering->gaussianBlurFiltering(img, scroll);
    else if(filter==MEDIANBLUR_FILTERING)
        preview =myImageFiltering->medianBlurFiltering(img, scroll);
    else if(filter==BILATERALBLUR_FILTERING)
        preview =myImageFiltering->bilateralBlurFiltering(img, scroll);

    imagePan->setPreviewOut(preview);

    QImage QtPreview = QImage((const unsigned char*)preview.data,static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    updateHistogram();
    mainPanelDock->setImageDetails(getImageDetails(false));
    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    emit imageForMouseChanged();
}

void MainWindow::otherFilters()
{
    on_otherFiltersChanged();
    if (blurAct->isChecked()||gaussianBlurAct->isChecked()||medianBlurAct->isChecked()||bilateralBlurAct->isChecked())
    {
        mainPanelDock->setOtherFiltersGroupBoxVisible(true);
        connect(mainPanelDock, SIGNAL(otherFiltersChanged()),this ,SLOT(on_otherFiltersChanged()));
        filterButtonsEnabled(false);
    }
}


void MainWindow::erodeOperation()
{
    filter=ERODE_MORPHO;
    activateChoose(true);
    morphoOperation();
    mainPanelDock->setMorphoGroupBoxVisible(true);
    mainPanelDock->setMorphoGroupBoxTitle("Erodi");
}

void MainWindow::dilateOperation()
{
    filter=DILATE_MORPHO;
    activateChoose(true);
    morphoOperation();
    mainPanelDock->setMorphoGroupBoxVisible(true);
    mainPanelDock->setMorphoGroupBoxTitle("Dilata");
}

void MainWindow::on_morphoOperationChanged()
{
    int iterations = mainPanelDock->getIterationsParameter();
    Mat img = imagePan->MyDisplay->getCvMat();
    Mat preview;
    if(filter==ERODE_MORPHO)
        preview =myImageFiltering->erodeImage(img, iterations);
    else if(filter==DILATE_MORPHO)
        preview =myImageFiltering->dilateImage(img, iterations);

    imagePan->setPreviewOut(preview);

    QImage QtPreview = QImage((const unsigned char*)preview.data,static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    updateHistogram();
    mainPanelDock->setImageDetails(getImageDetails(false));
    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    emit imageForMouseChanged();
}

void MainWindow::morphoOperation()
{
    on_morphoOperationChanged();
    if (erodeAct->isChecked()||dilateAct->isChecked())
    {
        mainPanelDock->setMorphoGroupBoxVisible(true);
        connect(mainPanelDock, SIGNAL(morphoChanged()),this ,SLOT(on_morphoOperationChanged()));

        filterButtonsEnabled(false);
    }
}

void MainWindow::on_openCloseOperationChanged()
{
    int openParameter = mainPanelDock->getOpenParameter();
    qDebug() << "open parameter: " << openParameter;

    int closeParameter = mainPanelDock->getCloseParameter();
        qDebug() << "close parameter: " << closeParameter;

    int openingOption= mainPanelDock->getOpeningClosingComboOption();

    qDebug() << "openingOption: " << openingOption;

    Mat img = imagePan->MyDisplay->getCvMat();
    Mat preview;
    if(openingOption==0)
        preview =myImageFiltering->openingImage(img, openParameter);
    else if(openingOption==1)
        preview=myImageFiltering->closingImage(img, closeParameter);

    imagePan->setPreviewOut(preview);
    QImage QtPreview = QImage((const unsigned char*)preview.data,static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    updateHistogram();
    mainPanelDock->setImageDetails(getImageDetails(false));
    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    emit imageForMouseChanged();
}

void MainWindow::openCloseOperation()
{
    filter= OPENCLOSE_MORPHO;
    on_openCloseOperationChanged();
    mainPanelDock->setOpenCLoseBoxVisible(true);
    activateChoose(true);
    connect(mainPanelDock, SIGNAL(openCloseChanged()),this ,SLOT(on_openCloseOperationChanged()));
    filterButtonsEnabled(false);
}

void MainWindow::activateChoose(bool _status)
{
    //shows dialog buttons
    mainPanelDock->activateControls(_status);
}

void MainWindow::filterButtonsEnabled(bool setValue)
{
    sobelAct->setEnabled(setValue);
    cannyAct->setEnabled(setValue);
    laplacianAct->setEnabled(setValue);
    blurAct->setEnabled(setValue);
    gaussianBlurAct->setEnabled(setValue);
    medianBlurAct->setEnabled(setValue);
    bilateralBlurAct->setEnabled(setValue);
    powerTransformAct->setEnabled(setValue);
    sliceTransformAct->setEnabled(setValue);
    erodeAct->setEnabled(setValue);
    dilateAct->setEnabled(setValue);
    openCloseAct->setEnabled(setValue);
}

void MainWindow::filterButtonsChecked(bool setValue)
{
    sobelAct->setChecked(setValue);
    cannyAct->setChecked(setValue);
    laplacianAct->setChecked(setValue);
    blurAct->setChecked(setValue);
    gaussianBlurAct->setChecked(setValue);
    medianBlurAct->setChecked(setValue);
    bilateralBlurAct->setChecked(setValue);
    powerTransformAct->setChecked(setValue);
    sliceTransformAct->setChecked(setValue);
    erodeAct->setChecked(setValue);
    dilateAct->setChecked(setValue);
    openCloseAct->setChecked(setValue);
}

void MainWindow::on_laplacianChanged()
{
    int sigma = mainPanelDock->getSigma();
    int smooth = mainPanelDock->getSmoothing();
    myImageFiltering->setSmoothing(smooth);
    Mat img = imagePan->MyDisplay->getCvMat();
    Mat preview =myImageFiltering->laplacianFiltering(img, sigma);
    imagePan->setPreviewOut(preview);
    QImage QtPreview = QImage((const unsigned char*)preview.data, static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    updateHistogram();
    mainPanelDock->setImageDetails(getImageDetails(false));
    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    emit imageForMouseChanged();
}

void MainWindow::laplacianFilter()
{
    on_laplacianChanged();
    filter= LAPLACIAN_FILTERING;

    activateChoose(true);
    mainPanelDock->setLaplacianGroupBoxVisible(true);
    connect(mainPanelDock, SIGNAL(laplacianChanged()),this ,SLOT(on_laplacianChanged()));
    filterButtonsEnabled(false);
}

void MainWindow::filterAccepted()
{
    if(filter==SOBEL_FILTERING)
    {
        disconnect(mainPanelDock, SIGNAL(sobelChanged()),this ,SLOT(on_SobelChanged()));
        sobelAct->setEnabled(true);
        sobelAct->setChecked(false);
        statusBar()->showMessage(tr("Filtrata con Sobel"), 2000);
        mainPanelDock->setTextHistory(tr("$: Filtrata con Sobel;"));
        mainPanelDock->setSobelGroupBoxVisible(false);
    }

    if(filter==CANNY_EDGE)
    {
        disconnect(mainPanelDock, SIGNAL(cannyChanged()),this ,SLOT(on_cannyChanged()));
        cannyAct->setEnabled(true);
        cannyAct->setChecked(false);
        statusBar()->showMessage(tr("Edge detection con Canny"), 2000);
        mainPanelDock->setTextHistory(tr("$: Edge detection con Canny;"));
        mainPanelDock->setCannyGroupBoxVisible(false);
    }

    else if(filter==LAPLACIAN_FILTERING)
    {
        disconnect(mainPanelDock, SIGNAL(laplacianChanged()),this ,SLOT(on_laplacianChanged()));
        laplacianAct->setEnabled(true);
        laplacianAct->setChecked(false);
        statusBar()->showMessage(tr("Filtrata con il laplaciana"), 2000);
        mainPanelDock->setTextHistory(tr("$: Filtrata con con il laplaciano;"));
        mainPanelDock->setLaplacianGroupBoxVisible(false);
    }

    else if(filter==POWER_TRANSFORM)
    {
        disconnect(mainPanelDock, SIGNAL(powerTransformChanged()),this ,SLOT(on_powerTransformChanged()));
        powerTransformAct->setEnabled(true);
        powerTransformAct->setChecked(false);
        statusBar()->showMessage(tr("Applicata la trasformazione potenza"), 2000);
        mainPanelDock->setTextHistory(tr("$: Trasformazione di potenza;"));
        mainPanelDock->setPowerGroupBoxVisible(false);
    }

    else if(filter==SLICE_TRANSFORM)
    {
        disconnect(mainPanelDock, SIGNAL(sliceChanged()),this ,SLOT(on_sliceLevelChanged()));
        sliceTransformAct->setEnabled(true);
        sliceTransformAct->setChecked(false);
        statusBar()->showMessage(tr("Applicata la trasformazione slice"), 2000);
        mainPanelDock->setTextHistory(tr("$: Trasformazione slice;"));
        mainPanelDock->setSliceGroupBoxVisible(false);
    }

    else if(filter==SLICE_TRANSFORM)
    {
        disconnect(mainPanelDock, SIGNAL(sliceChanged()),this ,SLOT(on_sliceLevelChanged()));
        sliceTransformAct->setEnabled(true);
        sliceTransformAct->setChecked(false);
        statusBar()->showMessage(tr("Applicata la trasformazione slice"), 2000);
        mainPanelDock->setTextHistory(tr("$: Trasformazione slice;"));
        mainPanelDock->setSliceGroupBoxVisible(false);
    }

    else if(filter==OPENCLOSE_MORPHO)
    {
        disconnect(mainPanelDock, SIGNAL(openCloseChanged()),this ,SLOT(on_openCloseOperationChanged()));
        openCloseAct->setEnabled(true);
        openCloseAct->setChecked(false);
        mainPanelDock->setOpenCLoseBoxVisible(false);
        if(mainPanelDock->getOpeningClosingComboOption()==0)
        {
            statusBar()->showMessage(tr("Applicata all'immagine l'apertura"), 2000);
            mainPanelDock->setTextHistory(tr("$: Apertura;"));
        }
        else if(mainPanelDock->getOpeningClosingComboOption()==1)
        {
            statusBar()->showMessage(tr("Applicata all'immagine la chiusura"), 2000);
            mainPanelDock->setTextHistory(tr("$: Chiusura;"));
            mainPanelDock->setSliceGroupBoxVisible(false);
        }
    }

    else if (filter==BLUR_FILTERING||filter==GAUSSIANBLUR_FILTERING||filter==MEDIANBLUR_FILTERING||filter==BILATERALBLUR_FILTERING)
    {
        disconnect(mainPanelDock, SIGNAL(otherFiltersChanged()),this ,SLOT(on_otherFiltersChanged()));
        mainPanelDock->setOtherFiltersGroupBoxVisible(false);
        if(filter==BLUR_FILTERING)
        {
            statusBar()->showMessage(tr("Blur Filter"), 2000);
            mainPanelDock->setTextHistory(tr("$: Filtro blur;"));
        }
        else if(filter==GAUSSIANBLUR_FILTERING)
        {
            statusBar()->showMessage(tr("Gaussian blur Filter"), 2000);
            mainPanelDock->setTextHistory(tr("$: Filtro blur gaussiano;"));
        }
        else if(filter==MEDIANBLUR_FILTERING)
        {
            statusBar()->showMessage(tr("Median blur Filter"), 2000);
            mainPanelDock->setTextHistory(tr("$: Filtro blur mediano;"));
        }
        else if(filter==BILATERALBLUR_FILTERING)
        {
            statusBar()->showMessage(tr("Bilateral blur Filter"), 2000);
            mainPanelDock->setTextHistory(tr("$: Filtro  blur bilaterale;"));
        }
    }
    else if (filter==ERODE_MORPHO||filter==DILATE_MORPHO)
    {
        disconnect(mainPanelDock, SIGNAL(morphoChanged()),this ,SLOT(on_morphoOperationChanged()));
        mainPanelDock->setMorphoGroupBoxVisible(false);
        if(filter==ERODE_MORPHO)
        {
            statusBar()->showMessage(tr("Applicata erosione"), 2000);
            mainPanelDock->setTextHistory(tr("$: Applicata erosione;"));
        }
        else if(filter==DILATE_MORPHO)
        {
            statusBar()->showMessage(tr("Applicata dilatazione"), 2000);
            mainPanelDock->setTextHistory(tr("$: Applicata dilatazione;"));
        }
    }
}

void MainWindow::filterRejected()
{
    if(filter==SOBEL_FILTERING)
    {
        disconnect(mainPanelDock, SIGNAL(sobelChanged()),this ,SLOT(on_SobelChanged()));
        statusBar()->showMessage(tr("Filtraggio con Sobel annullato"), 2000);
        mainPanelDock->setTextHistory(tr("$: Filtraggio con Sobel annullato;"));
        mainPanelDock->setSobelGroupBoxVisible(false);
    }

    else if(filter==CANNY_EDGE)
    {
        disconnect(mainPanelDock, SIGNAL(cannyChanged()),this ,SLOT(on_cannyChanged()));
        statusBar()->showMessage(tr("Edge detection con Canny annullato"), 2000);
        mainPanelDock->setTextHistory(tr("$: Edge detection con Canny annullato;"));
        mainPanelDock->setCannyGroupBoxVisible(false);
    }

    else if(filter==LAPLACIAN_FILTERING)
    {
        disconnect(mainPanelDock, SIGNAL(laplacianChanged()),this ,SLOT(on_laplacianChanged()));
        statusBar()->showMessage(tr("Filtraggio con il laplaciano annullato"), 2000);
        mainPanelDock->setTextHistory(tr("$: Filtraggio con il laplaciano annullato;"));
        mainPanelDock->setLaplacianGroupBoxVisible(false);
    }

    else if(filter==POWER_TRANSFORM)
    {
        disconnect(mainPanelDock, SIGNAL(powerTransformChanged()),this ,SLOT(on_powerTransformChanged()));
        statusBar()->showMessage(tr("Trasformazione di potenza annullata"), 2000);
        mainPanelDock->setTextHistory(tr("$: Trasformazione di potenza annullata;"));
        mainPanelDock->setPowerGroupBoxVisible(false);
    }

    else if(filter==SLICE_TRANSFORM)
    {
        disconnect(mainPanelDock, SIGNAL(sliceChanged()),this ,SLOT(on_sliceLevelChanged()));
        statusBar()->showMessage(tr("Trasformazione di slice annullata"), 2000);
        mainPanelDock->setTextHistory(tr("$: Trasformazione di slice annullata;"));
        mainPanelDock->setSliceGroupBoxVisible(false);
    }

    else if (filter==BLUR_FILTERING||filter==GAUSSIANBLUR_FILTERING||filter==MEDIANBLUR_FILTERING||filter==BILATERALBLUR_FILTERING)
    {
        disconnect(mainPanelDock, SIGNAL(otherFiltersChanged()),this ,SLOT(on_otherFiltersChanged()));
        mainPanelDock->setOtherFiltersGroupBoxVisible(false);
    }

    else if (filter==ERODE_MORPHO||filter==DILATE_MORPHO)
    {
        disconnect(mainPanelDock, SIGNAL(morphoChanged()),this ,SLOT(on_morphoOperationChanged()));
        mainPanelDock->setMorphoGroupBoxVisible(false);
    }

    else if (filter==OPENCLOSE_MORPHO)
    {
        disconnect(mainPanelDock, SIGNAL(openCloseChanged()),this ,SLOT(on_openCloseOperationChanged()));
        mainPanelDock->setOpenCLoseBoxVisible(false);
    }
}

void MainWindow::previewAccept()
{
    filterButtonsChecked(false);
    imagePan->previewAccept();
    activateChoose(false);
    filterButtonsEnabled(true);
    filterAccepted();
    setDockHistogram();
    mainPanelDock->setImageDetails(getImageDetails(true));
    imagePan->setPreviewOut(imagePan->MyDisplay->getCvMat());
    setOffImageView();
}

void MainWindow::previewReject()
{
    Mat preview = imagePan->getPreviewIn();
    QImage QtPreview = QImage((const unsigned char*)preview.data, static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    if (outputPixmap.isNull())
        outputPixmap=imagePan->MyDisplay->setOutputPixmap();

    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    activateChoose(false);
    filterButtonsEnabled(true);
    filterButtonsChecked(false);
    filterRejected();
    setDockHistogram();
    mainPanelDock->setImageDetails(getImageDetails(true));
    imagePan->setPreviewOut(imagePan->MyDisplay->getCvMat());
    setOffImageView();
}

void MainWindow::on_saltAndPepperNoise()
{
    int n = informationPanel->getSingleParameterValue();
    Mat img = imagePan->MyDisplay->getCvMat();
    Mat preview =myImageFiltering->addSaltAndPepperNoise(img, n);
    imagePan->setPreviewOut(preview);
    QImage QtPreview = QImage((const unsigned char*)preview.data,static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    updateHistogram();
    mainPanelDock->setImageDetails(getImageDetails(false));
    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    emit imageForMouseChanged();
}

void MainWindow::on_gaussianNoise()
{
    double mean = informationPanel->getMean();
    double variance = informationPanel->getVariance();
    Mat img = imagePan->MyDisplay->getCvMat();
    Mat preview =myImageFiltering->addGaussianNoise(img, mean, variance);
    imagePan->setPreviewOut(preview);
    QImage QtPreview = QImage((const unsigned char*)preview.data,static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    updateHistogram();
    mainPanelDock->setImageDetails(getImageDetails(false));
    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    emit imageForMouseChanged();
}

void MainWindow::on_reduceLevel()
{
    QString level = informationPanel->getLevel();
    Mat img = imagePan->MyDisplay->getCvMat();
    Mat preview =myImageFiltering->getBitPlaneSlices(img, level);
    imagePan->setPreviewOut(preview);
    QImage QtPreview = QImage((const unsigned char*)preview.data, static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    updateHistogram();
    mainPanelDock->setImageDetails(getImageDetails(false));
    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    emit imageForMouseChanged();
}

void MainWindow::on_reduceGreyLevels()
{
    int greyLevelParameter = informationPanel->getReduceGreyParameterValue();
    Mat img = imagePan->MyDisplay->getCvMat();
    Mat preview =myImageFiltering->colorReduce(img,greyLevelParameter);
    imagePan->setPreviewOut(preview);
    QImage QtPreview = QImage((const unsigned char*)preview.data, static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    updateHistogram();
    mainPanelDock->setImageDetails(getImageDetails(false));
    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    emit imageForMouseChanged();
}

void MainWindow::processingAccepted()
{
    imagePan->previewAccept();
    informationPanel->setSingleParameterGroupBoxVisible(false);
    informationPanel->setDialogButtonsVisible(false);
    informationPanel->setGaussNoiseBoxVisible(false);
    informationPanel->setreduceGreyGroupBoxVisible(false);
    imagePan->setPreviewOut(imagePan->MyDisplay->getCvMat());
    setOffImageView();
}


void MainWindow::processingRejected()
{
    Mat preview = imagePan->getPreviewIn();
    QImage QtPreview = QImage((const unsigned char*)preview.data, static_cast<int>(preview.cols), static_cast<int>(preview.rows), static_cast<int>(preview.step), QImage::Format_Indexed8);
    QPixmap outputPixmap=QPixmap::fromImage(QtPreview);
    if (outputPixmap.isNull())
    outputPixmap=imagePan->MyDisplay->setOutputPixmap();
    mainPanelDock->setOutputPix(outputPixmap);
    mainPanelDock->setContenentsImage();
    informationPanel->setSingleParameterGroupBoxVisible(false);
    informationPanel->setDialogButtonsVisible(false);
    informationPanel->setGaussNoiseBoxVisible(false);
    informationPanel->setreduceGreyGroupBoxVisible(false);
    setDockHistogram();
    mainPanelDock->setImageDetails(getImageDetails(true));
    imagePan->setPreviewOut(imagePan->MyDisplay->getCvMat());
    setOffImageView();
}

void MainWindow::selectROI()
{
    imagePan->myGraphicsView->setDragMode(QGraphicsView::NoDrag);

    dragAct->setChecked(false);
    imagePan->selectROI();
    roiSelectAct->setEnabled(false);
    mainPanelDock->setCancelButtonVisible(true);
}

void MainWindow::extractROI()
{
    mainPanelDock->setCancelButtonVisible(false);
    roiExtractAct->setEnabled(false);
    imagePan->extractROI();
    imagePan->scene->rubberBandDragEnabled(false);
    roiSelectAct->setChecked(false);
    roiSelectAct->setEnabled(true);
    mainPanelDock->setTextHistory(tr("$: ROI estratta;"));
    statusBar()->showMessage(tr("ROI estratta"), 2000);
}

void MainWindow::canceltROI()
{
    imagePan->cancelROI();
    mainPanelDock->setCancelButtonVisible(false);
    roiExtractAct->setEnabled(false);
    roiSelectAct->setChecked(false);
    roiSelectAct->setEnabled(true);
    mainPanelDock->setTextHistory(tr("$: Selezione ROI annullata;"));
    statusBar()->showMessage(tr("ROI selection deleted"), 2000);
}

void MainWindow::createHelpWindow(){
    QHelpEngine* helpEngine = new QHelpEngine(
                QApplication::applicationDirPath() +
                "/documentation/imageprocessing.1.0.qhc");

    helpEngine->setupData();
    QTabWidget* tWidget = new QTabWidget;
    tWidget->setMaximumWidth(200);
    tWidget->addTab(helpEngine->contentWidget(), "Sommario");
    tWidget->addTab(helpEngine->indexWidget(), "Indice");

    HelpBrowser *textViewer = new HelpBrowser(helpEngine);
    textViewer->setSource(
                QUrl("qthelp://ing.giordano.com.imageprocessing/doc/index.html"));

    connect(helpEngine->contentWidget(), SIGNAL(linkActivated(QUrl)), textViewer, SLOT(setSource(QUrl)));
    connect(helpEngine->indexWidget(), SIGNAL(linkActivated(QUrl, QString)), textViewer, SLOT(setSource(QUrl)));

    QSplitter *horizSplitter = new QSplitter(Qt::Horizontal);
    horizSplitter->insertWidget(0, tWidget);
    horizSplitter->insertWidget(1, textViewer);

    helpWindow = new QDockWidget(tr("Guida in linea"), this);
    helpWindow->setWidget(horizSplitter);
    helpWindow->setMinimumHeight(600);
    helpWindow->hide();
    addDockWidget(Qt::RightDockWidgetArea, helpWindow);
}

void MainWindow::setRoiExtractEnabled()
{
    roiExtractAct->setEnabled(true);
}

void MainWindow::dragMode(){
    if(dragAct->isChecked())
        imagePan->myGraphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    else
        imagePan->myGraphicsView->setDragMode(QGraphicsView::NoDrag);
}

void MainWindow::updateHistogram()
{
    Mat previewImage=imagePan->getPreviewOut();
    imagePan->showHistogramDiagram(previewImage);
    QPixmap hist_pix=imagePan->getHistogramPixmap();
    mainPanelDock->setHistogram(hist_pix);
}

void MainWindow::setDockHistogram()
{
    Mat previewImage=imagePan->MyDisplay->getCvMat();
    imagePan->showHistogramDiagram(previewImage);
    QPixmap hist_pix=imagePan->getHistogramPixmap();
    mainPanelDock->setHistogram(hist_pix);
}

void MainWindow::readSettings()
{
    QSettings settings("Carmine GIORDANO", "Spreadsheet");
    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();
    restoreGeometry(settings.value("geometry").toByteArray());
}

void MainWindow::writeSettings()
{
    QSettings settings("Carmine GIORDANO.", "Image Processing");
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("geometry", saveGeometry());
}
