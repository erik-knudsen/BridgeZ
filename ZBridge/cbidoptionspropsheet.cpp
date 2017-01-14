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

    ui->openingLeadType->addItem(tr("N/S Opening Leads vs Suit Contracts"));
    ui->openingLeadType->addItem(tr("E/W Opening Leads vs Suit Contracts"));
    ui->openingLeadType->addItem(tr("N/S Opening Leads vs NT Contracts"));
    ui->openingLeadType->addItem(tr("E/W Opening Leads vs NT Contracts"));

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
    else if (bidOptionDoc.lengthLead == THIRDBEST)
        ui->thirdBest->setChecked(true);
    else
        ui->lowEncouraging->setChecked(true);

    //N/S Opening leads vs. Suit Contracts.
    if (bidOptionDoc.openingLead[NS_SUIT][XX_N_S] == xX_N_S)
        ui->Xxns->setChecked(true);
    else
        ui->xXns->setChecked(true);

    if (bidOptionDoc.openingLead[NS_SUIT][XXX_N_S] == xXX_N_S)
        ui->Xxxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][XXX_N_S] == XxX_N_S)
        ui->xXxns->setChecked(true);
    else
        ui->xxXns->setChecked(true);

    if (bidOptionDoc.openingLead[NS_SUIT][AKX_N_S] == aKX_N_S)
        ui->Akxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][AKX_N_S] == AkX_N_S)
        ui->aKxns->setChecked(true);
    else
        ui->akXns->setChecked(true);

    if (bidOptionDoc.openingLead[NS_SUIT][KQX_N_S] == kQX_N_S)
        ui->Kqxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][KQX_N_S] == KqX_N_S)
        ui->kQxns->setChecked(true);
    else
        ui->kqXns->setChecked(true);

    if (bidOptionDoc.openingLead[NS_SUIT][QJX_N_S] == qJX_N_S)
        ui->Qjxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][QJX_N_S] == QjX_N_S)
        ui->qJxns->setChecked(true);
    else
        ui->qjXns->setChecked(true);

    if (bidOptionDoc.openingLead[NS_SUIT][JTX_N_S] == jTX_N_S)
        ui->Jtxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][JTX_N_S] == JtX_N_S)
        ui->jTxns->setChecked(true);
    else
        ui->jtXns->setChecked(true);

    if (bidOptionDoc.openingLead[NS_SUIT][KQTN_N_S] == kQTN_N_S)
        ui->Kqtnns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][KQTN_N_S] == KqTN_N_S)
        ui->kQtnnx->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][KQTN_N_S] == KQtN_N_S)
        ui->kqTnns->setChecked(true);
    else
        ui->kqtNns->setChecked(true);

    if (bidOptionDoc.openingLead[NS_SUIT][XXXX_N_S] == xXXX_N_S)
        ui->Xxxxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][XXXX_N_S] == XxXX_N_S)
        ui->xXxxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][XXXX_N_S] == XXxX_N_S)
        ui->xxXxns->setChecked(true);
    else
        ui->xxxXns->setChecked(true);

    if (bidOptionDoc.openingLead[NS_SUIT][XXXXX_N_S] == xXXXX_N_S)
        ui->Xxxxxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][XXXXX_N_S] == XxXXX_N_S)
        ui->xXxxxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][XXXXX_N_S] == XXxXX_N_S)
        ui->xxXxxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][XXXXX_N_S] == XXXxX_N_S)
        ui->xxxXxns->setChecked(true);
    else
        ui->xxxxXns->setChecked(true);

    if (bidOptionDoc.openingLead[NS_SUIT][TNX_N_S] == tNX_N_S)
        ui->Tnxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][TNX_N_S] == TnX_N_S)
        ui->tNxns->setChecked(true);
    else
        ui->tnXns->setChecked(true);

    if (bidOptionDoc.openingLead[NS_SUIT][KJTX_N_S] == kJTX_N_S)
        ui->Kjtxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][KJTX_N_S] == KjTX_N_S)
        ui->kJtxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][KJTX_N_S] == KJtX_N_S)
        ui->kjTxns->setChecked(true);
    else
        ui->kjtXns->setChecked(true);

    if (bidOptionDoc.openingLead[NS_SUIT][KTNX_N_S] == kTNX_N_S)
        ui->Ktnxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][KTNX_N_S] == KtNX_N_S)
        ui->kTnxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][KTNX_N_S] == KTnX_N_S)
        ui->ktNxns->setChecked(true);
    else
        ui->ktnXns->setChecked(true);

    if (bidOptionDoc.openingLead[NS_SUIT][QTNX_N_S] == qTNX_N_S)
        ui->Qtnxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][QTNX_N_S] == QtNX_N_S)
        ui->qTnxns->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_SUIT][QTNX_N_S] == QTnX_N_S)
        ui->qtNxns->setChecked(true);
    else
        ui->qtnXnx->setChecked(true);

    //E/W Opening leads vs. Suit Contracts.
    if (bidOptionDoc.openingLead[EW_SUIT][XX_E_S] == xX_E_S)
        ui->Xxes->setChecked(true);
    else
        ui->xXes->setChecked(true);

    if (bidOptionDoc.openingLead[EW_SUIT][XXX_E_S] == xXX_E_S)
        ui->Xxxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][XXX_E_S] == XxX_E_S)
        ui->xXxes->setChecked(true);
    else
        ui->xxXes->setChecked(true);

    if (bidOptionDoc.openingLead[EW_SUIT][AKX_E_S] == aKX_E_S)
        ui->Akxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][AKX_E_S] == AkX_E_S)
        ui->aKxes->setChecked(true);
    else
        ui->akXes->setChecked(true);

    if (bidOptionDoc.openingLead[EW_SUIT][KQX_E_S] == kQX_E_S)
        ui->Kqxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][KQX_E_S] == KqX_E_S)
        ui->kQxes->setChecked(true);
    else
        ui->kqXes->setChecked(true);

    if (bidOptionDoc.openingLead[EW_SUIT][QJX_E_S] == qJX_E_S)
        ui->Qjxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][QJX_E_S] == QjX_E_S)
        ui->qJxes->setChecked(true);
    else
        ui->qjXes->setChecked(true);

    if (bidOptionDoc.openingLead[EW_SUIT][JTX_E_S] == jTX_E_S)
        ui->Jtxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][JTX_E_S] == JtX_E_S)
        ui->jTxes->setChecked(true);
    else
        ui->jtXes->setChecked(true);

    if (bidOptionDoc.openingLead[EW_SUIT][KQTN_E_S] == kQTN_E_S)
        ui->Kqtnes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][KQTN_E_S] == KqTN_E_S)
        ui->kQtnes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][KQTN_E_S] == KQtN_E_S)
        ui->kqTnes->setChecked(true);
    else
        ui->kqtNes->setChecked(true);

    if (bidOptionDoc.openingLead[EW_SUIT][XXXX_E_S] == xXXX_E_S)
        ui->Xxxxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][XXXX_E_S] == XxXX_E_S)
        ui->xXxxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][XXXX_E_S] == XXxX_E_S)
        ui->xxXxes->setChecked(true);
    else
        ui->xxxXes->setChecked(true);

    if (bidOptionDoc.openingLead[EW_SUIT][XXXXX_E_S] == xXXXX_E_S)
        ui->Xxxxxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][XXXXX_E_S] == XxXXX_E_S)
        ui->xXxxxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][XXXXX_E_S] == XXxXX_E_S)
        ui->xxXxxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][XXXXX_E_S] == XXXxX_E_S)
        ui->xxxXxes->setChecked(true);
    else
        ui->xxxxXes->setChecked(true);

    if (bidOptionDoc.openingLead[EW_SUIT][TNX_E_S] == tNX_E_S)
        ui->Tnxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][TNX_E_S] == TnX_E_S)
        ui->tNxes->setChecked(true);
    else
        ui->tnXes->setChecked(true);

    if (bidOptionDoc.openingLead[EW_SUIT][KJTX_E_S] == kJTX_E_S)
        ui->Kjtxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][KJTX_E_S] == KjTX_E_S)
        ui->kJtxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][KJTX_E_S] == KJtX_E_S)
        ui->kjTxes->setChecked(true);
    else
        ui->kjtXes->setChecked(true);

    if (bidOptionDoc.openingLead[EW_SUIT][KTNX_E_S] == kTNX_E_S)
        ui->Ktnxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][KTNX_E_S] == KtNX_E_S)
        ui->kTnxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][KTNX_E_S] == KTnX_E_S)
        ui->ktNxes->setChecked(true);
    else
        ui->ktnXes->setChecked(true);

    if (bidOptionDoc.openingLead[EW_SUIT][QTNX_E_S] == qTNX_E_S)
        ui->Qtnxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][QTNX_E_S] == QtNX_E_S)
        ui->qTnxes->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_SUIT][QTNX_E_S] == QTnX_E_S)
        ui->qtNxes->setChecked(true);
    else
        ui->qtnXes->setChecked(true);

    //N/S Opening leads vs. NT Contracts.
    if (bidOptionDoc.openingLead[NS_NT][XX_N_NT] == xX_N_NT)
        ui->Xxnnt->setChecked(true);
    else
        ui->xXnnt->setChecked(true);

    if (bidOptionDoc.openingLead[NS_NT][XXX_N_NT] == xXX_N_NT)
        ui->Xxxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][XXX_N_NT] == XxX_N_NT)
        ui->xXxnnt->setChecked(true);
    else
        ui->xxXnnt->setChecked(true);

    if (bidOptionDoc.openingLead[NS_NT][AKJX_N_NT] == aKJX_N_NT)
        ui->Akjxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][AKJX_N_NT] == AkJX_N_NT)
        ui->aKjxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][AKJX_N_NT] == AKjX_N_NT)
        ui->akJxnnt->setChecked(true);
    else
        ui->akjXnnt->setChecked(true);

    if (bidOptionDoc.openingLead[NS_NT][AJTN_N_NT] == aJTN_N_NT)
        ui->Ajtnnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][AJTN_N_NT] == AjTN_N_NT)
        ui->aJtnnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][AJTN_N_NT] == AJtN_N_NT)
        ui->ajtNnnt->setChecked(true);
    else
        ui->ajtNnnt->setChecked(true);

    if (bidOptionDoc.openingLead[NS_NT][KQJX_N_NT] == kQJX_N_NT)
        ui->Kqjxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][KQJX_N_NT] == KqJX_N_NT)
        ui->kQjxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][KQJX_N_NT] == KQjX_N_NT)
        ui->kqJxnnt->setChecked(true);
    else
        ui->kqjXnnt->setChecked(true);

    if (bidOptionDoc.openingLead[NS_NT][QKTX_N_NT] == qKTX_N_NT)
        ui->Qktxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][QKTX_N_NT] == QkTX_N_NT)
        ui->qKtxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][QKTX_N_NT] == QKtX_N_NT)
        ui->qkTxnnt->setChecked(true);
    else
        ui->qktXnnt->setChecked(true);

    if (bidOptionDoc.openingLead[NS_NT][JTNX_N_NT] == jTNX_N_NT)
        ui->Jtnxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][JTNX_N_NT] == JtNX_N_NT)
        ui->jTnxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][JTNX_N_NT] == JTnX_N_NT)
        ui->jtNxnnt->setChecked(true);
    else
        ui->jtnXnnt->setChecked(true);

    if (bidOptionDoc.openingLead[NS_NT][XXXX_N_NT] == xXXX_N_NT)
        ui->Xxxxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][XXXX_N_NT] == XxXX_N_NT)
        ui->xXxxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][XXXX_N_NT] == XXxX_N_NT)
        ui->xxXxnnt->setChecked(true);
    else
        ui->xxxXnnt->setChecked(true);

    if (bidOptionDoc.openingLead[NS_NT][XXXXX_N_NT] == xXXXX_N_NT)
        ui->Xxxxxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][XXXXX_N_NT] == XxXXX_N_NT)
        ui->xXxxxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][XXXXX_N_NT] == XXxXX_N_NT)
        ui->xxXxxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][XXXXX_N_NT] == XXXxX_N_NT)
        ui->xxxXxnnt->setChecked(true);
    else
        ui->xxxxXnnt->setChecked(true);

    if (bidOptionDoc.openingLead[NS_NT][AQJX_N_NT] == aQJX_N_NT)
        ui->Aqjxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][AQJX_N_NT] == AqJX_N_NT)
        ui->aQjxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][AQJX_N_NT] == AQjX_N_NT)
        ui->aqJxnnt->setChecked(true);
    else
        ui->aqjXnnt->setChecked(true);

    if (bidOptionDoc.openingLead[NS_NT][ATNX_N_NT] == aTNX_N_NT)
        ui->Atnxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][ATNX_N_NT] == AtNX_N_NT)
        ui->aTnxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][ATNX_N_NT] == ATnX_N_NT)
        ui->atNxnnt->setChecked(true);
    else
        ui->atnXnnt->setChecked(true);

    if (bidOptionDoc.openingLead[NS_NT][KQTN_N_NT] == kQTN_N_NT)
        ui->Kqtnnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][KQTN_N_NT] == KqTN_N_NT)
        ui->kQtnnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][KQTN_N_NT] == KQtN_N_NT)
        ui->kqTnnnt->setChecked(true);
    else
        ui->kqtNnnt->setChecked(true);

    if (bidOptionDoc.openingLead[NS_NT][QTNX_N_NT] == qTNX_N_NT)
        ui->Qtnxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][QTNX_N_NT] == QtNX_N_NT)
        ui->qTnxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][QTNX_N_NT] == QTnX_N_NT)
        ui->qtNxnnt->setChecked(true);
    else
        ui->qtnXnnt->setChecked(true);

    if (bidOptionDoc.openingLead[NS_NT][TNXX_N_NT] == tNXX_N_NT)
        ui->Tnxxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][TNXX_N_NT] == TnXX_N_NT)
        ui->tNxxnnt->setChecked(true);
    else if (bidOptionDoc.openingLead[NS_NT][TNXX_N_NT] == TNxX_N_NT)
        ui->tnXxnnt->setChecked(true);
    else
        ui->tnxXnnt->setChecked(true);

    //E/W Opening leads vs. NT Contracts.
    if (bidOptionDoc.openingLead[EW_NT][XX_E_NT] == xX_E_NT)
        ui->Xxent->setChecked(true);
    else
        ui->xXent->setChecked(true);

    if (bidOptionDoc.openingLead[EW_NT][XXX_E_NT] == xXX_E_NT)
        ui->Xxxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][XXX_E_NT] == XxX_E_NT)
        ui->xXxent->setChecked(true);
    else
        ui->xxXent->setChecked(true);

    if (bidOptionDoc.openingLead[EW_NT][AKJX_E_NT] == aKJX_E_NT)
        ui->Akjxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][AKJX_E_NT] == AkJX_E_NT)
        ui->aKjxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][AKJX_E_NT] == AKjX_E_NT)
        ui->ekJxent->setChecked(true);
    else
        ui->akjXent->setChecked(true);

    if (bidOptionDoc.openingLead[EW_NT][AJTN_E_NT] == aJTN_E_NT)
        ui->Ajtnent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][AJTN_E_NT] == AjTN_E_NT)
        ui->aJtnent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][AJTN_E_NT] == AJtN_E_NT)
        ui->ajtNent->setChecked(true);
    else
        ui->ajtNent->setChecked(true);

    if (bidOptionDoc.openingLead[EW_NT][KQJX_E_NT] == kQJX_E_NT)
        ui->Kajxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][KQJX_E_NT] == KqJX_E_NT)
        ui->kQjxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][KQJX_E_NT] == KQjX_E_NT)
        ui->kqJxent->setChecked(true);
    else
        ui->kqjXent->setChecked(true);

    if (bidOptionDoc.openingLead[EW_NT][QKTX_E_NT] == qKTX_E_NT)
        ui->Qjtxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][QKTX_E_NT] == QkTX_E_NT)
        ui->qJtxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][QKTX_E_NT] == QKtX_E_NT)
        ui->qjTxent->setChecked(true);
    else
        ui->qjtXent->setChecked(true);

    if (bidOptionDoc.openingLead[EW_NT][JTNX_E_NT] == jTNX_E_NT)
        ui->Jtnxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][JTNX_E_NT] == JtNX_E_NT)
        ui->jTnxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][JTNX_E_NT] == JTnX_E_NT)
        ui->jtNxent->setChecked(true);
    else
        ui->jtnXent->setChecked(true);

    if (bidOptionDoc.openingLead[EW_NT][XXXX_E_NT] == xXXX_E_NT)
        ui->Xxxxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][XXXX_E_NT] == XxXX_E_NT)
        ui->xXxxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][XXXX_E_NT] == XXxX_E_NT)
        ui->xxXxent->setChecked(true);
    else
        ui->xxxXent->setChecked(true);

    if (bidOptionDoc.openingLead[EW_NT][XXXXX_E_NT] == xXXXX_E_NT)
        ui->Xxxxxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][XXXXX_E_NT] == XxXXX_E_NT)
        ui->xXxxxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][XXXXX_E_NT] == XXxXX_E_NT)
        ui->xxXxxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][XXXXX_E_NT] == XXXxX_E_NT)
        ui->xxxXxent->setChecked(true);
    else
        ui->xxxxXent->setChecked(true);

    if (bidOptionDoc.openingLead[EW_NT][AQJX_E_NT] == aQJX_E_NT)
        ui->Aqjxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][AQJX_E_NT] == AqJX_E_NT)
        ui->aQjxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][AQJX_E_NT] == AQjX_E_NT)
        ui->aqJxent->setChecked(true);
    else
        ui->aqjXent->setChecked(true);

    if (bidOptionDoc.openingLead[EW_NT][ATNX_E_NT] == aTNX_E_NT)
        ui->Atnxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][ATNX_E_NT] == AtNX_E_NT)
        ui->aTnxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][ATNX_E_NT] == ATnX_E_NT)
        ui->atNxent->setChecked(true);
    else
        ui->atnXent->setChecked(true);

    if (bidOptionDoc.openingLead[EW_NT][KQTN_E_NT] == kQTN_E_NT)
        ui->Kqt9ent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][KQTN_E_NT] == KqTN_E_NT)
        ui->kQtnent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][KQTN_E_NT] == KQtN_E_NT)
        ui->kqTnent->setChecked(true);
    else
        ui->kqtNent->setChecked(true);

    if (bidOptionDoc.openingLead[EW_NT][QTNX_E_NT] == qTNX_E_NT)
        ui->Qtnxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][QTNX_E_NT] == QtNX_E_NT)
        ui->qTnxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][QTNX_E_NT] == QTnX_E_NT)
        ui->qtNxent->setChecked(true);
    else
        ui->qtnXent->setChecked(true);

    if (bidOptionDoc.openingLead[EW_NT][TNXX_E_NT] == tNXX_E_NT)
        ui->Tnxxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][TNXX_E_NT] == TnXX_E_NT)
        ui->tNxxent->setChecked(true);
    else if (bidOptionDoc.openingLead[EW_NT][TNXX_E_NT] == TNxX_E_NT)
        ui->tnXxent->setChecked(true);
    else
        ui->tnxXent->setChecked(true);
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

