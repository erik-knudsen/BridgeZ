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

#include <QMap>
#include <QTime>
#include <QtDebug>
#include <QMutexLocker>
#include <cassert>

#include "cgamesdoc.h"

//Context for the processing of a pbn file.
enum PBNContext { TAG_CONTEXT, AUCTION_CONTEXT, PLAY_CONTEXT};

//IMP table.
const int IMP_TABLE[] = { 20, 50, 90, 130, 170, 220, 270, 320, 370, 430, 500, 600, 750, 900,
                          1100, 1300, 1500, 1750, 2000, 2250, 2500, 3000, 3500, 4000, 100000 };

//PBN values.
static const QString PBN_SEAT_NAMES[4] = {"W", "N", "E", "S"};
static const QString PBN_CARD = "23456789TJQKA";
static const QString PBN_SUIT_NAMES[5] = { "C", "D", "H", "S", "NT" };
static const QString PBN_VULNERABILITY_NAMES[4] = {"None", "NS", "EW", "All"};
static const QString PBN_SCORING_NAMES[5] = {"IMP", "MP", "RUBBER", "PRACTICE", "NOSCORE"};

/**
 * @brief Constructor for the games class.
 * @param[in] parent Parent.
 */
CGamesDoc::CGamesDoc(QObject *parent) :
    QObject(parent)
{
    dealType = RANDOM_DEAL;
    scoringMethod = PRACTICE;

    clearGames(scoringMethod);
}

/**
 * @brief Read original and played games.
 * @param[in] original The stream from which to read original games.
 * @param[in] played The stream from which to read already played games (auto and played).
 * @param[in] event The event to read (in case there are more events in a pbn file; have never seen that though).
 * @param[in] ScoringMethod Type of scoring method in configuration.
 *
 * Reads original games in case there are any and determines whether this game set is original or random. Then
 * read alredy played games in case there are any.
 */
void CGamesDoc::readGames(QTextStream &original, QTextStream &played,
                          QString &event, ScoringMethod scoringMethod) throw(PlayException)
{
    ScoringMethod originalScoringMethod = NOSCORE;

    //Clean up for new game.
    clearGames(NOSCORE);

    this->event = event;

    //Original games (check that there are original games).
    if (original.device() != 0)
    {
        readGames(original, event, true);
        if (games.size() > 0)
        {
            dealType = ORIGINAL_DEAL;
            if ((this->scoringMethod == RUBBER) || (this->scoringMethod == PRACTICE))
                this->scoringMethod = NOSCORE;
            originalScoringMethod = this->scoringMethod;
        }
    }

    //Played games (check that there are played games).
    if (played.device() != 0)
    {
        readGames(played, event, false);
    }

    if (this->scoringMethod == NOSCORE)
    {
        this->scoringMethod = scoringMethod;
    }

    if ((dealType != ORIGINAL_DEAL) || ((dealType == ORIGINAL_DEAL) && originalScoringMethod == NOSCORE))
        computerPlays =((this->scoringMethod == MP) || (this->scoringMethod == IMP));
}

/**
 * @brief Write original games.
 * @param[in] stream The stream to write the original games to.
 *
 * Writes all original games to the given stream. The event name of the games are only written for the
 * first game. Replication symbol is used for the following games.
 */
void CGamesDoc::writeOriginalGames(QTextStream &stream)
{
    QListIterator<CGame *> gameItr(games);
    if (gameItr.hasNext())
    {
        CGame *nextGame = gameItr.next();
        writeGame(stream, nextGame, ORIGINAL_GAME, event);
    }
    while (gameItr.hasNext())
    {
        CGame *nextGame = gameItr.next();
        writeGame(stream, nextGame, ORIGINAL_GAME, "#");
    }
}

/**
 * @brief Write played games.
 * @param[in] stream The stream to write the played games to.
 *
 * Writes all played games to the given stream. The event name of the games are only written for the
 * first game. Replication symbol is used for the following games. The sequence of games written is always
 * first played game then auto game. Each game might have been played in two ways, played game and auto
 * game.
 */
void CGamesDoc::writePlayedGames(QTextStream &stream)
{
    QListIterator<CGame *> gameItr(games);
    if (gameItr.hasNext())
    {
        CGame *nextGame = gameItr.next();
        writeGame(stream, nextGame, PLAYED_GAME, event);
        writeGame(stream, nextGame, AUTO_GAME, "#");
    }
    while (gameItr.hasNext())
    {
        CGame *nextGame = gameItr.next();
        writeGame(stream, nextGame, PLAYED_GAME, "#");
        writeGame(stream, nextGame, AUTO_GAME, "#");
    }
}

/**
 * @brief Clear all games and prepare initially for a new set of random games.
 * @param[in] ScoringMethod Type of scoring method in configuration.
 */
void CGamesDoc::clearGames(ScoringMethod scoringMethod)
{
    //Clean up for new (random) game.
    while (!games.isEmpty())
    {
        CGame *game = games.takeFirst();
        while (!game->auctionAndPlay.isEmpty())
            delete(game->auctionAndPlay.takeFirst());
        delete game;

    }
    event.clear();
    curEvent.clear();
    currentGameIndex = 0;
    dealType = RANDOM_DEAL;
    this->scoringMethod = scoringMethod;
    computerPlays = ((scoringMethod == MP) || (scoringMethod == IMP));

    emit sUpdateGame();
}

void CGamesDoc::prepNextDeal()
{
    currentGameIndex++;
    emit sUpdateGame();
}

/**
 * @brief Retrieve the next game in the current game set (server only).
 * @param[out] board Board number for the next game.
 * @param[out] cards The cards for the next game.
 * @param[out] dealer The dealer for the next game.
 * @param[out] vulnerable The vulnerability for the next game.
 *
 * Retrieves the next game in the current game set. Depending on the game set type (random or original),
 * then this might be a random generated game or the next game in a game set read from a pbn file.
 */
void CGamesDoc::getNextDeal(int *board, int cards[][13], Seat *dealer, Team *vulnerable)
{
    //Give random card distribution etc.?
    if (dealType == RANDOM_DEAL)
    {
        CGame *currentGame = new CGame();

        const Seat DEALER[4] = { NORTH_SEAT, EAST_SEAT, SOUTH_SEAT, WEST_SEAT };
        int i, j, inx;
        int cardDeck[52];

        //Info about board.
        int boardNo = currentGameIndex + 1;

        *board = boardNo;
        *dealer = DEALER[(boardNo - 1) % 4];

        if (scoringMethod != RUBBER)
        {
            const Team VULNERABLE[4] = { NEITHER, NORTH_SOUTH, EAST_WEST, BOTH };
            *vulnerable = VULNERABLE[((boardNo - 1) % 4 + boardNo/4)%4];
        }
        else
        {
            *vulnerable = NEITHER;
            if (currentGameIndex > 0)
                *vulnerable = getRubberVulnerable(currentGameIndex - 1);
        }

        currentGame->board = *board;
        currentGame->dealer = *dealer;
        currentGame->vulnerable = *vulnerable;

        //Shuffle card deck.
        for (i = 0; i < 52; i++)
            cardDeck[i] = i;
        QTime cur;
        qsrand(cur.currentTime().msec());
        for (i = 0; i < 52; i++)
        {
            inx = rand()%52;
            j = cardDeck[i];
            cardDeck[i] = cardDeck[inx];
            cardDeck[inx] = j;
        }

        //Give cards.
        for (j = 0; j < 4; j++)
            for (i = 0; i < 13; i++)
                cards[j][i] = cardDeck[j * 13 + i];

        //Save game.
        for (i = 0; i < 13; i++)
        {
            currentGame->wCards[i] = cards[WEST_SEAT][i];
            currentGame->nCards[i] = cards[NORTH_SEAT][i];
            currentGame->eCards[i] = cards[EAST_SEAT][i];
            currentGame->sCards[i] = cards[SOUTH_SEAT][i];
        }

        //Append to already played games.
        games.append(currentGame);
    }

    //Give cards as read from a pbn file?
    else
    {
        //Take card distribution etc. from next game.
        if (games.size() > currentGameIndex)
        {
            CGame *currentGame = games[currentGameIndex];
            *board = currentGame->board;
            *dealer = currentGame->dealer;
            if (scoringMethod != RUBBER)
                *vulnerable = currentGame->vulnerable;
            else
            {
                *vulnerable = NEITHER;
                if (currentGameIndex > 0)
                    *vulnerable = getRubberVulnerable(currentGameIndex - 1);
                currentGame->vulnerable = *vulnerable;
            }
            for (int i = 0; i < 13; i++)
            {
                cards[WEST_SEAT][i] = currentGame->wCards[i];
                cards[NORTH_SEAT][i] = currentGame->nCards[i];
                cards[EAST_SEAT][i] = currentGame->eCards[i];
                cards[SOUTH_SEAT][i] = currentGame->sCards[i];
            }
        }
    }
}

/**
 * @brief Retrieve the current game in the current game set (server only).
 * @param[out] board Board number for the current game.
 * @param[out] cards The cards for the current game.
 * @param[out] dealer The dealer for the current game.
 * @param[out] vulnerable The vulnerability for the current game.
 *
 * Retrieves the current game in the current game set.
 */
void CGamesDoc::getCurrentDeal(int *board, int cards[][13], Seat *dealer, Team *vulnerable)
{
    //Take card distribution etc. from current game.
    if (games.size() > currentGameIndex)
    {
        CGame *currentGame = games[currentGameIndex];
        *board = currentGame->board;
        *dealer = currentGame->dealer;
        *vulnerable = currentGame->vulnerable;
        for (int i = 0; i < 13; i++)
        {
            cards[WEST_SEAT][i] = currentGame->wCards[i];
            cards[NORTH_SEAT][i] = currentGame->nCards[i];
            cards[EAST_SEAT][i] = currentGame->eCards[i];
            cards[SOUTH_SEAT][i] = currentGame->sCards[i];
        }
    }
}

/**
 * @brief Set info for the next game in the current game set (client only).
 * @param[in] board Board number for the next game.
 * @param[in] dealer The dealer for the next game.
 * @param[in] vulnerable The vulnerability for the next game.
 *
 * Sets info for the next game in the current game set. Only info for random generated games need
 * to be set.
 */
void CGamesDoc::setNextDeal(int board, Seat dealer, Team vulnerable)
{
    //Random card distribution?
    if (dealType == RANDOM_DEAL)
    {
        CGame *currentGame = new CGame();

        currentGame->board = board;
        currentGame->dealer = dealer;
        currentGame->vulnerable = vulnerable;

        //Append to already played games.
        games.append(currentGame);
    }
}

/**
 * @brief Set cards for the next game in the current game set (client only).
 * @param[in] cards The cards for the next game.
 *
 * Sets cards for the next game in the current game set. Only cards for random generated games
 * need to be set.
 */
void CGamesDoc::setNextDeal(int cards[][13])
{
    //Give random card distribution etc.?
    if (dealType == RANDOM_DEAL)
    {
        CGame *currentGame = games[currentGameIndex];

        //Save game.
        for (int i = 0; i < 13; i++)
        {
            currentGame->wCards[i] = cards[WEST_SEAT][i];
            currentGame->nCards[i] = cards[NORTH_SEAT][i];
            currentGame->eCards[i] = cards[EAST_SEAT][i];
            currentGame->sCards[i] = cards[SOUTH_SEAT][i];
        }
    }
}

/**
 * @brief Set played result for the current game (manually played game).
 * @param[in] bidHistory The bid history.
 * @param[in] playHistory The play history.
 * @param[in] westName Name of west player.
 * @param[in] northName Name of north player.
 * @param[in] eastName Name of east player.
 * @param[in] southName Name of south player.
 * @param[in] declarer The declarer.
 * @param[in] contract The contract.
 * @param[in] contractModifier The conract modifier (if any).
 * @param[in] result The result.
 *
 * After the current game has been manually played, then the results are assigned to the current game
 * structure.
 */
void CGamesDoc::setPlayedResult(CBidHistory &bidHistory, CPlayHistory &playHistory, QString &westName,
                                QString &northName, QString &eastName, QString &southName)
{
    //In case names deviate from already defined player names, then use already defined names.
    QString wN, nN, eN, sN;
    wN = westName;
    nN = northName;
    eN = eastName;
    sN = southName;
    getPlayerNames(PLAYED_GAME, &wN, &nN, &eN, &sN);

    //Set result of manually played game.
    setResult(PLAYED_GAME, bidHistory, playHistory, wN, nN, eN, sN);
}

/**
 * @brief Set auto result for the current game (auto played game).
 * @param[in] bidHistory The bid history.
 * @param[in] playHistory The play history.
 * @param[in] westName Name of west player.
 * @param[in] northName Name of north player.
 * @param[in] eastName Name of east player.
 * @param[in] southName Name of south player.
 * @param[in] declarer The declarer.
 * @param[in] contract The contract.
 * @param[in] contractModifier The conract modifier (if any).
 * @param[in] result The result.
 *
 * After the current game has been auto played, then the results are assigned to the current game
 * structure.
 */
void CGamesDoc::setAutoResult(CBidHistory &bidHistory, CPlayHistory &playHistory, QString &westName,
                              QString &northName, QString &eastName, QString &southName)
{
    //In case names deviate from already defined auto names, then use already defined names.
    QString wN, nN, eN, sN;
    wN = westName;
    nN = northName;
    eN = eastName;
    sN = southName;
    getPlayerNames(AUTO_GAME, &wN, &nN, &eN, &sN);

    //Set result af automatically played game.
    setResult(AUTO_GAME, bidHistory, playHistory, wN, nN, eN, sN);
}

