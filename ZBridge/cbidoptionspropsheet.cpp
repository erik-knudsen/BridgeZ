/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidOptionsPropSheet.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Property sheet for selecting bid options.

  Platforms: Qt.

*/

/**
 * \file
 * Property sheet for selecting bid options.
 */

#include <QList>

#include "cbidoptionspropsheet.h"
#include "ui_cbidoptionspropsheet.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"
#include "cbidoptiondoc.h"

CBidOptionsPropSheet::CBidOptionsPropSheet(CBidOptionDoc &bidOptionDoc, CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBidOptionsPropSheet)
{
    QStringList list;

    ui->setupUi(this);
    this->app = app;
    this->doc = doc;
    this->bidOptionDoc = bidOptionDoc;

    list << tr("No signals") << tr("Count: high = even") << tr("Count: low = even")
            << tr("Attitude: high = positive") << tr("Attitude: low = positive");
    ui->signalPartnerLeadSuit->addItems(list);
    ui->signalDiscardingSuit->addItems(list);
    ui->signalPartnerLeadNT->addItems(list);
    ui->signalDiscardingNT->addItems(list);
    list.clear();
    list << tr("No signals") << tr("Count: high = even") << tr("Count: low = even");
    ui->signalDeclarerLeadSuit->addItems(list);
    ui->signalDeclarerLeadNT->addItems(list);

    //Set initial values.

    //Config.
    ui->biddingAggressiveness->setValue(bidOptionDoc.aggressiveness);

    //General.
    if (bidOptionDoc.bidStyle == EUROPEAN_STYLE)
        ui->europeanStyle->setChecked(true);
    else
        ui->americanStyle->setChecked(true);

    if (bidOptionDoc.majorsMode == MAJOR_FOUR)
        ui->majorFour->setChecked(true);
    else
        ui->majorFive->setChecked(true);

    ui->blackwood->setChecked(true);
    ui->gerber->setChecked(true);
    ui->cueBids->setChecked(true);
    ui->stayman->setChecked(true);
    if (bidOptionDoc.stayman == STAYMAN_3R_NF)
        ui->staymanNF->setChecked(true);
    else if (bidOptionDoc.stayman == STAYMAN_3R_F)
        ui->staymanF->setChecked(true);
    ui->jacobyTransfers->setChecked(bidOptionDoc.jacobyTransfers);
    if (bidOptionDoc.jacobyTransfers)
        ui->fourSuitTransfers->setChecked(bidOptionDoc.fourSuitTransfers);

    setDependencies();

    //2-Bids.
    if (bidOptionDoc.twoBidsMode == S2_NATURAL)
        ui->weakTwo->setChecked(false);
    else
    {
        ui->weakTwo->setChecked(true);
        if (bidOptionDoc.twoBidsMode== W2_OGUST)
            ui->w2Ogust->setChecked(true);
        else if (bidOptionDoc.twoBidsMode== W2_FEATURE)
            ui->w2Feature->setChecked(true);
        if (bidOptionDoc.w2NewSuit == W2_NEW_SUIT_NF)
            ui->w2NewSuitNF->setChecked(true);
        else if (bidOptionDoc.w2NewSuit == W2_NEW_SUIT_F)
            ui->w2NewSuitF->setChecked(true);
    }

    if (bidOptionDoc.twoClubOpenValues == OPEN_VALUE_20)
        ui->openValue20->setChecked(true);
    else if (bidOptionDoc.twoClubOpenValues == OPEN_VALUE_21)
        ui->openValue21->setChecked(true);
    else if (bidOptionDoc.twoClubOpenValues == OPEN_VALUE_22)
        ui->openValue22->setChecked(true);
    else
        ui->openValue23->setChecked(true);
    ui->openValue22->setChecked(true);
    ui->open2CBox->setEnabled(false);

    if (bidOptionDoc.twoCtwoD == TWO_C_TWO_D_NEGATIVE)
        ui->twoCtwoDNegative->setChecked(true);
    else if (bidOptionDoc.twoCtwoD == TWO_C_TWO_D_WAITING)
        ui->twoCtwoDWaiting->setChecked(true);

    //NT-Range.
    if (bidOptionDoc.oneNTRange == ONE_NT_RANGE_12_14)
    {
        ui->oneNTRange12_14->setChecked(true);
        ui->twoNT21->setEnabled(false);
    }
    else if (bidOptionDoc.oneNTRange == ONE_NT_RANGE_15_17)
        ui->oneNTRange15_17->setChecked(true);
    else if (bidOptionDoc.oneNTRange == ONE_NT_RANGE_16_18)
        ui->oneNTRange16_18->setChecked(true);
    ui->open1NTWeakMajor->setChecked(bidOptionDoc.open1NTWeakMajor);

    if (bidOptionDoc.twoNTRange == TWO_NT_RANGE_20_21)
        ui->twoNTRange20_21->setChecked(true);
    else if (bidOptionDoc.twoNTRange == TWO_NT_RANGE_21_22)
        ui->twoNTRange21_22->setChecked(true);
    else if (bidOptionDoc.twoNTRange == TWO_NT_RANGE_22_24)
        ui->twoNTRange22_24->setChecked(true);

    if (bidOptionDoc.threeNTRange == THREE_NT_RANGE_25_27)
        ui->threNTRange25_27->setChecked(true);
    else if (bidOptionDoc.threeNTRange == GAMBLING_3_NT)
        ui->gambling3NT->setChecked(true);
    else  if (bidOptionDoc.threeNTRange == GAMBLING_3_NT_ACOL)
        ui->gambling3NT_ACOL->setChecked(true);

    //NT Other.
    if (bidOptionDoc.twoNT11 == TWO_NT_11_18_20)
        ui->twoNT111820->setChecked(true);
    else if (bidOptionDoc.twoNT11 == TWO_NT_11_17_18)
        ui->twoNT111718->setChecked(true);

    if ((bidOptionDoc.oneNTRange ==ONE_NT_RANGE_12_14) || (bidOptionDoc.twoNT21 == TWO_NT_21_15_18))
        ui->twoNT211518->setChecked(true);
    else if (bidOptionDoc.twoNT21 == TWO_NT_21_12_14)
        ui->twoNT211214->setChecked(true);

    if (bidOptionDoc.rebid1NT == REBID_1NT_NAT)
        ui->rebid1NTNat->setChecked(true);
    else if (bidOptionDoc.rebid1NT == REBID_1NT_F)
        ui->rebid1NTF->setChecked(true);

    if (bidOptionDoc.oneNT3m == ONE_NT_3m_SI)
        ui->oneNT3mSI->setChecked(true);
    else if (bidOptionDoc.oneNT3m == ONE_NT_3M_GI)
        ui->oneNT3mGI->setChecked(true);
    else if (bidOptionDoc.oneNT3m == ONE_NT_3m_PE)
        ui->oneNT3mPE->setChecked(true);

    if (bidOptionDoc.oneNT3M == ONE_NT_3M_SI)
        ui->oneNT3MSI->setChecked(true);
    if (bidOptionDoc.oneNT3M == ONE_NT_3M_GF)
        ui->oneNT3MGF->setChecked(true);
    if (bidOptionDoc.oneNT3M == ONE_NT_3M_GI)
        ui->oneNT3MGI->setChecked(true);
    if (bidOptionDoc.oneNT3M == ONE_NT_3M_PE)
        ui->oneNT3MPE->setChecked(true);

    //Misc.
    ui->structuredReverses->setChecked(bidOptionDoc.structuredReverse);
    ui->takeOutDoubles->setChecked(bidOptionDoc.takeoutDoubles);
    ui->negativeDoubles->setChecked(bidOptionDoc.negativeDoubles);
    ui->michaelsCueBid->setChecked(bidOptionDoc.directCueBid == MICHAELS_CUEBID);
    ui->unusualNT->setChecked(bidOptionDoc.unusual2NT);
    ui->drury->setChecked(bidOptionDoc.drury);
    ui->unusualNT->setChecked(false);
    ui->unusualNT->setEnabled(false);
    ui->drury->setChecked(false);
    ui->drury->setEnabled(false);

    if (bidOptionDoc.jumpOvercalls == JUMP_OVERCALL_STRONG)
        ui->jumpOvercallWeak->setChecked(false);
    else
    {
        ui->jumpOvercallWeak->setChecked(true);
        if (bidOptionDoc.jumpOvercalls == JUMP_OVERCALL_WEAK_NATURAL)
            ui->wjoNatural->setChecked(true);
        else if (bidOptionDoc.jumpOvercalls == JUMP_OVERCALL_WEAK_OGUST)
            ui->wjoOgust->setChecked(true);
        else if (bidOptionDoc.jumpOvercalls == JUMP_OVERCALL_WEAK_FEATURE)
            ui->wjoFeature->setChecked(true);
    }

    if (bidOptionDoc.threeLevel ==SOUND_THREE_LEVEL)
        ui->soundThreeLevel->setChecked(true);
    else if (bidOptionDoc.threeLevel ==LIGHT_THREE_LEVEL)
        ui->lightThreeLevel->setChecked(true);
    else if (bidOptionDoc.threeLevel ==LIGHT_THREE_LEVEL_NV)
        ui->lightThreeLevelNV->setChecked(true);

    //Opening.
    ui->elevenHCPsRbsLm->setChecked(bidOptionDoc.elevenHCPsRbsLm);
    ui->elevenHCPs6Cs->setChecked(bidOptionDoc.elevenHCPs6Cs);
    ui->fourteenTPsGs->setChecked(bidOptionDoc.fourteenTPsGs);
    ui->fourteenTPsLs->setChecked(bidOptionDoc.fourteenTPsLs);
    ui->thirteeTPsLs->setChecked(bidOptionDoc.thirteenTPsLs);
    ui->openBox->setEnabled(false);

    //Signals.
    ui->signalPartnerLeadSuit->setCurrentIndex(bidOptionDoc.partnerLeadSuit);
    ui->signalDeclarerLeadSuit->setCurrentIndex(bidOptionDoc.declarerLeadNT);
    ui->signalDiscardingSuit->setCurrentIndex(bidOptionDoc.discardingSuit);
    ui->signalPartnerLeadNT->setCurrentIndex(bidOptionDoc.partnerLeadNT);
    ui->signalDeclarerLeadNT->setCurrentIndex(bidOptionDoc.declarerLeadNT);
    ui->signalDiscardingNT->setCurrentIndex(bidOptionDoc.discardingNT);

    //Leads.
    if (bidOptionDoc.lengthLead == FOURTH_BEST)
        ui->fourthBest->setChecked(true);
    else if (bidOptionDoc.lengthLead == THIRD_FIFTH_BEST)
        ui->thirdBest->setChecked(true);
    else
        ui->lowEncouraging->setChecked(true);

    //Opening leads vs. Suit Contracts.
    if (bidOptionDoc.openingLead[OPEN_SUIT][AK_INX] == Ak_VAL)
        ui->Aks->setChecked(true);
    else
        ui->aKs->setChecked(true);

    if (bidOptionDoc.openingLead[OPEN_SUIT][XXX_INX] == Xxx_VAL)
        ui->Xxxs->setChecked(true);
    else if (bidOptionDoc.openingLead[OPEN_SUIT][XXX_INX] == xXx_VAL)
        ui->xXxs->setChecked(true);
    else
        ui->xxXs->setChecked(true);

    //Opening leads vs. NT Contracts.
    if (bidOptionDoc.openingLead[OPEN_NT][AK_INX] == Ak_VAL)
        ui->Aknt->setChecked(true);
    else
        ui->aKnt->setChecked(true);

    if (bidOptionDoc.openingLead[OPEN_NT][XXX_INX] == Xxx_VAL)
        ui->Xxxnt->setChecked(true);
    else if (bidOptionDoc.openingLead[OPEN_NT][XXX_INX] == xXx_VAL)
        ui->xXxnt->setChecked(true);
    else
        ui->xxXnt->setChecked(true);
}

