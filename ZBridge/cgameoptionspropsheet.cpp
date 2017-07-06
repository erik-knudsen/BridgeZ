/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CGameOptionsPropSheet.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Property sheet for determining game options.

  Platforms: Qt.

*/

/**
 * \file
 * Property sheet for determining game options (definition).
 */

#include <QString>
#include <QTextStream>

#include "cgameoptionspropsheet.h"
#include "ui_cgameoptionspropsheet.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"

CGameOptionsPropSheet::CGameOptionsPropSheet(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CGameOptionsPropSheet)
{
    ui->setupUi(this);
    this->app = app;
    this->doc = doc;

    gameOptionDoc = doc->getGameOptions();

    QString text;

    //Interface.
    ui->insertBiddingPause->setChecked(gameOptionDoc.insertBiddingPause);
    QTextStream(&text) << gameOptionDoc.biddingPause/10.0;
    ui->biddingPauseLength->setText(text);
    ui->biddingPause->setValue(gameOptionDoc.biddingPause);
    ui->insertPlayPause->setChecked(gameOptionDoc.insertPlayPause);
    text.clear();
    QTextStream(&text) << gameOptionDoc.playPause/10.0;
    ui->playPauseLength->setText(text);
    ui->playPause->setValue(gameOptionDoc.playPause);

    //Scoring.
    if (gameOptionDoc.scoringMethod == IMP)
        ui->teamsIMP->setChecked(true);
    else if (gameOptionDoc.scoringMethod == MP)
        ui->duplicateMP->setChecked(true);
    else if (gameOptionDoc.scoringMethod == RUBBER)
        ui->RubberBridge->setChecked(true);
    else
        ui->practice->setChecked(true);
}

CGameOptionsPropSheet::~CGameOptionsPropSheet()
{
    delete ui;
}

void CGameOptionsPropSheet::on_buttonBox_accepted()
{
    doc->setGameOptions(gameOptionDoc);
}

void CGameOptionsPropSheet::on_biddingPause_sliderMoved(int position)
{
    QString text;
    QTextStream(&text) << position/10.0;
    ui->biddingPauseLength->setText(text);
    gameOptionDoc.biddingPause = position;
}

void CGameOptionsPropSheet::on_playPause_sliderMoved(int position)
{
    QString text;
    QTextStream(&text) << position/10.0;
    ui->playPauseLength->setText(text);
    gameOptionDoc.playPause = position;
}

void CGameOptionsPropSheet::on_insertBiddingPause_clicked(bool checked)
{
   gameOptionDoc.insertBiddingPause = checked;
}

void CGameOptionsPropSheet::on_insertPlayPause_clicked(bool checked)
{
    gameOptionDoc.insertPlayPause = checked;
}

void CGameOptionsPropSheet::on_teamsIMP_clicked()
{
    gameOptionDoc.scoringMethod = IMP;
}

void CGameOptionsPropSheet::on_duplicateMP_clicked()
{
    gameOptionDoc.scoringMethod = MP;
}

void CGameOptionsPropSheet::on_RubberBridge_clicked()
{
    gameOptionDoc.scoringMethod = RUBBER;
}

void CGameOptionsPropSheet::on_practice_clicked()
{
    gameOptionDoc.scoringMethod = PRACTICE;
}
