/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CRemoteProtocol.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Pack/unpack communication messages.

  Platforms: Qt.

*/

/**
 * \file
 * Pack/unpack communication messages (definitions).
 */

#include <QVector>

#include "cremoteprotocol.h"

//Indexed with enum Seat values.
const QString SEAT_NAMES_NET[4] =
{
    "West",
    "North",
    "East",
    "South"
};

//Indexed with enum Team values.
const QString VULNERABILITY_NAMES_NET[4] =
{
    "Neither vulnerable",
    "N/S vulnerable",
    "E/W vulnerable",
    "Both vulnerable"
};

enum BidType_Net { PASSES_NET = 0, DOUBLES_NET = 1, REDOUBLES_NET = 2, BIDS_NET};
const QString BIDTYPE_NET[4] =
{
    "passes",
    "doubles",
    "redoubles",
    "bids"
};

//Indexed with enum Suit values.
const QString SUITS_NET[5] =
{
    "C",
    "D",
    "H",
    "S",
    "NT"
};

//Indexed with enum ScoringMethod values.
const QString SCORINGMETHODS_NET[6] =
{
    "IMP",
    "MP",
    "RUBBER",
    "PRACTICE",
    "NOSCORE",
    "FORSCORE"
};


/**
 * @brief Get the message type from a line received.
 * @param line The line received.
 * @return The type.
 */
MsgType getMessageType(QString line) throw(NetProtocolException)
{
    if (line.contains("plays", Qt::CaseInsensitive))
        return PLAYERPLAYS_MSG;
    if ((line.contains("ready for", Qt::CaseInsensitive)) &&
             (line.contains("card to trick", Qt::CaseInsensitive)) &&
             !(line.contains("dummy", Qt::CaseInsensitive)))
        return READYFORPLAYER_MSG;
    if (line.contains("ready for dummy's card to trick", Qt::CaseInsensitive))
        return READYFORDUMMY_MSG;
    if ((line.contains("to lead", Qt::CaseInsensitive)) &&
             !(line.contains("dummy", Qt::CaseInsensitive)))
        return PLAYERTOLEAD_MSG;
    if (line.contains("Dummy to lead", Qt::CaseInsensitive))
        return DUMMYTOLEAD_MSG;
    if ((line.contains("passes", Qt::CaseInsensitive)) ||
             (line.contains("doubles", Qt::CaseInsensitive)) ||
             (line.contains("redoubles", Qt::CaseInsensitive)) ||
             (line.contains("bids", Qt::CaseInsensitive)))
        return BID_MSG;
    if ((line.contains("ready for", Qt::CaseInsensitive)) &&
             (line.contains("bid", Qt::CaseInsensitive)))
        return RBID_MSG;
    if (line.contains("ready for dummy", Qt::CaseInsensitive) &&
            !line.contains("card to trick"))
        return READYFORDUMMYCARDS_MSG;
    if (line.contains("Dummy's cards", Qt::CaseInsensitive))
        return DUMMYCARDS_MSG;

    if (line.contains("Connecting", Qt::CaseInsensitive))
        return CONNECT_MSG;
    if (line.contains("seated", Qt::CaseInsensitive))
        return SEATED_MSG;
    if (line.contains("ready for teams", Qt::CaseInsensitive))
        return RTNAMES_MSG;
    if (line.contains("Teams: N/S", Qt::CaseInsensitive))
        return TEAMNAMES_MSG;
    if (line.contains("ready to start", Qt::CaseInsensitive))
        return RSBOARD_MSG;
    if (line.contains("Start of board", Qt::CaseInsensitive))
        return STARTOFBOARD_MSG;
    if (line.contains("ready for deal", Qt::CaseInsensitive))
        return RDEALINFO_MSG;
    if (line.contains("Board number", Qt::CaseInsensitive))
        return DEALINFO_MSG;
    if (line.contains("ready for cards", Qt::CaseInsensitive))
        return RCARDS_MSG;
    if (line.contains("cards : ", Qt::CaseInsensitive))
        return CARDS_MSG;
    if (line.contains("Illegal bid", Qt::CaseInsensitive))
        return ILLEGALBID_MSG;
    if (line.contains("Illegal card", Qt::CaseInsensitive))
        return ILLEGALCARD_MSG;
    if (line.contains("End of session", Qt::CaseInsensitive))
        return ENDOFSESSION_MSG;
    if (line.contains("undo bid", Qt::CaseInsensitive))
        return UNDOBID_MSG;
    if (line.contains("undo trick", Qt::CaseInsensitive))
        return UNDOTRICK_MSG;
    if (line.contains("rebid", Qt::CaseInsensitive))
        return REBID_MSG;
    if (line.contains("replay", Qt::CaseInsensitive))
        return REPLAY_MSG;
    if (line.contains("attempt synchronize", Qt::CaseInsensitive))
        return ATTEMPT_SYNCHRONIZE_MSG;
    if (line.contains("confirm synchronize", Qt::CaseInsensitive))
        return CONFIRM_SYNCHRONIZE_MSG;
    if (line.contains("all synchronized", Qt::CaseInsensitive))
        return ALL_SYNCHRONIZED_MSG;
    if (line.contains("Original PBN Stream Start", Qt::CaseInsensitive))
        return ORIGINAL_PBN_START_MSG;
    if (line.contains("Played PBN Stream Start", Qt::CaseInsensitive))
        return PLAYED_PBN_START_MSG;

    throw NetProtocolException("Net - Unknown message: " + line.toStdString());
}