CBidOptionsPropSheet::~CBidOptionsPropSheet()
{
    delete ui;
}

void CBidOptionsPropSheet::on_biddingAggressiveness_valueChanged(int value)
{
    bidOptionDoc.aggressiveness = value;
}

void CBidOptionsPropSheet::on_americanStyle_clicked()
{
    bidOptionDoc.bidStyle = AMERICAN_STYLE;

    setDependencies();
}

void CBidOptionsPropSheet::on_europeanStyle_clicked()
{
    bidOptionDoc.bidStyle = EUROPEAN_STYLE;

    setDependencies();
}

void CBidOptionsPropSheet::on_majorFour_clicked()
{
    bidOptionDoc.majorsMode = MAJOR_FOUR;

    setDependencies();
}

void CBidOptionsPropSheet::on_majorFive_clicked()
{
    bidOptionDoc.majorsMode = MAJOR_FIVE;

    setDependencies();
}

void CBidOptionsPropSheet::on_blackwood_clicked(bool checked)
{
    ui->blackwood->setChecked(true);
}

void CBidOptionsPropSheet::on_gerber_clicked(bool checked)
{
    ui->gerber->setChecked(true);
}

void CBidOptionsPropSheet::on_cueBids_clicked(bool checked)
{
    ui->cueBids->setChecked(true);
}

