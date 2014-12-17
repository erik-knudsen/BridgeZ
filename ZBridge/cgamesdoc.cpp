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

#include "cgamesdoc.h"

enum PBNContext { TAG_CONTEXT, AUCTION_CONTEXT, PLAY_CONTEXT};

CGamesDoc::CGamesDoc(QObject *parent) :
    QObject(parent)
{
    dealType = RANDOM_DEAL;
}

void CGamesDoc::readGames(QTextStream &original, QTextStream &played, QString &event) throw(PlayException)
{
    //Clean up for new game.
    clearGames();

    this->event = event;

    //Original games (check that there are original games).
    if (original.device() != 0)
    {
        readGames(original, event, true);
        if (games.size() > 0)
            dealType = ORIGINAL_DEAL;
    }

    //Played games (check that there are played games).
    if (played.device() != 0)
        readGames(played, event, false);
}

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

void CGamesDoc::clearGames()
{
    //Clean up for new (random) game.
    while (!games.isEmpty())
    {
        CGame *game = games.takeFirst();
        while (!game->auctionAndPlay.isEmpty())
            delete(game->auctionAndPlay.takeFirst());
        delete game;

    }
    currentGameIndex = -1;
    dealType = RANDOM_DEAL;
}

void CGamesDoc::getNextDeal(int *board, int cards[][13], Seat *dealer, Team *vulnerable)
{
    currentGameIndex++;

    //Give random card distribution etc.?
    if (dealType == PLAYED_GAME)
    {
        CGame *currentGame = new CGame();

        Team VULNERABLE[4] = { NEITHER, NORTH_SOUTH, EAST_WEST, BOTH };
        Seat DEALER[4] = { NORTH_SEAT, EAST_SEAT, SOUTH_SEAT, WEST_SEAT };
        int i, j, inx;
        int cardDeck[52];

        //Info about board.
        int boardNo = currentGameIndex;

        *board = boardNo;
        *dealer = DEALER[boardNo%4];
        *vulnerable = VULNERABLE[(boardNo%4 + boardNo/4)%4];

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
    else
    {
        //Take card distribution etc. from next game.
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
}

void CGamesDoc::setPlayedResult(CBidHistory &bidHistory, CPlayHistory &playHistory, QString &westName,
                                QString &northName, QString &eastName, QString &southName, Seat declarer,
                                Bids contract, Bids contractModifier, int result)
{
    //Set result of manually played game.
    CAuctionAndPlay * auctionAndPlay = new CAuctionAndPlay();

    auctionAndPlay->gameType = PLAYED_GAME;
    auctionAndPlay->bidHistory = bidHistory;
    auctionAndPlay->playHistory = playHistory;
    auctionAndPlay->westName = westName;
    auctionAndPlay->northName = northName;
    auctionAndPlay->eastName = eastName;
    auctionAndPlay->southName = southName;
    auctionAndPlay->declarer = declarer;
    auctionAndPlay->contract = contract;
    auctionAndPlay->contractModifier = contractModifier;
    auctionAndPlay->result = result;

    games[currentGameIndex]->auctionAndPlay.append(auctionAndPlay);
}

void CGamesDoc::setAutoResult(CBidHistory &bidHistory, CPlayHistory &playHistory, QString &westName,
                              QString &northName, QString &eastName, QString &southName, Seat declarer,
                              Bids contract, Bids contractModifier, int result)
{
    //Set result af automatically played game.
    CAuctionAndPlay * auctionAndPlay = new CAuctionAndPlay();

    auctionAndPlay->gameType = AUTO_GAME;
    auctionAndPlay->bidHistory = bidHistory;
    auctionAndPlay->playHistory = playHistory;
    auctionAndPlay->westName = westName;
    auctionAndPlay->northName = northName;
    auctionAndPlay->eastName = eastName;
    auctionAndPlay->southName = southName;
    auctionAndPlay->declarer = declarer;
    auctionAndPlay->contract = contract;
    auctionAndPlay->contractModifier = contractModifier;
    auctionAndPlay->result = result;

    games[currentGameIndex]->auctionAndPlay.append(auctionAndPlay);
}

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

    //Map of tag names.
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

    //Initial tag values.
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

    //Read and preprocess pbn file.
    numLines = preloadPBNFile(pbnText, event, strLines, auctionNotes, playNotes);

    //Process one line at a time.
    gameNo = 0;
    context = TAG_CONTEXT;
    QListIterator<QString> lineItr(strLines);
    while (lineItr.hasNext())
    {
        QString currentLine = lineItr.next();

        //Check for end of game (blank line).
        if (currentLine.isEmpty())
        {
            gameNo++;
            //Check for multiple empty lines (should never happen - be tolerant though!).
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
                        bool error;
                        for (int i = 0; i < 13; i++)
                            if ((nextGame->wCards[i] != game->wCards[i]) || (nextGame->nCards[i] != game->nCards[i]) ||
                                    (nextGame->eCards[i] != game->eCards[i]) || (nextGame->sCards[i] != game->sCards[i]))
                                error = true;
                        if ((nextGame->dealer != game->dealer) || (nextGame->vulnerable != game->vulnerable) || error)
                            throw PlayException(QString("PBN - Illegal redundant info in games with board: %1").arg(game->board).toStdString());

                        if (!auctionAndPlay->westName.isEmpty() && !auctionAndPlay->northName.isEmpty() &&
                                !auctionAndPlay->eastName.isEmpty() && !auctionAndPlay->southName.isEmpty())
                        {
                            bool foundPlayed = false;
                            if (!originalGames)
                            {
                                //In case we are processing (by this program) played games, then the first game
                                //is a manually played game and the next (if existing) is an automatically
                                //played game (there will at most be two played games).
                                QListIterator<CAuctionAndPlay *> auctionAndPlayItr(nextGame->auctionAndPlay);
                                while (!foundPlayed && auctionAndPlayItr.hasNext())
                                {
                                    CAuctionAndPlay *nextAuctionAndPlay = auctionAndPlayItr.next();
                                    if (nextAuctionAndPlay->gameType == PLAYED_GAME)
                                        foundPlayed = true;
                                }
                            }
                            auctionAndPlay->gameType = (originalGames) ? (ORIGINAL_GAME) : (foundPlayed) ? (AUTO_GAME) : (PLAYED_GAME);
                            nextGame->auctionAndPlay.append(auctionAndPlay);
                            if (foundPlayed)
                                currentGameIndex = gameInx;
                        }
                        else
                            delete auctionAndPlay;

                        delete game;
                        game = 0;
                    }
                }
            }

            //New game?
            if ((game != 0) && (game->board != -1))
            {
                if (!auctionAndPlay->westName.isEmpty() && !auctionAndPlay->northName.isEmpty() &&
                        !auctionAndPlay->eastName.isEmpty() && !auctionAndPlay->southName.isEmpty())
                {
                    auctionAndPlay->gameType = (originalGames) ? (ORIGINAL_GAME) : (PLAYED_GAME);
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

                        //Have all (4) actors played?
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

                //Parse for relevant tags.
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
                    auctionAndPlay = new CAuctionAndPlay;
                    auctionAndPlay->declarer = NO_SEAT;
                    auctionAndPlay->contract = BID_NONE;
                    auctionAndPlay->contractModifier = BID_NONE;
                    auctionAndPlay->result = -1;
                }

                //Process tag.
                switch (tag)
                {
                case TAG_BOARD:
                {
                    int board = strValue.toInt();
                    if (board > 0)
                        game->board = board;
                    else
                    throw PlayException(QString("PBN - Illegal board value: %1").arg(strValue).toStdString());
                }
                    break;

                case TAG_WEST:
                    auctionAndPlay->westName = strValue;
                    break;

                case TAG_NORTH:
                    auctionAndPlay->northName = strValue;
                    break;

                case TAG_EAST:
                    auctionAndPlay->eastName = strValue;
                    break;

                case TAG_SOUTH:
                    auctionAndPlay->southName = strValue;
                    break;

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

                case TAG_DEAL:
                {
                    Seat dealer = getCards(strValue, game->wCards, game->nCards, game->eCards, game->sCards, game->board);
                    if (game->dealer == NO_SEAT)
                        game->dealer = dealer;
                }
                    break;

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
                        throw PlayException(QString("PBN - Illegal declarer: %1 %2 %3").arg(strValue).arg(" in board: ").arg(game->board).toStdString());
                    break;

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

                case TAG_RESULT:
                {
                    for (int i = 0; i < strValue.size(); i++)
                        if (!strValue[i].isDigit())
                            throw PlayException(QString("PBN - Illegal result: %1 %2 %3").arg(strValue).arg(" in board: ").arg(game->board).toStdString());
                        auctionAndPlay->result = strValue.toInt();
                }
                    break;

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
}

void CGamesDoc::writeGame(QTextStream &stream, CGame *game, GameType gameType, QString event)
{
    const QString SEAT_NAMES[4] = {"W", "N", "E", "S"};
    const QString VULNERABILITY_NAMES[4] = {"None", "NS", "EW", "All"};
    QString ev = event;
    QString line;
    QListIterator<CAuctionAndPlay *> auctionAndPlayItr(game->auctionAndPlay);
    while (auctionAndPlayItr.hasNext())
    {
        CAuctionAndPlay *nextAuctionAndPlay = auctionAndPlayItr.next();
        if (nextAuctionAndPlay->gameType == gameType)
        {
            stream << "[Event \"" << ev << "\"]\n";
            ev = "#";
            stream << QString("[Board \"%1\"]\n").arg(game->board).toLatin1();
            stream << QString("[West \"%1\"]\n").arg(nextAuctionAndPlay->westName);
            stream << QString("[North \"%1\"]\n").arg(nextAuctionAndPlay->northName);
            stream << QString("[East \"%1\"]\n").arg(nextAuctionAndPlay->eastName);
            stream << QString("[South \"%1\"]\n").arg(nextAuctionAndPlay->southName);
            stream << QString("[Dealer \"%1\"]\n").arg(SEAT_NAMES[game->dealer]);
            stream << QString("[Vulnerable \"%1\"]\n").arg(VULNERABILITY_NAMES[game->vulnerable]);
            stream << setCards(game->wCards, game->nCards, game->eCards, game->sCards, line).toLatin1();
            stream << QString("[Declarer \"%1\"]\n").arg(SEAT_NAMES[nextAuctionAndPlay->declarer]);
            stream << setContract(nextAuctionAndPlay->contract, nextAuctionAndPlay->contractModifier, line);
            stream << QString("[Result \"%1\"]\n").arg(nextAuctionAndPlay->result);
            makeAuction(stream, nextAuctionAndPlay->bidHistory);
            makePlay(stream, nextAuctionAndPlay->playHistory);
            stream << QString("\n");
        }
    }
}

//Preload PBN file, strip comments check for export tag and load only data for the given event.
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

bool CGamesDoc::searchGame(QString &line, QString &event)
{
    static QString curEvent;

    if (line.indexOf("[EVENT", 0, Qt::CaseInsensitive) == 0)
    {
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
    }

    return ((curEvent == event) || ((event.isEmpty() && curEvent == "?")));
}

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

int CGamesDoc::getCards(QString &strValue, int inx, int cards[], int board) throw(PlayException)
{
    Suit suits[4] = { SPADES, HEARTS, DIAMONDS, CLUBS };
    int faceValue;
    int i = 0;
    int k = 0;

    for (int suitInx = 0; suitInx < 4; suitInx++)
    {
        while ((faceValue = getFaceValue(strValue, i + inx, board)) != -1)
        {
            if (k >= 13)
                throw PlayException(QString("PBN - Illegal deal (size of hand - too many): %1 %2 %3").arg(strValue).arg(" in board: ").arg(board).toStdString());
            cards[k] = MAKE_CARD(suits[suitInx], faceValue);
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

int CGamesDoc::getFaceValue(QString &strValue, int inx, int board) throw(PlayException)
{
    const QString CARD = "23456789TJQKA";

    if (strValue.size() <= inx)
        return -1;
    if ((strValue[inx] == '.') || (strValue[inx] == ' '))
        return -1;

    if (!CARD.contains(strValue[inx], Qt::CaseInsensitive))
        throw PlayException(QString("PBN - Illegal deal (face value): %1 %2 %3").arg(strValue).arg(" in board: ").arg(board).toStdString());

    return CARD.indexOf(strValue[inx], 0, Qt::CaseInsensitive);
}

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

int CGamesDoc::getPlay(QString &line, int inx, int *playCall)
{
    const QString CARD = "23456789TJQKA";
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

    if ((partLine.size() != 2) || (!CARD.contains(partLine[1], Qt::CaseInsensitive)))
    {
        *playCall = -1;
        return inx;
    }

    face = CARD.indexOf(partLine[1], 0, Qt::CaseInsensitive);

    *playCall = MAKE_CARD(suit, face);

    return n;
}

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

QString &CGamesDoc::setCards(int wCards[], int nCards[], int eCards[], int sCards[], QString &line)
{
    line = "[Deal \"N:";

    //First north.
    setSuit(nCards, SPADES, line);
    line += ".";
    setSuit(nCards, HEARTS, line);
    line += ".";
    setSuit(nCards, DIAMONDS, line);
    line += ".";
    setSuit(nCards, CLUBS, line);

    line += " ";

    //Then east.
    setSuit(eCards, SPADES, line);
    line += ".";
    setSuit(eCards, HEARTS, line);
    line += ".";
    setSuit(eCards, DIAMONDS, line);
    line += ".";
    setSuit(eCards, CLUBS, line);

    line += " ";

    //Then south.
    setSuit(sCards, SPADES, line);
    line += ".";
    setSuit(sCards, HEARTS, line);
    line += ".";
    setSuit(sCards, DIAMONDS, line);
    line += ".";
    setSuit(sCards, CLUBS, line);

    line += " ";

    //Then west.
    setSuit(wCards, SPADES, line);
    line += ".";
    setSuit(wCards, HEARTS, line);
    line += ".";
    setSuit(wCards, DIAMONDS, line);
    line += ".";
    setSuit(wCards, CLUBS, line);

    line += "\"]\n";

    return line;
}

void CGamesDoc::setSuit(int cards[], Suit suit, QString &line)
{
    int i;
    const char CARD[] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

    for (i = 0; i < 13; i++)
    {
        if (((suit == SPADES) && (IS_SPADES(cards[i]))) ||
            ((suit == HEARTS) && (IS_HEARTS(cards[i]))) ||
            ((suit == DIAMONDS) && (IS_DIAMONDS(cards[i]))) ||
            ((suit == CLUBS) && (IS_CLUBS(cards[i]))))
            line += CARD[CARD_FACE(cards[i])];
    }
}

QString &CGamesDoc::setContract(Bids contract, Bids contractModifier, QString &line)
{
    const QString SUIT_NAMES[5] = { "C", "D", "H", "S", "NT" };
    Suit suit;
    int level;

    suit = BID_SUIT(contract);
    level = BID_LEVEL(contract);

    line = QString("[Contract \"%1%2").arg(level).arg(SUIT_NAMES[suit]);
    if (contractModifier == BID_DOUBLE)
        line += "X";
    else if (contractModifier == BID_REDOUBLE)
        line += "XX";

    line += "\"]\n";

    return line;
}

void CGamesDoc::makeAuction(QTextStream &stream, CBidHistory &bidHistory)
{
    const QString SUIT_NAMES[5] = { "C", "D", "H", "S", "NT" };
    const QString SEAT_NAMES[4] = { "W", "N", "E", "S"};
    QList<QString> notes;
    int noNotes = 0;

    int noBids = bidHistory.bidList.size();
    if (noBids > 0)
    {
        Seat dealer = bidHistory.bidList[0].bidder;
        stream << QString("[Auction \"%1\"]").arg(SEAT_NAMES[dealer]);
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
                stream << QString("%1%2 ").arg(level).arg(SUIT_NAMES[suit]);

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

void CGamesDoc::makePlay(QTextStream &stream, CPlayHistory &playHistory)
{
    const QString SUIT_NAMES[4] = { "C", "D", "H", "S"};
    const QString SEAT_NAMES[4] = { "W", "N", "E", "S"};
    const QString CARD = "23456789TJQKA";
    int noTrick = playHistory.getNoTrick();
    if (noTrick > 0)
    {
        Seat openLeader = playHistory.getOpenLeader();
        stream << QString("[Play \"%1\"]\n").arg(SEAT_NAMES[openLeader]);
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

            stream << QString("%1%2 %3%4 %5%6 %7%8\n").arg(SUIT_NAMES[s1]).arg(CARD[f1])
                      .arg(SUIT_NAMES[s2]).arg(CARD[f2])
                      .arg(SUIT_NAMES[s3]).arg(CARD[f3])
                      .arg(SUIT_NAMES[s4]).arg(CARD[f4]);
        }
        if (noTrick < 13)
            stream << "*\n";
    }
}
