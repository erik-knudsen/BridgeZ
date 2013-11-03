/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigHelpLevelPage.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard help level page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard help level page (definition).
 */

#include "cprogconfighelplevelpage.h"
#include "ui_cprogconfighelplevelpage.h"

CProgConfigHelpLevelPage::CProgConfigHelpLevelPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CProgConfigHelpLevelPage)
{
    ui->setupUi(this);
}

CProgConfigHelpLevelPage::~CProgConfigHelpLevelPage()
{
    delete ui;
}
