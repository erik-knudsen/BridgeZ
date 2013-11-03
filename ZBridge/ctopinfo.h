#ifndef CTOPINFO_H
#define CTOPINFO_H

#include <QGraphicsWidget>

class CTopInfo : public QGraphicsWidget
{
public:
    explicit CTopInfo(QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool pressed;
};

#endif // CTOPINFO_H
