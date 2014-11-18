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

#include "cgamesdoc.h"

enum PBNContext { TAG_CONTEXT, AUCTION_CONTEXT, PLAY_CONTEXT};

CGamesDoc::CGamesDoc(QObject *parent) :
    QObject(parent)
{
}

void CGamesDoc::readGames(QTextStream &original, QTextStream &played, QString &event) throw(PlayException)
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
    tagName["[EVENT"] = TAG_EVENT;
    tagName["[SITE"] = TAG_SITE;
    tagName["[DATE"] = TAG_DATE;
    tagName["[ROUND"] = TAG_ROUND;
    tagName["[BOARD"] = TAG_BOARD;
    tagName["[WEST"] = TAG_WEST;
    tagName["[NORTH"] = TAG_NORTH;
    tagName["[EAST"] = TAG_EAST;
    tagName["[SOUTH"] = TAG_SOUTH;
    tagName["[DEALER"] = TAG_DEALER;
    tagName["[VULNERABLE"] = TAG_VULNERABLE;
    tagName["[DEAL"] = TAG_DEAL;
    tagName["[SCORING"] = TAG_SCORING;
    tagName["[DECLARER"] = TAG_DECLARER;
    tagName["[CONTRACT"] = TAG_CONTRACT;
    tagName["[RESULT"] = TAG_RESULT;
    tagName["[AUCTION"] = TAG_AUCTION;
    tagName["[PLAY"] = TAG_PLAY;

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

    numLines = preloadPBNFile(original, event, strLines, auctionNotes, playNotes);

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
                QListIterator<CGame *> gameItr(originalGames);
                while (gameItr.hasNext())
                {
                    CGame *nextGame = gameItr.next();
                    if (nextGame->board == game->board)
                    {
                        //Should also check redundant info here (not yet done!).
                        if (!auctionAndPlay->westName.isEmpty() && !auctionAndPlay->northName.isEmpty() &&
                                !auctionAndPlay->eastName.isEmpty() && !auctionAndPlay->southName.isEmpty())
                            nextGame->auctionAndPlay.append(auctionAndPlay);
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
                    game->auctionAndPlay.append(auctionAndPlay);
                else
                    delete auctionAndPlay;

                originalGames.append(game);
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
            case AUCTION_CONTEXT:
            {
                Seat actor;
                Bids bidCall;
                int note;
                int inx = 0;
                int inxNext;

                do
                {
                    if ((inxNext = getBid(currentLine, inx, &bidCall)) != inx)
                    {
                        if (bidCall == BID_BLANK)       //*  (end of auction)
                        {
                            context = TAG_CONTEXT;
                            break;
                        }
                        else if (bidCall == BID_PLAYER) //AP (All Passes)
                        {
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
                        if ((inxNext = getNote(currentLine, inx, &note)) != inx)
                        {
                            inx = inxNext;
                            alert = auctionNotes[note + gameNo*32];
                        }
                        CBid bid(actor, bidCall, alert);
                        auctionAndPlay->bidHistory.appendBid(bid);
                        if (bidCall == BID_PASS)
                        {
                            noPasses++;
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
                        throw PlayException("PBN - Illegal bid: " + currentLine.toStdString());

                } while (inxNext != currentLine.size());
            }
                break;

            case PLAY_CONTEXT:
            {
                int playCall;
                int note;
                int inx = 0;
                int inxNext;

                do
                {
                    if ((inxNext = getPlay(currentLine, inx, &playCall)) != inx)
                    {
                        if ((playCall == -2) || (playCall = -3))       //* or -  (end of play or not played)
                        {
                            context = TAG_CONTEXT;
                            break;
                        }

                        inx = inxNext;
                        QString alert;
                        if ((inxNext = getNote(currentLine, inx, &note)) != inx)
                        {
                            inx = inxNext;
                            alert = playNotes[note + gameNo*32];
                        }
                        oneTrick[actorNo++] = playCall;
                        if (actorNo == 4)
                        {
                            for (int i = 0; i < 4; i++)
                            {
                                Seat seat = (Seat)((currentLeader + i) % 4);
                                int *cards = (seat == WEST_SEAT) ? (game->wCards) :
                                             (seat == NORTH_SEAT) ? (game->nCards) :
                                             (seat == EAST_SEAT) ? (game->eCards) : (game->sCards);
                                if (auctionAndPlay->playHistory.cardOk(oneTrick[i], seat, cards))
                                    auctionAndPlay->playHistory.setPlay(seat, trickNo, oneTrick[i]);
                                else
                                    throw PlayException("PBN - Illegal play: " + currentLine.toStdString());
                            }
                            currentLeader = auctionAndPlay->playHistory.getNextLeader();
                            actorNo = 0;
                            trickNo++;
                            if (trickNo == 13)
                            {
                                context = TAG_CONTEXT;
                                break;
                            }
                        }
                    }
                    else if (playCall == -1)       //Not a valid play.
                        throw PlayException("PBN - Illegal play: " + currentLine.toStdString());

                } while (inxNext != currentLine.size());
            }
                break;

            case TAG_CONTEXT:
            {
                QString strTag;
                QString strValue;

                //Parse for relevant tags.
                tagIds tag = parsePBNLine(currentLine, strTag, strValue, tagName);
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
                    auctionAndPlay = new CAuctionAndPlay;
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
                        throw PlayException("PBN - Illegal board: " + strValue.toStdString());
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
                        throw PlayException("PBN - Illegal dealer: " + strValue.toStdString());
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
                        throw PlayException("PBN - Illegal vulnerability: " + strValue.toStdString());
                    break;

                case TAG_DEAL:
                    getCards(strValue, game->wCards, game->nCards, game->eCards, game->sCards);
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
                        throw PlayException("PBN - Illegal declarer: " + strValue.toStdString());
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
                            throw PlayException("PBN - Illegal number of tricks in contract: " + strValue.toStdString());

                            level = strValue.mid(0, n).toInt();
                            if ((level < 1) || (level > 7) || (size <= n))
                                throw PlayException("PBN - Illegal format of contract: " + strValue.toStdString());

                                if (QString::compare(strValue.mid(n, 1), "S", Qt::CaseInsensitive) == 0)
                                    suit = SPADES;
                                else if (QString::compare(strValue.mid(n, 1), "H", Qt::CaseInsensitive) == 0)
                                    suit = HEARTS;
                                else if (QString::compare(strValue.mid(n, 1), "D", Qt::CaseInsensitive) == 0)
                                    suit = DIAMONDS;
                                else if (QString::compare(strValue.mid(n, 1), "C", Qt::CaseInsensitive) == 0)
                                    suit = CLUBS;
                                else if (QString::compare(strValue.mid(n, 1), "NT", Qt::CaseInsensitive) == 0)
                                    suit = NOTRUMP;
                                else
                                    throw PlayException("PBN - Illegal suit in contract: " + strValue.toStdString());

                                auctionAndPlay->contract = MAKE_BID(suit, level);

                                if (size == (n + 1))
                                    auctionAndPlay->contractModifier = BID_NONE;
                                else if ((size == (n + 2)) &&
                                        (QString::compare(strValue.mid(n + 1, 1), "X", Qt::CaseInsensitive) == 0))
                                    auctionAndPlay->contractModifier = BID_DOUBLE;
                                else if ((size == (n + 3)) &&
                                    (QString::compare(strValue.mid(n + 1, 2), "XX", Qt::CaseInsensitive) == 0))
                                    auctionAndPlay->contractModifier = BID_REDOUBLE;
                                else
                                    throw PlayException("PBN - Illegal contract modifier in contract: " + strValue.toStdString());
                    }
                    break;

                case TAG_RESULT:
                {
                    for (int i = 0; i < strValue.size(); i++)
                        if (!strValue[i].isDigit())
                            throw PlayException("PBN - Illegal result: " + strValue.toStdString());
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
                        throw PlayException("PBN - Illegal dealer: " + strValue.toStdString());

                    if (dealer != game->dealer)
                        throw PlayException("PBN - Illegal dealer: " + strValue.toStdString());

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
                        throw PlayException("PBN - Illegal leader: " + strValue.toStdString());

                     currentLeader = (Seat)((auctionAndPlay->declarer + 1) % 4);
                    if (leader != currentLeader)
                        throw PlayException("PBN - Illegal leader: " + strValue.toStdString());

                    auctionAndPlay->playHistory.setBidInfo(auctionAndPlay->contract, auctionAndPlay->contractModifier,
                                                           auctionAndPlay->declarer);
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

void CGamesDoc::readGames(QTextStream &stream, QList<CGame> &games, QString &event)
{

}

void CGamesDoc::writeGames(QTextStream &stream)
{

}

void CGamesDoc::getNextDeal(int *board, int wCards[], int nCards[], int eCards[], int sCards[], Seat *dealer, Team *vulnerable)
{

}

void CGamesDoc::setPlayedResult(CBidHistory &bidHistory, CPlayHistory &playHistory, QString &westName, QString &northName, QString &eastName, QString &southName, CBid &contract, CBid &contractModifier, int &result)
{

}

void CGamesDoc::setAutoResult(CBidHistory &bidHistory, CPlayHistory &playHistory, QString &westName, QString &northName, QString &eastName, QString &southName, CBid &contract, CBid &contractModifier, int &result)
{

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

            if (events.indexOf(event) == -1)
                events.append(event);
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
            if (line.indexOf("[NOTE ", 0, Qt::CaseInsensitive) == 0)
            {
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

    return (curEvent == event);
}

tagIds CGamesDoc::parsePBNLine(QString &currentLine, QString &strTag, QString &strValue, QMap<QString, tagIds> &tagName)
{
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

    //Retrieve the value.
    strValue = currentLine.mid(inx+1, size-inx-2);

    //Strip quotes.
    size = strValue.size();
    if ((size < 2) || (strValue[0] != '\"') || (strValue[size - 1] != '\"'))
        return TAG_NONE;

    strValue = strValue.mid(1, size - 2);
    strValue.trimmed();

    //Compare against known tags.
    if (tagName.contains(strTag))
        return (tagName[strTag]);

    //Nothing found.
    return TAG_NONE;
}

void CGamesDoc::getCards(QString &strValue, int *wCards, int *nCards, int *eCards, int *sCards) throw(PlayException)
{
    int seat;
    int *cards[4] = { wCards, nCards, eCards, sCards };

    //Check size (2 for dealer, 52 for cards, 3*4 for punctuations and 3 for spaces.
    if (strValue.size() != (2 + 52 + 3*4 + 3))
        throw PlayException("PBN - Illegal deal (size): " + strValue.toStdString());

    //Get dealer.
    if (QString::compare(strValue.mid(0, 2), "W:", Qt::CaseInsensitive) == 0)
        seat = (int)WEST_SEAT;
    else if (QString::compare(strValue.mid(0, 2), "N:", Qt::CaseInsensitive) == 0)
        seat = (int)NORTH_SEAT;
    else if (QString::compare(strValue.mid(0, 2), "E:", Qt::CaseInsensitive) == 0)
        seat = (int)EAST_SEAT;
    else if (QString::compare(strValue.mid(0, 2), "S:", Qt::CaseInsensitive) == 0)
            seat = (int)SOUTH_SEAT;
    else
        throw PlayException("PBN - Illegal deal (dealer): " + strValue.toStdString());

    int inx = 2;

    for (int i = 0; i < 4; i++)
    {
        inx = getCards(strValue, inx, cards[seat]);
        if (((i < 3) && (strValue[inx] != ' ')) || ((i == 3) && (inx != strValue.size())))
            throw PlayException("PBN - Illegal deal (hand terminator): " + strValue.toStdString());
        inx++;
        seat = (seat++) % 4;
    }
}

int CGamesDoc::getCards(QString &strValue, int inx, int cards[]) throw(PlayException)
{
    Suit suits[4] = { SPADES, HEARTS, DIAMONDS, CLUBS };
    int faceValue;
    int i = 0;
    int k = 0;

    for (int suitInx = 0; suitInx < 4; suitInx++)
    {
        while ((faceValue = getFaceValue(strValue, i + inx)) != -1)
        {
            if (k >= 13)
                throw PlayException("PBN - Illegal deal (size of hand - too many): " + strValue.toStdString());
            cards[i] = MAKE_CARD(suits[suitInx], faceValue);
            i++;
            k++;
        }

        if ((suitInx < 3) && (strValue[i + inx] != '.'))
            throw PlayException("PBN - Illegal deal (punctuation): " + strValue.toStdString());

        i++;
    }

    if (k != 13)
        throw PlayException("PBN - Illegal deal (size of hand - too few): " + strValue.toStdString());

    return (i - 1 + inx);
}

int CGamesDoc::getFaceValue(QString &strValue, int inx) throw(PlayException)
{
    const QString CARD = "23456789TBQKA";

    if (strValue.size() <= inx)
        return -1;
    if ((strValue[inx] == '.') || (strValue[inx] == ' '))
        return -1;

    if (!CARD.contains(strValue[inx], Qt::CaseInsensitive))
        throw PlayException("PBN - Illegal deal (face value): " + strValue.toStdString());

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
        n = line.indexOf(partLine) + partLine.size();
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

    if (*bidCall = BID_NONE)
        return inx;

    return n;
}

int CGamesDoc::getPlay(QString &line, int inx, int *playCall)
{
    const QString CARD = "23456789TBQKA";
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
    if ((n == -1) || ((n + 2) <= partLine.size()))
        return inx;

    partLine = partLine.mid(n + 1);
    n = partLine.indexOf('=');
    if (n == -1)
        return inx;

    *note = partLine.mid(0, n).toInt();

    return (n + 1);
}
