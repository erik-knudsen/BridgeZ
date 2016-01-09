/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CDealOptionsPropSheet.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Property sheet for determining deal options.

  Platforms: Qt.

*/

/**
 * \file
 * Property sheet for determining deal options (definition).
 */

#include <QMessageBox>
#include <QTime>

#include "cdealoptionspropsheet.h"
#include "ui_cdealoptionspropsheet.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"

CDealOptionsPropSheet::CDealOptionsPropSheet(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDealOptionsPropSheet)
{
    ui->setupUi(this);
    this->app = app;
    this->doc = doc;

    QString txt;

    dealOptionDoc = doc->getDealOptions();

    //High card points.
    QLineEdit *hcpLineEdit[5][4];
    hcpLineEdit[CLUBS][WEST_SEAT] = ui->hcpClubWest;
    hcpLineEdit[CLUBS][NORTH_SEAT] = ui->hcpClubNorth;
    hcpLineEdit[CLUBS][EAST_SEAT] = ui->hcpClubEast;
    hcpLineEdit[CLUBS][SOUTH_SEAT] = ui->hcpClubSouth;
    hcpLineEdit[DIAMONDS][WEST_SEAT] = ui->hcpDiamondWest;
    hcpLineEdit[DIAMONDS][NORTH_SEAT] = ui->hcpDiamondNorth;
    hcpLineEdit[DIAMONDS][EAST_SEAT] = ui->hcpDiamondEast;
    hcpLineEdit[DIAMONDS][SOUTH_SEAT] = ui->hcpDiamondSouth;
    hcpLineEdit[HEARTS][WEST_SEAT] = ui->hcpHeartWest;
    hcpLineEdit[HEARTS][NORTH_SEAT] = ui->hcpHeartNorth;
    hcpLineEdit[HEARTS][EAST_SEAT] = ui->hcpHeartEast;
    hcpLineEdit[HEARTS][SOUTH_SEAT] = ui->hcpHeartSouth;
    hcpLineEdit[SPADES][WEST_SEAT] = ui->hcpSpadeWest;
    hcpLineEdit[SPADES][NORTH_SEAT] = ui->hcpSpadeNorth;
    hcpLineEdit[SPADES][EAST_SEAT] = ui->hcpSpadeEast;
    hcpLineEdit[SPADES][SOUTH_SEAT] = ui->hcpSpadeSouth;
    hcpLineEdit[NOTRUMP][WEST_SEAT] = ui->hcpTotalWest;
    hcpLineEdit[NOTRUMP][NORTH_SEAT] = ui->hcpTotalNorth;
    hcpLineEdit[NOTRUMP][EAST_SEAT] = ui->hcpTotalEast;
    hcpLineEdit[NOTRUMP][SOUTH_SEAT] = ui->hcpTotalSouth;

    //j is suit and i is seat.
    for (int j = 0; j < 5; j++)
        for (int i = 0; i < 4; i++)
        {
            int low, high;
            low = dealOptionDoc.hcp[0][j][i];
            high = dealOptionDoc.hcp[1][j][i];
            int max = (j == NOTRUMP) ? (MAX_TOT_HCP) : (MAX_SUIT_HCP);
            if (high >= max)
                txt = QString("%1+").arg(low);
            else
                txt = QString("%1-%2").arg(low).arg(high);

            hcpLineEdit[j][i]->setText(txt);
        }

    //Suit size.
    QLineEdit *suitSizeLineEdit[4][4];
    suitSizeLineEdit[CLUBS][WEST_SEAT] = ui->suitSizeClubWest;
    suitSizeLineEdit[CLUBS][NORTH_SEAT] = ui->suitSizeClubNorth;
    suitSizeLineEdit[CLUBS][EAST_SEAT] = ui->suitSizeClubEast;
    suitSizeLineEdit[CLUBS][SOUTH_SEAT] = ui->suitSizeClubSouth;
    suitSizeLineEdit[DIAMONDS][WEST_SEAT] = ui->suitSizeDiamondWest;
    suitSizeLineEdit[DIAMONDS][NORTH_SEAT] = ui->suitSizeDiamondNorth;
    suitSizeLineEdit[DIAMONDS][EAST_SEAT] = ui->suitSizeDiamondEast;
    suitSizeLineEdit[DIAMONDS][SOUTH_SEAT] = ui->suitSizeDiamondSouth;
    suitSizeLineEdit[HEARTS][WEST_SEAT] = ui->suitSizeHeartWest;
    suitSizeLineEdit[HEARTS][NORTH_SEAT] = ui->suitSizeHeartNorth;
    suitSizeLineEdit[HEARTS][EAST_SEAT] = ui->suitSizeHeartEast;
    suitSizeLineEdit[HEARTS][SOUTH_SEAT] = ui->suitSizeHeartSouth;
    suitSizeLineEdit[SPADES][WEST_SEAT] = ui->suitSizeSpadeWest;
    suitSizeLineEdit[SPADES][NORTH_SEAT] = ui->suitSizeSpadeNorth;
    suitSizeLineEdit[SPADES][EAST_SEAT] = ui->suitSizeSpadeEast;
    suitSizeLineEdit[SPADES][SOUTH_SEAT] = ui->suitSizeSpadeSouth;

    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 4; i++)
        {
            int low, high;
            low = dealOptionDoc.suitSize[0][j][i];
            high = dealOptionDoc.suitSize[1][j][i];
            if (high >= MAX_SUIT_SIZE)
                txt = QString("%1+").arg(low);
            else
                txt = QString("%1-%2").arg(low).arg(high);

            suitSizeLineEdit[j][i]->setText(txt);
        }

    //Distribution points.
    QLineEdit *dpLineEdit[4];
    dpLineEdit[WEST_SEAT] = ui->dpWest;
    dpLineEdit[NORTH_SEAT] = ui->dpNorth;
    dpLineEdit[EAST_SEAT] = ui->dpEast;
    dpLineEdit[SOUTH_SEAT] = ui->dpSouth;

    for (int i = 0; i < 4; i++)
    {
        int low, high;
        low = dealOptionDoc.dp[0][i];
        high = dealOptionDoc.dp[1][i];
        if (high >= MAX_TOT_DP)
            txt = QString("%1+").arg(low);
        else
            txt = QString("%1-%2").arg(low).arg(high);

        dpLineEdit[i]->setText(txt);
    }
}

