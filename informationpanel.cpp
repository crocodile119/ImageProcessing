#include "informationpanel.h"
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
InformationPanel::InformationPanel()
{
    informationDockWidget = new QWidget();
    informationDockWidget->setObjectName(QStringLiteral("informationDockWidget"));
    verticalLayout = new QVBoxLayout(informationDockWidget);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

    treeWidget = new QTreeWidget(informationDockWidget);
    QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
    __qtreewidgetitem->setText(0, QStringLiteral("riduci livelli"));
    treeWidget->setHeaderItem(__qtreewidgetitem);
    QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(treeWidget);
    new QTreeWidgetItem(__qtreewidgetitem1);
    new QTreeWidgetItem(__qtreewidgetitem1);
    QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem(treeWidget);
    new QTreeWidgetItem(__qtreewidgetitem2);
    new QTreeWidgetItem(__qtreewidgetitem2);
    new QTreeWidgetItem(__qtreewidgetitem2);
    new QTreeWidgetItem(__qtreewidgetitem2);
    new QTreeWidgetItem(__qtreewidgetitem2);
    new QTreeWidgetItem(__qtreewidgetitem2);
    new QTreeWidgetItem(__qtreewidgetitem2);
    new QTreeWidgetItem(treeWidget);
    treeWidget->setObjectName(QStringLiteral("treeWidget"));
    treeWidget->setColumnCount(1);
    treeWidget->header()->setVisible(false);
    treeWidget->header()->setCascadingSectionResizes(true);
    treeWidget->header()->setDefaultSectionSize(200);
    treeWidget->header()->setHighlightSections(false);
    treeWidget->header()->setMinimumSectionSize(200);
    treeWidget->header()->setStretchLastSection(true);

    verticalLayout->addWidget(treeWidget);

    const bool __sortingEnabled = treeWidget->isSortingEnabled();
    treeWidget->setSortingEnabled(false);
    QTreeWidgetItem *___qtreewidgetitem = treeWidget->topLevelItem(0);
    ___qtreewidgetitem->setText(0, tr("aggiungi disturbo"));
    QTreeWidgetItem *___qtreewidgetitem1 = ___qtreewidgetitem->child(0);
    ___qtreewidgetitem1->setText(0, tr("rumore gaussiano"));
    QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem->child(1);
    ___qtreewidgetitem2->setText(0, tr("rumore salt and pepper"));

    QTreeWidgetItem *___qtreewidgetitem4 = treeWidget->topLevelItem(1);
    ___qtreewidgetitem4->setText(0,tr("riduci livelli"));
    QTreeWidgetItem *___qtreewidgetitem5 = ___qtreewidgetitem4->child(0);
    ___qtreewidgetitem5->setText(0, tr("2 livelli"));
    QTreeWidgetItem *___qtreewidgetitem6 = ___qtreewidgetitem4->child(1);
    ___qtreewidgetitem6->setText(0, tr("4 livelli"));
    QTreeWidgetItem *___qtreewidgetitem7 = ___qtreewidgetitem4->child(2);
    ___qtreewidgetitem7->setText(0, tr("8 livelli"));
    QTreeWidgetItem *___qtreewidgetitem8 = ___qtreewidgetitem4->child(3);
    ___qtreewidgetitem8->setText(0, tr("16 livelli"));
    QTreeWidgetItem *___qtreewidgetitem9 = ___qtreewidgetitem4->child(4);
    ___qtreewidgetitem9->setText(0, tr("32 livelli"));
    QTreeWidgetItem *___qtreewidgetitem10 = ___qtreewidgetitem4->child(5);
    ___qtreewidgetitem10->setText(0, tr("64 livelli"));
    QTreeWidgetItem *___qtreewidgetitem11 = ___qtreewidgetitem4->child(6);
    ___qtreewidgetitem11->setText(0, tr("128 livelli"));
    QTreeWidgetItem *___qtreewidgetitem12 = treeWidget->topLevelItem(2);
    ___qtreewidgetitem12->setText(0, tr("riduci numero di livelli di grigio"));
    treeWidget->setSortingEnabled(__sortingEnabled);

    //gaussNoiseGroupBox controls
    gaussNoiseGroupBox = new QGroupBox(informationDockWidget);
    gaussNoiseGroupBox->setObjectName(QStringLiteral("gaussNoiseGroupBox"));
    gaussNoiseGroupBox->setEnabled(true);
    gaussNoiseGroupBox->setAutoFillBackground(false);
    gaussNoiseGridLayout = new QGridLayout(gaussNoiseGroupBox);
    gaussNoiseGridLayout->setObjectName(QStringLiteral("gaussNoiseGridLayout"));
    meanSpinBox = new QDoubleSpinBox(gaussNoiseGroupBox);
    meanSpinBox->setObjectName(QStringLiteral("meanSpinBox"));
    meanSpinBox->setMinimum(0);
    meanSpinBox->setMaximum(1);
    meanSpinBox->setSingleStep(0.1);
    meanSpinBox->setValue(1.0);
    gaussNoiseGridLayout->addWidget(meanSpinBox, 2, 1, 1, 1);

    varianceSpinBox = new QDoubleSpinBox(gaussNoiseGroupBox);
    varianceSpinBox->setObjectName(QStringLiteral("varianceSpinBox"));
    varianceSpinBox->setMinimum(0);
    varianceSpinBox->setMaximum(1);
    varianceSpinBox->setSingleStep(0.1);

    gaussNoiseGridLayout->addWidget(varianceSpinBox, 2, 2, 1, 1);

    meanLabel = new QLabel(gaussNoiseGroupBox);
    meanLabel->setObjectName(QStringLiteral("meanLabel"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(meanLabel->sizePolicy().hasHeightForWidth());
    meanLabel->setSizePolicy(sizePolicy);
    meanLabel->setAlignment(Qt::AlignCenter);

    gaussNoiseGridLayout->addWidget(meanLabel, 1, 1, 1, 1);

    varianceLabel = new QLabel(gaussNoiseGroupBox);
    varianceLabel->setObjectName(QStringLiteral("ScaleLabel"));

    sizePolicy.setHeightForWidth(varianceLabel->sizePolicy().hasHeightForWidth());
    varianceLabel->setSizePolicy(sizePolicy);
    varianceLabel->setAlignment(Qt::AlignCenter);

    gaussNoiseGridLayout->addWidget(varianceLabel, 1, 2, 1, 1);

    verticalLayout->addWidget(gaussNoiseGroupBox);

    gaussNoiseGroupBox->setTitle(tr("Rumore gaussiano"));
    meanLabel->setText(tr("Media"));
    varianceLabel->setText(tr("Varianza"));

// single parameter groupbox

    singleParameterGroupBox = new QGroupBox(informationDockWidget);
    singleParameterGroupBox->setObjectName(QStringLiteral("singleParameterGroupBox"));
    singleParameterGroupBox->setEnabled(true);
    singleParameterGroupBox->setAutoFillBackground(false);

    singleParameterGridLayout = new QGridLayout(singleParameterGroupBox);
    singleParameterGridLayout->setObjectName(QStringLiteral("singleParameterGridLayout"));

    scrollLabel = new QLabel(singleParameterGroupBox);
    scrollLabel->setObjectName(QStringLiteral("scrollLabel"));
    scrollLabel->setAlignment(Qt::AlignCenter);

    singleParameterGridLayout->addWidget(scrollLabel, 0, 0, 1, 1);

    scrollLayout = new QHBoxLayout();
    scrollLayout->setObjectName(QStringLiteral("scrollLayout"));
    scrollDisplayLabel = new QLabel(singleParameterGroupBox);
    scrollDisplayLabel->setObjectName(QStringLiteral("scrollDisplayLabel"));
    scrollDisplayLabel->setAutoFillBackground(false);
    scrollDisplayLabel->setScaledContents(true);
    scrollDisplayLabel->setWordWrap(false);

    scrollLayout->addWidget(scrollDisplayLabel);

    singleParameterScrollBar = new QScrollBar(singleParameterGroupBox);
    singleParameterScrollBar->setObjectName(QStringLiteral("singleParameterBar"));
    QSizePolicy singleParameterSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    singleParameterSizePolicy.setHorizontalStretch(1);
    singleParameterSizePolicy.setVerticalStretch(0);
    singleParameterSizePolicy.setHeightForWidth(singleParameterScrollBar->sizePolicy().hasHeightForWidth());
    singleParameterScrollBar->setSizePolicy(singleParameterSizePolicy);
    singleParameterScrollBar->setMinimum(1);
    singleParameterScrollBar->setMaximum(260000);
    singleParameterScrollBar->setSingleStep(10);
    singleParameterScrollBar->setValue(2);
    singleParameterScrollBar->setSliderPosition(0);
    singleParameterScrollBar->setOrientation(Qt::Horizontal);

    scrollLayout->addWidget(singleParameterScrollBar);

    scrollLabel = new QLabel(singleParameterGroupBox);
    scrollLabel->setObjectName(QStringLiteral("scrollLabel"));
    scrollLabel->setAlignment(Qt::AlignCenter);
    singleParameterGridLayout->addWidget(scrollLabel, 3, 0, 1, 1);

    singleParameterGridLayout->addLayout(scrollLayout, 2, 0, 1, 1);
    verticalLayout->addWidget(singleParameterGroupBox);

    singleParameterGroupBox->setTitle(tr("Rumore Salt and Pepper"));
    scrollLabel->setText(tr("Densità"));
    scrollDisplayLabel->setText(tr("2"));


// reduce grey levels groupbox

    reduceGreyGroupBox = new QGroupBox(informationDockWidget);
    reduceGreyGroupBox->setObjectName(QStringLiteral("reduceGreyGroupBox"));
    reduceGreyGroupBox->setEnabled(true);
    reduceGreyGroupBox->setAutoFillBackground(false);

    reduceGreyGridLayout = new QGridLayout(reduceGreyGroupBox);
    reduceGreyGridLayout->setObjectName(QStringLiteral("reduceGreyGridLayout"));

    reduceGreyLabel = new QLabel(reduceGreyGroupBox);
    reduceGreyLabel->setObjectName(QStringLiteral("reduceGreyLabel"));
    reduceGreyLabel->setAlignment(Qt::AlignCenter);

    reduceGreyGridLayout->addWidget(reduceGreyLabel, 0, 0, 1, 9);

    reduceGreyScrollBar = new QScrollBar(reduceGreyGroupBox);
    reduceGreyScrollBar->setObjectName(QStringLiteral("reduceGreyScrollBar"));
    reduceGreyScrollBar->setMinimum(1);
    reduceGreyScrollBar->setMaximum(255);
    reduceGreyScrollBar->setSingleStep(1);
    reduceGreyScrollBar->setValue(1);
    reduceGreyScrollBar->setSliderPosition(128);
    reduceGreyScrollBar->setOrientation(Qt::Horizontal);

    reduceGreyGridLayout->addWidget(reduceGreyScrollBar, 1, 1, 1, 8);

    reduceGreyDisplayLabel = new QLabel(reduceGreyGroupBox);
    reduceGreyDisplayLabel->setObjectName(QStringLiteral("reduceGreyDisplayLabel"));
    reduceGreyDisplayLabel->setAlignment(Qt::AlignCenter);
    reduceGreyGridLayout->addWidget(reduceGreyDisplayLabel, 1, 0, 1, 1);

    verticalLayout->addWidget(reduceGreyGroupBox);

    reduceGreyGroupBox->setTitle(tr("Riduce il numero dei livelli di grigio"));
    reduceGreyLabel->setText(tr("Livelli di grigio"));
    reduceGreyDisplayLabel->setText(tr("1"));

    //push buttons
    buttonsWidget = new QWidget(informationDockWidget);
    buttonsWidget->setObjectName(QStringLiteral("buttonsWidget"));
    buttonsGridLayout = new QGridLayout(buttonsWidget);
    buttonsGridLayout->setObjectName(QStringLiteral("buttonsGridLayout"));
    rejectButton = new QPushButton(buttonsWidget);
    rejectButton->setObjectName(QStringLiteral("rejectButton"));

    QSizePolicy buttonSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    buttonSizePolicy.setHorizontalStretch(0);
    buttonSizePolicy.setVerticalStretch(1);
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
    acceptButton->setText(tr("Applica"));

    //set the dockwidget
    setWidget(informationDockWidget);
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    treeWidget->setVisible(false);
    singleParameterGroupBox->setVisible(false);
    //informationTextEdit->setVisible(false);
    buttonsWidget->setVisible(false);
    gaussNoiseGroupBox->setVisible(false);
    reduceGreyGroupBox->setVisible(false);

    connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(on_treeWidget_itemClicked(QTreeWidgetItem*, int)));
    connect(singleParameterScrollBar, SIGNAL(valueChanged(int)), this, SLOT(on_singleParameterScrollBar_valueChanged(int)));
    connect(reduceGreyScrollBar, SIGNAL(valueChanged(int)), this, SLOT(on_reduceGreyScrollBar_valueChanged(int)));
    connect(meanSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_meanSpinBox_valueChanged(double)));
    connect(varianceSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_meanSpinBox_valueChanged(double)));
    connect(rejectButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(on_acceptButton_clicked()));
}