void CGamesDoc::setResult(GameType gameType, CBidHistory &bidHistory, CPlayHistory &playHistory, QString &westName,
                                QString &northName, QString &eastName, QString &southName)
{
    //Check for passed out games.
    if (bidHistory.passedOut())
            playHistory.setBidInfo(BID_PASS, BID_NONE, NO_SEAT);

    //Check for not or partially played games.
    if (((playHistory.getContract() != BID_NONE) && (playHistory.getResult() != -1)) ||
        (playHistory.getContract() == BID_PASS))
    {
        CAuctionAndPlay * auctionAndPlay = new CAuctionAndPlay();

        auctionAndPlay->gameType = gameType;
        auctionAndPlay->bidHistory = bidHistory;
        auctionAndPlay->playHistory = playHistory;
        auctionAndPlay->westName = westName;
        auctionAndPlay->northName = northName;
        auctionAndPlay->eastName = eastName;
        auctionAndPlay->southName = southName;
        if (playHistory.getContract() == BID_PASS)
        {
            auctionAndPlay->declarer = NO_SEAT;
            auctionAndPlay->contract = BID_PASS;
            auctionAndPlay->contractModifier = BID_NONE;
        }
        else
        {
            auctionAndPlay->declarer = playHistory.getDeclarer();
            auctionAndPlay->contract = playHistory.getContract();
            auctionAndPlay->contractModifier = playHistory.getContractModifier();
        }
        auctionAndPlay->result = playHistory.getResult();

        QMutexLocker locker(&lock);
        games[currentGameIndex]->auctionAndPlay.append(auctionAndPlay);
    }
}

/**
 * @brief Determine the events in a pbn file.
 * @param[in] original The stream to read the pbn file from.
 * @param[out] events The events found.
 *
 * Determine the pbn events in a pbn file (I have never seen more than one event in a pbn file).
 */
void CGamesDoc::determineEvents(QTextStream &original, QStringList &events)
{
    QString line;
    QString event;

    while (!original.atEnd())
    {
        line = original.readLine();

        if (line.indexOf("[EVENT", 0, Qt::CaseInsensitive) == 0)
        {
            int firstInx = line.indexOf('"');
            int lastInx = line.lastIndexOf('"');

            //Check for syntax (silently discard in case of syntax error).
            if ((firstInx == -1) || (firstInx == lastInx))
                continue;

            //Handle header (##) and substitution (#).
            if (line.indexOf("##", firstInx + 1) == (firstInx + 1))
                event = line.mid(firstInx + 3, lastInx - (firstInx + 3));
            else if (line.indexOf('#', firstInx + 1) != (firstInx + 1))
                event = line.mid(firstInx + 1, lastInx - (firstInx + 1));
            else
                continue;

            if (event == "?")
                event = "";

            if (events.indexOf(event) == -1)
                events.append(event);
        }
    }
}

int CGamesDoc::getNumberOfNotPlayedGames()
{
    int noGames = (dealType == RANDOM_DEAL) ? (NO_RANDOM_DEALS) : (games.size());

    return (noGames - getNumberPlayedGames());
}

int CGamesDoc::getNumberPlayedGames()
{
    return currentGameIndex;
}

int CGamesDoc::getNumberAuctionAndPlay(int gameIndex)
{
    assert(gameIndex < games.size());

    return (games[gameIndex]->auctionAndPlay.size());
}

int CGamesDoc::getMaxNumberAuctionAndPlay()
{
    int maxNuberAuctionAndPlay = 0;

    for (int i = 0; i < games.size(); i++)
        if (games[i]->auctionAndPlay.size() > maxNuberAuctionAndPlay)
            maxNuberAuctionAndPlay = games[i]->auctionAndPlay.size();

    return maxNuberAuctionAndPlay;
}

int CGamesDoc::getPlayedAuctionAndPlayIndex(int gameIndex)
{
    int auctionAndPlayIndex;
    int numberAuctionAndPlay = getNumberAuctionAndPlay(gameIndex);

    for (auctionAndPlayIndex = 0; auctionAndPlayIndex < numberAuctionAndPlay; auctionAndPlayIndex++)
        if (games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->gameType == PLAYED_GAME)
            break;

    if (auctionAndPlayIndex >= numberAuctionAndPlay)
        auctionAndPlayIndex = -1;

    return auctionAndPlayIndex;
}

void CGamesDoc::getGame(int gameIndex, int *board, Seat *dealer, Team *vulnerable,
             int *wCards, int *nCards, int *eCards, int *sCards)
{
    assert(gameIndex < games.size());

    *board = games[gameIndex]->board;
    *dealer = games[gameIndex]->dealer;
    *vulnerable = games[gameIndex]->vulnerable;
    if (wCards != 0)
    {
        assert ((nCards != 0) && (eCards != 0) && (sCards != 0));
        for (int i = 0; i < 13; i++)
        {
            wCards[i] = games[gameIndex]->wCards[i];
            nCards[i] = games[gameIndex]->nCards[i];
            eCards[i] = games[gameIndex]->eCards[i];
            sCards[i] = games[gameIndex]->sCards[i];
        }
    }
}

void CGamesDoc::getPlayerNames(GameType gameType, QString *westName, QString *northName, QString *eastName, QString *southName)
{
    assert((gameType == PLAYED_GAME) || (gameType == AUTO_GAME));

    QListIterator<CGame *> gameItr(games);
    while (gameItr.hasNext())
    {
        CGame *nextGame = gameItr.next();
        QListIterator<CAuctionAndPlay *> auctionAndPlayItr(nextGame->auctionAndPlay);
        while (auctionAndPlayItr.hasNext())
        {
            CAuctionAndPlay *nextAuctionAndPlay = auctionAndPlayItr.next();
            if (nextAuctionAndPlay->gameType == gameType)
            {
                *westName = nextAuctionAndPlay->westName;
                *northName = nextAuctionAndPlay->northName;
                *eastName = nextAuctionAndPlay->eastName;
                *southName = nextAuctionAndPlay->southName;

                return;
            }
        }
    }
}

void CGamesDoc::getActorNames(int gameIndex, int auctionAndPlayIndex,
                              QString *westName, QString *northName, QString *eastName, QString *southName)
{
    assert(gameIndex < games.size());

    if ((auctionAndPlayIndex < 0) || (auctionAndPlayIndex >= games[gameIndex]->auctionAndPlay.size()))
        return;

    *westName = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->westName;
    *northName = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->northName;
    *eastName = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->eastName;
    *southName = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->southName;
}

void CGamesDoc::getAuctionAndPlay(int gameIndex, int auctionAndPlayIndex, Seat *declarer, Bids *contract,
                                  Bids *contractModifier, int *result,
                                  CBidHistory *bidHistory, CPlayHistory *playHistory)
{
    assert(gameIndex < games.size());

    *declarer = NO_SEAT;
    *contract = BID_NONE;
    *contractModifier = BID_NONE;
    *result = -1;

    if ((auctionAndPlayIndex >= 0) && (auctionAndPlayIndex < games[gameIndex]->auctionAndPlay.size()))
    {
        *declarer = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->declarer;
        *contract =games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->contract;
        *contractModifier = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->contractModifier;
        *result = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->result;
        if (bidHistory != 0)
            *bidHistory = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->bidHistory;
        if (playHistory != 0)
            *playHistory = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->playHistory;
    }
}

/**
 * @brief Calculate the duplicate score for the given game/play.
 * @param[in] gameIndex Index of the given game.
 * @param[in] auctionAndPlayIndex Index of the given play.
 * @param[in] ns If true then calculate relative to NS else calculate relative to declarer.
 * @return The calculated duplicate score. The value is negative if the score belongs to
 *         the opponent.
 */
int CGamesDoc::getDuplicateScore(int gameIndex, int auctionAndPlayIndex, bool ns)
{
    assert(gameIndex < games.size());
    assert(auctionAndPlayIndex < games[gameIndex]->auctionAndPlay.size());

    Seat dealer, declarer;
    Team vulnerable;
    Bids contract, contractModifier;
    int result;
    int duplicateScore;

    dealer = games[gameIndex]->dealer;
    vulnerable = games[gameIndex]->vulnerable;
    declarer = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->declarer;
    contract = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->contract;
    contractModifier = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->contractModifier;
    result = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->result;

    //Was it a pass game?
    if (contract == BID_PASS)
        return 0;

    Suit suit = BID_SUIT(contract);
    int level = BID_LEVEL(contract);

    bool declarerVulnerable = (vulnerable == BOTH) ||
           ((vulnerable == NORTH_SOUTH) && (declarer == SOUTH_SEAT) || (declarer == NORTH_SEAT)) ||
          ((vulnerable ==EAST_WEST) && (declarer == EAST_SEAT) || (declarer == WEST_SEAT));

    //Contract made?
    if (result >= (level + 6))
    {
        //Contract points.
        int contractPoints = ISNOTRUMP(suit) ? (40) : ISMAJOR(suit) ? (30) : (20);
        if (result > 7)
            contractPoints += (ISMINOR(suit) ? (20) : (30)) * (result - 7);
        if (IS_DOUBLE_BID(contractModifier))
            contractPoints *=2;
        else if (IS_REDOUBLE_BID(contractModifier))
            contractPoints *=4;

        //Overtrick points.
        int prTrick;
        if (IS_DOUBLE_BID(contractModifier))
            prTrick = declarerVulnerable ? (200) : (100);
        else if (IS_REDOUBLE_BID(contractModifier))
            prTrick = declarerVulnerable ? (400) : (200);
        else
            prTrick = ISMINOR(suit) ? (20) : (30);
        int overtrickPoints = prTrick * (result - (level + 6));

        //Slam bonus.
        int slamBonus;
        slamBonus = (level >= 6) ? (declarerVulnerable ? (750) : (500)) : (0);
        if (level == 7)
            slamBonus *= 2;

        //Doubled or redoubled bonus.
        int doubleBonus = IS_DOUBLE_BID(contractModifier) ? (50) : IS_REDOUBLE_BID(contractModifier) ? (100) :(0);

        //Game or part game bonus.
        int gameBonus = (contractPoints < 100) ? (50) : declarerVulnerable ? (500) : (300);

        duplicateScore = contractPoints + overtrickPoints + slamBonus + doubleBonus + gameBonus;
    }

    //Contract not made.
    else
    {
        int underTrick_1, underTrick_2_3, underTrick_4_;

        if (declarerVulnerable)
        {
            if (IS_DOUBLE_BID(contractModifier))
            {
                underTrick_1 = 200;
                underTrick_2_3 = 300;
                underTrick_4_ = 300;
            }
            else if (IS_REDOUBLE_BID(contractModifier))
            {
                underTrick_1 = 400;
                underTrick_2_3 = 600;
                underTrick_4_ = 600;
            }
            else
                underTrick_1 = underTrick_2_3 = underTrick_4_ = 100;
        }
        else
        {
            if (IS_DOUBLE_BID(contractModifier))
            {
                underTrick_1 = 100;
                underTrick_2_3 = 200;
                underTrick_4_ = 300;
            }
            else if (IS_REDOUBLE_BID(contractModifier))
            {
                underTrick_1 = 200;
                underTrick_2_3 = 400;
                underTrick_4_ = 600;
            }
            else
                underTrick_1 = underTrick_2_3 = underTrick_4_ = 50;
        }

        int noUnderTricks = (level + 6) - result;
        if (noUnderTricks == 1)
            duplicateScore = underTrick_1;
        else if ((noUnderTricks == 2) || (noUnderTricks == 3))
            duplicateScore = underTrick_1 + (noUnderTricks - 1) * underTrick_2_3;
        else
            duplicateScore = underTrick_1 + 2 * underTrick_2_3 + (noUnderTricks - 3) * underTrick_4_;

        duplicateScore = -duplicateScore;
    }

    if (ns & ((declarer == WEST_SEAT) || (declarer == EAST_SEAT)))
            duplicateScore = -duplicateScore;

    return duplicateScore;
}

/**
 * @brief Calculate duplicate points for a given game/play.
 * @param[in]gameIndex Index of the game.
 * @param[in] auctionAndPlayIndex Index of the play.
 * @param[in] scoringMethod Scoring method is either MP or IMP.
 * @param[in] ns If true then calculate relative to NS else calculate relative to declarer.
 * @return The calculated duplicate point. The value is negative if the point belongs to
 *         the opponent.
 */
