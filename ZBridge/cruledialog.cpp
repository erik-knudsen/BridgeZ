/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CRuleDialog.h
  Developers: eak

  Revision History:
  4-jul-2016 eak: Original

  Abstract: Rule dialog.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the rule dialog (part of the bid database editor).
 */

#include <QStringList>
#include <QMessageBox>

#include "cruledialog.h"
#include "ui_cruledialog.h"

/**
 * @brief Constructor for rule dialog.
 * Initializes the rule dialog.
 *
 * @param rule Points to inital values of the rule to edit.
 * @param ruleIdDesc Text description of the rule id.
 * @param bid The bid (only to be displayed).
 * @param parent The dialogs parent.
 */
CRuleDialog::CRuleDialog(CRule *rule, CBidDesc *bidDesc, QString &bid, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRuleDialog)
{
    ui->setupUi(this);

    this->rule = rule;
    this->bidDesc = bidDesc;

    id = rule->getId();
    priority = rule->getPriority();
    status = rule->getStatus();
    scoringMethod = rule->getScoringMethod();
    vulnerability = rule->getVulnerability();
    alertId = rule->getAlertId();
    rule->getFeatures(&lowFeatures, &highFeatures);

    //Set initial values.
    //Id.
    ui->ruleId->setText(QString::number(id));    
    ui->ruleIdDesc->setPlainText(bidDesc->getRuleIdDesc(id));

    //Next bid.
    ui->ruleBid->setText(bid);

    //Priority.
    ui->rulePriority->setValue(priority);

    //Status.
    QStringList labels;
    labels << tr("Non Forcing") << tr("Forcing") << tr("Game Forcing") << tr("Must Pass");
    ui->ruleForcing->addItems(labels);
    ui->ruleForcing->setCurrentIndex(status);

    //Scoring method.
    labels.clear();
    labels << tr("IMP") << tr("MP") << tr("No score");
    ui->ruleScoringMethod->addItems(labels);
    int inx = (scoringMethod <= MP) ? scoringMethod : 2;
    ui->ruleScoringMethod->setCurrentIndex(inx);

    //Vulnerability (Self, Opponent).
    labels.clear();
    labels << tr("Ignore, Ignore") << tr("Not Vul, Ignore") << tr("Vul, Ignore") << tr("Not Vul, Not Vul");
    labels << tr("Not Vul, Vul") << tr("Vul, Not Vul") << tr("Vul, Vul");
    ui->ruleVulnerability->addItems(labels);
    ui->ruleVulnerability->setCurrentIndex(vulnerability);

    //Alertno.
    ui->ruleAlertId->setText(QString::number(alertId));
    ui->alertIdDesc->setPlainText(bidDesc->getAlertIdDesc(alertId));

    //Ranges.
    setText(ui->hcpS, lowFeatures.getHcp(SPADES), highFeatures.getHcp(SPADES), 0, highFeatures.getMaxHcp(SPADES));
    setText(ui->hcpH, lowFeatures.getHcp(HEARTS), highFeatures.getHcp(HEARTS), 0, highFeatures.getMaxHcp(HEARTS));
    setText(ui->hcpD, lowFeatures.getHcp(DIAMONDS), highFeatures.getHcp(DIAMONDS), 0, highFeatures.getMaxHcp(DIAMONDS));
    setText(ui->hcpC, lowFeatures.getHcp(CLUBS), highFeatures.getHcp(CLUBS), 0, highFeatures.getMaxHcp(CLUBS));
    setText(ui->hcpA, lowFeatures.getHcp(ANY), highFeatures.getHcp(ANY), 0, highFeatures.getMaxHcp(ANY));

    setText(ui->controlsS, lowFeatures.getControls(SPADES), highFeatures.getControls(SPADES), 0, highFeatures.getMaxControls(SPADES));
    setText(ui->controlsH, lowFeatures.getControls(HEARTS), highFeatures.getControls(HEARTS), 0, highFeatures.getMaxControls(HEARTS));
    setText(ui->controlsD, lowFeatures.getControls(DIAMONDS), highFeatures.getControls(DIAMONDS), 0, highFeatures.getMaxControls(DIAMONDS));
    setText(ui->controlsC, lowFeatures.getControls(CLUBS), highFeatures.getControls(CLUBS), 0, highFeatures.getMaxControls(CLUBS));
    setText(ui->controlsA, lowFeatures.getControls(ANY), highFeatures.getControls(ANY), 0, highFeatures.getMaxControls(ANY));

    setText(ui->hcS, lowFeatures.getHc(SPADES), highFeatures.getHc(SPADES), 0, highFeatures.getMaxHc(SPADES));
    setText(ui->hcH, lowFeatures.getHc(HEARTS), highFeatures.getHc(HEARTS), 0, highFeatures.getMaxHc(HEARTS));
    setText(ui->hcD, lowFeatures.getHc(DIAMONDS), highFeatures.getHc(DIAMONDS), 0, highFeatures.getMaxHc(DIAMONDS));
    setText(ui->hcC, lowFeatures.getHc(CLUBS), highFeatures.getHc(CLUBS), 0, highFeatures.getMaxHc(CLUBS));
    setText(ui->hcA, lowFeatures.getHc(ANY), highFeatures.getHc(ANY), 0, highFeatures.getMaxHc(ANY));

    setText(ui->honorS, lowFeatures.getHonor(SPADES), highFeatures.getHonor(SPADES), 0, highFeatures.getMaxHonor(SPADES));
    setText(ui->honorH, lowFeatures.getHonor(HEARTS), highFeatures.getHonor(HEARTS), 0, highFeatures.getMaxHonor(HEARTS));
    setText(ui->honorD, lowFeatures.getHonor(DIAMONDS), highFeatures.getHonor(DIAMONDS), 0, highFeatures.getMaxHonor(DIAMONDS));
    setText(ui->honorC, lowFeatures.getHonor(CLUBS), highFeatures.getHonor(CLUBS), 0, highFeatures.getMaxHonor(CLUBS));
    setText(ui->honorA, lowFeatures.getHonor(ANY), highFeatures.getHonor(ANY), 0, highFeatures.getMaxHonor(ANY));

    setText(ui->aceS, lowFeatures.getCountCard(SPADES, ACE), highFeatures.getCountCard(SPADES, ACE), 0, highFeatures.getMaxCountCard(SPADES));
    setText(ui->aceH, lowFeatures.getCountCard(HEARTS, ACE), highFeatures.getCountCard(HEARTS, ACE), 0, highFeatures.getMaxCountCard(HEARTS));
    setText(ui->aceD, lowFeatures.getCountCard(DIAMONDS, ACE), highFeatures.getCountCard(DIAMONDS, ACE), 0, highFeatures.getMaxCountCard(DIAMONDS));
    setText(ui->aceC, lowFeatures.getCountCard(CLUBS, ACE), highFeatures.getCountCard(CLUBS, ACE), 0, highFeatures.getMaxCountCard(CLUBS));
    setText(ui->aceA, lowFeatures.getCountCard(ANY, ACE), highFeatures.getCountCard(ANY, ACE), 0, highFeatures.getMaxCountCard(ANY));

    setText(ui->kingS, lowFeatures.getCountCard(SPADES, KING), highFeatures.getCountCard(SPADES, KING), 0, highFeatures.getMaxCountCard(SPADES));
    setText(ui->kingH, lowFeatures.getCountCard(HEARTS, KING), highFeatures.getCountCard(HEARTS, KING), 0, highFeatures.getMaxCountCard(HEARTS));
    setText(ui->kingD, lowFeatures.getCountCard(DIAMONDS, KING), highFeatures.getCountCard(DIAMONDS, KING), 0, highFeatures.getMaxCountCard(DIAMONDS));
    setText(ui->kingC, lowFeatures.getCountCard(CLUBS, KING), highFeatures.getCountCard(CLUBS, KING), 0, highFeatures.getMaxCountCard(CLUBS));
    setText(ui->kingA, lowFeatures.getCountCard(ANY, KING), highFeatures.getCountCard(ANY, KING), 0, highFeatures.getMaxCountCard(ANY));

    setText(ui->queenS, lowFeatures.getCountCard(SPADES, QUEEN), highFeatures.getCountCard(SPADES, QUEEN), 0, highFeatures.getMaxCountCard(SPADES));
    setText(ui->queenH, lowFeatures.getCountCard(HEARTS, QUEEN), highFeatures.getCountCard(HEARTS, QUEEN), 0, highFeatures.getMaxCountCard(HEARTS));
    setText(ui->queenD, lowFeatures.getCountCard(DIAMONDS, QUEEN), highFeatures.getCountCard(DIAMONDS, QUEEN), 0, highFeatures.getMaxCountCard(DIAMONDS));
    setText(ui->queenC, lowFeatures.getCountCard(CLUBS, QUEEN), highFeatures.getCountCard(CLUBS, QUEEN), 0, highFeatures.getMaxCountCard(CLUBS));
    setText(ui->queenA, lowFeatures.getCountCard(ANY, QUEEN), highFeatures.getCountCard(ANY, QUEEN), 0, highFeatures.getMaxCountCard(ANY));

    setText(ui->jackS, lowFeatures.getCountCard(SPADES, JACK), highFeatures.getCountCard(SPADES, JACK), 0, highFeatures.getMaxCountCard(SPADES));
    setText(ui->jackH, lowFeatures.getCountCard(HEARTS, JACK), highFeatures.getCountCard(HEARTS, JACK), 0, highFeatures.getMaxCountCard(HEARTS));
    setText(ui->jackD, lowFeatures.getCountCard(DIAMONDS, JACK), highFeatures.getCountCard(DIAMONDS, JACK), 0, highFeatures.getMaxCountCard(DIAMONDS));
    setText(ui->jackC, lowFeatures.getCountCard(CLUBS, JACK), highFeatures.getCountCard(CLUBS, JACK), 0, highFeatures.getMaxCountCard(CLUBS));
    setText(ui->jackA, lowFeatures.getCountCard(ANY, JACK), highFeatures.getCountCard(ANY, JACK), 0, highFeatures.getMaxCountCard(ANY));

    setText(ui->tenS, lowFeatures.getCountCard(SPADES, TEN), highFeatures.getCountCard(SPADES, TEN), 0, highFeatures.getMaxCountCard(SPADES));
    setText(ui->tenH, lowFeatures.getCountCard(HEARTS, TEN), highFeatures.getCountCard(HEARTS, TEN), 0, highFeatures.getMaxCountCard(HEARTS));
    setText(ui->tenD, lowFeatures.getCountCard(DIAMONDS, TEN), highFeatures.getCountCard(DIAMONDS, TEN), 0, highFeatures.getMaxCountCard(DIAMONDS));
    setText(ui->tenC, lowFeatures.getCountCard(CLUBS, TEN), highFeatures.getCountCard(CLUBS, TEN), 0, highFeatures.getMaxCountCard(CLUBS));
    setText(ui->tenA, lowFeatures.getCountCard(ANY, TEN), highFeatures.getCountCard(ANY, TEN), 0, highFeatures.getMaxCountCard(ANY));

    setText(ui->rkcbS, lowFeatures.getRkcb(SPADES), highFeatures.getRkcb(SPADES), 0, highFeatures.getMaxRkcb());
    setText(ui->rkcbH, lowFeatures.getRkcb(HEARTS), highFeatures.getRkcb(HEARTS), 0, highFeatures.getMaxRkcb());
    setText(ui->rkcbD, lowFeatures.getRkcb(DIAMONDS), highFeatures.getRkcb(DIAMONDS), 0, highFeatures.getMaxRkcb());
    setText(ui->rkcbC, lowFeatures.getRkcb(CLUBS), highFeatures.getRkcb(CLUBS), 0, highFeatures.getMaxRkcb());

    setText(ui->interS, lowFeatures.getInter(SPADES), highFeatures.getInter(SPADES), 0, highFeatures.getMaxInter(SPADES));
    setText(ui->interH, lowFeatures.getInter(HEARTS), highFeatures.getInter(HEARTS), 0, highFeatures.getMaxInter(HEARTS));
    setText(ui->interD, lowFeatures.getInter(DIAMONDS), highFeatures.getInter(DIAMONDS), 0, highFeatures.getMaxInter(DIAMONDS));
    setText(ui->interC, lowFeatures.getInter(CLUBS), highFeatures.getInter(CLUBS), 0, highFeatures.getMaxInter(CLUBS));
    setText(ui->interA, lowFeatures.getInter(ANY), highFeatures.getInter(ANY), 0, highFeatures.getMaxInter(ANY));

    setText(ui->loserS, lowFeatures.getLoosers(SPADES), highFeatures.getLoosers(SPADES), 0, highFeatures.getMaxLoosers(SPADES));
    setText(ui->loserH, lowFeatures.getLoosers(HEARTS), highFeatures.getLoosers(HEARTS), 0, highFeatures.getMaxLoosers(HEARTS));
    setText(ui->loserD, lowFeatures.getLoosers(DIAMONDS), highFeatures.getLoosers(DIAMONDS), 0, highFeatures.getMaxLoosers(DIAMONDS));
    setText(ui->loserC, lowFeatures.getLoosers(CLUBS), highFeatures.getLoosers(CLUBS), 0, highFeatures.getMaxLoosers(CLUBS));
    setText(ui->loserA, lowFeatures.getLoosers(ANY), highFeatures.getLoosers(ANY), 0, highFeatures.getMaxLoosers(ANY));

    setText(ui->quickS, lowFeatures.getQuickTricks(SPADES), highFeatures.getQuickTricks(SPADES), 0, highFeatures.getMaxQuickTricks(SPADES));
    setText(ui->quickH, lowFeatures.getQuickTricks(HEARTS), highFeatures.getQuickTricks(HEARTS), 0, highFeatures.getMaxQuickTricks(HEARTS));
    setText(ui->quickD, lowFeatures.getQuickTricks(DIAMONDS), highFeatures.getQuickTricks(DIAMONDS), 0, highFeatures.getMaxQuickTricks(DIAMONDS));
    setText(ui->quickC, lowFeatures.getQuickTricks(CLUBS), highFeatures.getQuickTricks(CLUBS), 0, highFeatures.getMaxQuickTricks(CLUBS));
    setText(ui->quickA, lowFeatures.getQuickTricks(ANY), highFeatures.getQuickTricks(ANY), 0, highFeatures.getMaxQuickTricks(ANY));

    setText(ui->playS, lowFeatures.getPlayingTricks(SPADES), highFeatures.getPlayingTricks(SPADES), 0, highFeatures.getMaxPlayingTricks());
    setText(ui->playH, lowFeatures.getPlayingTricks(HEARTS), highFeatures.getPlayingTricks(HEARTS), 0, highFeatures.getMaxPlayingTricks());
    setText(ui->playD, lowFeatures.getPlayingTricks(DIAMONDS), highFeatures.getPlayingTricks(DIAMONDS), 0, highFeatures.getMaxPlayingTricks());
    setText(ui->playC, lowFeatures.getPlayingTricks(CLUBS), highFeatures.getPlayingTricks(CLUBS), 0, highFeatures.getMaxPlayingTricks());
    setText(ui->playA, lowFeatures.getPlayingTricks(ANY), highFeatures.getPlayingTricks(ANY), 0, highFeatures.getMaxPlayingTricks());

    setText(ui->stopNTS, lowFeatures.getStopNT(SPADES), highFeatures.getStopNT(SPADES), 0, highFeatures.getMaxStopNT());
    setText(ui->stopNTH, lowFeatures.getStopNT(HEARTS), highFeatures.getStopNT(HEARTS), 0, highFeatures.getMaxStopNT());
    setText(ui->stopNTD, lowFeatures.getStopNT(DIAMONDS), highFeatures.getStopNT(DIAMONDS), 0, highFeatures.getMaxStopNT());
    setText(ui->stopNTC, lowFeatures.getStopNT(CLUBS), highFeatures.getStopNT(CLUBS), 0, highFeatures.getMaxStopNT());

    setText(ui->qltyS, lowFeatures.getQlty(SPADES), highFeatures.getQlty(SPADES), 0, highFeatures.getMaxQlty());
    setText(ui->qltyH, lowFeatures.getQlty(HEARTS), highFeatures.getQlty(HEARTS), 0, highFeatures.getMaxQlty());
    setText(ui->qltyD, lowFeatures.getQlty(DIAMONDS), highFeatures.getQlty(DIAMONDS), 0, highFeatures.getMaxQlty());
    setText(ui->qltyC, lowFeatures.getQlty(CLUBS), highFeatures.getQlty(CLUBS), 0, highFeatures.getMaxQlty());

    setText(ui->pointsS, lowFeatures.getPoints(SPADES), highFeatures.getPoints(SPADES), 0, highFeatures.getMaxPoints());
    setText(ui->pointsH, lowFeatures.getPoints(HEARTS), highFeatures.getPoints(HEARTS), 0, highFeatures.getMaxPoints());
    setText(ui->pointsD, lowFeatures.getPoints(DIAMONDS), highFeatures.getPoints(DIAMONDS), 0, highFeatures.getMaxPoints());
    setText(ui->pointsC, lowFeatures.getPoints(CLUBS), highFeatures.getPoints(CLUBS), 0, highFeatures.getMaxPoints());
    setText(ui->pointsA, lowFeatures.getPoints(ANY), highFeatures.getPoints(ANY), 0, highFeatures.getMaxPoints());

    setText(ui->dpS, lowFeatures.getDp(SPADES), highFeatures.getDp(SPADES), 0, highFeatures.getMaxDp());
    setText(ui->dpH, lowFeatures.getDp(HEARTS), highFeatures.getDp(HEARTS), 0, highFeatures.getMaxDp());
    setText(ui->dpD, lowFeatures.getDp(DIAMONDS), highFeatures.getDp(DIAMONDS), 0, highFeatures.getMaxDp());
    setText(ui->dpC, lowFeatures.getDp(CLUBS), highFeatures.getDp(CLUBS), 0, highFeatures.getMaxDp());
    setText(ui->dpA, lowFeatures.getDp(ANY), highFeatures.getDp(ANY), 0, highFeatures.getMaxDp());

    setText(ui->LengthS, lowFeatures.getSuitLen(SPADES), highFeatures.getSuitLen(SPADES), 0, highFeatures.getMaxSuitLen());
    setText(ui->LengthH, lowFeatures.getSuitLen(HEARTS), highFeatures.getSuitLen(HEARTS), 0, highFeatures.getMaxSuitLen());
    setText(ui->LengthD, lowFeatures.getSuitLen(DIAMONDS), highFeatures.getSuitLen(DIAMONDS), 0, highFeatures.getMaxSuitLen());
    setText(ui->LengthC, lowFeatures.getSuitLen(CLUBS), highFeatures.getSuitLen(CLUBS), 0, highFeatures.getMaxSuitLen());

    setText(ui->longestS, lowFeatures.getLongest(SPADES), highFeatures.getLongest(SPADES), 0, highFeatures.getMaxLongest());
    setText(ui->longestH, lowFeatures.getLongest(HEARTS), highFeatures.getLongest(HEARTS), 0, highFeatures.getMaxLongest());
    setText(ui->longestD, lowFeatures.getLongest(DIAMONDS), highFeatures.getLongest(DIAMONDS), 0, highFeatures.getMaxLongest());
    setText(ui->longestC, lowFeatures.getLongest(CLUBS), highFeatures.getLongest(CLUBS), 0, highFeatures.getMaxLongest());

    setText(ui->longS, lowFeatures.getLong(SPADES), highFeatures.getLong(SPADES), 0, highFeatures.getMaxLong());
    setText(ui->longH, lowFeatures.getLong(HEARTS), highFeatures.getLong(HEARTS), 0, highFeatures.getMaxLong());
    setText(ui->longD, lowFeatures.getLong(DIAMONDS), highFeatures.getLong(DIAMONDS), 0, highFeatures.getMaxLong());
    setText(ui->longC, lowFeatures.getLong(CLUBS), highFeatures.getLong(CLUBS), 0, highFeatures.getMaxLong());

    setText(ui->difSH, lowFeatures.getDif2(SPADES, HEARTS), highFeatures.getDif2(SPADES, HEARTS), -lowFeatures.getMaxDif2(), highFeatures.getMaxDif2());
    setText(ui->difSD, lowFeatures.getDif2(SPADES, DIAMONDS), highFeatures.getDif2(SPADES, DIAMONDS), -lowFeatures.getMaxDif2(), highFeatures.getMaxDif2());
    setText(ui->difHD, lowFeatures.getDif2(HEARTS, DIAMONDS), highFeatures.getDif2(HEARTS, DIAMONDS), -lowFeatures.getMaxDif2(), highFeatures.getMaxDif2());
    setText(ui->difSC, lowFeatures.getDif2(SPADES, CLUBS), highFeatures.getDif2(SPADES, CLUBS), -lowFeatures.getMaxDif2(), highFeatures.getMaxDif2());
    setText(ui->difHC, lowFeatures.getDif2(HEARTS, CLUBS), highFeatures.getDif2(HEARTS, CLUBS), -lowFeatures.getMaxDif2(), highFeatures.getMaxDif2());
    setText(ui->difDC, lowFeatures.getDif2(DIAMONDS, CLUBS), highFeatures.getDif2(DIAMONDS, CLUBS), -lowFeatures.getMaxDif2(), highFeatures.getMaxDif2());

    setText(ui->sumSH, lowFeatures.getSuitLen2(SPADES, HEARTS), highFeatures.getSuitLen2(SPADES, HEARTS), 0, highFeatures.getMaxSuitLen2());
    setText(ui->sumSD, lowFeatures.getSuitLen2(SPADES, DIAMONDS), highFeatures.getSuitLen2(SPADES, DIAMONDS), 0, highFeatures.getMaxSuitLen2());
    setText(ui->sumHD, lowFeatures.getSuitLen2(HEARTS, DIAMONDS), highFeatures.getSuitLen2(HEARTS, DIAMONDS), 0, highFeatures.getMaxSuitLen2());
    setText(ui->sumSC, lowFeatures.getSuitLen2(SPADES, CLUBS), highFeatures.getSuitLen2(SPADES, CLUBS), 0, highFeatures.getMaxSuitLen2());
    setText(ui->sumHC, lowFeatures.getSuitLen2(HEARTS, CLUBS), highFeatures.getSuitLen2(HEARTS, CLUBS), 0, highFeatures.getMaxSuitLen2());
    setText(ui->sumDC, lowFeatures.getSuitLen2(DIAMONDS, CLUBS), highFeatures.getSuitLen2(DIAMONDS, CLUBS), 0, highFeatures.getMaxSuitLen2());

    setText(ui->balanced, lowFeatures.getBal(), highFeatures.getBal(), 0, highFeatures.getMaxBal());
    setText(ui->longerThan3, lowFeatures.getSuits(), highFeatures.getSuits(), 1, highFeatures.getMaxSuits());
    setText(ui->longerMajor, lowFeatures.getLMaj(), highFeatures.getLMaj(), 0, highFeatures.getMaxLMaj());
    setText(ui->longerMinor, lowFeatures.getLMin(), highFeatures.getLMin(), 0, highFeatures.getMaxLMin());
}