void CBidOptionsPropSheet::on_stayman_clicked(bool checked)
{
    ui->stayman->setChecked(true);
}

void CBidOptionsPropSheet::on_staymanNF_clicked()
{
    bidOptionDoc.stayman = STAYMAN_3R_NF;
}

void CBidOptionsPropSheet::on_staymanF_clicked()
{
    bidOptionDoc.stayman = STAYMAN_3R_F;
}

void CBidOptionsPropSheet::on_jacobyTransfers_clicked(bool checked)
{
    bidOptionDoc.jacobyTransfers = checked;
    if (!bidOptionDoc.jacobyTransfers)
        ui->fourSuitTransfers->setChecked(false);
    else
        ui->fourSuitTransfers->setChecked(bidOptionDoc.fourSuitTransfers);
}

void CBidOptionsPropSheet::on_fourSuitTransfers_clicked(bool checked)
{
    bidOptionDoc.fourSuitTransfers = checked;
}

void CBidOptionsPropSheet::on_limitRaises_clicked(bool checked)
{
    if (bidOptionDoc.bidStyle == EUROPEAN_STYLE)
        ui->limitRaises->setChecked(true);
    else
    {
        bidOptionDoc.limitRaises = checked;
        setDependencies();
    }
}

void CBidOptionsPropSheet::on_jacoby2NT_clicked(bool checked)
{
    if ((bidOptionDoc.bidStyle == AMERICAN_STYLE) && (bidOptionDoc.majorsMode == MAJOR_FIVE) ||
            (bidOptionDoc.bidStyle == EUROPEAN_STYLE))
        bidOptionDoc.jacoby2NT = checked;
    setDependencies();
}