float CGamesDoc::getDuplicatePointBoard(int gameIndex, int auctionAndPlayIndex,
                                        int scoringMethod, bool ns)
{
    float point = 0;

    assert(gameIndex < games.size());

    int maxNumberAuctionAndPlay = getMaxNumberAuctionAndPlay();
    assert(maxNumberAuctionAndPlay >= 2);

    //Has the game been played?
    if (auctionAndPlayIndex == -1)
        return (scoringMethod == MP) ? (maxNumberAuctionAndPlay - 1) : (0);

    assert(auctionAndPlayIndex < games[gameIndex]->auctionAndPlay.size());

    int nrPlayed = games[gameIndex]->auctionAndPlay.size();
    if (nrPlayed == 1)
        return (scoringMethod == MP) ? (maxNumberAuctionAndPlay - 1) : (0);

    Seat declarer = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->declarer;

    //Allocate score table and get duplicate scores.
    int *scores = new int[nrPlayed];
    for (int i = 0; i < nrPlayed; i++)
        scores[i] = getDuplicateScore(gameIndex, i, true);

    if (scoringMethod == MP)
    {
        //Calculate point based on MP.
        int *gt = new int[nrPlayed];
        int *eq = new int[nrPlayed];

        for (int i = 0; i < nrPlayed; i++)
            gt[i] = eq[i] = 0;

        for (int i = 0; i < nrPlayed; i++)
            for (int j = 0; j < nrPlayed; j++)
            {
                if (scores[i] < scores[j])
                    gt[i]++;
                else if (scores[i] == scores[j])
                    eq[i]++;
            }

        point = 2*nrPlayed - 2 * (gt[auctionAndPlayIndex] + 1);
        for (int j = 1; j < eq[auctionAndPlayIndex]; j++)
            point += 2*nrPlayed - 2 * (gt[auctionAndPlayIndex] + 1 + j);
        point = point / eq[auctionAndPlayIndex];
        if (!ns && ((declarer == WEST_SEAT) || (declarer == EAST_SEAT)))
            point = 2*nrPlayed - 2 - point;

        delete []eq;
        delete []gt;

        //Correct for non played games.
        if (maxNumberAuctionAndPlay > nrPlayed)
            point += (maxNumberAuctionAndPlay - nrPlayed);

        //Normalize to %.
        point = point * 100 / (2 * maxNumberAuctionAndPlay - 2);
    }
    else
    {
        //Calculate point based on IMP. First played games.
        for (int i = 0; i < nrPlayed; i++)
            if (auctionAndPlayIndex != i)
            {
                int diff = scores[auctionAndPlayIndex] - scores[i];
                int k = -1;
                while (abs(diff) > IMP_TABLE[++k]) ;
                point += (diff > 0) ? k : -k;
            }
        //Next non played games (IMP is 0 for non played games).
        for (int i = nrPlayed; i < maxNumberAuctionAndPlay; i++)
        {
            int diff = scores[auctionAndPlayIndex] - 0;
            int k = -1;
            while (abs(diff) > IMP_TABLE[++k]) ;
            point += (diff > 0) ? k : -k;

        }
        if (!ns &&((declarer == WEST_SEAT) || (declarer == EAST_SEAT)))
            point = -point;

        //Normalize.
        point /= maxNumberAuctionAndPlay;
    }
    delete []scores;

    return point;
}

/**
 * @brief Calculate total duplicate points for a given pair of players.
 * @param[in] gameIndex Calculate until and including the game with this index.
 * @param[in] nameWN Name of West (or North).
 * @param[in] nameES Name of East (or South).
 * @param[in] scoringMethod Scoring method is either MP or IMP.
 * @return  The calculated total duplicate point. The value is negative if the point
 *          belongs to the opponent.
 */
float CGamesDoc::getDuplicateResultAll(int gameIndex, QString &nameWN, QString &nameES,
                                     int scoringMethod)
{
    assert(gameIndex < games.size());

    int maxNumberAuctionAndPlay = getMaxNumberAuctionAndPlay();

    assert(maxNumberAuctionAndPlay >= 2);

    float result = 0;
    int no = 0;

    for (int i = 0; i <= gameIndex; i++)
    {
        Seat seat;
        float point;
        int auctionAndPlayIndex = getIndexAndSeat(i, nameWN, nameES, &seat);
        no++;
        point = getDuplicatePointBoard(i, auctionAndPlayIndex, scoringMethod, true);
            if ((scoringMethod == MP) && ((seat == WEST_SEAT) || (seat == EAST_SEAT)))
                point = 100 - point;
            else if ((scoringMethod == IMP) && ((seat == WEST_SEAT) || (seat == EAST_SEAT)))
                point = -point;

            result += point;
    }
    if (no > 0)
        result /=no;

    return result;
}

/**
 * @brief Get pairs of players in the games.
 * @param[in] gameIndex Until and including the game with this index.
 * @param[out] pairWN List of names west/north.
 * @param[out] pairES List of names east/south.
 * @return Number of pairs.
 */
int CGamesDoc::getPairs(int gameIndex, QStringList &pairWN, QStringList &pairES)
{
    assert(gameIndex < games.size());

    int noPairs = 0;

    //Search all games.
    for (int i = 0; i <= gameIndex; i++)
    {
        //Search all plays (of this game).
        for (int j = 0; j < games[i]->auctionAndPlay.size(); j++)
        {
            if (!(pairWN.contains(games[i]->auctionAndPlay[j]->westName, Qt::CaseInsensitive)) ||
                (!pairES.contains(games[i]->auctionAndPlay[j]->eastName, Qt::CaseInsensitive)))
            {
                //Found a new west/east pair.
                pairWN.append(games[i]->auctionAndPlay[j]->westName);
                pairES.append(games[i]->auctionAndPlay[j]->eastName);
                noPairs++;
            }
            if (!(pairWN.contains(games[i]->auctionAndPlay[j]->northName, Qt::CaseInsensitive)) ||
                (!pairES.contains(games[i]->auctionAndPlay[j]->southName, Qt::CaseInsensitive)))
            {
                //Found a new north/south pair.
                pairWN.append(games[i]->auctionAndPlay[j]->northName);
                pairES.append(games[i]->auctionAndPlay[j]->southName);
                noPairs++;
            }
        }
    }
    return noPairs;
}

/**
 * @brief Get below the line points for a given (user) played game/play in rubber play.
 * @param[in] gameIndex Index of the game
 * @return Below the line points. Relative to NS.
 */
int CGamesDoc::getBelowTheLine(int gameIndex)
{
    assert(gameIndex < games.size());

    int auctionAndPlayIndex = getPlayedAuctionAndPlayIndex(gameIndex);

    //Has the game been played?
    if (auctionAndPlayIndex == -1)
        return 0;

    assert(auctionAndPlayIndex < games[gameIndex]->auctionAndPlay.size());

    int result = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->result;
    Bids contract = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->contract;

    //Was it a pass game?
    if (contract == BID_PASS)
        return 0;

    int level = BID_LEVEL(contract);
    int contractPoints = 0;

    //Contract made?
    if (result >= (level + 6))
    {
        Seat declarer = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->declarer;
        Suit suit = BID_SUIT(contract);
        Bids contractModifier = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->contractModifier;

        //Contract points.
        contractPoints = ISNOTRUMP(suit) ? (40) : ISMAJOR(suit) ? (30) : (20);
        if (result > 7)
            contractPoints += (ISMINOR(suit) ? (20) : (30)) * (result - 7);
        if (IS_DOUBLE_BID(contractModifier))
            contractPoints *=2;
        else if (IS_REDOUBLE_BID(contractModifier))
            contractPoints *=4;

        if ((declarer == WEST_SEAT) || (declarer == EAST_SEAT))
                contractPoints = -contractPoints;
    }

    return contractPoints;
}

/**
 * @brief Get above the line points for a given game/play in rubber play.
 * @param[in] gameIndex Index of the game
 * @return Above the line points. Relative to NS.
 */
int CGamesDoc::getAboveTheLine(int gameIndex)
{
    assert(gameIndex < games.size());

    int auctionAndPlayIndex = getPlayedAuctionAndPlayIndex(gameIndex);

    //Has the game been played?
    if (auctionAndPlayIndex == -1)
        return 0;

    Seat dealer, declarer;
    Team vulnerable;
    Bids contract, contractModifier;
    int result;
    int aboveTheLineScore;

    dealer = games[gameIndex]->dealer;
    vulnerable = games[gameIndex]->vulnerable;
    declarer = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->declarer;
    contract = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->contract;
    contractModifier = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->contractModifier;
    result = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->result;

    Suit suit = BID_SUIT(contract);
    int level = BID_LEVEL(contract);

    bool declarerVulnerable = (vulnerable == BOTH) ||
           ((vulnerable == NORTH_SOUTH) && (declarer == SOUTH_SEAT) || (declarer == NORTH_SEAT)) ||
          ((vulnerable ==EAST_WEST) && (declarer == EAST_SEAT) || (declarer == WEST_SEAT));

    //Contract made?
    if (result >= (level + 6))
    {
        //Overtrick points.
        int prTrick;
        if (IS_DOUBLE_BID(contractModifier))
            prTrick = declarerVulnerable ? (200) : (100);
        else if (IS_REDOUBLE_BID(contractModifier))
            prTrick = declarerVulnerable ? (400) : (200);
        else
            prTrick = ISMINOR(suit) ? (20) : (30);
        int overtrickPoints = prTrick * (result - (level + 6));

        //Slam bonus.
        int slamBonus;
        slamBonus = (level >= 6) ? (declarerVulnerable ? (750) : (500)) : (0);
        if (level == 7)
            slamBonus *= 2;

        //Doubled or redoubled bonus.
        int doubleBonus = IS_DOUBLE_BID(contractModifier) ? (50) : IS_REDOUBLE_BID(contractModifier) ? (100) :(0);

        aboveTheLineScore = overtrickPoints + slamBonus + doubleBonus;
    }

    //Contract not made.
    else
    {
        int underTrick_1, underTrick_2_3, underTrick_4_;

        if (declarerVulnerable)
        {
            if (IS_DOUBLE_BID(contractModifier))
            {
                underTrick_1 = 200;
                underTrick_2_3 = 300;
                underTrick_4_ = 300;
            }
            else if (IS_REDOUBLE_BID(contractModifier))
            {
                underTrick_1 = 400;
                underTrick_2_3 = 600;
                underTrick_4_ = 600;
            }
            else
                underTrick_1 = underTrick_2_3 = underTrick_4_ = 100;
        }
        else
        {
            if (IS_DOUBLE_BID(contractModifier))
            {
                underTrick_1 = 100;
                underTrick_2_3 = 200;
                underTrick_4_ = 300;
            }
            else if (IS_REDOUBLE_BID(contractModifier))
            {
                underTrick_1 = 200;
                underTrick_2_3 = 400;
                underTrick_4_ = 600;
            }
            else
                underTrick_1 = underTrick_2_3 = underTrick_4_ = 50;
        }

        int noUnderTricks = (level + 6) - result;
        if (noUnderTricks == 1)
            aboveTheLineScore = underTrick_1;
        else if ((noUnderTricks == 2) || (noUnderTricks == 3))
            aboveTheLineScore = underTrick_1 + (noUnderTricks - 1) * underTrick_2_3;
        else
            aboveTheLineScore = underTrick_1 + 2 * underTrick_2_3 + (noUnderTricks - 3) * underTrick_4_;

        aboveTheLineScore = -aboveTheLineScore;
    }

    if ((declarer == WEST_SEAT) || (declarer == EAST_SEAT))
            aboveTheLineScore = -aboveTheLineScore;

    return aboveTheLineScore;
}

/**
 * @brief Get honor bonus points for a given game/play in rubber play.
 * @param[in] gameIndex Index of the game
 * @param[in] auctionAndPlayIndex Index of the play.
 * @return Honor bonus points. Relative to NS.
 */
int CGamesDoc::getHonorBonus(int gameIndex, int auctionAndPlayIndex)
{
    assert(gameIndex < games.size());

    Bids contract = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->contract;
    Suit suit = BID_SUIT(contract);


    if (suit != NOTRUMP)
    {
        int ace = SUIT_INTERVAL[suit][2];
        int king = SUIT_INTERVAL[suit][2] - 1;
        int queen = SUIT_INTERVAL[suit][2] - 2;
        int jack = SUIT_INTERVAL[suit][2] - 3;
        int ten = SUIT_INTERVAL[suit][2] - 4;

        int west, north, east, south;
        west = north = east = south = 0;

        for (int i = 0; i < 13; i++)
        {
            int wCard = games[gameIndex]->wCards[i];
            int eCard = games[gameIndex]->eCards[i];
            int nCard = games[gameIndex]->nCards[i];
            int sCard = games[gameIndex]->sCards[i];
            if ((wCard == ace) || (wCard == king) || (wCard == queen) ||
                    (wCard == jack) || (wCard == ten))
                west ++;
            if ((eCard == ace) || (eCard == king) || (eCard == queen) ||
                    (eCard == jack) || (eCard == ten))
                east ++;
            if ((nCard == ace) || (nCard == king) || (nCard == queen) ||
                    (nCard == jack) || (nCard == ten))
                north ++;
            if ((sCard == ace) || (sCard == king) || (sCard == queen) ||
                    (sCard == jack) || (sCard == ten))
                south ++;
        }
        if ((north == 5) || (south == 5))
            return 150;
        if ((west == 5) || (east == 5))
            return -150;
        if ((north == 4) || (south == 4))
            return 100;
        if ((west == 4) || (east == 4))
            return -100;
    }
    else
    {
        int aceS = SUIT_INTERVAL[SPADES][2];
        int aceH = SUIT_INTERVAL[HEARTS][2];
        int aceD = SUIT_INTERVAL[DIAMONDS][2];
        int aceC = SUIT_INTERVAL[CLUBS][2];

        int west, north, east, south;
        west = north = east = south = 0;

        for (int i = 0; i < 13; i++)
        {
            int wCard = games[gameIndex]->wCards[i];
            int eCard = games[gameIndex]->eCards[i];
            int nCard = games[gameIndex]->nCards[i];
            int sCard = games[gameIndex]->sCards[i];
            if ((wCard == aceS) || (wCard == aceH) || (wCard == aceD) || (wCard == aceC))
                west ++;
            if ((nCard == aceS) || (nCard == aceH) || (nCard == aceD) || (nCard == aceC))
                north ++;
            if ((eCard == aceS) || (eCard == aceH) || (eCard == aceD) || (eCard == aceC))
                east ++;
            if ((sCard == aceS) || (sCard == aceH) || (sCard == aceD) || (sCard == aceC))
                south ++;
        }
        if ((north == 4) || (south == 4))
            return 150;
        if ((west == 4) || (east == 4))
            return -150;
    }
    return 0;
}

ScoringMethod CGamesDoc::getScoringMethod()
{
    if (((scoringMethod == MP) || (scoringMethod == IMP)) && (getMaxNumberAuctionAndPlay() < 2))
        return PRACTICE;

    return scoringMethod;
}

