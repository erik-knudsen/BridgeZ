/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CGameOptionDoc.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Game option data.

  Platforms: Qt.

*/

/**
 * \file
 * Game option data (declaration).
 */

#ifndef CGAMEOPTIONDOC_H
#define CGAMEOPTIONDOC_H

class CGameOptionDoc
{
public:
    CGameOptionDoc();

    void WriteSettings();
    void ReadSettings();

    //Help level.
    int helpLevel;

    //Interface.
    bool autoPlayLastCard;
    bool autoJumpCursor;
    bool insertBiddingPause;
    int biddingPause;
    bool insertPlayPause;
    int playPause;

    //Mechanics.
    bool autoBidStart;
    bool computerCanClaim;
    bool allowRebidPassedHands;
    bool enableTracing;
    int traceLevel;
    bool showPassedHands;
    bool enableAnalysisDuringHints;

    //Counting.
    int valuationMode;
    float aceValue;
    float kingValue;
    float queenValue;
    float jackValue;
    float tenValue;

    //Scoring.
    bool useDuplicateScoring;
    bool scoreHonorsBonuses;

    //Files.
    bool saveIntermediatePositions;
    bool exposePBNGameCards;
};

#endif // CGAMEOPTIONDOC_H
