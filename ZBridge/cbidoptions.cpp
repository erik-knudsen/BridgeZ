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

/**
 * @brief The constructor sets up the user interface.
 *
 *   - Finds available bid option sets.
 *   - Determines current bid option sets for North/South and East/Weat.
 */
CBidOptions::CBidOptions(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBidOptions)
{
    //Set up user interface.
    ui->setupUi(this);

    this->app = app;
    this->doc = doc;

    ui->northSouthConvention->setInsertPolicy(QComboBox::InsertAlphabetically);
    ui->eastWestConvention->setInsertPolicy(QComboBox::InsertAlphabetically);

    //Find available bid option sets.
    curBidOption = doc->getCurBidOption();
    bidOptions = doc->getBidOptions();
    nsIndex = ewIndex = -1;
    if (!bidOptions->empty())
    {
        int i;

        //Add bid option sets to drop down.
        for (i = 0; i < bidOptions->size(); i++)
        {
            ui->northSouthConvention->addItem((*bidOptions)[i].name);
            ui->eastWestConvention->addItem((*bidOptions)[i].name);
        }
        nsIndex = ui->northSouthConvention->findText(curBidOption[NSBIDOPTIONSINDEX]);
        ewIndex = ui->eastWestConvention->findText(curBidOption[EWBIDOPTIONSINDEX]);
        if (nsIndex < 0) nsIndex = 0;
        if (ewIndex < 0) ewIndex = 0;

        //Set current bid option sets.
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

/**
 * @brief Select North/South.
 */
void CBidOptions::on_northSouth_clicked()
{
    ui->northSouthConvention->setEnabled(true);
    ui->eastWestConvention->setEnabled(false);
}

/**
 * @brief Select East/West.
 */
void CBidOptions::on_eastWest_clicked()
{
    ui->northSouthConvention->setEnabled(false);
    ui->eastWestConvention->setEnabled(true);
}

/**
 * @brief Select new bid option set for North/South
 */
void CBidOptions::on_northSouthConvention_currentIndexChanged(int index)
{
    nsIndex = index;
}

/**
 * @brief Select new bid option set for East/West.
 */
void CBidOptions::on_eastWestConvention_currentIndexChanged(int index)
{
    ewIndex = index;
}

/**
 * @brief Create a new bid option set.
 */
void CBidOptions::on_newConvention_clicked()
{
    int i;
    QString refConventionName;

    //Determine reference bid option set.
    if ((nsIndex >= 0) && (ui->northSouth->isChecked()))
        refConventionName = ui->northSouthConvention->currentText();
    else
        if ((ewIndex >= 0) && (ui->eastWest->isChecked()))
            refConventionName = ui->eastWestConvention->currentText();

    //Activate new convention dialog (to determine name of new bid option set).
    CNewConvention newConventionDialog(refConventionName, this);
    if (newConventionDialog.exec() == QDialog::Accepted)
    {
        //Name is accepted.
        QString newConventionName = newConventionDialog.getNewConventionName();

        //Check name of new bid option set.
        if (newConventionName.isEmpty())
        {
            QMessageBox::warning(this, tr("ZBridge"),
                  tr("Illegal name of new bid option set.\nThe name must not be empty."));
            return;
        }

        for (i = 0; i < bidOptions->size(); i++)
        if ((*bidOptions)[i].name == newConventionName)
        {
            QMessageBox::warning(this, tr("ZBridge"),
                  tr("Illegal name of new bid option set.\nThe bid option set already exists."));
            return;
        }
        for (i = 0; i < bidOptions->size(); i++)
            if ((*bidOptions)[i].name == refConventionName)
                break;

        //Name is ok. Activate bid options property sheet to determine bid options for the new
        //bid option set.
        CBidOptionDoc bidOption;
        bidOption = doc->getDefaultBidOption();
        if (i < bidOptions->size())
            bidOption = (*bidOptions)[i];
        CBidOptionsPropSheet biddingOptsDialog(bidOption, app, doc, this);
        if (biddingOptsDialog.exec() == QDialog::Accepted)
        {
            //The new bid option set was accepted. Add it to available bid option sets.
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

/**
 * @brief Edit the current bid option set.
 */
void CBidOptions::on_editConvention_clicked()
{
    QString curConventionName;
    int i;

    //Determine which bid option set to edit.
    if (ui->northSouth->isChecked())
        curConventionName = ui->northSouthConvention->currentText();
    else
        curConventionName = ui->eastWestConvention->currentText();

    for (i = 0; i < bidOptions->size(); i++)
        if ((*bidOptions)[i].name == curConventionName)
            break;

    //Activate bid option property sheet to edit the current bid option set.
    CBidOptionsPropSheet biddingOptsDialog((*bidOptions)[i], app, doc, this);
    if (biddingOptsDialog.exec() == QDialog::Accepted)
    {
        //Editing was accepted. Save the changes.
        (*bidOptions)[i] = biddingOptsDialog.getBidOption();

        doc->SaveBidOptions();
    }
}

/**
 * @brief Delete the current bid option set.
 */
void CBidOptions::on_DeleteConvention_clicked()
{
    int delIndex;
    QString delConventionName;

    //Determine which bid option set to delete.
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

    //Delete the bid option set.
    bidOptions->removeAt(i);
    ui->northSouthConvention->removeItem(delIndex);
    ui->eastWestConvention->removeItem(delIndex);
    if (bidOptions->size() == 0)
    {
        ui->DeleteConvention->setEnabled(false);
        ui->editConvention->setEnabled(false);
    }

    //Save the changes.
    doc->SaveBidOptions();
}

/**
 * @brief Ok was clicked.
 */
void CBidOptions::on_buttonBox_accepted()
{
    if ((nsIndex >= 0) && (ewIndex >= 0))
    {
        //Save current North/South and East/West bid option set choices.
        curBidOption[NSBIDOPTIONSINDEX] = ui->northSouthConvention->currentText();
        curBidOption[EWBIDOPTIONSINDEX] = ui->eastWestConvention->currentText();
    }
}
