#ifndef CBOTTOMINFOAUCTIONBUTTON_H
#define CBOTTOMINFOAUCTIONBUTTON_H

#include <QGraphicsWidget>

class CBottomInfoButton : public QGraphicsWidget
{
public:
    explicit CBottomInfoButton(int horSize, int verSize, QString text, QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void connectButton(QObject *buttonSignal) { this->buttonSignal = buttonSignal; }
    void setButtonId(int buttonId) { this->buttonId = buttonId; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    int horSize, verSize;
    int buttonId;
    QString text;
    bool pressed;
    QObject *buttonSignal;
};

#endif // CBOTTOMINFOBUTTON_H