void CBidOptionsPropSheet::on_openingLeadType_currentIndexChanged(int index)
{
    ui->openingLeads->setCurrentIndex(index);
}

void CBidOptionsPropSheet::on_fourthBest_clicked()
{
    bidOptionDoc.lengthLead = FOURTH_BEST;
}

void CBidOptionsPropSheet::on_thirdBest_clicked()
{
    bidOptionDoc.lengthLead = THIRDBEST;
}

void CBidOptionsPropSheet::on_lowEncouraging_clicked()
{
    bidOptionDoc.lengthLead = LOW_ENCOURAGING;
}

void CBidOptionsPropSheet::on_Xxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XX_N_S] = xX_N_S;
}

void CBidOptionsPropSheet::on_xXns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XX_N_S] = Xx_N_S;
}

void CBidOptionsPropSheet::on_Xxxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXX_N_S] = xXX_N_S;
}

void CBidOptionsPropSheet::on_xXxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXX_N_S] = XxX_N_S;
}

void CBidOptionsPropSheet::on_xxXns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXX_N_S] = XXx_N_S;
}

void CBidOptionsPropSheet::on_Akxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][AKX_N_S] = aKX_N_S;
}

void CBidOptionsPropSheet::on_aKxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][AKX_N_S] = AkX_N_S;
}

