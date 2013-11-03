#ifndef CCENTERCARDS_H
#define CCENTERCARDS_H

#include <QGraphicsWidget>
#include <QGraphicsSimpleTextItem>

#include "Defines.h"
#include "ccard.h"
#include "csquare.h"

class CCenterCards : public QGraphicsWidget
{
public:
    explicit CCenterCards(QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void connectButton(QObject *buttonSignal) { this->buttonSignal = buttonSignal; }

    void showCard(Position pos, int card);
    void clearCard(Position pos);
    void showVulnerable(Position pos);
    void clearVulnerable(Position pos);
    void showText(Position pos, char ewns);
    void clearText(Position pos);
    void showYourTurn(Position pos);
    void clearYourTurn();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool pressed;

    QObject *buttonSignal;

    CCard lCard, tCard, rCard, bCard;
    CSquare lVulnerable, tVulnerable, rVulnerable, bVulnerable;
    QGraphicsSimpleTextItem lText, tText, rText, bText;
};

#endif // CCENTERCARDS_H
