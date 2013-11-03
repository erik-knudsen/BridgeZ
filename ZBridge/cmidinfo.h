#ifndef CMIDINFO_H
#define CMIDINFO_H

#include <QGraphicsWidget>

class CMidInfo : public QGraphicsWidget
{
public:
    explicit CMidInfo(QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool pressed;
};

#endif // CMIDINFO_H
