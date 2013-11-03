
#include "chand.h"

#ifndef CRLHAND_H
#define CRLHAND_H

class CRLHand : public CHand
{
public:
    CRLHand();

    void prepareToDrawFaces(QGraphicsWidget *widget);
    void prepareToDrawBacks(int cardBack, QGraphicsWidget *widget);
};

#endif // CRLHAND_H
