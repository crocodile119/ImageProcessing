#include "previewcommands.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

const int UndoCommandType = 100;
const int RedoCommandType = 101;

PreviewCommand::PreviewCommand(ImageProcessing *ImgExample, Mat previewOut, QGraphicsScene *scene, QUndoCommand *parent):

    QUndoCommand(parent)


{
    this->ImgExample = ImgExample;
    this->scene = scene;
    MyDisplay= new Display();
    MyDisplay=ImgExample->getDisplay();
    previewIn=MyDisplay->getCvMat();
    ImgExample->setDisplay(MyDisplay);


    this->previewOut = previewOut;
}

void PreviewCommand::redo(){
    resetImage(RedoCommandType);
}

void PreviewCommand::undo(){
    resetImage(UndoCommandType);
}

void PreviewCommand::resetImage(const int commandType){

         if(commandType == UndoCommandType)

         {
         MyDisplay->setCvMat(previewIn);
         MyDisplay->setQtImage(previewIn);

         img_pix = MyDisplay->setOutputPixmap();
         scene->addPixmap(img_pix);
         }

         else
         {
             MyDisplay->setCvMat(previewOut);
             MyDisplay->setQtImage(previewOut);

             img_pix = MyDisplay->setOutputPixmap();
             scene->addPixmap(img_pix);
         }
                  ImgExample->setDisplay(MyDisplay);
                  scene->update();
}


PreviewCommand::~PreviewCommand()
{

}