void CBidOptionsPropSheet::on_splinterBids_clicked(bool checked)
{
    if ((bidOptionDoc.bidStyle == EUROPEAN_STYLE) && !bidOptionDoc.jacoby2NT)
        bidOptionDoc.splinterbids = checked;
    setDependencies();
}

void CBidOptionsPropSheet::on_fourthSuitForcing_clicked(bool checked)
{
    setDependencies();
}

void CBidOptionsPropSheet::on_weakTwo_clicked(bool checked)
{
    ui->w2Feature->setChecked(true);

    if (checked)
        bidOptionDoc.twoBidsMode = W2_FEATURE;
    else
        bidOptionDoc.twoBidsMode = S2_NATURAL;
}

void CBidOptionsPropSheet::on_w2Ogust_clicked()
{
    bidOptionDoc.twoBidsMode = W2_OGUST;
}

void CBidOptionsPropSheet::on_w2Feature_clicked()
{
    bidOptionDoc.twoBidsMode = W2_FEATURE;
}

void CBidOptionsPropSheet::on_w2NewSuitNF_clicked()
{
    bidOptionDoc.w2NewSuit = W2_NEW_SUIT_NF;
}

void CBidOptionsPropSheet::on_w2NewSuitF_clicked()
{
    bidOptionDoc.w2NewSuit = W2_NEW_SUIT_F;
}

void CBidOptionsPropSheet::on_openValue20_clicked()
{
    bidOptionDoc.twoClubOpenValues = OPEN_VALUE_20;
}

void CBidOptionsPropSheet::on_openValue21_clicked()
{
    bidOptionDoc.twoClubOpenValues = OPEN_VALUE_21;
}

void CBidOptionsPropSheet::on_openValue22_clicked()
{
    bidOptionDoc.twoClubOpenValues = OPEN_VALUE_22;
}

