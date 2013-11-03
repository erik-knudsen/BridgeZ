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

#include "cdealOptionspropsheet.h"
#include "ui_cdealOptionspropsheet.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"

CDealOptionsPropSheet::CDealOptionsPropSheet(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDealOptionsPropSheet)
{
    ui->setupUi(this);
    this->app = app;
    this->doc = doc;

    dealOptionDoc = doc->getDealOptions();

    //Points.
    ui->minGamePts->setValue(dealOptionDoc.minGamePts);
    ui->maxGamePts->setValue(dealOptionDoc.maxGamePts);
    ui->minMinorGamePts->setValue(dealOptionDoc.minMinorGamePts);
    ui->maxMinorGamePts->setValue(dealOptionDoc.maxMinorGamePts);
    ui->minMajorGamePts->setValue(dealOptionDoc.minMajorGamePts);
    ui->maxMajorGamePts->setValue(dealOptionDoc.maxMajorGamePts);
    ui->minNoTrumpGamePts->setValue(dealOptionDoc.minNoTrumpGamePts);
    ui->maxNoTrumpGamePts->setValue(dealOptionDoc.maxNoTrumpGamePts);
    ui->minSlamPts->setValue(dealOptionDoc.minSlamPts);
    ui->maxSlamPts->setValue(dealOptionDoc.maxSlamPts);
    ui->minSmallSlamPts->setValue(dealOptionDoc.minSmallSlamPts);
    ui->maxSmallSlamPts->setValue(dealOptionDoc.maxSmallSlamPts);
    ui->minGrandSlamPts->setValue(dealOptionDoc.minGrandSlamPts);
    ui->maxGrandSlamPts->setValue(dealOptionDoc.maxGrandSlamPts);

    //Minors.
    if (dealOptionDoc.minCardsInMinor == SEVEN_MIN_CARDS_IN_MINOR)
        ui->sevenMinCardsInMinor->setChecked(true);
    else if (dealOptionDoc.minCardsInMinor == EIGHT_MIN_CARDS_IN_MINOR)
        ui->eightMinCardsInMinor->setChecked(true);
    else
        ui->nineMinCardsInMinor->setChecked(true);

    if (dealOptionDoc.minTopMinorCard == ACE_MIN_TOP_MINOR_CARD)
        ui->aceMinTopMinorCard->setChecked(true);
    else if (dealOptionDoc.minTopMinorCard == KING_MIN_TOP_MINOR_CARD)
        ui->kingMinTopMinorCard->setChecked(true);
    else
        ui->queenMinTopMinorCard->setChecked(true);

    if (dealOptionDoc.minSuitDistMinor == FOUR_THREE_MIN_SUIT_DIST_MINOR)
        ui->fourThreeMinSuitDistMinor->setChecked(true);
    else if (dealOptionDoc.minSuitDistMinor == FOUR_FOUR_MIN_SUIT_DIST_MINOR)
        ui->fourFourMinSuitDistMinor->setChecked(true);
    else if (dealOptionDoc.minSuitDistMinor == FIVE_THREE_MIN_SUIT_DIST_MINOR)
        ui->fiveThreeMinSuitDistMinor->setChecked(true);
    else
        ui->fiveFourMinSuitDistMinor->setChecked(true);

    //Majors.
    if (dealOptionDoc.minCardsInMajor == SEVEN_MIN_CARDS_IN_MAJOR)
        ui->sevenMinCardsInMajor->setChecked(true);
    else if (dealOptionDoc.minCardsInMinor == EIGHT_MIN_CARDS_IN_MAJOR)
        ui->eightMinCardsInMajor->setChecked(true);
    else
        ui->nineMinCardsInMajor->setChecked(true);

    if (dealOptionDoc.minTopMajorCard == ACE_MIN_TOP_MAJOR_CARD)
        ui->aceMinTopMajorCard->setChecked(true);
    else if (dealOptionDoc.minTopMajorCard == KING_MIN_TOP_MAJOR_CARD)
        ui->kingMinTopMajorCard->setChecked(true);
    else
        ui->queenMinTopMajorCard->setChecked(true);

    if (dealOptionDoc.minSuitDistMajor == FOUR_THREE_MIN_SUIT_DIST_MAJOR)
        ui->fourThreeMinSuitDistMajor->setChecked(true);
    else if (dealOptionDoc.minSuitDistMajor == FOUR_FOUR_MIN_SUIT_DIST_MAJOR)
        ui->fourFourMinSuitDistMajor->setChecked(true);
    else if (dealOptionDoc.minSuitDistMajor == FIVE_THREE_MIN_SUIT_DIST_MAJOR)
        ui->fiveThreeMinSuitDistMajor->setChecked(true);
    else
        ui->fiveFourMinSuitDistMajor->setChecked(true);

    //No Trumps.
    ui->needTwoBalancedHandsForNT->setChecked(dealOptionDoc.needTwoBalancedHandsForNT);

    //Slam
    if (dealOptionDoc.acesNeededForSlam == FOUR_ACES_NEEDED_FOR_SLAM)
        ui->fourAcesNeededForSlam->setChecked(true);
    else
        ui->threeAcesNeededForSlam->setChecked(true);

    if (dealOptionDoc.acesNeededForSmallSlam == FOUR_ACES_NEEDED_FOR_SMALL_SLAM)
        ui->fourAcesNeededForSmallSlam->setChecked(true);
    else
        ui->threeAcesNeededForSmallSlam->setChecked(true);

    if (dealOptionDoc.kingsNeededForSlam == FOUR_KINGS_NEEDED_FOR_SLAM)
        ui->fourKingsNeededForSlam->setChecked(true);
    else if (dealOptionDoc.kingsNeededForSlam == THREE_KINGS_NEEDED_FOR_SLAM)
        ui->threeKingsNeededForSlam->setChecked(true);
    else
        ui->twoKingsNeededForSlam->setChecked(true);

    if (dealOptionDoc.kingsNeededForSmallSlam == FOUR_KINGS_NEEDED_FOR_SMALL_SLAM)
        ui->fourKingsNeededForSmallSlam->setChecked(true);
    else if (dealOptionDoc.kingsNeededForSmallSlam == THREE_KINGS_NEEDED_FOR_SMALL_SLAM)
        ui->threeKingsNeededForSmallSlam->setChecked(true);
    else
        ui->twoKingsNeededForSmallSlam->setChecked(true);

    if (dealOptionDoc.kingsNeededForGrandSlam == FOUR_KINGS_NEEDED_FOR_GRAND_SLAM)
        ui->fourKingsNeededForGrandSlam->setChecked(true);
    else
        ui->threeKingsNeededForGrandSlam->setChecked(true);

    //Misc.
    ui->giveSouthBestHand->setChecked(dealOptionDoc.giveSouthBestHand);
    ui->balancedHands->setChecked(dealOptionDoc.balancedHands);
    ui->enableDealNumbering->setChecked(dealOptionDoc.enableDealNumbering);
}

