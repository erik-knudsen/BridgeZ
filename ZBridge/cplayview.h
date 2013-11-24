#ifndef CPLAYVIEW_H
#define CPLAYVIEW_H

#include <QGraphicsView>

#include "defines.h"

class QGraphicsScene;
class CCenterCards;
class CLRCards;
class CTBCards;
class CMidInfo;
class CTopInfo;
class CBottomInfo;
class CBidDialog;
class CTopInfoAuction;
class CTopInfoPlay;
class CMidInfoAuction;
class CMidInfoPlay;
class CBottomInfoAuction;
class CBottomInfoPlay;
class CBottomInfoButton;

class CPlayView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit CPlayView(QWidget *parent = 0);
    ~CPlayView();

public slots:

public:
    void setParams(Seat bottomSeat, int cardBack);
    void resetView();
    void setInfoAuction(QString board, Team team, Seat dealer);
    void showInfoAuction(bool show);
    void setInfoPlay(QString board, Team team, Seat dealer, Seat declarer, Bids contract, Bids dbl);
    void showInfoPlay(bool show);
    void showNSTricks(int tricks);
    void showEWTricks(int tricks);
    void showInfoAuctionButton(bool show, int id = 0);
    void showInfoPlayButton(bool show, int id = 0);
    void setTrumpSuit(Suit trumpSuit);
    void setAndShowAllCards(bool hasWest, bool showWest, int *westCards, bool hasNorth, bool showNorth, int *northCards,
                            bool hasEast, bool showEast, int *eastCards, bool hasSouth, bool showSouth, int *southCards);
    void setAndShowCards(Seat seat, bool hasSeat, bool showSeat, int *cards);
    void showBidDialog(bool show);
    void showBid(Seat seat, Bids bid);
    void undoBid(int noBid);
    void undoTrick(int wCard, int nCard, int eCard, int sCard);
    void clearCardOnTable(Seat seat);
    void showCardOnTable(Seat seat, int card);
    void clearCardsOnTable();
    void showEWVulnerable();
    void showNSVulnerable();
    void clearVulnerable();
    void showEWNSText();
    void clearEWNSText();
    void showDummy(Seat dummy);
    void showYourTurn(Seat turn);
    void clearYourTurn();
    void clearCard(Seat seat, int cardValue);
    void showClearedCard(Seat seat, int noCard);

    void enableBidder(Seat bidder, Bids lastBid, Bids doubleBid);
    void disableBidder(Seat bidder);
    void enablePlayer(Seat player);
    void disablePlayer(Seat player);
    void enableContinue();
    void disableContinue();

signals:
    void bidValue(Bids bid);
    void playValue(int card);
    void bidBackup();
    void bidHint();
    void bidRestart();
    void bidClose();
    void buttonClicked(int button);
    void handClicked(Seat seat);
    void bidClicked(Seat seat, Bids bid);

private:
    virtual void customEvent(QEvent *event);
    void createSceneAndWidgetsAndLayout();
    void createChildren();

    QGraphicsScene *scene;
    CCenterCards *centerCards;
    CLRCards *lCards;
    CTBCards *tCards;
    CLRCards *rCards;
    CTBCards *bCards;
    CMidInfo *midInfo;
    CTopInfo *topInfo;
    CBottomInfo *bottomInfo;

    CTopInfoAuction *topInfoAuction;
    CTopInfoPlay *topInfoPlay;
    CMidInfoAuction *midInfoAuction;
    CMidInfoPlay *midInfoPlay;
    CBottomInfoAuction *bottomInfoAuction;
    CBottomInfoPlay *bottomInfoPlay;
    CBottomInfoButton *bottomInfoAuctionButton;
    CBottomInfoButton *bottomInfoPlayButton;

    CBidDialog *m_pBidDlg;
    bool firstBidDialogShow;

    int cardBack;
    Seat posToSeat[4];
    Position seatToPos[4];
    Suit trumpSuit;
};

#endif // CPLAYVIEW_H
