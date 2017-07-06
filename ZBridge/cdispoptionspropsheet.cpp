/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CDisplayOptionsPropSheet.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Property sheet for determining display options.

  Platforms: Qt.

*/

/**
 * \file
 * Property sheet for determining display options (definition).
 */

#include <QFontDialog>
#include <QFileDialog>
#include <QColorDialog>
#include <QPalette>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "QTextStream"

#include "cdispoptionspropsheet.h"
#include "ui_cdispoptionspropsheet.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"

CDispOptionsPropSheet::CDispOptionsPropSheet(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDispOptionsPropSheet)
{
    ui->setupUi(this);
    this->app = app;
    this->doc = doc;

    displayOptionDoc = doc->getDisplayOptions();

    //Card Backs.
    if (displayOptionDoc.cardBack == CARD_BACK_0)
        ui->cardBack0->setChecked(true);
    else if (displayOptionDoc.cardBack == CARD_BACK_1)
        ui->cardBack1->setChecked(true);
    else if (displayOptionDoc.cardBack == CARD_BACK_2)
        ui->cardBack2->setChecked(true);
    else if (displayOptionDoc.cardBack == CARD_BACK_3)
        ui->cardBack3->setChecked(true);
    else if (displayOptionDoc.cardBack == CARD_BACK_4)
        ui->cardBack4->setChecked(true);
    else
        ui->cardBack5->setChecked(true);

    QColor backgroundColor(displayOptionDoc.backgroundRed, displayOptionDoc.backgroundGreen, displayOptionDoc.backgroundBlue);
    ui->colorWatch->setAutoFillBackground(true);
    QPalette pal = ui->colorWatch->palette();
    pal.setColor(QPalette::Window, backgroundColor);
    ui->colorWatch->setPalette(pal);
}

CDispOptionsPropSheet::~CDispOptionsPropSheet()
{
    delete ui;
}

void CDispOptionsPropSheet::on_buttonBox_accepted()
{
    doc->setDisplayOptions(displayOptionDoc);
}

void CDispOptionsPropSheet::on_backgroundColor_clicked()
{
    QColorDialog backgroundColor;
    if (backgroundColor.exec() == QDialog::Accepted)
    {
        QColor color = backgroundColor.currentColor();
        displayOptionDoc.backgroundRed = color.red();
        displayOptionDoc.backgroundGreen = color.green();
        displayOptionDoc.backgroundBlue = color.blue();

        QPalette pal = ui->colorWatch->palette();
        pal.setColor(QPalette::Window, color);
        ui->colorWatch->setPalette(pal);
    }
}

void CDispOptionsPropSheet::on_cardBack0_clicked()
{
    CheckCardBack(CARD_BACK_0);
    displayOptionDoc.cardBack = CARD_BACK_0;
}

void CDispOptionsPropSheet::on_cardBack1_clicked()
{
    CheckCardBack(CARD_BACK_1);
    displayOptionDoc.cardBack = CARD_BACK_1;
}

void CDispOptionsPropSheet::on_cardBack2_clicked()
{
    CheckCardBack(CARD_BACK_2);
    displayOptionDoc.cardBack = CARD_BACK_2;
}

void CDispOptionsPropSheet::on_cardBack3_clicked()
{
    CheckCardBack(CARD_BACK_3);
    displayOptionDoc.cardBack = CARD_BACK_3;
}

void CDispOptionsPropSheet::on_cardBack4_clicked()
{
    CheckCardBack(CARD_BACK_4);
    displayOptionDoc.cardBack = CARD_BACK_4;
}

void CDispOptionsPropSheet::on_cardBack5_clicked()
{
    CheckCardBack(CARD_BACK_5);
    displayOptionDoc.cardBack = CARD_BACK_5;
}

void CDispOptionsPropSheet::CheckCardBack(int newCardBack)
{
    ui->cardBack0->setChecked(CARD_BACK_0 == newCardBack);
    ui->cardBack1->setChecked(CARD_BACK_1 == newCardBack);
    ui->cardBack2->setChecked(CARD_BACK_2 == newCardBack);
    ui->cardBack3->setChecked(CARD_BACK_3 == newCardBack);
    ui->cardBack4->setChecked(CARD_BACK_4 == newCardBack);
    ui->cardBack5->setChecked(CARD_BACK_5 == newCardBack);
}