CDealOptionsPropSheet::~CDealOptionsPropSheet()
{
    delete ui;
}

void CDealOptionsPropSheet::on_buttonBox_accepted()
{
    doc->setDealOptions(dealOptionDoc);
}

void CDealOptionsPropSheet::on_minGamePts_valueChanged(int arg1)
{
    dealOptionDoc.minGamePts = arg1;
}

void CDealOptionsPropSheet::on_maxGamePts_valueChanged(int arg1)
{
    dealOptionDoc.maxGamePts = arg1;
}

void CDealOptionsPropSheet::on_minMajorGamePts_valueChanged(int arg1)
{
    dealOptionDoc.minMajorGamePts = arg1;
}

void CDealOptionsPropSheet::on_maxMajorGamePts_valueChanged(int arg1)
{
    dealOptionDoc.maxMajorGamePts = arg1;
}

void CDealOptionsPropSheet::on_minMinorGamePts_valueChanged(int arg1)
{
    dealOptionDoc.minMinorGamePts = arg1;
}

void CDealOptionsPropSheet::on_maxMinorGamePts_valueChanged(int arg1)
{
    dealOptionDoc.maxMinorGamePts = arg1;
}

void CDealOptionsPropSheet::on_minNoTrumpGamePts_valueChanged(int arg1)
{
    dealOptionDoc.minNoTrumpGamePts = arg1;
}

void CDealOptionsPropSheet::on_maxNoTrumpGamePts_valueChanged(int arg1)
{
    dealOptionDoc.maxNoTrumpGamePts = arg1;
}

