/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CPlayDialog.cpp
  Developers: eak

  Revision History:
  4-feb-2015 eak: Original

  Abstract: Show play for a given board.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of a class for showing the play
 * (auction and play) for one board.
 */

#include "Defines.h"
#include "cplayview.h"
#include "cgamesdoc.h"
#include "cbidhistory.h"
#include "cplayhistory.h"
#include "cplaydialog.h"
#include "ui_cplaydialog.h"

/**
 * @brief The constructor sets up the display.
 *
 * Set window title.Allocate view, show auction and play info, show the cards of the 4 hands,
 * show the bids, initialize for traversing the play.
 */
CPlayDialog::CPlayDialog(CGamesDoc *games, int gameIndex, int auctionIndex, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPlayDialog)
{
    //Save info.
    this->games = games;
    this->gameIndex = gameIndex;
    this->auctionIndex = auctionIndex;

    //Setup user interface.
    ui->setupUi(this);

    //Customize window.
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    //Set window title.
    QString westName, northName, eastName, southName;
    games->getActorNames(gameIndex, auctionIndex,
                         &westName, &northName, &eastName, &southName);
    QString players = QString("%1-%2 / %3-%4").arg(northName).arg(southName).arg(eastName).arg(westName);
    setWindowTitle(tr("Players (N-S / E-W): ") + players);

    //Allocate view for showing the auction and play.
    playView = new CPlayView(this);

    //Set size for windows.
    resize(QSize(SCENE_HOR_SIZE + 30, SCENE_VER_SIZE + 130));
    playView->resize(QSize(SCENE_HOR_SIZE + 30, SCENE_VER_SIZE + 130));
    setMinimumSize(QSize(SCENE_HOR_SIZE * .75, SCENE_VER_SIZE * .75));
    setMaximumSize(QSize(SCENE_HOR_SIZE * 1.25, SCENE_VER_SIZE * 1.25));

    //Get and show auction and play info for current game.
    games->getGame(gameIndex, &board, &dealer, &vulnerable, wCards, nCards, eCards, sCards);
    QString str;
    str.setNum(board);
    games->getAuctionAndPlay(gameIndex, auctionIndex,
                    &declarer, &contract, &dbl, &result, &bidHistory, &playHistory);
    playView->setInfoPlay(str, vulnerable, dealer, declarer, contract, dbl);
    playView->showNSTricks(0);
    playView->showEWTricks(0);
    playView->showInfoAuctionPlay(true);

    //Show center widget.
    playView->showEWNSTextOnTable();
    if ((vulnerable == NORTH_SOUTH) || (vulnerable == BOTH))
        playView->showNSVulnerableOnTable();
    if ((vulnerable == EAST_WEST) || (vulnerable == BOTH))
        playView->showEWVulnerableOnTable();

    playView->showDummyOnTable((Seat)((declarer + 2) & 3));

    //Show the cards in play view.
    bool showWest, showNorth, showEast, showSouth;
    showWest = showNorth = showEast = showSouth = true;
    playView->setAndShowAllCards(true, showWest, wCards, true, showNorth, nCards,
                        true, showEast, eCards, true, showSouth, sCards);

    if (contract != BID_NONE)
    {
        //Show bids.
        QListIterator<CBid> bidItr(bidHistory.bidList);
        while (bidItr.hasNext())
        {
            CBid bid = bidItr.next();
            playView->showBid(bid.bidder, bid.bid);
            playView->showBid((Seat)((bid.bidder + 1) & 3), BID_PLAYER);
        }
        if (contract != BID_PASS)
        {
            //Rearrange display of cards for the contracts trumpsuit.
            playView->setTrumpSuit(BID_SUIT(contract));

            //Initialize for traversing cards (first player is leader). No cards played yet and no tricks taken yet.
            player = (Seat)((declarer + 1) & 3);
            playNo = trickNo = 0;
            playHistory.getTrick(trickNo, trick);

            //Are there any cards to traverse?
            if (trick[player] != -1)
            {
                //Dialog for traversing play of cards.
                playShow = new CPlayShow(this);
                playShow->show();
                connect(playShow, &CPlayShow::playValue, this, &CPlayDialog::playValue);
                connect(playShow, &CPlayShow::playClose, this, &CPlayDialog::playClose);
                playShow->setEnabled(REVIEW_PREV, false);
            }
        }
    }
}

