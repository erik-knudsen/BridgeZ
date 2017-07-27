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
    void on_fourthBest_clicked();
    void on_thirdBest_clicked();
    void on_lowEncouraging_clicked();
    //Opening leads in suit contracts.
    void on_Aks_clicked();
    void on_aKs_clicked();
    void on_Xxxs_clicked();
    void on_xXxs_clicked();
    void on_xxXs_clicked();
    //Opening leads in NT contracts.
    void on_Aknt_clicked();
    void on_aKnt_clicked();
    void on_Xxxnt_clicked();
    void on_xXxnt_clicked();
    void on_xxXnt_clicked();

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
