/* Erik Aagaard Knudsen.
  Copyright © 2014 - All Rights Reserved

  Project: ZBridge
  File: CRubberScoreDialog.cpp
  Developers: eak

  Revision History:
  9-aug-2014 eak: Original

  Abstract: Rubber score dialog.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the rubber score dialog class.
 */

#include <QPainter>

#include "cgamesdoc.h"
#include "crubberscoredialog.h"
#include "ui_crubberscoredialog.h"

CRubberScoreDialog::CRubberScoreDialog(CGamesDoc *games, int index, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRubberScoreDialog)
{
    ui->setupUi(this);

    this->games = games;
    this->index = index;

    int playedAuctionAndPlayIndex = games->getPlayedAuctionAndPlayIndex(index);

    rubberPoints = new rubberPoint[index + 1];

    for (int i = 0; i <= index; i++)
    {
        int board;
        Bids contract, contractModifier;
        int tricks;
        Seat declarer;
        Team vulnerable;
        int nsAbove, nsBelow, nsTotal, nsLedger;
        int ewAbove, ewBelow, ewTotal, ewLedger;
        bool game, rubberDone;
        rubberDone = games->getRubberPoints(i, playedAuctionAndPlayIndex, &game,
                                            &board, &contract, &contractModifier, &tricks,
                                            &declarer, &vulnerable,
                                            &nsAbove, &nsBelow, &nsTotal, &nsLedger,
                                            &ewAbove, &ewBelow, &ewTotal, &ewLedger);
        rubberPoints[i].gameDone = game;
        rubberPoints[i].rubberDone = rubberDone;
        rubberPoints[i].board = board;
        rubberPoints[i].contract = contract;
        rubberPoints[i].contractModifier;
        rubberPoints[i].tricks = tricks;
        rubberPoints[i].declarer = declarer;
        rubberPoints[i].vulnerable = vulnerable;
        rubberPoints[i].nsAbove = nsAbove;
        rubberPoints[i].nsBelow = nsBelow;
        rubberPoints[i].nsTotal = nsTotal;
        rubberPoints[i].nsLedger = nsLedger;
        rubberPoints[i].ewAbove = ewAbove;
        rubberPoints[i].ewBelow = ewBelow;
        rubberPoints[i].ewTotal = ewTotal;
        rubberPoints[i].ewLedger = ewLedger;
    }
}

CRubberScoreDialog::~CRubberScoreDialog()
{
    delete [] rubberPoints;
    delete ui;
}

void CRubberScoreDialog::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    QString nsTxt = tr("NS");
    QString ewTxt = tr("EW");

    QString boardTxt = tr("Board");
    QString contractTxt = tr("Contract");
    QString tricksTxt = tr("Tricks");
    QString scoreTxt = tr("Score");

    int headLine2Y = 40;
    int nsBoardX = 10;
    int nsContractX = nsBoardX + 7 * boardTxt.size() + 10;
    int nsTricksX = nsContractX + 7 * contractTxt.size() + 10;
    int nsScoreX = nsTricksX + 7 * tricksTxt.size() + 10;
    int ewScoreX = nsScoreX + 7 * scoreTxt.size() + 20;
    int ewTricksX = ewScoreX + 7 * scoreTxt.size() + 10;
    int ewContractX = ewTricksX + 7 * tricksTxt.size() + 10;
    int ewBoardX = ewContractX + 7 * contractTxt.size() + 10;

    int headLine1Y = 20;
    int nsX = (nsBoardX + nsScoreX + 7 * scoreTxt.size()) / 2;
    int ewX = (ewScoreX + ewBoardX + 7 * boardTxt.size()) / 2;

    int sepLineHeadX1 = 0;
    int sepLineHeadX2 = ewBoardX + 7 * boardTxt.size() + 10;
    int sepLineHeadY = headLine2Y + 10;

    int sepVertX = (nsScoreX + 6 * scoreTxt.size() + ewScoreX) / 2;
    int sepVert1Y = 0;;
    int sepVert2Y = 270;

    resize(sepLineHeadX2, sepVert2Y);

    painter.drawText(nsX, headLine1Y, nsTxt);
    painter.drawText(ewX, headLine1Y, ewTxt);

    painter.drawText(nsBoardX, headLine2Y, boardTxt);
    painter.drawText(nsContractX, headLine2Y, contractTxt);
    painter.drawText(nsTricksX, headLine2Y, tricksTxt);
    painter.drawText(nsScoreX, headLine2Y, scoreTxt);

    painter.drawText(ewScoreX, headLine2Y, scoreTxt);
    painter.drawText(ewTricksX, headLine2Y, tricksTxt);
    painter.drawText(ewContractX, headLine2Y, contractTxt);
    painter.drawText(ewBoardX, headLine2Y, boardTxt);

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);
    pen.setBrush(Qt::black);

    painter.setPen(pen);
    painter.drawLine(sepLineHeadX1, sepLineHeadY, sepLineHeadX2, sepLineHeadY);
    painter.drawLine(sepVertX, sepVert1Y, sepVertX, sepVert2Y);
}
