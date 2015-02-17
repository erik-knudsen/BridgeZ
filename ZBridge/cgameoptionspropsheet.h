/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CGameOptionsPropSheet.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Property sheet for determining game options.

  Platforms: Qt.

*/

/**
 * \file
 * Property sheet for determining game options (declaration).
 */

#ifndef CGAMEOPTIONSPROPSHEET_H
#define CGAMEOPTIONSPROPSHEET_H

#include <QDialog>

#include "cgameoptiondoc.h"

class CZBridgeApp;
class CZBridgeDoc;

namespace Ui {
class CGameOptionsPropSheet;
}

/**
 * @brief Game option property sheet.
 *
 * Shows the game option property sheet and allows the user to set/change the options.
 */
class CGameOptionsPropSheet : public QDialog
{
    Q_OBJECT
    
public:
    explicit CGameOptionsPropSheet(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent = 0);
    ~CGameOptionsPropSheet();

private slots:
    void on_biddingPause_sliderMoved(int position);
    void on_playPause_sliderMoved(int position);
    void on_buttonBox_accepted();
    void on_autoPlayLastCard_clicked(bool checked);
    void on_autoJumpCursor_clicked(bool checked);
    void on_insertBiddingPause_clicked(bool checked);
    void on_insertPlayPause_clicked(bool checked);
    void on_autoBidStart_clicked(bool checked);
    void on_computerCanClaim_clicked(bool checked);
    void on_enableTracing_clicked(bool checked);
    void on_showPassedHands_clicked(bool checked);
    void on_enableAnalysisDuringHints_clicked(bool checked);
    void on_traceLevel_currentIndexChanged(int index);
    void on_allowRebidPassedHands_clicked(bool checked);
    void on_gorenValuationMode_clicked();
    void on_ginsburgValuationMode_clicked();
    void on_customValuationMode_clicked();    
    void on_teamsIMP_clicked();
    void on_duplicateMP_clicked();
    void on_RubberBridge_clicked();
    void on_practice_clicked();
    void on_saveIntermediatePositions_clicked(bool checked);
    void on_exposePBNGameCards_clicked(bool checked);

private:
    void enableValues(bool enable);

    Ui::CGameOptionsPropSheet *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
    CGameOptionDoc gameOptionDoc;
};

#endif // CGAMEOPTIONSPROPSHEET_H
