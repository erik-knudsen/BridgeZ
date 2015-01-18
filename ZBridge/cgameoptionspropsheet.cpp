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

    QStringList labels;
    labels << tr("Level 1") << tr("Level 2") << tr("Level 3") << tr("Level 4") << tr("Level 5") << tr("Level 6");
    ui->traceLevel->addItems(labels);

    gameOptionDoc = doc->getGameOptions();

    QString text;

    //Interface.
    ui->autoPlayLastCard->setChecked(gameOptionDoc.autoPlayLastCard);
    ui->autoJumpCursor->setChecked(gameOptionDoc.autoJumpCursor);
    ui->insertBiddingPause->setChecked(gameOptionDoc.insertBiddingPause);
    QTextStream(&text) << gameOptionDoc.biddingPause/10.0;
    ui->biddingPauseLength->setText(text);
    ui->biddingPause->setValue(gameOptionDoc.biddingPause);
    ui->insertPlayPause->setChecked(gameOptionDoc.insertPlayPause);
    text.clear();
    QTextStream(&text) << gameOptionDoc.playPause/10.0;
    ui->playPauseLength->setText(text);
    ui->playPause->setValue(gameOptionDoc.playPause);

    //Mechanics.
    ui->autoBidStart->setChecked(gameOptionDoc.autoBidStart);
    ui->computerCanClaim->setChecked(gameOptionDoc.computerCanClaim);
    ui->allowRebidPassedHands->setChecked(gameOptionDoc.allowRebidPassedHands);
    ui->enableTracing->setChecked(gameOptionDoc.enableTracing);
    ui->showPassedHands->setChecked(gameOptionDoc.showPassedHands);
    ui->enableAnalysisDuringHints->setChecked(gameOptionDoc.enableAnalysisDuringHints);
    ui->traceLevel->setCurrentIndex(gameOptionDoc.traceLevel);

    //Counting.
    if (gameOptionDoc.valuationMode == GOREN_VALUATION_MODE)
        ui->gorenValuationMode->setChecked(true);
    else if (gameOptionDoc.valuationMode == GINSBURG_VALUATION_MODE)
        ui->ginsburgValuationMode->setChecked(true);
    else
        ui->customValuationMode->setChecked(true);
    ui->aceValue->setText(QString().setNum(gameOptionDoc.aceValue));
    ui->kingValue->setText(QString().setNum(gameOptionDoc.kingValue));
    ui->queenValue->setText(QString().setNum(gameOptionDoc.queenValue));
    ui->jackValue->setText(QString().setNum(gameOptionDoc.jackValue));
    ui->tenValue->setText(QString().setNum(gameOptionDoc.tenValue));
    if (!ui->customValuationMode->isChecked())
        enableValues(false);

    //Scoring.
    if (gameOptionDoc.scoringMethod == TEAMS_IMP)
        ui->teamsIMP->setChecked(true);
    else if (gameOptionDoc.scoringMethod == DUPLICATE_MP)
        ui->duplicateMP->setChecked(true);
    else
        ui->RubberBridge->setChecked(true);

    //Files.
    ui->saveIntermediatePositions->setChecked(gameOptionDoc.saveIntermediatePositions);
    ui->exposePBNGameCards->setChecked(gameOptionDoc.exposePBNGameCards);
}

CGameOptionsPropSheet::~CGameOptionsPropSheet()
{
    delete ui;
}

void CGameOptionsPropSheet::on_buttonBox_accepted()
{
    if (gameOptionDoc.valuationMode == CUSTOM_VALUATION_MODE)
    {
        gameOptionDoc.aceValue = ui->aceValue->text().toFloat();
        gameOptionDoc.kingValue = ui->kingValue->text().toFloat();
        gameOptionDoc.queenValue = ui->queenValue->text().toFloat();
        gameOptionDoc.jackValue = ui->jackValue->text().toFloat();
        gameOptionDoc.tenValue = ui->tenValue->text().toFloat();
    }
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

void CGameOptionsPropSheet::on_autoPlayLastCard_clicked(bool checked)
{
    gameOptionDoc.autoPlayLastCard = checked;
}

void CGameOptionsPropSheet::on_autoJumpCursor_clicked(bool checked)
{
    gameOptionDoc.autoJumpCursor = checked;
}

void CGameOptionsPropSheet::on_insertBiddingPause_clicked(bool checked)
{
   gameOptionDoc.insertBiddingPause = checked;
}

void CGameOptionsPropSheet::on_insertPlayPause_clicked(bool checked)
{
    gameOptionDoc.insertPlayPause = checked;
}

void CGameOptionsPropSheet::on_autoBidStart_clicked(bool checked)
{
    gameOptionDoc.autoBidStart = checked;
}

void CGameOptionsPropSheet::on_computerCanClaim_clicked(bool checked)
{
    gameOptionDoc.computerCanClaim = checked;
}

void CGameOptionsPropSheet::on_allowRebidPassedHands_clicked(bool checked)
{
    gameOptionDoc.allowRebidPassedHands = checked;
}

void CGameOptionsPropSheet::on_enableTracing_clicked(bool checked)
{
    gameOptionDoc.enableTracing = checked;
}

void CGameOptionsPropSheet::on_showPassedHands_clicked(bool checked)
{
    gameOptionDoc.showPassedHands = checked;
}

void CGameOptionsPropSheet::on_enableAnalysisDuringHints_clicked(bool checked)
{
    gameOptionDoc.enableAnalysisDuringHints = checked;
}

void CGameOptionsPropSheet::on_traceLevel_currentIndexChanged(int index)
{
    gameOptionDoc.traceLevel = index;
}

void CGameOptionsPropSheet::on_gorenValuationMode_clicked()
{
    enableValues(false);
    gameOptionDoc.valuationMode = GOREN_VALUATION_MODE;
}

void CGameOptionsPropSheet::on_ginsburgValuationMode_clicked()
{
    enableValues(false);
    gameOptionDoc.valuationMode = GINSBURG_VALUATION_MODE;
}

void CGameOptionsPropSheet::on_customValuationMode_clicked()
{
    enableValues(true);
    gameOptionDoc.valuationMode = CUSTOM_VALUATION_MODE;
}

void CGameOptionsPropSheet::enableValues(bool enable)
{
    ui->aceValue->setEnabled(enable);
    ui->kingValue->setEnabled(enable);
    ui->queenValue->setEnabled(enable);
    ui->jackValue->setEnabled(enable);
    ui->tenValue->setEnabled(enable);
}

void CGameOptionsPropSheet::on_teamsIMP_clicked()
{
    gameOptionDoc.scoringMethod = TEAMS_IMP;
}

void CGameOptionsPropSheet::on_duplicateMP_clicked()
{
    gameOptionDoc.scoringMethod = DUPLICATE_MP;
}

void CGameOptionsPropSheet::on_RubberBridge_clicked()
{
    gameOptionDoc.scoringMethod = RUBBER_BRIDGE;
}

void CGameOptionsPropSheet::on_saveIntermediatePositions_clicked(bool checked)
{
    gameOptionDoc.saveIntermediatePositions = checked;
}

void CGameOptionsPropSheet::on_exposePBNGameCards_clicked(bool checked)
{
    gameOptionDoc.exposePBNGameCards = checked;
}
