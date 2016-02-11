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

#include "mt19937ar.h"
#include "clayoutcardsdialog.h"
#include "ui_clayoutcardsdialog.h"

/**
 * @brief Constructor for the layout card class.
 * @param games Pointer to the games to edit.
 * @param parent Parent of the class.
 *
 * Initialization is performed and the first deal to edit is shown.
 */
CLayoutCardsDialog::CLayoutCardsDialog(CGamesDoc *games, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLayoutCardsDialog)
{
    this->games = games;

    ui->setupUi(this);

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

    //Pointer to hands.
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

    //Combo boxes with dealer and vulnerability.
    QStringList labels;
    labels << tr("West") << tr("North") << tr("East") << tr("South");
    ui->dealer->addItems(labels);

    labels.clear();
    labels << tr("Neither") << tr("North/South") << tr("East/West") << tr("Both");
    ui->vul->addItems(labels);

    //Set up the first deal to edit.
    relInx = 0;
    setCurrentDeal();
    upDateSelectButtons();
}

CLayoutCardsDialog::~CLayoutCardsDialog()
{
    delete ui;
}

/**
 * @brief Current seat is set to west.
 */
void CLayoutCardsDialog::on_west_clicked()
{
    currentSeat = WEST_SEAT;
    selectSeat(currentSeat, pSeat);
}

/**
 * @brief Current seat is set to north.
 */
void CLayoutCardsDialog::on_north_clicked()
{
    currentSeat = NORTH_SEAT;
    selectSeat(currentSeat, pSeat);
}

/**
 * @brief Current seat is set to east.
 */
void CLayoutCardsDialog::on_east_clicked()
{
    currentSeat = EAST_SEAT;
    selectSeat(currentSeat, pSeat);
}

/**
 * @brief Current seat is set to south.
 */
