#ifndef CTBCARDS_H
#define CTBCARDS_H

#include <QGraphicsWidget>

#include "ccards.h"
#include "Defines.h"

class CTBCards : public CCards
{
public:
    explicit CTBCards(Position pos, QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;

    void prepareToDrawFaces(QGraphicsWidget *widget);
    void prepareToDrawBacks(QGraphicsWidget *widget);
};

#endif // CTBCARDS_H
