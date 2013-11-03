/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CAboutDlg.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: About dialog.

  Platforms: Qt.

*/

/**
 * @file About dialog (declaration).
 * About dialog (declaration).
 */

#ifndef CABOUTDLG_H
#define CABOUTDLG_H

#include <QDialog>

class CZBridgeApp;
class CZBridgeDoc;

namespace Ui {
class CAboutDlg;
}

/**
 * @brief About dialog
 *
 * Version, copyright etc. are shown in the dialog.
 */
class CAboutDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CAboutDlg(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent = 0);
    ~CAboutDlg();
    
private:
    Ui::CAboutDlg *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
};

#endif // CABOUTDLG_H
