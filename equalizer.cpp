#include "equalizer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

const int UndoCommandType = 100;
const int RedoCommandType = 101;

ImageEqualizerCommand::ImageEqualizerCommand(ImageProcessing *ImgExample,
                                               QGraphicsScene *scene, QUndoCommand *parent):

    QUndoCommand(parent)

{
    this->ImgExample = ImgExample;
    this->scene= scene;
    MyDisplay= new Display();
    MyDisplay=ImgExample->getDisplay();
    imgUnprocessed=MyDisplay->getCvMat();
    ImgExample->setDisplay(MyDisplay);
}


void ImageEqualizerCommand::redo()
{
    resetImageEqualizer(RedoCommandType);
}

void ImageEqualizerCommand::undo()
{
    resetImageEqualizer(UndoCommandType);
}

void ImageEqualizerCommand::resetImageEqualizer(const int commandType)
{
    if(commandType == UndoCommandType)
    {
        MyDisplay->setCvMat(imgUnprocessed);
        MyDisplay->setQtImage(imgUnprocessed);
        img_pix = MyDisplay->setOutputPixmap();
        scene->addPixmap(img_pix);
    }
    else
    {
        Mat image=MyDisplay->getCvMat();
        Mat result;
        equalizeHist(image,result);
        MyDisplay->setCvMat(result);
        MyDisplay->setQtImage(result);
        img_pix = MyDisplay->setOutputPixmap();
        scene->addPixmap(img_pix);
    }
    ImgExample->setDisplay(MyDisplay);
    scene->update();
}

ImageEqualizerCommand::~ImageEqualizerCommand()
{
}