void InformationPanel::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->text(column)=="rumore salt and pepper")
    {
        setSingleParameterGroupBoxVisible(true);
        buttonsWidget->setVisible(true);
        gaussNoiseGroupBox->setVisible(false);
        emit singleParameterChanged();
    }
    else if(item->text(column)=="rumore gaussiano")
    {
        setSingleParameterGroupBoxVisible(false);
        setreduceGreyGroupBoxVisible(false);
        gaussNoiseGroupBox->setVisible(true);
        buttonsWidget->setVisible(true);
        emit addingGaussianNoise();
    }
    else if(item->text(column)=="aggiungi disturbo")
    {
        setSingleParameterGroupBoxVisible(false);
        buttonsWidget->setVisible(false);
        gaussNoiseGroupBox->setVisible(false);
        setreduceGreyGroupBoxVisible(false);
    }

    else if(item->text(column)=="riduci livelli")
    {
        setSingleParameterGroupBoxVisible(false);
        buttonsWidget->setVisible(false);
        gaussNoiseGroupBox->setVisible(false);
        setreduceGreyGroupBoxVisible(false);
    }

    else if(item->text(column)=="riduci numero di livelli di grigio")
    {
        setSingleParameterGroupBoxVisible(false);
        buttonsWidget->setVisible(true);
        gaussNoiseGroupBox->setVisible(false);
        setreduceGreyGroupBoxVisible(true);
        emit redeuceGreyChanged();
    }
    else
    {
        level =item->text(column);
        setSingleParameterGroupBoxVisible(false);
        gaussNoiseGroupBox->setVisible(false);
        buttonsWidget->setVisible(true);
        emit planeChanged();
    }
}

