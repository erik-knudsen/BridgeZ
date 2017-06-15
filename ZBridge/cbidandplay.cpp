/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidAndPlay.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Bid and play.

  Platforms: Qt.

*/

/**
 * @file Bid and play.
 * Handling of bids and play.
 */

#include <cassert>

#include "cbidandplayengines.h"
#include "cbidandplay.h"

CBidAndPlay::CBidAndPlay()
{
}

void CBidAndPlay::setSeat(Seat seat)
{
    this->seat = seat;

    bidHistory.setSeat(seat);
}

/**
 * @brief Reset bid history.
 */
void CBidAndPlay::resetBidHistory()
{
    bidHistory.resetBidHistory();
}

/**
 * @brief Append bid to bid history.
 *
 * Appends the bid to the bid history and determines and also appends the rules
 * for the bid.
 *
 * @param bid The bid to append.
 */
void CBidAndPlay::appendBid(Seat bidder, Bids bid, Team vulnerable)
{
    QString alert;
    QList<CRule *> rules;
    bool substitute = false;

    rules = bidAndPlayEngines->getpRules(bidder, bidHistory, bid, vulnerable, &substitute);
    if (rules.size() > 0)
        alert = bidAndPlayEngines->getAlertIdDesc(rules[0]->getAlertId());

    CBid bidEntry(bidder, bid, alert, rules, substitute);

    bidHistory.appendBid(bidEntry);
}

/**
* @brief Undo some of the bid history.
*
* @param bid Last regular bidders bid (not double, redouble or pass) after one round (4) bids have been popped.
* @return One less than number of bids given until (and including) last regular bidder or REBID if bid history gets reset.
* */
int CBidAndPlay::bidUndo(Bids *bid)
{
    return bidHistory.undo(bid);
}

/**
 * @brief Based on bid engine get next bid.
 *
 * @param seat Bidders seat.
 * @param teamVul Vulnerability for the bidder.
 * @return The automatically calculated bid.
 */
CBid CBidAndPlay::getNextBid(Seat seat, Team teamVul)
{
    return (bidAndPlayEngines->getNextBid(seat, ownFeatures, bidHistory, teamVul));
}

void CBidAndPlay::setActorsCards(int cards[])
{
    for (int i = 0; i < 13; i++) actorsCards[i] = cards[i];

    ownFeatures.setCardFeatures(cards);
}

/**
 * @brief Based on play engine get next play.
 *
 * @param player The player (dummys cards are played by declarer).
 * @param dummySeat Dummy.
 * @return The automatically calculated play.
 */
int CBidAndPlay::getNextPlay(Seat player, Seat dummySeat)
{
    return bidAndPlayEngines->getNextPlay(player, dummySeat, actorsCards, dummysCards, bidHistory, playHistory);
}

/**
 * @brief Calculates a textual description of the feature limits for the bid.
 *
 * @param bid The bid to do the calculation for.
 * @return Textual description of the feature limits.
 */
