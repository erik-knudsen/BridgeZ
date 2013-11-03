/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigFinishPage.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard finish page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard finish page (definition).
 */

#include "cprogconfigfinishpage.h"
#include "ui_cprogconfigfinishpage.h"

CProgConfigFinishPage::CProgConfigFinishPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CProgConfigFinishPage)
{
    ui->setupUi(this);
}

CProgConfigFinishPage::~CProgConfigFinishPage()
{
    delete ui;
}
