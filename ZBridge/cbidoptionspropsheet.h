/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidOptionsPropSheet.h
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

#ifndef CBIDOPTIONSPROPSHEET_H
#define CBIDOPTIONSPROPSHEET_H

#include <QDialog>

#include "cbidoptiondoc.h"

class CZBridgeApp;
class CZBridgeDoc;
//class CBidOptionDoc;

namespace Ui {
class CBidOptionsPropSheet;
}

/**
 * @brief Property sheet for selecting bid options.
 */
class CBidOptionsPropSheet : public QDialog
{
    Q_OBJECT
    
public:
    explicit CBidOptionsPropSheet(CBidOptionDoc &bidOption, CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent = 0);
    ~CBidOptionsPropSheet();

    CBidOptionDoc &getBidOption() { return bidOptionDoc; }
    
private slots:
    //--------------------Bidding and Play Conventions.--------------------------
    //Opening leads.
    void on_openingLeadType_currentIndexChanged(int index);
    void on_fourthBest_clicked();
    void on_thirdBest_clicked();
    void on_lowEncouraging_clicked();
    //N/S opening leads in NT contracts.
    void on_Xxnnt_clicked();
    void on_xXnnt_clicked();
    void on_Xxxnnt_clicked();
    void on_xXxnnt_clicked();
    void on_xxXnnt_clicked();
    void on_Akjxnnt_clicked();
    void on_aKjxnnt_clicked();
    void on_akJxnnt_clicked();
    void on_akjXnnt_clicked();
    void on_Ajtnnnt_clicked();
    void on_aJtnnnt_clicked();
    void on_ajTnnnn_clicked();
    void on_ajtNnnt_clicked();
    void on_Kqjxnnt_clicked();
    void on_kQjxnnt_clicked();
    void on_kqJxnnt_clicked();
    void on_kqjXnnt_clicked();
    void on_Qktxnnt_clicked();
    void on_qKtxnnt_clicked();
    void on_qkTxnnt_clicked();
    void on_qktXnnt_clicked();
    void on_Jtnxnnt_clicked();
    void on_jTnxnnt_clicked();
    void on_jtNxnnt_clicked();
    void on_jtnXnnt_clicked();
    void on_Xxxxnnt_clicked();
    void on_xXxxnnt_clicked();
    void on_xxXxnnt_clicked();
    void on_xxxXnnt_clicked();
    void on_Xxxxxnnt_clicked();
    void on_xXxxxnnt_clicked();
    void on_xxXxxnnt_clicked();
    void on_xxxXxnnt_clicked();
    void on_xxxxXnnt_clicked();
    void on_Aqjxnnt_clicked();
    void on_aQjxnnt_clicked();
    void on_aqJxnnt_clicked();
    void on_aqjXnnt_clicked();
    void on_Atnxnnt_clicked();
    void on_aTnxnnt_clicked();
    void on_atNxnnt_clicked();
    void on_atnXnnt_clicked();
    void on_Kqtnnnt_clicked();
    void on_kQtnnnt_clicked();
    void on_kqTnnnt_clicked();
    void on_kqtNnnt_clicked();
    void on_Qtnxnnt_clicked();
    void on_qTnxnnt_clicked();
    void on_qtNxnnt_clicked();
    void on_qtnXnnt_clicked();
    void on_Tnxxnnt_clicked();
    void on_tNxxnnt_clicked();
    void on_tnXxnnt_clicked();
    void on_tnxXnnt_clicked();
    //E/W opening leads in suit contracts.
    void on_Xxes_clicked();
    void on_xXes_clicked();
    void on_Xxxes_clicked();
    void on_xXxes_clicked();
    void on_xxXes_clicked();
    void on_Akxes_clicked();
    void on_aKxes_clicked();
    void on_akXes_clicked();
    void on_Kqxes_clicked();
    void on_kQxes_clicked();
    void on_kqXes_clicked();
    void on_Qjxes_clicked();
    void on_qJxes_clicked();
    void on_qjXes_clicked();
    void on_Jtxes_clicked();
    void on_jTxes_clicked();
    void on_jtXes_clicked();
    void on_Kqtnes_clicked();
    void on_kQtnes_clicked();
    void on_kqTnes_clicked();
    void on_kqtNes_clicked();
    void on_Xxxxes_clicked();
    void on_xXxxes_clicked();
    void on_xxXxes_clicked();
    void on_xxxXes_clicked();
    void on_Xxxxxes_clicked();
    void on_xXxxxes_clicked();
    void on_xxXxxes_clicked();
    void on_xxxXxes_clicked();
    void on_xxxxXes_clicked();
    void on_Tnxes_clicked();
    void on_tNxes_clicked();
    void on_tnXes_clicked();
    void on_Kjtxes_clicked();
    void on_kJtxes_clicked();
    void on_kjTxes_clicked();
    void on_kjtXes_clicked();
    void on_Ktnxes_clicked();
    void on_kTnxes_clicked();
    void on_ktNxes_clicked();
    void on_ktnXes_clicked();
    void on_Qtnxes_clicked();
    void on_qTnxes_clicked();
    void on_qtNxes_clicked();
    void on_qtnXes_clicked();
    //N/S opening leads in suit contracts.
    void on_Xxns_clicked();
    void on_xXns_clicked();
    void on_Xxxns_clicked();
    void on_xXxns_clicked();
    void on_xxXns_clicked();
    void on_Akxns_clicked();
    void on_aKxns_clicked();
    void on_akXns_clicked();
    void on_Kqxns_clicked();
    void on_kQxns_clicked();
    void on_kqXns_clicked();
    void on_Qjxns_clicked();
    void on_qJxns_clicked();
    void on_qjXns_clicked();
    void on_Jtxns_clicked();
    void on_jTxns_clicked();
    void on_jtXns_clicked();
    void on_Kqtnns_clicked();
    void on_kQtnnx_clicked();
    void on_kqTnns_clicked();
    void on_kqtNns_clicked();
    void on_Xxxxns_clicked();
    void on_xXxxns_clicked();
    void on_xxXxns_clicked();
    void on_xxxXns_clicked();
    void on_Xxxxxns_clicked();
    void on_xXxxxns_clicked();
    void on_xxXxxns_clicked();
    void on_xxxXxns_clicked();
    void on_xxxxXns_clicked();
    void on_Tnxns_clicked();
    void on_tNxns_clicked();
    void on_tnXns_clicked();
    void on_Kjtxns_clicked();
    void on_kJtxns_clicked();
    void on_kjTxns_clicked();
    void on_kjtXns_clicked();
    void on_Ktnxns_clicked();
    void on_kTnxns_clicked();
    void on_ktNxns_clicked();
    void on_ktnXns_clicked();
    void on_Qtnxns_clicked();
    void on_qTnxns_clicked();
    void on_qtNxns_clicked();
    void on_qtnXnx_clicked();
    //E/W opening leads in NT contracts.
    void on_Xxent_clicked();
    void on_xXent_clicked();
    void on_Xxxent_clicked();
    void on_xXxent_clicked();
    void on_xxXent_clicked();
    void on_Akjxent_clicked();
    void on_aKjxent_clicked();
    void on_ekJxent_clicked();
    void on_akjXent_clicked();
    void on_Ajtnent_clicked();
    void on_aJtnent_clicked();
    void on_ajTnent_clicked();
    void on_ajtNent_clicked();
    void on_Kajxent_clicked();
    void on_kQjxent_clicked();
    void on_kqJxent_clicked();
    void on_kqjXent_clicked();
    void on_Qjtxent_clicked();
    void on_qJtxent_clicked();
    void on_qjTxent_clicked();
    void on_qjtXent_clicked();
    void on_Jtnxent_clicked();
    void on_jTnxent_clicked();
    void on_jtNxent_clicked();
    void on_jtnXent_clicked();
    void on_Xxxxent_clicked();
    void on_xXxxent_clicked();
    void on_xxXxent_clicked();
    void on_xxxXent_clicked();
    void on_Xxxxxent_clicked();
    void on_xXxxxent_clicked();
    void on_xxXxxent_clicked();
    void on_xxxXxent_clicked();
    void on_xxxxXent_clicked();
    void on_Aqjxent_clicked();
    void on_aQjxent_clicked();
    void on_aqJxent_clicked();
    void on_aqjXent_clicked();
    void on_Atnxent_clicked();
    void on_aTnxent_clicked();
    void on_atNxent_clicked();
    void on_atnXent_clicked();
    void on_Kqt9ent_clicked();
    void on_kQtnent_clicked();
    void on_kqTnent_clicked();
    void on_kqtNent_clicked();
    void on_Qtnxent_clicked();
    void on_qTnxent_clicked();
    void on_qtNxent_clicked();
    void on_qtnXent_clicked();
    void on_Tnxxent_clicked();
    void on_tNxxent_clicked();
    void on_tnXxent_clicked();
    void on_tnxXent_clicked();