CRuleDialog::~CRuleDialog()
{
    delete ui;
}

void CRuleDialog::on_ruleId_editingFinished()
{
    int low, high;

    QString text = ui->ruleId->text();

    bool ok = getValues(text, 0, 0xffff, &low, &high);
    if (!ok || (low != high))
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->ruleId->setFocus();
    }
    else
    {
        id = low;
        ui->ruleIdDesc->setPlainText(bidDesc->getRuleIdDesc(id));
    }
}

void CRuleDialog::on_rulePriority_valueChanged(int arg1)
{
    priority = arg1;
}


void CRuleDialog::on_ruleForcing_activated(int index)
{
    status = (Forcing)index;
}


void CRuleDialog::on_ruleScoringMethod_activated(int index)
{
    scoringMethod = (ScoringMethod)((index <= MP) ? index : 2);
}


void CRuleDialog::on_ruleVulnerability_activated(int index)
{
    vulnerability = (Vulnerability)index;
}


void CRuleDialog::on_ruleAlertId_editingFinished()
{
    int low, high;

    QString text = ui->ruleAlertId ->text();

    bool ok = getValues(text, 0, 0xff, &low, &high);
    if (!ok || (low != high))
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->ruleAlertId->setFocus();
    }
    else
    {
        alertId = low;
        ui->alertIdDesc->setPlainText(bidDesc->getAlertIdDesc(alertId));
    }
}

