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
    bidStyle = EUROPEAN_STYLE;
    majorsMode = MAJOR_FOUR;
    blackWood = true;
    gerber = true;
    cueBids = true;
    stayman = STAYMAN_3R_NF;
    jacobyTransfers = false;
    fourSuitTransfers = false;
    limitRaises = true;
    jacoby2NT = false;
    splinterbids = false;
    fourSuitForcing = true;

    //2 bids.
    twoBidsMode = W2_FEATURE;
    w2NewSuit = W2_NEW_SUIT_NF;
    twoClubOpenValues = OPEN_VALUE_22;
    twoCtwoD = TWO_C_TWO_D_NEGATIVE;

    //NT range.
    oneNTRange = ONE_NT_RANGE_15_17;
    twoNTRange = TWO_NT_RANGE_20_21;
    threeNTRange = THREE_NT_RANGE_25_27;
    open1NTWeakMajor = true;

    //NT other.
    twoNT11 = TWO_NT_11_18_20;
    twoNT21 = TWO_NT_21_12_14;
    rebid1NT = REBID_1NT_NAT;
    oneNT3m = ONE_NT_3m_SI;
    oneNT3M = ONE_NT_3M_GI;

    //Misc.
    structuredReverse = true;
    takeoutDoubles = true;
    negativeDoubles = false;
    directCueBid = STRONG_CUE_BID;
    unusual2NT = false;
    drury = false;
    jumpOvercalls = JUMP_OVERCALL_WEAK_NATURAL;
    threeLevel = SOUND_THREE_LEVEL;

    //Opening.
    elevenHCPsRbsLm = false;
    elevenHCPs6Cs = false;
    fourteenTPsGs = false;
    fourteenTPsLs = false;
    thirteenTPsLs = false;

    //Leads.
    lengthLead = LOW_ENCOURAGING;
    for (i = 0; i < 2; i++)
    for(j = 0; j < MAXOPENINGLEADS; j++)
        openingLead[i][j] = 0;

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
    bidStyle = bidOptionDoc.bidStyle;
    majorsMode = bidOptionDoc.majorsMode;
    blackWood = bidOptionDoc.blackWood;
    gerber = bidOptionDoc.gerber;
    cueBids = bidOptionDoc.cueBids;
    stayman = bidOptionDoc.stayman;
    jacobyTransfers = bidOptionDoc.jacobyTransfers;
    fourSuitTransfers = bidOptionDoc.fourSuitTransfers;
    limitRaises = bidOptionDoc.limitRaises;
    jacoby2NT = bidOptionDoc.jacoby2NT;
    splinterbids = bidOptionDoc.splinterbids;
    fourSuitForcing = bidOptionDoc.fourSuitForcing;

    //2 bids.
    twoBidsMode = bidOptionDoc.twoBidsMode;
    w2NewSuit = bidOptionDoc.w2NewSuit;
    twoClubOpenValues = bidOptionDoc.twoClubOpenValues;
    twoCtwoD = bidOptionDoc.twoCtwoD;

    //NT range.
    oneNTRange = bidOptionDoc.oneNTRange;
    twoNTRange = bidOptionDoc.twoNTRange;
    threeNTRange = bidOptionDoc.threeNTRange;
    open1NTWeakMajor = bidOptionDoc.open1NTWeakMajor;

    //NT other.
    twoNT11 = bidOptionDoc.twoNT11;
    twoNT21 = bidOptionDoc.twoNT21;
    rebid1NT = bidOptionDoc.rebid1NT;
    oneNT3m = bidOptionDoc.oneNT3m;
    oneNT3M = bidOptionDoc.oneNT3M;

    //Three level;

    //Misc.
    structuredReverse = bidOptionDoc.structuredReverse;
    takeoutDoubles = bidOptionDoc.takeoutDoubles;
    negativeDoubles = bidOptionDoc.negativeDoubles;
    directCueBid = bidOptionDoc.directCueBid;
    unusual2NT = bidOptionDoc.unusual2NT;
    drury = bidOptionDoc.drury;
    jumpOvercalls = bidOptionDoc.jumpOvercalls;
    threeLevel = bidOptionDoc.threeLevel;

    //Opening.
    elevenHCPsRbsLm = bidOptionDoc.elevenHCPsRbsLm;
    elevenHCPs6Cs = bidOptionDoc.elevenHCPs6Cs;
    fourteenTPsGs = bidOptionDoc.fourteenTPsGs;
    fourteenTPsLs = bidOptionDoc.fourteenTPsLs;
    thirteenTPsLs = bidOptionDoc.thirteenTPsLs;

    //Leads.
    lengthLead = bidOptionDoc.lengthLead;
    for (i = 0; i < 2; i++)
    for(j = 0; j < MAXOPENINGLEADS; j++)
        openingLead[i][j] = bidOptionDoc.openingLead[i][j];

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
        <<bidOptionDoc.bidStyle << bidOptionDoc.majorsMode << bidOptionDoc.blackWood << bidOptionDoc.gerber
        << bidOptionDoc.cueBids << bidOptionDoc.stayman << bidOptionDoc.jacobyTransfers
        << bidOptionDoc.fourSuitTransfers << bidOptionDoc.limitRaises << bidOptionDoc.jacoby2NT
        << bidOptionDoc.splinterbids<< bidOptionDoc.fourSuitForcing
        << bidOptionDoc.twoBidsMode << bidOptionDoc.w2NewSuit << bidOptionDoc.twoClubOpenValues
        << bidOptionDoc.twoCtwoD
        << bidOptionDoc.oneNTRange << bidOptionDoc.twoNTRange << bidOptionDoc.threeNTRange
        << bidOptionDoc.open1NTWeakMajor
        << bidOptionDoc.twoNT11<< bidOptionDoc.twoNT21 << bidOptionDoc.rebid1NT
        << bidOptionDoc.oneNT3m << bidOptionDoc.oneNT3M
        << bidOptionDoc.structuredReverse << bidOptionDoc.takeoutDoubles << bidOptionDoc.negativeDoubles
        << bidOptionDoc.directCueBid << bidOptionDoc.unusual2NT << bidOptionDoc.drury
        << bidOptionDoc.jumpOvercalls << bidOptionDoc.threeLevel
        << bidOptionDoc.elevenHCPsRbsLm << bidOptionDoc.elevenHCPs6Cs
        << bidOptionDoc.fourteenTPsGs << bidOptionDoc.fourteenTPsLs << bidOptionDoc.thirteenTPsLs
        << bidOptionDoc.lengthLead;

    for (i = 0; i < 2; i++)
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
        >>bidOptionDoc.bidStyle >> bidOptionDoc.majorsMode >> bidOptionDoc.blackWood >> bidOptionDoc.gerber
        >> bidOptionDoc.cueBids >> bidOptionDoc.stayman >> bidOptionDoc.jacobyTransfers
        >> bidOptionDoc.fourSuitTransfers >> bidOptionDoc.limitRaises >> bidOptionDoc.jacoby2NT
        >> bidOptionDoc.splinterbids>> bidOptionDoc.fourSuitForcing
        >> bidOptionDoc.twoBidsMode >> bidOptionDoc.w2NewSuit >> bidOptionDoc.twoClubOpenValues
        >> bidOptionDoc.twoCtwoD
        >> bidOptionDoc.oneNTRange >> bidOptionDoc.twoNTRange >> bidOptionDoc.threeNTRange
        >> bidOptionDoc.open1NTWeakMajor
        >> bidOptionDoc.twoNT11>> bidOptionDoc.twoNT21 >> bidOptionDoc.rebid1NT
        >> bidOptionDoc.oneNT3m >> bidOptionDoc.oneNT3M
        >> bidOptionDoc.structuredReverse >> bidOptionDoc.takeoutDoubles >> bidOptionDoc.negativeDoubles
        >> bidOptionDoc.directCueBid >> bidOptionDoc.unusual2NT >> bidOptionDoc.drury
        >> bidOptionDoc.jumpOvercalls >> bidOptionDoc.threeLevel
        >> bidOptionDoc.elevenHCPsRbsLm >> bidOptionDoc.elevenHCPs6Cs
        >> bidOptionDoc.fourteenTPsGs >> bidOptionDoc.fourteenTPsLs >> bidOptionDoc.thirteenTPsLs
        >> bidOptionDoc.lengthLead;

    for (i = 0; i < 2; i++)
    for (j = 0; j < MAXOPENINGLEADS; j++)
        stream >> bidOptionDoc.openingLead[i][j];

    stream >> bidOptionDoc.partnerLeadSuit >> bidOptionDoc.declarerLeadSuit
        >> bidOptionDoc.discardingSuit >> bidOptionDoc.partnerLeadNT >> bidOptionDoc.declarerLeadNT
        >> bidOptionDoc.discardingNT;

    return stream;
}