CDealOptionsPropSheet::~CDealOptionsPropSheet()
{
    delete ui;
}

void CDealOptionsPropSheet::on_ok_clicked()
{
    int noDeals = 100000;
    int noOk = checkDealOptions(noDeals);

    QMessageBox::information(0, tr("ZBridge"),
                             QString(tr("Found %1 deals in %2 random deals.")).arg(noOk).arg(noDeals));

    if (noOk > 0)
    {
        doc->setDealOptions(dealOptionDoc);
        accept();
    }
}

void CDealOptionsPropSheet::on_cancel_clicked()
{
    accept();
}

void CDealOptionsPropSheet::on_hcpTotalWest_editingFinished()
{
    int low, high;

    QString text = ui->hcpTotalWest->text();

    bool ok = getValues(text, MAX_TOT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpTotalWest->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][NOTRUMP][WEST_SEAT] = low;
        dealOptionDoc.hcp[1][NOTRUMP][WEST_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_hcpTotalNorth_editingFinished()
{
    int low, high;

    QString text = ui->hcpTotalNorth->text();

    bool ok = getValues(text, MAX_TOT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpTotalNorth->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][NOTRUMP][NORTH_SEAT] = low;
        dealOptionDoc.hcp[1][NOTRUMP][NORTH_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_hcpTotalEast_editingFinished()
{
    int low, high;

    QString text = ui->hcpTotalEast->text();

    bool ok = getValues(text, MAX_TOT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpTotalEast->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][NOTRUMP][EAST_SEAT] = low;
        dealOptionDoc.hcp[1][NOTRUMP][EAST_SEAT] = high;
    }

}

void CDealOptionsPropSheet::on_hcpTotalSouth_editingFinished()
{
    int low, high;

    QString text = ui->hcpTotalSouth->text();

    bool ok = getValues(text, MAX_TOT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpTotalSouth->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][NOTRUMP][SOUTH_SEAT] = low;
        dealOptionDoc.hcp[1][NOTRUMP][SOUTH_SEAT] = high;
    }

}

void CDealOptionsPropSheet::on_hcpSpadeWest_editingFinished()
{
    int low, high;

    QString text = ui->hcpSpadeWest->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpSpadeWest->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][SPADES][WEST_SEAT] = low;
        dealOptionDoc.hcp[1][SPADES][WEST_SEAT] = high;
    }

}

void CDealOptionsPropSheet::on_hcpSpadeNorth_editingFinished()
{
    int low, high;

    QString text = ui->hcpSpadeNorth->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpSpadeNorth->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][SPADES][NORTH_SEAT] = low;
        dealOptionDoc.hcp[1][SPADES][NORTH_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_hcpSpadeEast_editingFinished()
{
    int low, high;

    QString text = ui->hcpSpadeEast->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpSpadeEast->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][SPADES][EAST_SEAT] = low;
        dealOptionDoc.hcp[1][SPADES][EAST_SEAT] = high;
    }

}

