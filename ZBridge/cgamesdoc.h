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

//PBN tags.
enum tagIds { TAG_EVENT = 0, TAG_SITE, TAG_DATE, TAG_ROUND, TAG_BOARD, TAG_WEST, TAG_NORTH, TAG_EAST, TAG_SOUTH,
            TAG_DEALER, TAG_VULNERABLE, TAG_DEAL, TAG_SCORING, TAG_DECLARER, TAG_CONTRACT, TAG_RESULT,
            TAG_AUCTION, TAG_PLAY, TAG_NONE};

/**
 * @brief This class implements the handling for a set of bridge games (including results).
 *
 * The class handles reading and writing of pbn files with bridge games. It can also generate random bridge games.
 * It stores data for the games in an internal format.\n
 * There are three types of games:
 *   - Already played games read from a pbn file. There are called original games. If the games are random
 *     generated (by this program), then there are no original games.
 *   - Games played manually by the user in cooperation with this program. They are called played games.
 *   - Games played automatically by this program. They are called auto games.
 * When data is saved, only played and auto games are saved.\n
 * There are two types of game sets:
 *   - Original games as read from a pbn file.
 *   - Random generated (by this program) games.
 */
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
    enum DealType { ORIGINAL_DEAL, RANDOM_DEAL };               /**< The two types of game sets. */
    enum GameType { ORIGINAL_GAME, PLAYED_GAME, AUTO_GAME };    /**< The three types of games. */

    /**
     * @brief Structure with info for auction and play of how a game was played.
     */
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

    /**
     * @brief Structure for one game with a list of how the game was played by different players.
     */
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
    QString &setCards(Seat dealer, int wCards[], int nCards[], int eCards[], int sCards[], QString &line);
    void setSuit(int cards[], Suit suit, QString &line);
    QString &setContract(Bids contract, Bids contractModifier, QString &line);
    void makeAuction(QTextStream &stream, CBidHistory &bidHistory);
    void makePlay(QTextStream &stream, CPlayHistory &playHistory);

    QString event;              /**< The current game event. */
    QList<CGame *> games;       /**< List with all games. */
    int currentGameIndex;       /**< Index of the game currently being played. */
    DealType dealType;          /**< Type of current game set. */
};

#endif // CGAMESDOC_H
