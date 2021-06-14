#ifndef SHOWHISTOGRAM_H
#define SHOWHISTOGRAM_H

#include <QDialog>

namespace Ui {
class ShowHistogram;
}

class ShowHistogram : public QDialog
{
    Q_OBJECT

public:
    explicit ShowHistogram(QWidget *parent = 0);
    ~ShowHistogram();
    Ui::ShowHistogram *ui;

private slots:

private:

};

#endif // SHOWHISTOGRAM_H