/**
 *@brief  Get the seat from a line received.
 * @param line The line.
 * @return The seat.
 */
Seat CMsg::getSeat(QString line) throw(NetProtocolException)
{
    if (line.contains(SEAT_NAMES_NET[WEST_SEAT], Qt::CaseInsensitive))
        return WEST_SEAT;
    else if (line.contains(SEAT_NAMES_NET[NORTH_SEAT], Qt::CaseInsensitive))
        return NORTH_SEAT;
    else if (line.contains(SEAT_NAMES_NET[EAST_SEAT], Qt::CaseInsensitive))
        return  EAST_SEAT;
    else if (line.contains(SEAT_NAMES_NET[SOUTH_SEAT], Qt::CaseInsensitive))
        return SOUTH_SEAT;
    else
        throw NetProtocolException("Net - No seat: " + line.toStdString());
}

/**
 * @brief Get the team from a line received.
 * @param line The line.
 * @return The team.
 */
Team CMsg::getTeam(QString line) throw(NetProtocolException)
{
    if (line.contains(VULNERABILITY_NAMES_NET[NEITHER], Qt::CaseInsensitive))
        return NEITHER;
    else if (line.contains(VULNERABILITY_NAMES_NET[NORTH_SOUTH], Qt::CaseInsensitive))
        return NORTH_SOUTH;
    else if (line.contains(VULNERABILITY_NAMES_NET[EAST_WEST], Qt::CaseInsensitive))
        return  EAST_WEST;
    else if (line.contains(VULNERABILITY_NAMES_NET[BOTH], Qt::CaseInsensitive))
        return BOTH;
    else
        throw NetProtocolException("Net - No vulnerability: " + line.toStdString());
}

/**
 * @brief Get the card values from a cards line received.
 * @param line The line.
 */
void CMsg::getCardValues(int cards[], QString line, int first) throw(NetProtocolException)
{
    int i, next, face;

    i = 0;

    next = line.indexOf("S ", first, Qt::CaseInsensitive) + 1;
    while ((face = getFaceValue(line, next)) != -1)
    {
        if (i >= 13)
            throw NetProtocolException("Net - Cards: " + line.toStdString());
        cards[i++] = MAKE_CARD(SPADES, face);
        next += 2;
    }

    next = line.indexOf("H ", first, Qt::CaseInsensitive) + 1;
    while ((face = getFaceValue(line, next)) != -1)
    {
        if (i >= 13)
            throw NetProtocolException("Net - Cards: " + line.toStdString());
        cards[i++] = MAKE_CARD(HEARTS, face);
        next += 2;
    }

    next = line.indexOf("D ", first, Qt::CaseInsensitive) + 1;
    while ((face = getFaceValue(line, next)) != -1)
    {
        if (i >= 13)
            throw NetProtocolException("Net - Cards: " + line.toStdString());
        cards[i++] = MAKE_CARD(DIAMONDS, face);
        next += 2;
    }

    next = line.indexOf("C ", first, Qt::CaseInsensitive) + 1;
    while ((face = getFaceValue(line, next)) != -1)
    {
        if (i >= 13)
            throw NetProtocolException("Net - Cards: " + line.toStdString());
        cards[i++] = MAKE_CARD(CLUBS, face);
        next += 2;
    }

    if (i != 13)
        throw NetProtocolException("Net - Cards: " + line.toStdString());
}

/**
 * @brief Get next face value from a line received.
 * @param line The line.
 * @param next Index into line.
 * @return The face value.
 */
int CMsg::getFaceValue(QString line, int next) throw(NetProtocolException)
{
    const QString CARD = "23456789TJQKA";

    if (line.size() <= next)
        throw NetProtocolException("Net - Cards: " + line.toStdString());

    if ((line.at(next) == '.') || (line.at(next) == '-'))
        return -1;

    if (!(line.size() > next))
        throw NetProtocolException("Net - Cards: " + line.toStdString());

    if ((line.at(next + 1) == '.') || (line.at(next + 1) == '-'))
        return -1;

    if (!CARD.contains(line.at(next + 1)))
        throw NetProtocolException("Net - Cards: " + line.toStdString());

    return CARD.indexOf(line.at(next + 1), 0, Qt::CaseInsensitive);
}

//Set face values in a string to send.
/**
 * @brief Set face values in a string to send.
 * @param cards The card values.
 * @param line The line to send.
 */
