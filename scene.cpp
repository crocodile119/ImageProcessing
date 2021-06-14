#include <QtGui>
#include <QtCore>
#include <QApplication>
#include <QImage>
#include <QRubberBand>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneMoveEvent>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QRectF>
#include <QPolygonF>
#include <QDebug>
#include "scene.h"

Scene::Scene(QObject *parent)
    : QGraphicsScene(parent)
{
rubberband = 0;
polygonNumber=0;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    //allow scene to process its events first.
    QGraphicsScene::mousePressEvent(event);
    if((rubberBandDragFlag==true) && (polygonNumber==0))
    {
    //if no selection open up a rubberband.
    if(selectedItems().isEmpty()) {
        QGraphicsView *view = qobject_cast<QGraphicsView*>(event->widget()->parent());
        Q_ASSERT(view);
        rubberband = new QRubberBand(QRubberBand::Rectangle, view->viewport());
        QPoint p(view->mapFromScene(event->scenePos()));
        qDebug() << "Origine: " << p;
        rubberband->setGeometry(QRect(p, p));
        rubberband->show();
        origin = view->mapFromScene(event->scenePos());
        qDebug() << "Origine da origin: " << origin;
        }
    }
    else {
    QGraphicsScene::mousePressEvent(event);
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

        if(rubberband && rubberBandDragFlag) {
        QGraphicsView *view = qobject_cast<QGraphicsView*>(rubberband->parent()->parent());
        Q_ASSERT(view);
        origin = view->mapFromScene(event->buttonDownScenePos(Qt::LeftButton));
        qDebug() << "Origine da origin in mouseMoveEvent: " << origin;
        QRectF rect(origin, view->mapFromScene(event->scenePos()));
        qDebug() << "QRectF in mouseMoveEvent: " << rect;
        rubberband->setGeometry(rect.toRect().normalized());
        polygon = view->mapToScene(rect.toRect().normalized());
        QPainterPath path;
        path.addPolygon(polygon);
        setSelectionArea(path);
            ++polygonNumber;
    }
else {
QGraphicsScene::mouseMoveEvent(event);
}
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
        if(rubberband && polygonNumber!=0 && rubberBandDragFlag) {
            //QRect rectangle =rubberband->rect();
                item =new MyGraphicsRectItem(polygon);

                item->setBrush(Qt::CrossPattern);
                item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
                | QGraphicsItem::ItemIsFocusable);
                addItem(item);
                delete rubberband;
                rubberband = 0;
                emit selected();
                }
else {
QGraphicsScene::mouseReleaseEvent(event);
}
}

void Scene::rubberBandDragEnabled(bool _flag)
{
    rubberBandDragFlag=_flag;
}

QRectF Scene::getRectangleCoordinates()
{
    QPointF itemPos = item->pos();
    QPointF itemBoundingPos = item->boundingRect().topLeft();
    QSizeF itemBoundingDimensions = item->boundingRect().size();
    QPointF newPos = itemPos + itemBoundingPos;
    qDebug() << "posizione vertice in alto a sinistra del rettangolo: " << itemPos;
    qDebug() << "Dimensioni del rettangolo: " << itemBoundingDimensions;
    QRectF newRect = QRectF(newPos, itemBoundingDimensions);
             return newRect;
}

void Scene::reset()
{
    polygonNumber=0;
    rubberband = 0;
}

void Scene::deleteSelection()
{
    delete item;
    item=0;
}

bool Scene::checkSelection()
{
    if(item==0)
        return false;
    else
        return true;
}

Scene::~Scene()
{

}
