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

CGameOptionDoc::CGameOptionDoc()
{
   //Interface.
   insertBiddingPause = true;
   biddingPause = 3;
   insertPlayPause = true;
   playPause = 3;

   //Scoring.
   scoringMethod = MP;
}

void CGameOptionDoc::WriteSettings()
{
    QSettings settings("ZBridge settings", "Game");

    settings.setValue("insertBiddingPause", insertBiddingPause);
    settings.setValue("biddingPause", biddingPause);
    settings.setValue("insertPlayPause", insertPlayPause);
    settings.setValue("playPause", playPause);

    settings.setValue("scoringMethod", scoringMethod);
}

void CGameOptionDoc::ReadSettings()
{
    QSettings settings("ZBridge settings", "Game");

    insertBiddingPause = settings.value("insertBiddingPause", true).toBool();
    biddingPause = settings.value("biddingPause", 3).toInt();
    insertPlayPause = settings.value("insertPlayPause", true).toBool();
    playPause = settings.value("playPause", 3).toInt();

    int tmp = settings.value("scoringMethod", MP).toInt();
    scoringMethod = ((tmp >= IMP) && (tmp <= PRACTICE)) ? (ScoringMethod)tmp : MP;
}
