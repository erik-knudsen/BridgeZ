/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigPausesPage.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard pauses page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard pauses page (definition).
 */

#include <QString>
#include <QTextStream>

#include "cprogconfigpausespage.h"
#include "ui_cprogconfigpausespage.h"

CProgConfigPausesPage::CProgConfigPausesPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CProgConfigPausesPage)
{
    ui->setupUi(this);
}

CProgConfigPausesPage::~CProgConfigPausesPage()
{
    delete ui;
}

void CProgConfigPausesPage::on_biddingPause_sliderMoved(int position)
{
    QString text;
    QTextStream(&text) << position/10.0;
    ui->biddingPauseLength->setText(text);
}

void CProgConfigPausesPage::on_playPause_sliderMoved(int position)
{
    QString text;
    QTextStream(&text) << position/10.0;
    ui->playPauseLength->setText(text);
}