void CRuleDialog::on_hcpS_editingFinished()
{
    int low, high;

    QString text = ui->hcpS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHcp(SPADES), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpS->setFocus();
    }
    else
    {
        lowFeatures.setHcp(SPADES, low);
        highFeatures.setHcp(SPADES, high);
    }
}

void CRuleDialog::on_hcpH_editingFinished()
{
    int low, high;

    QString text = ui->hcpH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHcp(HEARTS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpH->setFocus();
    }
    else
    {
        lowFeatures.setHcp(HEARTS, low);
        highFeatures.setHcp(HEARTS, high);
    }
}

void CRuleDialog::on_hcpD_editingFinished()
{
    int low, high;

    QString text = ui->hcpD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHcp(DIAMONDS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpD->setFocus();
    }
    else
    {
        lowFeatures.setHcp(DIAMONDS, low);
        highFeatures.setHcp(DIAMONDS, high);
    }
}

void CRuleDialog::on_hcpC_editingFinished()
{
    int low, high;

    QString text = ui->hcpC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHcp(CLUBS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpC->setFocus();
    }
    else
    {
        lowFeatures.setHcp(CLUBS, low);
        highFeatures.setHcp(CLUBS, high);
    }
}

void CRuleDialog::on_hcpA_editingFinished()
{
    int low, high;

    QString text = ui->hcpA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHcp(ANY), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcpA->setFocus();
    }
    else
    {
        lowFeatures.setHcp(ANY, low);
        highFeatures.setHcp(ANY, high);
    }
}

