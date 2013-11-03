/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CDealOptionsPropSheet.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Property sheet for determining deal options.

  Platforms: Qt.

*/

/**
 * \file
 * Property sheet for determining deal options (declaration).
 */

#ifndef CDEALOPTIONSPROPSHEET_H
#define CDEALOPTIONSPROPSHEET_H

#include <QDialog>

#include "cdealoptiondoc.h"

class CZBridgeApp;
class CZBridgeDoc;

namespace Ui {
class CDealOptionsPropSheet;
}

class CDealOptionsPropSheet : public QDialog
{
    Q_OBJECT
    
public:
    explicit CDealOptionsPropSheet(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent = 0);
    ~CDealOptionsPropSheet();

private slots:
    void on_buttonBox_accepted();
    void on_minGamePts_valueChanged(int arg1);
    void on_maxGamePts_valueChanged(int arg1);
    void on_minMajorGamePts_valueChanged(int arg1);
    void on_maxMajorGamePts_valueChanged(int arg1);
    void on_minMinorGamePts_valueChanged(int arg1);
    void on_maxMinorGamePts_valueChanged(int arg1);
    void on_minNoTrumpGamePts_valueChanged(int arg1);
    void on_maxNoTrumpGamePts_valueChanged(int arg1);
    void on_minSlamPts_valueChanged(int arg1);
    void on_maxSlamPts_valueChanged(int arg1);
    void on_minSmallSlamPts_valueChanged(int arg1);
    void on_maxSmallSlamPts_valueChanged(int arg1);
    void on_minGrandSlamPts_valueChanged(int arg1);
    void on_maxGrandSlamPts_valueChanged(int arg1);
    void on_sevenMinCardsInMinor_clicked();
    void on_eightMinCardsInMinor_clicked();
    void on_nineMinCardsInMinor_clicked();
    void on_aceMinTopMinorCard_clicked();
    void on_kingMinTopMinorCard_clicked();
    void on_queenMinTopMinorCard_clicked();
    void on_fourThreeMinSuitDistMinor_clicked();
    void on_fourFourMinSuitDistMinor_clicked();
    void on_fiveThreeMinSuitDistMinor_clicked();
    void on_fiveFourMinSuitDistMinor_clicked();
    void on_sevenMinCardsInMajor_clicked();
    void on_eightMinCardsInMajor_clicked();
    void on_nineMinCardsInMajor_clicked();
    void on_aceMinTopMajorCard_clicked();
    void on_kingMinTopMajorCard_clicked();
    void on_queenMinTopMajorCard_clicked();
    void on_fourThreeMinSuitDistMajor_clicked();
    void on_fourFourMinSuitDistMajor_clicked();
    void on_fiveThreeMinSuitDistMajor_clicked();
    void on_fiveFourMinSuitDistMajor_clicked();
    void on_needTwoBalancedHandsForNT_clicked(bool checked);
    void on_fourAcesNeededForSlam_clicked();
    void on_threeAcesNeededForSlam_clicked();
    void on_fourAcesNeededForSmallSlam_clicked();
    void on_threeAcesNeededForSmallSlam_clicked();
    void on_fourKingsNeededForSlam_clicked();
    void on_threeKingsNeededForSlam_clicked();
    void on_twoKingsNeededForSlam_clicked();
    void on_fourKingsNeededForSmallSlam_clicked();
    void on_threeKingsNeededForSmallSlam_clicked();
    void on_twoKingsNeededForSmallSlam_clicked();
    void on_fourKingsNeededForGrandSlam_clicked();
    void on_threeKingsNeededForGrandSlam_clicked();
    void on_giveSouthBestHand_clicked(bool checked);
    void on_balancedHands_clicked(bool checked);
    void on_enableDealNumbering_clicked(bool checked);

private:
    Ui::CDealOptionsPropSheet *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
    CDealOptionDoc dealOptionDoc;
};

#endif // CDEALOPTIONSPROPSHEET_H