void CBidOptionsPropSheet::on_akXns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][AKX_N_S] = AKx_N_S;
}

void CBidOptionsPropSheet::on_Kqxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][KQX_N_S] = kQX_N_S;
}

void CBidOptionsPropSheet::on_kQxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][KQX_N_S] = KqX_N_S;
}

void CBidOptionsPropSheet::on_kqXns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][KQX_N_S] = KQx_N_S;
}

void CBidOptionsPropSheet::on_Qjxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][QJX_N_S] = qJX_N_S;
}

void CBidOptionsPropSheet::on_qJxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXX_N_S] = QjX_N_S;
}

void CBidOptionsPropSheet::on_qjXns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXX_N_S] = QJx_N_S;
}

void CBidOptionsPropSheet::on_Jtxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][JTX_N_S] = jTX_N_S;
}

void CBidOptionsPropSheet::on_jTxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][JTX_N_S] = JtX_N_S;
}

void CBidOptionsPropSheet::on_jtXns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][JTX_N_S] = JTx_N_S;
}

void CBidOptionsPropSheet::on_Kqtnns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][KQTN_N_S] = kQTN_N_S;
}

void CBidOptionsPropSheet::on_kQtnnx_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXX_N_S] = KqTN_N_S;
}

void CBidOptionsPropSheet::on_kqTnns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXX_N_S] = KQtN_N_S;
}