void CRuleDialog::on_controlsS_editingFinished()
{
    int low, high;

    QString text = ui->controlsS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxControls(SPADES), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->controlsS->setFocus();
    }
    else
    {
        lowFeatures.setControls(SPADES, low);
        highFeatures.setControls(SPADES, high);
    }
}

void CRuleDialog::on_controlsH_editingFinished()
{
    int low, high;

    QString text = ui->controlsH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxControls(HEARTS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->controlsH->setFocus();
    }
    else
    {
        lowFeatures.setControls(HEARTS, low);
        highFeatures.setControls(HEARTS, high);
    }
}

void CRuleDialog::on_controlsD_editingFinished()
{
    int low, high;

    QString text = ui->controlsD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxControls(DIAMONDS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->controlsD->setFocus();
    }
    else
    {
        lowFeatures.setControls(DIAMONDS, low);
        highFeatures.setControls(DIAMONDS, high);
    }

}

void CRuleDialog::on_controlsC_editingFinished()
{
    int low, high;

    QString text = ui->controlsC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxControls(CLUBS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->controlsC->setFocus();
    }
    else
    {
        lowFeatures.setControls(CLUBS, low);
        highFeatures.setControls(CLUBS, high);
    }
}

void CRuleDialog::on_controlsA_editingFinished()
{
    int low, high;

    QString text = ui->controlsA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxControls(ANY), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->controlsA->setFocus();
    }
    else
    {
        lowFeatures.setControls(ANY, low);
        highFeatures.setControls(ANY, high);
    }
}

void CRuleDialog::on_hcS_editingFinished()
{
    int low, high;

    QString text = ui->hcS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHc(SPADES), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcS->setFocus();
    }
    else
    {
        lowFeatures.setHc(SPADES, low);
        highFeatures.setHc(SPADES, high);
    }
}

void CRuleDialog::on_hcH_editingFinished()
{
    int low, high;

    QString text = ui->hcH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHc(HEARTS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcH->setFocus();
    }
    else
    {
        lowFeatures.setHc(HEARTS, low);
        highFeatures.setHc(HEARTS, high);
    }
}

void CRuleDialog::on_hcD_editingFinished()
{
    int low, high;

    QString text = ui->hcD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHc(DIAMONDS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcD->setFocus();
    }
    else
    {
        lowFeatures.setHc(DIAMONDS, low);
        highFeatures.setHc(DIAMONDS, high);
    }
}

void CRuleDialog::on_hcC_editingFinished()
{
    int low, high;

    QString text = ui->hcC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHc(CLUBS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcC->setFocus();
    }
    else
    {
        lowFeatures.setHc(CLUBS, low);
        highFeatures.setHc(CLUBS, high);
    }
}

void CRuleDialog::on_hcA_editingFinished()
{
    int low, high;

    QString text = ui->hcA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHc(ANY), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->hcA->setFocus();
    }
    else
    {
        lowFeatures.setHc(ANY, low);
        highFeatures.setHc(ANY, high);
    }
}

void CRuleDialog::on_honorS_editingFinished()
{
    int low, high;

    QString text = ui->honorS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHonor(SPADES), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->honorS->setFocus();
    }
    else
    {
        lowFeatures.setHonor(SPADES, low);
        highFeatures.setHonor(SPADES, high);
    }
}

void CRuleDialog::on_honorH_editingFinished()
{
    int low, high;

    QString text = ui->honorH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHonor(HEARTS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->honorH->setFocus();
    }
    else
    {
        lowFeatures.setHonor(HEARTS, low);
        highFeatures.setHonor(HEARTS, high);
    }
}

void CRuleDialog::on_honorD_editingFinished()
{
    int low, high;

    QString text = ui->honorD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHonor(DIAMONDS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->honorD->setFocus();
    }
    else
    {
        lowFeatures.setHonor(DIAMONDS, low);
        highFeatures.setHonor(DIAMONDS, high);
    }

}

void CRuleDialog::on_honorC_editingFinished()
{
    int low, high;

    QString text = ui->honorC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHonor(CLUBS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->honorC->setFocus();
    }
    else
    {
        lowFeatures.setHonor(CLUBS, low);
        highFeatures.setHonor(CLUBS, high);
    }
}

void CRuleDialog::on_honorA_editingFinished()
{
    int low, high;

    QString text = ui->honorA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxHonor(ANY), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->honorA->setFocus();
    }
    else
    {
        lowFeatures.setHonor(ANY, low);
        highFeatures.setHonor(ANY, high);
    }
}

void CRuleDialog::on_aceS_editingFinished()
{
    int low, high;

    QString text = ui->aceS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(SPADES), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->aceS->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(SPADES, ACE, low);
        highFeatures.setCountCard(SPADES, ACE, high);
    }
}

