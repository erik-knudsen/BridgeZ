#ifndef CBOTTOMINFO_H
#define CBOTTOMINFO_H

#include <QGraphicsWidget>

class CBottomInfo : public QGraphicsWidget
{
public:
    explicit CBottomInfo(QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool pressed;
};

#endif // CBOTTOMINFO_H
