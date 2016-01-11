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

/**
 * @brief Deal option property sheet.
 *
 * This class shows the deal option property sheet and allows the user to set/change the deal options.
 */
class CDealOptionsPropSheet : public QDialog
{
    Q_OBJECT
    
public:
    explicit CDealOptionsPropSheet(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent = 0);
    ~CDealOptionsPropSheet();

private slots:    
    void on_ok_clicked();
    void on_cancel_clicked();

    void on_hcpTotalWest_editingFinished();
    void on_hcpTotalNorth_editingFinished();
    void on_hcpTotalEast_editingFinished();
    void on_hcpTotalSouth_editingFinished();
    void on_hcpSpadeWest_editingFinished();
    void on_hcpSpadeNorth_editingFinished();
    void on_hcpSpadeEast_editingFinished();
    void on_hcpSpadeSouth_editingFinished();
    void on_hcpHeartWest_editingFinished();
    void on_hcpHeartNorth_editingFinished();
    void on_hcpHeartEast_editingFinished();
    void on_hcpHeartSouth_editingFinished();
    void on_hcpDiamondWest_editingFinished();
    void on_hcpDiamondNorth_editingFinished();
    void on_hcpDiamondEast_editingFinished();
    void on_hcpDiamondSouth_editingFinished();
    void on_hcpClubWest_editingFinished();
    void on_hcpClubNorth_editingFinished();
    void on_hcpClubEast_editingFinished();
    void on_hcpClubSouth_editingFinished();

    void on_suitSizeSpadeWest_editingFinished();
    void on_suitSizeSpadeNorth_editingFinished();
    void on_suitSizeSpadeEast_editingFinished();
    void on_suitSizeSpadeSouth_editingFinished();
    void on_suitSizeHeartWest_editingFinished();
    void on_suitSizeHeartNorth_editingFinished();
    void on_suitSizeHeartEast_editingFinished();
    void on_suitSizeHeartSouth_editingFinished();
    void on_suitSizeDiamondWest_editingFinished();
    void on_suitSizeDiamondNorth_editingFinished();
    void on_suitSizeDiamondEast_editingFinished();
    void on_suitSizeDiamondSouth_editingFinished();
    void on_suitSizeClubWest_editingFinished();
    void on_suitSizeClubNorth_editingFinished();
    void on_suitSizeClubEast_editingFinished();
    void on_suitSizeClubSouth_editingFinished();

    void on_dpWest_editingFinished();
    void on_dpNorth_editingFinished();
    void on_dpEast_editingFinished();
    void on_dpSouth_editingFinished();

private:
    bool getValues(QString &text, int max, int *low, int *high);

    Ui::CDealOptionsPropSheet *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
    CDealOptionDoc dealOptionDoc;
};

#endif // CDEALOPTIONSPROPSHEET_H
