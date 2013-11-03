/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CDisplayOptionDoc.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Display options (data).

  Platforms: Qt.

*/

/**
 * \file
 * Display options (data) (definition).
 */

#include <QSettings>

#include "cdisplayoptiondoc.h"
#include "Defines.h"

CDisplayOptionDoc::CDisplayOptionDoc()
{
    suitSequence = SHDC_SUIT_SEQUENCE;
    showDummyTrumpsOnLeft = true;

    tripleBuffer = false;
    animateCards = true;
    granularity = DEFAULT_ANIMATION_GRANULARITY;

    cardBack = 0;

    autoShowBidHistory = true;
    autoHideBidHistory = true;
    autoShowPlayHistory = false;
    autoHidePlayHistory = true;
    showLayoutWhenEditing = true;

    autoHintDialogFont = "";
    bidAndPlayHistoryDialogFont = "";
    analysisDialogFont = "";
    useSuitSymbols = true;

    lowResolutionOption = false;
    showStartupBanner = true;
    showBackgroundBitmap = true;
    bitmapMode = CENTER_BITMAP_MODE;
    scaleLargeBitmaps = true;
    filePath = "";
    backgroundRed = backgroundBlue = backgroundGreen = 0;
}

// Assignment operator
CDisplayOptionDoc& CDisplayOptionDoc::operator=(const CDisplayOptionDoc& displayOptionDoc)
{
    // check for self-assignment
    if (this == &displayOptionDoc)
        return *this;

    suitSequence = displayOptionDoc.suitSequence;
    showDummyTrumpsOnLeft =displayOptionDoc.showDummyTrumpsOnLeft ;

    tripleBuffer = displayOptionDoc.tripleBuffer;
    animateCards = displayOptionDoc.animateCards;
    granularity = displayOptionDoc.granularity;

    cardBack = displayOptionDoc.cardBack;

    autoShowBidHistory = displayOptionDoc.autoShowBidHistory;
    autoHideBidHistory = displayOptionDoc.autoHideBidHistory;
    autoShowPlayHistory = displayOptionDoc.autoShowPlayHistory;
    autoHidePlayHistory = displayOptionDoc.autoHidePlayHistory;
    showLayoutWhenEditing = displayOptionDoc.showLayoutWhenEditing;

    autoHintDialogFont = displayOptionDoc.autoHintDialogFont;
    bidAndPlayHistoryDialogFont = displayOptionDoc.bidAndPlayHistoryDialogFont;
    analysisDialogFont = displayOptionDoc.analysisDialogFont;
    useSuitSymbols = displayOptionDoc.useSuitSymbols;

    lowResolutionOption = displayOptionDoc.lowResolutionOption;
    showStartupBanner = displayOptionDoc.showStartupBanner;
    showBackgroundBitmap = displayOptionDoc.showBackgroundBitmap;
    bitmapMode = displayOptionDoc.bitmapMode;
    scaleLargeBitmaps = displayOptionDoc.scaleLargeBitmaps;
    filePath = displayOptionDoc.filePath;
    backgroundRed = displayOptionDoc.backgroundRed;
    backgroundGreen = displayOptionDoc.backgroundGreen;
    backgroundBlue = displayOptionDoc.backgroundBlue;

    return *this;
}

void CDisplayOptionDoc::WriteSettings()
{
    QSettings settings("ZBridge settings", "Display");

    settings.setValue("suitSequence", suitSequence);
    settings.setValue("showDummyTrumpsOnLeft", showDummyTrumpsOnLeft);

    settings.setValue("tripleBuffer", tripleBuffer);
    settings.setValue("animateCards", animateCards);
    settings.setValue("granularity", granularity);

    settings.setValue("cardBack", cardBack);

    settings.setValue("autoShowBidHistory", autoShowBidHistory);
    settings.setValue("autoHideBidHistory", autoHideBidHistory);
    settings.setValue("autoShowPlayHistory", autoShowPlayHistory);
    settings.setValue("autoHidePlayHistory", autoHidePlayHistory);
    settings.setValue("showLayoutWhenEditing", showLayoutWhenEditing);

    settings.setValue("autoHintDialogFont", autoHintDialogFont);
    settings.setValue("bidAndPlayHistoryDialogFont", bidAndPlayHistoryDialogFont);
    settings.setValue("analysisDialogFont", analysisDialogFont);
    settings.setValue("useSuitSymbols", useSuitSymbols);

    settings.setValue("lowResolutionOption", lowResolutionOption);
    settings.setValue("showStartupBanner", showStartupBanner);
    settings.setValue("showBackgroundBitmap", showBackgroundBitmap);
    settings.setValue("bitmapMode", bitmapMode);
    settings.setValue("scaleLargeBitmaps", scaleLargeBitmaps);
    settings.setValue("filePath", filePath);
    settings.setValue("backgroundRed", backgroundRed);
    settings.setValue("backgroundGreen", backgroundGreen);
    settings.setValue("backgroundBlue", backgroundBlue);
}

void CDisplayOptionDoc::ReadSettings()
{
    QSettings settings("ZBridge settings", "Display");

    suitSequence = settings.value("suitSequence", SHDC_SUIT_SEQUENCE).toInt();
    showDummyTrumpsOnLeft = settings.value("showDummyTrumpsOnLeft", true).toBool();

    tripleBuffer = settings.value("tripleBuffer", false).toBool();
    animateCards = settings.value("animateCards", true).toBool();
    granularity = settings.value("granularity", DEFAULT_ANIMATION_GRANULARITY).toInt();

    cardBack = settings.value("cardBack", 0).toInt();

    autoShowBidHistory = settings.value("autoShowBidHistory", true).toBool();
    autoHideBidHistory = settings.value("autoHideBidHistory", true).toBool();
    autoShowPlayHistory = settings.value("autoShowPlayHistory", false).toBool();
    autoHidePlayHistory  = settings.value("autoHidePlayHistory", true).toBool();
    showLayoutWhenEditing = settings.value("showLayoutWhenEditing", true).toBool() ;

    autoHintDialogFont = settings.value("autoHintDialogFont", "").toString();
    bidAndPlayHistoryDialogFont = settings.value("bidAndPlayHistoryDialogFont", "").toString();
    analysisDialogFont = settings.value("analysisDialogFont", "").toString();
    useSuitSymbols = settings.value("useSuitSymbols", true).toBool() ;

    lowResolutionOption = settings.value("lowResolutionOption", false).toBool();
    showStartupBanner = settings.value("showStartupBanner", true).toBool();
    showBackgroundBitmap = settings.value("showBackgroundBitmap", true).toBool();
    bitmapMode = settings.value("bitmapMode", CENTER_BITMAP_MODE).toInt();
    scaleLargeBitmaps = settings.value("scaleLargeBitmaps", true).toBool();
    filePath = settings.value("filePath", "").toString();
    backgroundRed = settings.value("backgroundRed", 0).toInt();
    backgroundGreen = settings.value("backgroundGreen", 0).toInt();
    backgroundBlue = settings.value("backgroundBlue", 0).toInt();
}
