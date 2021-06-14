#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <display.h>
#include <ImageProcessing.h>
#include <opencv2/core/core.hpp>
#include <QGraphicsView>
#include <QGraphicsScene>

class ImageProcessingCommand : public QUndoCommand
{
public:
    ImageProcessingCommand(ImageProcessing *ImgExample, int currentIndex, QGraphicsScene *scene,
                           QUndoCommand *parent = 0);


    virtual ~ImageProcessingCommand ();
    void undo();
    void redo();
private:
    ImageProcessing *ImgExample;
    Display *MyDisplay;
    QGraphicsScene *scene;
    Mat img_mat;
    Mat imgUnprocessed;
    Mat processed;
    int currentIndex;
    void resetImage(const int commandType);
    QPixmap img_pix;
};

#endif // COMMANDS_H
