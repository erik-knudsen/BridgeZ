#ifndef CMIDINFOPLAY_H
#define CMIDINFOPLAY_H

#include <QGraphicsWidget>

class CMidInfoPlay : public QGraphicsWidget
{
public:
    explicit CMidInfoPlay(QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool pressed;
};

#endif // CMIDINFOPLAY_H
