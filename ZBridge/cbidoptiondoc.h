/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidOptionDoc.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Bid options.

  Platforms: Qt.

*/

/**
 * \file
 * Bid options (declaration).
 */

#ifndef CBIDOPTIONDOC_H
#define CBIDOPTIONDOC_H

#include "Defines.h"

class QString;
class QDataStream;

/**
 * @brief This class defines a set of bid options. It also describes how the options are saved and
 * restored form persistent storage.
 */
class CBidOptionDoc
{
public:
    CBidOptionDoc();

    CBidOptionDoc& operator=(const CBidOptionDoc& bidOptionDoc);

    QString name;

    //Config.
    int aggressiveness;

    //General.
    int bidStyle;
    int majorsMode;
    bool blackWood;
    bool gerber;
    bool cueBids;
    int stayman;
    bool jacobyTransfers;
    bool fourSuitTransfers;
    bool limitRaises;
    bool jacoby2NT;
    bool splinterbids;
    bool fourSuitForcing;

    //2 bids.
    int twoBidsMode;
    int w2NewSuit;
    int twoClubOpenValues;
    int twoCtwoD;

    //NT range.
    int oneNTRange;
    int twoNTRange;
    int threeNTRange;
    bool open1NTWeakMajor;

    //NT other.
    int twoNT11;
    int twoNT21;
    int rebid1NT;
    int oneNT3m;
    int oneNT3M;

    //Misc.
    bool structuredReverse;
    bool takeoutDoubles;
    bool negativeDoubles;
    int directCueBid;
    bool unusual2NT;
    bool drury;
    int jumpOvercalls;
    int threeLevel;

    //Opening.
    bool elevenHCPsRbsLm;
    bool elevenHCPs6Cs;
    bool fourteenTPsGs;
    bool fourteenTPsLs;
    bool thirteenTPsLs;

    //Leads.
    int lengthLead;
    int openingLead[2][MAXOPENINGLEADS];

    //Signals.
    int partnerLeadSuit;
    int declarerLeadSuit;
    int discardingSuit;
    int partnerLeadNT;
    int declarerLeadNT;
    int discardingNT;
};

QDataStream& operator <<( QDataStream& stream, const CBidOptionDoc& bidOptionDoc);
QDataStream& operator >>( QDataStream& stream, CBidOptionDoc& bidOptionDoc);

#endif // CBIDOPTIONDOC_H
