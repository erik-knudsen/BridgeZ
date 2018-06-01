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
    void on_insertBiddingPause_clicked(bool checked);
    void on_insertPlayPause_clicked(bool checked);
    void on_teamsIMP_clicked();
    void on_duplicateMP_clicked();
    void on_RubberBridge_clicked();
    void on_practice_clicked();
    void on_level0_clicked();
    void on_level1_clicked();
    void on_level2_clicked();
    void on_level3_clicked();

private:
    Ui::CGameOptionsPropSheet *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
    CGameOptionDoc gameOptionDoc;
};

#endif // CGAMEOPTIONSPROPSHEET_H