void CMsg::setFaceValues(int cards[], QString &line)
{
    int i, j;
    QVector<int> handCardValues;
    const char CARD[] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

    for (int i = 0; i < 13; i++)
        handCardValues.append(cards[i]);

    qSort(handCardValues.begin(), handCardValues.end(), qGreater<int>());

    //Get spades.
    line += " S";
    for (i = 0, j = 0; i < 13; i++)
    {
        if (IS_SPADES(handCardValues.at(i)))
        {
            line += " ";
            line += CARD[CARD_FACE(handCardValues.at(i))];
            j++;
        }
    }
    if ( j == 0)
        line += " -";
    else
        line += '.';

    //Get hearts.
    line += " H";
    for (i = 0, j = 0; i < 13; i++)
    {
        if (IS_HEARTS(handCardValues.at(i)))
        {
            line += " ";
            line += CARD[CARD_FACE(handCardValues.at(i))];
            j++;
        }
    }
    if ( j == 0)
        line += " -";
    else
        line += '.';

    //Get diamonds.
    line += " D";
    for (i = 0, j = 0; i < 13; i++)
    {
        if (IS_DIAMONDS(handCardValues.at(i)))
        {
            line += " ";
            line += CARD[CARD_FACE(handCardValues.at(i))];
            j++;
        }
    }
    if ( j == 0)
        line += " -";
    else
        line += '.';

    //Get clubs.
    line += " C";
    for (i = 0, j = 0; i < 13; i++)
    {
        if (IS_CLUBS(handCardValues.at(i)))
        {
            line += " ";
            line += CARD[CARD_FACE(handCardValues.at(i))];
            j++;
        }
    }
    if ( j == 0)
        line += " -";
    else
        line += '.';
}


//Unpacking and packing of the different communication messages.
//--------------------------------------------------------------------

CConnectMsg::CConnectMsg(QString name, Seat seat, int protocol)
{
    msgType = CONNECT_MSG;

    this->name = name;
    this->seat = seat;
    this->protocol = protocol;

    msgToLine();
}

CConnectMsg::CConnectMsg(QString line) throw(NetProtocolException)
{
    msgType = CONNECT_MSG;

    this->line = line;

    lineToMsg();
}

void CConnectMsg::msgToLine()
{
    line = QString("Connecting \"%1\" as %2 using protocol version %3\r\n").arg(name).arg(SEAT_NAMES_NET[seat]).arg(protocol);
}

void CConnectMsg::lineToMsg() throw (NetProtocolException)
{ 
    if (!line.contains("Connecting", Qt::CaseInsensitive) ||
        !line.contains("as", Qt::CaseInsensitive) ||
        !line.contains("using protocol version", Qt::CaseInsensitive) ||
        !(line.count(QChar('"')) == 2))
        throw NetProtocolException("Net - Connect: " + line.toStdString());

    seat = getSeat(line);

    protocol = 0;
    int i = line.size() - 1;
    while (line[i].isDigit()) i--;
    if (i != (line.size() - 1))
        protocol = line.right(line.size() - 1 - i).toInt();

    name = line.section('"', 1, 1);
}


CSeatedMsg::CSeatedMsg(Seat seat, QString teamName)
{
    msgType = SEATED_MSG;

    this->seat = seat;
    this->teamName = teamName;

    msgToLine();
}

CSeatedMsg::CSeatedMsg(QString line) throw (NetProtocolException)
{
    msgType = SEATED_MSG;

    this->line = line;

    lineToMsg();
}

void CSeatedMsg::msgToLine()
{
    line = QString("%1 \"%2\" seated\r\n").arg(SEAT_NAMES_NET[seat]).arg(teamName);
}

void CSeatedMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("seated", Qt::CaseInsensitive) ||
        !(line.count(QChar('"')) == 2))
        throw NetProtocolException("Net - Seated: " + line.toStdString());

    seat = getSeat(line);

    teamName = line.section('"', 1, 1);
}


CRTNamesMsg::CRTNamesMsg(Seat seat)
{
    msgType = RTNAMES_MSG;

    this->seat = seat;

    msgToLine();
}

CRTNamesMsg::CRTNamesMsg(QString line) throw (NetProtocolException)
{
    msgType = RTNAMES_MSG;

    this->line = line;

    lineToMsg();
}

void CRTNamesMsg::msgToLine()
{
    line = QString("%1 ready for teams\r\n").arg(SEAT_NAMES_NET[seat]);
}

void CRTNamesMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("ready for teams", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Ready for teams: " + line.toStdString());

    seat = getSeat(line);
}


CTeamNamesMsg::CTeamNamesMsg(QString nsTeamName, QString ewTeamName)
{
    msgType = TEAMNAMES_MSG;

    this->nsTeamName = nsTeamName;
    this->ewTeamName = ewTeamName;

    msgToLine();
}

CTeamNamesMsg::CTeamNamesMsg(QString line) throw (NetProtocolException)
{
    msgType = TEAMNAMES_MSG;

    this->line = line;

    lineToMsg();
}

void CTeamNamesMsg::msgToLine()
{
    line = QString("Teams: N/S: \"%1\". E/W: \"%2\"\r\n").arg(nsTeamName).arg(ewTeamName);
}

void CTeamNamesMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("Teams: N/S: ", Qt::CaseInsensitive) ||
        !line.contains(". E/W: ", Qt::CaseInsensitive) ||
        !(line.count(QChar('"')) == 4))
        throw NetProtocolException("Net - Teamnames: " + line.toStdString());

    nsTeamName = line.section('"', 1, 1);
    ewTeamName = line.section('"', 3, 3);
}