void CRuleDialog::on_aceH_editingFinished()
{
    int low, high;

    QString text = ui->aceH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(HEARTS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->aceH->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(HEARTS, ACE, low);
        highFeatures.setCountCard(HEARTS, ACE, high);
    }
}

void CRuleDialog::on_aceD_editingFinished()
{
    int low, high;

    QString text = ui->aceD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(DIAMONDS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->aceD->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(DIAMONDS, ACE, low);
        highFeatures.setCountCard(DIAMONDS, ACE, high);
    }
}

void CRuleDialog::on_aceC_editingFinished()
{
    int low, high;

    QString text = ui->aceC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(CLUBS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->aceC->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(CLUBS, ACE, low);
        highFeatures.setCountCard(CLUBS, ACE, high);
    }
}

void CRuleDialog::on_aceA_editingFinished()
{
    int low, high;

    QString text = ui->aceA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(ANY), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->aceA->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(ANY, ACE, low);
        highFeatures.setCountCard(ANY, ACE, high);
    }
}

void CRuleDialog::on_kingS_editingFinished()
{
    int low, high;

    QString text = ui->kingS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(SPADES), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->kingS->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(SPADES, KING, low);
        highFeatures.setCountCard(SPADES, KING, high);
    }

}

void CRuleDialog::on_kingH_editingFinished()
{
    int low, high;

    QString text = ui->kingH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(HEARTS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->kingH->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(HEARTS, KING, low);
        highFeatures.setCountCard(HEARTS, KING, high);
    }

}

void CRuleDialog::on_kingD_editingFinished()
{
    int low, high;

    QString text = ui->kingD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(DIAMONDS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->kingD->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(DIAMONDS, KING, low);
        highFeatures.setCountCard(DIAMONDS, KING, high);
    }
}

void CRuleDialog::on_kingC_editingFinished()
{
    int low, high;

    QString text = ui->kingC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(CLUBS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->kingC->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(CLUBS, KING, low);
        highFeatures.setCountCard(CLUBS, KING, high);
    }
}

void CRuleDialog::on_kingA_editingFinished()
{
    int low, high;

    QString text = ui->kingA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(ANY), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->kingA->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(ANY, KING, low);
        highFeatures.setCountCard(ANY, KING, high);
    }
}

void CRuleDialog::on_queenS_editingFinished()
{
    int low, high;

    QString text = ui->queenS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(SPADES), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->queenS->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(SPADES, QUEEN, low);
        highFeatures.setCountCard(SPADES, QUEEN, high);
    }
}

void CRuleDialog::on_queenH_editingFinished()
{
    int low, high;

    QString text = ui->queenH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(HEARTS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->queenH->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(HEARTS, QUEEN, low);
        highFeatures.setCountCard(HEARTS, QUEEN, high);
    }
}

void CRuleDialog::on_queenD_editingFinished()
{
    int low, high;

    QString text = ui->queenD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(DIAMONDS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->queenD->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(DIAMONDS, QUEEN, low);
        highFeatures.setCountCard(DIAMONDS, QUEEN, high);
    }
}

void CRuleDialog::on_queenC_editingFinished()
{
    int low, high;

    QString text = ui->queenC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(CLUBS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->queenC->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(CLUBS, QUEEN, low);
        highFeatures.setCountCard(CLUBS, QUEEN, high);
    }
}

void CRuleDialog::on_queenA_editingFinished()
{
    int low, high;

    QString text = ui->queenA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(ANY), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->queenA->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(ANY, QUEEN, low);
        highFeatures.setCountCard(ANY, QUEEN, high);
    }
}

void CRuleDialog::on_jackS_editingFinished()
{
    int low, high;

    QString text = ui->jackS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(SPADES), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->jackS->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(SPADES, JACK, low);
        highFeatures.setCountCard(SPADES, JACK, high);
    }
}

void CRuleDialog::on_jackH_editingFinished()
{
    int low, high;

    QString text = ui->jackH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(HEARTS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->jackH->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(HEARTS, JACK, low);
        highFeatures.setCountCard(HEARTS, JACK, high);
    }

}

void CRuleDialog::on_jackD_editingFinished()
{
    int low, high;

    QString text = ui->jackD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(DIAMONDS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->jackD->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(DIAMONDS, JACK, low);
        highFeatures.setCountCard(DIAMONDS, JACK, high);
    }

}

void CRuleDialog::on_jackC_editingFinished()
{
    int low, high;

    QString text = ui->jackC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(CLUBS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->jackC->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(CLUBS, JACK, low);
        highFeatures.setCountCard(CLUBS, JACK, high);
    }

}

void CRuleDialog::on_jackA_editingFinished()
{
    int low, high;

    QString text = ui->jackA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(ANY), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->jackA->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(ANY, JACK, low);
        highFeatures.setCountCard(ANY, JACK, high);
    }
}

void CRuleDialog::on_tenS_editingFinished()
{
    int low, high;

    QString text = ui->tenS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(SPADES), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->tenS->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(SPADES, TEN, low);
        highFeatures.setCountCard(SPADES, TEN, high);
    }
}

void CRuleDialog::on_tenH_editingFinished()
{
    int low, high;

    QString text = ui->tenH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(HEARTS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->tenH->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(HEARTS, TEN, low);
        highFeatures.setCountCard(HEARTS, TEN, high);
    }
}

void CRuleDialog::on_tenD_editingFinished()
{
    int low, high;

    QString text = ui->tenD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(DIAMONDS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->tenD->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(DIAMONDS, TEN, low);
        highFeatures.setCountCard(DIAMONDS, TEN, high);
    }
}

void CRuleDialog::on_tenC_editingFinished()
{
    int low, high;

    QString text = ui->tenC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(CLUBS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->tenC->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(CLUBS, TEN, low);
        highFeatures.setCountCard(CLUBS, TEN, high);
    }
}

void CRuleDialog::on_tenA_editingFinished()
{
    int low, high;

    QString text = ui->tenA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxCountCard(ANY), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->tenA->setFocus();
    }
    else
    {
        lowFeatures.setCountCard(ANY, TEN, low);
        highFeatures.setCountCard(ANY, TEN, high);
    }
}

void CRuleDialog::on_rkcbS_editingFinished()
{
    int low, high;

    QString text = ui->rkcbS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxRkcb(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->rkcbS->setFocus();
    }
    else
    {
        lowFeatures.setRkcb(SPADES, low);
        highFeatures.setRkcb(SPADES, high);
    }
}

void CRuleDialog::on_rkcbH_editingFinished()
{
    int low, high;

    QString text = ui->rkcbH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxRkcb(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->rkcbH->setFocus();
    }
    else
    {
        lowFeatures.setRkcb(HEARTS, low);
        highFeatures.setRkcb(HEARTS, high);
    }
}

void CRuleDialog::on_rkcbD_editingFinished()
{
    int low, high;

    QString text = ui->rkcbD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxRkcb(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->rkcbD->setFocus();
    }
    else
    {
        lowFeatures.setRkcb(DIAMONDS, low);
        highFeatures.setRkcb(DIAMONDS, high);
    }
}

void CRuleDialog::on_rkcbC_editingFinished()
{
    int low, high;

    QString text = ui->rkcbC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxRkcb(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->rkcbC->setFocus();
    }
    else
    {
        lowFeatures.setRkcb(CLUBS, low);
        highFeatures.setRkcb(CLUBS, high);
    }
}

void CRuleDialog::on_interS_editingFinished()
{
    int low, high;

    QString text = ui->interS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxInter(SPADES), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->interS->setFocus();
    }
    else
    {
        lowFeatures.setInter(SPADES, low);
        highFeatures.setInter(SPADES, high);
    }
}

