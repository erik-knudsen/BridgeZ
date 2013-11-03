/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigSuitsDisplayPage.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard suits display page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard suits display page (definition).
 */

#include "cprogconfigsuitsdisplaypage.h"
#include "ui_cprogconfigsuitsdisplaypage.h"

CProgConfigSuitsDisplayPage::CProgConfigSuitsDisplayPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CProgConfigSuitsDisplayPage)
{
    ui->setupUi(this);
}

CProgConfigSuitsDisplayPage::~CProgConfigSuitsDisplayPage()
{
    delete ui;
}
