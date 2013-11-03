/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidOptionDoc.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Model (data etc.)

  Platforms: Qt.

*/

/**
 * \file
 * Model (data etc.) (definition).
 */

#include "QString"
#include "QDataStream"
#include "cbidoptiondoc.h"

CBidOptionDoc::CBidOptionDoc()
{
    int i,j;

    //Default values.
    name = "Natural";

    //Config.
    aggressiveness = 0;

    //General.
    majorsMode = MAJOR_FOUR;
    blackWood = false;
    gerber = false;
    cueBids = false;
    stayman = false;
    jacobyTransfers = false;
    fourSuitTransfers = false;
    limitRaises = false;
    splinterbids = false;
    takeoutDoubles = false;
    negativeDoubles = false;

    //2 bids.
    twoBidsMode = STRONG_TWO;
    twoNTafterWeak2 = 0;
    twoClubOpenValues = OPEN_VALUE_21;

    //NT range.
    oneNTRange = ONE_NT_RANGE_15_17;
    twoNTRange = TWO_NT_RANGE_20_21;
    threeNTRange = THREE_NT_RANGE_25_27;
    gambling3NT = false;

    //Misc.
    jacoby2NT = false;
    michaelsCueBid = false;
    unusual2NT = false;
    drury = false;
    fourSuitForcing = false;
    structuredReverse = false;
    jumpOvercalls = false;

    //Opening.
    elevenHCPsRbsLm = false;
    elevenHCPs6Cs = false;
    fourteenTPsGs = false;
    fourteenTPsLs = false;
    thirteenTPsLs = false;

    //Leads.
    lengthLead = LOW_ENCOURAGING;
    for (i = 0; i< 4; i++)
    for(j = 0; j < MAXOPENINGLEADS; j++)
    {
        openingLead[i][j] = 0;
    }

    //Signals.
    partnerLeadSuit = SIGNAL_ATTITUDE_LOW;
    declarerLeadSuit = SIGNAL_NO_SIGNAL;
    discardingSuit = SIGNAL_ATTITUDE_LOW;
    partnerLeadNT = SIGNAL_ATTITUDE_LOW;
    declarerLeadNT = SIGNAL_NO_SIGNAL;
    discardingNT = SIGNAL_ATTITUDE_LOW;
}

// Assignment operator
CBidOptionDoc& CBidOptionDoc::operator=(const CBidOptionDoc& bidOptionDoc)
{
    // check for self-assignment
    if (this == &bidOptionDoc)
        return *this;

    int i,j;

    //Default values.
    name = bidOptionDoc.name;

    //Config.
    aggressiveness = bidOptionDoc.aggressiveness;

    //General.
    majorsMode = bidOptionDoc.majorsMode;
    blackWood = bidOptionDoc.blackWood;
    gerber = bidOptionDoc.gerber;
    cueBids = bidOptionDoc.cueBids;
    stayman = bidOptionDoc.stayman;
    jacobyTransfers = bidOptionDoc.jacobyTransfers;
    fourSuitTransfers = bidOptionDoc.fourSuitTransfers;
    limitRaises = bidOptionDoc.limitRaises;
    splinterbids = bidOptionDoc.splinterbids;
    takeoutDoubles = bidOptionDoc.takeoutDoubles;
    negativeDoubles = bidOptionDoc.negativeDoubles;

    //2 bids.
    twoBidsMode = bidOptionDoc.twoBidsMode;
    twoNTafterWeak2 = bidOptionDoc.twoNTafterWeak2;
    twoClubOpenValues = bidOptionDoc.twoClubOpenValues;

    //NT range.
    oneNTRange = bidOptionDoc.oneNTRange;
    twoNTRange = bidOptionDoc.twoNTRange;
    threeNTRange = bidOptionDoc.threeNTRange;
    gambling3NT = bidOptionDoc.gambling3NT;

    //Misc.
    jacoby2NT = bidOptionDoc.jacoby2NT;
    michaelsCueBid = bidOptionDoc.michaelsCueBid;
    unusual2NT = bidOptionDoc.unusual2NT;
    drury = bidOptionDoc.drury;
    fourSuitForcing = bidOptionDoc.fourSuitForcing;
    structuredReverse = bidOptionDoc.structuredReverse;
    jumpOvercalls = bidOptionDoc.jumpOvercalls;

    //Opening.
    elevenHCPsRbsLm = bidOptionDoc.elevenHCPsRbsLm;
    elevenHCPs6Cs = bidOptionDoc.elevenHCPs6Cs;
    fourteenTPsGs = bidOptionDoc.fourteenTPsGs;
    fourteenTPsLs = bidOptionDoc.fourteenTPsLs;
    thirteenTPsLs = bidOptionDoc.thirteenTPsLs;

    //Leads.
    lengthLead = bidOptionDoc.lengthLead;
    for (i = 0; i< 4; i++)
    for(j = 0; j < MAXOPENINGLEADS; j++)
    {
        openingLead[i][j] = bidOptionDoc.openingLead[i][j];
    }

    //Signals.
    partnerLeadSuit = bidOptionDoc.partnerLeadSuit;
    declarerLeadSuit = bidOptionDoc.declarerLeadSuit;
    discardingSuit = bidOptionDoc.discardingSuit;
    partnerLeadNT = bidOptionDoc.partnerLeadNT;
    declarerLeadNT = bidOptionDoc.declarerLeadNT;
    discardingNT = bidOptionDoc.discardingNT;

    return *this;
}