void CDealOptionsPropSheet::on_hcpSpadeSouth_editingFinished()
{
    int low, high;

    QString text = ui->hcpSpadeSouth->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpSpadeSouth->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][SPADES][SOUTH_SEAT] = low;
        dealOptionDoc.hcp[1][SPADES][SOUTH_SEAT] = high;
    }

}

void CDealOptionsPropSheet::on_hcpHeartWest_editingFinished()
{
    int low, high;

    QString text = ui->hcpHeartWest->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpHeartWest->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][HEARTS][WEST_SEAT] = low;
        dealOptionDoc.hcp[1][HEARTS][WEST_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_hcpHeartNorth_editingFinished()
{
    int low, high;

    QString text = ui->hcpHeartNorth->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpHeartNorth->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][HEARTS][NORTH_SEAT] = low;
        dealOptionDoc.hcp[1][HEARTS][NORTH_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_hcpHeartEast_editingFinished()
{
    int low, high;

    QString text = ui->hcpHeartEast->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpHeartEast->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][HEARTS][EAST_SEAT] = low;
        dealOptionDoc.hcp[1][HEARTS][EAST_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_hcpHeartSouth_editingFinished()
{
    int low, high;

    QString text = ui->hcpHeartSouth->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpHeartSouth->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][HEARTS][SOUTH_SEAT] = low;
        dealOptionDoc.hcp[1][HEARTS][SOUTH_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_hcpDiamondWest_editingFinished()
{
    int low, high;

    QString text = ui->hcpDiamondWest->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpDiamondWest->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][DIAMONDS][WEST_SEAT] = low;
        dealOptionDoc.hcp[1][DIAMONDS][WEST_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_hcpDiamondNorth_editingFinished()
{
    int low, high;

    QString text = ui->hcpDiamondNorth->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpDiamondNorth->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][DIAMONDS][NORTH_SEAT] = low;
        dealOptionDoc.hcp[1][DIAMONDS][NORTH_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_hcpDiamondEast_editingFinished()
{
    int low, high;

    QString text = ui->hcpDiamondEast->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpDiamondEast->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][DIAMONDS][EAST_SEAT] = low;
        dealOptionDoc.hcp[1][DIAMONDS][EAST_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_hcpDiamondSouth_editingFinished()
{
    int low, high;

    QString text = ui->hcpDiamondSouth->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpDiamondSouth->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][DIAMONDS][SOUTH_SEAT] = low;
        dealOptionDoc.hcp[1][DIAMONDS][SOUTH_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_hcpClubWest_editingFinished()
{
    int low, high;

    QString text = ui->hcpClubWest->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpClubWest->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][CLUBS][WEST_SEAT] = low;
        dealOptionDoc.hcp[1][CLUBS][WEST_SEAT] = high;
    }

}

void CDealOptionsPropSheet::on_hcpClubNorth_editingFinished()
{
    int low, high;

    QString text = ui->hcpClubNorth->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpClubNorth->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][CLUBS][NORTH_SEAT] = low;
        dealOptionDoc.hcp[1][CLUBS][NORTH_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_hcpClubEast_editingFinished()
{
    int low, high;

    QString text = ui->hcpClubEast->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpClubEast->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][CLUBS][EAST_SEAT] = low;
        dealOptionDoc.hcp[1][CLUBS][EAST_SEAT] = high;
    }

}

void CDealOptionsPropSheet::on_hcpClubSouth_editingFinished()
{
    int low, high;

    QString text = ui->hcpClubSouth->text();

    bool ok = getValues(text, MAX_SUIT_HCP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpClubSouth->setFocus();
    }
    else
    {
        dealOptionDoc.hcp[0][CLUBS][SOUTH_SEAT] = low;
        dealOptionDoc.hcp[1][CLUBS][SOUTH_SEAT] = high;
    }

}

void CDealOptionsPropSheet::on_suitSizeSpadeWest_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeSpadeWest->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeSpadeWest->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][SPADES][WEST_SEAT] = low;
        dealOptionDoc.suitSize[1][SPADES][WEST_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_suitSizeSpadeNorth_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeSpadeNorth->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeSpadeNorth->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][SPADES][NORTH_SEAT] = low;
        dealOptionDoc.suitSize[1][SPADES][NORTH_SEAT] = high;
    }

}

void CDealOptionsPropSheet::on_suitSizeSpadeEast_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeSpadeEast->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeSpadeEast->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][SPADES][EAST_SEAT] = low;
        dealOptionDoc.suitSize[1][SPADES][EAST_SEAT] = high;
    }

}

