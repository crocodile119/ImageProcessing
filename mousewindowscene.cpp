#include "mousewindowscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>

MouseWindowScene::MouseWindowScene(QObject *parent)
    : QGraphicsScene(parent)
{
     rectangle=0;
     myMode = InsertItem;
}

void MouseWindowScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    if(myMode==InsertItem){

        MousePressPosition =(mouseEvent->scenePos(), mouseEvent->scenePos());
        rectangle = new QGraphicsRectItem(QRectF(mouseEvent->scenePos(), mouseEvent->scenePos()));
        addItem(rectangle);
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MouseWindowScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
     if (myMode==InsertItem && rectangle!= 0) {

        QPointF mouseCoord = mouseEvent->scenePos();
            if((mouseCoord.rx()>MousePressPosition.x()&& mouseCoord.ry()>MousePressPosition.y()))
            {
                    QRectF newRectangle(MousePressPosition, mouseEvent->scenePos() );
                         rectangle->setRect(newRectangle);
                         addItem(rectangle);
                         rectangle->setPen(QColor(0, 0, 255, 255));
            }
                    else if ((mouseCoord.rx()<MousePressPosition.x()&& mouseCoord.ry()<MousePressPosition.y()))
            {
                    QRectF newRectangle(mouseEvent->scenePos(), MousePressPosition);
                         rectangle->setRect(newRectangle);
                         rectangle->setPen(QColor(0, 0, 255, 255));
                         addItem(rectangle);
            }
            QGraphicsScene::update();
    }
        QGraphicsScene::mousePressEvent(mouseEvent);

}

void MouseWindowScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode==InsertItem && rectangle!= 0) {

        QPointF mouseCoord = mouseEvent->scenePos();
            if((mouseCoord.rx()>MousePressPosition.x()&& mouseCoord.ry()>MousePressPosition.y()))
            {
                    QRectF newRectangle(MousePressPosition, mouseEvent->scenePos() );
                         rectangle->setRect(newRectangle);
                         rectangle->setPen(QColor(0, 0, 255, 255));
                         addItem(rectangle);
            }
                    else if ((mouseCoord.rx()<MousePressPosition.x()&& mouseCoord.ry()<MousePressPosition.y()))
            {
                    QRectF newRectangle(mouseEvent->scenePos(), MousePressPosition);
                         rectangle->setRect(newRectangle);
                         rectangle->setPen(QColor(0, 0, 255, 255));
                         addItem(rectangle);
            }
            QGraphicsScene::update();
    }

    QGraphicsScene::mousePressEvent(mouseEvent);

}

void MouseWindowScene::setMode(Mode mode)
{
    myMode = mode;
}

void MouseWindowScene::setItemColor(QColor &color)
{
    myItemColor = color;
    if(isItemChange())
        {
        QGraphicsRectItem *item = qgraphicsitem_cast<QGraphicsRectItem *>(selectedItems().first());
       item->setBrush(myItemColor);
   }
}

bool MouseWindowScene::isItemChange()
{
       if(!selectedItems().empty())
            return true;

    return false;
}

MouseWindowScene::~MouseWindowScene()
    {
    }