CRSBoardMsg::CRSBoardMsg(Seat seat)
{
    msgType = RSBOARD_MSG;

    this->seat = seat;

    msgToLine();
}

CRSBoardMsg::CRSBoardMsg(QString line) throw (NetProtocolException)
{
    msgType = RSBOARD_MSG;

    this->line = line;

    lineToMsg();
}

void CRSBoardMsg::msgToLine()
{
    line = QString("%1 ready to start\r\n").arg(SEAT_NAMES_NET[seat]);
}

void CRSBoardMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("ready to start", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Ready to start: " + line.toStdString());

    seat = getSeat(line);
}


CStartOfBoardMsg::CStartOfBoardMsg()
{
    msgType = STARTOFBOARD_MSG;

    msgToLine();
}

CStartOfBoardMsg::CStartOfBoardMsg(QString line) throw (NetProtocolException)
{
    msgType = STARTOFBOARD_MSG;

    this->line = line;

    lineToMsg();
}

void CStartOfBoardMsg::msgToLine()
{
    line = QString("Start of board\r\n");
}

void CStartOfBoardMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("Start of board", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Start of board: " + line.toStdString());
}


CRDealInfoMsg::CRDealInfoMsg(Seat seat)
{
    msgType = RDEALINFO_MSG;

    this->seat = seat;

    msgToLine();
}

CRDealInfoMsg::CRDealInfoMsg(QString line) throw (NetProtocolException)
{
    msgType = RDEALINFO_MSG;

    this->line = line;

    lineToMsg();
}

void CRDealInfoMsg::msgToLine()
{
    line = QString("%1 ready for deal\r\n").arg(SEAT_NAMES_NET[seat]);
}

void CRDealInfoMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("ready for deal", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Ready for deal: " + line.toStdString());

    seat = getSeat(line);
}


CDealInfoMsg::CDealInfoMsg(int boardNumber, Seat dealer, Team vulnerability)
{
    msgType = DEALINFO_MSG;

    this ->boardNumber = boardNumber;
    this->dealer = dealer;
    this->vulnerability = vulnerability;

    msgToLine();
}

CDealInfoMsg::CDealInfoMsg(QString line) throw (NetProtocolException)
{
    msgType = DEALINFO_MSG;

    this->line = line;

    lineToMsg();
}

void CDealInfoMsg::msgToLine()
{
    line = QString("Board number %1. Dealer %2. %3\r\n").arg(boardNumber).arg(SEAT_NAMES_NET[dealer]).arg(VULNERABILITY_NAMES_NET[vulnerability]);
}

void CDealInfoMsg::lineToMsg() throw (NetProtocolException)
{
    int m;

    if (!line.contains("Board number", Qt::CaseInsensitive) ||
        !line.contains("Dealer", Qt::CaseInsensitive) ||
        !(line.count(QChar('.')) == 2) ||
        ((m = line.indexOf('.')) <= 13))
        throw NetProtocolException("Net - Deal info: " + line.toStdString());

    dealer = getSeat(line);
    vulnerability = getTeam(line);
    boardNumber = line.mid(13, m - 13).toInt();
}


CRCardsMsg::CRCardsMsg(Seat seat)
{
    msgType = RCARDS_MSG;

    this->seat = seat;

    msgToLine();
}

CRCardsMsg::CRCardsMsg(QString line)
{
    msgType = RCARDS_MSG;

    this->line = line;

    lineToMsg();
}

void CRCardsMsg::msgToLine()
{
    line = QString("%1 ready for cards\r\n").arg(SEAT_NAMES_NET[seat]);
}

