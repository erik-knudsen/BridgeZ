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

/**
 * @brief Bid constructor.
 * @param bidder The bidder.
 * @param bid The bid.
 * @param alert An alert (if any).
 * @param rules The rules for the bid.
 * @param substitute Indicates if a substitute bid was used.
 *
 * Constructs the bid.
 */
CBid::CBid(Seat bidder, Bids bid, QString alert, QList<CRule *> &rules, bool substitute)
{
    this->bidder = bidder;
    this->bid = bid;
    this->alert = alert;
    this->rules = rules;
    this->substitute = substitute;
    delRules = false;
}

CBid::CBid(Seat bidder, Bids bid, QString alert)
{
    this->bidder = bidder;
    this->bid = bid;
    this->alert = alert;
    substitute = false;
    delRules = false;
}

CBid::~CBid()
{
    if (delRules)
    for (int i = 0; i < rules.size(); i++)
        if (!rules[i]->isdBRule())
            delete rules[i];
}