/**
 * @brief Calculate rubber points for a given game/play.
 * @param[in] gameIndex Index of the game.
 * @param[out] nsAbove Above points for NS until now.
 * @param[out] nsBelow Below points for NS until now.
 * @param[out] nsTotal Total points for NS until now.
 * @param[out] nsLedger Ledger points for NS until now.
 * @param[out] ewAbove Above points for EW until now.
 * @param[out] ewBelow Below points for EW until now.
 * @param[out] ewTotal Total points for EW until now.
 * @param[out]  ewLedger Ledger points for EW until now.
 * @return True if NS or EW just won a rubber. Otherwise false.
 */
bool CGamesDoc::getRubberPoints(int gameIndex, bool *gameDone,
                                int *board, Bids *contract, Bids *contractModifier,
                                int *tricks, Seat *declarer, Team *vulnerable,
                                int *nsAbove, int *nsBelow, int *nsTotal, int *nsLedger,
                                int *ewAbove, int *ewBelow, int *ewTotal, int *ewLedger)
{
    assert(gameIndex < games.size());

    int auctionAndPlayIndex = getPlayedAuctionAndPlayIndex(gameIndex);

    int belowTheLineNSPoint, belowTheLineEWPoint, aboveTheLineNSPoint, aboveTheLineEWPoint;

    int belowTheLineNSTotal = 0, belowTheLineEWTotal = 0;
    int aboveTheLineNSTotal = 0, aboveTheLineEWTotal = 0;
    int belowTheLineNSLedger = 0, belowTheLineEWLedger = 0;
    int aboveTheLineNSLedger = 0, aboveTheLineEWLedger = 0;

    int belowTheLineNSGamePoint = 0, belowTheLineEWGamePoint = 0;
    int belowTheLineNSGame = 0, belowTheLineEWGame = 0;
    bool game = false, rubberDone = false;

    //Begin with the first play.
    for (int i = 0; i <= gameIndex; i++)
    {
        //Assure initialized for each play.
        belowTheLineNSPoint = belowTheLineEWPoint = aboveTheLineNSPoint = aboveTheLineEWPoint = 0;

        //Reset when a rubber (NS or EW) is done.
        if (rubberDone)
        {
            belowTheLineNSTotal = belowTheLineEWTotal = 0;
            aboveTheLineNSTotal = aboveTheLineEWTotal = 0;
            rubberDone = false;
        }
        if (game)
            game = false;

        //Calculation related to below the line points.
        int belowTheLinePoint = getBelowTheLine(i);
        if (belowTheLinePoint > 0)
        {
            //Update NS below the line points.
            belowTheLineNSPoint = belowTheLinePoint;
            belowTheLineNSGamePoint += belowTheLinePoint;
            belowTheLineNSTotal += belowTheLinePoint;
            belowTheLineNSLedger += belowTheLinePoint;
            if (belowTheLineNSGamePoint >= 100)
            {
                //Rubber game for NS.
                game = true;
                belowTheLineNSGame++;
                if (belowTheLineNSGame == 2)
                {
                    //Rubber is done. Update NS above the line points with rubber bonus points
                    aboveTheLineNSPoint += (belowTheLineEWGame == 0) ? (700) : (500);
                    aboveTheLineNSTotal += (belowTheLineEWGame == 0) ? (700) : (500);
                    aboveTheLineNSLedger += (belowTheLineEWGame == 0) ? (700) : (500);
                    belowTheLineNSGame = belowTheLineEWGame = 0;
                    rubberDone = true;
                }
                //Initialize for new rubber game (NS or EW).
                belowTheLineNSGamePoint = belowTheLineEWGamePoint = 0;
            }
        }
        else if (belowTheLinePoint < 0)
        {
            //Update EW below the line points.
            belowTheLineEWPoint = -belowTheLinePoint;
            belowTheLineEWGamePoint -= belowTheLinePoint;
            belowTheLineEWTotal -= belowTheLinePoint;
            belowTheLineEWLedger -= belowTheLinePoint;
            if (belowTheLineEWGamePoint >= 100)
            {
                //Rubber game for EW.
                game = true;
                belowTheLineEWGame++;
                if (belowTheLineEWGame == 2)
                {
                    //Rubber is done. Update EW above the line points with rubber bonus points
                    aboveTheLineEWPoint += (belowTheLineNSGame == 0) ? (700) : (500);
                    aboveTheLineEWTotal += (belowTheLineNSGame == 0) ? (700) : (500);
                    aboveTheLineEWLedger += (belowTheLineNSGame == 0) ? (700) : (500);
                    belowTheLineNSGame = belowTheLineEWGame = 0;
                    rubberDone = true;
                }
                //Initialize for new rubber game (NS or EW).
                belowTheLineNSGamePoint = belowTheLineEWGamePoint = 0;
            }
        }

        //Calculation related to above the line points.
        int aboveTheLinePoint = getAboveTheLine(i);
        if (aboveTheLinePoint > 0)
        {
            //Update NS above the line points.
            aboveTheLineNSPoint += aboveTheLinePoint;
            aboveTheLineNSTotal += aboveTheLinePoint;
            aboveTheLineNSLedger += aboveTheLinePoint;
        }
        else if (aboveTheLinePoint < 0)
        {
            //Update EW above the line points.
            aboveTheLineEWPoint -= aboveTheLinePoint;
            aboveTheLineEWTotal -= aboveTheLinePoint;
            aboveTheLineEWLedger -= aboveTheLinePoint;
        }

        //Calculation related to honor bonus.
        int honorBonus = getHonorBonus(i, auctionAndPlayIndex);
        if (honorBonus > 0)
        {
            //Update NS above the line points.
            aboveTheLineNSPoint += honorBonus;
            aboveTheLineNSTotal += honorBonus;
            aboveTheLineNSLedger += honorBonus;
        }
        else if (honorBonus < 0)
        {
            //Update EW above the line points.
            aboveTheLineEWPoint -= honorBonus;
            aboveTheLineEWTotal -= honorBonus;
            aboveTheLineEWLedger -= honorBonus;
        }
    }

    //Return values.
    *gameDone = game;
    *board = games[gameIndex]->board;
    *contract = BID_NONE;
    *contractModifier = BID_NONE;
    *tricks = -1;
    *declarer = NO_SEAT;
    if (auctionAndPlayIndex != -1)
    {
        *contract = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->contract;
        *contractModifier = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->contractModifier;
        *tricks = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->result;
        *declarer = games[gameIndex]->auctionAndPlay[auctionAndPlayIndex]->declarer;
    }
    *vulnerable = games[gameIndex]->vulnerable;
    *nsAbove = aboveTheLineNSPoint;
    *ewAbove = aboveTheLineEWPoint;
    *nsBelow = belowTheLineNSPoint;
    *ewBelow = belowTheLineEWPoint;
    *nsTotal = aboveTheLineNSTotal + belowTheLineNSTotal;
    *ewTotal = aboveTheLineEWTotal + belowTheLineEWTotal;
    *nsLedger = aboveTheLineNSLedger + belowTheLineNSLedger;
    *ewLedger = aboveTheLineEWLedger + belowTheLineEWLedger;

    return rubberDone;
}

/**
 * @brief Read games from a pbn file.
 * @param[in] pbnText The stream to read from.
 * @param[in] event The event to read.
 * @param[in] originalGames If true read original games. if false read played games.
 *
 * Reads either original or played games. In case of played games then each game has been played
 * maximum two times. The sequence of played games are always the same. The first play for a game is
 * manually played and the second play is auto played.
 */
