/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidOptionDoc.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Model (data).

  Platforms: Qt.

*/

/**
 * \file
 * Model (data) (declaration).
 */

#ifndef CBIDOPTIONDOC_H
#define CBIDOPTIONDOC_H

#include "Defines.h"

class QString;
class QDataStream;

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
    bool stayman;
    bool jacobyTransfers;
    bool fourSuitTransfers;
    bool limitRaises;
    bool splinterbids;
    bool takeoutDoubles;
    bool negativeDoubles;

    //2 bids.
    int twoBidsMode;
    int twoNTafterWeak2;
    int twoClubOpenValues;

    //NT range.
    int oneNTRange;
    int twoNTRange;
    int threeNTRange;
    bool gambling3NT;

    //Misc.
    bool jacoby2NT;
    bool michaelsCueBid;
    bool unusual2NT;
    bool drury;
    bool fourSuitForcing;
    bool structuredReverse;
    int jumpOvercalls;

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
