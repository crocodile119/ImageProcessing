#include "showhistogram.h"
#include "ui_showhistogram.h"

ShowHistogram::ShowHistogram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowHistogram)
{
    ui->setupUi(this);
}

ShowHistogram::~ShowHistogram()
{
    delete ui;
}
