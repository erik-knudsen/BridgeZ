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
 * The file implements the declaration of the lay out cards dialog class.
 */

#ifndef CLAYOUTCARDSDIALOG_H
#define CLAYOUTCARDSDIALOG_H

#include <QDialog>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>

#include "Defines.h"
#include "cgamesdoc.h"

class CGamesDoc;

namespace Ui {
class CLayoutCardsDialog;
}

class CLayoutCardsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CLayoutCardsDialog(CGamesDoc *games, QWidget *parent = 0);
    ~CLayoutCardsDialog();

private slots:
    void on_west_clicked();
    void on_north_clicked();
    void on_east_clicked();
    void on_south_clicked();
    void on_SA_clicked();
    void on_SK_clicked();
    void on_SQ_clicked();
    void on_SJ_clicked();
    void on_ST_clicked();
    void on_S9_clicked();
    void on_S8_clicked();
    void on_S7_clicked();
    void on_S6_clicked();
    void on_S5_clicked();
    void on_S4_clicked();
    void on_S3_clicked();
    void on_S2_clicked();
    void on_HA_clicked();
    void on_HK_clicked();
    void on_HQ_clicked();
    void on_HJ_clicked();
    void on_HT_clicked();
    void on_H9_clicked();
    void on_H8_clicked();
    void on_H7_clicked();
    void on_H6_clicked();
    void on_H5_clicked();
    void on_H4_clicked();
    void on_H3_clicked();
    void on_H2_clicked();
    void on_DA_clicked();
    void on_DK_clicked();
    void on_DQ_clicked();
    void on_DJ_clicked();
    void on_DT_clicked();
    void on_D9_clicked();
    void on_D8_clicked();
    void on_D7_clicked();
    void on_D6_clicked();
    void on_D5_clicked();
    void on_D4_clicked();
    void on_D3_clicked();
    void on_D2_clicked();
    void on_CA_clicked();
    void on_CK_clicked();
    void on_CQ_clicked();
    void on_CJ_clicked();
    void on_CT_clicked();
    void on_C9_clicked();
    void on_C8_clicked();
    void on_C7_clicked();
    void on_C6_clicked();
    void on_C5_clicked();
    void on_C4_clicked();
    void on_C3_clicked();
    void on_C2_clicked();
    void on_first_clicked();
    void on_backward_clicked();
    void on_forward_clicked();
    void on_last_clicked();
    void on_clearDeal_clicked();
    void on_dealRemaining_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    void initialize();
    void insertCard(int card, int cCards[4][13]);
    void insertSorted(int face, int cCards[13]);
    void removeCard(Suit suit, int face, Seat *seat);
    void selectSeat(Seat currentSeat, QPushButton *pSeat[4]);
    void handsText(QString &text, int cCards[13]);
    void cardClicked(Suit suit, int face);
    void upDateSelectButtons();

    Ui::CLayoutCardsDialog *ui;
    CGamesDoc *games;
    struct buttons
    {
        bool isDealt;
        QToolButton *pButton;
    } buttons[4][13];
    QString faces[13];

    int board;
    Seat dealer;
    Team vul;
    int cards[4][13];
    int cCards[4][4][13];
    QLabel *pHands[4][4];

    int relInx;
    Seat currentSeat;
    QPushButton *pSeat[4];
    int count[4];

    bool gameChanged;
    bool updateSelect;
};

#endif // CLAYOUTCARDSDIALOG_H