void CBidOptionsPropSheet::on_openValue23_clicked()
{
    bidOptionDoc.twoClubOpenValues = OPEN_VALUE_23;
}

void CBidOptionsPropSheet::on_twoCtwoDNegative_clicked()
{
    bidOptionDoc.twoCtwoD = TWO_C_TWO_D_NEGATIVE;
}

void CBidOptionsPropSheet::on_twoCtwoDWaiting_clicked()
{
    bidOptionDoc.twoCtwoD = TWO_C_TWO_D_WAITING;
}

void CBidOptionsPropSheet::on_oneNTRange12_14_clicked()
{
    bidOptionDoc.oneNTRange = ONE_NT_RANGE_12_14;

    ui->twoNT211518->setChecked(true);
    ui->twoNT21->setEnabled(false);
}

void CBidOptionsPropSheet::on_oneNTRange15_17_clicked()
{
    ui->twoNT21->setEnabled(true);
    if (bidOptionDoc.twoNT21 == TWO_NT_21_15_18)
        ui->twoNT211518->setChecked(true);
    else if (bidOptionDoc.twoNT21 == TWO_NT_21_12_14)
        ui->twoNT211214->setChecked(true);

    bidOptionDoc.oneNTRange = ONE_NT_RANGE_15_17;
}

void CBidOptionsPropSheet::on_oneNTRange16_18_clicked()
{
    ui->twoNT21->setEnabled(true);
    if (bidOptionDoc.twoNT21 == TWO_NT_21_15_18)
        ui->twoNT211518->setChecked(true);
    else if (bidOptionDoc.twoNT21 == TWO_NT_21_12_14)
        ui->twoNT211214->setChecked(true);

    bidOptionDoc.oneNTRange = ONE_NT_RANGE_16_18;
}

void CBidOptionsPropSheet::on_open1NTWeakMajor_clicked(bool checked)
{
    bidOptionDoc.open1NTWeakMajor = checked;
}

void CBidOptionsPropSheet::on_twoNTRange20_21_clicked()
{
    bidOptionDoc.twoNTRange = TWO_NT_RANGE_20_21;
}

void CBidOptionsPropSheet::on_twoNTRange21_22_clicked()
{
    bidOptionDoc.twoNTRange = TWO_NT_RANGE_21_22;
}

void CBidOptionsPropSheet::on_twoNTRange22_24_clicked()
{
    bidOptionDoc.twoNTRange = TWO_NT_RANGE_22_24;
}

void CBidOptionsPropSheet::on_threNTRange25_27_clicked()
{
    bidOptionDoc.threeNTRange = THREE_NT_RANGE_25_27;
}

void CBidOptionsPropSheet::on_gambling3NT_clicked()
{
    bidOptionDoc.threeNTRange = GAMBLING_3_NT;
}

void CBidOptionsPropSheet::on_gambling3NT_ACOL_clicked()
{
    bidOptionDoc.threeNTRange = GAMBLING_3_NT_ACOL;
}

void CBidOptionsPropSheet::on_twoNT111820_clicked()
{
    bidOptionDoc.twoNT11 = TWO_NT_11_18_20;
}

void CBidOptionsPropSheet::on_twoNT111718_clicked()
{
    bidOptionDoc.twoNT11 = TWO_NT_11_17_18;
}

void CBidOptionsPropSheet::on_twoNT211518_clicked()
{
    bidOptionDoc.twoNT21 = TWO_NT_21_15_18;
}

void CBidOptionsPropSheet::on_twoNT211214_clicked()
{
    bidOptionDoc.twoNT21 = TWO_NT_21_12_14;
}

void CBidOptionsPropSheet::on_rebid1NTNat_clicked()
{
    bidOptionDoc.rebid1NT = REBID_1NT_NAT;
}

void CBidOptionsPropSheet::on_rebid1NTF_clicked()
{
    bidOptionDoc.rebid1NT = REBID_1NT_F;
}

void CBidOptionsPropSheet::on_oneNT3mSI_clicked()
{
    bidOptionDoc.oneNT3m = ONE_NT_3m_SI;
}

void CBidOptionsPropSheet::on_oneNT3mGI_clicked()
{
    bidOptionDoc.oneNT3m = ONE_NT_3M_GI;
}

