#ifndef MYGRAPHICSRECTITEM_H
#define MYGRAPHICSRECTITEM_H

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>

class MyGraphicsRectItem : public QGraphicsPolygonItem
{
public:
      explicit MyGraphicsRectItem(const QPolygonF &);
      ~MyGraphicsRectItem();
      QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // MYGRAPHICSRECTITEM_H
