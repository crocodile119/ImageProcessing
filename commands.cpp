#include "commands.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

const int UndoCommandType = 100;
const int RedoCommandType = 101;

ImageProcessingCommand::ImageProcessingCommand(ImageProcessing *ImgExample, int currentIndex,
                                                QGraphicsScene *scene, QUndoCommand *parent):QUndoCommand(parent)


{
    this->ImgExample = ImgExample;
    this->scene = scene;
    MyDisplay= new Display();
    MyDisplay=ImgExample->getDisplay();
    imgUnprocessed=MyDisplay->getCvMat();
    //ImgExample->setDisplay(MyDisplay);


    this->currentIndex = currentIndex;
}

void ImageProcessingCommand::redo()
{
    resetImage(RedoCommandType);
}

void ImageProcessingCommand::undo()
{
    resetImage(UndoCommandType);
}

void ImageProcessingCommand::resetImage(const int commandType)
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
        img_mat = MyDisplay->getCvMat();
        if(currentIndex == 0)
           ImgExample->negativeProcessing(img_mat, processed);
        else if(currentIndex == 1)
           ImgExample->logProcessing(img_mat, processed);
        else if(currentIndex == 2)
           ImgExample->invProcessing(img_mat, processed);
        else if(currentIndex == 3)
           ImgExample->powProcessing(img_mat, processed);
        else if(currentIndex == 4)
           ImgExample->sqrtProcessing(img_mat, processed);
        else if(currentIndex == 5)
           ImgExample->cannyProcessing(img_mat, processed);

        MyDisplay->setCvMat(processed);
        MyDisplay->setQtImage(processed);

         img_pix = MyDisplay->setOutputPixmap();
         scene->addPixmap(img_pix);
      }
    ImgExample->setDisplay(MyDisplay);
    scene->update();
}


ImageProcessingCommand::~ImageProcessingCommand()
{

}
