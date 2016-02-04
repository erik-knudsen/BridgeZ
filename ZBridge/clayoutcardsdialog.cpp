/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CLayOutCardsDialog.h
  Developers: eak

  Revision History:
  30-jan-2016 eak: Original

  Abstract: Lay out cards dialog class.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the lay out cards dialog class.
 */

#include "cgamesdoc.h"
#include "clayoutcardsdialog.h"
#include "ui_clayoutcardsdialog.h"

CLayoutCardsDialog::CLayoutCardsDialog(CGamesDoc *games, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLayoutCardsDialog)
{
    ui->setupUi(this);

    //Cards faces.
    faces[0] = tr("A");
    faces[1] = tr("K");
    faces[2] = tr("Q");
    faces[3] = tr("J");
    faces[4] = tr("T");
    faces[5] = tr("9");
    faces[6] = tr("8");
    faces[7] = tr("7");
    faces[8] = tr("6");
    faces[9] = tr("5");
    faces[10] = tr("4");
    faces[11] = tr("3");
    faces[12] = tr("2");

    //No cards are dealt initially.
    for (int i= 0; i < 4; i++)
    for (int j = 0; j < 13; j++)
        buttons[i][j].isDealt = false;

    //Pointer to buttons (indexed by color, face).
    buttons[SPADES][12].pButton = ui->SA;
    buttons[SPADES][11].pButton = ui->SK;
    buttons[SPADES][10].pButton = ui->SQ;
    buttons[SPADES][9].pButton = ui->SJ;
    buttons[SPADES][8].pButton = ui->ST;
    buttons[SPADES][7].pButton = ui->S9;
    buttons[SPADES][6].pButton = ui->S8;
    buttons[SPADES][5].pButton = ui->S7;
    buttons[SPADES][4].pButton = ui->S6;
    buttons[SPADES][3].pButton = ui->S5;
    buttons[SPADES][2].pButton = ui->S4;
    buttons[SPADES][1].pButton = ui->S3;
    buttons[SPADES][0].pButton = ui->S2;

    buttons[HEARTS][12].pButton = ui->HA;
    buttons[HEARTS][11].pButton = ui->HK;
    buttons[HEARTS][10].pButton = ui->HQ;
    buttons[HEARTS][9].pButton = ui->HJ;
    buttons[HEARTS][8].pButton = ui->HT;
    buttons[HEARTS][7].pButton = ui->H9;
    buttons[HEARTS][6].pButton = ui->H8;
    buttons[HEARTS][5].pButton = ui->H7;
    buttons[HEARTS][4].pButton = ui->H6;
    buttons[HEARTS][3].pButton = ui->H5;
    buttons[HEARTS][2].pButton = ui->H4;
    buttons[HEARTS][1].pButton = ui->H3;
    buttons[HEARTS][0].pButton = ui->H2;

    buttons[DIAMONDS][12].pButton = ui->DA;
    buttons[DIAMONDS][11].pButton = ui->DK;
    buttons[DIAMONDS][10].pButton = ui->DQ;
    buttons[DIAMONDS][9].pButton = ui->DJ;
    buttons[DIAMONDS][8].pButton = ui->DT;
    buttons[DIAMONDS][7].pButton = ui->D9;
    buttons[DIAMONDS][6].pButton = ui->D8;
    buttons[DIAMONDS][5].pButton = ui->D7;
    buttons[DIAMONDS][4].pButton = ui->D6;
    buttons[DIAMONDS][3].pButton = ui->D5;
    buttons[DIAMONDS][2].pButton = ui->D4;
    buttons[DIAMONDS][1].pButton = ui->D3;
    buttons[DIAMONDS][0].pButton = ui->D2;

    buttons[CLUBS][12].pButton = ui->CA;
    buttons[CLUBS][11].pButton = ui->CK;
    buttons[CLUBS][10].pButton = ui->CQ;
    buttons[CLUBS][9].pButton = ui->CJ;
    buttons[CLUBS][8].pButton = ui->CT;
    buttons[CLUBS][7].pButton = ui->C9;
    buttons[CLUBS][6].pButton = ui->C8;
    buttons[CLUBS][5].pButton = ui->C7;
    buttons[CLUBS][4].pButton = ui->C6;
    buttons[CLUBS][3].pButton = ui->C5;
    buttons[CLUBS][2].pButton = ui->C4;
    buttons[CLUBS][1].pButton = ui->C3;
    buttons[CLUBS][0].pButton = ui->C2;
}

CLayoutCardsDialog::~CLayoutCardsDialog()
{
    delete ui;
}

