/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CZBridgeDoc.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: ZBridge model (data etc.).

  Platforms: Qt.

*/

/**
 * @file
 * ZBridge model (data etc.).
 */

#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QSettings>

#include "czbridgedoc.h"
#include "ZBridgeException.h"

CZBridgeDoc *CZBridgeDoc::instance = 0;


/**
 * Constructor for the model container.
 * Reads data from permanent storage.
 */
CZBridgeDoc::CZBridgeDoc(QObject *parent) :
    QObject(parent)
{
    ReadSeatOptions();
    seatOptions.role = STANDALONE_ROLE;
    ReadSettings();
    LoadBidOptions();
    ReadDealOptions();
    ReadDisplayOptions();
    ReadGameOptions();
    ReadWizardOptions();
    ReadBidDBFileName();

    LoadBidDB();

    synchronizeOptions(false);
}

/**
 * This static method returns a pointer to the (singleton) data container.
 */
CZBridgeDoc* CZBridgeDoc::Instance()
{
    return instance;
}

/**
 * Synchronize options with wizard generated options.
 *
 * @param[in] all If true then all options else only bid options.
 */
void CZBridgeDoc::synchronizeOptions(bool all)
{
    if (all)
    {
        //Game options.
        gameOptions.helpLevel = wizardOptions.helpLevel;
        gameOptions.enableTracing = wizardOptions.enableTracing;
        gameOptions.scoringMethod = wizardOptions.scoringMethod;
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
        defaultBidOption.blackWood = true;
        defaultBidOption.gerber = true;
        defaultBidOption.cueBids = true;
        defaultBidOption.stayman = STAYMAN_3R_NF;
        defaultBidOption.jacobyTransfers = false;
        defaultBidOption.fourSuitTransfers = false;
        defaultBidOption.limitRaises = true;
        defaultBidOption.splinterbids = false;
        defaultBidOption.takeoutDoubles = true;
        defaultBidOption.negativeDoubles = false;

        //2 bids.
        defaultBidOption.twoBidsMode = STRONG_TWO;
        defaultBidOption.twoClubOpenValues = OPEN_VALUE_22;
        defaultBidOption.twoCtwoD = TWO_C_TWO_D_WAITING;

        //NT range.
        defaultBidOption.oneNTRange = ONE_NT_RANGE_15_17;
        defaultBidOption.twoNTRange = TWO_NT_RANGE_20_21;
        defaultBidOption.threeNTRange = THREE_NT_RANGE_25_27;
        defaultBidOption.gambling3NT = false;

        //NT other.
        defaultBidOption.oneNT3m = ONE_NT_3m_ST;
        defaultBidOption.oneNT3M = ONE_NT_3M_GT;
        defaultBidOption.twoNTAfter11 = TWO_NT_11_18_20;
        defaultBidOption.after1NTRebid = REBID_1NT_NAT;
        defaultBidOption.open1NTWeakMajor = true;
        defaultBidOption.twoNTAfter21 = TWO_NT_21_12_14;

        //Three level;
        defaultBidOption.threeLevelPE = SOUND_THREE_LEVEL;

        //Misc.
        defaultBidOption.jacoby2NT = false;
        defaultBidOption.directCueBid = STRONG_CUE_BID;
        defaultBidOption.unusual2NT = false;
        defaultBidOption.drury = false;
        defaultBidOption.fourSuitForcing = true;
        defaultBidOption.structuredReverse = true;
        defaultBidOption.jumpOvercalls = JUMP_OVERCALL_WEAK;
        defaultBidOption.afterWJO = W2_NATURAL;

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
        defaultBidOption.stayman = STAYMAN_3R_NF;
        defaultBidOption.jacobyTransfers = false;
        defaultBidOption.fourSuitTransfers = false;
        defaultBidOption.limitRaises = false;
        defaultBidOption.splinterbids = false;
        defaultBidOption.takeoutDoubles = true;
        defaultBidOption.negativeDoubles = false;

        //2 bids.
        defaultBidOption.twoBidsMode = STRONG_TWO;
        defaultBidOption.twoClubOpenValues = OPEN_VALUE_22;
        defaultBidOption.twoCtwoD = TWO_C_TWO_D_WAITING;

        //NT range.
        defaultBidOption.oneNTRange = ONE_NT_RANGE_16_18;
        defaultBidOption.twoNTRange = TWO_NT_RANGE_22_24;
        defaultBidOption.threeNTRange = THREE_NT_RANGE_25_27;
        defaultBidOption.gambling3NT = false;

        //NT other.
        defaultBidOption.oneNT3m = ONE_NT_3m_GT;
        defaultBidOption.oneNT3M = ONE_NT_3M_GT;
        defaultBidOption.twoNTAfter11 = TWO_NT_11_18_20;
        defaultBidOption.after1NTRebid = REBID_1NT_NAT;
        defaultBidOption.open1NTWeakMajor = true;
        defaultBidOption.twoNTAfter21 = TWO_NT_21_12_14;

        //Three level;
        defaultBidOption.threeLevelPE = SOUND_THREE_LEVEL;

        //Misc.
        defaultBidOption.jacoby2NT = false;
        defaultBidOption.directCueBid = STRONG_CUE_BID;
        defaultBidOption.unusual2NT = false;
        defaultBidOption.drury = false;
        defaultBidOption.fourSuitForcing = true;
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
        defaultBidOption.partnerLeadSuit = SIGNAL_ATTITUDE_HIGH;
        defaultBidOption.declarerLeadSuit = SIGNAL_NO_SIGNAL;
        defaultBidOption.discardingSuit = SIGNAL_ATTITUDE_HIGH;
        defaultBidOption.partnerLeadNT = SIGNAL_ATTITUDE_HIGH;
        defaultBidOption.declarerLeadNT = SIGNAL_NO_SIGNAL;
        defaultBidOption.discardingNT = SIGNAL_ATTITUDE_HIGH;
    }
    else if (wizardOptions.biddingStyle == STANDARD_AMERICAN_BASIC_BIDDING_STYLE)
    {
        //General.
        defaultBidOption.majorsMode = MAJOR_FIVE;
        defaultBidOption.blackWood = true;
        defaultBidOption.gerber = true;
        defaultBidOption.cueBids = true;
        defaultBidOption.stayman = STAYMAN_3R_NF;
        defaultBidOption.jacobyTransfers = true;
        defaultBidOption.fourSuitTransfers = true;
        defaultBidOption.limitRaises = true;
        defaultBidOption.splinterbids = false;
        defaultBidOption.takeoutDoubles = true;
        defaultBidOption.negativeDoubles = false;

        //2 bids.
        defaultBidOption.twoBidsMode = WEAK_TWO;
        defaultBidOption.twoClubOpenValues = OPEN_VALUE_22;
        defaultBidOption.afterW2 = W2_FEATURE;
        defaultBidOption.w2NewSuit = W2_NEW_SUIT_NF;
        defaultBidOption.twoCtwoD = TWO_C_TWO_D_WAITING;

        //NT range.
        defaultBidOption.oneNTRange = ONE_NT_RANGE_16_18;
        defaultBidOption.twoNTRange = TWO_NT_RANGE_21_22;
        defaultBidOption.threeNTRange = THREE_NT_RANGE_25_27;
        defaultBidOption.gambling3NT = false;

        //NT other.
        defaultBidOption.oneNT3m = ONE_NT_3m_ST;
        defaultBidOption.oneNT3M = ONE_NT_3M_GF;
        defaultBidOption.twoNTAfter11 = TWO_NT_11_17_18;
        defaultBidOption.after1NTRebid = REBID_1NT_NAT;
        defaultBidOption.open1NTWeakMajor = true;
        defaultBidOption.twoNTAfter21 = TWO_NT_21_12_14;

        //Three level;
        defaultBidOption.threeLevelPE = SOUND_THREE_LEVEL;

        //Misc.
        defaultBidOption.jacoby2NT = false;
        defaultBidOption.directCueBid = STRONG_CUE_BID;
        defaultBidOption.unusual2NT = false;
        defaultBidOption.drury = false;
        defaultBidOption.fourSuitForcing = true;
        defaultBidOption.structuredReverse = true;
        defaultBidOption.jumpOvercalls = JUMP_OVERCALL_WEAK;
        defaultBidOption.afterWJO = W2_NATURAL;

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
        defaultBidOption.stayman = STAYMAN_3R_NF;
        defaultBidOption.jacobyTransfers = true;
        defaultBidOption.fourSuitTransfers = false;
        defaultBidOption.limitRaises = true;
        defaultBidOption.splinterbids = false;
        defaultBidOption.takeoutDoubles = true;
        defaultBidOption.negativeDoubles = true;

        //2 bids.
        defaultBidOption.twoBidsMode = WEAK_TWO;
        defaultBidOption.twoClubOpenValues = OPEN_VALUE_22;
        defaultBidOption.afterW2 = W2_FEATURE;
        defaultBidOption.w2NewSuit = W2_NEW_SUIT_F;
        defaultBidOption.twoCtwoD = TWO_C_TWO_D_WAITING;

        //NT range.
        defaultBidOption.oneNTRange = ONE_NT_RANGE_15_17;
        defaultBidOption.twoNTRange = TWO_NT_RANGE_20_21;
        defaultBidOption.threeNTRange = THREE_NT_RANGE_25_27;
        defaultBidOption.gambling3NT = false;

        //NT other.
        defaultBidOption.oneNT3m = ONE_NT_3m_GT;
        defaultBidOption.oneNT3M = ONE_NT_3M_ST;
        defaultBidOption.twoNTAfter11 = TWO_NT_11_17_18;
        defaultBidOption.after1NTRebid = REBID_1NT_NAT;
        defaultBidOption.open1NTWeakMajor = true;
        defaultBidOption.twoNTAfter21 = TWO_NT_21_15_18;

        //Three level;
        defaultBidOption.threeLevelPE = SOUND_THREE_LEVEL;

        //Misc.
        defaultBidOption.jacoby2NT = true;
        defaultBidOption.directCueBid = MICHAELS_CUEBID;
        defaultBidOption.unusual2NT = false;
        defaultBidOption.drury = false;
        defaultBidOption.fourSuitForcing = true;
        defaultBidOption.structuredReverse = true;
        defaultBidOption.jumpOvercalls = JUMP_OVERCALL_WEAK;
        defaultBidOption.afterWJO = W2_FEATURE;

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
        defaultBidOption.stayman = STAYMAN_3R_NF;
        defaultBidOption.jacobyTransfers = false;
        defaultBidOption.fourSuitTransfers = false;
        defaultBidOption.limitRaises = true;
        defaultBidOption.splinterbids = true;
        defaultBidOption.takeoutDoubles = true;
        defaultBidOption.negativeDoubles = false;

        //2 bids.
        defaultBidOption.twoBidsMode = STRONG_TWO;
        defaultBidOption.twoClubOpenValues = OPEN_VALUE_22;
        defaultBidOption.twoCtwoD = TWO_C_TWO_D_WAITING;

        //NT range.
        defaultBidOption.oneNTRange = ONE_NT_RANGE_12_14;
        defaultBidOption.twoNTRange = TWO_NT_RANGE_20_21;
        defaultBidOption.threeNTRange = GAMBLING_3_NT_ACOL;
        defaultBidOption.gambling3NT = true;

        //NT other.
        defaultBidOption.oneNT3m = ONE_NT_3m_ST;
        defaultBidOption.oneNT3M = ONE_NT_3M_GT;
        defaultBidOption.twoNTAfter11 = TWO_NT_11_17_18;
        defaultBidOption.after1NTRebid = REBID_1NT_NAT;
        defaultBidOption.open1NTWeakMajor = true;
        defaultBidOption.twoNTAfter21 = TWO_NT_21_15_18;

        //Three level;
        defaultBidOption.threeLevelPE = SOUND_THREE_LEVEL;

        //Misc.
        defaultBidOption.jacoby2NT = false;
        defaultBidOption.directCueBid = STRONG_CUE_BID;
        defaultBidOption.unusual2NT = false;
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

    //Update permanent storage.
    WriteGameOptions();
    WriteDisplayOptions();
}

/**
 * @brief Get current North/South bid options.
 *
 * @return Current North/South bid options.
 */
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

/**
 * @brief Get Current East/West bid options.
 *
 * @return Current East/West bid options.
 */
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

/**
 * @brief Write seat options to permanent storage.
 */
void CZBridgeDoc::WriteSeatOptions()
{
    seatOptions.WriteSettings();
}

/**
 * @brief Read seat options from permanent storage.
 */
void CZBridgeDoc::ReadSeatOptions()
{
    seatOptions.ReadSettings();
}

/**
 * @brief Write current convention settings to permanent storage.
 */
void CZBridgeDoc::WriteSettings()
{
    QSettings settings("ZBridge settings", "Conventions");

    settings.setValue("nsConvention", curBidOption[NSBIDOPTIONSINDEX]);
    settings.setValue("ewConvention", curBidOption[EWBIDOPTIONSINDEX]);
}

/**
 * @brief Read current convention settings from permanent storage.
 */
void CZBridgeDoc::ReadSettings()
{
    QSettings settings("ZBridge settings", "Conventions");

    curBidOption[NSBIDOPTIONSINDEX] = settings.value("nsConvention","").toString();
    curBidOption[EWBIDOPTIONSINDEX] = settings.value("ewConvention","").toString();
}

/**
 * @brief Write bid options to permanent storage.
 */
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

/**
 * @brief Read bid options from permanent storage.
 */
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

/**
 * @brief Write deal options to permanent storage
 */
void CZBridgeDoc::WriteDealOptions()
{
    dealOptions.WriteSettings();
}

/**
 * @brief Read deal options from permanent storage
 */
void CZBridgeDoc::ReadDealOptions()
{
    dealOptions.ReadSettings();
}

/**
 * @brief Write display options to permanent storage
 */
void CZBridgeDoc::WriteDisplayOptions()
{
    displayOptions.WriteSettings();
}

/**
 * @brief Read display options from permanent storage
 */
void CZBridgeDoc::ReadDisplayOptions()
{
    displayOptions.ReadSettings();
}

/**
 * @brief Write game options to permanent storage
 */
void CZBridgeDoc::WriteGameOptions()
{
    gameOptions.WriteSettings();
}

/**
 * @brief Read game options from permanent storage
 */
void CZBridgeDoc::ReadGameOptions()
{
    gameOptions.ReadSettings();
}

/**
 * @brief Write wizard options to permanent storage
 */
void CZBridgeDoc::WriteWizardOptions()
{
    wizardOptions.WriteSettings();
}

/**
 * @brief Read wizard options from permanent storage
 */
void CZBridgeDoc::ReadWizardOptions()
{
    wizardOptions.ReadSettings();
}

/**
 * @brief Read bid database file name from permanent storage.
 */
void CZBridgeDoc::WriteBidDBFileName()
{
    QSettings settings("ZBridge settings", "div");

    settings.setValue("bidDBFileName", bidDBFileName);
}

/**
 * @brief Write bid database file name to permanent storage.
 */
void CZBridgeDoc::ReadBidDBFileName()
{
    QSettings settings("ZBridge settings", "div");

    bidDBFileName = settings.value("bidDBFileName","").toString();
}

/**
 * @brief Write bid database to permanent storage.
 */
void CZBridgeDoc::SaveBidDB()
{
    try
    {
        QFile file(bidDBFileName);
        if (file.open(QIODevice::WriteOnly))
        {
            QDataStream out(&file);
            out << bidDB;
            file.close();
            if (out.status() != QDataStream::Ok)
                throw PlayException(tr("Bid DB: Write of bid DB failed").toStdString());
        }
        QString descFilename = bidDBFileName.left(bidDBFileName.indexOf(".bsf", 0, Qt::CaseInsensitive)) + ".dsc";
        QFile descFile(descFilename);
        if (descFile.open(QIODevice::WriteOnly))
        {
            QDataStream out(&descFile);
            out << bidDesc;
            descFile.close();
            if (out.status() != QDataStream::Ok)
                throw PlayException(tr("Bid DB: Write of bid description failed").toStdString());
        }
    }
    catch (PlayException &e)
    {
        QMessageBox::critical(0, tr("ZBridge"), e.what());
    }
}

/**
 * @brief Read bid database from permanent storage.
 */
void CZBridgeDoc::LoadBidDB()
{
    if (bidDBFileName.size() == 0)
        return;

    try
    {
        bidDB.clearBidDB();
        bidDesc.clearBidDesc();

        QFile file(bidDBFileName);
        if (file.open(QIODevice::ReadOnly))
        {
            QDataStream in(&file);
            in >> bidDB;
            file.close();
            if (in.status() != QDataStream::Ok)
                throw PlayException(tr("Bid DB: Corrupt or version not supported or not a bid database").toStdString());
        }

        //Open file with text description of pages etc. in the bid database.
        QString descFilename = bidDBFileName.left(bidDBFileName.indexOf(".bsf", 0, Qt::CaseInsensitive)) + ".dsc";
        QFile descFile(descFilename);
        if (descFile.open(QIODevice::ReadOnly))
        {
            QDataStream in(&descFile);
            in >> bidDesc;
            descFile.close();
            if (in.status() != QDataStream::Ok)
                throw PlayException(tr("Bid DB: Corrupt or version not supported or not a bid description").toStdString());
        }
    }
    catch (PlayException &e)
    {
        bidDB.clearBidDB();
        bidDesc.clearBidDesc();

        QMessageBox::critical(0, tr("ZBridge"), e.what());
    }
}

/**
 * @brief CZBridgeDoc::setFileComments
 * @param fileComments
 */
void CZBridgeDoc::setFileComments(QString &fileComments)
{
}

/**
 * @brief CZBridgeDoc::getFileComments
 * @return
 */
QString &CZBridgeDoc::getFileComments()
{
    QString *txt = new QString("text");
    return *txt;
}

/**
 * @brief CZBridgeDoc::setShowCommentsUponOpen
 * @param autoShow
 */
void CZBridgeDoc::setShowCommentsUponOpen(bool autoShow)
{
}

/**
 * @brief CZBridgeDoc::getShowCommentsUponOpen
 * @return
 */
bool CZBridgeDoc::getShowCommentsUponOpen()
{
    return false;
}
