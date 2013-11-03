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

    //Suits.
    if (displayOptionDoc.suitSequence == SHCD_SUIT_SEQUENCE)
        ui->shcdSuitSequence->setChecked(true);
    else if (displayOptionDoc.suitSequence == SHDC_SUIT_SEQUENCE)
        ui->shdcSuitSequence->setChecked(true);
    else if (displayOptionDoc.suitSequence == DCHS_SUIT_SEQUENCE)
        ui->dchsSuitSequence->setChecked(true);
    else if (displayOptionDoc.suitSequence == CDHS_SUIT_SEQUENCE)
        ui->cdhsSuitSequence->setChecked(true);
    else
        ui->hsdcSuitSequence->setChecked(true);
    ui->showDummyTrumpsOnLeft->setChecked(displayOptionDoc.showDummyTrumpsOnLeft);

    //Cards.
    ui->tripleBuffer->setChecked(displayOptionDoc.tripleBuffer);
    ui->animateCards->setChecked(displayOptionDoc.animateCards);
    ui->granularity->setValue(displayOptionDoc.granularity);

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

    //Dialogs.
    ui->autoShowBidHistory->setChecked(displayOptionDoc.autoShowBidHistory);
    ui->autoHideBidHistory->setChecked(displayOptionDoc.autoHideBidHistory);
    ui->autoShowPlayHistory->setChecked(displayOptionDoc.autoShowPlayHistory);
    ui->autoHidePlayHistory->setChecked(displayOptionDoc.autoHidePlayHistory);

    ui->showLayoutWhenEditing->setChecked(displayOptionDoc.showLayoutWhenEditing);

    //Fonts.
    QFont font;
    QString fontName;
    if (displayOptionDoc.autoHintDialogFont.size() == 0)
        ui->autoHintDialogFont->setText(tr("System Default"));
    else
    {
        font.fromString(displayOptionDoc.autoHintDialogFont);
        QTextStream(&fontName) << font.family() << "  " << font.pointSize();
        ui->autoHintDialogFont->setText(fontName);
    }

    if (displayOptionDoc.bidAndPlayHistoryDialogFont.size() == 0)
        ui->bidAndPlayHistoryDialogFont->setText(tr("System Default"));
    else
    {
        font.fromString(displayOptionDoc.bidAndPlayHistoryDialogFont);
        fontName.clear();
        QTextStream(&fontName) << font.family() << "  " << font.pointSize();
        ui->bidAndPlayHistoryDialogFont->setText(fontName);
    }

    if (displayOptionDoc.analysisDialogFont.size() == 0)
        ui->analysisDialogFont->setText(tr("System Default"));
    else
    {
        font.fromString(displayOptionDoc.analysisDialogFont);
        fontName.clear();
        QTextStream(&fontName) << font.family() << "  " << font.pointSize();
        ui->analysisDialogFont->setText(fontName);
    }

    ui->useSuitSymbols->setChecked(displayOptionDoc.useSuitSymbols);

    //Misc.
    ui->lowResOption->setChecked(displayOptionDoc.lowResolutionOption);
    ui->showStarupBanner->setChecked(displayOptionDoc.showStartupBanner);
    ui->showBackgroundBitmap->setChecked(displayOptionDoc.showBackgroundBitmap);

    if (displayOptionDoc.bitmapMode == TILE_BITMAP_MODE)
        ui->tileBitmapMode->setChecked(true);
    else
        ui->centerBitmapMode->setChecked(true);

    ui->scaleLargeBitmaps->setChecked(displayOptionDoc.scaleLargeBitmaps);
    ui->filePath->setText(displayOptionDoc.filePath);

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

void CDispOptionsPropSheet::on_shcdSuitSequence_clicked()
{
    displayOptionDoc.suitSequence = SHCD_SUIT_SEQUENCE;
}

void CDispOptionsPropSheet::on_shdcSuitSequence_clicked()
{
    displayOptionDoc.suitSequence = SHDC_SUIT_SEQUENCE;
}

void CDispOptionsPropSheet::on_dchsSuitSequence_clicked()
{
    displayOptionDoc.suitSequence = DCHS_SUIT_SEQUENCE;
}

void CDispOptionsPropSheet::on_cdhsSuitSequence_clicked()
{
    displayOptionDoc.suitSequence = CDHS_SUIT_SEQUENCE;
}

void CDispOptionsPropSheet::on_hsdcSuitSequence_clicked()
{
    displayOptionDoc.suitSequence = HSDC_SUIT_SEQUENCE;
}

void CDispOptionsPropSheet::on_showDummyTrumpsOnLeft_clicked(bool checked)
{
    displayOptionDoc.showDummyTrumpsOnLeft = checked;
}

