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
    int majorsMode;
    bool blackWood;
    bool gerber;
    bool cueBids;
    int stayman;
    bool jacobyTransfers;
    bool fourSuitTransfers;
    bool limitRaises;
    bool splinterbids;
    bool takeoutDoubles;
    bool negativeDoubles;

    //2 bids.
    int twoBidsMode;
    int twoClubOpenValues;
    int afterW2;
    int w2NewSuit;
    int twoCtwoD;

    //NT range.
    int oneNTRange;
    int twoNTRange;
    int threeNTRange;
    bool gambling3NT;

    //NT other.
    int oneNT3m;
    int oneNT3M;
    int twoNTAfter11;
    int after1NTRebid;
    bool open1NTWeakMajor;
    int twoNTAfter21;

    //Three level;
    int threeLevelPE;

    //Misc.
    bool jacoby2NT;
    int directCueBid;
    bool unusual2NT;
    bool drury;
    bool fourSuitForcing;
    bool structuredReverse;
    int jumpOvercalls;
    int afterWJO;

    //Opening.
    bool elevenHCPsRbsLm;
    bool elevenHCPs6Cs;
    bool fourteenTPsGs;
    bool fourteenTPsLs;
    bool thirteenTPsLs;

    //Leads.
    int lengthLead;
    int openingLead[4][MAXOPENINGLEADS];

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
