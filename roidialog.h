#ifndef ROIDIALOG_H
#define ROIDIALOG_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class RoiDialog;
}

class RoiDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoiDialog(QWidget *parent = 0);
    ~RoiDialog();
     Ui::RoiDialog *ui;
     QPixmap qtRoiPixmap;

private:

private slots:
     void reject();
     void on_pushButton_clicked();
     void on_saveROI_clicked();
};

#endif // ROIDIALOG_H
