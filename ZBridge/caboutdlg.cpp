/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CAboutDlg.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: About dialog.

  Platforms: Qt.

*/

/**
 * @file
 * About dialog for ZBridge (definition).
 */

#include "caboutdlg.h"
#include "ui_caboutdlg.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"

/**
 * @brief CAboutDlg::CAboutDlg
 *
 * The constructor initializes the About Dialog and shows relevant
 * information in the dialog.
 *
 * @param app Application pointer.
 * @param doc Document pointer.
 * @param parent Parent.
 */
CAboutDlg::CAboutDlg(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAboutDlg)
{
    ui->setupUi(this);
    this->app = app;
    this->doc = doc;
}

CAboutDlg::~CAboutDlg()
{
    delete ui;
}