void CBidOptionsPropSheet::on_kqtNns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXX_N_S] = KQTn_N_S;
}

void CBidOptionsPropSheet::on_Xxxxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXXX_N_S] = xXXXX_N_S;
}

void CBidOptionsPropSheet::on_xXxxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXXX_N_S] = XxXX_N_S;
}

void CBidOptionsPropSheet::on_xxXxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXXX_N_S] = XXxX_N_S;
}

void CBidOptionsPropSheet::on_xxxXns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXXX_N_S] = XXXx_N_S;
}

void CBidOptionsPropSheet::on_Xxxxxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXXXX_N_S] = xXXXX_N_S;
}

void CBidOptionsPropSheet::on_xXxxxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXXXX_N_S] = XxXXX_N_S;
}

void CBidOptionsPropSheet::on_xxXxxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXXXX_N_S] = XXxXX_N_S;
}

void CBidOptionsPropSheet::on_xxxXxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXXXX_N_S] = XXXxX_N_S;
}

void CBidOptionsPropSheet::on_xxxxXns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXXXX_N_S] = XXXXx_N_S;
}

void CBidOptionsPropSheet::on_Tnxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][TNX_N_S] = tNX_N_S;
}

void CBidOptionsPropSheet::on_tNxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXX_N_S] = TnX_N_S;
}

