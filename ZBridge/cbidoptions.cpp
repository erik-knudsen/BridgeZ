/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidOptions.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Select current bid option set (conventions etc.).

  Platforms: Qt.

*/

/**
 * \file
 * Select current bid option set (conventions etc.) (definition).
 */

#include <QMessageBox>

#include "cbidoptions.h"
#include "ui_cbidoptions.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"
#include "cbidoptionspropsheet.h"
#include "cnewconvention.h"
#include "cbidoptiondoc.h"

CBidOptions::CBidOptions(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBidOptions)
{
    ui->setupUi(this);

    this->app = app;
    this->doc = doc;

    ui->northSouthConvention->setInsertPolicy(QComboBox::InsertAlphabetically);
    ui->eastWestConvention->setInsertPolicy(QComboBox::InsertAlphabetically);

    curBidOption = doc->getCurBidOption();
    bidOptions = doc->getBidOptions();
    nsIndex = ewIndex = -1;
    if (!bidOptions->empty())
    {
        int i;

        for (i = 0; i < bidOptions->size(); i++)
        {
            ui->northSouthConvention->addItem((*bidOptions)[i].name);
            ui->eastWestConvention->addItem((*bidOptions)[i].name);
        }
        nsIndex = ui->northSouthConvention->findText(curBidOption[NSBIDOPTIONSINDEX]);
        ewIndex = ui->eastWestConvention->findText(curBidOption[EWBIDOPTIONSINDEX]);
        if (nsIndex < 0) nsIndex = 0;
        if (ewIndex < 0) ewIndex = 0;

        ui->northSouthConvention->setCurrentIndex(nsIndex);
        ui->eastWestConvention->setCurrentIndex(ewIndex);
    }
    else
    {
        ui->DeleteConvention->setEnabled(false);
        ui->editConvention->setEnabled(false);
    }

    ui->northSouth->setChecked(true);
    ui->eastWestConvention->setEnabled(false);
}

CBidOptions::~CBidOptions()
{
    delete ui;
}

void CBidOptions::on_northSouth_clicked()
{
    ui->northSouthConvention->setEnabled(true);
    ui->eastWestConvention->setEnabled(false);
}

void CBidOptions::on_eastWest_clicked()
{
    ui->northSouthConvention->setEnabled(false);
    ui->eastWestConvention->setEnabled(true);
}

void CBidOptions::on_northSouthConvention_currentIndexChanged(int index)
{
    nsIndex = index;
}

void CBidOptions::on_eastWestConvention_currentIndexChanged(int index)
{
    ewIndex = index;
}

void CBidOptions::on_newConvention_clicked()
{
    int i;
    QString refConventionName;

    if ((nsIndex >= 0) && (ui->northSouth->isChecked()))
        refConventionName = ui->northSouthConvention->currentText();
    else
        if ((ewIndex >= 0) && (ui->eastWest->isChecked()))
            refConventionName = ui->eastWestConvention->currentText();

    CNewConvention newConventionDialog(refConventionName, this);
    if (newConventionDialog.exec() == QDialog::Accepted)
    {
        QString newConventionName = newConventionDialog.getNewConventionName();

        if (newConventionName.isEmpty())
        {
            QMessageBox::warning(this, tr("ZBridge"),
                  tr("Illegal name of new convention.\nThe name must not be empty."));
            return;
        }

        for (i = 0; i < bidOptions->size(); i++)
        if ((*bidOptions)[i].name == newConventionName)
        {
            QMessageBox::warning(this, tr("ZBridge"),
                  tr("Illegal name of new convention.\nThe convention already exists."));
            return;
        }
        for (i = 0; i < bidOptions->size(); i++)
            if ((*bidOptions)[i].name == refConventionName)
                break;

        CBidOptionDoc bidOption;
        bidOption = doc->getDefaultBidOption();
        if (i < bidOptions->size())
            bidOption = (*bidOptions)[i];
        CBidOptionsPropSheet biddingOptsDialog(bidOption, app, doc, this);
        if (biddingOptsDialog.exec() == QDialog::Accepted)
        {
            bidOption = biddingOptsDialog.getBidOption();
            bidOption.name = newConventionName;
            bidOptions->append(bidOption);
            ui->northSouthConvention->addItem(bidOption.name);
            ui->eastWestConvention->addItem(bidOption.name);
            ui->DeleteConvention->setEnabled(true);
            ui->editConvention->setEnabled(true);

            doc->SaveBidOptions();
        }
    }
}

void CBidOptions::on_editConvention_clicked()
{
    QString curConventionName;
    int i;

    if (ui->northSouth->isChecked())
        curConventionName = ui->northSouthConvention->currentText();
    else
        curConventionName = ui->eastWestConvention->currentText();

    for (i = 0; i < bidOptions->size(); i++)
        if ((*bidOptions)[i].name == curConventionName)
            break;

    CBidOptionsPropSheet biddingOptsDialog((*bidOptions)[i], app, doc, this);
    if (biddingOptsDialog.exec() == QDialog::Accepted)
    {
        (*bidOptions)[i] = biddingOptsDialog.getBidOption();

        doc->SaveBidOptions();
    }
}

void CBidOptions::on_DeleteConvention_clicked()
{
    int delIndex;
    QString delConventionName;

    if (ui->northSouth->isChecked())
    {
        delConventionName = ui->northSouthConvention->currentText();
        delIndex = nsIndex;
    }
    else
    {
        delConventionName = ui->eastWestConvention->currentText();
        delIndex = ewIndex;
    }

    int i;
    for (i = 0; i < bidOptions->size(); i++)
        if ((*bidOptions)[i].name == delConventionName)
            break;

    bidOptions->removeAt(i);
    ui->northSouthConvention->removeItem(delIndex);
    ui->eastWestConvention->removeItem(delIndex);
    if (bidOptions->size() == 0)
    {
        ui->DeleteConvention->setEnabled(false);
        ui->editConvention->setEnabled(false);
    }

    doc->SaveBidOptions();
}

void CBidOptions::on_buttonBox_accepted()
{
    if ((nsIndex >= 0) && (ewIndex >= 0))
    {
        curBidOption[NSBIDOPTIONSINDEX] = ui->northSouthConvention->currentText();
        curBidOption[EWBIDOPTIONSINDEX] = ui->eastWestConvention->currentText();
    }
}
