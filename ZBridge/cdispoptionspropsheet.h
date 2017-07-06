/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CDisplayOptionsPropSheet.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Property sheet for determining display options.

  Platforms: Qt.

*/

/**
 * \file
 * Property sheet for determining display options (declaration).
 */

#ifndef CDISPOPTIONSPROPSHEET_H
#define CDISPOPTIONSPROPSHEET_H

#include <QDialog>

#include "cdisplayoptiondoc.h"

class CZBridgeApp;
class CZBridgeDoc;

namespace Ui {
class CDispOptionsPropSheet;
}

/**
 * @brief Display option property sheet.
 *
 * This class allows the user to set/change the display options.
 */
class CDispOptionsPropSheet : public QDialog
{
    Q_OBJECT
    
public:
    explicit CDispOptionsPropSheet(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent = 0);
    ~CDispOptionsPropSheet();

private slots:
    void on_backgroundColor_clicked();
    void on_buttonBox_accepted();
    void on_cardBack0_clicked();
    void on_cardBack1_clicked();
    void on_cardBack2_clicked();
    void on_cardBack3_clicked();
    void on_cardBack4_clicked();
    void on_cardBack5_clicked();

private:
    void CheckCardBack(int newCardBack);
    Ui::CDispOptionsPropSheet *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
    CDisplayOptionDoc displayOptionDoc;
};

#endif // CDISPOPTIONSPROPSHEET_H