void CBidOptionsPropSheet::on_tnXns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][XXX_N_S] = TNx_N_S;
}

void CBidOptionsPropSheet::on_Kjtxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][KJTX_N_S] = kJTX_N_S;
}

void CBidOptionsPropSheet::on_kJtxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][KJTX_N_S] = KjTX_N_S;
}

void CBidOptionsPropSheet::on_kjTxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][KJTX_N_S] = KJtX_N_S;
}

void CBidOptionsPropSheet::on_kjtXns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][KJTX_N_S] = KJTx_N_S;
}

void CBidOptionsPropSheet::on_Ktnxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][KTNX_N_S] = kTNX_N_S;
}

void CBidOptionsPropSheet::on_kTnxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][KTNX_N_S] = KtNX_N_S;
}

void CBidOptionsPropSheet::on_ktNxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][KTNX_N_S] = KTnX_N_S;
}

void CBidOptionsPropSheet::on_ktnXns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][KTNX_N_S] = KTNx_N_S;
}

void CBidOptionsPropSheet::on_Qtnxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][QTNX_N_S] = qTNX_N_S;
}

void CBidOptionsPropSheet::on_qTnxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][QTNX_N_S] = QtNX_N_S;
}

void CBidOptionsPropSheet::on_qtNxns_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][QTNX_N_S] = QTnX_N_S;
}

void CBidOptionsPropSheet::on_qtnXnx_clicked()
{
    bidOptionDoc.openingLead[NS_SUIT][QTNX_N_S] = QTNx_N_S;
}

void CBidOptionsPropSheet::on_Xxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XX_E_S] = xX_E_S;
}

void CBidOptionsPropSheet::on_xXes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XX_E_S] = Xx_E_S;
}

void CBidOptionsPropSheet::on_Xxxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XXX_E_S] = xXX_E_S;
}

void CBidOptionsPropSheet::on_xXxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XXX_E_S] = XxX_E_S;
}

void CBidOptionsPropSheet::on_xxXes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XXX_E_S] = XXx_E_S;
}

void CBidOptionsPropSheet::on_Akxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][AKX_E_S] = aKX_E_S;
}

void CBidOptionsPropSheet::on_aKxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][AKX_E_S] = AkX_E_S;
}

void CBidOptionsPropSheet::on_akXes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][AKX_E_S] = AKx_E_S;
}

void CBidOptionsPropSheet::on_Kqxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KQX_E_S] = kQX_E_S;

}

void CBidOptionsPropSheet::on_kQxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KQX_E_S] = KqX_E_S;
}

void CBidOptionsPropSheet::on_kqXes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KQX_E_S] = KQx_E_S;

}

void CBidOptionsPropSheet::on_Qjxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][QJX_E_S] = qJX_E_S;
}

void CBidOptionsPropSheet::on_qJxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][QJX_E_S] = QjX_E_S;
}

void CBidOptionsPropSheet::on_qjXes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][QJX_E_S] = QJx_E_S;

}

void CBidOptionsPropSheet::on_Jtxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][JTX_E_S] = jTX_E_S;
}

void CBidOptionsPropSheet::on_jTxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][JTX_E_S] = JtX_E_S;
}