void CDealOptionsPropSheet::on_minSlamPts_valueChanged(int arg1)
{
    dealOptionDoc.minSlamPts = arg1;
}

void CDealOptionsPropSheet::on_maxSlamPts_valueChanged(int arg1)
{
    dealOptionDoc.maxSlamPts = arg1;
}

void CDealOptionsPropSheet::on_minSmallSlamPts_valueChanged(int arg1)
{
    dealOptionDoc.minSmallSlamPts = arg1;
}

void CDealOptionsPropSheet::on_maxSmallSlamPts_valueChanged(int arg1)
{
    dealOptionDoc.maxSmallSlamPts = arg1;
}

void CDealOptionsPropSheet::on_minGrandSlamPts_valueChanged(int arg1)
{
    dealOptionDoc.minGrandSlamPts = arg1;
}

void CDealOptionsPropSheet::on_maxGrandSlamPts_valueChanged(int arg1)
{
    dealOptionDoc.maxGrandSlamPts = arg1;
}

void CDealOptionsPropSheet::on_sevenMinCardsInMinor_clicked()
{
    dealOptionDoc.minCardsInMinor = SEVEN_MIN_CARDS_IN_MINOR;
}

void CDealOptionsPropSheet::on_eightMinCardsInMinor_clicked()
{
    dealOptionDoc.minCardsInMinor = EIGHT_MIN_CARDS_IN_MINOR;
}

void CDealOptionsPropSheet::on_nineMinCardsInMinor_clicked()
{
    dealOptionDoc.minCardsInMinor = NINE_MIN_CARDS_IN_MINOR;
}

void CDealOptionsPropSheet::on_aceMinTopMinorCard_clicked()
{
    dealOptionDoc.minTopMinorCard = ACE_MIN_TOP_MINOR_CARD;
}

void CDealOptionsPropSheet::on_kingMinTopMinorCard_clicked()
{
    dealOptionDoc.minTopMinorCard = KING_MIN_TOP_MINOR_CARD;
}

void CDealOptionsPropSheet::on_queenMinTopMinorCard_clicked()
{
    dealOptionDoc.minTopMinorCard = QUEEN_MIN_TOP_MINOR_CARD;
}

void CDealOptionsPropSheet::on_fourThreeMinSuitDistMinor_clicked()
{
    dealOptionDoc.minSuitDistMinor = FOUR_THREE_MIN_SUIT_DIST_MINOR;
}

void CDealOptionsPropSheet::on_fourFourMinSuitDistMinor_clicked()
{
    dealOptionDoc.minSuitDistMinor = FOUR_FOUR_MIN_SUIT_DIST_MINOR;
}

void CDealOptionsPropSheet::on_fiveThreeMinSuitDistMinor_clicked()
{
    dealOptionDoc.minSuitDistMinor = FIVE_THREE_MIN_SUIT_DIST_MINOR;
}

void CDealOptionsPropSheet::on_fiveFourMinSuitDistMinor_clicked()
{
    dealOptionDoc.minSuitDistMinor = FIVE_FOUR_MIN_SUIT_DIST_MINOR;
}

void CDealOptionsPropSheet::on_sevenMinCardsInMajor_clicked()
{
    dealOptionDoc.minCardsInMajor = SEVEN_MIN_CARDS_IN_MAJOR;
}

void CDealOptionsPropSheet::on_eightMinCardsInMajor_clicked()
{
    dealOptionDoc.minCardsInMajor = EIGHT_MIN_CARDS_IN_MAJOR;
}

void CDealOptionsPropSheet::on_nineMinCardsInMajor_clicked()
{
    dealOptionDoc.minCardsInMajor = SEVEN_MIN_CARDS_IN_MAJOR;
}

void CDealOptionsPropSheet::on_aceMinTopMajorCard_clicked()
{
    dealOptionDoc.minTopMajorCard = ACE_MIN_TOP_MAJOR_CARD;
}

void CDealOptionsPropSheet::on_kingMinTopMajorCard_clicked()
{
    dealOptionDoc.minTopMajorCard = KING_MIN_TOP_MAJOR_CARD;
}

void CDealOptionsPropSheet::on_queenMinTopMajorCard_clicked()
{
    dealOptionDoc.minTopMajorCard = QUEEN_MIN_TOP_MAJOR_CARD;
}

