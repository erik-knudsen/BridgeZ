/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigBiddingPage.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard bidding page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard bidding page (definition).
 */

#include "cprogconfigbiddingpage.h"
#include "ui_cprogconfigbiddingpage.h"

CProgConfigBiddingPage::CProgConfigBiddingPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CProgConfigBiddingPage)
{
    ui->setupUi(this);
}

CProgConfigBiddingPage::~CProgConfigBiddingPage()
{
    delete ui;
}