void CBidOptionsPropSheet::on_jtXes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][JTX_E_S] = JTx_E_S;
}

void CBidOptionsPropSheet::on_Kqtnes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KQTN_E_S] = kQTN_E_S;
}

void CBidOptionsPropSheet::on_kQtnes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KQTN_E_S] = KqTN_E_S;
}

void CBidOptionsPropSheet::on_kqTnes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KQTN_E_S] = KQtN_E_S;
}

void CBidOptionsPropSheet::on_kqtNes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KQTN_E_S] = KQTn_E_S;
}

void CBidOptionsPropSheet::on_Xxxxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XXXX_E_S] = xXXXX_E_S;
}

void CBidOptionsPropSheet::on_xXxxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XXXX_E_S] = XxXX_E_S;
}

void CBidOptionsPropSheet::on_xxXxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XXXX_E_S] = XXxX_E_S;
}

void CBidOptionsPropSheet::on_xxxXes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XXXX_E_S] = XXXx_E_S;
}

void CBidOptionsPropSheet::on_Xxxxxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XXXX_E_S] = xXXXX_E_S;
}

void CBidOptionsPropSheet::on_xXxxxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XXXX_E_S] = XxXXX_E_S;
}

void CBidOptionsPropSheet::on_xxXxxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XXXX_E_S] = XXxXX_E_S;
}

void CBidOptionsPropSheet::on_xxxXxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XXXX_E_S] = XXXxX_E_S;
}

void CBidOptionsPropSheet::on_xxxxXes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][XXXX_E_S] = XXXXx_E_S;
}

void CBidOptionsPropSheet::on_Tnxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][TNX_E_S] = tNX_E_S;
}

void CBidOptionsPropSheet::on_tNxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][TNX_E_S] = TnX_E_S;
}

void CBidOptionsPropSheet::on_tnXes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][TNX_E_S] = TNx_E_S;
}

void CBidOptionsPropSheet::on_Kjtxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KJTX_E_S] = kJTX_E_S;
}

void CBidOptionsPropSheet::on_kJtxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KJTX_E_S] = KjTX_E_S;
}

void CBidOptionsPropSheet::on_kjTxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KJTX_E_S] = KJtX_E_S;
}

void CBidOptionsPropSheet::on_kjtXes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KJTX_E_S] = KJTx_E_S;

}

void CBidOptionsPropSheet::on_Ktnxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KTNX_E_S] = kTNX_E_S;
}

void CBidOptionsPropSheet::on_kTnxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KTNX_E_S] = KtNX_E_S;
}

void CBidOptionsPropSheet::on_ktNxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KTNX_E_S] = KTnX_E_S;

}

void CBidOptionsPropSheet::on_ktnXes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][KTNX_E_S] = KTNx_E_S;
}

void CBidOptionsPropSheet::on_Qtnxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][QTNX_E_S] = qTNX_E_S;
}

void CBidOptionsPropSheet::on_qTnxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][QTNX_E_S] = QtNX_E_S;
}

void CBidOptionsPropSheet::on_qtNxes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][QTNX_E_S] = QTnX_E_S;
}

void CBidOptionsPropSheet::on_qtnXes_clicked()
{
    bidOptionDoc.openingLead[EW_SUIT][QTNX_E_S] = QTNx_E_S;
}

void CBidOptionsPropSheet::on_Xxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XX_N_NT] = xX_N_NT;
}

void CBidOptionsPropSheet::on_xXnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XX_N_NT] = Xx_N_NT;
}

void CBidOptionsPropSheet::on_Xxxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XXX_N_NT] = xXX_N_NT;
}

void CBidOptionsPropSheet::on_xXxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XXX_N_NT] = XxX_N_NT;
}

void CBidOptionsPropSheet::on_xxXnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XXX_N_NT] = XXx_N_NT;
}

void CBidOptionsPropSheet::on_Akjxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][AKJX_N_NT] = aKJX_N_NT;
}

void CBidOptionsPropSheet::on_aKjxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][AKJX_N_NT] = AkJX_N_NT;
}

void CBidOptionsPropSheet::on_akJxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][AKJX_N_NT] = AKjX_N_NT;
}

void CBidOptionsPropSheet::on_akjXnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][AKJX_N_NT] = AKJx_N_NT;
}

void CBidOptionsPropSheet::on_Ajtnnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][AJTN_N_NT] = aJTN_N_NT;
}

void CBidOptionsPropSheet::on_aJtnnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][AJTN_N_NT] = AjTN_N_NT;
}

void CBidOptionsPropSheet::on_ajTnnnn_clicked()
{
    bidOptionDoc.openingLead[NS_NT][AJTN_N_NT] = AJtN_N_NT;
}

void CBidOptionsPropSheet::on_ajtNnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][AJTN_N_NT] = AJTn_N_NT;
}

void CBidOptionsPropSheet::on_Kqjxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][KQJX_N_NT] = kQJX_N_NT;
}

void CBidOptionsPropSheet::on_kQjxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][KQJX_N_NT] = KqJX_N_NT;
}

void CBidOptionsPropSheet::on_kqJxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][KQJX_N_NT] = KQjX_N_NT;
}

void CBidOptionsPropSheet::on_kqjXnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][KQJX_N_NT] = KQJx_N_NT;
}

void CBidOptionsPropSheet::on_Qktxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][QKTX_N_NT] = qKTX_N_NT;
}

