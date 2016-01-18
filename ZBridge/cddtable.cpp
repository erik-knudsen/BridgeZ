/*Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CDDTable.cpp
  Developers: eak

  Revision History:
  14-jan-2016 eak: Original

  Abstract: Calculate and show double dummy results and par.

  Platforms: Qt.

*/

/**
 * \file
 * Calculate and show double dummy results and par(definition).
 */

#include <QMessageBox>
#include <QString>

#include "dll.h"
#include "cddtable.h"
#include "ui_cddtable.h"

CDDTable::CDDTable(int cards[][13], Team vulnerable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDDTable)
{
    ui->setupUi(this);

    ddTableDeal tableDeal;
    ddTableResults table;
    parResults pRes;

    for (int h = 0; h < DDS_HANDS; h++)
      for (int s = 0; s < DDS_SUITS; s++)
        tableDeal.cards[h][s] = 0;

    //Format according to dds specs.
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
        {
            int hand = (i + 3) % 4;
            int card = cards[i][j];
            Suit suit = CARD_SUIT(card);
            int face = CARD_FACE(card);
            tableDeal.cards[hand][3 - suit] |= (1 << (face + 2));
        }

    int res = CalcDDtable(tableDeal, &table);
    if (res != RETURN_NO_FAULT)
    {
        char line[80];
        ErrorMessage(res, line);;

        QMessageBox::critical(0, tr("ZBridge"), tr("DDS error: ") + QString(line));
    }

    int vul = (vulnerable == NEITHER) ? (0) : (vulnerable == NORTH_SOUTH) ? (2) :
              (vulnerable == EAST_WEST) ? (3) : (1);
    res = Par(&table, &pRes, vul);

    QString txt;

    //N-S NT.
    if (table.resTable[4][0] == table.resTable[4][2])
        txt = QString::number(table.resTable[4][0]);
    else
        txt = QString("%1 - %2").arg(table.resTable[4][0]).arg(table.resTable[4][2]);
    ui->tricksNTNS->setText(txt);

    //E-W NT
    if (table.resTable[4][1] == table.resTable[4][3])
        txt = QString::number(table.resTable[4][1]);
    else
        txt = QString("%1 - %2").arg(table.resTable[4][1]).arg(table.resTable[4][3]);
    ui->tricksNTEW->setText(txt);

    //N-S S.
    if (table.resTable[0][0] == table.resTable[0][2])
        txt = QString::number(table.resTable[0][0]);
    else
        txt = QString("%1 - %2").arg(table.resTable[0][0]).arg(table.resTable[0][2]);
    ui->tricksSNS->setText(txt);

    //E-W S.
    if (table.resTable[0][1] == table.resTable[0][3])
        txt = QString::number(table.resTable[0][1]);
    else
        txt = QString("%1 - %2").arg(table.resTable[0][1]).arg(table.resTable[0][3]);
    ui->tricksSEW->setText(txt);

    //N-S H.
    if (table.resTable[1][0] == table.resTable[1][2])
        txt = QString::number(table.resTable[1][0]);
    else
        txt = QString("%1 - %2").arg(table.resTable[1][0]).arg(table.resTable[1][2]);
    ui->tricksHNS->setText(txt);

    //E-W H.
    if (table.resTable[1][1] == table.resTable[1][3])
        txt = QString::number(table.resTable[1][1]);
    else
        txt = QString("%1 - %2").arg(table.resTable[1][1]).arg(table.resTable[1][3]);
    ui->tricksHEW->setText(txt);

    //N-S D.
    if (table.resTable[2][0] == table.resTable[2][2])
        txt = QString::number(table.resTable[2][0]);
    else
        txt = QString("%1 - %2").arg(table.resTable[2][0]).arg(table.resTable[2][2]);
    ui->tricksDNS->setText(txt);

    //E-W D.
    if (table.resTable[2][1] == table.resTable[2][3])
        txt = QString::number(table.resTable[2][1]);
    else
        txt = QString("%1 - %2").arg(table.resTable[2][1]).arg(table.resTable[2][3]);
    ui->tricksDEW->setText(txt);

    //N-S C.
    if (table.resTable[3][0] == table.resTable[3][2])
        txt = QString::number(table.resTable[3][0]);
    else
        txt = QString("%1 - %2").arg(table.resTable[3][0]).arg(table.resTable[3][2]);
    ui->tricksCNS->setText(txt);

    //E-W C.
    if (table.resTable[3][1] == table.resTable[3][3])
        txt = QString::number(table.resTable[3][1]);
    else
        txt = QString("%1 - %2").arg(table.resTable[3][1]).arg(table.resTable[3][3]);
    ui->tricksCEW->setText(txt);

    //Par contract.
    txt = QString(pRes.parContractsString[0]);
    ui->parContract->setText(txt);

    //Par score.
    txt = QString(pRes.parScore[0]);
    ui->parScore->setText(txt);
}

CDDTable::~CDDTable()
{
    delete ui;
}