QString CBidAndPlay::featuresOfBid(CBid &bid)
{
    QString features;

    if (bid.rules.size() > 0)
    {
        //Status.
        Forcing forcing = bid.rules[0]->getStatus();
        QStringList labels;
        labels << tr("Non Forcing") << tr("Forcing") << tr("Game Forcing") << tr("Must Pass");
        features += tr("Status: ") + labels[forcing];

        for (int i = 0; i < bid.rules.size(); i++)
        {
            features += "\n";

            CFeatures lowFeatures;
            CFeatures highFeatures;
            bid.rules[i]->getFeatures(&lowFeatures, &highFeatures);

            //HCP.
            int hcpLow = lowFeatures.getHcp(ANY);
            int hcpHigh = highFeatures.getHcp(ANY);
            if ((hcpLow > 0) || (hcpHigh < highFeatures.getMaxHcp(ANY)))
            {
                if (hcpHigh == hcpLow)
                    features += QString(tr("HCP: ") + "%1 ; ").arg(hcpLow);
                else if (hcpHigh == highFeatures.getMaxHcp(ANY))
                    features += QString(tr("HCP: ") + "%1+ ; ").arg(hcpLow);
                else if (hcpLow == 0)
                    features += QString(tr("HCP: ") + "+%1 ; ").arg(hcpHigh);
                else
                    features += QString(tr("HCP: ") + "%1-%2 ; ").arg(hcpLow).arg(hcpHigh);
            }

            //DP.
            bool found = false;
            for (int i = 0; i < 5; i++)
            {
                Suit suit = (Suit)((i < 4) ? (i) : (-1));
                int dpLow = lowFeatures.getDp(suit);
                int dpHigh = highFeatures.getDp(suit);
                if ((dpLow > 0) || (dpHigh < highFeatures.getMaxDp()))
                    found = true;
            }
            if (found)
            {
                features += tr("DP ");
                for (int i = 0; i < 5; i++)
                {
                    Suit suit = (Suit)((i < 4) ? (i) : (-1));
                    int dpLow = lowFeatures.getDp(suit);
                    int dpHigh = highFeatures.getDp(suit);
                    if ((dpLow > 0) || (dpHigh < highFeatures.getMaxDp()))
                    {
                        features += QCoreApplication::translate("defines", SUIT_NAMES[i]) + ":";
                        if (dpHigh == dpLow)
                            features += QString("%1 ").arg(dpLow);
                        else if (dpHigh == highFeatures.getMaxDp())
                            features += QString("%1+ ").arg(dpLow);
                        else if (dpLow == 0)
                            features += QString("+%1 ").arg(dpHigh);
                        else
                            features += QString("%1-%2 ").arg(dpLow).arg(dpHigh);
                    }
                }
                features += "; ";
            }

            //TP.
            found = false;
            for (int i = 0; i < 5; i++)
            {
                Suit suit = (Suit)((i < 4) ? (i) : (-1));
                int tpLow = lowFeatures.getPoints(suit);
                int tpHigh = highFeatures.getPoints(suit);
                if ((tpLow > 0) || (tpHigh < highFeatures.getMaxPoints()))
                    found = true;
            }
            if (found)
            {
                features += tr("TP ");
                for (int i = 0; i < 5; i++)
                {
                    Suit suit = (Suit)((i < 4) ? (i) : (-1));
                    int tpLow = lowFeatures.getPoints(suit);
                    int tpHigh = highFeatures.getPoints(suit);
                    if ((tpLow > 0) || (tpHigh < highFeatures.getMaxPoints()))
                    {
                        features += QCoreApplication::translate("defines", SUIT_NAMES[i]) + ":";
                        if (tpHigh == tpLow)
                            features += QString("%1 ").arg(tpLow);
                        else if (tpHigh == highFeatures.getMaxPoints())
                            features += QString("%1+ ").arg(tpLow);
                        else if (tpLow == 0)
                            features += QString("+%1 ").arg(tpHigh);
                        else
                            features += QString("%1-%2 ").arg(tpLow).arg(tpHigh);
                    }
                }
                features += "; ";
            }

            //Suit length.
            found = false;
            for (int i = 0; i < 4; i++)
            {
                int slLow = lowFeatures.getSuitLen((Suit)i);
                int slHigh = highFeatures.getSuitLen((Suit)i);
                if ((slLow > 0) || (slHigh < highFeatures.getMaxSuitLen()))
                    found = true;
            }
            if (found)
            {
                features += tr("SL ");
                for (int i = 0; i < 4; i++)
                {
                    int slLow = lowFeatures.getSuitLen((Suit)i);
                    int slHigh = highFeatures.getSuitLen((Suit)i);
                    if ((slLow > 0) || (slHigh < highFeatures.getMaxSuitLen()))
                    {
                        features += QCoreApplication::translate("defines", SUIT_NAMES[i]) + ":";
                        if (slHigh == slLow)
                            features += QString("%1 ").arg(slLow);
                        else if (slHigh == highFeatures.getMaxSuitLen())
                            features += QString("%1+ ").arg(slLow);
                        else if (slLow == 0)
                            features += QString("+%1 ").arg(slHigh);
                        else
                            features += QString("%1-%2 ").arg(slLow).arg(slHigh);
                    }
                }
                features += "; ";
            }

            //Difference in length.
            found = false;
            for (int suit_1 = 3; suit_1 > 0; suit_1--)
            {
                QString suit_1_name =  QCoreApplication::translate("defines", SUIT_NAMES[suit_1]);
                for (int suit_2 = suit_1 - 1; suit_2 >= 0; suit_2--)
                {
                    QString suit_2_name =  QCoreApplication::translate("defines", SUIT_NAMES[suit_2]);
                    QString txt = tr("DL ") + suit_1_name + suit_2_name + ":";
                    int difLow = lowFeatures.getDif2((Suit)suit_1, (Suit)suit_2);
                    int difHigh = highFeatures.getDif2((Suit)suit_1, (Suit)suit_2);
                    if ((difLow > -highFeatures.getMaxDif2()) || (difHigh < highFeatures.getMaxDif2()))
                        found = true;
                }
            }
            if (found)
            {
                features += tr("DL ");
                for (int suit_1 = 3; suit_1 > 0; suit_1--)
                {
                    QString suit_1_name =  QCoreApplication::translate("defines", SUIT_NAMES[suit_1]);
                    for (int suit_2 = suit_1 - 1; suit_2 >= 0; suit_2--)
                    {
                        int difLow = lowFeatures.getDif2((Suit)suit_1, (Suit)suit_2);
                        int difHigh = highFeatures.getDif2((Suit)suit_1, (Suit)suit_2);
                        if ((difLow > -highFeatures.getMaxDif2()) || (difHigh < highFeatures.getMaxDif2()))
                        {
                            QString suit_2_name =  QCoreApplication::translate("defines", SUIT_NAMES[suit_2]);
                            features += suit_1_name + suit_2_name + ":";
                            if (difHigh == difLow)
                                features += QString("%1 ").arg(difLow);
                            else if (difHigh == highFeatures.getMaxDif2())
                                features += QString("%1+ ").arg(difLow);
                            else if (difLow == -lowFeatures.getMaxDif2())
                                features += QString("+%1 ").arg(difHigh);
                            else
                                features += QString("%1-%2 ").arg(difLow).arg(difHigh);
                        }
                    }
                }
                features += "; ";
            }

            //Aces.
            int aceLow = lowFeatures.getCountCard(ANY, ACE);
            int aceHigh = highFeatures.getCountCard(ANY, ACE);
            if ((aceLow > 0) || (aceHigh < highFeatures.getMaxCountCard(ANY)))
            {
                if (aceHigh == aceLow)
                    features += QString(tr("A: ") + "%1 ; ").arg(aceLow);
                else if (aceHigh == highFeatures.getMaxCountCard(ANY))
                    features += QString(tr("A: ") + "%1+ ; ").arg(aceLow);
                else if (aceLow == 0)
                    features += QString(tr("A: ") + "+%1 ; ").arg(aceHigh);
                else
                    features += QString(tr("A: ") + "%1-%2 ; ").arg(aceLow).arg(aceHigh);
            }

            //Kings.
            int kingLow = lowFeatures.getCountCard(ANY, KING);
            int kingHigh = highFeatures.getCountCard(ANY, KING);
            if ((kingLow > 0) || (kingHigh < highFeatures.getMaxCountCard(ANY)))
            {
                if (kingHigh == kingLow)
                    features += QString(tr("K: ") + "%1 ; ").arg(kingLow);
                else if (kingHigh == highFeatures.getMaxCountCard(ANY))
                    features += QString(tr("K: ") + "%1+ ; ").arg(kingLow);
                else if (kingLow == 0)
                    features += QString(tr("K: ") + "+%1 ; ").arg(kingHigh);
                else
                    features += QString(tr("K: ") + "%1-%2 ; ").arg(kingLow).arg(kingHigh);
            }

            //Playing tricks.
            found = false;
            for (int i = 0; i < 5; i++)
            {
                Suit suit = (Suit)((i < 4) ? (i) : (-1));
                int ptLow = lowFeatures.getPlayingTricks(suit);
                int ptHigh = highFeatures.getPlayingTricks(suit);
                if ((ptLow > 0) || (ptHigh < highFeatures.getMaxPlayingTricks()))
                    found = true;
            }
            if (found)
            {
                features += tr("PT ");
                for (int i = 0; i < 5; i++)
                {
                    Suit suit = (Suit)((i < 4) ? (i) : (-1));
                    int ptLow = lowFeatures.getPlayingTricks(suit);
                    int ptHigh = highFeatures.getPlayingTricks(suit);
                    if ((ptLow > 0) || (ptHigh < highFeatures.getMaxPlayingTricks()))
                    {
                        features += QCoreApplication::translate("defines", SUIT_NAMES[i]) + ":";
                        if (ptHigh == ptLow)
                            features += QString("%1 ").arg(ptLow);
                        else if (ptHigh == highFeatures.getMaxDp())
                            features += QString("%1+ ").arg(ptLow);
                        else if (ptLow == 0)
                            features += QString("+%1 ").arg(ptHigh);
                        else
                            features += QString("%1-%2 ").arg(ptLow).arg(ptHigh);
                    }
                }
                features += "; ";
            }

            //Stopper quality.
            found = false;
            for (int i = 0; i < 4; i++)
            {
                int sqLow = lowFeatures.getStopNT((Suit)i);
                int sqHigh = highFeatures.getStopNT((Suit)i);
                if ((sqLow > 0) || (sqHigh < highFeatures.getMaxStopNT()))
                    found = true;
            }
            if (found)
            {
                features += tr("SQ ");
                for (int i = 0; i < 4; i++)
                {
                    int sqLow = lowFeatures.getStopNT((Suit)i);
                    int sqHigh = highFeatures.getStopNT((Suit)i);
                    if ((sqLow > 0) || (sqHigh < highFeatures.getMaxStopNT()))
                    {
                        features += QCoreApplication::translate("defines", SUIT_NAMES[i]) + ":";
                        if (sqHigh == sqLow)
                            features += QString("%1 ").arg(sqLow);
                        else if (sqHigh == highFeatures.getMaxStopNT())
                            features += QString("%1+ ").arg(sqLow);
                        else if (sqLow == 0)
                            features += QString("+%1 ").arg(sqHigh);
                        else
                            features += QString("%1-%2 ").arg(sqLow).arg(sqHigh);
                    }
                }
                features +="; ";
            }

            //General quality.
            found = false;
            for (int i = 0; i < 4; i++)
            {
                int gqLow = lowFeatures.getQlty((Suit)i);
                int gqHigh = highFeatures.getQlty((Suit)i);
                if ((gqLow > 0) || (gqHigh < highFeatures.getMaxQlty()))
                    found = true;
            }
            if (found)
            {
                features += tr("GQ ");
                for (int i = 0; i < 4; i++)
                {
                    int gqLow = lowFeatures.getQlty((Suit)i);
                    int gqHigh = highFeatures.getQlty((Suit)i);
                    if ((gqLow > 0) || (gqHigh < highFeatures.getMaxQlty()))
                    {
                        features += QCoreApplication::translate("defines", SUIT_NAMES[i]) + ":";
                        if (gqHigh == gqLow)
                            features += QString("%1 ").arg(gqLow);
                        else if (gqHigh == highFeatures.getMaxQlty())
                            features += QString("%1+ ").arg(gqLow);
                        else if (gqLow == 0)
                            features += QString("+%1 ").arg(gqHigh);
                        else
                            features += QString("%1-%2 ").arg(gqLow).arg(gqHigh);
                    }
                }
                features += "; ";
            }
        }
    }
    return features;
}

/**
 * @brief Get textual description of the feature limits for the last bid in the bid history.
 *
 * @return Textual description of the feature limits for the last bid in the bid history.
 */
QString CBidAndPlay::featuresOfLastBid()
{
    assert (bidHistory.bidList.size() > 0);

    return featuresOfBid(bidHistory.bidList.last());
}

/**
 * @brief Get the alert of the last bid.
 *
 * @return Alert of the last bid.
 */
QString CBidAndPlay::alertOfLastBid()
{
    assert (bidHistory.bidList.size() > 0);

    return bidHistory.bidList.last().alert;
}
