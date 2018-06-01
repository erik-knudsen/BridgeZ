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
//    delRules = false;
}

CBid::CBid(Seat bidder, Bids bid, QString alert)
{
    this->bidder = bidder;
    this->bid = bid;
    this->alert = alert;
    substitute = false;
//    delRules = false;
}

/**
 * @brief Destructor deletes non DB rules.
 *
 * The rules delimiting the bid can be either found in the bidding database or they can be found
 * by a built in algoritmic approach. In the first case the rule pointers points directly into the
 * bidding database and should therefore not be deleted. In the second case the rules are calculated
 * and should be deleted upon destruction.
 */
CBid::~CBid()
{
    //Below does not work on all implementations (when in a list it seems there
    //are internal copying of elements, so it is not sufficient to clear the list).
/*    if (delRules)
    for (int i = 0; i < rules.size(); i++)
        if (!rules[i]->isdBRule())
            delete rules[i];
*/}

void CBid::deleteRules()
{
    for (int i = 0; i < rules.size(); i++)
        if (!rules[i]->isdBRule())
            delete rules[i];
}
