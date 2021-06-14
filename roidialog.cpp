#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include "roidialog.h"
#include "ui_roidialog.h"

RoiDialog::RoiDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoiDialog)
{
    ui->setupUi(this);
}

RoiDialog::~RoiDialog()
{
    delete ui;
}

void RoiDialog::reject()
{

        QDialog::accept();

}

void RoiDialog::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                               tr("Esprt in csv"), ".",
                               tr("Cascade Style  (*.csv)"));

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);

    if (file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream data( &file );
        QStringList strList;
        QApplication::setOverrideCursor(Qt::WaitCursor);
        for( int r = 0; r < ui->tableWidget->rowCount(); ++r )
        {
            strList.clear();
            for( int c = 0; c < ui->tableWidget->columnCount(); ++c )
            {
                strList << "\" "+ui->tableWidget->item( r, c )->text()+"\" ";
            }
            data << strList.join( ";" )+"\n";
        }
        file.close();
        QApplication::restoreOverrideCursor();
        QMessageBox::information(this, tr("Image processing"),
                             tr("File exported correctly"));
    }

}


void RoiDialog::on_saveROI_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "../untitled.png",
                               tr("Images (*.png *.xpm *.jpg)"));

    if (fileName.isEmpty()) {
        return;
    }
    //qtRoiImage=qtRoiImage.rgbSwapped();
    //Q_ASSERT(!qtRoiPixmap.isNull());
         qtRoiPixmap.save(fileName, 0,-1);
}