    //Config Bidding aggressiveness.
    void on_biddingAggressiveness_valueChanged(int value);

    //General configuration.
    void on_americanStyle_clicked();
    void on_europeanStyle_clicked();
    void on_majorFour_clicked();
    void on_majorFive_clicked();
    void on_blackwood_clicked(bool checked);
    void on_gerber_clicked(bool checked);
    void on_cueBids_clicked(bool checked);
    void on_stayman_clicked(bool checked);
    void on_staymanNF_clicked();
    void on_staymanF_clicked();
    void on_jacobyTransfers_clicked(bool checked);
    void on_fourSuitTransfers_clicked(bool checked);
    void on_limitRaises_clicked(bool checked);
    void on_jacoby2NT_clicked(bool checked);
    void on_splinterBids_clicked(bool checked);
    void on_fourthSuitForcing_clicked(bool checked);

    //2-Bids configuration.
    void on_weakTwo_clicked(bool checked);    
    void on_w2Ogust_clicked();
    void on_w2Feature_clicked();
    void on_w2NewSuitNF_clicked();
    void on_w2NewSuitF_clicked();
    void on_openValue20_clicked();
    void on_openValue21_clicked();
    void on_openValue22_clicked();
    void on_openValue23_clicked();
    void on_twoCtwoDNegative_clicked();
    void on_twoCtwoDWaiting_clicked();

