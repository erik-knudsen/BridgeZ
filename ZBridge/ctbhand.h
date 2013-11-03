
#include "chand.h"

#ifndef CTBHAND_H
#define CTBHAND_H

class CTBHand : public CHand
{
public:
    CTBHand();

    void prepareToDrawFaces(QGraphicsWidget *widget);
    void prepareToDrawBacks(int cardBack, QGraphicsWidget *widget);
};

#endif // CTBHAND_H
