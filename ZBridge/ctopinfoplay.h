#ifndef CTOPINFOPLAY_H
#define CTOPINFOPLAY_H

#include <QGraphicsWidget>

#include "defines.h"

class CTopInfoPlay : public QGraphicsWidget
{
public:
    explicit CTopInfoPlay(QGraphicsWidget *parent = 0);

    void setBoardId(QString board);
    void setVulnerability(Team team);
    void setDealer(Seat seat);
    void setDeclarer(Seat seat);
    void setContract(Bids bid, Bids dbl);

private:
    QGraphicsSimpleTextItem *boardIdLabel;
    QGraphicsSimpleTextItem *vulnerabelLabel;
    QGraphicsSimpleTextItem *dealerLabel;
    QGraphicsSimpleTextItem *declarerLabel;
    QGraphicsSimpleTextItem *contractLabel;
    QGraphicsSimpleTextItem *vulnerabel;
    QGraphicsSimpleTextItem *boardId;
    QGraphicsSimpleTextItem *dealer;
    QGraphicsSimpleTextItem *declarer;
    QGraphicsPixmapItem *contract;
    QGraphicsPixmapItem *contract_dbl;
};

#endif // CTOPINFOPLAY_H