void CGamesDoc::readGames(QTextStream &pbnText, QString &event, bool originalGames) throw(PlayException)
{
    int numLines;
    PBNContext context;
    QStringList strLines;
    QMap<QString, tagIds> tagName;
    QMap<tagIds, QString> tagMap;
    QMap<int, QString> auctionNotes;
    QMap<int, QString> playNotes;
    int actorNo, noPasses, trickNo, gameNo;
    Seat currentLeader;
    int oneTrick[4];

    CAuctionAndPlay *auctionAndPlay = 0;
    CGame *game = 0;

    //Map of pbn tag names.
    tagName["EVENT"] = TAG_EVENT;
    tagName["SITE"] = TAG_SITE;
    tagName["DATE"] = TAG_DATE;
    tagName["ROUND"] = TAG_ROUND;
    tagName["BOARD"] = TAG_BOARD;
    tagName["WEST"] = TAG_WEST;
    tagName["NORTH"] = TAG_NORTH;
    tagName["EAST"] = TAG_EAST;
    tagName["SOUTH"] = TAG_SOUTH;
    tagName["DEALER"] = TAG_DEALER;
    tagName["VULNERABLE"] = TAG_VULNERABLE;
    tagName["DEAL"] = TAG_DEAL;
    tagName["SCORING"] = TAG_SCORING;
    tagName["DECLARER"] = TAG_DECLARER;
    tagName["CONTRACT"] = TAG_CONTRACT;
    tagName["RESULT"] = TAG_RESULT;
    tagName["AUCTION"] = TAG_AUCTION;
    tagName["PLAY"] = TAG_PLAY;

    //Initial pbn tag values.
    tagMap[TAG_EVENT] = "";
    tagMap[TAG_SITE] = "";
    tagMap[TAG_DATE] = "";
    tagMap[TAG_ROUND] = "";
    tagMap[TAG_BOARD] = "";
    tagMap[TAG_WEST] = "";
    tagMap[TAG_NORTH] = "";
    tagMap[TAG_EAST] = "";
    tagMap[TAG_SOUTH] = "";
    tagMap[TAG_SCORING] = "";
    tagMap[TAG_RESULT] = "";

    //Read and preprocess pbn file (remove comments etc.).
    numLines = preloadPBNFile(pbnText, event, strLines, auctionNotes, playNotes);

    //Process one line at a time.
    gameNo = 0;
    context = TAG_CONTEXT;
    QListIterator<QString> lineItr(strLines);
    bool endOfFile = false;
    bool hasNext;
    while ((hasNext = lineItr.hasNext()) || !endOfFile)
    {
        QString currentLine;
        if (hasNext)
            currentLine = lineItr.next().trimmed();
        else
            endOfFile = true;

        //Check for end of game (blank line - or could be end of file).
        if (currentLine.isEmpty())
        {
            gameNo++;
            //Check for multiple empty lines or end of file.
            if (game != 0)
            {
                //Check for already found game, but new auction and play.
                QListIterator<CGame *> gameItr(games);
                int gameInx = -1;
                while (gameItr.hasNext())
                {
                    gameInx++;
                    CGame *nextGame = gameItr.next();
                    if (nextGame->board == game->board)
                    {
                        //Check redundant info (cards etc.).
                        bool error = false;
                        for (int i = 0; i < 13; i++)
                            if ((nextGame->wCards[i] != game->wCards[i]) || (nextGame->nCards[i] != game->nCards[i]) ||
                                    (nextGame->eCards[i] != game->eCards[i]) || (nextGame->sCards[i] != game->sCards[i]))
                                error = true;
                        if ((nextGame->dealer != game->dealer) || error)
                            throw PlayException(QString("PBN - Illegal redundant info in games with board: %1").arg(game->board).toStdString());

                        if (!auctionAndPlay->westName.isEmpty() && !auctionAndPlay->northName.isEmpty() &&
                                !auctionAndPlay->eastName.isEmpty() && !auctionAndPlay->southName.isEmpty())
                        {
                            GameType gameType = ORIGINAL_GAME;
                            if (!originalGames)
                            {
                                //In case we are processing (by this program) played games, then automatically                                //is a manually played game and the next (if existing) is an automatically
                                //played games are identified by the same prefix in all actor names.
                                gameType = PLAYED_GAME;
                                if ((auctionAndPlay->westName.indexOf(AUTO_SEAT_NAME_PREFIX) == 0) &&
                                    (auctionAndPlay->northName.indexOf(AUTO_SEAT_NAME_PREFIX) == 0) &&
                                    (auctionAndPlay->eastName.indexOf(AUTO_SEAT_NAME_PREFIX) == 0) &&
                                    (auctionAndPlay->southName.indexOf(AUTO_SEAT_NAME_PREFIX) == 0))
                                    gameType = AUTO_GAME;
                                currentGameIndex = gameInx + 1;
                            }
                            auctionAndPlay->gameType = gameType;
                            nextGame->auctionAndPlay.append(auctionAndPlay);
                        }
                        else
                            delete auctionAndPlay;

                        delete game;
                        game = 0;
                        break;
                    }
                }
            }

            //New game?
            if ((game != 0) && (game->board != -1))
            {
                if (!auctionAndPlay->westName.isEmpty() && !auctionAndPlay->northName.isEmpty() &&
                        !auctionAndPlay->eastName.isEmpty() && !auctionAndPlay->southName.isEmpty())
                {
                    if (originalGames)
                        auctionAndPlay->gameType = ORIGINAL_GAME;
                    else if ((auctionAndPlay->westName.indexOf(AUTO_SEAT_NAME_PREFIX) == 0) &&
                        (auctionAndPlay->northName.indexOf(AUTO_SEAT_NAME_PREFIX) == 0) &&
                        (auctionAndPlay->eastName.indexOf(AUTO_SEAT_NAME_PREFIX) == 0) &&
                        (auctionAndPlay->southName.indexOf(AUTO_SEAT_NAME_PREFIX) == 0))
                        auctionAndPlay->gameType = AUTO_GAME;
                    else
                        auctionAndPlay->gameType = PLAYED_GAME;
                    game->auctionAndPlay.append(auctionAndPlay);

                    //Update the current game index to the next game. We do not want to play already played games.
                    if (!originalGames)
                        currentGameIndex = games.size();
                }
                else
                    delete auctionAndPlay;

                games.append(game);
                game = 0;
            }

            //Not accepted?
            if (game != 0)
            {
                delete game;
                game = 0;
                delete auctionAndPlay;
            }
            context = TAG_CONTEXT;
        }

        //Process line.
        else
        {
            if (((context == AUCTION_CONTEXT) || (context == PLAY_CONTEXT)) &&
                 (currentLine.indexOf("[", 0, Qt::CaseInsensitive) == 0))
                context = TAG_CONTEXT;

            //There are three contexts. In auction context we are processing auction lines. In play context
            //we are processing play lines. Otherwise (in tag context) we are processing tag lines.
            switch (context)
            {
            //We are processing auction information.
            case AUCTION_CONTEXT:
            {
                Seat actor;
                Bids bidCall;
                int note;
                int inx = 0;
                int inxNext;

                do
                {
                    //Get next bid.
                    if ((inxNext = getBid(currentLine, inx, &bidCall)) != inx)
                    {
                        if (bidCall == BID_BLANK)       //*  (end of auction)
                        {
                            context = TAG_CONTEXT;
                            break;
                        }
                        else if (bidCall == BID_PLAYER) //AP (All Passes)
                        {
                            //Append 3 consecutive passes.
                            bidCall = BID_PASS;
                            for (int i = noPasses; i < 3; i++)
                            {
                                actor = (Seat)((game->dealer + actorNo) % 4);
                                CBid bid(actor, bidCall, "");
                                actorNo++;
                                auctionAndPlay->bidHistory.appendBid(bid);
                            }
                            context = TAG_CONTEXT;
                            break;
                        }

                        inx = inxNext;
                        QString alert;
                        actor = (Seat)((game->dealer + actorNo) % 4);
                        actorNo++;
                        //Check for notes.
                        if ((inxNext = getNote(currentLine, inx, &note)) != inx)
                        {
                            inx = inxNext;
                            alert = auctionNotes[note + gameNo*32];
                        }
                        CBid bid(actor, bidCall, alert);
                        auctionAndPlay->bidHistory.appendBid(bid);
                        //Check for pass.
                        if (bidCall == BID_PASS)
                        {
                            noPasses++;
                            //Finished?
                            if (noPasses == 3)          //3 consecutive passes ends auction.
                            {
                                context = TAG_CONTEXT;
                                break;
                            }
                        }
                        else
                            noPasses = 0;
                    }
                    else if (bidCall == BID_NONE)       //Not a valid bid.
                        throw PlayException((QString("PBN - Illegal bid: %1 %2 %3").arg(currentLine).arg(" in board: ").arg(game->board)).toStdString());

                } while (inxNext != currentLine.size());
            }
                break;

            //We are processing play information.
            case PLAY_CONTEXT:
            {
                int playCall;
                int note;
                int inx = 0;
                int inxNext;

                do
                {
                    //Get next play.
                    if ((inxNext = getPlay(currentLine, inx, &playCall)) != inx)
                    {
                        if ((playCall == -2) || (playCall == -3))       //* or -  (end of play or not played)
                        {
                            context = TAG_CONTEXT;
                            break;
                        }

                        //Check for notes.
                        inx = inxNext;
                        QString alert;
                        if ((inxNext = getNote(currentLine, inx, &note)) != inx)
                        {
                            inx = inxNext;
                            alert = playNotes[note + gameNo*32];
                        }
                        //Save play.
                        oneTrick[actorNo++] = playCall;

                        //Have all (4) actors played (we skip the last trick if it is incomplete)?
                        if (actorNo == 4)
                        {
                            //Save play.
                            for (int i = 0; i < 4; i++)
                            {
                                Seat seat = (Seat)((currentLeader + i) % 4);
                                int *cards = (seat == WEST_SEAT) ? (game->wCards) :
                                             (seat == NORTH_SEAT) ? (game->nCards) :
                                             (seat == EAST_SEAT) ? (game->eCards) : (game->sCards);
                                int trickInx = (4 + seat - (auctionAndPlay->declarer + 1)) % 4;
                                  if (auctionAndPlay->playHistory.cardOk(oneTrick[trickInx], seat, cards))
                                    auctionAndPlay->playHistory.setPlay(seat, trickNo, oneTrick[trickInx]);
                                else
                                    throw PlayException((QString("PBN - Illegal play: %1 %2 %3").arg(currentLine).arg(" in board: ").arg(game->board)).toStdString());
                            }
                            //Update to next trick.
                            currentLeader = auctionAndPlay->playHistory.getNextLeader();
                            actorNo = 0;
                            trickNo++;
                            //Finished?
                            if (trickNo == 13)
                            {
                                context = TAG_CONTEXT;
                                break;
                            }
                        }
                    }
                    else if (playCall == -1)       //Not a valid play.
                        throw PlayException((QString("PBN - Illegal play: %1 %2 %3").arg(currentLine).arg(" in board: ").arg(game->board)).toStdString());

                } while (inxNext != currentLine.size());
            }
                break;

            //We are processing tags.
            case TAG_CONTEXT:
            {
                QString strValue;

                //Parse for relevant tags. Skip empty and non relevant tags.
                tagIds tag = parsePBNLine(currentLine, strValue, tagName);
                if ((tag == TAG_NONE) || strValue.isEmpty() || (strValue == "?"))
                    continue;

                //The tag is relevant.
                //Check for # and ##.
                if (tagMap.contains(tag))
                {
                    if (strValue.indexOf("## ") == 0)
                        strValue = strValue.mid(3);
                    else if (strValue.indexOf("#") == 0)
                        strValue = tagMap[tag];
                    tagMap[tag] = strValue;
                }

                //Allocate new game object (at the first line of the new board)?
                if (game == 0)
                {
                    game = new CGame;
                    game->board = -1;
                    game->dealer = NO_SEAT;
                    game->vulnerable = NONE;
                    for (int i = 0; i < 13; i++)
                        game->wCards[i] = game->nCards[i] = game->eCards[i] = game->sCards[i] = -1;
                    auctionAndPlay = new CAuctionAndPlay;
                    auctionAndPlay->declarer = NO_SEAT;
                    auctionAndPlay->contract = BID_NONE;
                    auctionAndPlay->contractModifier = BID_NONE;
                    auctionAndPlay->result = -1;
                }

                //Process tag.
                switch (tag)
                {
                //Board tag.
                case TAG_BOARD:
                {
                    int board = strValue.toInt();
                    if (board > 0)
                        game->board = board;
                    else
                        throw PlayException(QString("PBN - Illegal board value: %1").arg(strValue).toStdString());
                }
                    break;

                //Name of west player.
                case TAG_WEST:
                    auctionAndPlay->westName = strValue;
                    break;

                //Name of north player.
                case TAG_NORTH:
                    auctionAndPlay->northName = strValue;
                    break;

                //Name of east player.
                case TAG_EAST:
                    auctionAndPlay->eastName = strValue;
                    break;

                //Name of south player.
                case TAG_SOUTH:
                    auctionAndPlay->southName = strValue;
                    break;

                //Dealer tag.
                case TAG_DEALER:
                    if (QString::compare(strValue, "W", Qt::CaseInsensitive) == 0)
                        game->dealer = WEST_SEAT;
                    else if (QString::compare(strValue, "N", Qt::CaseInsensitive) == 0)
                        game->dealer = NORTH_SEAT;
                    else if (QString::compare(strValue, "E", Qt::CaseInsensitive) == 0)
                        game->dealer = EAST_SEAT;
                    else if (QString::compare(strValue, "S", Qt::CaseInsensitive) == 0)
                        game->dealer = SOUTH_SEAT;
                    else
                    throw PlayException(QString("PBN - Illegal dealer: %1 %2 %3").arg(strValue).arg(" in board: ").arg(game->board).toStdString());
                    break;

                //Vulnerable tag.
                case TAG_VULNERABLE:
                    if ((QString::compare(strValue, "NONE", Qt::CaseInsensitive) == 0) ||
                        (QString::compare(strValue, "LOVE", Qt::CaseInsensitive) == 0) ||
                        (QString::compare(strValue, "-", Qt::CaseInsensitive) == 0))
                        game->vulnerable = NEITHER;
                    else if (QString::compare(strValue, "NS", Qt::CaseInsensitive) == 0)
                        game->vulnerable = NORTH_SOUTH;
                    else if (QString::compare(strValue, "EW", Qt::CaseInsensitive) == 0)
                        game->vulnerable = EAST_WEST;
                    else if ((QString::compare(strValue, "ALL", Qt::CaseInsensitive) == 0) ||
                             (QString::compare(strValue, "BOTH", Qt::CaseInsensitive) == 0))
                        game->vulnerable = BOTH;
                    else
                        throw PlayException(QString("PBN - Illegal vulnerability: %1 %2 %3").arg(strValue).arg(" in board: ").arg(game->board).toStdString());
                    break;

                //Deal tag.
                case TAG_DEAL:
                {
                    Seat dealer = getCards(strValue, game->wCards, game->nCards, game->eCards, game->sCards, game->board);
                    if (game->dealer == NO_SEAT)
                        game->dealer = dealer;

//                    if (dealer != game->dealer)
//                        throw PlayException(QString("PBN - Illegal dealer: %1 %2 %3").arg(dealer).arg(" in board: ").arg(game->board).toStdString());
                }
                    break;

                //Scoring tag.
                case TAG_SCORING:
                {
                    ScoringMethod currentScoringMethod;
                    if (QString::compare(strValue, "IMP", Qt::CaseInsensitive) == 0)
                        currentScoringMethod = IMP;
                    else if (QString::compare(strValue, "MP", Qt::CaseInsensitive) == 0)
                        currentScoringMethod = MP;
                    else if (QString::compare(strValue, "RUBBER", Qt::CaseInsensitive) == 0)
                        currentScoringMethod = RUBBER;
                    else if (QString::compare(strValue, "PRACTICE", Qt::CaseInsensitive) == 0)
                        currentScoringMethod = PRACTICE;
                    else
                        currentScoringMethod = FORSCORE;

                    if ((scoringMethod != NOSCORE) && (currentScoringMethod != scoringMethod))
                        throw PlayException(QString("PBN - Illegal scoring method: %1 %2 %3").arg(currentScoringMethod).arg(" in board: ").arg(game->board).toStdString());

                    scoringMethod = currentScoringMethod;
                }
                    break;


                //Declarer tag.
                case TAG_DECLARER:
                    if (QString::compare(strValue, "W", Qt::CaseInsensitive) == 0)
                        auctionAndPlay->declarer = WEST_SEAT;
                    else if (QString::compare(strValue, "N", Qt::CaseInsensitive) == 0)
                        auctionAndPlay->declarer = NORTH_SEAT;
                    else if (QString::compare(strValue, "E", Qt::CaseInsensitive) == 0)
                        auctionAndPlay->declarer = EAST_SEAT;
                    else if (QString::compare(strValue, "S", Qt::CaseInsensitive) == 0)
                        auctionAndPlay->declarer = SOUTH_SEAT;
                    else
                        auctionAndPlay->declarer = NO_SEAT;
                    break;

                //Contract tag.
                case TAG_CONTRACT:
                    if (QString::compare(strValue, "PASS", Qt::CaseInsensitive) == 0)
                    {
                        auctionAndPlay->contract = BID_PASS;
                        auctionAndPlay->contractModifier = BID_NONE;
                    }
                    else
                    {
                        int level;
                        Suit suit;
                        int size = strValue.size();
                        int n = 0;
                        if ((size > 0) && (strValue[0].isDigit())) n = 1;
                        if ((n = 1) && (size > 1) && (strValue[1].isDigit())) n = 2;
                        if (n == 0)
                            throw PlayException(QString("PBN - Illegal number of tricks in contract: %1 %2 %3").arg(strValue).arg(" in board: ").arg(game->board).toStdString());

                        level = strValue.mid(0, n).toInt();
                        if ((level < 1) || (level > 7) || (size <= n))
                            throw PlayException(QString("PBN - Illegal format of contract: %1 %2 %3").arg(strValue).arg(" in board: ").arg(game->board).toStdString());

                        int m = 1;
                        if (QString::compare(strValue.mid(n, 1), "S", Qt::CaseInsensitive) == 0)
                            suit = SPADES;
                        else if (QString::compare(strValue.mid(n, 1), "H", Qt::CaseInsensitive) == 0)
                            suit = HEARTS;
                        else if (QString::compare(strValue.mid(n, 1), "D", Qt::CaseInsensitive) == 0)
                            suit = DIAMONDS;
                        else if (QString::compare(strValue.mid(n, 1), "C", Qt::CaseInsensitive) == 0)
                            suit = CLUBS;
                        else if (QString::compare(strValue.mid(n, 2), "NT", Qt::CaseInsensitive) == 0)
                        { suit = NOTRUMP; m = 2;}
                        else
                            throw PlayException(QString("PBN - Illegal suit in contract: %1 %2 %3").arg(strValue).arg(" in board: ").arg(game->board).toStdString());

                        auctionAndPlay->contract = MAKE_BID(suit, level);

                        if (size == (n + m))
                            auctionAndPlay->contractModifier = BID_NONE;
                        else if ((size == (n + m + 1)) &&
                                 (QString::compare(strValue.mid(n + m, 1), "X", Qt::CaseInsensitive) == 0))
                            auctionAndPlay->contractModifier = BID_DOUBLE;
                        else if ((size == (n + m + 2)) &&
                                 (QString::compare(strValue.mid(n + m, 2), "XX", Qt::CaseInsensitive) == 0))
                            auctionAndPlay->contractModifier = BID_REDOUBLE;
                        else
                            throw PlayException(QString("PBN - Illegal contract modifier in contract: %1 %2 %3").arg(strValue).arg(" in board: ").arg(game->board).toStdString());
                    }
                    break;

                //Result tag.
                case TAG_RESULT:
                {
                    for (int i = 0; i < strValue.size(); i++)
                        if (!strValue[i].isDigit())
                            throw PlayException(QString("PBN - Illegal result: %1 %2 %3").arg(strValue).arg(" in board: ").arg(game->board).toStdString());
                    if (strValue.size() == 0)
                        auctionAndPlay->result = -1;
                    else
                        auctionAndPlay->result = strValue.toInt();
                }
                    break;

                //Auction tag.
                case TAG_AUCTION:
                {
                    Seat dealer;

                    if (QString::compare(strValue, "W", Qt::CaseInsensitive) == 0)
                        dealer = WEST_SEAT;
                    else if (QString::compare(strValue, "N", Qt::CaseInsensitive) == 0)
                        dealer = NORTH_SEAT;
                    else if (QString::compare(strValue, "E", Qt::CaseInsensitive) == 0)
                        dealer = EAST_SEAT;
                    else if (QString::compare(strValue, "S", Qt::CaseInsensitive) == 0)
                        dealer = SOUTH_SEAT;
                    else
                        throw PlayException(QString("PBN - Illegal dealer: %1 %2 %3").arg(strValue).arg(" in board: ").arg(game->board).toStdString());

                    if (game->dealer == NO_SEAT)
                        game->dealer = dealer;
                    if (dealer != game->dealer)
                        throw PlayException(QString("PBN - Illegal dealer: %1 %2 %3").arg(strValue).arg(" in board: ").arg(game->board).toStdString());

                    auctionAndPlay->bidHistory.resetBidHistory();

                    actorNo = 0;
                    noPasses = 0;
                    context = AUCTION_CONTEXT;
                    break;
                }

                //Play tag.
                case TAG_PLAY:
                {
                    Seat leader;

                    if (QString::compare(strValue, "W", Qt::CaseInsensitive) == 0)
                        leader = WEST_SEAT;
                    else if (QString::compare(strValue, "N", Qt::CaseInsensitive) == 0)
                        leader = NORTH_SEAT;
                    else if (QString::compare(strValue, "E", Qt::CaseInsensitive) == 0)
                        leader = EAST_SEAT;
                    else if (QString::compare(strValue, "S", Qt::CaseInsensitive) == 0)
                        leader = SOUTH_SEAT;
                    else
                        throw PlayException(QString("PBN - Illegal leader: %1 %2 %3").arg(strValue).arg(" in board: ").arg(game->board).toStdString());

                    if (auctionAndPlay->declarer == NO_SEAT)
                        auctionAndPlay->declarer = (Seat)((leader + 4 - 1) % 4);

                     currentLeader = (Seat)((auctionAndPlay->declarer + 1) % 4);
                    if (leader != currentLeader)
                        throw PlayException(QString("PBN - Illegal leader: %1 %2 %3").arg(strValue).arg(" in board: ").arg(game->board).toStdString());

                    auctionAndPlay->playHistory.setBidInfo(auctionAndPlay->contract, auctionAndPlay->contractModifier,
                                                           currentLeader);
                    actorNo = 0;
                    trickNo = 0;
                    context = PLAY_CONTEXT;
                    break;
                }

                //Not used tags.
                default:
                    break;
                }
                break;
            }

            default:
                break;
            }
        }
    }

    //Check consistency of original pbn file (played/auto games are not read yet).
    if (originalGames)
    {
        //First check for scoring method.
        if (games.size() > 0)
        {
            //Scoring method are not always explicitly stated in pbn files.
            int noPlays = games[0]->auctionAndPlay.size();
            if ((scoringMethod == NOSCORE) && (noPlays >= 1))
                scoringMethod = ((noPlays == 2) || (noPlays == 1)) ? (IMP) : (MP);

            //Not known scoring method (and rubber) does not use auction and play.
            if ((scoringMethod == FORSCORE) || (scoringMethod == RUBBER))
            {
                for (int i = 0; i < games.size(); i++)
                    while (!games[i]->auctionAndPlay.isEmpty())
                        delete(game->auctionAndPlay.takeFirst());
                scoringMethod = NOSCORE;
            }
        }

        //Consistency check.
        QListIterator<CGame *> gameItr(games);
        while (gameItr.hasNext())
        {
            //Check proper board, dealer and vulnerability.
            CGame *nextGame = gameItr.next();
            if ((nextGame->board <= 0) || (nextGame->dealer == NO_SEAT) ||
                    (nextGame->vulnerable == NONE))
                throw PlayException(QString("PBN - Consistency error in original games (Dealer, Board or vulnerability) in board: %1").arg(nextGame->board).toStdString());

            //Check cards.
            for (int i = 0; i < 13; i++)
                if ((nextGame->wCards[i] < 0) || (nextGame->nCards[i] < 0) ||
                        (nextGame->eCards[i] < 0) || (nextGame->sCards[i] < 0))
                    throw PlayException(QString("PBN - Consistency error in original games (Cards missing in Deal) in board: %1").arg(nextGame->board).toStdString());

            //Check auction and play.
            QListIterator<CAuctionAndPlay *> auctionAndPlayItr(nextGame->auctionAndPlay);
            while (auctionAndPlayItr.hasNext())
            {
                CAuctionAndPlay *nextAuctionAndPlay = auctionAndPlayItr.next();
                if (((nextAuctionAndPlay->declarer == NO_SEAT) && (nextAuctionAndPlay->contract != BID_PASS)) ||
                        (nextAuctionAndPlay->contract == BID_NONE) ||
                        ((nextAuctionAndPlay->result < 0) && (nextAuctionAndPlay->contract != BID_PASS)))
                    throw PlayException(QString("PBN - Consistency error in original games (auction and play) in board: %1").arg(nextGame->board).toStdString());
            }
        }
    }

    //Check played and auto games (original games, played/auto games are all read now).
    else if (getNumberPlayedGames() > 0)
    {
        //Are there any originals.
        QListIterator<CGame *> gameItr(games);
        bool found = false;
        while (gameItr.hasNext())
        {
            CGame *nextGame = gameItr.next();
            if ((nextGame->auctionAndPlay.size() > 0) && (nextGame->auctionAndPlay[0]->gameType == ORIGINAL_GAME))
                found = true;
        }

        //If there are originals then there must be originals in all.
        if (found)
        {
            gameItr.toFront();
            while (gameItr.hasNext())
            {
                CGame *nextGame = gameItr.next();

                //Is there an inconsistency (no original)?
                if ((nextGame->auctionAndPlay.size() == 0) ||
                        ((nextGame->auctionAndPlay.size() > 0) && (nextGame->auctionAndPlay[0]->gameType != ORIGINAL_GAME)))
                    throw PlayException(QString("PBN - Consistency error in played games (no original) in board: %1").arg(nextGame->board).toStdString());
            }
        }

        //If no originals then dealtype is random. Check scoring methor, board, dealer, vulnerability and cards.
        else
        {
            //Scoring method are always given for played games.
            if ((games.size() > 0) && ((scoringMethod == NOSCORE) || (scoringMethod == FORSCORE)))
                throw PlayException(QString("PBN - Consistency error in played games (no scoring method)").toStdString());

            gameItr.toFront();
            while (gameItr.hasNext())
            {
                //Check proper board, dealer and vulnerability.
                CGame *nextGame = gameItr.next();
                if ((nextGame->board <= 0) || (nextGame->dealer == NO_SEAT) ||
                        (nextGame->vulnerable == NONE))
                    throw PlayException(QString("PBN - Consistency error in played games (Dealer, Board or Vulnerability) in board: %1").arg(nextGame->board).toStdString());

                //Check cards.
                for (int i = 0; i < 13; i++)
                    if ((nextGame->wCards[i] < 0) || (nextGame->nCards[i] < 0) ||
                            (nextGame->eCards[i] < 0) || (nextGame->sCards[i] < 0))
                        throw PlayException(QString("PBN - Consistency error in played games (Cards missing in Deal) in board: %1").arg(nextGame->board).toStdString());
            }
        }

        //Check consistency of played/auto games. Has been checked for original games.
        gameItr.toFront();
        QString pW, pN, pE, pS, aW, aN, aE, aS;
        while (gameItr.hasNext())
        {
            CGame *nextGame = gameItr.next();
            QListIterator<CAuctionAndPlay *> auctionAndPlayItr(nextGame->auctionAndPlay);
            bool foundPlay, foundAuto;
            foundPlay = foundAuto = false;
            while (auctionAndPlayItr.hasNext())
            {
                CAuctionAndPlay *nextAuctionAndPlay = auctionAndPlayItr.next();
                if (nextAuctionAndPlay->gameType == ORIGINAL_GAME)
                    continue;

                if ((foundPlay &&(nextAuctionAndPlay->gameType == PLAYED_GAME)) ||
                    (foundAuto &&(nextAuctionAndPlay->gameType == AUTO_GAME)))
                    throw PlayException(QString("PBN - Consistency error in played games (auction and play) in board: %1").arg(nextGame->board).toStdString());
                if (nextAuctionAndPlay->gameType == PLAYED_GAME)
                {
                    foundPlay = true;
                    if (pW == "")
                    {
                        pW = nextAuctionAndPlay->westName;
                        pN = nextAuctionAndPlay->northName;
                        pE = nextAuctionAndPlay->eastName;
                        pS = nextAuctionAndPlay->southName;
                    }
                    else if ((pW != nextAuctionAndPlay->westName) ||
                             (pN != nextAuctionAndPlay->northName) ||
                             (pE != nextAuctionAndPlay->eastName) ||
                             (pS != nextAuctionAndPlay->southName))
                        throw PlayException(QString("PBN - Consistency error in played games (auction and play) in board: %1").arg(nextGame->board).toStdString());
                }
                else if (nextAuctionAndPlay->gameType == AUTO_GAME)
                {
                    foundAuto = true;
                    if (aW == "")
                    {
                        aW = nextAuctionAndPlay->westName;
                        aN = nextAuctionAndPlay->northName;
                        aE = nextAuctionAndPlay->eastName;
                        aS = nextAuctionAndPlay->southName;
                    }
                    else if ((aW != nextAuctionAndPlay->westName) ||
                             (aN != nextAuctionAndPlay->northName) ||
                             (aE != nextAuctionAndPlay->eastName) ||
                             (aS != nextAuctionAndPlay->southName))
                        throw PlayException(QString("PBN - Consistency error in played games (auction and play) in board: %1").arg(nextGame->board).toStdString());
                }
                else
                    throw PlayException(QString("PBN - Consistency error in played games (auction and play) in board: %1").arg(nextGame->board).toStdString());

                if (((nextAuctionAndPlay->declarer == NO_SEAT) && (nextAuctionAndPlay->contract != BID_PASS)) ||
                         (nextAuctionAndPlay->contract == BID_NONE) ||
                         ((nextAuctionAndPlay->result < 0) && (nextAuctionAndPlay->contract != BID_PASS)))
                    throw PlayException(QString("PBN - Consistency error in played games (auction and play) in board: %1").arg(nextGame->board).toStdString());
            }
        }
    }
}

