#ifndef EQUALIZER_H
#define EQUALIZER_H

#include <QUndoCommand>
#include <display.h>
#include <ImageProcessing.h>
#include <QGraphicsView>
#include <QGraphicsScene>

class ImageEqualizerCommand : public QUndoCommand
{
public:
    ImageEqualizerCommand(ImageProcessing *ImgExample, QGraphicsScene *scene,
                           QUndoCommand *parent = 0);

    virtual ~ImageEqualizerCommand ();
    void undo();
    void redo();
private:
    ImageProcessing *ImgExample;
    Display *MyDisplay;
    QGraphicsScene *scene;
    Mat img_mat;
    Mat imgUnprocessed;
    Mat processed;
    void resetImageEqualizer(const int);
    QPixmap img_pix;
};

#endif // EQUALIZER_H