void CBidOptionsPropSheet::on_qKtxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][QKTX_N_NT] = QkTX_N_NT;
}

void CBidOptionsPropSheet::on_qkTxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][QKTX_N_NT] = QKtX_N_NT;
}

void CBidOptionsPropSheet::on_qktXnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][QKTX_N_NT] = QKTx_N_NT;
}

void CBidOptionsPropSheet::on_Jtnxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][JTNX_N_NT] = jTNX_N_NT;
}

void CBidOptionsPropSheet::on_jTnxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][JTNX_N_NT] = JtNX_N_NT;
}

void CBidOptionsPropSheet::on_jtNxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][JTNX_N_NT] = JTnX_N_NT;
}

void CBidOptionsPropSheet::on_jtnXnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][JTNX_N_NT] = JTNx_N_NT;
}

void CBidOptionsPropSheet::on_Xxxxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XXXX_N_NT] = xXXX_N_NT;
}

void CBidOptionsPropSheet::on_xXxxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XXXX_N_NT] = XxXX_N_NT;
}

void CBidOptionsPropSheet::on_xxXxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XXXX_N_NT] = XXxX_N_NT;
}

void CBidOptionsPropSheet::on_xxxXnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XXXX_N_NT] = XXXx_N_NT;
}

void CBidOptionsPropSheet::on_Xxxxxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XXXXX_N_NT] = xXXXX_N_NT;
}

void CBidOptionsPropSheet::on_xXxxxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XXXXX_N_NT] = XxXXX_N_NT;
}

void CBidOptionsPropSheet::on_xxXxxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XXXXX_N_NT] = XXxXX_N_NT;
}

void CBidOptionsPropSheet::on_xxxXxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XXXXX_N_NT] = XXXxX_N_NT;
}

void CBidOptionsPropSheet::on_xxxxXnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XXXXX_N_NT] = XXXXx_N_NT;
}

void CBidOptionsPropSheet::on_Aqjxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][AQJX_N_NT] = aQJX_N_NT;
}

void CBidOptionsPropSheet::on_aQjxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][AQJX_N_NT] = AqJX_N_NT;
}

void CBidOptionsPropSheet::on_aqJxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][AQJX_N_NT] = AQjX_N_NT;
}

void CBidOptionsPropSheet::on_aqjXnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][AQJX_N_NT] = AQJx_N_NT;
}

void CBidOptionsPropSheet::on_Atnxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][ATNX_N_NT] = aTNX_N_NT;
}

void CBidOptionsPropSheet::on_aTnxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][ATNX_N_NT] = AtNX_N_NT;
}

void CBidOptionsPropSheet::on_atNxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][ATNX_N_NT] = ATnX_N_NT;
}

void CBidOptionsPropSheet::on_atnXnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][ATNX_N_NT] = ATNx_N_NT;
}

void CBidOptionsPropSheet::on_Kqtnnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][KQTN_N_NT] = kQTN_N_NT;
}

void CBidOptionsPropSheet::on_kQtnnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][KQTN_N_NT] = KqTN_N_NT;
}

void CBidOptionsPropSheet::on_kqTnnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][KQTN_N_NT] = KQtN_N_NT;
}

void CBidOptionsPropSheet::on_kqtNnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][KQTN_N_NT] = KQTn_N_NT;
}

void CBidOptionsPropSheet::on_Qtnxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][QTNX_N_NT] = qTNX_N_NT;
}

void CBidOptionsPropSheet::on_qTnxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][QTNX_N_NT] = QtNX_N_NT;
}

void CBidOptionsPropSheet::on_qtNxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][QTNX_N_NT] = QTnX_N_NT;
}

void CBidOptionsPropSheet::on_qtnXnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][QTNX_N_NT] = QTNx_N_NT;
}

void CBidOptionsPropSheet::on_Tnxxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][TNXX_N_NT] = tNXX_N_NT;
}

void CBidOptionsPropSheet::on_tNxxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][TNXX_N_NT] = TnXX_N_NT;
}

void CBidOptionsPropSheet::on_tnXxnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][TNXX_N_NT] = TNxX_N_NT;
}

void CBidOptionsPropSheet::on_tnxXnnt_clicked()
{
    bidOptionDoc.openingLead[NS_NT][TNXX_N_NT] = TNXx_N_NT;
}

void CBidOptionsPropSheet::on_Xxent_clicked()
{
    bidOptionDoc.openingLead[NS_NT][XX_N_NT] = xX_N_NT;
}

void CBidOptionsPropSheet::on_xXent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][XX_E_NT] = Xx_E_NT;
}

void CBidOptionsPropSheet::on_Xxxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][XXX_E_NT] = xXX_E_NT;
}

void CBidOptionsPropSheet::on_xXxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][XXX_E_NT] = XxX_E_NT;
}

void CBidOptionsPropSheet::on_xxXent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][XXX_E_NT] = XXx_E_NT;
}

void CBidOptionsPropSheet::on_Akjxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][AKJX_E_NT] = aKJX_E_NT;
}

void CBidOptionsPropSheet::on_aKjxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][AKJX_E_NT] = AkJX_E_NT;
}

void CBidOptionsPropSheet::on_ekJxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][AKJX_E_NT] = AKjX_E_NT;
}

void CBidOptionsPropSheet::on_akjXent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][AKJX_E_NT] = AKJx_E_NT;
}

