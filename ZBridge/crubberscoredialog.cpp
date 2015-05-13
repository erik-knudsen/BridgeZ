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

    struct rubberPoint
    {
        bool gameDone, rubberDone;
        int nsAbove, nsBelow, nsTotal, nsLedger;
        int ewAbove, ewBelow, ewTotal, ewLedger;
    };

    rubberPoint *rubberPoints = new rubberPoint[index + 1];

    for (int i = 0; i <= index; i++)
    {
        int nsAbove, nsBelow, nsTotal, nsLedger;
        int ewAbove, ewBelow, ewTotal, ewLedger;
        bool game, rubberDone;
        rubberDone = games->getRubberPoints(i, playedAuctionAndPlayIndex, &game,
                                            &nsAbove, &nsBelow, &nsTotal, &nsLedger,
                                            &ewAbove, &ewBelow, &ewTotal, &ewLedger);
        rubberPoints[i].gameDone = game;
        rubberPoints[i].rubberDone = rubberDone;
        rubberPoints[i].nsAbove = nsAbove;
        rubberPoints[i].nsBelow = nsBelow;
        rubberPoints[i].nsTotal = nsTotal;
        rubberPoints[i].nsLedger = nsLedger;
        rubberPoints[i].ewAbove = ewAbove;
        rubberPoints[i].ewBelow = ewBelow;
        rubberPoints[i].ewTotal = ewTotal;
        rubberPoints[i].ewLedger = ewLedger;
    }

    delete[] rubberPoints;
}

CRubberScoreDialog::~CRubberScoreDialog()
{
    delete ui;
}