/**
 * @brief Write a game in pbn format.
 * @param[in] stream The stream to write to.
 * @param[in] game The game to write.
 * @param[in] gameType The game type for the game.
 * @param[in] event The event.
 *
 * Writes the game including all plays as determined by the game type (original, played or auto).
 */
void CGamesDoc::writeGame(QTextStream &stream, CGame *game, GameType gameType, QString event)
{
    QString ev = event;
    QString line;

    //Random deals have no originals.
    if ((dealType == RANDOM_DEAL) && (gameType == ORIGINAL_GAME))
        return;

    //Are there any games with the required game type?
    QListIterator<CAuctionAndPlay *> auctionAndPlayItr(game->auctionAndPlay);
    bool found = false;
    while (auctionAndPlayItr.hasNext())
    {
        CAuctionAndPlay *nextAuctionAndPlay = auctionAndPlayItr.next();
        if (nextAuctionAndPlay->gameType == gameType)
            found = true;
    }
    auctionAndPlayItr.toFront();

    //Original games must always be written (even if not played).
    if (!found && (gameType == ORIGINAL_GAME))
    {
        stream << "[Event \"" << ev << "\"]\n";
        stream << QString("[Board \"%1\"]\n").arg(game->board).toLatin1();
        stream << QString("[Dealer \"%1\"]\n").arg(PBN_SEAT_NAMES[game->dealer]);
        stream << QString("[Vulnerable \"%1\"]\n").arg(PBN_VULNERABILITY_NAMES[game->vulnerable]);
        stream << setCards(game->dealer, game->wCards, game->nCards, game->eCards, game->sCards, line).toLatin1();
        stream << QString("[Scoring \"%1\"]\n\n").arg(PBN_SCORING_NAMES[scoringMethod]);
    }

    //Have we found games with the required game type?
    else if (found)
    {
        QListIterator<CAuctionAndPlay *> auctionAndPlayItr(game->auctionAndPlay);
        while (auctionAndPlayItr.hasNext())
        {
            CAuctionAndPlay *nextAuctionAndPlay = auctionAndPlayItr.next();
            if (nextAuctionAndPlay->gameType == gameType)
            {
                stream << "[Event \"" << ev << "\"]\n";
                ev = "#";       //Following events are written as inheritance of first event.
                stream << QString("[Board \"%1\"]\n").arg(game->board).toLatin1();
                stream << QString("[West \"%1\"]\n").arg(nextAuctionAndPlay->westName);
                stream << QString("[North \"%1\"]\n").arg(nextAuctionAndPlay->northName);
                stream << QString("[East \"%1\"]\n").arg(nextAuctionAndPlay->eastName);
                stream << QString("[South \"%1\"]\n").arg(nextAuctionAndPlay->southName);
                stream << QString("[Dealer \"%1\"]\n").arg(PBN_SEAT_NAMES[game->dealer]);
                stream << QString("[Vulnerable \"%1\"]\n").arg(PBN_VULNERABILITY_NAMES[game->vulnerable]);
                stream << setCards(game->dealer, game->wCards, game->nCards, game->eCards, game->sCards, line).toLatin1();
                stream << QString("[Scoring \"%1\"]\n").arg(PBN_SCORING_NAMES[scoringMethod]);
                if (nextAuctionAndPlay->contract == BID_PASS)
                {
                    stream << QString("[Declarer \"\"]\n");
                    stream << QString("[Contract \"PASS\"]\n");
                    stream << QString("[Result \"\"]\n");
                }
                else
                {
                    stream << QString("[Declarer \"%1\"]\n").arg(PBN_SEAT_NAMES[nextAuctionAndPlay->declarer]);
                    stream << setContract(nextAuctionAndPlay->contract, nextAuctionAndPlay->contractModifier, line);
                    stream << QString("[Result \"%1\"]\n").arg(nextAuctionAndPlay->result);
                }
                makeAuction(stream, nextAuctionAndPlay->bidHistory);
                makePlay(stream, nextAuctionAndPlay->playHistory);
                stream << QString("\n");
            }
        }
    }
}