QString InformationPanel::getLevel()
{
    return level;
}

void InformationPanel::setTreeWidgetVisible(bool status)
{
    treeWidget->setVisible(status);
}

//single parameter group
void InformationPanel::setSingleParameterGroupBoxVisible(bool status)
{
    singleParameterGroupBox->setVisible(status);
}

void InformationPanel::setDialogButtonsVisible(bool status)
{
    buttonsWidget->setVisible(status);
}

int InformationPanel::getSingleParameterValue()
{
    return scrollDisplayLabel->text().toInt();
}

void InformationPanel::on_singleParameterScrollBar_valueChanged(int value)
{
    QString singleParameterString (QString::number(value));
    scrollDisplayLabel->setText(singleParameterString);
    emit singleParameterChanged();
}

void InformationPanel::setGroupBoxTitle(QString title)
{
    singleParameterGroupBox->setTitle(title);
}

// gaussian noise group

void InformationPanel::on_meanSpinBox_valueChanged(double _value)
{
    Q_UNUSED(_value);
    emit addingGaussianNoise();
}

void InformationPanel::on_varianceSpinBox_valueChanged(double _value)
{
    Q_UNUSED(_value);
    emit addingGaussianNoise();
}

double InformationPanel::getMean()
{
    return meanSpinBox->value();
}

double InformationPanel::getVariance()
{
    return varianceSpinBox->value();
}

void InformationPanel::setGaussNoiseBoxVisible(bool _visible)
{
    gaussNoiseGroupBox->setVisible(_visible);
}

//reduce grey levels group
void InformationPanel::setreduceGreyGroupBoxVisible(bool status)
{
    reduceGreyGroupBox->setVisible(status);
}

int InformationPanel::getReduceGreyParameterValue()
{
    int myValue=reduceGreyDisplayLabel->text().toInt();
    myValue=256 - myValue;
    return myValue;
}

void InformationPanel::on_reduceGreyScrollBar_valueChanged(int value)
{
    QString reduceGreyString (QString::number(value));
    reduceGreyDisplayLabel->setText(reduceGreyString);
    emit redeuceGreyChanged();
}


//buttons
void InformationPanel::on_acceptButton_clicked()
{
    emit accept();
}

void InformationPanel::on_cancelButton_clicked()
{
    emit cancel();
}

InformationPanel::~InformationPanel()
{
}


