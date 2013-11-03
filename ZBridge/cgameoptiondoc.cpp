/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CGameOptionDoc.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Game option data.

  Platforms: Qt.

*/

/**
 * \file
 * Game option data (definition).
 */

#include <QSettings>

#include "cgameoptiondoc.h"
#include "Defines.h"

CGameOptionDoc::CGameOptionDoc()
{
   //Help level.
   helpLevel = BASIC_HELP_LEVEL;

   //Interface.
   autoPlayLastCard = false;
   autoJumpCursor = false;
   insertBiddingPause = true;
   biddingPause = 3;
   insertPlayPause = true;
   playPause = 3;

   //Mechanics.
   autoBidStart = true;
   computerCanClaim = false;
   allowRebidPassedHands = false;
   enableTracing = false;
   traceLevel = LEVEL_1_TRACE_LEVEL;
   showPassedHands = false;
   enableAnalysisDuringHints = true;

   //Counting.
   valuationMode = GOREN_VALUATION_MODE;
   aceValue = 4.0;
   kingValue = 3.0;
   queenValue = 2.0;
   jackValue = 1.0;
   tenValue = 0.0;

   //Scoring.
   useDuplicateScoring = true;
   scoreHonorsBonuses = false;

   //Files.
   saveIntermediatePositions = false;
   exposePBNGameCards = false;
}

void CGameOptionDoc::WriteSettings()
{
    QSettings settings("ZBridge settings", "Game");

    settings.setValue("helpLevel", helpLevel);

    settings.setValue("autoPlayLastCard", autoPlayLastCard);
    settings.setValue("autoJumpCursor", autoJumpCursor);
    settings.setValue("insertBiddingPause", insertBiddingPause);
    settings.setValue("biddingPause", biddingPause);
    settings.setValue("insertPlayPause", insertPlayPause);
    settings.setValue("playPause", playPause);

    settings.setValue("autoBidStart", autoBidStart);
    settings.setValue("computerCanClaim", computerCanClaim);
    settings.setValue("allowRebidPassedHands", allowRebidPassedHands);
    settings.setValue("enableTracing", enableTracing);
    settings.setValue("traceLevel", traceLevel);
    settings.setValue("showPassedHands", showPassedHands);
    settings.setValue("enableAnalysisDuringHints",  enableAnalysisDuringHints);

    settings.setValue("valuationMode", valuationMode);
    settings.setValue("aceValue", aceValue);
    settings.setValue("kingValue", kingValue);
    settings.setValue("queenValue", queenValue);
    settings.setValue("jackValue", jackValue);
    settings.setValue("tenValue", tenValue);

    settings.setValue("useDuplicateScoring", useDuplicateScoring);
    settings.setValue("scoreHonorsBonuses", scoreHonorsBonuses);

    settings.setValue("saveIntermediatePositions", saveIntermediatePositions);
    settings.setValue("exposePBNGameCards", exposePBNGameCards);
}

void CGameOptionDoc::ReadSettings()
{
    QSettings settings("ZBridge settings", "Game");

    helpLevel = settings.value("helpLevel", BASIC_HELP_LEVEL).toInt();

    autoPlayLastCard = settings.value("autoPlayLastCard", false).toBool();
    autoJumpCursor = settings.value("autoJumpCursor", false).toBool();
    insertBiddingPause = settings.value("insertBiddingPause", true).toBool();
    biddingPause = settings.value("biddingPause", 3).toInt();
    insertPlayPause = settings.value("insertPlayPause", true).toBool();
    playPause = settings.value("playPause", 3).toInt();

    autoBidStart = settings.value("autoBidStart", true).toBool();
    computerCanClaim = settings.value("computerCanClaim", false).toBool();
    allowRebidPassedHands = settings.value("allowRebidPassedHands", false).toBool();
    enableTracing = settings.value("enableTracing", false).toBool();
    traceLevel = settings.value("traceLevel", LEVEL_1_TRACE_LEVEL).toInt();
    showPassedHands = settings.value("showPassedHands", false).toBool();
    enableAnalysisDuringHints = settings.value("enableAnalysisDuringHints", true).toBool();

    valuationMode = settings.value("valuationMode", GOREN_VALUATION_MODE).toInt();
    aceValue = settings.value("aceValue", 4.0).toFloat();
    kingValue = settings.value("kingValue", 3.0).toFloat();
    queenValue = settings.value("queenValue", 2.0).toFloat();
    jackValue = settings.value("jackValue", 1.0).toFloat();
    tenValue =settings.value("tenValue", 0.0).toFloat();

    useDuplicateScoring = settings.value("useDuplicateScoring", true).toBool();
    scoreHonorsBonuses = settings.value("scoreHonorsBonuses", false).toBool();

    saveIntermediatePositions = settings.value("saveIntermediatePositions", false).toBool();
    exposePBNGameCards = settings.value("exposePBNGameCards", false).toBool();
}
