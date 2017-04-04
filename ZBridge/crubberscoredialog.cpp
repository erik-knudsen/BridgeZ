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
        rubberDone = games->getRubberPoints(i, &game,
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

    //Respond to update of games
    connect(games, &CGamesDoc::sUpdateGame, this, &CRubberScoreDialog::sUpdateTable, Qt::QueuedConnection);
}

CRubberScoreDialog::~CRubberScoreDialog()
{
    delete [] rubberPoints;
    delete ui;
}

void CRubberScoreDialog::sUpdateTable()
{
    close();
}

void CRubberScoreDialog::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    int lineSize = 20;
    int columnSize = 60;

    //Player texts.
    QString nsTxt = tr("NS");
    QString ewTxt = tr("EW");

    //Column texts.
    QString boardTxt = tr("Board");
    QString contractTxt = tr("Contract");
    QString tricksTxt = tr("Tricks");
    QString scoreTxt = tr("Score");
    QString totalTxt = tr("Total");
    QString ledgerTxt = tr("Ledger");

    int headLine2Y = 2 * lineSize;    //Y coordinate for column texts.
    //Column X coordinates.
    int nsBoardX = columnSize / 3;
    int nsContractX = nsBoardX + columnSize;
    int nsTricksX = nsContractX + columnSize;
    int nsScoreX = nsTricksX + columnSize;
    int ewScoreX = nsScoreX + columnSize + columnSize / 3;
    int ewTricksX = ewScoreX + columnSize;
    int ewContractX = ewTricksX + columnSize;
    int ewBoardX = ewContractX + columnSize;

    //Player text coordinates.
    int headLine1Y = lineSize;
    int nsX = (nsBoardX + nsScoreX + columnSize) / 2;
    int ewX = (ewScoreX + ewBoardX + columnSize) / 2;

    //Horizontal separator line coordinates.
    int sepLineX1 = 0;
    int sepLineX2 = ewBoardX + columnSize;
    int sepLineHeadY = headLine2Y + lineSize / 2;

    //Vertical separator line coordinates.
    int sepVertX = nsScoreX + columnSize;
    int sepVert1Y = 0;

    //Determine plays in the chosen rubber.
    int firstPlay, lastPlay;
    lastPlay = index;
    for (firstPlay = lastPlay - 1; firstPlay >= 0; firstPlay--)
        if (rubberPoints[firstPlay].rubberDone)
            break;
    if (firstPlay > 0)
        firstPlay++;
    if (firstPlay < 0)
        firstPlay = 0;

    //Determine total number of above and below lines.
    int minAbove = 7;       //Minimum above lines.
    int noNSAbove = 0, noNSBelow = 0, noEWAbove = 0, noEWBelow = 0;
    for (int i = firstPlay; i <= lastPlay; i++)
    {
        if (rubberPoints[i].nsAbove > 0)
            noNSAbove++;
        if (rubberPoints[i].ewAbove > 0)
            noEWAbove++;
        if (rubberPoints[i].nsBelow > 0)
            noNSBelow++;
        if (rubberPoints[i].ewBelow > 0)
            noEWBelow++;
        if (rubberPoints[i].gameDone)
        {
            noNSBelow++;
            noEWBelow++;
            if (noNSBelow > noEWBelow)
                noEWBelow = noNSBelow;
            else
                noNSBelow = noEWBelow;
        }
    }
    int noAbove = (noNSAbove > noEWAbove) ? (noNSAbove) : (noEWAbove);
    if (noAbove < minAbove)
        noAbove = minAbove;

    int noBelow = (noNSBelow > noEWBelow) ? (noNSBelow) : (noEWBelow);
    if (rubberPoints[lastPlay].rubberDone)
        noBelow += 1;

    //Y coordinates of first above and first below and separator line.
    int firstAboveY = sepLineHeadY + noAbove * lineSize;
    int aboveLineY = firstAboveY + lineSize / 2;
    int firstBelowY = aboveLineY + lineSize;

    //Vertical separator bottom Y coordinate.
    int sepVert2Y = firstBelowY + noBelow * lineSize;

    //Resize to show all content.
    resize(sepLineX2, sepVert2Y);

    //Draw player headline texts.
    painter.drawText(nsX, headLine1Y, nsTxt);
    painter.drawText(ewX, headLine1Y, ewTxt);

    //Draw column texts.
    painter.drawText(nsBoardX, headLine2Y, boardTxt);
    painter.drawText(nsContractX, headLine2Y, contractTxt);
    painter.drawText(nsTricksX, headLine2Y, tricksTxt);
    painter.drawText(nsScoreX, headLine2Y, scoreTxt);

    painter.drawText(ewScoreX, headLine2Y, scoreTxt);
    painter.drawText(ewTricksX, headLine2Y, tricksTxt);
    painter.drawText(ewContractX, headLine2Y, contractTxt);
    painter.drawText(ewBoardX, headLine2Y, boardTxt);

    //Draw headline separator line, player separator line and above/below separator line.
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);
    pen.setBrush(Qt::black);
    painter.setPen(pen);
    painter.drawLine(sepLineX1, sepLineHeadY, sepLineX2, sepLineHeadY);
    painter.drawLine(sepVertX, sepVert1Y, sepVertX, sepVert2Y);
    painter.drawLine(sepLineX1, aboveLineY, sepLineX2, aboveLineY);

    //Draw plays in the rubber.
    pen.setWidth(1);
    painter.setPen(pen);
    int nsAboveY = 0, ewAboveY = 0, nsBelowY = 0, ewBelowY = 0;
    for (int i = firstPlay; i <= lastPlay; i++)
    if (rubberPoints[i].declarer != NO_SEAT)
    {
        //Contract etc..
        Team vulnerable = rubberPoints[i].vulnerable;
        Suit suit = BID_SUIT(rubberPoints[i].contract);
        int level = BID_LEVEL(rubberPoints[i].contract);
        Seat declarer = rubberPoints[i].declarer;
        QString contract = QString("%1 %2%3").arg(QCoreApplication::translate("defines", SEAT_NAMES[declarer])[0]).
                arg(level).
                arg(QCoreApplication::translate("defines", SUIT_NAMES[suit]));
        if (rubberPoints[i].contractModifier == BID_DOUBLE)
            contract += " X";
        else if (rubberPoints[i].contractModifier == BID_REDOUBLE)
            contract += " XX";

        if (rubberPoints[i].nsAbove != 0)
        {
            int y = firstAboveY - nsAboveY;

            //Board.
            painter.drawText(nsBoardX, y, QString("%1").arg(rubberPoints[i].board));

            //Contract.
            if ((vulnerable == BOTH) ||
                 ((vulnerable == NORTH_SOUTH) && ((declarer == NORTH_SEAT) || (declarer == SOUTH_SEAT))) ||
                 ((vulnerable == EAST_WEST) && ((declarer == WEST_SEAT) || (declarer == EAST_SEAT))))
            {
                pen.setBrush(Qt::red);
                painter.setPen(pen);
                painter.drawText(nsContractX, y, contract);
                pen.setBrush(Qt::black);
                painter.setPen(pen);
            }
            else
                painter.drawText(nsContractX, y, contract);

            //Result.
            painter.drawText(nsTricksX, y, QString("%1").arg(rubberPoints[i].tricks));

            //Score.
            painter.drawText(nsScoreX, y, QString("%1").arg(rubberPoints[i].nsAbove));

            nsAboveY += lineSize;
        }
        if (rubberPoints[i].ewAbove != 0)
        {
            int y = firstAboveY - ewAboveY;

            //Board.
            painter.drawText(ewBoardX, y, QString("%1").arg(rubberPoints[i].board));

            //Contract.
            if ((vulnerable == BOTH) ||
                 ((vulnerable == NORTH_SOUTH) && ((declarer == NORTH_SEAT) || (declarer == SOUTH_SEAT))) ||
                 ((vulnerable == EAST_WEST) && ((declarer == WEST_SEAT) || (declarer == EAST_SEAT))))
            {
                pen.setBrush(Qt::red);
                painter.setPen(pen);
                painter.drawText(ewContractX, y, contract);
                pen.setBrush(Qt::black);
                painter.setPen(pen);
            }
            else
                painter.drawText(ewContractX, y, contract);

            //Result.
            painter.drawText(ewTricksX, y, QString("%1").arg(rubberPoints[i].tricks));

            //Score.
            painter.drawText(ewScoreX, y, QString("%1").arg(rubberPoints[i].ewAbove));

            ewAboveY += lineSize;
        }
        if (rubberPoints[i].nsBelow != 0)
        {
            int y = firstBelowY + nsBelowY;

            //Board.
            painter.drawText(nsBoardX, y, QString("%1").arg(rubberPoints[i].board));

            //Contract.
            if ((vulnerable == BOTH) ||
                 ((vulnerable == NORTH_SOUTH) && ((declarer == NORTH_SEAT) || (declarer == SOUTH_SEAT))) ||
                 ((vulnerable == EAST_WEST) && ((declarer == WEST_SEAT) || (declarer == EAST_SEAT))))
            {
                pen.setBrush(Qt::red);
                painter.setPen(pen);
                painter.drawText(nsContractX, y, contract);
                pen.setBrush(Qt::black);
                painter.setPen(pen);
            }
            else
                painter.drawText(nsContractX, y, contract);

            //Result.
            painter.drawText(nsTricksX, y, QString("%1").arg(rubberPoints[i].tricks));

            //Score.
            painter.drawText(nsScoreX, y, QString("%1").arg(rubberPoints[i].nsBelow));

            nsBelowY += lineSize;
        }
        if (rubberPoints[i].ewBelow != 0)
        {
            int y = firstBelowY + ewBelowY;

            //Board.
            painter.drawText(ewBoardX, y, QString("%1").arg(rubberPoints[i].board));

            //Contract.
            if ((vulnerable == BOTH) ||
                 ((vulnerable == NORTH_SOUTH) && ((declarer == NORTH_SEAT) || (declarer == SOUTH_SEAT))) ||
                 ((vulnerable == EAST_WEST) && ((declarer == WEST_SEAT) || (declarer == EAST_SEAT))))
            {
                pen.setBrush(Qt::red);
                painter.setPen(pen);
                painter.drawText(ewContractX, y, contract);
                pen.setBrush(Qt::black);
                painter.setPen(pen);
            }
            else
                painter.drawText(ewContractX, y, contract);

            //Result.
            painter.drawText(ewTricksX, y, QString("%1").arg(rubberPoints[i].tricks));

            //Score.
            painter.drawText(ewScoreX, y, QString("%1").arg(rubberPoints[i].ewBelow));

            ewBelowY += lineSize;
        }
        if (rubberPoints[i].gameDone && !rubberPoints[i].rubberDone)
        {
            nsBelowY += lineSize / 2;
            ewBelowY += lineSize / 2;
            if (nsBelowY > ewBelowY)
                ewBelowY = nsBelowY;
            else
                nsBelowY = ewBelowY;

            int y = firstBelowY + ewBelowY - lineSize;
            painter.drawLine(sepLineX1, y, sepLineX2, y);
        }
        if (rubberPoints[i].rubberDone)
        {
            pen.setWidth(3);
            painter.setPen(pen);

            //Separator line.
            int y = (nsBelowY > ewBelowY) ? (nsBelowY) : (ewBelowY);
            y += firstBelowY - lineSize / 2;
            painter.drawLine(sepLineX1, y, sepLineX2, y);

            //Total.
            y += lineSize;
            painter.drawText(nsContractX, y, totalTxt);
            painter.drawText(nsScoreX, y, QString("%1").arg(rubberPoints[i].nsTotal));
            painter.drawText(ewScoreX, y, QString("%1").arg(rubberPoints[i].ewTotal));

            //Ledger.
            y += lineSize;
            painter.drawText(nsContractX, y, ledgerTxt);
            painter.drawText(nsScoreX, y, QString("%1").arg(rubberPoints[i].nsLedger));
            painter.drawText(ewScoreX, y, QString("%1").arg(rubberPoints[i].ewLedger));
        }
    }
}