void CDealOptionsPropSheet::on_suitSizeSpadeSouth_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeSpadeSouth->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeSpadeSouth->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][SPADES][SOUTH_SEAT] = low;
        dealOptionDoc.suitSize[1][SPADES][SOUTH_SEAT] = high;
    }

}

void CDealOptionsPropSheet::on_suitSizeHeartWest_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeHeartWest->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeHeartWest->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][HEARTS][WEST_SEAT] = low;
        dealOptionDoc.suitSize[1][HEARTS][WEST_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_suitSizeHeartNorth_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeHeartNorth->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeHeartNorth->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][HEARTS][NORTH_SEAT] = low;
        dealOptionDoc.suitSize[1][HEARTS][NORTH_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_suitSizeHeartEast_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeHeartEast->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeHeartEast->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][HEARTS][EAST_SEAT] = low;
        dealOptionDoc.suitSize[1][HEARTS][EAST_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_suitSizeHeartSouth_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeHeartSouth->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"),tr("Illegal value: ") + text);
        ui->suitSizeHeartSouth->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][HEARTS][SOUTH_SEAT] = low;
        dealOptionDoc.suitSize[1][HEARTS][SOUTH_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_suitSizeDiamondWest_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeDiamondWest->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeDiamondWest->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][DIAMONDS][WEST_SEAT] = low;
        dealOptionDoc.suitSize[1][DIAMONDS][WEST_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_suitSizeDiamondNorth_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeDiamondNorth->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeDiamondNorth->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][DIAMONDS][NORTH_SEAT] = low;
        dealOptionDoc.suitSize[1][DIAMONDS][NORTH_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_suitSizeDiamondEast_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeDiamondEast->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeDiamondEast->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][DIAMONDS][EAST_SEAT] = low;
        dealOptionDoc.suitSize[1][DIAMONDS][EAST_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_suitSizeDiamondSouth_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeDiamondSouth->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeDiamondSouth->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][DIAMONDS][SOUTH_SEAT] = low;
        dealOptionDoc.suitSize[1][DIAMONDS][SOUTH_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_suitSizeClubWest_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeClubWest->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeClubWest->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][CLUBS][WEST_SEAT] = low;
        dealOptionDoc.suitSize[1][CLUBS][WEST_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_suitSizeClubNorth_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeClubNorth->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeClubNorth->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][CLUBS][NORTH_SEAT] = low;
        dealOptionDoc.suitSize[1][CLUBS][NORTH_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_suitSizeClubEast_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeClubEast->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeClubEast->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][CLUBS][EAST_SEAT] = low;
        dealOptionDoc.suitSize[1][CLUBS][EAST_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_suitSizeClubSouth_editingFinished()
{
    int low, high;

    QString text = ui->suitSizeClubSouth->text();

    bool ok = getValues(text, MAX_SUIT_SIZE, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->suitSizeClubSouth->setFocus();
    }
    else
    {
        dealOptionDoc.suitSize[0][CLUBS][SOUTH_SEAT] = low;
        dealOptionDoc.suitSize[1][CLUBS][SOUTH_SEAT] = high;
    }

}

void CDealOptionsPropSheet::on_dpWest_editingFinished()
{
    int low, high;

    QString text = ui->dpWest->text();

    bool ok = getValues(text, MAX_TOT_DP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->dpWest->setFocus();
    }
    else
    {
        dealOptionDoc.dp[0][WEST_SEAT] = low;
        dealOptionDoc.dp[1][WEST_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_dpNorth_editingFinished()
{
    int low, high;

    QString text = ui->dpNorth->text();

    bool ok = getValues(text, MAX_TOT_DP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->dpNorth->setFocus();
    }
    else
    {
        dealOptionDoc.dp[0][NORTH_SEAT] = low;
        dealOptionDoc.dp[1][NORTH_SEAT] = high;
    }
}

void CDealOptionsPropSheet::on_dpEast_editingFinished()
{
    int low, high;

    QString text = ui->dpEast->text();

    bool ok = getValues(text, MAX_TOT_DP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->dpEast->setFocus();
    }
    else
    {
        dealOptionDoc.dp[0][EAST_SEAT] = low;
        dealOptionDoc.dp[1][EAST_SEAT] = high;
    }

}

void CDealOptionsPropSheet::on_dpSouth_editingFinished()
{
    int low, high;

    QString text = ui->dpSouth->text();

    bool ok = getValues(text, MAX_TOT_DP, &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->dpSouth->setFocus();
    }
    else
    {
        dealOptionDoc.dp[0][SOUTH_SEAT] = low;
        dealOptionDoc.dp[1][SOUTH_SEAT] = high;
    }
}

bool CDealOptionsPropSheet::getValues(QString &text, int max, int *low, int *high)
{
    int inx;
    bool ok;

    text = text.trimmed();
    if (text.contains('+') && text.endsWith('+') && !text.startsWith('+'))
    {
        *low = text.left(text.size() - 1).toInt(&ok);
        *high = max;
    }
    else if (((inx = text.indexOf('-')) != -1) && !text.endsWith('-') && !text.startsWith('-'))
    {
        *low = text.left(inx).toInt(&ok);
        if (ok)
            *high = text.right(text.size() - inx - 1).toInt(&ok);
    }
    else if (!text.contains('+') && !text.contains('-'))
        *low = *high = text.toInt(&ok);
    else
        ok = false;

    if (ok)
        ok = (*low >= 0) && (*high >= 0) && (*low <= max) && (*high <= max) && (*low <= *high);

    return ok;
}

int CDealOptionsPropSheet::checkDealOptions(int noDeals)
{
    int noOk = 0;

    //k is deck number.
    for (int k = 0; k < noDeals; k++)
    {
        int i, j, inx;
        int cardDeck[52];
        int cardValues[52];
        int cards[4][13];

        //Deal card deck.
        //i is card number.
        for (i = 0; i < 52; i++)
            cardValues[i] = i;
        QTime cur;
        qsrand(cur.currentTime().msec());
        for (i = 51; i >= 0; i--)
        {
            inx = rand()%(i + 1);
            cardDeck[i] = cardValues[inx];
            for (j = inx; j < i; j++)
                cardValues[j] = cardValues[j + 1];
        }

        //Shuffle card deck.
/*        for (int l = 0; l < 10; l++)
        for (i = 0; i < 52; i++)
        {
            inx = rand()%52;
            j = cardDeck[i];
            cardDeck[i] = cardDeck[inx];
            cardDeck[inx] = j;
        }
*/
        //Give cards.
        //j is seat and i is card.
        for (j = 0; j < 4; j++)
            for (i = 0; i < 13; i++)
                cards[j][i] = cardDeck[j * 13 + i];

        //Calculate hand characteristica.
        //suit first then seat.
        int hcp[5][4];
        int suitSize[4][4];
        int dp[4];
        //i is suit and j is seat.
        for (i = 0; i < 5; i++)
            for (j = 0; j < 4; j++)
                hcp[i][j] = 0;
        for (j = 0; j < 4; j++)
        {
            dp[j] = 0;
            for (i = 0; i < 4; i++)
                suitSize[i][j] = 0;
        }

        //j is seat.
        for (j = 0; j < 4; j++)
        {
            //i is card.
            for (i = 0; i < 13; i++)
            {
                int card = cards[j][i];
                Suit suit = CARD_SUIT(card);
                int face = CARD_FACE(card);
                if (face > 8)
                {
                    hcp[suit][j] += (face - 8);
                    hcp[NOTRUMP][j] += (face - 8);
                }
                suitSize[suit][j]++;
            }
            //i is suit.
            for (i = 0; i < 4; i++)
                if (suitSize[i][j] <= 2)
                    dp[j] += (3 - suitSize[i][j]);
        }

        //Check deck of cards.
        //j is seat and i is suit.
        bool cont = true;
        for (j = 0; j < 4; j++)
        {
            for (i = 0; i < 4; i++)
                if ((dealOptionDoc.suitSize[0][i][j] > suitSize[i][j]) ||
                        (dealOptionDoc.suitSize[1][i][j] < suitSize[i][j]))
                    cont = false;
            if (!cont)
                break;
            for (i = 0; i < 5; i++)
                if ((dealOptionDoc.hcp[0][i][j] > hcp[i][j]) ||
                        (dealOptionDoc.hcp[1][i][j] < hcp[i][j]))
                    cont = false;
            if (!cont)
                break;
            if ((dealOptionDoc.dp[0][j] > dp[j]) || (dealOptionDoc.dp[1][j] < dp[j]))
                cont = false;
            if (!cont)
                break;
        }

        if (cont)
            noOk++;
    }
    return noOk;
}
