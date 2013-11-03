#ifndef CTOPINFOAUCTION_H
#define CTOPINFOAUCTION_H

#include <QGraphicsWidget>

#include "defines.h"

class QGraphicsSimpleTextItem;

class CTopInfoAuction : public QGraphicsWidget
{
public:
    explicit CTopInfoAuction(QGraphicsWidget *parent = 0);

    void setBoardId(QString board);
    void setVulnerability(Team team);
    void setDealer(Seat seat);

private:
    QGraphicsSimpleTextItem *boardIdLabel;
    QGraphicsSimpleTextItem *vulnerabelLabel;
    QGraphicsSimpleTextItem *dealerLabel;
    QGraphicsSimpleTextItem *boardId;
    QGraphicsSimpleTextItem *vulnerabel;
    QGraphicsSimpleTextItem *dealer;
};

#endif // CTOPINFOAUCTION_H
