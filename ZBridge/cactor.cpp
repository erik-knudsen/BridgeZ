#include "CTblMngr.h"
#include "cactor.h"

CActor::CActor(CTblMngr *tableManager) :
    QObject(tableManager)
{
    this->tableManager = tableManager;

    connect(this, &CActor::sConnect, tableManager, &CTblMngr::sConnect);
    connect(this, &CActor::sRTNames, tableManager, &CTblMngr::sRTNames);
    connect(this, &CActor::sRSBoard, tableManager, &CTblMngr::sRSBoard);
    connect(this, &CActor::sRDealInfo, tableManager, &CTblMngr::sRDealInfo);
    connect(this, &CActor::sRCards, tableManager, &CTblMngr::sRCards);
    connect(this, &CActor::sRBid, tableManager, &CTblMngr::sRBid);
    connect(this, &CActor::sBid, tableManager, &CTblMngr::sBid);
    connect(this, &CActor::sPlayerPlays, tableManager, &CTblMngr::sPlayerPlays);
    connect(this, &CActor::sReadyForPlayer, tableManager, &CTblMngr::sReadyForPlayer);
    connect(this, &CActor::sReadyForDummy, tableManager, &CTblMngr::sReadyForDummy);
    connect(this, &CActor::sReadyForDummyCards, tableManager, &CTblMngr::sReadyForDummyCards);
}

CActor::~CActor()
{
}