/**
 * @brief Preload pbn file.
 * @param[in] PBNFile The text stream to read from.
 * @param[in] event The event to read.
 * @param[out] strLines The preprocessed lines read.
 * @param[out] auctionNotes Auction notes (used for alerts).
 * @param[out] playNotes Play notes (not used)
 * @return Number of lines after preprocessing.
 *
 * Preload PBN file, strip comments check for export tag and load only data for the given event.
 */
int CGamesDoc::preloadPBNFile(QTextStream &PBNFile, QString event, QStringList &strLines,
                              QMap<int, QString> &auctionNotes, QMap<int, QString> &playNotes)
{
    enum NoteType { AUCTION_NOTE, PLAY_NOTE};
    int numLinesRead;
    int gameNo;
    QString line;
    bool skipToNext, gameFound, inComment, commentFound, cont, exportFound, checkNote;
    NoteType noteType;
    QString beforeComment, afterComment;

    gameNo = 0;
    skipToNext = false;
    gameFound = false;
    exportFound = false;
    commentFound = false;
    inComment = false;
    numLinesRead = 0;
    while (!PBNFile.atEnd())
    {
        int inx, start;
        start = 0;
        line = PBNFile.readLine();

        //Take care of escape
        if (!inComment && line[0] == '%')
        {
            if (line.indexOf("% EXPORT", 0, Qt::CaseInsensitive) == 0)
                exportFound = true;
            continue;
        }

        //Remove comments.
        cont = true;
        while (cont)
        {
            cont = false;
            if (!inComment &&((inx = line.indexOf('{', start)) != -1))
            {
                commentFound = true;
                inComment = true;
                beforeComment += line.mid(start, inx - start);
                start = inx + 1;
                cont = true;
            }
            if (inComment && ((inx = line.indexOf('}', start)) != -1))
            {
                inComment = false;
                afterComment = line.mid(inx + 1);
                start = inx + 1;
                cont = true;
            }
        }

        if (!inComment)
        {
            if (commentFound)
            {
                commentFound = false;
                line = beforeComment + afterComment;
                beforeComment.clear();
                line = line.trimmed();
                if (line.isEmpty())
                    continue;
            }

            line = line.trimmed();

            //Skip lines that do not belong to the given event.
            if (skipToNext)
            {
                //Empty line signals end of event.
                if (line.isEmpty())
                    skipToNext = false;
                continue;
            }

            if (!gameFound)
            {
                //Check if the next game belongs to the given event.
                skipToNext = !searchGame(line, event);
                if (skipToNext)
                    continue;
                gameFound = true;
            }

            //Remove !.
            line.remove('!');

            // remove any question marks that are not preceded by a quote character
            // '?' characters may be used in tag values as well as used for quality ratings
            for(int i = 0; i < line.size(); i++)
            {
                if ((i > 0) && (line[i] == '?') && (line[i-1] != '\"'))
                {
                    line = line.left(i) + line.mid(i+1);
                    i--;
                }
            }

            //Remove whitespace.
            line = line.trimmed();

            //Empty line signals end of game. Start searching for next game for the given event.
            if (line.isEmpty())
            {
                gameNo++;
                gameFound = false;
                skipToNext = false;
            }

            //Check for auction note?
            else if (line.indexOf("[Auction", 0, Qt::CaseInsensitive) == 0)
            {
                checkNote = true;
                noteType = AUCTION_NOTE;
            }

            //Check for play note?
            else if (line.indexOf("[Play", 0, Qt::CaseInsensitive) == 0)
            {
                checkNote = true;
                noteType = PLAY_NOTE;
            }

            //Note found?
            else if ((checkNote) && (line.indexOf("[NOTE ", 0, Qt::CaseInsensitive) == 0))
            {
                //Retrieve note number and value.
                int firstQuote = line.indexOf('"');
                int lastQuote = line.lastIndexOf('"');
                int colon = line.indexOf(':');
                if ((colon > (firstQuote + 1)) && (colon < lastQuote))
                {
                    int inx = line.mid(firstQuote + 1, colon - firstQuote - 1).toInt();
                    if ((inx >= 1) && (inx <= 32))
                    {
                        QString strValue = line.mid(colon + 1, lastQuote - colon - 1);
                        if (noteType == AUCTION_NOTE)
                            auctionNotes.insert(inx + gameNo*32, strValue);
                        else
                            playNotes.insert(inx + gameNo*32, strValue);
                    }
                }
            }

            //Exit search for Note?
            else if (checkNote && (line.indexOf("[", 0, Qt::CaseInsensitive) == 0))
                    checkNote = false;

            //Append line if not a note.
            if (line.indexOf("[NOTE ", 0, Qt::CaseInsensitive) != 0)
            {
                //First remove NAG's.
                while (line.indexOf('$') != -1)
                {
                    int nagInx = line.indexOf('$');
                    QString partLine = line.mid(nagInx);
                    int i = 1;
                    while ((i < partLine.size()) && partLine[i].isDigit())
                       i++;
                    if (i != 1)
                        line = line.mid(0, nagInx) + line.mid(nagInx + i);
                }
                strLines.append(line);
                numLinesRead++;
            }
        }
    }

    return numLinesRead;
}

//Search for the given event in the given line. Also take care of header (##) and substitution (#).
bool CGamesDoc::searchGame(QString &line, QString &event)
{
    if (line.indexOf("[EVENT", 0, Qt::CaseInsensitive) != 0)
        return false;

    int firstInx = line.indexOf('"');
    int lastInx = line.lastIndexOf('"');

    //Check for syntax (silently discard in case of syntax error).
    if ((firstInx == -1) || (firstInx == lastInx))
        return false;

    //Handle header (##) and substitution (#).
    if (line.indexOf("##", firstInx + 1) == (firstInx + 1))
        curEvent = line.mid(firstInx + 3, lastInx - (firstInx + 3));
    else if (line.indexOf('#', firstInx + 1) != (firstInx + 1))
        curEvent = line.mid(firstInx + 1, lastInx - (firstInx + 1));

    return ((curEvent == event) || ((event.isEmpty() && curEvent == "?")));
}

//Parse pbn line and determine tag id and value.
tagIds CGamesDoc::parsePBNLine(QString &currentLine, QString &strValue, QMap<QString, tagIds> &tagName)
{
    QString strTag;

    //Clear output.
    strTag = strValue = "";

    //Look for opening and closing brackets.
    int size = currentLine.size();
    if ((size < 2) || (currentLine[0] != '[') || (currentLine[size-1] != ']'))
        return TAG_NONE;

    //Retrieve the tag text.
    int inx = currentLine.indexOf(' ', 0);
    if (inx < 0)
        return TAG_NONE;
    strTag = currentLine.mid(1, inx-1);
    strTag = strTag.toUpper();

    //Retrieve the value.
    strValue = currentLine.mid(inx+1, size-inx-2);

    //Strip quotes.
    size = strValue.size();
    if ((size < 2) || (strValue[0] != '"') || (strValue[size - 1] != '"'))
        return TAG_NONE;

    strValue = strValue.mid(1, size - 2);
    strValue.trimmed();

    //Compare against known tags.
    if (tagName.contains(strTag))
        return (tagName[strTag]);

    //Nothing found.
    return TAG_NONE;
}

/**
 * @brief Get cards for a pbn deal line.
 * @param[in] strValue The deal line.
 * @param[out] wCards Cards for west.
 * @param[out] nCards Cards for north.
 * @param[out] eCards Cards for east.
 * @param[out] sCards Cards for south.
 * @param[in] board Current board.
 * @return Dealer.
 *
 * @exception PlayException is thrown in case of illegal deal syntax.
 */
Seat CGamesDoc::getCards(QString &strValue, int wCards[], int nCards[], int eCards[], int sCards[], int board) throw(PlayException)
{
    Seat dealer;
    int *cards[4] = { wCards, nCards, eCards, sCards };

    //Check size (2 for dealer, 52 for cards, 3*4 for punctuations and 3 for spaces.
    if (strValue.size() != (2 + 52 + 3*4 + 3))
        throw PlayException(QString("PBN - Illegal deal (size): %1 %2 %3").arg(strValue).arg(" in board: ").arg(board).toStdString());

    //Get dealer.
    if (QString::compare(strValue.mid(0, 2), "W:", Qt::CaseInsensitive) == 0)
        dealer = WEST_SEAT;
    else if (QString::compare(strValue.mid(0, 2), "N:", Qt::CaseInsensitive) == 0)
        dealer = NORTH_SEAT;
    else if (QString::compare(strValue.mid(0, 2), "E:", Qt::CaseInsensitive) == 0)
        dealer = EAST_SEAT;
    else if (QString::compare(strValue.mid(0, 2), "S:", Qt::CaseInsensitive) == 0)
            dealer = SOUTH_SEAT;
    else
        throw PlayException(QString("PBN - Illegal deal (dealer): %1 %2 %3").arg(strValue).arg(" in board: ").arg(board).toStdString());

    int seat = dealer;

    int inx = 2;

    //Get cards one hand at a time.
    for (int i = 0; i < 4; i++)
    {
        inx = getCards(strValue, inx, cards[seat], board);
        if (((i < 3) && (strValue[inx] != ' ')) || ((i == 3) && (inx != strValue.size())))
            throw PlayException(QString("PBN - Illegal deal (hand terminator): %1 %2 %3").arg(strValue).arg(" in board: ").arg(board).toStdString());
        inx++;
        seat = (++seat) % 4;
    }

    return dealer;
}

//Get cards for one hand in a pbn deal line.
int CGamesDoc::getCards(QString &strValue, int inx, int cards[], int board) throw(PlayException)
{
    const Suit SUITS[4] = { SPADES, HEARTS, DIAMONDS, CLUBS};
    int faceValue;
    int i = 0;
    int k = 0;

    for (int suitInx = 0; suitInx < 4; suitInx++)
    {
        while ((faceValue = getFaceValue(strValue, i + inx, board)) != -1)
        {
            if (k >= 13)
                throw PlayException(QString("PBN - Illegal deal (size of hand - too many): %1 %2 %3").arg(strValue).arg(" in board: ").arg(board).toStdString());
            cards[k] = MAKE_CARD(SUITS[suitInx], faceValue);
            i++;
            k++;
        }

        if ((suitInx < 3) && (strValue[i + inx] != '.'))
            throw PlayException(QString("PBN - Illegal deal (punctuation): %1 %2 %3").arg(strValue).arg(" in board: ").arg(board).toStdString());

        i++;
    }

    if (k != 13)
        throw PlayException(QString("PBN - Illegal deal (size of hand - too few): %1 %2 %3").arg(strValue).arg(" in board: ").arg(board).toStdString());

    return (i - 1 + inx);
}

//Get face value for a given position in a pbn deal line.
int CGamesDoc::getFaceValue(QString &strValue, int inx, int board) throw(PlayException)
{
    if (strValue.size() <= inx)
        return -1;
    if ((strValue[inx] == '.') || (strValue[inx] == ' '))
        return -1;

    if (!PBN_CARD.contains(strValue[inx], Qt::CaseInsensitive))
        throw PlayException(QString("PBN - Illegal deal (face value): %1 %2 %3").arg(strValue).arg(" in board: ").arg(board).toStdString());

    return PBN_CARD.indexOf(strValue[inx], 0, Qt::CaseInsensitive);
}

/**
 * @brief Get bid for a given position in a pbn auction line.
 * @param[in] line The auction line.
 * @param[in] inx The index for the position in the line.
 * @param[out] bidCall The bid (might be BID_NONE if no bid is found).
 * @return Next index after the found bid. If nothing (no bid) is found then return the start index (inx).
 */