void CDealOptionsPropSheet::on_fourThreeMinSuitDistMajor_clicked()
{
    dealOptionDoc.minSuitDistMajor = FOUR_THREE_MIN_SUIT_DIST_MAJOR;
}

void CDealOptionsPropSheet::on_fourFourMinSuitDistMajor_clicked()
{
    dealOptionDoc.minSuitDistMajor = FOUR_FOUR_MIN_SUIT_DIST_MAJOR;
}

void CDealOptionsPropSheet::on_fiveThreeMinSuitDistMajor_clicked()
{
    dealOptionDoc.minSuitDistMajor = FIVE_THREE_MIN_SUIT_DIST_MAJOR;
}

void CDealOptionsPropSheet::on_fiveFourMinSuitDistMajor_clicked()
{
    dealOptionDoc.minSuitDistMajor = FIVE_FOUR_MIN_SUIT_DIST_MAJOR;
}

void CDealOptionsPropSheet::on_needTwoBalancedHandsForNT_clicked(bool checked)
{
    dealOptionDoc.needTwoBalancedHandsForNT = checked;
}

void CDealOptionsPropSheet::on_fourAcesNeededForSlam_clicked()
{
    dealOptionDoc.acesNeededForSlam = FOUR_ACES_NEEDED_FOR_SLAM;
}

void CDealOptionsPropSheet::on_threeAcesNeededForSlam_clicked()
{
    dealOptionDoc.acesNeededForSlam = THREE_ACES_NEEDED_FOR_SLAM;
}

void CDealOptionsPropSheet::on_fourAcesNeededForSmallSlam_clicked()
{
    dealOptionDoc.acesNeededForSmallSlam = FOUR_ACES_NEEDED_FOR_SMALL_SLAM;
}

void CDealOptionsPropSheet::on_threeAcesNeededForSmallSlam_clicked()
{
    dealOptionDoc.acesNeededForSmallSlam = THREE_ACES_NEEDED_FOR_SMALL_SLAM;
}

void CDealOptionsPropSheet::on_fourKingsNeededForSlam_clicked()
{
    dealOptionDoc.kingsNeededForSlam = FOUR_KINGS_NEEDED_FOR_SLAM;
}

void CDealOptionsPropSheet::on_threeKingsNeededForSlam_clicked()
{
    dealOptionDoc.kingsNeededForSlam = THREE_KINGS_NEEDED_FOR_SLAM;
}

void CDealOptionsPropSheet::on_twoKingsNeededForSlam_clicked()
{
    dealOptionDoc.kingsNeededForSlam = TWO_KINGS_NEEDED_FOR_SLAM;
}

void CDealOptionsPropSheet::on_fourKingsNeededForSmallSlam_clicked()
{
    dealOptionDoc.kingsNeededForSmallSlam = FOUR_KINGS_NEEDED_FOR_SMALL_SLAM;
}

void CDealOptionsPropSheet::on_threeKingsNeededForSmallSlam_clicked()
{
    dealOptionDoc.kingsNeededForSmallSlam = THREE_KINGS_NEEDED_FOR_SMALL_SLAM;
}

void CDealOptionsPropSheet::on_twoKingsNeededForSmallSlam_clicked()
{
    dealOptionDoc.kingsNeededForSmallSlam = TWO_KINGS_NEEDED_FOR_SMALL_SLAM;
}

void CDealOptionsPropSheet::on_fourKingsNeededForGrandSlam_clicked()
{
    dealOptionDoc.kingsNeededForGrandSlam = FOUR_KINGS_NEEDED_FOR_GRAND_SLAM;
}

void CDealOptionsPropSheet::on_threeKingsNeededForGrandSlam_clicked()
{
    dealOptionDoc.kingsNeededForGrandSlam = THREE_KINGS_NEEDED_FOR_GRAND_SLAM;
}

void CDealOptionsPropSheet::on_giveSouthBestHand_clicked(bool checked)
{
    dealOptionDoc.giveSouthBestHand = checked;
}

void CDealOptionsPropSheet::on_balancedHands_clicked(bool checked)
{
    dealOptionDoc.balancedHands = checked;
}

void CDealOptionsPropSheet::on_enableDealNumbering_clicked(bool checked)
{
    dealOptionDoc.enableDealNumbering = checked;
}