void CLayoutCardsDialog::on_west_clicked()
{

}

void CLayoutCardsDialog::on_north_clicked()
{

}

void CLayoutCardsDialog::on_east_clicked()
{

}

void CLayoutCardsDialog::on_south_clicked()
{

}

void CLayoutCardsDialog::on_SA_clicked()
{

}

void CLayoutCardsDialog::on_SK_clicked()
{

}

void CLayoutCardsDialog::on_SQ_clicked()
{

}

void CLayoutCardsDialog::on_SJ_clicked()
{

}

void CLayoutCardsDialog::on_ST_clicked()
{

}

void CLayoutCardsDialog::on_S9_clicked()
{

}

void CLayoutCardsDialog::on_S8_clicked()
{

}

void CLayoutCardsDialog::on_S7_clicked()
{

}

void CLayoutCardsDialog::on_S6_clicked()
{

}

void CLayoutCardsDialog::on_S5_clicked()
{

}

void CLayoutCardsDialog::on_S4_clicked()
{

}

void CLayoutCardsDialog::on_S3_clicked()
{

}

void CLayoutCardsDialog::on_S2_clicked()
{

}

void CLayoutCardsDialog::on_HA_clicked()
{

}

void CLayoutCardsDialog::on_HK_clicked()
{

}

void CLayoutCardsDialog::on_HQ_clicked()
{

}

void CLayoutCardsDialog::on_HJ_clicked()
{

}

void CLayoutCardsDialog::on_HT_clicked()
{

}

void CLayoutCardsDialog::on_H9_clicked()
{

}

void CLayoutCardsDialog::on_H8_clicked()
{

}

void CLayoutCardsDialog::on_H7_clicked()
{

}

void CLayoutCardsDialog::on_H6_clicked()
{

}

void CLayoutCardsDialog::on_H5_clicked()
{

}

void CLayoutCardsDialog::on_H4_clicked()
{

}

void CLayoutCardsDialog::on_H3_clicked()
{

}

void CLayoutCardsDialog::on_H2_clicked()
{

}

void CLayoutCardsDialog::on_DA_clicked()
{

}

void CLayoutCardsDialog::on_DK_clicked()
{

}

void CLayoutCardsDialog::on_DQ_clicked()
{

}

void CLayoutCardsDialog::on_DJ_clicked()
{

}

void CLayoutCardsDialog::on_DT_clicked()
{

}

void CLayoutCardsDialog::on_D9_clicked()
{

}

void CLayoutCardsDialog::on_D8_clicked()
{

}

void CLayoutCardsDialog::on_D7_clicked()
{

}

void CLayoutCardsDialog::on_D6_clicked()
{

}

void CLayoutCardsDialog::on_D5_clicked()
{

}

void CLayoutCardsDialog::on_D4_clicked()
{

}

void CLayoutCardsDialog::on_D3_clicked()
{

}

void CLayoutCardsDialog::on_D2_clicked()
{

}

void CLayoutCardsDialog::on_CA_clicked()
{

}

void CLayoutCardsDialog::on_CK_clicked()
{

}

void CLayoutCardsDialog::on_CQ_clicked()
{

}

void CLayoutCardsDialog::on_CJ_clicked()
{

}

void CLayoutCardsDialog::on_CT_clicked()
{

}

void CLayoutCardsDialog::on_C9_clicked()
{

}

void CLayoutCardsDialog::on_C8_clicked()
{

}

void CLayoutCardsDialog::on_C7_clicked()
{

}

void CLayoutCardsDialog::on_C6_clicked()
{

}

void CLayoutCardsDialog::on_C5_clicked()
{

}

void CLayoutCardsDialog::on_C4_clicked()
{

}

void CLayoutCardsDialog::on_C3_clicked()
{

}

void CLayoutCardsDialog::on_C2_clicked()
{

}

void CLayoutCardsDialog::on_backward_clicked()
{

}

void CLayoutCardsDialog::on_forward_clicked()
{

}

void CLayoutCardsDialog::on_spinBoard_valueChanged(const QString &arg1)
{

}

void CLayoutCardsDialog::on_spinDealer_valueChanged(const QString &arg1)
{

}

void CLayoutCardsDialog::on_spinVul_valueChanged(const QString &arg1)
{

}

void CLayoutCardsDialog::on_clearDeal_clicked()
{

}

void CLayoutCardsDialog::on_dealRemaining_clicked()
{

}

void CLayoutCardsDialog::on_buttonBox_accepted()
{

}

void CLayoutCardsDialog::on_buttonBox_rejected()
{

}