int CGamesDoc::getBid(QString &line, int inx, Bids *bidCall)
{
    if (inx >= line.size())
        return line.size();

    QString partLine;
    partLine = line.mid(inx).trimmed();
    if (partLine.isEmpty())
        return line.size();

    int n = partLine.indexOf(' ');
    int m = partLine.indexOf("\t");
    if ((m != -1) && (m < n))
        n = m;
    if (n != -1)
    {
        partLine = partLine.mid(0, n);
        n = line.mid(inx).indexOf(partLine) + partLine.size() + inx;
    }
    else
        n = line.size();

    *bidCall = BID_NONE;

    if (QString::compare(partLine, "AP", Qt::CaseInsensitive) == 0)
        *bidCall = BID_PLAYER;
    else if (QString::compare(partLine, "Pass", Qt::CaseInsensitive) == 0)
        *bidCall = BID_PASS;
    else if (QString::compare(partLine, "X", Qt::CaseInsensitive) == 0)
        *bidCall = BID_DOUBLE;
    else if (QString::compare(partLine, "XX", Qt::CaseInsensitive) == 0)
        *bidCall = BID_REDOUBLE;
    else if (QString::compare(partLine, "*", Qt::CaseInsensitive) == 0)
        *bidCall = BID_BLANK;
    else if ((partLine.size() >= 2) && partLine[0].isDigit())
    {
        int level = partLine.mid(0, 1).toInt();
        if ((level >= 1) && (level <= 7))
        {
            if (QString::compare(partLine.mid(1), "S", Qt::CaseInsensitive) == 0)
                *bidCall = MAKE_BID(SPADES, level);
            else if (QString::compare(partLine.mid(1), "H", Qt::CaseInsensitive) == 0)
                *bidCall = MAKE_BID(HEARTS, level);
            else if (QString::compare(partLine.mid(1), "D", Qt::CaseInsensitive) == 0)
                *bidCall = MAKE_BID(DIAMONDS, level);
            else if (QString::compare(partLine.mid(1), "C", Qt::CaseInsensitive) == 0)
                *bidCall = MAKE_BID(CLUBS, level);
            else if (QString::compare(partLine.mid(1), "NT", Qt::CaseInsensitive) == 0)
                *bidCall = MAKE_BID(NOTRUMP, level);
        }
    }

    if (*bidCall == BID_NONE)
        return inx;

    return n;
}

/**
 * @brief Get play for a given position in a pbn play line.
 * @param[in] line The line.
 * @param[in] inx The index of the position.
 * @param[out] playCall The play (-1 for no play, -2 for -, -3 for *).
 * @return The next index after the found play. If nothing (no play) is found then return the start index (inx).
 */
int CGamesDoc::getPlay(QString &line, int inx, int *playCall)
{
    Suit suit;
    int face;

    if (inx >= line.size())
        return line.size();

    QString partLine;
    partLine = line.mid(inx).trimmed();
    if (partLine.isEmpty())
        return line.size();

    int n = partLine.indexOf(' ');
    int m = partLine.indexOf("\t");
    if ((m != -1) && (m < n))
        n = m;
    if (n != -1)
    {
        partLine = partLine.mid(0, n);
        n = line.indexOf(partLine) + partLine.size();
    }
    else
        n = line.size();

    if (QString::compare(partLine.mid(0, 1), "S", Qt::CaseInsensitive) == 0)
        suit = SPADES;
    else if (QString::compare(partLine.mid(0, 1), "H", Qt::CaseInsensitive) == 0)
        suit = HEARTS;
    else if (QString::compare(partLine.mid(0, 1), "D", Qt::CaseInsensitive) == 0)
        suit = DIAMONDS;
    else if (QString::compare(partLine.mid(0, 1), "C", Qt::CaseInsensitive) == 0)
        suit = CLUBS;
    else if (QString::compare(partLine.mid(0, 1), "-", Qt::CaseInsensitive) == 0)
    {
        *playCall = -2;
        return n;
    }
    else if (QString::compare(partLine.mid(0, 1), "*", Qt::CaseInsensitive) == 0)
    {
        *playCall = -3;
        return n;
    }
    else
    {
        *playCall = -1;
        return inx;
    }

    if ((partLine.size() != 2) || (!PBN_CARD.contains(partLine[1], Qt::CaseInsensitive)))
    {
        *playCall = -1;
        return inx;
    }

    face = PBN_CARD.indexOf(partLine[1], 0, Qt::CaseInsensitive);

    *playCall = MAKE_CARD(suit, face);

    return n;
}

/**
 * @brief Get note number from a given position in an auction line. Maybe there is none.
 * @param[in] line The auction line.
 * @param[in] inx The index for the position.
 * @param[out] note number.
 * @return The next index (start index (inx) if no note reference is found).
 */
int CGamesDoc::getNote(QString &line, int inx, int *note)
{
    if (inx >= line.size())
        return line.size();

    QString partLine;
    partLine = line.mid(inx).trimmed();
    if (partLine.isEmpty())
        return line.size();

    int n = partLine.indexOf('=');
    if ((n == -1) || (n != 0))
        return inx;

    int partLineInx = line.indexOf(partLine);

    partLine = partLine.mid(1);
    n = partLine.indexOf('=');
    if ((n == -1) ||(n == 0) ||(n > 2) || ((n == 2) && !partLine[0].isDigit() && !partLine[1].isDigit()) ||
            ((n == 1) && !partLine[0].isDigit()))
        return inx;

    *note = partLine.mid(0, n).toInt();

    return (partLineInx + n + 2);
}

/**
 * @brief Construct a pbn deal line.
 * @param[in] dealer The dealer.
 * @param[in] wCards The cards for west.
 * @param[in] nCards The cards for north.
 * @param[in] eCards The cards for east.
 * @param[in] sCards The cards for south.
 * @param[out] line The constructed deal line.
 * @return The line.
 */
QString &CGamesDoc::setCards(Seat dealer, int wCards[], int nCards[], int eCards[], int sCards[], QString &line)
{
    int *cards[4] = { wCards, nCards, eCards, sCards };
    int seat = dealer;

    line = QString("[Deal \"%1:").arg(PBN_SEAT_NAMES[dealer]);

    for (int i = 0; i < 4; i++)
    {
        setSuit(cards[seat], SPADES, line);
        line += ".";
        setSuit(cards[seat], HEARTS, line);
        line += ".";
        setSuit(cards[seat], DIAMONDS, line);
        line += ".";
        setSuit(cards[seat], CLUBS, line);

        if (i != 3)
            line += " ";
        seat = (++seat) % 4;
    }

    line += "\"]\n";

    return line;
}

//Sets the cards for a given suit in the pbn deal line.
void CGamesDoc::setSuit(int cards[], Suit suit, QString &line)
{
    for (int i = 0; i < 13; i++)
        if (((suit == SPADES) && (IS_SPADES(cards[i]))) ||
                ((suit == HEARTS) && (IS_HEARTS(cards[i]))) ||
                ((suit == DIAMONDS) && (IS_DIAMONDS(cards[i]))) ||
                ((suit == CLUBS) && (IS_CLUBS(cards[i]))))
            line += PBN_CARD[CARD_FACE(cards[i])];
}

/**
 * @brief Construct a pbn contract line.
 * @param[in] contract The contract.
 * @param[in] contractModifier The contract modifier (double or redouble).
 * @param[out] line The contract line constructed.
 * @return The line.
 */
QString &CGamesDoc::setContract(Bids contract, Bids contractModifier, QString &line)
{
    Suit suit;
    int level;

    suit = BID_SUIT(contract);
    level = BID_LEVEL(contract);

    line = QString("[Contract \"%1%2").arg(level).arg(PBN_SUIT_NAMES[suit]);
    if (contractModifier == BID_DOUBLE)
        line += "X";
    else if (contractModifier == BID_REDOUBLE)
        line += "XX";

    line += "\"]\n";

    return line;
}

/**
 * @brief Make pbn auction lines (including notes).
 * @param[in] stream The stream to write to.
 * @param[in] bidHistory The bid history to use for making the auction lines.
 */
void CGamesDoc::makeAuction(QTextStream &stream, CBidHistory &bidHistory)
{
    QList<QString> notes;
    int noNotes = 0;

    int noBids = bidHistory.bidList.size();
    if (noBids > 0)
    {
        Seat dealer = bidHistory.bidList[0].bidder;
        stream << QString("[Auction \"%1\"]").arg(PBN_SEAT_NAMES[dealer]);
        for (int i = 0; i < noBids; i++)
        {
            //Line shift for each four bids.
            if ((i % 4) == 0)
                stream << "\n";

            Bids bid = bidHistory.bidList[i].bid;

            // Double bid?
            if (bid == BID_DOUBLE)
                stream << "X ";

            //Redouble bid ?
            else if (bid == BID_REDOUBLE)
                stream << "XX ";

            //Pass bid?
            else if (bid == BID_PASS)
                stream << "Pass ";

            //Ordinary bid?
            else
            {
                //Get suit and level.
                Suit suit = BID_SUIT(bid);
                int level = BID_LEVEL(bid);

                //Output level and suit.
                stream << QString("%1%2 ").arg(level).arg(PBN_SUIT_NAMES[suit]);

                //Is there an alert?
                if (bidHistory.bidList[i].alert.size() > 0)
                {
                    //Calculate alert note and save for later output.
                    noNotes++;
                    QString alert = QString("[Note \"%1:%2\"]\n").arg(noNotes).arg(bidHistory.bidList[i].alert);
                    notes.append(alert);

                    //Output reference to alert note.
                    stream << QString("=%1= ").arg(noNotes);
                }
            }
        }
        stream << "\n";

        //Output alert notes.
        for (int i = 0; i < notes.size(); i++)
            stream << notes[i];
    }
}

/**
 * @brief Make pbn play lines.
 * @param[in] stream The stream to write to.
 * @param[in] playHistory The play history to use for making the play lines..
 */
void CGamesDoc::makePlay(QTextStream &stream, CPlayHistory &playHistory)
{
    int noTrick = playHistory.getNoTrick();
    if (noTrick > 0)
    {
        Seat openLeader = playHistory.getOpenLeader();
        stream << QString("[Play \"%1\"]\n").arg(PBN_SEAT_NAMES[openLeader]);
        for (int i = 0; i < noTrick; i++)
        {
            int trick[4];
            Suit s1, s2, s3, s4;
            int f1, f2, f3, f4;

            playHistory.getTrick(i, trick);

            s1 = CARD_SUIT(trick[openLeader]);
            f1 = CARD_FACE(trick[openLeader]);
            s2 = CARD_SUIT(trick[(openLeader + 1) % 4]);
            f2 = CARD_FACE(trick[(openLeader + 1) % 4]);
            s3 = CARD_SUIT(trick[(openLeader + 2) % 4]);
            f3 = CARD_FACE(trick[(openLeader + 2) % 4]);
            s4 = CARD_SUIT(trick[(openLeader + 3) % 4]);
            f4 = CARD_FACE(trick[(openLeader + 3) % 4]);

            stream << QString("%1%2 %3%4 %5%6 %7%8\n").arg(PBN_SUIT_NAMES[s1]).arg(PBN_CARD[f1])
                      .arg(PBN_SUIT_NAMES[s2]).arg(PBN_CARD[f2])
                      .arg(PBN_SUIT_NAMES[s3]).arg(PBN_CARD[f3])
                      .arg(PBN_SUIT_NAMES[s4]).arg(PBN_CARD[f4]);
        }
        if (noTrick < 13)
            stream << "*\n";
    }
}

int CGamesDoc::getIndexAndSeat(int gameIndex, QString &nameWN, QString &nameES, Seat *seat)
{
    assert(gameIndex < games.size());

    int i;
    for (i = 0; i < games[gameIndex]->auctionAndPlay.size(); i++)
    {
        QString westName = games[gameIndex]->auctionAndPlay[i]->westName;
        QString northName = games[gameIndex]->auctionAndPlay[i]->northName;
        QString eastName = games[gameIndex]->auctionAndPlay[i]->eastName;
        QString southName = games[gameIndex]->auctionAndPlay[i]->southName;

        if ((nameWN.compare(westName, Qt::CaseInsensitive) == 0) &&
              (nameES.compare(eastName, Qt::CaseInsensitive) == 0))
        {
            *seat = WEST_SEAT;
            break;
        }
        if ((nameWN.compare(northName, Qt::CaseInsensitive) == 0) &&
              (nameES.compare(southName, Qt::CaseInsensitive) == 0))
        {
            *seat = NORTH_SEAT;
            break;
        }
    }
    if (i >= games[gameIndex]->auctionAndPlay.size())
        i = -1;

    return i;
}

/**
 * @brief Calculate rubber vulnerability for the next play after a given (user) played game/play.
 * @param[in] gameIndex Index of the game.
 * @return Calculate rubber vulnerability for next play.
 */
Team CGamesDoc::getRubberVulnerable(int gameIndex)
{
    assert(gameIndex < games.size());

    Team vulnerable = NEITHER;
    int belowTheLineNSPoint = 0, belowTheLineEWPoint = 0;
    int belowTheLineNSGame = 0, belowTheLineEWGame = 0;

    //Start from the first play.
    for (int i = 0; i <= gameIndex; i++)
    {
        int belowTheLinePoint = getBelowTheLine(i);

        if (belowTheLinePoint > 0)
        {
            //Points for NS.
            belowTheLineNSPoint += belowTheLinePoint;
            if (belowTheLineNSPoint >= 100)
            {
                //NS won a game.
                belowTheLineNSGame++;
                if (belowTheLineNSGame == 2)
                {
                    //NS won a rubber.
                    vulnerable = NEITHER;
                    belowTheLineNSGame = belowTheLineEWGame = 0;
                }
                else
                    vulnerable = (vulnerable == NEITHER) ? (NORTH_SOUTH) : (BOTH);

                belowTheLineNSPoint = belowTheLineEWPoint = 0;
            }
        }
        else if (belowTheLinePoint < 0)
        {
            //Points for EW.
            belowTheLineEWPoint -= belowTheLinePoint;
            if (belowTheLineEWPoint >= 100)
            {
                //EW won a game.
                belowTheLineEWGame++;
                if (belowTheLineEWGame == 2)
                {
                    //EW won a rubber.
                    vulnerable = NEITHER;
                    belowTheLineNSGame = belowTheLineEWGame = 0;
                }
                else
                    vulnerable = (vulnerable == NEITHER) ? (EAST_WEST) : (BOTH);

                belowTheLineNSPoint = belowTheLineEWPoint = 0;
            }
        }
    }

    return vulnerable;
}
