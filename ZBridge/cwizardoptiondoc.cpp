/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CWizardOptionDoc.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard data.

  Platforms: Qt.
*/

/**
 * \file
 * Wizard data (definition).
 */

#include <QSettings>

#include "cwizardoptiondoc.h"
#include "Defines.h"

CWizardOptionDoc::CWizardOptionDoc()
{    
    //Help Level.
    helpLevel = BASIC_HELP_LEVEL;
    enableTracing = false;

    //View Settings.
    autoShowBidHistory = true;
    autoShowPlayHistory = false;
    useSuitSymbols = true;

    //Suits Display.
    suitSequence = SHDC_SUIT_SEQUENCE;
    showDummyTrumpsOnLeft = true;

    //Game Options.
    scoringMethod = DUPLICATE_MP;

    //Pauses.
    insertBiddingPause = true;
    biddingPauseLength = 3;
    insertPlayPause = true;
    playPauseLenght = 3;

    //Bidding Style.
    biddingStyle = GOREN_BIDDING_STYLE;
}

void CWizardOptionDoc::WriteSettings()
{
    QSettings settings("ZBridge settings", "Wizard");

    //Help Level.
    settings.setValue("helpLevel", helpLevel);
    settings.setValue("enableTracing", enableTracing);

    //View Settings.
    settings.setValue("autoShowBidHistory", autoShowBidHistory);
    settings.setValue("autoShowPlayHistory", autoShowPlayHistory);
    settings.setValue("useSuitSymbols", useSuitSymbols);

    //Suits Display.
    settings.setValue("suitSequence", suitSequence);
    settings.setValue("showDummyTrumpsOnLeft", showDummyTrumpsOnLeft);

    //Game Options.
    settings.setValue("scoringMethod", scoringMethod);

    //Pauses.
    settings.setValue("insertBiddingPause", insertBiddingPause);
    settings.setValue("biddingPauseLength", biddingPauseLength);
    settings.setValue("insertPlayPause", insertPlayPause);
    settings.setValue("playPauseLenght", playPauseLenght);

    //Bidding Style.
    settings.setValue("biddingStyle", biddingStyle);
}

void CWizardOptionDoc::ReadSettings()
{
    QSettings settings("ZBridge settings", "Wizard");

    //Help Level.
    helpLevel = settings.value("helpLevel", BASIC_HELP_LEVEL).toBool();
    enableTracing = settings.value("enableTracing", false).toBool();

    //View Settings.
    autoShowBidHistory = settings.value("autoShowBidHistory", true).toBool();
    autoShowPlayHistory = settings.value("autoShowPlayHistory", false).toBool();
    useSuitSymbols = settings.value("useSuitSymbols", true).toBool();

    //Suits Display.
    suitSequence = settings.value("suitSequence", SHDC_SUIT_SEQUENCE).toInt();
    showDummyTrumpsOnLeft = settings.value("showDummyTrumpsOnLeft", true).toBool();

    //Game Options.
    scoringMethod = settings.value("scoringMethod", DUPLICATE_MP).toInt();

    //Pauses.
    insertBiddingPause = settings.value("insertBiddingPause", true).toBool();
    biddingPauseLength = settings.value("biddingPauseLength", 3).toInt();
    insertPlayPause = settings.value("insertPlayPause", true).toBool();
    playPauseLenght = settings.value("playPauseLenght", 3).toInt();

    //Bidding Style.
    biddingStyle = settings.value("biddingStyle", GOREN_BIDDING_STYLE).toInt();
}
