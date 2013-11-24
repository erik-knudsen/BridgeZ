#ifndef CLRCARDS_H
#define CLRCARDS_H

#include <QGraphicsWidget>

#include "ccards.h"
#include "Defines.h"

class CLRCards : public CCards
{
public:
    explicit CLRCards(Position pos, QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;

    void prepareToDrawFaces(QGraphicsWidget *widget);
    void prepareToDrawBacks(QGraphicsWidget *widget);
};

#endif // CLRCARDS_H