void CBidOptionsPropSheet::on_oneNT3mPE_clicked()
{
    bidOptionDoc.oneNT3m = ONE_NT_3m_PE;
}

void CBidOptionsPropSheet::on_oneNT3MSI_clicked()
{
    bidOptionDoc.oneNT3M = ONE_NT_3M_SI;
}

void CBidOptionsPropSheet::on_oneNT3MGF_clicked()
{
    bidOptionDoc.oneNT3M = ONE_NT_3M_GF;
}

void CBidOptionsPropSheet::on_oneNT3MGI_clicked()
{
    bidOptionDoc.oneNT3M = ONE_NT_3M_GI;
}

void CBidOptionsPropSheet::on_oneNT3MPE_clicked()
{
    bidOptionDoc.oneNT3M = ONE_NT_3M_PE;
}

void CBidOptionsPropSheet::on_structuredReverses_clicked(bool checked)
{
    bidOptionDoc.structuredReverse = checked;
}

void CBidOptionsPropSheet::on_takeOutDoubles_clicked(bool checked)
{
    bidOptionDoc.takeoutDoubles = checked;
}

void CBidOptionsPropSheet::on_negativeDoubles_clicked(bool checked)
{
    bidOptionDoc.negativeDoubles = checked;
}

void CBidOptionsPropSheet::on_michaelsCueBid_clicked(bool checked)
{
    bidOptionDoc.directCueBid = checked;
}

void CBidOptionsPropSheet::on_unusualNT_clicked(bool checked)
{
    bidOptionDoc.unusual2NT = checked;
}

void CBidOptionsPropSheet::on_drury_clicked(bool checked)
{
    bidOptionDoc.drury = checked;
}

void CBidOptionsPropSheet::on_jumpOvercallWeak_clicked(bool checked)
{
    if (checked)
    {
        bidOptionDoc.jumpOvercalls = JUMP_OVERCALL_WEAK_NATURAL;
        ui->wjoNatural->setChecked(true);
    }
    else
        bidOptionDoc.jumpOvercalls = JUMP_OVERCALL_STRONG;
}

void CBidOptionsPropSheet::on_wjoNatural_clicked()
{
    bidOptionDoc.jumpOvercalls = JUMP_OVERCALL_WEAK_NATURAL;
}

void CBidOptionsPropSheet::on_wjoOgust_clicked()
{
    bidOptionDoc.jumpOvercalls = JUMP_OVERCALL_WEAK_OGUST;
}

void CBidOptionsPropSheet::on_wjoFeature_clicked()
{
    bidOptionDoc.jumpOvercalls = JUMP_OVERCALL_WEAK_FEATURE;
}

void CBidOptionsPropSheet::on_soundThreeLevel_clicked()
{
    bidOptionDoc.threeLevel = SOUND_THREE_LEVEL;
}

void CBidOptionsPropSheet::on_lightThreeLevel_clicked()
{
    bidOptionDoc.threeLevel = LIGHT_THREE_LEVEL;
}

void CBidOptionsPropSheet::on_lightThreeLevelNV_clicked()
{
    bidOptionDoc.threeLevel = LIGHT_THREE_LEVEL_NV;
}

void CBidOptionsPropSheet::on_elevenHCPsRbsLm_clicked(bool checked)
{
    bidOptionDoc.elevenHCPsRbsLm = checked;
}

void CBidOptionsPropSheet::on_elevenHCPs6Cs_clicked(bool checked)
{
    bidOptionDoc.elevenHCPs6Cs = checked;
}

void CBidOptionsPropSheet::on_fourteenTPsGs_clicked(bool checked)
{
    bidOptionDoc.fourteenTPsGs = checked;
}

void CBidOptionsPropSheet::on_fourteenTPsLs_clicked(bool checked)
{
    bidOptionDoc.fourteenTPsLs = checked;
}

void CBidOptionsPropSheet::on_thirteeTPsLs_clicked(bool checked)
{
    bidOptionDoc.thirteenTPsLs = checked;
}

void CBidOptionsPropSheet::on_signalPartnerLeadSuit_currentIndexChanged(int index)
{
    bidOptionDoc.partnerLeadSuit = index;
}

void CBidOptionsPropSheet::on_signalDeclarerLeadSuit_currentIndexChanged(int index)
{
    bidOptionDoc.declarerLeadSuit = index;
}