void CRCardsMsg::lineToMsg()
{
    if (!line.contains("ready for cards", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Ready for cards: " + line.toStdString());

    seat = getSeat(line);
}


CCardsMsg::CCardsMsg(Seat player, int cards[13])
{
    msgType = CARDS_MSG;

    this->player = player;
    for (int i = 0; i < 13; i++)
        this->cards[i] = cards[i];

    msgToLine();
}

CCardsMsg::CCardsMsg(QString line) throw (NetProtocolException)
{
    msgType = CARDS_MSG;

    this->line = line;

    lineToMsg();
}

void CCardsMsg::msgToLine()
{
    line = QString("%1's cards :").arg(SEAT_NAMES_NET[player]);

    setFaceValues(cards, line);

    line += "\r\n";
}

void CCardsMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("cards", Qt::CaseInsensitive) ||
        !line.contains("S ", Qt::CaseInsensitive) ||
        !line.contains("H ", Qt::CaseInsensitive) ||
        !line.contains("D ", Qt::CaseInsensitive) ||
        !line.contains("C ", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Cards: " + line.toStdString());

    player = getSeat(line);

    getCardValues(cards, line, line.indexOf("cards", 0, Qt::CaseInsensitive) + 5);
}


CBidMsg::CBidMsg(Seat bidder, Bids bid)
{
    msgType = BID_MSG;

    this->bidder = bidder;
    this->bid = bid;

    msgToLine();
}

CBidMsg::CBidMsg(QString line) throw (NetProtocolException)
{
    msgType = BID_MSG;

    this->line = line;

    lineToMsg();
}

void CBidMsg::msgToLine()
{
    line = QString("%1 ").arg(SEAT_NAMES_NET[bidder]);

    if (bid == BID_PASS)
        line += BIDTYPE_NET[PASSES_NET] + "\r\n";
    else if (bid == BID_DOUBLE)
        line += BIDTYPE_NET[DOUBLES_NET] + "\r\n";
    else if (bid == BID_REDOUBLE)
        line += BIDTYPE_NET[REDOUBLES_NET] + "\r\n";
    else
        line += QString("%1 %2%3\r\n").arg(BIDTYPE_NET[BIDS_NET]).
                arg(QString().setNum((bid - 1) / 5 + 1)).arg(SUITS_NET[(bid - 1) % 5]);
}

void CBidMsg::lineToMsg() throw (NetProtocolException)
{
    if (!(line.contains(BIDTYPE_NET[PASSES_NET], Qt::CaseInsensitive) ||
          line.contains(BIDTYPE_NET[DOUBLES_NET], Qt::CaseInsensitive) ||
          line.contains(BIDTYPE_NET[REDOUBLES_NET], Qt::CaseInsensitive) ||
          line.contains(BIDTYPE_NET[BIDS_NET], Qt::CaseInsensitive)))
        throw NetProtocolException("Net - Bids: " + line.toStdString());

    bidder = getSeat(line);

    if (line.contains(BIDTYPE_NET[PASSES_NET], Qt::CaseInsensitive))
        bid = BID_PASS;
    else if (line.contains(BIDTYPE_NET[DOUBLES_NET], Qt::CaseInsensitive))
        bid = BID_DOUBLE;
    else if (line.contains(BIDTYPE_NET[REDOUBLES_NET], Qt::CaseInsensitive))
        bid = BID_REDOUBLE;
    else
    {
        Suit j;

        int i = line.indexOf(BIDTYPE_NET[BIDS_NET], 0, Qt::CaseInsensitive) + BIDTYPE_NET[BIDS_NET].size() + 1;
        if ( i >= line.size())
            throw NetProtocolException("Net - Bids: " + line.toStdString());
        int level = line.at(i).toLatin1() - 48;
        if ((level < 1) || (level > 7))
            throw NetProtocolException("Net - Bids: " + line.toStdString());

        if (QString(line.mid(i)).contains(SUITS_NET[SPADES], Qt::CaseInsensitive))
            j = SPADES;
        else if (QString(line.mid(i)).contains(SUITS_NET[HEARTS], Qt::CaseInsensitive))
            j = HEARTS;
        else if (QString(line.mid(i)).contains(SUITS_NET[DIAMONDS], Qt::CaseInsensitive))
            j = DIAMONDS;
        else if (QString(line.mid(i)).contains(SUITS_NET[CLUBS], Qt::CaseInsensitive))
            j = CLUBS;
        else if (QString(line.mid(i)).contains(SUITS_NET[NOTRUMP], Qt::CaseInsensitive))
            j = NOTRUMP;
        else
            throw NetProtocolException("Net - Bids: " + line.toStdString());

        bid = (Bids)((level - 1) * 5 + j + 1);
    }

}


CRBidMsg::CRBidMsg(Seat player, Seat bidder)
{
    msgType = RBID_MSG;

    this->player = player;
    this->bidder = bidder;

    msgToLine();
}

CRBidMsg::CRBidMsg(QString line) throw (NetProtocolException)
{
    msgType = RBID_MSG;

    this->line = line;

    lineToMsg();
}

void CRBidMsg::msgToLine()
{
    line = QString("%1 ready for %2's bid\r\n").arg(SEAT_NAMES_NET[player]).arg(SEAT_NAMES_NET[bidder]);
}

void CRBidMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("ready for", Qt::CaseInsensitive) ||
        !line.contains("bid", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Ready for bids: " + line.toStdString());

    player = getSeat(line.mid(0, 7));
    bidder = getSeat(QString(line.mid(10)));
}


CPlayerToLeadMsg::CPlayerToLeadMsg(Seat player)
{
    msgType = PLAYERTOLEAD_MSG;

    this->player = player;

    msgToLine();
}

CPlayerToLeadMsg::CPlayerToLeadMsg(QString line) throw (NetProtocolException)
{
    msgType = PLAYERTOLEAD_MSG;

    this->line = line;

    lineToMsg();
}

void CPlayerToLeadMsg::msgToLine()
{
    line = QString("%1 to lead\r\n").arg(SEAT_NAMES_NET[player]);
}

void CPlayerToLeadMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("to lead", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Player to lead: " + line.toStdString());

    player = getSeat(line);
}


CDummyToLeadMsg::CDummyToLeadMsg()
{
    msgType = DUMMYTOLEAD_MSG;

    msgToLine();
}

CDummyToLeadMsg::CDummyToLeadMsg(QString line)
{
    msgType = DUMMYTOLEAD_MSG;

    this->line = line;

    lineToMsg();
}

void CDummyToLeadMsg::msgToLine()
{
    line = QString("Dummy to lead\r\n");
}

void CDummyToLeadMsg::lineToMsg()
{
    if (!line.contains("Dummy to lead", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Dummy to lead: " + line.toStdString());
}


CPlayerPlaysMsg::CPlayerPlaysMsg(Seat player, int card)
{
    msgType = PLAYERPLAYS_MSG;

    this->player = player;
    this->card = card;

    msgToLine();
}

CPlayerPlaysMsg::CPlayerPlaysMsg(QString line) throw (NetProtocolException)
{
    msgType = PLAYERPLAYS_MSG;

    this->line = line;

    lineToMsg();
}

void CPlayerPlaysMsg::msgToLine()
{
    const QString CARD = "23456789TJQKA";

    Suit suit = CARD_SUIT(card);
    int face = CARD_FACE(card);

    line = QString("%1 plays %2%3\r\n").arg(SEAT_NAMES_NET[player]).arg(CARD[face]).arg(SUITS_NET[suit]);
}

void CPlayerPlaysMsg::lineToMsg() throw (NetProtocolException)
{
    const QString CARD = "23456789TJQKA";

    if (!line.contains("plays", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Player plays: " + line.toStdString());

    player = getSeat(line);

    int i = line.indexOf("plays", 0, Qt::CaseInsensitive) + 5;
    if ((i + 2) >= line.size())
        throw NetProtocolException("Net - Player plays: " + line.toStdString());

    if (!CARD.contains(line.at(i + 1), Qt::CaseInsensitive))
        throw NetProtocolException("Net - Player plays: " + line.toStdString());

    int face = CARD.indexOf(line.at(i + 1), 0 , Qt::CaseInsensitive);

    Suit suit;
    if (line.at(i + 2) == SUITS_NET[SPADES])
        suit = SPADES;
    else if (line.at(i + 2) == SUITS_NET[HEARTS])
        suit = HEARTS;
    else if (line.at(i + 2) == SUITS_NET[DIAMONDS])
        suit = DIAMONDS;
    else if (line.at(i + 2) == SUITS_NET[CLUBS])
        suit = CLUBS;
    else
        throw NetProtocolException("Net - Player plays: " + line.toStdString());

    card = MAKE_CARD(suit, face);
}


CReadyForPlayerMsg::CReadyForPlayerMsg(Seat seat, Seat player, int trick)
{
    msgType = READYFORPLAYER_MSG;

    this->seat = seat;
    this->player = player;
    this->trick = trick;

    msgToLine();
}

CReadyForPlayerMsg::CReadyForPlayerMsg(QString line) throw (NetProtocolException)
{
    msgType = READYFORPLAYER_MSG;

    this->line = line;

    lineToMsg();
}

void CReadyForPlayerMsg::msgToLine()
{
    line = QString("%1 ready for %2's card to trick %3\r\n").arg(SEAT_NAMES_NET[seat]).arg(SEAT_NAMES_NET[player]).arg(QString().setNum(trick));
}

void CReadyForPlayerMsg::lineToMsg() throw (NetProtocolException)
{
    bool ok;

    if (!line.contains("ready for", Qt::CaseInsensitive) ||
        !line.contains("card to trick", Qt::CaseInsensitive) )
        throw NetProtocolException("Net - Ready for player: " + line.toStdString());

    seat = getSeat(line.mid(0, 7));
    player = getSeat(QString(line.mid(10)));

    int i = line.indexOf("card to trick", 0, Qt::CaseInsensitive) + 13;
    trick = line.mid(i).toInt(&ok);
    if (!ok)
        throw NetProtocolException("Net - Ready for player: " + line.toStdString());
}


CReadyForDummyMsg::CReadyForDummyMsg(Seat seat, int trick)
{
    msgType = READYFORDUMMY_MSG;

    this->seat = seat;
    this->trick = trick;

    msgToLine();
}

CReadyForDummyMsg::CReadyForDummyMsg(QString line) throw (NetProtocolException)
{
    msgType = READYFORDUMMY_MSG;

    this->line = line;

    lineToMsg();
}

void CReadyForDummyMsg::msgToLine()
{
    line = QString("%1 ready for dummy's card to trick %2\r\n").arg(SEAT_NAMES_NET[seat]).arg(QString().setNum(trick));
}

void CReadyForDummyMsg::lineToMsg() throw (NetProtocolException)
{
    bool ok;

    if (!line.contains("ready for dummy's card to trick", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Ready for dummy: " + line.toStdString());

    seat = getSeat(line);

    int i = line.indexOf("card to trick", 0, Qt::CaseInsensitive) + 13;
    trick = line.mid(i).toInt(&ok);
    if (!ok)
        throw NetProtocolException("Net - Ready for player: " + line.toStdString());
}


CIllegalCardMsg::CIllegalCardMsg()
{
    msgType = ILLEGALCARD_MSG;

    msgToLine();
}

CIllegalCardMsg::CIllegalCardMsg(QString line)
{
    msgType = ILLEGALCARD_MSG;

    this->line = line;

    lineToMsg();
}

void CIllegalCardMsg::msgToLine()
{
    line = QString("Illegal card\r\n");
}

void CIllegalCardMsg::lineToMsg()
{
    if (!line.contains("Illegal card", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Illegal card: " + line.toStdString());
}


CIllegalBidMsg::CIllegalBidMsg()
{
    msgType = ILLEGALBID_MSG;

    msgToLine();
}

CIllegalBidMsg::CIllegalBidMsg(QString line)
{
    msgType = ILLEGALBID_MSG;

    this->line = line;

    lineToMsg();
}

void CIllegalBidMsg::msgToLine()
{
    line = QString("Illegal bid\r\n");
}

void CIllegalBidMsg::lineToMsg()
{
    if (!line.contains("Illegal bid", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Illegal bid: " + line.toStdString());
}


CReadyForDummyCardsMsg::CReadyForDummyCardsMsg(Seat seat)
{
    msgType = READYFORDUMMYCARDS_MSG;

    this->seat = seat;

    msgToLine();
}

CReadyForDummyCardsMsg::CReadyForDummyCardsMsg(QString line) throw (NetProtocolException)
{
    msgType = READYFORDUMMYCARDS_MSG;

    this->line = line;

    lineToMsg();
}

void CReadyForDummyCardsMsg::msgToLine()
{
    line = QString("%1 ready for dummy\r\n").arg(SEAT_NAMES_NET[seat]);
}

void CReadyForDummyCardsMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("ready for dummy", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Ready for dummy: " + line.toStdString());

    seat = getSeat(line);
}


CDummyCardsMsg::CDummyCardsMsg(int cards[13])
{
    msgType = DUMMYCARDS_MSG;

    for (int i = 0; i < 13; i++)
        this->cards[i] = cards[i];

    msgToLine();
}

CDummyCardsMsg::CDummyCardsMsg(QString line) throw (NetProtocolException)
{
    msgType = DUMMYCARDS_MSG;

    this->line = line;

    lineToMsg();
}

void CDummyCardsMsg::msgToLine()
{
    line = QString("Dummy's cards : ");

    setFaceValues(cards, line);

    line += "\r\n";
}

void CDummyCardsMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("cards", Qt::CaseInsensitive) &&
        !line.contains("S ", Qt::CaseInsensitive) &&
        !line.contains("H ", Qt::CaseInsensitive) &&
        !line.contains("D ", Qt::CaseInsensitive) &&
        !line.contains("C ", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Dummy's cards: " + line.toStdString());

    getCardValues(cards, line, line.indexOf("cards", 0, Qt::CaseInsensitive) + 5);
}


CEndOfSessionMsg::CEndOfSessionMsg()
{
    msgType = ENDOFSESSION_MSG;

    msgToLine();
}

CEndOfSessionMsg::CEndOfSessionMsg(QString line)
{
    msgType = ENDOFSESSION_MSG;

    this->line = line;

    lineToMsg();
}

void CEndOfSessionMsg::msgToLine()
{
    line = QString("End of session\r\n");
}

void CEndOfSessionMsg::lineToMsg()
{
    if (!line.contains("End of session", Qt::CaseInsensitive))
        throw NetProtocolException("Net - End of session: " + line.toStdString());
}

CUndoBidMsg::CUndoBidMsg()
{
    msgType = UNDOBID_MSG;

    msgToLine();
}

CUndoBidMsg::CUndoBidMsg(QString line) throw (NetProtocolException)
{
    msgType = UNDOBID_MSG;

    this->line = line;

    lineToMsg();
}

void CUndoBidMsg::msgToLine()
{
    line = QString("undo bid\r\n");
}

void CUndoBidMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("undo bid", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Undo bid: " + line.toStdString());
}

CUndoTrickMsg::CUndoTrickMsg()
{
    msgType = UNDOTRICK_MSG;

    msgToLine();
}

CUndoTrickMsg::CUndoTrickMsg(QString line) throw (NetProtocolException)
{
    msgType = UNDOTRICK_MSG;

    this->line = line;

    lineToMsg();
}

void CUndoTrickMsg::msgToLine()
{
    line = QString("undo trick\r\n");
}

void CUndoTrickMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("undo trick", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Undo trick: " + line.toStdString());
}

CReBidMsg::CReBidMsg()
{
    msgType = REBID_MSG;

    msgToLine();
}

CReBidMsg::CReBidMsg(QString line) throw (NetProtocolException)
{
    msgType = REBID_MSG;

    this->line = line;

    lineToMsg();
}

void CReBidMsg::msgToLine()
{
    line = QString("rebid\r\n");
}

void CReBidMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("rebid", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Rebid: " + line.toStdString());
}

CRePlayMsg::CRePlayMsg()
{
    msgType = REPLAY_MSG;

    msgToLine();
}

CRePlayMsg::CRePlayMsg(QString line) throw (NetProtocolException)
{
    msgType = REPLAY_MSG;

    this->line = line;

    lineToMsg();
}

void CRePlayMsg::msgToLine()
{
    line = QString("replay\r\n");
}

void CRePlayMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("replay", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Replay: " + line.toStdString());
}


CAttemptSynchronizeMsg::CAttemptSynchronizeMsg(Seat seat)
{
    msgType = ATTEMPT_SYNCHRONIZE_MSG;

    this->seat = seat;

    msgToLine();
}

CAttemptSynchronizeMsg::CAttemptSynchronizeMsg(QString line) throw (NetProtocolException)
{
    msgType = ATTEMPT_SYNCHRONIZE_MSG;

    this->line = line;

    lineToMsg();
}

void CAttemptSynchronizeMsg::msgToLine()
{
    line = QString("%1 attempt synchronize\r\n").arg(SEAT_NAMES_NET[seat]);
}

void CAttemptSynchronizeMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("attempt synchronize", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Ready to start: " + line.toStdString());

    seat = getSeat(line);
}


CConfirmSynchronizeMsg::CConfirmSynchronizeMsg(Seat seat)
{
    msgType = CONFIRM_SYNCHRONIZE_MSG;

    this->seat = seat;

    msgToLine();
}

CConfirmSynchronizeMsg::CConfirmSynchronizeMsg(QString line) throw (NetProtocolException)
{
    msgType = CONFIRM_SYNCHRONIZE_MSG;

    this->line = line;

    lineToMsg();
}

void CConfirmSynchronizeMsg::msgToLine()
{
    line = QString("%1 confirm synchronize\r\n").arg(SEAT_NAMES_NET[seat]);
}

void CConfirmSynchronizeMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("confirm synchronize", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Ready to start: " + line.toStdString());

    seat = getSeat(line);
}


CAllSynchronizedMsg::CAllSynchronizedMsg(Seat seat)
{
    msgType = ALL_SYNCHRONIZED_MSG;

    this->seat = seat;

    msgToLine();
}

CAllSynchronizedMsg::CAllSynchronizedMsg(QString line) throw (NetProtocolException)
{
    msgType = ALL_SYNCHRONIZED_MSG;

    this->line = line;

    lineToMsg();
}

void CAllSynchronizedMsg::msgToLine()
{
    line = QString("%1 all synchronized\r\n").arg(SEAT_NAMES_NET[seat]);
}

void CAllSynchronizedMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("all synchronized", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Ready to start: " + line.toStdString());

    seat = getSeat(line);
}


COriginalPBNStartMsg::COriginalPBNStartMsg(ScoringMethod scoringMethod)
{
    msgType = ORIGINAL_PBN_START_MSG;

    this->scoringMethod = scoringMethod;

    msgToLine();
}

COriginalPBNStartMsg::COriginalPBNStartMsg(QString line) throw (NetProtocolException)
{
    msgType = ORIGINAL_PBN_START_MSG;

    this->line = line;

    lineToMsg();
}

void COriginalPBNStartMsg::msgToLine()
{
    line = QString("Original PBN Stream Start. Default scoringmethod is: %1\r\n").arg(SCORINGMETHODS_NET[scoringMethod]);
}

void COriginalPBNStartMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("Original PBN Stream Start", Qt::CaseInsensitive) ||
            !line.contains("Default scoringmethod is", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Original PBN Start: " + line.toStdString());

    if (line.contains(SCORINGMETHODS_NET[IMP], Qt::CaseInsensitive))
        scoringMethod = IMP;
    else if (line.contains(SCORINGMETHODS_NET[MP], Qt::CaseInsensitive))
        scoringMethod = MP;
    else if (line.contains(SCORINGMETHODS_NET[RUBBER], Qt::CaseInsensitive))
        scoringMethod = RUBBER;
    else if (line.contains(SCORINGMETHODS_NET[PRACTICE], Qt::CaseInsensitive))
        scoringMethod = PRACTICE;
    else if (line.contains(SCORINGMETHODS_NET[NOSCORE], Qt::CaseInsensitive))
        scoringMethod = NOSCORE;
    else if (line.contains(SCORINGMETHODS_NET[FORSCORE], Qt::CaseInsensitive))
        scoringMethod = FORSCORE;
    else
        throw NetProtocolException("Net - No default scoring method: " + line.toStdString());
}


CPlayedPBNStartMsg::CPlayedPBNStartMsg()
{
    msgType = PLAYED_PBN_START_MSG;

    msgToLine();
}

CPlayedPBNStartMsg::CPlayedPBNStartMsg(QString line) throw (NetProtocolException)
{
    msgType = PLAYED_PBN_START_MSG;

    this->line = line;

    lineToMsg();
}

void CPlayedPBNStartMsg::msgToLine()
{
    line = QString("Played PBN Stream Start\r\n");
}

void CPlayedPBNStartMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("Played PBN Stream Start", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Played PBN Start: " + line.toStdString());
}


CEscapePBNMsg::CEscapePBNMsg()
{
    msgType = ESCAPE_MSG;

    msgToLine();
}

CEscapePBNMsg::CEscapePBNMsg(QString line) throw (NetProtocolException)
{
    msgType = ESCAPE_MSG;

    this->line = line;

    lineToMsg();
}

void CEscapePBNMsg::msgToLine()
{
    line = QString("Escape PBN Stream\r\n");
}

void CEscapePBNMsg::lineToMsg() throw (NetProtocolException)
{
    if (!line.contains("Escape PBN Stream", Qt::CaseInsensitive))
        throw NetProtocolException("Net - Escape PBN: " + line.toStdString());
}
