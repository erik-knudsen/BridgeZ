/* Erik Aagaard Knudsen.
  Copyright © 2014 - All Rights Reserved

  Project: ZBridge
  File: CGamesDoc.cpp
  Developers: eak

  Revision History:
  31-aug-2014 eak: Original

  Abstract: A set of bridge games (including results) played by the same players.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements handling of data for a set of bridge games (including results).
 */

#ifndef CGAMESDOC_H
#define CGAMESDOC_H

#include <QObject>
#include <QTextStream>
#include <QMap>

#include "ZBridgeException.h"
#include "cbidhistory.h"
#include "cplayhistory.h"

enum tagIds { TAG_EVENT = 0, TAG_SITE, TAG_DATE, TAG_ROUND, TAG_BOARD, TAG_WEST, TAG_NORTH, TAG_EAST, TAG_SOUTH,
            TAG_DEALER, TAG_VULNERABLE, TAG_DEAL, TAG_SCORING, TAG_DECLARER, TAG_CONTRACT, TAG_RESULT,
            TAG_AUCTION, TAG_PLAY, TAG_NONE};

class CGamesDoc : public QObject
{
    Q_OBJECT
public:
    explicit CGamesDoc(QObject *parent = 0);
    void readGames(QTextStream &original, QTextStream &played, QString &event) throw(PlayException);
    void writeOriginalGames(QTextStream &stream);
    void writePlayedGames(QTextStream &stream);
    void clearGames();
    void getNextDeal(int *board, int cards[][13], Seat *dealer, Team *vulnerable);
    void setPlayedResult(CBidHistory &bidHistory, CPlayHistory &playHistory, QString &westName, QString &northName,
                   QString &eastName, QString &southName, Seat declarer, Bids contract, Bids contractModifier, int result);
    void setAutoResult(CBidHistory &bidHistory, CPlayHistory &playHistory, QString &westName, QString &northName,
                   QString &eastName, QString &southName, Seat declarer, Bids contract, Bids contractModifier, int result);
    void determineEvents(QTextStream &original, QStringList &events);
signals:

public slots:

private:
    enum DealType { ORIGINAL_DEAL, RANDOM_DEAL };
    enum GameType { ORIGINAL_GAME, PLAYED_GAME, AUTO_GAME };
    struct CAuctionAndPlay
    {
        GameType gameType;
        CBidHistory bidHistory;
        CPlayHistory playHistory;
        QString westName;
        QString northName;
        QString eastName;
        QString southName;
        Seat declarer;
        Bids contract;
        Bids contractModifier;
        int result;
    };

    struct CGame
    {
        int board;
        int wCards[13];
        int nCards[13];
        int eCards[13];
        int sCards[13];
        Seat dealer;
        Team vulnerable;
        QList<CAuctionAndPlay *> auctionAndPlay;
    };

    void readGames(QTextStream &pbnText, QString &event, bool originalGames) throw(PlayException);
    void writeGame(QTextStream &stream, CGame *game, GameType gameType, QString event);
    int preloadPBNFile(QTextStream &PBNFile, QString event, QStringList &strLines,
                       QMap<int, QString> &auctionNotes,  QMap<int, QString> &playNotes);
    bool searchGame(QString &line, QString &event);
    tagIds parsePBNLine(QString &currentLine, QString &strValue, QMap<QString, tagIds> &tagName);
    Seat getCards(QString &strValue, int wCards[], int nCards[], int eCards[], int sCards[], int board) throw(PlayException);
    int getCards(QString &strValue, int inx, int cards[], int board) throw(PlayException);
    int getFaceValue(QString &strValue, int inx, int board)  throw(PlayException);
    int getBid(QString &line, int inx, Bids *bidCall);
    int getPlay(QString &line, int inx, int *playCall);
    int getNote(QString &line, int inx, int *note);

    QString event;
    QList<CGame *> games;
    int currentGameIndex;
    DealType dealType;
};

#endif // CGAMESDOC_H
