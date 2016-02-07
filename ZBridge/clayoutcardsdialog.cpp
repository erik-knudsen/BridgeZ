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

#include <cassert>
#include <QMessageBox>

#include "cgamesdoc.h"
#include "clayoutcardsdialog.h"
#include "ui_clayoutcardsdialog.h"

CLayoutCardsDialog::CLayoutCardsDialog(CGamesDoc *games, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLayoutCardsDialog)
{
    ui->setupUi(this);

    //Initial seat selected.
    currentSeat = WEST_SEAT;

    //Cards faces.
    faces[0] = tr("2");
    faces[1] = tr("3");
    faces[2] = tr("4");
    faces[3] = tr("5");
    faces[4] = tr("6");
    faces[5] = tr("7");
    faces[6] = tr("8");
    faces[7] = tr("9");
    faces[8] = tr("T");
    faces[9] = tr("J");
    faces[10] = tr("Q");
    faces[11] = tr("K");
    faces[12] = tr("A");

    //Seats.
    pSeat[WEST_SEAT] = ui->west;
    pSeat[NORTH_SEAT] = ui->north;
    pSeat[EAST_SEAT] = ui->east;
    pSeat[SOUTH_SEAT] = ui->south;
    for (int i = 0; i < 4; i++)
        pSeat[i]->setCheckable(true);

    //No cards are dealt initially.
    for (int i = 0; i < 4; i++)
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

    pHands[WEST_SEAT][SPADES] = ui->westSpade;
    pHands[WEST_SEAT][HEARTS] = ui->westHeart;
    pHands[WEST_SEAT][DIAMONDS] = ui->westDiamond;
    pHands[WEST_SEAT][CLUBS] = ui->westClub;

    pHands[NORTH_SEAT][SPADES] = ui->northSpade;
    pHands[NORTH_SEAT][HEARTS] = ui->northHeart;
    pHands[NORTH_SEAT][DIAMONDS] = ui->northDiamond;
    pHands[NORTH_SEAT][CLUBS] = ui->northClub;

    pHands[EAST_SEAT][SPADES] = ui->eastSpade;
    pHands[EAST_SEAT][HEARTS] = ui->eastHeart;
    pHands[EAST_SEAT][DIAMONDS] = ui->eastDiamond;
    pHands[EAST_SEAT][CLUBS] = ui->eastClub;

    pHands[SOUTH_SEAT][SPADES] = ui->southSpade;
    pHands[SOUTH_SEAT][HEARTS] = ui->southHeart;
    pHands[SOUTH_SEAT][DIAMONDS] = ui->southDiamond;
    pHands[SOUTH_SEAT][CLUBS] = ui->southClub;

    for (int i = 0; i < 4; i++)
    for (int k = 0; k < 4; k++)
    for (int j = 0; j < 13; j++)
       cCards[i][k][j] = -1;

    relInx = 0;
    if (games->getDeal(relInx, &board, cards, &dealer, &vul))
    {
        for (int i = 0; i < 4; i++)
            count[i] = 13;

        for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
        {
            buttons[i][j].isDealt = true;
            buttons[i][j].pButton->setText(" ");
        }
        initialize();

        for (int i = 0; i < 4; i++)
        for (int k = 0; k < 4; k++)
        {
            QString text;
            handsText(text, cCards[i][k]);
            pHands[i][k]->setText(text);
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
            count[i] = 0;

        for (int i = 0; i < 4; i++)
        for (int k = 0; k < 4; k++)
            pHands[i][k]->setText(" ");

        dealer = Seat((board - 1) % 4);
        vul = Team(((board - 1) % 4 + board/4)%4);
    }
    ui->board->setText(QString::number(board));
    selectSeat(currentSeat, pSeat);
    QStringList labels;
    labels << tr("West") << tr("North") << tr("East") << tr("South");
    ui->dealer->addItems(labels);
    ui->dealer->setCurrentIndex(dealer);

    labels.clear();
    labels << tr("Neither") << tr("North/South") << tr("East/West") << tr("Both");
    ui->vul->addItems(labels);
    ui->vul->setCurrentIndex(vul);
}

CLayoutCardsDialog::~CLayoutCardsDialog()
{
    delete ui;
}

void CLayoutCardsDialog::on_west_clicked()
{
    currentSeat = WEST_SEAT;
    selectSeat(currentSeat, pSeat);
}

void CLayoutCardsDialog::on_north_clicked()
{
    currentSeat = NORTH_SEAT;
    selectSeat(currentSeat, pSeat);
}

void CLayoutCardsDialog::on_east_clicked()
{
    currentSeat = EAST_SEAT;
    selectSeat(currentSeat, pSeat);
}

void CLayoutCardsDialog::on_south_clicked()
{
    currentSeat = SOUTH_SEAT;
    selectSeat(currentSeat, pSeat);
}

void CLayoutCardsDialog::on_SA_clicked()
{
    cardClicked(SPADES, 12);
}

void CLayoutCardsDialog::on_SK_clicked()
{
    cardClicked(SPADES, 11);
}

void CLayoutCardsDialog::on_SQ_clicked()
{
    cardClicked(SPADES, 10);
}

void CLayoutCardsDialog::on_SJ_clicked()
{
    cardClicked(SPADES, 9);
}

void CLayoutCardsDialog::on_ST_clicked()
{
    cardClicked(SPADES, 8);
}

void CLayoutCardsDialog::on_S9_clicked()
{
    cardClicked(SPADES, 7);
}

void CLayoutCardsDialog::on_S8_clicked()
{
    cardClicked(SPADES, 6);
}

void CLayoutCardsDialog::on_S7_clicked()
{
    cardClicked(SPADES, 5);
}

void CLayoutCardsDialog::on_S6_clicked()
{
    cardClicked(SPADES, 4);
}

void CLayoutCardsDialog::on_S5_clicked()
{
    cardClicked(SPADES, 3);
}

void CLayoutCardsDialog::on_S4_clicked()
{
    cardClicked(SPADES, 2);
}

void CLayoutCardsDialog::on_S3_clicked()
{
    cardClicked(SPADES, 1);
}

void CLayoutCardsDialog::on_S2_clicked()
{
    cardClicked(SPADES, 0);
}

void CLayoutCardsDialog::on_HA_clicked()
{
    cardClicked(HEARTS, 12);
}

void CLayoutCardsDialog::on_HK_clicked()
{
    cardClicked(HEARTS, 11);
}

void CLayoutCardsDialog::on_HQ_clicked()
{
    cardClicked(HEARTS, 10);
}

void CLayoutCardsDialog::on_HJ_clicked()
{
    cardClicked(HEARTS, 9);
}

void CLayoutCardsDialog::on_HT_clicked()
{
    cardClicked(HEARTS, 8);
}

void CLayoutCardsDialog::on_H9_clicked()
{
    cardClicked(HEARTS, 7);
}

void CLayoutCardsDialog::on_H8_clicked()
{
    cardClicked(HEARTS, 6);
}

void CLayoutCardsDialog::on_H7_clicked()
{
    cardClicked(HEARTS, 5);
}

void CLayoutCardsDialog::on_H6_clicked()
{
    cardClicked(HEARTS, 4);
}

void CLayoutCardsDialog::on_H5_clicked()
{
    cardClicked(HEARTS, 3);
}

void CLayoutCardsDialog::on_H4_clicked()
{
    cardClicked(HEARTS, 2);
}

void CLayoutCardsDialog::on_H3_clicked()
{
    cardClicked(HEARTS, 1);
}

void CLayoutCardsDialog::on_H2_clicked()
{
    cardClicked(HEARTS, 0);
}

void CLayoutCardsDialog::on_DA_clicked()
{
    cardClicked(DIAMONDS, 12);
}

void CLayoutCardsDialog::on_DK_clicked()
{
    cardClicked(DIAMONDS, 11);
}

void CLayoutCardsDialog::on_DQ_clicked()
{
    cardClicked(DIAMONDS, 10);
}

void CLayoutCardsDialog::on_DJ_clicked()
{
    cardClicked(DIAMONDS, 9);
}

void CLayoutCardsDialog::on_DT_clicked()
{
    cardClicked(DIAMONDS, 8);
}

void CLayoutCardsDialog::on_D9_clicked()
{
    cardClicked(DIAMONDS, 7);
}

void CLayoutCardsDialog::on_D8_clicked()
{
    cardClicked(DIAMONDS, 6);
}

void CLayoutCardsDialog::on_D7_clicked()
{
    cardClicked(DIAMONDS, 5);
}

void CLayoutCardsDialog::on_D6_clicked()
{
    cardClicked(DIAMONDS, 4);
}

void CLayoutCardsDialog::on_D5_clicked()
{
    cardClicked(DIAMONDS, 3);
}

void CLayoutCardsDialog::on_D4_clicked()
{
    cardClicked(DIAMONDS, 2);
}

void CLayoutCardsDialog::on_D3_clicked()
{
    cardClicked(DIAMONDS, 2);
}

void CLayoutCardsDialog::on_D2_clicked()
{
    cardClicked(DIAMONDS, 0);
}

void CLayoutCardsDialog::on_CA_clicked()
{
    cardClicked(CLUBS, 12);
}

void CLayoutCardsDialog::on_CK_clicked()
{
    cardClicked(CLUBS, 11);
}

void CLayoutCardsDialog::on_CQ_clicked()
{
    cardClicked(CLUBS, 10);
}

void CLayoutCardsDialog::on_CJ_clicked()
{
    cardClicked(CLUBS, 9);
}

void CLayoutCardsDialog::on_CT_clicked()
{
    cardClicked(CLUBS, 8);
}

void CLayoutCardsDialog::on_C9_clicked()
{
    cardClicked(CLUBS, 7);
}

void CLayoutCardsDialog::on_C8_clicked()
{
    cardClicked(CLUBS, 6);
}

void CLayoutCardsDialog::on_C7_clicked()
{
    cardClicked(CLUBS, 5);
}

void CLayoutCardsDialog::on_C6_clicked()
{
    cardClicked(CLUBS, 4);
}

void CLayoutCardsDialog::on_C5_clicked()
{
    cardClicked(CLUBS, 3);
}

void CLayoutCardsDialog::on_C4_clicked()
{
    cardClicked(CLUBS, 2);
}

void CLayoutCardsDialog::on_C3_clicked()
{
    cardClicked(CLUBS, 1);
}

void CLayoutCardsDialog::on_C2_clicked()
{
    cardClicked(CLUBS, 0);
}

void CLayoutCardsDialog::on_backward_clicked()
{
}

void CLayoutCardsDialog::on_forward_clicked()
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

void CLayoutCardsDialog::initialize()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
            insertCard(cards[i][j], cCards[i]);
}