void CRuleDialog::on_interH_editingFinished()
{
    int low, high;

    QString text = ui->interH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxInter(HEARTS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->interH->setFocus();
    }
    else
    {
        lowFeatures.setInter(HEARTS, low);
        highFeatures.setInter(HEARTS, high);
    }
}

void CRuleDialog::on_interD_editingFinished()
{
    int low, high;

    QString text = ui->interD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxInter(DIAMONDS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->interD->setFocus();
    }
    else
    {
        lowFeatures.setInter(DIAMONDS, low);
        highFeatures.setInter(DIAMONDS, high);
    }
}

void CRuleDialog::on_interC_editingFinished()
{
    int low, high;

    QString text = ui->interC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxInter(CLUBS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->interC->setFocus();
    }
    else
    {
        lowFeatures.setInter(CLUBS, low);
        highFeatures.setInter(CLUBS, high);
    }

}

void CRuleDialog::on_interA_editingFinished()
{
    int low, high;

    QString text = ui->interA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxInter(ANY), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->interA->setFocus();
    }
    else
    {
        lowFeatures.setInter(ANY, low);
        highFeatures.setInter(ANY, high);
    }
}

void CRuleDialog::on_loserS_editingFinished()
{
    int low, high;

    QString text = ui->loserS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLoosers(SPADES), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->loserS->setFocus();
    }
    else
    {
        lowFeatures.setLoosers(SPADES, low);
        highFeatures.setLoosers(SPADES, high);
    }
}

void CRuleDialog::on_loserH_editingFinished()
{
    int low, high;

    QString text = ui->loserH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLoosers(HEARTS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->loserH->setFocus();
    }
    else
    {
        lowFeatures.setLoosers(HEARTS, low);
        highFeatures.setLoosers(HEARTS, high);
    }
}

void CRuleDialog::on_loserD_editingFinished()
{
    int low, high;

    QString text = ui->loserD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLoosers(DIAMONDS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->loserD->setFocus();
    }
    else
    {
        lowFeatures.setLoosers(DIAMONDS, low);
        highFeatures.setLoosers(DIAMONDS, high);
    }
}

void CRuleDialog::on_loserC_editingFinished()
{
    int low, high;

    QString text = ui->loserC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLoosers(CLUBS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->loserC->setFocus();
    }
    else
    {
        lowFeatures.setLoosers(CLUBS, low);
        highFeatures.setLoosers(CLUBS, high);
    }
}

void CRuleDialog::on_loserA_editingFinished()
{
    int low, high;

    QString text = ui->loserA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLoosers(ANY), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->loserA->setFocus();
    }
    else
    {
        lowFeatures.setLoosers(ANY, low);
        highFeatures.setLoosers(ANY, high);
    }
}

void CRuleDialog::on_quickS_editingFinished()
{
    int low, high;

    QString text = ui->quickS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxQuickTricks(SPADES), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->quickS->setFocus();
    }
    else
    {
        lowFeatures.setQuickTricks(SPADES, low);
        highFeatures.setQuickTricks(SPADES, high);
    }
}

void CRuleDialog::on_quickH_editingFinished()
{
    int low, high;

    QString text = ui->quickH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxQuickTricks(HEARTS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->quickH->setFocus();
    }
    else
    {
        lowFeatures.setQuickTricks(HEARTS, low);
        highFeatures.setQuickTricks(HEARTS, high);
    }
}

void CRuleDialog::on_quickD_editingFinished()
{
    int low, high;

    QString text = ui->quickD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxQuickTricks(DIAMONDS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->quickD->setFocus();
    }
    else
    {
        lowFeatures.setQuickTricks(DIAMONDS, low);
        highFeatures.setQuickTricks(DIAMONDS, high);
    }
}

void CRuleDialog::on_quickC_editingFinished()
{
    int low, high;

    QString text = ui->quickC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxQuickTricks(CLUBS), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->quickC->setFocus();
    }
    else
    {
        lowFeatures.setQuickTricks(CLUBS, low);
        highFeatures.setQuickTricks(CLUBS, high);
    }
}

void CRuleDialog::on_quickA_editingFinished()
{
    int low, high;

    QString text = ui->quickA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxQuickTricks(ANY), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->quickA->setFocus();
    }
    else
    {
        lowFeatures.setQuickTricks(ANY, low);
        highFeatures.setQuickTricks(ANY, high);
    }
}

void CRuleDialog::on_playS_editingFinished()
{
    int low, high;

    QString text = ui->playS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxPlayingTricks(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->playS->setFocus();
    }
    else
    {
        lowFeatures.setPlayingTricks(SPADES, low);
        highFeatures.setPlayingTricks(SPADES, high);
    }
}

void CRuleDialog::on_playH_editingFinished()
{
    int low, high;

    QString text = ui->playH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxPlayingTricks(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->playH->setFocus();
    }
    else
    {
        lowFeatures.setPlayingTricks(HEARTS, low);
        highFeatures.setPlayingTricks(HEARTS, high);
    }
}

void CRuleDialog::on_playD_editingFinished()
{
    int low, high;

    QString text = ui->playD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxPlayingTricks(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->playD->setFocus();
    }
    else
    {
        lowFeatures.setPlayingTricks(DIAMONDS, low);
        highFeatures.setPlayingTricks(DIAMONDS, high);
    }
}

void CRuleDialog::on_playC_editingFinished()
{
    int low, high;

    QString text = ui->playC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxPlayingTricks(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->playC->setFocus();
    }
    else
    {
        lowFeatures.setPlayingTricks(CLUBS, low);
        highFeatures.setPlayingTricks(CLUBS, high);
    }
}

void CRuleDialog::on_playA_editingFinished()
{
    int low, high;

    QString text = ui->playA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxPlayingTricks(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->playA->setFocus();
    }
    else
    {
        lowFeatures.setPlayingTricks(ANY, low);
        highFeatures.setPlayingTricks(ANY, high);
    }
}

void CRuleDialog::on_stopNTS_editingFinished()
{
    int low, high;

    QString text = ui->stopNTS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxStopNT(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->stopNTS->setFocus();
    }
    else
    {
        lowFeatures.setStopNT(SPADES, low);
        highFeatures.setStopNT(SPADES, high);
    }
}

void CRuleDialog::on_stopNTH_editingFinished()
{
    int low, high;

    QString text = ui->stopNTH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxStopNT(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->stopNTH->setFocus();
    }
    else
    {
        lowFeatures.setStopNT(HEARTS, low);
        highFeatures.setStopNT(HEARTS, high);
    }
}

void CRuleDialog::on_stopNTD_editingFinished()
{
    int low, high;

    QString text = ui->stopNTD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxStopNT(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->stopNTD->setFocus();
    }
    else
    {
        lowFeatures.setStopNT(DIAMONDS, low);
        highFeatures.setStopNT(DIAMONDS, high);
    }
}

void CRuleDialog::on_stopNTC_editingFinished()
{
    int low, high;

    QString text = ui->stopNTC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxStopNT(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->stopNTC->setFocus();
    }
    else
    {
        lowFeatures.setStopNT(CLUBS, low);
        highFeatures.setStopNT(CLUBS, high);
    }
}

void CRuleDialog::on_qltyS_editingFinished()
{
    int low, high;

    QString text = ui->qltyS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxQlty(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->qltyS->setFocus();
    }
    else
    {
        lowFeatures.setQlty(SPADES, low);
        highFeatures.setQlty(SPADES, high);
    }
}

void CRuleDialog::on_qltyH_editingFinished()
{
    int low, high;

    QString text = ui->qltyH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxQlty(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->qltyH->setFocus();
    }
    else
    {
        lowFeatures.setQlty(HEARTS, low);
        highFeatures.setQlty(HEARTS, high);
    }
}

