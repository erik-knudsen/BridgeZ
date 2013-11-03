/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBid.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: One bid.

  Platforms: Qt.

*/

/**
 * @file One bid.
 * One bid (definition).
 */

#include "cbid.h"

CBid::CBid(Seat bidder, Bids bid, QString alert)
{
    this->bidder = bidder;
    this->bid = bid;
    this->alert = alert;
}
