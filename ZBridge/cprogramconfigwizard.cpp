/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigWizard.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard (definition).
 */

#include <QString>
#include <QTextStream>

#include "cprogramconfigwizard.h"
#include "ui_cprogramconfigwizard.h"
#include "ui_cprogconfighelplevelpage.h"
#include "ui_cprogconfigviewsettingspage.h"
#include "ui_cprogconfigsuitsdisplaypage.h"
#include "ui_cprogconfiggamemechanicspage.h"
#include "ui_cprogconfigpausespage.h"
#include "ui_cprogconfigbiddingpage.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"


CProgramConfigWizard::CProgramConfigWizard(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QWizard(parent),
    ui(new Ui::CProgramConfigWizard)
{
    ui->setupUi(this);
    this->app = app;
    this->doc = doc;

    wizardOptionDoc = doc->getWizardOptions();

    //Help Level page.
    if (wizardOptionDoc.helpLevel == BEGIN_HELP_LEVEL)
        ui->progConfigHelpLevelPage->ui->beginHelpLevel->setChecked(true);
    else if (wizardOptionDoc.helpLevel == BASIC_HELP_LEVEL)
        ui->progConfigHelpLevelPage->ui->basicHelpLevel->setChecked(true);
    else
        ui->progConfigHelpLevelPage->ui->minimumHelpLevel->setChecked(true);

    ui->progConfigHelpLevelPage->ui->enableTracing->setChecked(wizardOptionDoc.enableTracing);

    //View Settings page.
    ui->progConfigViewSettingsPage->ui->autoShowBidHistory->setChecked(wizardOptionDoc.autoShowBidHistory);
    ui->progConfigViewSettingsPage->ui->autoShowPlayHistory->setChecked(wizardOptionDoc.autoShowPlayHistory);
    ui->progConfigViewSettingsPage->ui->useSuitSymbols->setChecked(wizardOptionDoc.useSuitSymbols);

    //Suits Display page.
    if (wizardOptionDoc.suitSequence == SHCD_SUIT_SEQUENCE)
        ui->progConfigSuitsDisplayPage->ui->shcdSuitSequence->setChecked(true);
    else if (wizardOptionDoc.suitSequence == SHDC_SUIT_SEQUENCE)
        ui->progConfigSuitsDisplayPage->ui->shdcSuitSequence->setChecked(true);
    else if (wizardOptionDoc.suitSequence == DCHS_SUIT_SEQUENCE)
        ui->progConfigSuitsDisplayPage->ui->dchsSuitSequence->setChecked(true);
    else if (wizardOptionDoc.suitSequence == CDHS_SUIT_SEQUENCE)
        ui->progConfigSuitsDisplayPage->ui->cdhsSuitSequence->setChecked(true);
    else
        ui->progConfigSuitsDisplayPage->ui->hsdcSuitSequence->setChecked(true);

    ui->progConfigSuitsDisplayPage->ui->showDummyTrumpsOnLeft->setChecked(wizardOptionDoc.showDummyTrumpsOnLeft);

    //Game Options page.
    if (wizardOptionDoc.scoringMethod == TEAMS_IMP)
        ui->progConfigGameMechanicsPage->ui->teamsIMP->setChecked(true);
    else if (wizardOptionDoc.scoringMethod == DUPLICATE_MP)
        ui->progConfigGameMechanicsPage->ui->duplicateMP->setChecked(true);
    else if (wizardOptionDoc.scoringMethod == RUBBER_BRIDGE)
        ui->progConfigGameMechanicsPage->ui->rubberBridge->setChecked(true);
    else
        ui->progConfigGameMechanicsPage->ui->practice->setChecked(true);

    //Pauses page.
    QString text;
    ui->progConfigPausesPage->ui->insertBiddingPause->setChecked(wizardOptionDoc.insertBiddingPause);
    QTextStream(&text) << wizardOptionDoc.biddingPauseLength/10.0;
    ui->progConfigPausesPage->ui->biddingPauseLength->setText(text);
    ui->progConfigPausesPage->ui->biddingPause->setValue(wizardOptionDoc.biddingPauseLength);

    ui->progConfigPausesPage->ui->insertPlayPause->setChecked(wizardOptionDoc.insertPlayPause);
    text.clear();
    QTextStream(&text) << wizardOptionDoc.playPauseLenght/10.0;
    ui->progConfigPausesPage->ui->playPauseLength->setText(text);
    ui->progConfigPausesPage->ui->playPause->setValue(wizardOptionDoc.playPauseLenght);

    //Bidding Style page.
    if (wizardOptionDoc.biddingStyle == NATURAL_BIDDING_STYLE)
        ui->progConfigBiddingPage->ui->naturalBiddingStyle->setChecked(true);
    else if (wizardOptionDoc.biddingStyle == GOREN_BIDDING_STYLE)
        ui->progConfigBiddingPage->ui->gorenBiddingStyle->setChecked(true);
    else if (wizardOptionDoc.biddingStyle == STANDARD_AMERICAN_BASIC_BIDDING_STYLE)
        ui->progConfigBiddingPage->ui->standardAmericanBasicBiddingStyle->setChecked(true);
    else if (wizardOptionDoc.biddingStyle == STANDARD_AMERICAN_YELLOW_CARD_BIDDING_STYLE)
        ui->progConfigBiddingPage->ui->standardAmericanYellowCardBiddingStyle->setChecked(true);
    else
        ui->progConfigBiddingPage->ui->acolBiddingStyle->setChecked(true);
}

CProgramConfigWizard::~CProgramConfigWizard()
{
    delete ui;
}

void CProgramConfigWizard::accept()
{
    //Help Level page.
    if(ui->progConfigHelpLevelPage->ui->beginHelpLevel->isChecked())
        wizardOptionDoc.helpLevel = BEGIN_HELP_LEVEL;
    else if (ui->progConfigHelpLevelPage->ui->basicHelpLevel->isChecked())
        wizardOptionDoc.helpLevel = BASIC_HELP_LEVEL;
    else
        wizardOptionDoc.helpLevel = MINIMUM_HELP_LEVEL;

    wizardOptionDoc.enableTracing = ui->progConfigHelpLevelPage->ui->enableTracing->isChecked();

    //View Settings page.
    wizardOptionDoc.autoShowBidHistory = ui->progConfigViewSettingsPage->ui->autoShowBidHistory->isChecked();
    wizardOptionDoc.autoShowPlayHistory = ui->progConfigViewSettingsPage->ui->autoShowPlayHistory->isChecked();
    wizardOptionDoc.useSuitSymbols = ui->progConfigViewSettingsPage->ui->useSuitSymbols->isChecked();

    //Suits Display page.
    if (ui->progConfigSuitsDisplayPage->ui->shcdSuitSequence->isChecked())
        wizardOptionDoc.suitSequence = SHCD_SUIT_SEQUENCE;
    else if (ui->progConfigSuitsDisplayPage->ui->shdcSuitSequence->isChecked())
        wizardOptionDoc.suitSequence = SHDC_SUIT_SEQUENCE;
    else if (ui->progConfigSuitsDisplayPage->ui->dchsSuitSequence->isChecked())
        wizardOptionDoc.suitSequence = DCHS_SUIT_SEQUENCE;
    else if (ui->progConfigSuitsDisplayPage->ui->cdhsSuitSequence->isChecked())
        wizardOptionDoc.suitSequence = CDHS_SUIT_SEQUENCE;
    else if (ui->progConfigSuitsDisplayPage->ui->hsdcSuitSequence->isChecked())
        wizardOptionDoc.suitSequence = HSDC_SUIT_SEQUENCE;

    wizardOptionDoc.showDummyTrumpsOnLeft = ui->progConfigSuitsDisplayPage->ui->showDummyTrumpsOnLeft->isChecked();

    //Game Options page.
    if (ui->progConfigGameMechanicsPage->ui->teamsIMP->isChecked())
        wizardOptionDoc.scoringMethod = TEAMS_IMP;
    else if (ui->progConfigGameMechanicsPage->ui->duplicateMP->isChecked())
        wizardOptionDoc.scoringMethod = DUPLICATE_MP;
    else if (ui->progConfigGameMechanicsPage->ui->rubberBridge->isChecked())
        wizardOptionDoc.scoringMethod = RUBBER_BRIDGE;
    else
        wizardOptionDoc.scoringMethod = PRACTICE;

    //Pauses page.
    wizardOptionDoc.insertBiddingPause = ui->progConfigPausesPage->ui->insertBiddingPause->isChecked();
    wizardOptionDoc.biddingPauseLength = ui->progConfigPausesPage->ui->biddingPause->value();

    wizardOptionDoc.insertPlayPause = ui->progConfigPausesPage->ui->insertPlayPause->isChecked();
    wizardOptionDoc.playPauseLenght = ui->progConfigPausesPage->ui->playPause->value();

    //Bidding Style page.
    if (ui->progConfigBiddingPage->ui->naturalBiddingStyle->isChecked())
        wizardOptionDoc.biddingStyle = NATURAL_BIDDING_STYLE;
    else if (ui->progConfigBiddingPage->ui->gorenBiddingStyle->isChecked())
        wizardOptionDoc.biddingStyle = GOREN_BIDDING_STYLE;
    else if (ui->progConfigBiddingPage->ui->standardAmericanBasicBiddingStyle->isChecked())
        wizardOptionDoc.biddingStyle = STANDARD_AMERICAN_BASIC_BIDDING_STYLE;
    else if (ui->progConfigBiddingPage->ui->standardAmericanYellowCardBiddingStyle->isChecked())
        wizardOptionDoc.biddingStyle = STANDARD_AMERICAN_YELLOW_CARD_BIDDING_STYLE;
    else if (ui->progConfigBiddingPage->ui->acolBiddingStyle->isChecked())
        wizardOptionDoc.biddingStyle = ACOL_BIDDING_STYLE;

    doc->setWizardOptions(wizardOptionDoc);

    QDialog::accept();
}