void CRuleDialog::on_qltyD_editingFinished()
{
    int low, high;

    QString text = ui->qltyD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxQlty(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->qltyD->setFocus();
    }
    else
    {
        lowFeatures.setQlty(DIAMONDS, low);
        highFeatures.setQlty(DIAMONDS, high);
    }
}

void CRuleDialog::on_qltyC_editingFinished()
{
    int low, high;

    QString text = ui->qltyC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxQlty(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->qltyC->setFocus();
    }
    else
    {
        lowFeatures.setQlty(CLUBS, low);
        highFeatures.setQlty(CLUBS, high);
    }
}

void CRuleDialog::on_pointsS_editingFinished()
{
    int low, high;

    QString text = ui->pointsS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxPoints(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->pointsS->setFocus();
    }
    else
    {
        lowFeatures.setPoints(SPADES, low);
        highFeatures.setPoints(SPADES, high);
    }
}

void CRuleDialog::on_pointsH_editingFinished()
{
    int low, high;

    QString text = ui->pointsH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxPoints(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->pointsH->setFocus();
    }
    else
    {
        lowFeatures.setPoints(HEARTS, low);
        highFeatures.setPoints(HEARTS, high);
    }
}

void CRuleDialog::on_pointsD_editingFinished()
{
    int low, high;

    QString text = ui->pointsD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxPoints(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->pointsD->setFocus();
    }
    else
    {
        lowFeatures.setPoints(DIAMONDS, low);
        highFeatures.setPoints(DIAMONDS, high);
    }
}

void CRuleDialog::on_pointsC_editingFinished()
{
    int low, high;

    QString text = ui->pointsC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxPoints(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->pointsC->setFocus();
    }
    else
    {
        lowFeatures.setPoints(CLUBS, low);
        highFeatures.setPoints(CLUBS, high);
    }
}

void CRuleDialog::on_pointsA_editingFinished()
{
    int low, high;

    QString text = ui->pointsA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxPoints(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->pointsA->setFocus();
    }
    else
    {
        lowFeatures.setPoints(ANY, low);
        highFeatures.setPoints(ANY, high);
    }
}

void CRuleDialog::on_dpS_editingFinished()
{
    int low, high;

    QString text = ui->dpS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxDp(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->dpS->setFocus();
    }
    else
    {
        lowFeatures.setDp(SPADES, low);
        highFeatures.setDp(SPADES, high);
    }
}

void CRuleDialog::on_dpH_editingFinished()
{
    int low, high;

    QString text = ui->dpH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxDp(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->dpH->setFocus();
    }
    else
    {
        lowFeatures.setDp(HEARTS, low);
        highFeatures.setDp(HEARTS, high);
    }
}

void CRuleDialog::on_dpD_editingFinished()
{
    int low, high;

    QString text = ui->dpD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxDp(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->dpD->setFocus();
    }
    else
    {
        lowFeatures.setDp(DIAMONDS, low);
        highFeatures.setDp(DIAMONDS, high);
    }
}

void CRuleDialog::on_dpC_editingFinished()
{
    int low, high;

    QString text = ui->dpC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxDp(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->dpC->setFocus();
    }
    else
    {
        lowFeatures.setDp(CLUBS, low);
        highFeatures.setDp(CLUBS, high);
    }
}

void CRuleDialog::on_dpA_editingFinished()
{
    int low, high;

    QString text = ui->dpA->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxDp(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->dpA->setFocus();
    }
    else
    {
        lowFeatures.setDp(ANY, low);
        highFeatures.setDp(ANY, high);
    }
}

void CRuleDialog::on_LengthS_editingFinished()
{
    int low, high;

    QString text = ui->LengthS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxSuitLen(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->LengthS->setFocus();
    }
    else
    {
        lowFeatures.setSuitLen(SPADES, low);
        highFeatures.setSuitLen(SPADES, high);
    }
}

void CRuleDialog::on_LengthH_editingFinished()
{
    int low, high;

    QString text = ui->LengthH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxSuitLen(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->LengthH->setFocus();
    }
    else
    {
        lowFeatures.setSuitLen(HEARTS, low);
        highFeatures.setSuitLen(HEARTS, high);
    }
}

void CRuleDialog::on_LengthD_editingFinished()
{
    int low, high;

    QString text = ui->LengthD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxSuitLen(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->LengthD->setFocus();
    }
    else
    {
        lowFeatures.setSuitLen(DIAMONDS, low);
        highFeatures.setSuitLen(DIAMONDS, high);
    }
}

void CRuleDialog::on_LengthC_editingFinished()
{
    int low, high;

    QString text = ui->LengthC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxSuitLen(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->LengthC->setFocus();
    }
    else
    {
        lowFeatures.setSuitLen(CLUBS, low);
        highFeatures.setSuitLen(CLUBS, high);
    }
}

void CRuleDialog::on_longestS_editingFinished()
{
    int low, high;

    QString text = ui->longestS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLongest(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->longestS->setFocus();
    }
    else
    {
        lowFeatures.setLongest(SPADES, low);
        highFeatures.setLongest(SPADES, high);
    }
}

void CRuleDialog::on_longestH_editingFinished()
{
    int low, high;

    QString text = ui->longestH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLongest(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->longestH->setFocus();
    }
    else
    {
        lowFeatures.setLongest(HEARTS, low);
        highFeatures.setLongest(HEARTS, high);
    }
}

void CRuleDialog::on_longestD_editingFinished()
{
    int low, high;

    QString text = ui->longestD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLongest(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->longestD->setFocus();
    }
    else
    {
        lowFeatures.setLongest(DIAMONDS, low);
        highFeatures.setLongest(DIAMONDS, high);
    }
}

void CRuleDialog::on_longestC_editingFinished()
{
    int low, high;

    QString text = ui->longestC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLongest(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->longestC->setFocus();
    }
    else
    {
        lowFeatures.setLongest(CLUBS, low);
        highFeatures.setLongest(CLUBS, high);
    }
}

void CRuleDialog::on_longS_editingFinished()
{
    int low, high;

    QString text = ui->longS->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLong(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->longS->setFocus();
    }
    else
    {
        lowFeatures.setLong(SPADES, low);
        highFeatures.setLong(SPADES, high);
    }
}

void CRuleDialog::on_longH_editingFinished()
{
    int low, high;

    QString text = ui->longH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLong(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->longH->setFocus();
    }
    else
    {
        lowFeatures.setLong(HEARTS, low);
        highFeatures.setLong(HEARTS, high);
    }
}

void CRuleDialog::on_longD_editingFinished()
{
    int low, high;

    QString text = ui->longD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLong(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->longD->setFocus();
    }
    else
    {
        lowFeatures.setLong(DIAMONDS, low);
        highFeatures.setLong(DIAMONDS, high);
    }
}

void CRuleDialog::on_longC_editingFinished()
{
    int low, high;

    QString text = ui->longC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLong(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->longC->setFocus();
    }
    else
    {
        lowFeatures.setLong(CLUBS, low);
        highFeatures.setLong(CLUBS, high);
    }
}

