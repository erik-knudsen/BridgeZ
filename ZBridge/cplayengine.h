/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CPlayEngine.h
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Play engine.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the play engine.
 */

#ifndef CPLAYENGINE_H
#define CPLAYENGINE_H

#include "cbidoptiondoc.h"
#include "Defines.h"

class CBidHistory;
class CPlayHistory;

/**
 * @brief Play engine.
 *
 * The next card to play is determined as follows:\n
 *   - Calculate a number of possible hand distributions based on remaining cards to play
 *     and on the players signals as defined by the user.
 *   - Solve these hands using double dummy algoritms (Bo Haglund, Soren Hein, Bob Richardson: DDS v. 2.8).
 *   - Select the best card to play based on this.\n
 */
class CPlayEngine
{
public:
    CPlayEngine(CBidOptionDoc &nsBidOptionDoc, CBidOptionDoc &ewBidOptionDoc);

    int getNextPlay(Seat seat, Seat dummySeat, int ownCards[], int dummyCards[], CBidHistory &bidHistory,
                    CPlayHistory &playHistory);

private:
    int calcWeight(int hands[4][13], Seat seat, Seat dummySeat, CBidHistory &bidHistory, CPlayHistory &playHistory,
                        CBidOptionDoc &nsBidOptions, CBidOptionDoc &ewBidOptions);
    int getBestCard(int cards[], int ownCards[], int dummyCards[], Seat seat, Seat dummySeat, CBidHistory &bidHistory,
                    CPlayHistory &playHistory, CBidOptionDoc &nsBidOptions, CBidOptionDoc &ewBidOptions);

    CBidOptionDoc nsBidOptions;
    CBidOptionDoc ewBidOptions;
};

#endif // CPLAYENGINE_H
