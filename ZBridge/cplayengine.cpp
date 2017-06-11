/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CPlayEngine.cpp
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Play engine.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the play engine.
 */

#include "cbidhistory.h"
#include "cplayhistory.h"
#include "cplayengine.h"

CPlayEngine::CPlayEngine()
{
}

/**
 * @brief Determine the next card to play.
 *
 * @param seat The players seat.
 * @param dummySeat The dummys seat.
 * @param ownCards The players cards.
 * @param dummyCards The dummys cards.
 * @param bidHistory The bid history.
 * @param playHistory The play history.
 * @return The next card to play.
 */
int CPlayEngine::getNextPlay(Seat seat, Seat dummySeat, int ownCards[], int dummyCards[],
                                    CBidHistory &bidHistory, CPlayHistory &playHistory)
{
    int i;

    int *cards = (seat == dummySeat) ? dummyCards : ownCards;

    for (i = 0; i < 52; i++)
        if (playHistory.cardOk(i, seat, cards))
            break;

    return i;
}