    //NT Range configuration. configuration.
    void on_oneNTRange12_14_clicked();
    void on_oneNTRange15_17_clicked();
    void on_oneNTRange16_18_clicked();
    void on_twoNTRange20_21_clicked();    
    void on_open1NTWeakMajor_clicked(bool checked);
    void on_twoNTRange21_22_clicked();
    void on_twoNTRange22_24_clicked();
    void on_threNTRange25_27_clicked();
    void on_gambling3NT_clicked();
    void on_gambling3NT_ACOL_clicked();

    //NT Other.
    void on_twoNT111820_clicked();
    void on_twoNT111718_clicked();
    void on_twoNT211518_clicked();
    void on_twoNT211214_clicked();
    void on_rebid1NTNat_clicked();
    void on_rebid1NTF_clicked();
    void on_oneNT3mSI_clicked();
    void on_oneNT3mGI_clicked();
    void on_oneNT3mPE_clicked();
    void on_oneNT3MSI_clicked();
    void on_oneNT3MGF_clicked();
    void on_oneNT3MGI_clicked();
    void on_oneNT3MPE_clicked();

    //Misc. configuration.
    void on_structuredReverses_clicked(bool checked);
    void on_takeOutDoubles_clicked(bool checked);
    void on_negativeDoubles_clicked(bool checked);
    void on_michaelsCueBid_clicked(bool checked);
    void on_unusualNT_clicked(bool checked);
    void on_drury_clicked(bool checked);
    void on_jumpOvercallWeak_clicked(bool checked);   
    void on_wjoNatural_clicked();
    void on_wjoOgust_clicked();
    void on_wjoFeature_clicked();
    void on_soundThreeLevel_clicked();
    void on_lightThreeLevel_clicked();
    void on_lightThreeLevelNV_clicked();

    //Opening configuration.
    void on_elevenHCPsRbsLm_clicked(bool checked);
    void on_elevenHCPs6Cs_clicked(bool checked);
    void on_fourteenTPsGs_clicked(bool checked);
    void on_fourteenTPsLs_clicked(bool checked);
    void on_thirteeTPsLs_clicked(bool checked);

    //Signals configuration.
    void on_signalPartnerLeadSuit_currentIndexChanged(int index);
    void on_signalDeclarerLeadSuit_currentIndexChanged(int index);
    void on_signalDiscardingSuit_currentIndexChanged(int index);
    void on_signalPartnerLeadNT_currentIndexChanged(int index);
    void on_signalDeclarerLeadNT_currentIndexChanged(int index);
    void on_signalDiscardingNT_currentIndexChanged(int index);

private:
    void setDependencies();

    Ui::CBidOptionsPropSheet *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
    CBidOptionDoc bidOptionDoc;
};

#endif // CBIDOPTIONSPROPSHEET_H