void CRuleDialog::on_difSH_editingFinished()
{
    int low, high;

    QString text = ui->difSH->text();

    bool ok = getValues(text, -lowFeatures.getMaxDif2(), lowFeatures.getMaxDif2(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->difSH->setFocus();
    }
    else
    {
        lowFeatures.setDif2(SPADES, HEARTS, low);
        highFeatures.setDif2(SPADES, HEARTS, high);
    }
}

void CRuleDialog::on_difSD_editingFinished()
{
    int low, high;

    QString text = ui->difSD->text();

    bool ok = getValues(text, -lowFeatures.getMaxDif2(), lowFeatures.getMaxDif2(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->difSD->setFocus();
    }
    else
    {
        lowFeatures.setDif2(SPADES, DIAMONDS, low);
        highFeatures.setDif2(SPADES, DIAMONDS, high);
    }
}

void CRuleDialog::on_difHD_editingFinished()
{
    int low, high;

    QString text = ui->difHD->text();

    bool ok = getValues(text, -lowFeatures.getMaxDif2(), lowFeatures.getMaxDif2(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->difHD->setFocus();
    }
    else
    {
        lowFeatures.setDif2(HEARTS, DIAMONDS, low);
        highFeatures.setDif2(HEARTS, DIAMONDS, high);
    }
}

void CRuleDialog::on_difSC_editingFinished()
{
    int low, high;

    QString text = ui->difSC->text();

    bool ok = getValues(text, -lowFeatures.getMaxDif2(), lowFeatures.getMaxDif2(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->difSC->setFocus();
    }
    else
    {
        lowFeatures.setDif2(SPADES, CLUBS, low);
        highFeatures.setDif2(SPADES, CLUBS, high);
    }
}

void CRuleDialog::on_difHC_editingFinished()
{
    int low, high;

    QString text = ui->difHC->text();

    bool ok = getValues(text, -lowFeatures.getMaxDif2(), lowFeatures.getMaxDif2(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->difHC->setFocus();
    }
    else
    {
        lowFeatures.setDif2(HEARTS, CLUBS, low);
        highFeatures.setDif2(HEARTS, CLUBS, high);
    }
}

void CRuleDialog::on_difDC_editingFinished()
{
    int low, high;

    QString text = ui->difDC->text();

    bool ok = getValues(text, -lowFeatures.getMaxDif2(), lowFeatures.getMaxDif2(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->difDC->setFocus();
    }
    else
    {
        lowFeatures.setDif2(DIAMONDS, CLUBS, low);
        highFeatures.setDif2(DIAMONDS, CLUBS, high);
    }
}

void CRuleDialog::on_sumSH_editingFinished()
{
    int low, high;

    QString text = ui->sumSH->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxSuitLen2(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->sumSH->setFocus();
    }
    else
    {
        lowFeatures.setSuitLen2(SPADES, HEARTS, low);
        highFeatures.setSuitLen2(SPADES, HEARTS, high);
    }
}

void CRuleDialog::on_sumSD_editingFinished()
{
    int low, high;

    QString text = ui->sumSD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxSuitLen2(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->sumSD->setFocus();
    }
    else
    {
        lowFeatures.setSuitLen2(SPADES, DIAMONDS, low);
        highFeatures.setSuitLen2(SPADES, DIAMONDS, high);
    }
}

void CRuleDialog::on_sumHD_editingFinished()
{
    int low, high;

    QString text = ui->sumHD->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxSuitLen2(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->sumHD->setFocus();
    }
    else
    {
        lowFeatures.setSuitLen2(HEARTS, DIAMONDS, low);
        highFeatures.setSuitLen2(HEARTS, DIAMONDS, high);
    }
}

void CRuleDialog::on_sumSC_editingFinished()
{
    int low, high;

    QString text = ui->sumSC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxSuitLen2(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->sumSC->setFocus();
    }
    else
    {
        lowFeatures.setSuitLen2(SPADES, CLUBS, low);
        highFeatures.setSuitLen2(SPADES, CLUBS, high);
    }
}

void CRuleDialog::on_sumHC_editingFinished()
{
    int low, high;

    QString text = ui->sumHC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxSuitLen2(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->sumHC->setFocus();
    }
    else
    {
        lowFeatures.setSuitLen2(HEARTS, CLUBS, low);
        highFeatures.setSuitLen2(HEARTS, CLUBS, high);
    }
}

void CRuleDialog::on_sumDC_editingFinished()
{
    int low, high;

    QString text = ui->sumDC->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxSuitLen2(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->sumDC->setFocus();
    }
    else
    {
        lowFeatures.setSuitLen2(DIAMONDS, CLUBS, low);
        highFeatures.setSuitLen2(DIAMONDS, CLUBS, high);
    }
}

void CRuleDialog::on_balanced_editingFinished()
{
    int low, high;

    QString text = ui->balanced->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxBal(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->balanced->setFocus();
    }
    else
    {
        lowFeatures.setBal(low);
        highFeatures.setBal(high);
    }
}

void CRuleDialog::on_longerThan3_editingFinished()
{
    int low, high;

    QString text = ui->longerThan3->text();

    bool ok = getValues(text, 1, lowFeatures.getMaxSuits(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->longerThan3->setFocus();
    }
    else
    {
        lowFeatures.setSuits(low);
        highFeatures.setSuits(high);
    }
}

void CRuleDialog::on_longerMajor_editingFinished()
{
    int low, high;

    QString text = ui->longerMajor->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLMaj(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->longerMajor->setFocus();
    }
    else
    {
        lowFeatures.setLMaj(low);
        highFeatures.setLMaj(high);
    }
}

void CRuleDialog::on_longerMinor_editingFinished()
{
    int low, high;

    QString text = ui->longerMinor->text();

    bool ok = getValues(text, 0, lowFeatures.getMaxLMin(), &low, &high);
    if (!ok)
    {
        QMessageBox::critical(0, tr("ZBridge"), tr("Illegal value: ") + text);
        ui->longerMinor->setFocus();
    }
    else
    {
        lowFeatures.setLMin(low);
        highFeatures.setLMin(high);
    }
}

void CRuleDialog::on_buttonBox_accepted()
{
    rule->setId(id);
    rule->setPriority(priority);
    rule->setStatus(status);
    rule->setScoringMethod(scoringMethod);
    rule->setVulnerability(vulnerability);
    rule->setAlertId(alertId);
    rule->setFeatures(lowFeatures, highFeatures);

    QString text = ui->ruleIdDesc->toPlainText();
    bidDesc->setRuleIdDesc(id, text);
    text = ui->alertIdDesc->toPlainText();
    bidDesc->setAlertIdDesc(alertId, text);

    accept();
}

void CRuleDialog::on_buttonBox_rejected()
{
    reject();
}

void CRuleDialog::setText(QLineEdit *text, int low, int high, int min, int max)
{
    if ((low == min) && (high == max))
        text->setText("");
    else if (low == high)
        text->setText(QString("%1").arg(low));
    else if (high == max)
        text->setText(QString("%1+").arg(low));
    else if (low == min)
        text->setText(QString("+%1").arg(high));
    else
        text->setText(QString("%1-%2").arg(low).arg(high));
}

bool CRuleDialog::getValues(QString &text, int min, int max, int *low, int *high)
{
    int inx, count;
    bool ok = true;

    text = text.trimmed();
    count = text.count('-');
    if ((text.size() == 0) || (text == "-"))
    {
        *low = min;
        *high = max;
    }
    //(-)ddd+
    else if (text.contains('+') && text.endsWith('+') && (text.size() > 1))
    {
        *low = text.left(text.size() - 1).toInt(&ok);
        *high = max;
    }
    //+(-)ddd
    else if (text.contains('+') && text.startsWith('+') && (text.size() > 1))
    {
        *low = min;
        *high = text.right(1).toInt(&ok);
    }
    //(-)ddd
    else if (!text.contains('+') && (!text.contains('-') || ((count == 1) && (text.startsWith('-')))))
        *low = *high = text.toInt(&ok);
    //ddd-ddd, -ddd-ddd, ddd--ddd, -ddd--ddd
    else if (((count == 1) && (!text.startsWith('-'))) || (count == 2) || (count == 3))
    {
        inx = text.indexOf('-');
        if (inx == 0)
            inx = text.indexOf('-', inx + 1);
        *low = text.left(inx).toInt(&ok);
        if (ok)
            *high = text.right(text.size() - inx - 1).toInt(&ok);
    }
    else
        ok = false;

    if (ok)
        ok = (*low >= min) && (*high >= min) && (*low <= max) && (*high <= max) && (*low <= *high);

    return ok;
}
