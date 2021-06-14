#ifndef PREVIEWCOMMANDS_H
#define PREVIEWCOMMANDS_H

#include <QUndoCommand>
#include <display.h>
#include <ImageProcessing.h>
#include <opencv2/core/core.hpp>
#include <QGraphicsView>
#include <QGraphicsScene>

class PreviewCommand : public QUndoCommand
{
public:
    PreviewCommand(ImageProcessing *ImgExample, Mat previewOut, QGraphicsScene *scene,
                           QUndoCommand *parent = 0);


    virtual ~PreviewCommand ();
    void undo();
    void redo();
private:
    ImageProcessing *ImgExample;
    Display *MyDisplay;
    QGraphicsScene *scene;
    Mat previewOut;
    Mat previewIn;

    void resetImage(const int commandType);
    QPixmap img_pix;
};

#endif // PREVIEWCOMMANDS_H
