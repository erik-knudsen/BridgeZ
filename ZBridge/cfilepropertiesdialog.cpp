/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CFileProperties.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Play file properties.

  Platforms: Qt.

*/

/**
 * \file
 * Play file properties (definition).
 */

#include "cfilepropertiesdialog.h"
#include "ui_cfilepropertiesdialog.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"

CFilePropertiesDialog::CFilePropertiesDialog(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFilePropertiesDialog)
{
    ui->setupUi(this);

    this->app = app;
    this->doc = doc;
}

CFilePropertiesDialog::~CFilePropertiesDialog()
{
    delete ui;
}