CPlayDialog::~CPlayDialog()
{
    delete ui;
}

/**
 * @brief Handle resize of play dialog (and children) windows.
 *
 * @param resizeEvent Parameters for resize.
 */
void CPlayDialog::resizeEvent(QResizeEvent *resizeEvent)
{
    QSize size = resizeEvent->size();
    QSize oldSize = resizeEvent->oldSize();
    if (oldSize.width() > 0)
    {
       float scaleXFactor = float(size.width() - 30)/(oldSize.width() - 30);
       float scaleYFactor = float(size.height() - 130)/(oldSize.height() - 130);
       playView->resize(size);
       playView->scale(scaleXFactor, scaleYFactor);
    }
}

/**
 * @brief Slot for forward, backward signal. Shows the card play sequentially on the CPlayView view.
 */
void CPlayDialog::playValue(ReviewVal reviewVal)
{
    //Step forward?
    if (reviewVal == REVIEW_NEXT)
    {
        //Complete trick already shown?
        if (playNo == 4)
        {
            int ewTricks, nsTricks;

            //Get info for the trick. Clear trick from center display and show trick info.
            playHistory.getTrickInfo(trickNo, ewTricks, nsTricks, player);
            playView->clearCardsOnTable();
            playView->showNSTricks(nsTricks);
            playView->showEWTricks(ewTricks);

            //Is this the last trick?
            if (trickNo == (playHistory.getNoTrick() - 1))
            {
                playShow->setEnabled(REVIEW_NEXT, false);
                return;
            }

            //Update for the next trick.
            trickNo++;
            playHistory.getTrick(trickNo, trick);
            playNo = 0;
        }

        //Check that the next card is actually played.
        if (trick[player] != -1)
        {
            //Reached the first play?
            if ((playNo == 0) && (trickNo == 0))
                playShow->setEnabled(REVIEW_PREV, true);

            //Show played card on center display.
            playView->showCardOnTable(player, trick[player]);

            //Clear played card from hand.
            playView->clearCard(player, trick[player]);

            //Update for next player.
            player = (Seat)((player + 1) & 3);
            playNo++;
        }
    }

    //Step backward.
    else
    {
        //Backward step from last position.
        if (!playShow->isEnabled(REVIEW_NEXT))
        {
            int ewTricks, nsTricks;
            playShow->setEnabled(REVIEW_NEXT, true);

            //Show the 4 cards of the last trick on center display.
            //Get trick info and show the trick.
            playView->undoCard(player, false);
            playHistory.getTrickInfo(trickNo - 1, ewTricks, nsTricks, player);
            playView->showNSTricks(nsTricks);
            playView->showEWTricks(ewTricks);
            return;
        }

        //Update for previous player and unstack played card.
        player = (Seat)((player - 1) & 3);
        playNo--;
        playView->undoCard(player, true);

        //At the end?
        if ((playNo == 0) && (trickNo == 0))
        {
            playShow->setEnabled(REVIEW_PREV, false);
            return;
        }

        //Previous trick?
        if (playNo == 0)
        {
            int ewTricks, nsTricks;

            //Get trick info.
                trickNo--;
                if (trickNo > 0)
                    playHistory.getTrickInfo(trickNo - 1, ewTricks, nsTricks, player);
                else
                {
                    ewTricks = nsTricks = 0;
                    player = declarer;
                }
                //Show trick and get trick.
                playView->showNSTricks(nsTricks);
                playView->showEWTricks(ewTricks);
                playHistory.getTrick(trickNo, trick);
                playNo = 4;
        }
    }
}

void CPlayDialog::playClose()
{
    close();
}