void CDispOptionsPropSheet::on_tripleBuffer_clicked(bool checked)
{
    displayOptionDoc.tripleBuffer = checked;
}

void CDispOptionsPropSheet::on_animateCards_clicked(bool checked)
{
    displayOptionDoc.animateCards = checked;
}

void CDispOptionsPropSheet::on_granularity_valueChanged(int value)
{
    displayOptionDoc.granularity = value;
}

void CDispOptionsPropSheet::on_autoShowBidHistory_clicked(bool checked)
{
    displayOptionDoc.autoShowBidHistory = checked;
}

void CDispOptionsPropSheet::on_autoHideBidHistory_clicked(bool checked)
{
    displayOptionDoc.autoHideBidHistory = checked;
}

void CDispOptionsPropSheet::on_autoShowPlayHistory_clicked(bool checked)
{
    displayOptionDoc.autoShowPlayHistory = checked;
}

void CDispOptionsPropSheet::on_autoHidePlayHistory_clicked(bool checked)
{
    displayOptionDoc.autoHidePlayHistory = checked;
}

void CDispOptionsPropSheet::on_showLayoutWhenEditing_clicked(bool checked)
{
    displayOptionDoc.showLayoutWhenEditing = checked;
}

void CDispOptionsPropSheet::on_setAutoHintDialogFont_clicked()
{
    QFontDialog autoHintDialogFont;
    if (autoHintDialogFont.exec() == QDialog::Accepted)
    {
        QFont font = autoHintDialogFont.selectedFont();
        displayOptionDoc.autoHintDialogFont =font.toString();
        QString fontName;
        QTextStream(&fontName) << font.family() << "  " << font.pointSize();
        ui->autoHintDialogFont->setText(fontName);
    }
}

void CDispOptionsPropSheet::on_setHistoryDialogFont_clicked()
{
    QFontDialog historyDialogFont;
    if (historyDialogFont.exec() == QDialog::Accepted)
    {
        QFont font = historyDialogFont.selectedFont();
        displayOptionDoc.bidAndPlayHistoryDialogFont =font.toString();
        QString fontName;
        QTextStream(&fontName) << font.family() << "  " << font.pointSize();
        ui->bidAndPlayHistoryDialogFont->setText(fontName);
    }
}

void CDispOptionsPropSheet::on_setAnalysisDialogFont_clicked()
{
    QFontDialog analysisDialogFont;
    if (analysisDialogFont.exec() == QDialog::Accepted)
    {
        QFont font = analysisDialogFont.selectedFont();
        displayOptionDoc.analysisDialogFont = font.toString();
        QString fontName;
        QTextStream(&fontName) << font.family() << "  " << font.pointSize();
        ui->analysisDialogFont->setText(fontName);
    }
}

void CDispOptionsPropSheet::on_resetAutoHintDialogFont_clicked()
{
    displayOptionDoc.autoHintDialogFont = "";
    ui->autoHintDialogFont->setText(tr("System Default"));
}

void CDispOptionsPropSheet::on_resetHistoryDialogFont_clicked()
{
    displayOptionDoc.bidAndPlayHistoryDialogFont = "";
    ui->bidAndPlayHistoryDialogFont->setText(tr("System Default"));
}

void CDispOptionsPropSheet::on_resetAnalysisDialogFont_clicked()
{
    displayOptionDoc.analysisDialogFont = "";
    ui->analysisDialogFont->setText(tr("System Default"));
}

void CDispOptionsPropSheet::on_useSuitSymbols_clicked(bool checked)
{
    displayOptionDoc.useSuitSymbols = checked;
}

void CDispOptionsPropSheet::on_lowResOption_clicked(bool checked)
{
    displayOptionDoc.lowResolutionOption = checked;
}

void CDispOptionsPropSheet::on_showStarupBanner_clicked(bool checked)
{
    displayOptionDoc.showStartupBanner = checked;
}

void CDispOptionsPropSheet::on_showBackgroundBitmap_clicked(bool checked)
{
    displayOptionDoc.showBackgroundBitmap = checked;
}

void CDispOptionsPropSheet::on_tileBitmapMode_clicked()
{
    displayOptionDoc.bitmapMode = TILE_BITMAP_MODE;
}

void CDispOptionsPropSheet::on_centerBitmapMode_clicked()
{
    displayOptionDoc.bitmapMode = CENTER_BITMAP_MODE;
}

void CDispOptionsPropSheet::on_scaleLargeBitmaps_clicked(bool checked)
{
    displayOptionDoc.scaleLargeBitmaps = checked;
}

void CDispOptionsPropSheet::on_browse_clicked()
{
    QFileDialog browse;
    if (browse.exec() == QDialog::Accepted)
    {
        QStringList list = browse.selectedFiles();
        displayOptionDoc.filePath = list.first();
    }
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