void CBidOptionsPropSheet::on_Ajtnent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][AJTN_E_NT] = aJTN_E_NT;
}

void CBidOptionsPropSheet::on_aJtnent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][AJTN_E_NT] = AjTN_E_NT;
}

void CBidOptionsPropSheet::on_ajTnent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][AJTN_E_NT] = AJtN_E_NT;
}

void CBidOptionsPropSheet::on_ajtNent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][AJTN_E_NT] = AJTn_E_NT;
}

void CBidOptionsPropSheet::on_Kajxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][KQJX_E_NT] = kQJX_E_NT;
}

void CBidOptionsPropSheet::on_kQjxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][KQJX_E_NT] = KqJX_E_NT;
}

void CBidOptionsPropSheet::on_kqJxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][KQJX_E_NT] = KQjX_E_NT;
}

void CBidOptionsPropSheet::on_kqjXent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][KQJX_E_NT] = KQJx_E_NT;
}

void CBidOptionsPropSheet::on_Qjtxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][QKTX_E_NT] = qKTX_E_NT;
}

void CBidOptionsPropSheet::on_qJtxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][QKTX_E_NT] = QkTX_E_NT;
}

void CBidOptionsPropSheet::on_qjTxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][QKTX_E_NT] = QKtX_E_NT;
}

void CBidOptionsPropSheet::on_qjtXent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][QKTX_E_NT] = QKTx_E_NT;
}

void CBidOptionsPropSheet::on_Jtnxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][JTNX_E_NT] = jTNX_E_NT;
}

void CBidOptionsPropSheet::on_jTnxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][JTNX_E_NT] = JtNX_E_NT;
}

void CBidOptionsPropSheet::on_jtNxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][JTNX_E_NT] = JTnX_E_NT;
}

void CBidOptionsPropSheet::on_jtnXent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][JTNX_E_NT] = JTNx_E_NT;
}

void CBidOptionsPropSheet::on_Xxxxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][XXXX_E_NT] = xXXX_E_NT;
}

void CBidOptionsPropSheet::on_xXxxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][XXXX_E_NT] = XxXX_E_NT;
}

void CBidOptionsPropSheet::on_xxXxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][XXXX_E_NT] = XXxX_E_NT;
}

void CBidOptionsPropSheet::on_xxxXent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][XXXX_E_NT] = XXXx_E_NT;
}

void CBidOptionsPropSheet::on_Xxxxxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][XXXXX_E_NT] = xXXXX_E_NT;
}

void CBidOptionsPropSheet::on_xXxxxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][XXXXX_E_NT] = XxXXX_E_NT;
}

void CBidOptionsPropSheet::on_xxXxxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][XXXXX_E_NT] = XXxXX_E_NT;
}

void CBidOptionsPropSheet::on_xxxXxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][XXXXX_E_NT] = XXXxX_E_NT;
}

void CBidOptionsPropSheet::on_xxxxXent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][XXXXX_E_NT] = XXXXx_E_NT;
}

void CBidOptionsPropSheet::on_Aqjxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][AQJX_E_NT] = aQJX_E_NT;
}

void CBidOptionsPropSheet::on_aQjxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][AQJX_E_NT] = AqJX_E_NT;
}

void CBidOptionsPropSheet::on_aqJxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][AQJX_E_NT] = AQjX_E_NT;
}

void CBidOptionsPropSheet::on_aqjXent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][AQJX_E_NT] = AQJx_E_NT;
}

void CBidOptionsPropSheet::on_Atnxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][ATNX_E_NT] = aTNX_E_NT;
}

void CBidOptionsPropSheet::on_aTnxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][ATNX_E_NT] = AtNX_E_NT;
}

void CBidOptionsPropSheet::on_atNxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][ATNX_E_NT] = ATnX_E_NT;
}

void CBidOptionsPropSheet::on_atnXent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][ATNX_E_NT] = ATNx_E_NT;
}

void CBidOptionsPropSheet::on_Kqt9ent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][KQTN_E_NT] = kQTN_E_NT;
}

void CBidOptionsPropSheet::on_kQtnent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][KQTN_E_NT] = KqTN_E_NT;
}

void CBidOptionsPropSheet::on_kqTnent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][KQTN_E_NT] = KQtN_E_NT;
}

void CBidOptionsPropSheet::on_kqtNent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][KQTN_E_NT] = KQTn_E_NT;
}

void CBidOptionsPropSheet::on_Qtnxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][QTNX_E_NT] = qTNX_E_NT;
}

void CBidOptionsPropSheet::on_qTnxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][QTNX_E_NT] = QtNX_E_NT;
}

void CBidOptionsPropSheet::on_qtNxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][QTNX_E_NT] = QTnX_E_NT;
}

void CBidOptionsPropSheet::on_qtnXent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][QTNX_E_NT] = QTNx_E_NT;
}

void CBidOptionsPropSheet::on_Tnxxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][TNXX_E_NT] = tNXX_E_NT;
}

void CBidOptionsPropSheet::on_tNxxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][TNXX_E_NT] = TnXX_E_NT;
}

void CBidOptionsPropSheet::on_tnXxent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][TNXX_E_NT] = TNxX_E_NT;
}

void CBidOptionsPropSheet::on_tnxXent_clicked()
{
    bidOptionDoc.openingLead[EW_NT][TNXX_E_NT] = TNXx_E_NT;
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
