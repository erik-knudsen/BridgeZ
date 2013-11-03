/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CZBridgeDoc.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: ZBridge mode (data etc.).

  Platforms: Qt.

*/

/**
 * \file
 * ZBridge model (data etc.) (definition).
 */

#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QSettings>

#include "czbridgedoc.h"

CZBridgeDoc *CZBridgeDoc::instance = 0;

CZBridgeDoc::CZBridgeDoc(QObject *parent) :
    QObject(parent)
{
    ReadSeatOptions();
    seatOptions.role = SERVER_ROLE;
    ReadSettings();
    LoadBidOptions();
    ReadDealOptions();
    ReadDisplayOptions();
    ReadGameOptions();
    ReadWizardOptions();

    synchronizeOptions(false);
}

CZBridgeDoc* CZBridgeDoc::Instance()
{
    return instance;
}

void CZBridgeDoc::synchronizeOptions(bool all)
{
    if (all)
    {
        //Game options.
        gameOptions.helpLevel = wizardOptions.helpLevel;
        gameOptions.enableTracing = wizardOptions.enableTracing;
        gameOptions.useDuplicateScoring = wizardOptions.useDuplicateScoring;
        gameOptions.scoreHonorsBonuses = wizardOptions.scoreHonorsBonuses;
        gameOptions.insertBiddingPause = wizardOptions.insertBiddingPause;
        gameOptions.biddingPause = wizardOptions.biddingPauseLength;
        gameOptions.insertPlayPause = wizardOptions.insertPlayPause;
        gameOptions.playPause = wizardOptions.playPauseLenght;

        //Display options.
        displayOptions.autoShowBidHistory = wizardOptions.autoShowBidHistory;
        displayOptions.autoShowPlayHistory = wizardOptions.autoShowPlayHistory;
        displayOptions.useSuitSymbols = wizardOptions.useSuitSymbols;
        displayOptions.suitSequence = wizardOptions.suitSequence;
        displayOptions.showDummyTrumpsOnLeft = wizardOptions.showDummyTrumpsOnLeft;
    }
    //Bidding style.
    if (wizardOptions.biddingStyle == NATURAL_BIDDING_STYLE)
    {
        //General.
        defaultBidOption.majorsMode = MAJOR_FOUR;
        defaultBidOption.blackWood = false;
        defaultBidOption.gerber = false;
        defaultBidOption.cueBids = false;
        defaultBidOption.stayman = false;
        defaultBidOption.jacobyTransfers = false;
        defaultBidOption.fourSuitTransfers = false;
        defaultBidOption.limitRaises = false;
        defaultBidOption.splinterbids = false;
        defaultBidOption.takeoutDoubles = false;
        defaultBidOption.negativeDoubles = false;

        //2 bids.
        defaultBidOption.twoBidsMode = STRONG_TWO;
        defaultBidOption.twoNTafterWeak2 = 0;
        defaultBidOption.twoClubOpenValues = OPEN_VALUE_22;

        //NT range.
        defaultBidOption.oneNTRange = ONE_NT_RANGE_15_17;
        defaultBidOption.twoNTRange = TWO_NT_RANGE_20_21;
        defaultBidOption.threeNTRange = THREE_NT_RANGE_25_27;
        defaultBidOption.gambling3NT = false;

        //Misc.
        defaultBidOption.jacoby2NT = false;
        defaultBidOption.michaelsCueBid = false;
        defaultBidOption.unusual2NT = false;
        defaultBidOption.drury = false;
        defaultBidOption.fourSuitForcing = false;
        defaultBidOption.structuredReverse = false;
        defaultBidOption.jumpOvercalls = JUMP_OVERCALL_STRONG;

        //Opening.
        defaultBidOption.elevenHCPsRbsLm = false;
        defaultBidOption.elevenHCPs6Cs = false;
        defaultBidOption.fourteenTPsGs = false;
        defaultBidOption.fourteenTPsLs = false;
        defaultBidOption.thirteenTPsLs = false;

        //Leads.
        defaultBidOption.lengthLead = LOW_ENCOURAGING;

        //Signals.
        defaultBidOption.partnerLeadSuit = SIGNAL_ATTITUDE_LOW;
        defaultBidOption.declarerLeadSuit = SIGNAL_NO_SIGNAL;
        defaultBidOption.discardingSuit = SIGNAL_ATTITUDE_LOW;
        defaultBidOption.partnerLeadNT = SIGNAL_ATTITUDE_LOW;
        defaultBidOption.declarerLeadNT = SIGNAL_NO_SIGNAL;
        defaultBidOption.discardingNT = SIGNAL_ATTITUDE_LOW;
    }
    else if (wizardOptions.biddingStyle == GOREN_BIDDING_STYLE)
    {
        //General.
        defaultBidOption.majorsMode = MAJOR_FOUR;
        defaultBidOption.blackWood = true;
        defaultBidOption.gerber = true;
        defaultBidOption.cueBids = true;
        defaultBidOption.stayman = true;
        defaultBidOption.jacobyTransfers = false;
        defaultBidOption.fourSuitTransfers = false;
        defaultBidOption.limitRaises = false;
        defaultBidOption.splinterbids = false;
        defaultBidOption.takeoutDoubles = true;
        defaultBidOption.negativeDoubles = false;

        //2 bids.
        defaultBidOption.twoBidsMode = STRONG_TWO;
        defaultBidOption.twoNTafterWeak2 = 0;
        defaultBidOption.twoClubOpenValues = OPEN_VALUE_22;

        //NT range.
        defaultBidOption.oneNTRange = ONE_NT_RANGE_15_17;
        defaultBidOption.twoNTRange = TWO_NT_RANGE_20_21;
        defaultBidOption.threeNTRange = THREE_NT_RANGE_25_27;
        defaultBidOption.gambling3NT = false;

        //Misc.
        defaultBidOption.jacoby2NT = false;
        defaultBidOption.michaelsCueBid = false;
        defaultBidOption.unusual2NT = false;
        defaultBidOption.drury = false;
        defaultBidOption.fourSuitForcing = false;
        defaultBidOption.structuredReverse = true;
        defaultBidOption.jumpOvercalls = JUMP_OVERCALL_STRONG;

        //Opening.
        defaultBidOption.elevenHCPsRbsLm = false;
        defaultBidOption.elevenHCPs6Cs = false;
        defaultBidOption.fourteenTPsGs = false;
        defaultBidOption.fourteenTPsLs = false;
        defaultBidOption.thirteenTPsLs = false;

        //Leads.
        defaultBidOption.lengthLead = LOW_ENCOURAGING;

        //Signals.
        defaultBidOption.partnerLeadSuit = SIGNAL_ATTITUDE_LOW;
        defaultBidOption.declarerLeadSuit = SIGNAL_NO_SIGNAL;
        defaultBidOption.discardingSuit = SIGNAL_ATTITUDE_LOW;
        defaultBidOption.partnerLeadNT = SIGNAL_ATTITUDE_LOW;
        defaultBidOption.declarerLeadNT = SIGNAL_NO_SIGNAL;
        defaultBidOption.discardingNT = SIGNAL_ATTITUDE_LOW;
    }
    else if (wizardOptions.biddingStyle == STANDARD_AMERICAN_BASIC_BIDDING_STYLE)
    {
        //General.
        defaultBidOption.majorsMode = MAJOR_FIVE;
        defaultBidOption.blackWood = true;
        defaultBidOption.gerber = true;
        defaultBidOption.cueBids = true;
        defaultBidOption.stayman = true;
        defaultBidOption.jacobyTransfers = true;
        defaultBidOption.fourSuitTransfers = false;
        defaultBidOption.limitRaises = true;
        defaultBidOption.splinterbids = false;
        defaultBidOption.takeoutDoubles = true;
        defaultBidOption.negativeDoubles = true;

        //2 bids.
        defaultBidOption.twoBidsMode = WEAK_TWO;
        defaultBidOption.twoNTafterWeak2 = 0;
        defaultBidOption.twoClubOpenValues = OPEN_VALUE_22;

        //NT range.
        defaultBidOption.oneNTRange = ONE_NT_RANGE_15_17;
        defaultBidOption.twoNTRange = TWO_NT_RANGE_20_21;
        defaultBidOption.threeNTRange = THREE_NT_RANGE_25_27;
        defaultBidOption.gambling3NT = false;

        //Misc.
        defaultBidOption.jacoby2NT = false;
        defaultBidOption.michaelsCueBid = false;
        defaultBidOption.unusual2NT = false;
        defaultBidOption.drury = false;
        defaultBidOption.fourSuitForcing = false;
        defaultBidOption.structuredReverse = true;
        defaultBidOption.jumpOvercalls = JUMP_OVERCALL_WEAK;

        //Opening.
        defaultBidOption.elevenHCPsRbsLm = false;
        defaultBidOption.elevenHCPs6Cs = false;
        defaultBidOption.fourteenTPsGs = false;
        defaultBidOption.fourteenTPsLs = false;
        defaultBidOption.thirteenTPsLs = false;

        //Leads.
        defaultBidOption.lengthLead = FOURTH_BEST;

        //Signals.
        defaultBidOption.partnerLeadSuit = SIGNAL_ATTITUDE_HIGH;
        defaultBidOption.declarerLeadSuit = SIGNAL_COUNT_HIGH;
        defaultBidOption.discardingSuit = SIGNAL_ATTITUDE_HIGH;
        defaultBidOption.partnerLeadNT = SIGNAL_ATTITUDE_HIGH;
        defaultBidOption.declarerLeadNT = SIGNAL_COUNT_HIGH;
        defaultBidOption.discardingNT = SIGNAL_ATTITUDE_HIGH;
    }
    else if (wizardOptions.biddingStyle == STANDARD_AMERICAN_YELLOW_CARD_BIDDING_STYLE)
    {
        //General.
        defaultBidOption.majorsMode = MAJOR_FIVE;
        defaultBidOption.blackWood = true;
        defaultBidOption.gerber = true;
        defaultBidOption.cueBids = true;
        defaultBidOption.stayman = true;
        defaultBidOption.jacobyTransfers = true;
        defaultBidOption.fourSuitTransfers = true;
        defaultBidOption.limitRaises = true;
        defaultBidOption.splinterbids = false;
        defaultBidOption.takeoutDoubles = true;
        defaultBidOption.negativeDoubles = true;

        //2 bids.
        defaultBidOption.twoBidsMode = WEAK_TWO;
        defaultBidOption.twoNTafterWeak2 = 0;
        defaultBidOption.twoClubOpenValues = OPEN_VALUE_22;

        //NT range.
        defaultBidOption.oneNTRange = ONE_NT_RANGE_15_17;
        defaultBidOption.twoNTRange = TWO_NT_RANGE_20_21;
        defaultBidOption.threeNTRange = THREE_NT_RANGE_25_27;
        defaultBidOption.gambling3NT = false;

        //Misc.
        defaultBidOption.jacoby2NT = true;
        defaultBidOption.michaelsCueBid = true;
        defaultBidOption.unusual2NT = true;
        defaultBidOption.drury = false;
        defaultBidOption.fourSuitForcing = true;
        defaultBidOption.structuredReverse = true;
        defaultBidOption.jumpOvercalls = JUMP_OVERCALL_WEAK;

        //Opening.
        defaultBidOption.elevenHCPsRbsLm = false;
        defaultBidOption.elevenHCPs6Cs = false;
        defaultBidOption.fourteenTPsGs = false;
        defaultBidOption.fourteenTPsLs = false;
        defaultBidOption.thirteenTPsLs = false;

        //Leads.
        defaultBidOption.lengthLead = FOURTH_BEST;

        //Signals.
        defaultBidOption.partnerLeadSuit = SIGNAL_ATTITUDE_HIGH;
        defaultBidOption.declarerLeadSuit = SIGNAL_COUNT_HIGH;
        defaultBidOption.discardingSuit = SIGNAL_ATTITUDE_HIGH;
        defaultBidOption.partnerLeadNT = SIGNAL_ATTITUDE_HIGH;
        defaultBidOption.declarerLeadNT = SIGNAL_COUNT_HIGH;
        defaultBidOption.discardingNT = SIGNAL_ATTITUDE_HIGH;
    }
    else if (wizardOptions.biddingStyle == ACOL_BIDDING_STYLE)
    {
        //General.
        defaultBidOption.majorsMode = MAJOR_FOUR;
        defaultBidOption.blackWood = true;
        defaultBidOption.gerber = true;
        defaultBidOption.cueBids = true;
        defaultBidOption.stayman = true;
        defaultBidOption.jacobyTransfers = false;
        defaultBidOption.fourSuitTransfers = false;
        defaultBidOption.limitRaises = false;
        defaultBidOption.splinterbids = true;
        defaultBidOption.takeoutDoubles = true;
        defaultBidOption.negativeDoubles = false;

        //2 bids.
        defaultBidOption.twoBidsMode = STRONG_TWO;
        defaultBidOption.twoNTafterWeak2 = 0;
        defaultBidOption.twoClubOpenValues = OPEN_VALUE_22;

        //NT range.
        defaultBidOption.oneNTRange = ONE_NT_RANGE_12_14;
        defaultBidOption.twoNTRange = TWO_NT_RANGE_20_21;
        defaultBidOption.threeNTRange = THREE_NT_RANGE_25_27;
        defaultBidOption.gambling3NT = true;

        //Misc.
        defaultBidOption.jacoby2NT = false;
        defaultBidOption.michaelsCueBid = true;
        defaultBidOption.unusual2NT = true;
        defaultBidOption.drury = false;
        defaultBidOption.fourSuitForcing = true;
        defaultBidOption.structuredReverse = false;
        defaultBidOption.jumpOvercalls = JUMP_OVERCALL_STRONG;

        //Opening.
        defaultBidOption.elevenHCPsRbsLm = false;
        defaultBidOption.elevenHCPs6Cs = false;
        defaultBidOption.fourteenTPsGs = false;
        defaultBidOption.fourteenTPsLs = false;
        defaultBidOption.thirteenTPsLs = false;

        //Leads.
        defaultBidOption.lengthLead = FOURTH_BEST;

        //Signals.
        defaultBidOption.partnerLeadSuit = SIGNAL_ATTITUDE_LOW;
        defaultBidOption.declarerLeadSuit = SIGNAL_COUNT_LOW;
        defaultBidOption.discardingSuit = SIGNAL_ATTITUDE_LOW;
        defaultBidOption.partnerLeadNT = SIGNAL_ATTITUDE_LOW;
        defaultBidOption.declarerLeadNT = SIGNAL_COUNT_LOW;
        defaultBidOption.discardingNT = SIGNAL_ATTITUDE_LOW;
    }

    //Update.
    WriteGameOptions();
    WriteDisplayOptions();
}

