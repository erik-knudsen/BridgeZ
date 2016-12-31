/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidOptionDoc.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Bid options.

  Platforms: Qt.

*/

/**
 * \file
 * Bid options (definition).
 */

#include "QString"
#include "QDataStream"
#include "cbidoptiondoc.h"

/**
 * @brief The constructor initializes all bid options with defalt values.
 */
CBidOptionDoc::CBidOptionDoc()
{
    int i,j;

    //Default values.
    name = "Natural";

    //Config.
    aggressiveness = 0;

    //General.
    majorsMode = MAJOR_FOUR;
    blackWood = true;
    gerber = true;
    cueBids = true;
    stayman = STAYMAN_3R_NF;
    jacobyTransfers = false;
    fourSuitTransfers = false;
    limitRaises = true;
    splinterbids = false;
    takeoutDoubles = true;
    negativeDoubles = false;

    //2 bids.
    twoBidsMode = STRONG_TWO;
    twoClubOpenValues = OPEN_VALUE_22;

    //NT range.
    oneNTRange = ONE_NT_RANGE_15_17;
    twoNTRange = TWO_NT_RANGE_20_21;
    threeNTRange = THREE_NT_RANGE_25_27;
    gambling3NT = false;

    //NT other.
    oneNT3m = ONE_NT_3m_ST;
    oneNT3M = ONE_NT_3M_GT;
    twoNTAfter11 = TWO_NT_11_18_20;
    after1NTRebid = REBID_1NT_NAT;
    open1NTWeakMajor = true;
    twoNTAfter21 = TWO_NT_21_12_14;

    //Three level;
    threeLevelPE = SOUND_THREE_LEVEL;

    //Misc.
    jacoby2NT = false;
    directCueBid = STRONG_CUE_BID;
    unusual2NT = false;
    drury = false;
    fourSuitForcing = true;
    structuredReverse = true;
    jumpOvercalls = JUMP_OVERCALL_WEAK;

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

/**
 * @brief Assignment operator.
 */
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
    twoClubOpenValues = bidOptionDoc.twoClubOpenValues;
    afterW2 = bidOptionDoc.afterW2;
    w2NewSuit = bidOptionDoc.w2NewSuit;
    twoCtwoD = bidOptionDoc.twoCtwoD;

    //NT range.
    oneNTRange = bidOptionDoc.oneNTRange;
    twoNTRange = bidOptionDoc.twoNTRange;
    threeNTRange = bidOptionDoc.threeNTRange;
    gambling3NT = bidOptionDoc.gambling3NT;

    //NT other.
    oneNT3m = bidOptionDoc.oneNT3m;
    oneNT3M = bidOptionDoc.oneNT3M;
    twoNTAfter11 = bidOptionDoc.twoNTAfter11;
    after1NTRebid = bidOptionDoc.after1NTRebid;
    open1NTWeakMajor = bidOptionDoc.open1NTWeakMajor;
    twoNTAfter21 = bidOptionDoc.twoNTAfter21;

    //Three level;
    threeLevelPE = bidOptionDoc.threeLevelPE;

    //Misc.
    jacoby2NT = bidOptionDoc.jacoby2NT;
    directCueBid = bidOptionDoc.directCueBid;
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

/**
 * @brief Write operator.
 * @param stream The stream to write to.
 * @param bidOptionDoc The options to write.
 */
QDataStream& operator <<( QDataStream& stream, const CBidOptionDoc& bidOptionDoc )
{
    int i, j;

    stream << (qint8) 1 << bidOptionDoc.name << bidOptionDoc.aggressiveness
        << bidOptionDoc.majorsMode << bidOptionDoc.blackWood << bidOptionDoc.gerber
        << bidOptionDoc.cueBids << bidOptionDoc.stayman << bidOptionDoc.jacobyTransfers
        << bidOptionDoc.fourSuitTransfers << bidOptionDoc.limitRaises << bidOptionDoc.splinterbids
        << bidOptionDoc.takeoutDoubles << bidOptionDoc.negativeDoubles << bidOptionDoc.twoBidsMode
        << bidOptionDoc.twoClubOpenValues << bidOptionDoc.afterW2 << bidOptionDoc.w2NewSuit
        << bidOptionDoc.twoCtwoD << bidOptionDoc.oneNTRange
        << bidOptionDoc.twoNTRange << bidOptionDoc.threeNTRange << bidOptionDoc.gambling3NT
        << bidOptionDoc.oneNT3m << bidOptionDoc.oneNT3M << bidOptionDoc.twoNTAfter11
        << bidOptionDoc.after1NTRebid << bidOptionDoc.open1NTWeakMajor
        << bidOptionDoc.twoNTAfter21 << bidOptionDoc.threeLevelPE
        << bidOptionDoc.jacoby2NT << bidOptionDoc.directCueBid << bidOptionDoc.unusual2NT
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

/**
 * @brief Read operator.
 * @param stream The stream to read from.
 * @param bidOptionDoc The bid option document to read into.
 */
QDataStream& operator >>( QDataStream& stream, CBidOptionDoc& bidOptionDoc )
{
    qint8 version;
    int i, j;

    stream >> version >> bidOptionDoc.name >> bidOptionDoc.aggressiveness
        >> bidOptionDoc.majorsMode >> bidOptionDoc.blackWood >> bidOptionDoc.gerber
        >> bidOptionDoc.cueBids >> bidOptionDoc.stayman >> bidOptionDoc.jacobyTransfers
        >> bidOptionDoc.fourSuitTransfers >> bidOptionDoc.limitRaises >> bidOptionDoc.splinterbids
        >> bidOptionDoc.takeoutDoubles >> bidOptionDoc.negativeDoubles >> bidOptionDoc.twoBidsMode
        >> bidOptionDoc.twoClubOpenValues >> bidOptionDoc.afterW2 >> bidOptionDoc.w2NewSuit
        >> bidOptionDoc.twoCtwoD >> bidOptionDoc.oneNTRange
        >> bidOptionDoc.twoNTRange >> bidOptionDoc.threeNTRange >> bidOptionDoc.gambling3NT
        >> bidOptionDoc.oneNT3m >> bidOptionDoc.oneNT3M >> bidOptionDoc.twoNTAfter11
        >> bidOptionDoc.after1NTRebid >> bidOptionDoc.open1NTWeakMajor
        >> bidOptionDoc.twoNTAfter21 >> bidOptionDoc.threeLevelPE
        >> bidOptionDoc.jacoby2NT >> bidOptionDoc.directCueBid >> bidOptionDoc.unusual2NT
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


