#ifndef MOUSEWINDOWSCENE_H
#define MOUSEWINDOWSCENE_H

#include "scene.h"
#include <QGraphicsScene>

    class MouseWindowScene : public QGraphicsScene
    {
        Q_OBJECT
public:
        enum Mode {InsertItem, MoveItem};

        explicit MouseWindowScene( QObject *parent = 0);
         ~MouseWindowScene();
        void mousePressEvent(QGraphicsSceneMouseEvent *);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
        void setItemColor(QColor &);
        bool isItemChange();
        void setMode(Mode);

private:
            Mode myMode;
            QGraphicsRectItem *rectangle;
            QColor myItemColor;
            QPointF MousePressPosition;
};

#endif // MOUSEWINDOWSCENE_H
