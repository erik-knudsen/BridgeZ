/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigViesSettingsPage.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard view settings page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard view settings page (definition).
 */

#include "cprogconfigviewsettingspage.h"
#include "ui_cprogconfigviewsettingspage.h"

CProgConfigViewSettingsPage::CProgConfigViewSettingsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CProgConfigViewSettingsPage)
{
    ui->setupUi(this);
}

CProgConfigViewSettingsPage::~CProgConfigViewSettingsPage()
{
    delete ui;
}