QDataStream& operator <<( QDataStream& stream, const CBidOptionDoc& bidOptionDoc )
{
    int i, j;

    stream << (qint8) 1 << bidOptionDoc.name << bidOptionDoc.aggressiveness
        << bidOptionDoc.majorsMode << bidOptionDoc.blackWood << bidOptionDoc.gerber
        << bidOptionDoc.cueBids << bidOptionDoc.stayman << bidOptionDoc.jacobyTransfers
        << bidOptionDoc.fourSuitTransfers << bidOptionDoc.limitRaises << bidOptionDoc.splinterbids
        << bidOptionDoc.takeoutDoubles << bidOptionDoc.negativeDoubles << bidOptionDoc.twoBidsMode
        << bidOptionDoc.twoNTafterWeak2 << bidOptionDoc.twoClubOpenValues << bidOptionDoc.oneNTRange
        << bidOptionDoc.twoNTRange << bidOptionDoc.threeNTRange << bidOptionDoc.gambling3NT
        << bidOptionDoc.jacoby2NT << bidOptionDoc.michaelsCueBid << bidOptionDoc.unusual2NT
        << bidOptionDoc.drury << bidOptionDoc.fourSuitForcing << bidOptionDoc.structuredReverse
        << bidOptionDoc.jumpOvercalls << bidOptionDoc.elevenHCPsRbsLm << bidOptionDoc.elevenHCPs6Cs
        << bidOptionDoc.fourteenTPsGs << bidOptionDoc.fourteenTPsLs << bidOptionDoc.thirteenTPsLs
        << bidOptionDoc.lengthLead;

    for (i = 0; i < 4; i++)
    for (j = 0; j < MAXOPENINGLEADS; j++)
        stream << bidOptionDoc.openingLead[i][j];

    stream << bidOptionDoc.partnerLeadSuit << bidOptionDoc.declarerLeadSuit
        << bidOptionDoc.discardingSuit << bidOptionDoc.partnerLeadNT << bidOptionDoc.declarerLeadNT
        << bidOptionDoc.discardingNT;

    return stream;
}

QDataStream& operator >>( QDataStream& stream, CBidOptionDoc& bidOptionDoc )
{
    qint8 version;
    int i, j;

    stream >> version >> bidOptionDoc.name >> bidOptionDoc.aggressiveness
        >> bidOptionDoc.majorsMode >> bidOptionDoc.blackWood >> bidOptionDoc.gerber
        >> bidOptionDoc.cueBids >> bidOptionDoc.stayman >> bidOptionDoc.jacobyTransfers
        >> bidOptionDoc.fourSuitTransfers >> bidOptionDoc.limitRaises >> bidOptionDoc.splinterbids
        >> bidOptionDoc.takeoutDoubles >> bidOptionDoc.negativeDoubles >> bidOptionDoc.twoBidsMode
        >> bidOptionDoc.twoNTafterWeak2 >> bidOptionDoc.twoClubOpenValues >> bidOptionDoc.oneNTRange
        >> bidOptionDoc.twoNTRange >> bidOptionDoc.threeNTRange >> bidOptionDoc.gambling3NT
        >> bidOptionDoc.jacoby2NT >> bidOptionDoc.michaelsCueBid >> bidOptionDoc.unusual2NT
        >> bidOptionDoc.drury >> bidOptionDoc.fourSuitForcing >> bidOptionDoc.structuredReverse
        >> bidOptionDoc.jumpOvercalls >> bidOptionDoc.elevenHCPsRbsLm >> bidOptionDoc.elevenHCPs6Cs
        >> bidOptionDoc.fourteenTPsGs >> bidOptionDoc.fourteenTPsLs >> bidOptionDoc.thirteenTPsLs
        >> bidOptionDoc.lengthLead;

    for (i = 0; i < 4; i++)
    for (j = 0; j < MAXOPENINGLEADS; j++)
        stream >> bidOptionDoc.openingLead[i][j];

    stream >> bidOptionDoc.partnerLeadSuit >> bidOptionDoc.declarerLeadSuit
        >> bidOptionDoc.discardingSuit >> bidOptionDoc.partnerLeadNT >> bidOptionDoc.declarerLeadNT
        >> bidOptionDoc.discardingNT;

    return stream;
}