CBidOptionDoc &CZBridgeDoc::getNSBidOptions()
{
    if (bidOptions.empty())
        return defaultBidOption;

    int i;

    for (i = 0; i < bidOptions.size(); i++)
        if (QString::compare(bidOptions[i].name, curBidOption[NSBIDOPTIONSINDEX]) == 0)
            break;

    if (i == bidOptions.size())
        i = 0;

    return bidOptions[i];
}

CBidOptionDoc &CZBridgeDoc::getEWBidOptions()
{
    if (bidOptions.empty())
        return defaultBidOption;

    int i;

    for (i = 0; i < bidOptions.size(); i++)
        if (QString::compare(bidOptions[i].name, curBidOption[EWBIDOPTIONSINDEX]) == 0)
            break;

    if (i == bidOptions.size())
        i = 0;

    return bidOptions[i];
}

void CZBridgeDoc::WriteSeatOptions()
{
    seatOptions.WriteSettings();
}

void CZBridgeDoc::ReadSeatOptions()
{
    seatOptions.ReadSettings();
}

void CZBridgeDoc::WriteSettings()
{
    QSettings settings("ZBridge settings", "Conventions");

    settings.setValue("nsConvention", curBidOption[NSBIDOPTIONSINDEX]);
    settings.setValue("ewConvention", curBidOption[EWBIDOPTIONSINDEX]);
}

