/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigIntroPage.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard intro page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard intro page (definition).
 */

#include "cprogconfigintropage.h"
#include "ui_cprogconfigintropage.h"

CProgConfigIntroPage::CProgConfigIntroPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CProgConfigIntroPage)
{
    ui->setupUi(this);
}

CProgConfigIntroPage::~CProgConfigIntroPage()
{
    delete ui;
}