void CLayoutCardsDialog::on_south_clicked()
{
    currentSeat = SOUTH_SEAT;
    selectSeat(currentSeat, pSeat);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_SA_clicked()
{
    cardClicked(SPADES, 12);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_SK_clicked()
{
    cardClicked(SPADES, 11);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_SQ_clicked()
{
    cardClicked(SPADES, 10);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_SJ_clicked()
{
    cardClicked(SPADES, 9);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_ST_clicked()
{
    cardClicked(SPADES, 8);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_S9_clicked()
{
    cardClicked(SPADES, 7);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_S8_clicked()
{
    cardClicked(SPADES, 6);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_S7_clicked()
{
    cardClicked(SPADES, 5);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_S6_clicked()
{
    cardClicked(SPADES, 4);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_S5_clicked()
{
    cardClicked(SPADES, 3);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_S4_clicked()
{
    cardClicked(SPADES, 2);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_S3_clicked()
{
    cardClicked(SPADES, 1);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_S2_clicked()
{
    cardClicked(SPADES, 0);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_HA_clicked()
{
    cardClicked(HEARTS, 12);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_HK_clicked()
{
    cardClicked(HEARTS, 11);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_HQ_clicked()
{
    cardClicked(HEARTS, 10);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_HJ_clicked()
{
    cardClicked(HEARTS, 9);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_HT_clicked()
{
    cardClicked(HEARTS, 8);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_H9_clicked()
{
    cardClicked(HEARTS, 7);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_H8_clicked()
{
    cardClicked(HEARTS, 6);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_H7_clicked()
{
    cardClicked(HEARTS, 5);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_H6_clicked()
{
    cardClicked(HEARTS, 4);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_H5_clicked()
{
    cardClicked(HEARTS, 3);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_H4_clicked()
{
    cardClicked(HEARTS, 2);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_H3_clicked()
{
    cardClicked(HEARTS, 1);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_H2_clicked()
{
    cardClicked(HEARTS, 0);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_DA_clicked()
{
    cardClicked(DIAMONDS, 12);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_DK_clicked()
{
    cardClicked(DIAMONDS, 11);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_DQ_clicked()
{
    cardClicked(DIAMONDS, 10);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_DJ_clicked()
{
    cardClicked(DIAMONDS, 9);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_DT_clicked()
{
    cardClicked(DIAMONDS, 8);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_D9_clicked()
{
    cardClicked(DIAMONDS, 7);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_D8_clicked()
{
    cardClicked(DIAMONDS, 6);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_D7_clicked()
{
    cardClicked(DIAMONDS, 5);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_D6_clicked()
{
    cardClicked(DIAMONDS, 4);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_D5_clicked()
{
    cardClicked(DIAMONDS, 3);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_D4_clicked()
{
    cardClicked(DIAMONDS, 2);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_D3_clicked()
{
    cardClicked(DIAMONDS, 1);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_D2_clicked()
{
    cardClicked(DIAMONDS, 0);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_CA_clicked()
{
    cardClicked(CLUBS, 12);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_CK_clicked()
{
    cardClicked(CLUBS, 11);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_CQ_clicked()
{
    cardClicked(CLUBS, 10);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_CJ_clicked()
{
    cardClicked(CLUBS, 9);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_CT_clicked()
{
    cardClicked(CLUBS, 8);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_C9_clicked()
{
    cardClicked(CLUBS, 7);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_C8_clicked()
{
    cardClicked(CLUBS, 6);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_C7_clicked()
{
    cardClicked(CLUBS, 5);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_C6_clicked()
{
    cardClicked(CLUBS, 4);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_C5_clicked()
{
    cardClicked(CLUBS, 3);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_C4_clicked()
{
    cardClicked(CLUBS, 2);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_C3_clicked()
{
    cardClicked(CLUBS, 1);
}

/**
 * @brief Card to deal or retract was clicked.
 */
void CLayoutCardsDialog::on_C2_clicked()
{
    cardClicked(CLUBS, 0);
}

/**
 * @brief ealer was changed.
 * @param index
 */
void CLayoutCardsDialog::on_dealer_currentIndexChanged(int index)
{
    dealChanged = true;
    upDateSelectButtons();
}

/**
 * @brief Vulnerability was changed.
 * @param arg1
 */
void CLayoutCardsDialog::on_vul_currentIndexChanged(int index)
{
    dealChanged = true;
    upDateSelectButtons();
}

/**
 * @brief Edit first deal.
 */
void CLayoutCardsDialog::on_first_clicked()
{
    bool none = ((count[0] == 0) && (count[1] == 0) && (count[2] == 0) && (count[3] == 0));
    bool all = ((count[0] == 13) && (count[1] == 13) && (count[2] == 13) && (count[3] == 13));
    assert((relInx != 0) && (none || all));

    if (all && dealChanged)
    {
        getCurrentDeal(cards, &dealer, &vul);
        games->setDeal(relInx, board, cards, dealer, vul);
    }
    relInx = 0;
    setCurrentDeal();
    upDateSelectButtons();
}

/**
 * @brief Edit previous deal.
 */
void CLayoutCardsDialog::on_backward_clicked()
{
    bool none = ((count[0] == 0) && (count[1] == 0) && (count[2] == 0) && (count[3] == 0));
    bool all = ((count[0] == 13) && (count[1] == 13) && (count[2] == 13) && (count[3] == 13));
    assert((relInx != 0) && (none || all));

    if (all && dealChanged)
    {
        getCurrentDeal(cards, &dealer, &vul);
        games->setDeal(relInx, board, cards, dealer, vul);
    }
    relInx--;
    setCurrentDeal();
    upDateSelectButtons();
}

/**
 * @brief Edit next deal.
 */
void CLayoutCardsDialog::on_forward_clicked()
{
    bool none = ((count[0] == 0) && (count[1] == 0) && (count[2] == 0) && (count[3] == 0));
    bool all = ((count[0] == 13) && (count[1] == 13) && (count[2] == 13) && (count[3] == 13));
    assert(none || all);

    if (all && dealChanged)
    {
        getCurrentDeal(cards, &dealer, &vul);
        games->setDeal(relInx, board, cards, dealer, vul);
    }
    relInx++;
    setCurrentDeal();
    upDateSelectButtons();
}

/**
 * @brief Edit last deal.
 */
void CLayoutCardsDialog::on_last_clicked()
{
    bool none = ((count[0] == 0) && (count[1] == 0) && (count[2] == 0) && (count[3] == 0));
    bool all = ((count[0] == 13) && (count[1] == 13) && (count[2] == 13) && (count[3] == 13));
    assert(none || all);

    if (all && dealChanged)
    {
        getCurrentDeal(cards, &dealer, &vul);
        games->setDeal(relInx, board, cards, dealer, vul);
    }
    relInx = games->getNumberOfNotPlayedGames(0);
    setCurrentDeal();
    upDateSelectButtons();
}

/**
 * @brief Clear current deal.
 */
void CLayoutCardsDialog::on_clearDeal_clicked()
{  
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 13; j++)
        if (buttons[i][j].isDealt)
            cardClicked((Suit)i, j);
}

/**
 * @brief Deal remaing cards (random) in current deal.
 */
void CLayoutCardsDialog::on_dealRemaining_clicked()
{
    Seat oldSeat = currentSeat;
    int cardValues[52];
    int noCard = 0;

    //Find cards remaining to be dealt.
    for (int k = 0; k < 4; k++)
    for (int j = 0; j < 13; j++)
        if (!buttons[k][j].isDealt)
        {
            int card = MAKE_CARD(k, j);
            cardValues[noCard++] = card;
        }

    for (int i = 0; i < 4; i++)
    {
        //For seat calculate count of cards already dealt to this seat.
        int count = 0;
        for (int k = 0; k < 4; k++)
            for (int j = 0; j < 13; j++)
                if (cCards[i][k][j] != -1)
                    count++;

        currentSeat = (Seat)i;

        //Deal remaing cards for the seat.
        for (int j = count; j < 13; j++)
        {
            assert(noCard != 0);

            int inx = genrand_int32()%(noCard);
            int card = cardValues[inx];
            Suit suit = CARD_SUIT(card);
            int face = CARD_FACE(card);
            cardClicked(suit, face);

            --noCard;
            for (int k = inx; k < noCard; k++)
                cardValues[k] = cardValues[k + 1];
        }
    }
    currentSeat = oldSeat;
}

/**
 * @brief Accept deals.
 */
void CLayoutCardsDialog::on_buttonBox_accepted()
{
    bool none = ((count[0] == 0) && (count[1] == 0) && (count[2] == 0) && (count[3] == 0));
    bool all = ((count[0] == 13) && (count[1] == 13) && (count[2] == 13) && (count[3] == 13));
    assert(none || all);

    if (all && dealChanged)
    {
        getCurrentDeal(cards, &dealer, &vul);
        games->setDeal(relInx, board, cards, dealer, vul);
    }
}

/**
 * @brief Cancel edits.
 */
void CLayoutCardsDialog::on_buttonBox_rejected()
{
}

/**
 * @brief Insert card in currently dealt card.
 * @param card The card to insert.
 * @param cCards Has the currently dealt cards for a given seat and will be updated.
 */
void CLayoutCardsDialog::insertCard(int card, int cCards[4][13])
{
    Suit suit = CARD_SUIT(card);
    int face = CARD_FACE(card);

    assert ((suit != ANY) && (face != -1));

    insertSorted(face, cCards[suit]);
}

/**
 * @brief Insert face sorted in currently dealt faces.
 * @param face The face to insert.
 * @param cCards Has the currently dealt cards for a given seat and suit and will be updated.
 */
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

/**
 * @brief Remove a card from the dealt cards.
 * @param suit Suit of the card.
 * @param face Face of the card.
 * @param[out] seat Returns the seat from where the card was removed.
 */
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

/**
 * @brief Select the current seat.
 * @param currentSeat The seat to select as the current seat.
 * @param pSeat Pointer to seat buttons.
 */
void CLayoutCardsDialog::selectSeat(Seat currentSeat, QPushButton *pSeat[4])
{
    for (int i = 0; i < 4; i++)
        pSeat[i]->setChecked(i == currentSeat);
}

/**
 * @brief Generate the currently selected faces, given the seat and suit.
 * @param[out] text The generated text.
 * @param cCards Currently selected faces for a given seat, suit.
 */
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

/**
 * @brief A card was clicked.
 * @param suit The suit of the clicked card.
 * @param face The face of the clicked card.
 */
void CLayoutCardsDialog::cardClicked(Suit suit, int face)
{
    if (buttons[suit][face].isDealt)
    {
        if (dealComplete)
        {
            dealComplete = false;
            updateSelect = true;
        }

        dealChanged = true;

        //Move it from dealt to not dealt.
        Seat seat;
        removeCard(suit, face, &seat);

        count[seat]--;

        QString text;
        handsText(text, cCards[seat][suit]);
        pHands[seat][suit]->setText(text);

        buttons[suit][face].isDealt = false;
        buttons[suit][face].pButton->setText(faces[face]);

        upDateSelectButtons();
    }
    else if (count[currentSeat] < 13)
    {
        //Move it from not dealt to dealt.
        if (dealComplete)
        {
            dealComplete = false;
            updateSelect = true;
        }

        dealChanged = true;

        count[currentSeat]++;

        QString text;
        insertSorted(face, cCards[currentSeat][suit]);
        handsText(text, cCards[currentSeat][suit]);
        pHands[currentSeat][suit]->setText(text);

        buttons[suit][face].isDealt = true;
        buttons[suit][face].pButton->setText(" ");

        upDateSelectButtons();
    }
    else
        QMessageBox::warning(0, tr("ZBridge"), tr("Hand is full."));
}

/**
 * @brief Enable/disable relevant buttons according to state.
 */
void CLayoutCardsDialog::upDateSelectButtons()
{
    bool none = ((count[0] == 0) && (count[1] == 0) && (count[2] == 0) && (count[3] == 0));
    bool all = ((count[0] == 13) && (count[1] == 13) && (count[2] == 13) && (count[3] == 13));
    if (none || all)
    {
        dealComplete = true;
        updateSelect = true;
    }

    if (!updateSelect)
        return;

    updateSelect = false;

    QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);

    ui->clearDeal->setDisabled(none);
    ui->dealRemaining->setDisabled(all);

    int noEditable = games->getNumberOfNotPlayedGames(0);
    if (((noEditable == 0) && !all) || (!none && !all))
    {
        ui->backward->setDisabled(true);
        ui->forward->setDisabled(true);
        ui->first->setDisabled(true);
        ui->last->setDisabled(true);
        okButton->setDisabled(true);
    }
    else if (all || none)
    {
        ui->backward->setDisabled(relInx == 0);
        ui->forward->setDisabled((relInx == noEditable) && none);
        ui->first->setDisabled(relInx == 0);
        ui->last->setDisabled((relInx == noEditable) && none);
        okButton->setDisabled(!dealChanged);
    }
}

/**
 * @brief Get the currently edited deal in a format suitable for storing.
 * @param[out] cards The cards of the deal.
 * @param[out] dealer The dealer.
 * @param[out] vulnerable The vulnerability.
 */
void CLayoutCardsDialog::getCurrentDeal(int cards[4][13], Seat *dealer, Team *vulnerable)
{
    int m;

    for (int i = 0; i < 4; i++)
    {
        m = 0;
        for (int k = 0; k < 4; k++)
        for (int j = 0; j < 13; j++)
        {
            if (cCards[i][k][j] == -1)
                break;
            cards[i][m++] = MAKE_CARD(k, cCards[i][k][j]);
            assert(m  <= 13);
        }
        assert( m == 13);
    }
    *dealer = (Seat)ui->dealer->currentIndex();
    *vulnerable = (Team)ui->vul->currentIndex();
}

/**
 * @brief Set a current deal in the internal format.
 */
void CLayoutCardsDialog::setCurrentDeal()
{
    for (int i = 0; i < 4; i++)
    for (int k = 0; k < 4; k++)
    for (int j = 0; j < 13; j++)
       cCards[i][k][j] = -1;

    dealChanged = false;

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

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 13; j++)
                insertCard(cards[i][j], cCards[i]);

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
        dealer = Seat((board - 1) % 4);
        vul = Team(((board - 1) % 4 + board/4)%4);

        for (int i = 0; i < 4; i++)
            count[i] = 0;

        for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
        {
            buttons[i][j].isDealt = false;
            buttons[i][j].pButton->setText(faces[j]);
        }

        for (int i = 0; i < 4; i++)
        for (int k = 0; k < 4; k++)
            pHands[i][k]->setText(" ");
    }

    ui->board->setText(QString::number(board));
    currentSeat = WEST_SEAT;
    selectSeat(currentSeat, pSeat);
    ui->dealer->setCurrentIndex(dealer);
    ui->vul->setCurrentIndex(vul);
}
