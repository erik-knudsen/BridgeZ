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

/**
 * @brief Reset bid history.
 */
void CBidAndPlay::resetBidHistory()
{
    bidHistory.resetBidHistory();
}

/**
 * @brief Append bid to bid history.
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
* @param bid Last regular bidders bid (not double, redouble or pass) after one round (4) bids have been popped.
* @return One less than number of bids given until (and including) last regular bidder or REBID if bid history gets reset.
* */
int CBidAndPlay::bidUndo(Bids *bid)
{
    return bidHistory.undo(bid);
}

/**
 * @brief Based on bid engine get next bid.
 * @param seat Bidders seat.
 * @param teamVul Vulnerability for the bidder.
 * @return The automatically calculated bid.
 */
Bids CBidAndPlay::getNextBid(Seat seat, Team teamVul)
{
    Bids bid;

    bid = bidAndPlayEngines->getNextBid(seat, bidHistory, actorsCards, teamVul);

    return bid;
/*
    if (bidHistory.bidList.isEmpty())
        return BID_1C;

    if (bidHistory.bidList.last().bid == BID_PASS)
        return BID_PASS;

    if (bidHistory.bidList.last().bid == BID_7NT)
        return BID_PASS;

    if (bidHistory.bidList.last().bid == BID_DOUBLE)
    return BID_PASS;
//        return (Bids)(bidHistory.bidList[bidHistory.bidList.size() - 2].bid + 1);

//    if (bidHistory.bidList.last().bid == BID_REDOUBLE)
//        return BID_PASS;
//        return (Bids)(bidHistory.bidList[bidHistory.bidList.size() - 3].bid + 1);

    return (Bids)(bidHistory.bidList.last().bid + 1);
*/
}

/**
 * @brief Based on play engine get next play.
 * @param player The player.
 * @param dummySeat Dummy.
 * @return The automatically calculated play.
 */
int CBidAndPlay::getNextPlay(Seat player, Seat dummySeat)
{
    int i;

    int *cards = (player == dummySeat) ? dummysCards : actorsCards;

    for (i = 0; i < 52; i++)
        if (playHistory.cardOk(i, player, cards))
            break;

    return i;
}

QString CBidAndPlay::featuresOfLastBid()
{
    QString features;

    assert (bidHistory.bidList.size() > 0);

    CBid lastBid = bidHistory.bidList.last();
    if (lastBid.rules.size() > 0)
    {
        //Status.
        Forcing forcing = lastBid.rules[0]->getStatus();
        QStringList labels;
        labels << tr("Non Forcing") << tr("Forcing") << tr("Game Forcing") << tr("Must Pass");
        features += tr("Status: ") + labels[forcing];

        for (int i = 0; i < lastBid.rules.size(); i++)
        {
            features += "\n";

            CFeatures lowFeatures;
            CFeatures highFeatures;
            lastBid.rules[i]->getFeatures(&lowFeatures, &highFeatures);

            //HCP.
            int hcpLow = lowFeatures.getHcp(ANY);
            int hcpHigh = highFeatures.getHcp(ANY);
            if ((hcpLow > 0) || (hcpHigh < lowFeatures.getMaxHcp(ANY)))
            {
                if (hcpHigh == lowFeatures.getMaxHcp(ANY))
                    features += QString(tr("HCP: ") + "%1  ").arg(hcpLow);
                else
                    features += QString(tr("HCP: ") + "%1-%2  ").arg(hcpLow).arg(hcpHigh);
            }

            //DP.
            int dpLow = lowFeatures.getDp(ANY);
            int dpHigh = highFeatures.getDp(ANY);
            if ((dpLow > 0) || (dpHigh < lowFeatures.getMaxDp()))
            {
                if (dpHigh == lowFeatures.getMaxDp())
                    features += QString(tr("DP: ") + "%1  ").arg(dpLow);
                else
                    features += QString(tr("DP: ") + "%1-%2  ").arg(dpLow).arg(dpHigh);
            }

            //TP.
            int tpLow = lowFeatures.getPoints(ANY);
            int tpHigh = highFeatures.getPoints(ANY);
            if ((tpLow > 0) || (tpHigh < lowFeatures.getMaxPoints()))
            {
                if (dpHigh == lowFeatures.getMaxDp())
                    features += QString(tr("TP: ") + "%1+  ").arg(dpLow);
                else
                    features += QString(tr("TP: ") + "%1-%2  ").arg(dpLow).arg(dpHigh);
            }

            //Suits.
            for (int suit = 0; suit < 4; suit++)
            {
                QString suitName = QCoreApplication::translate("defines", SUIT_NAMES[suit]);
                int lLow = lowFeatures.getSuitLen((Suit)suit);
                int lHigh = highFeatures.getSuitLen((Suit)suit);
                if ((lLow > 0) || (lHigh < lowFeatures.getMaxSuitLen()))
                {
                    if (lHigh == lowFeatures.getMaxSuitLen())
                        features += QString(suitName + ": %1+  ").arg(lLow);
                    else
                        features += QString(suitName + ": %1-%2  ").arg(lLow).arg(lHigh);
                }
                else
                {
                    Suit low = ANY;
                    Suit high = ANY;
                    for (int i = 0; i < 4; i++)
                    if ((Suit)i != suit)
                    {
                        int difLow = lowFeatures.getDif2((Suit)suit, (Suit)i);
                        int difHigh = highFeatures.getDif2((Suit)suit, (Suit)i);
                        if (difHigh <= 0)
                            high = (Suit)i;
                        else if (difLow >= 0)
                            low = (Suit)i;
                    }

                    if ((low != ANY) || (high != ANY))
                    {
                        if (low = ANY)
                            features += QString(suitName + ": 0-%1  ").
                                    arg(QCoreApplication::translate("defines", SUIT_NAMES[high]));
                        else if (high == ANY)
                            features += QString(suitName + ": %1+  ").
                                    arg(QCoreApplication::translate("defines", SUIT_NAMES[low]));
                        else
                            features += QString(suitName + ": %1-%2  ").
                                    arg(QCoreApplication::translate("defines", SUIT_NAMES[low])).
                                    arg(QCoreApplication::translate("defines", SUIT_NAMES[high]));
                    }
                }
            }

            //Aces.
            int aceLow = lowFeatures.getCountCard(ANY, ACE);
            int aceHigh = highFeatures.getCountCard(ANY, ACE);
            if ((aceLow > 0) || (aceHigh < lowFeatures.getMaxCountCard(ANY)))
            {
                if (aceHigh == lowFeatures.getMaxCountCard(ANY))
                    features += QString(tr("A: ") + "%1+  ").arg(aceLow);
                else
                    features += QString(tr("A: ") + "%1-%2  ").arg(aceLow).arg(aceHigh);
            }

            //Kings.
            int kingLow = lowFeatures.getCountCard(ANY, KING);
            int kingHigh = highFeatures.getCountCard(ANY, KING);
            if ((kingLow > 0) || (kingHigh < lowFeatures.getMaxCountCard(ANY)))
            {
                if (kingHigh == lowFeatures.getMaxCountCard(ANY))
                    features += QString(tr("K: ") + "%1+  ").arg(kingLow);
                else
                    features += QString(tr("K: ") + "%1-%2  ").arg(kingLow).arg(kingHigh);
            }
        }
    }

    return features;
}

QString CBidAndPlay::alertOfLastBid()
{
    assert (bidHistory.bidList.size() > 0);

    CBid lastBid = bidHistory.bidList.last();

    return lastBid.alert;
}
