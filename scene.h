#ifndef WINDOWSELECTIONSCENE_H
#define WINDOWSELECTIONSCENE_H

#include <QWidget>
#include <QRubberBand>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "mygraphicsrectitem.h"


class Scene : public QGraphicsScene
{
            Q_OBJECT
    public:
        explicit Scene( QObject *parent = 0);
        ~Scene();
        void rubberBandDragEnabled(bool);
        QRectF getRectangleCoordinates();
        void deleteSelection();
        void reset();
        bool checkSelection();

    signals:
        void selected();

    protected:

        void mousePressEvent(QGraphicsSceneMouseEvent * event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    private slots:


    private:
        QPointF origin;
        QRubberBand *rubberband;
        QPainterPath path;
        QPolygonF polygon;
        int polygonNumber;
        bool rubberBandDragFlag;
        MyGraphicsRectItem *item;
};

#endif // WINDOWSELECTIONSCENE_H
