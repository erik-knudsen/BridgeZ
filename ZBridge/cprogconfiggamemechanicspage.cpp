/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigGameMechanics.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard game mechanics page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard game mechanics page (definition).
 */

#include "cprogconfiggamemechanicspage.h"
#include "ui_cprogconfiggamemechanicspage.h"

CProgConfigGameMechanicsPage::CProgConfigGameMechanicsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CProgConfigGameMechanicsPage)
{
    ui->setupUi(this);
}

CProgConfigGameMechanicsPage::~CProgConfigGameMechanicsPage()
{
    delete ui;
}