void CLayoutCardsDialog::insertCard(int card, int cCards[4][13])
{
    Suit suit = CARD_SUIT(card);
    int face = CARD_FACE(card);

    assert ((suit != ANY) && (face != -1));

    insertSorted(face, cCards[suit]);
}

void CLayoutCardsDialog::insertSorted(int face, int cCards[13])
{
    int inx;

    for (inx = 0; inx < 13; inx++)
    if ((cCards[inx] == - 1) || (face > cCards[inx]))
            break;

    assert(inx != 13);

    for (int i = 12; i > inx; i--)
        cCards[i] = cCards[i - 1];

    cCards[inx] = face;
}

void CLayoutCardsDialog::removeCard(Suit suit, int face, Seat *seat)
{
    int i, j;

    for (i = 0; (i < 4); i++)
    {
        for (j = 0; j < 13; j++)
            if (face == cCards[i][suit][j])
                break;
        if (j != 13)
            break;
    }

    assert ((i < 4) && (j < 13));

    *seat = (Seat)i;
    for (int inx = j; inx < 12; inx++)
        cCards[i][suit][inx] = cCards[i][suit][inx + 1];
    cCards[i][suit][12] = -1;
}

void CLayoutCardsDialog::selectSeat(Seat currentSeat, QPushButton *pSeat[4])
{
    for (int i = 0; i < 4; i++)
        pSeat[i]->setChecked(i == currentSeat);
}

void CLayoutCardsDialog::handsText(QString &text, int cCards[])
{
    for (int j = 0; (j < 13) && (cCards[j] != -1); j++)
    {
        int card = cCards[j];
        int face = CARD_FACE(card);
        assert(face != -1);
        text += faces[face];
    }
}

void CLayoutCardsDialog::cardClicked(Suit suit, int face)
{
    if (buttons[suit][face].isDealt)
    {
        //Move it from dealt to not dealt.
        Seat seat;
        removeCard(suit, face, &seat);

        count[seat]--;

        QString text;
        handsText(text, cCards[seat][suit]);
        pHands[seat][suit]->setText(text);

        buttons[suit][face].isDealt = false;
        buttons[suit][face].pButton->setText(faces[face]);
    }
    else if (count[currentSeat] < 13)
    {
        count[currentSeat]++;

        QString text;
        insertSorted(face, cCards[currentSeat][suit]);
        handsText(text, cCards[currentSeat][suit]);
        pHands[currentSeat][suit]->setText(text);

        buttons[suit][face].isDealt = true;
        buttons[suit][face].pButton->setText(" ");
    }
    else
        QMessageBox::warning(0, tr("ZBridge"), tr("Hand is full."));
}