void CZBridgeDoc::ReadSettings()
{
    QSettings settings("ZBridge settings", "Conventions");

    curBidOption[NSBIDOPTIONSINDEX] = settings.value("nsConvention","").toString();
    curBidOption[EWBIDOPTIONSINDEX] = settings.value("ewConvention","").toString();
}

void CZBridgeDoc::SaveBidOptions()
{
    QFile file(BIDOPTIONFILENAME);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << bidOptions;
    if (out.status() != QDataStream::Ok)
        QMessageBox::warning(0, tr("ZBridge"), tr("Bid options could not be saved."));
    file.close();
}

void CZBridgeDoc::LoadBidOptions()
{
    QFile file(BIDOPTIONFILENAME);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> bidOptions;
    if (in.status() != QDataStream::Ok)
        QMessageBox::warning(0, tr("ZBridge"), tr("Bid options could not be loaded.\n"
                                                  "Defaults will be used."));
    file.close();
}

void CZBridgeDoc::WriteDealOptions()
{
    dealOptions.WriteSettings();
}

void CZBridgeDoc::ReadDealOptions()
{
    dealOptions.ReadSettings();
}

void CZBridgeDoc::WriteDisplayOptions()
{
    displayOptions.WriteSettings();
}

void CZBridgeDoc::ReadDisplayOptions()
{
    displayOptions.ReadSettings();
}

void CZBridgeDoc::WriteGameOptions()
{
    gameOptions.WriteSettings();
}

void CZBridgeDoc::ReadGameOptions()
{
    gameOptions.ReadSettings();
}

void CZBridgeDoc::WriteWizardOptions()
{
    wizardOptions.WriteSettings();
}

void CZBridgeDoc::ReadWizardOptions()
{
    wizardOptions.ReadSettings();
}

void CZBridgeDoc::setFileComments(QString &fileComments)
{
}

QString &CZBridgeDoc::getFileComments()
{
    QString *txt = new QString("text");
    return *txt;
}

void CZBridgeDoc::setShowCommentsUponOpen(bool autoShow)
{
}

bool CZBridgeDoc::getShowCommentsUponOpen()
{
    return false;
}
