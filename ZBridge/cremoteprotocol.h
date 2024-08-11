/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CRemoteProtocol.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Pack/unpack communication messages.

  Platforms: Qt.

*/

/**
 * \file
 * Pack/unpack communication messages (declarations).
 */

#ifndef CREMOTEPROTOCOL_H
#define CREMOTEPROTOCOL_H

#include "ZBridgeException.h"
#include "Defines.h"

MsgType getMessageType(QString line);

/**
 * @brief Base class for all communication messages.
 *
 * It defines common operations for all messages.
 */
class CMsg
{
public:
    CMsg(){ }

private:
    /**
     * @brief Pack message into line.
     */
    virtual void msgToLine() = 0;
    /**
     * @brief Unpack message from line.
     */
    virtual void lineToMsg() = 0;

protected:
    Seat getSeat(QString line);
    Team getTeam(QString line);
    void getCardValues(int cards[13], QString line, int first);
    int getFaceValue(QString line, int next);
    void setFaceValues(int cards[13], QString &line);

public:
    MsgType msgType;
};


class CConnectMsg : public CMsg
{
public:
    CConnectMsg(QString name, Seat seat, int protocol);
    CConnectMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    QString name;
    Seat seat;
    int protocol;

    QString line;
};

class CSeatedMsg : public CMsg
{
public:
    CSeatedMsg(Seat seat, QString teamName);
    CSeatedMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat seat;
    QString teamName;

    QString line;
};

class CRTNamesMsg : public CMsg
{
public:
    CRTNamesMsg(Seat seat);
    CRTNamesMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat seat;

    QString line;
};

class CTeamNamesMsg : public CMsg
{
public:
    CTeamNamesMsg(QString nsTeamName, QString ewTeamName);
    CTeamNamesMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    QString nsTeamName;
    QString ewTeamName;

    QString line;
};

class CRSBoardMsg : public CMsg
{
public:
    CRSBoardMsg(Seat seat);
    CRSBoardMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat seat;

    QString line;
};

class CStartOfBoardMsg : public CMsg
{
public:
    CStartOfBoardMsg();
    CStartOfBoardMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    QString line;
};

class CRDealInfoMsg : public CMsg
{
public:
    CRDealInfoMsg(Seat seat);
    CRDealInfoMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat seat;

    QString line;
};

class CDealInfoMsg : public CMsg
{
public:
    CDealInfoMsg(int boardNumber, Seat dealer, Team vulnerability);
    CDealInfoMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    int boardNumber;
    Seat dealer;
    Team vulnerability;

    QString line;
};

class CRCardsMsg : public CMsg
{
public:
    CRCardsMsg(Seat seat);
    CRCardsMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat seat;

    QString line;
};

class CCardsMsg : public CMsg
{
public:
    CCardsMsg(Seat player, int cards[13]);
    CCardsMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat player;
    int cards[13];

    QString line;
};

class CBidMsg : public CMsg
{
public:
    CBidMsg(Seat bidder, Bids bid);
    CBidMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat bidder;
    Bids bid;

    QString line;
};

class CRBidMsg : public CMsg
{
public:
    CRBidMsg(Seat player, Seat bidder);
    CRBidMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat player;
    Seat bidder;

    QString line;
};

class CPlayerToLeadMsg : public CMsg
{
public:
    CPlayerToLeadMsg(Seat player);
    CPlayerToLeadMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat player;
    QString line;
};

class CDummyToLeadMsg : public CMsg
{
public:
    CDummyToLeadMsg();
    CDummyToLeadMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    QString line;
};

class CPlayerPlaysMsg : public CMsg
{
public:
    CPlayerPlaysMsg(Seat player, int card);
    CPlayerPlaysMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat player;
    int card;

    QString line;
};

class CReadyForPlayerMsg : public CMsg
{
public:
    CReadyForPlayerMsg(Seat seat, Seat player, int trick);
    CReadyForPlayerMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat seat;
    Seat player;
    int trick;

    QString line;
};

class CReadyForDummyMsg : public CMsg
{
public:
    CReadyForDummyMsg(Seat seat, int trick);
    CReadyForDummyMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat seat;
    int trick;

    QString line;
};

class CIllegalCardMsg : public CMsg
{
public:
    CIllegalCardMsg();
    CIllegalCardMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    QString line;
};

class CIllegalBidMsg : public CMsg
{
public:
    CIllegalBidMsg();
    CIllegalBidMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    QString line;
};

class CReadyForDummyCardsMsg : public CMsg
{
public:
    CReadyForDummyCardsMsg(Seat seat);
    CReadyForDummyCardsMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat seat;

    QString line;
};

class CDummyCardsMsg : public CMsg
{
public:
    CDummyCardsMsg(int cards[13]);
    CDummyCardsMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    int cards[13];

    QString line;
};

class CEndOfSessionMsg : public CMsg
{
public:
    CEndOfSessionMsg();
    CEndOfSessionMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    QString line;
};

class CUndoBidMsg : public CMsg
{
public:
    CUndoBidMsg();
    CUndoBidMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    QString line;
};

class CUndoTrickMsg : public CMsg
{
public:
    CUndoTrickMsg();
    CUndoTrickMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    QString line;
};


class CReBidMsg : public CMsg
{
public:
    CReBidMsg();
    CReBidMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    QString line;
};


class CRePlayMsg : public CMsg
{
public:
    CRePlayMsg();
    CRePlayMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    QString line;
};


class CAttemptSynchronizeMsg : public CMsg
{
public:
    CAttemptSynchronizeMsg(Seat seat);
    CAttemptSynchronizeMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat seat;

    QString line;
};


class CConfirmSynchronizeMsg : public CMsg
{
public:
    CConfirmSynchronizeMsg(Seat seat);
    CConfirmSynchronizeMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat seat;

    QString line;
};


class CAllSynchronizedMsg : public CMsg
{
public:
    CAllSynchronizedMsg(Seat seat);
    CAllSynchronizedMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    Seat seat;

    QString line;
};


class COriginalPBNStartMsg : public CMsg
{
public:
    COriginalPBNStartMsg(ScoringMethod scoringMethod);
    COriginalPBNStartMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    ScoringMethod scoringMethod;

    QString line;
};


class CPlayedPBNStartMsg : public CMsg
{
public:
    CPlayedPBNStartMsg();
    CPlayedPBNStartMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    QString line;
};


class CEscapePBNMsg : public CMsg
{
public:
    CEscapePBNMsg();
    CEscapePBNMsg(QString line);

private:
    void msgToLine();
    void lineToMsg();

public:
    QString line;
};


#endif // CREMOTEPROTOCOL_H
