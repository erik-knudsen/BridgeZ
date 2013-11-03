/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidHistory.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Bid history.

  Platforms: Qt.

*/

/**
 * \file
 * Bid History (declaration).
 */

#ifndef CBIDHISTORY_H
#define CBIDHISTORY_H

#include <QVector>

#include "cbid.h"

/**
 * @brief The CBidHistory class describes the bid history.
 */
class CBidHistory
{
public:
    CBidHistory();

    void appendBid(CBid &bid);
    void removeBid();
    void resetBidHistory();
    int undo(Bids *bid);

    QVector<CBid> bidList;
};

#endif // CBIDHISTORY_H