void CBidOptionsPropSheet::on_signalDiscardingSuit_currentIndexChanged(int index)
{
    bidOptionDoc.discardingSuit = index;
}

void CBidOptionsPropSheet::on_signalPartnerLeadNT_currentIndexChanged(int index)
{
    bidOptionDoc.partnerLeadNT = index;
}

void CBidOptionsPropSheet::on_signalDeclarerLeadNT_currentIndexChanged(int index)
{
    bidOptionDoc.declarerLeadNT = index;
}

void CBidOptionsPropSheet::on_signalDiscardingNT_currentIndexChanged(int index)
{
    bidOptionDoc.discardingNT = index;
}

void CBidOptionsPropSheet::on_fourthBest_clicked()
{
    bidOptionDoc.lengthLead = FOURTH_BEST;
}

void CBidOptionsPropSheet::on_thirdBest_clicked()
{
    bidOptionDoc.lengthLead = THIRD_FIFTH_BEST;
}

void CBidOptionsPropSheet::on_lowEncouraging_clicked()
{
    bidOptionDoc.lengthLead = LOW_ENCOURAGING;
}

void CBidOptionsPropSheet::on_Aks_clicked()
{
    bidOptionDoc.openingLead[OPEN_SUIT][AK_INX] = Ak_VAL;
}

void CBidOptionsPropSheet::on_aKs_clicked()
{
    bidOptionDoc.openingLead[OPEN_SUIT][AK_INX] = aK_VAL;
}

void CBidOptionsPropSheet::on_Xxxs_clicked()
{
    bidOptionDoc.openingLead[OPEN_SUIT][XXX_INX] = Xxx_VAL;
}

void CBidOptionsPropSheet::on_xXxs_clicked()
{
    bidOptionDoc.openingLead[OPEN_SUIT][XXX_INX] = xXx_VAL;
}

void CBidOptionsPropSheet::on_xxXs_clicked()
{
    bidOptionDoc.openingLead[OPEN_SUIT][XXX_INX] = xxX_VAL;
}

void CBidOptionsPropSheet::on_Aknt_clicked()
{
    bidOptionDoc.openingLead[OPEN_NT][AK_INX] = Ak_VAL;
}

void CBidOptionsPropSheet::on_aKnt_clicked()
{
    bidOptionDoc.openingLead[OPEN_NT][AK_INX] = aK_VAL;
}

void CBidOptionsPropSheet::on_Xxxnt_clicked()
{
    bidOptionDoc.openingLead[OPEN_NT][XXX_INX] = Xxx_VAL;
}

void CBidOptionsPropSheet::on_xXxnt_clicked()
{
    bidOptionDoc.openingLead[OPEN_NT][XXX_INX] = xXx_VAL;
}

void CBidOptionsPropSheet::on_xxXnt_clicked()
{
    bidOptionDoc.openingLead[OPEN_NT][XXX_INX] = xxX_VAL;
}

void CBidOptionsPropSheet::setDependencies()
{
    if (bidOptionDoc.bidStyle == EUROPEAN_STYLE)
    {
        ui->limitRaises->setChecked(true);
//        ui->limitRaises->setStyleSheet("background-color: yellow");
        ui->fourthSuitForcing->setChecked(true);
        if (bidOptionDoc.jacoby2NT)
        {
            ui->splinterBids->setChecked(true);
        }
        else
        {
            ui->splinterBids->setChecked(bidOptionDoc.splinterbids);
        }
    }
    else
    {
        ui->limitRaises->setChecked(bidOptionDoc.limitRaises);
        if (!bidOptionDoc.limitRaises)
        {
            ui->jacoby2NT->setChecked(false);
            ui->splinterBids->setChecked(false);
            ui->fourthSuitForcing->setChecked(false);
        }
        else
        {
            bool jacoby = (bidOptionDoc.majorsMode == MAJOR_FIVE) ? bidOptionDoc.jacoby2NT : true;
            ui->jacoby2NT->setChecked(jacoby);
            if (jacoby)
            {
                ui->splinterBids->setChecked(true);
                ui->fourthSuitForcing->setChecked(true);
            }
            else
            {
                ui->splinterBids->setChecked(false);
                ui->fourthSuitForcing->setChecked(false);
            }
        }
    }
}
