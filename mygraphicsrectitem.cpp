#include "mygraphicsrectitem.h"


MyGraphicsRectItem::MyGraphicsRectItem(const QPolygonF &rect):QGraphicsPolygonItem(rect)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

QVariant MyGraphicsRectItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
    // value is the new position.
        QPointF newPos = value.toPointF();
        QRectF rect = scene()->sceneRect();
        if (!rect.contains(newPos)) {
        // Keep the item inside the scene rect.
        newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
        newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
        return newPos;
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

MyGraphicsRectItem::~MyGraphicsRectItem()
{
}
