/*Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CBidDBDefine.cpp
  Developers: eak

  Revision History:
  1-dec-2016 eak: Original

  Abstract: Defines for bidding database.

  Platforms: Qt.

*/

#include "cbiddbdefine.h"

CBidDBDefine::CBidDBDefine(CBidOptionDoc &nsBidOptions, CBidOptionDoc &ewBidOptions)
{
    calcIds(nsBidOptions, nsPages, nsRules);
    calcIds(ewBidOptions, ewPages, ewRules);
}

QSet<qint16> &CBidDBDefine::getPages(Seat seat)
{
    return ((seat == NORTH_SEAT) || (seat == SOUTH_SEAT)) ? nsPages : ewPages;
}

QSet<qint16> &CBidDBDefine::getRules(Seat seat)
{
    return ((seat == NORTH_SEAT) || (seat == SOUTH_SEAT)) ? nsRules : ewRules;
}

/**
 * @brief Calculate relevant pages and rule ids.
 *
 * @param bidOptions Options that selects pages and rules.
 * @param pages Calculated pages.
 * @param rules Calculated rule ids.
 */
void CBidDBDefine::calcIds(CBidOptionDoc &bidOptions, QSet<qint16> &pages, QSet<qint16> &rules)
{
    //Pages*****************************************************************************************
    //Common for all.
    pages<<OR_COM_1<<OR_COM_2<<RR_COM<<OR_1m_2m_FL<<D_C_COMMON<<D_D_COMMON<<D_H_COMMON<<D_S_COMMON;
    pages<<D_C1_916<<D_D1_916<<D_H1_916<<D_S1_916<<D_C_12<<D_D_12<<D_H_12<<D_S_12<<D_OPS_COMMON;
    pages<<D_OVS_COMMON<<D_OVS_RDP<<D_OVS_CONO<<D_OVS_RD_1<<D_OVS_RD_2<<D_RO_COMMON<<D_O_IND<<D_mLF;
    pages<<D_SNT_OFF<<D_B_PO_COMMON<<D_RB_COMMON_1<<D_RB_SD_OFF<<D_RB_COMMON_2<<D_RB_mLF<<D_RB_ACO;
    pages<<D_RB_MO<<D_RB_MO_OFF;

    //Weak jump overcall (6-9 HCP).
    if (bidOptions.jumpOvercalls != JUMP_OVERCALL_STRONG)
    {
        pages<<D_C2_1116_69<<D_D2_1116_69<<D_H2_1116_69<<D_S2_1116_69<<D_WJO_SNT_OFF;
      if (bidOptions.jumpOvercalls == JUMP_OVERCALL_WEAK_NATURAL)
            pages<<D_OVS_R_WJO_N;
        else if (bidOptions.jumpOvercalls == JUMP_OVERCALL_WEAK_OGUST)
            pages<<D_OVS_R_WJO_O;
        else if (bidOptions.jumpOvercalls == JUMP_OVERCALL_WEAK_FEATURE)
            pages<<D_OVS_R_WJO_F;
    }
    //Intermediate jump overcall (13-16 HCP).
    else
        pages<<D_C2_1116_1316<<D_D2_1116_1316<<D_H2_1116_1316<<D_S2_1116_1316<<D_OVS_R_IJO<<D_IJO_SNT_OFF;

    //Strong cuebid.
    if (bidOptions.directCueBid == STRONG_CUE_BID)
        pages<<D_C_SCB<<D_D_SCB<<D_H_SCB<<D_S_SCB<<D_OPS_SCB<<D_OVS_R_SCB<<D_MCB_OFF;
    //Michaels cue bid.
    else
        pages<<D_C_MCB<<D_D_MCB<<D_H_MCB<<D_S_MCB<<D_OPS_MCB<<D_OVS_R_MCB<<D_MCB_ON;

    //Penalty double against overcall.
    if (!bidOptions.negativeDoubles)
        pages<<D_OPS_PD;
    //Negative double against overcall.
    else
        pages<<D_OPS_ND<<D_OVS_ND;

    //American.
    if (bidOptions.bidStyle ==AMERICAN_STYLE)
    {
        pages<< ORA_1C_1N<<ORA_1m_2N<<ORA_1m_3N;

        //Forcing raise - minor and major.
        if (!bidOptions.limitRaises)
            pages<<OR_1m_3m_F<<OR_1M_3M_F<<RR_4N;   //Always 4. suit natural.

        //Limit raise - minor and major.
        else
            pages<<OR_1m_3m_L<<OR_1M_3M_L<<D_OPS_L;
    }

    //European (always simple limit).
    else
    {
        pages<<ORE_1C_1N<<ORE_1m_2N<<ORE_1m_3N<<RR_4F;
        pages<<OR_1m_3m_L<<OR_1M_3M_L<<D_OPS_L;

        //Simple limit raise only.
        if (!bidOptions.jacoby2NT)
        {
            pages<<ORE45_1M_2N_N;

            //Splinter.
            if (bidOptions.splinterbids)
                pages<<E45_L_S<<OR_SPLINTER<<RR_S;
            else
                pages<<E45_SL;
        }
        //Simple limit raise + 2NT major raise forcing (Jacoby 2NT).
        else
            pages<<ORE45_1M_2N_J<<E45_L_S<<OR_SPLINTER<<RR_S; //Always Splinter.
    }

    //Weak 1NT.
    if (bidOptions.oneNTRange == ONE_NT_RANGE_12_14)
    {
        pages<<D_W<<D_W_NO<<D_W_N2C_N<<D_W_N2D_N<<D_W_N2H2S_N<<D_B_PO_W;

        //2D/2H/2S sign off.
        if (!bidOptions.jacobyTransfers)
            pages<<D_W_SO;
        //Jacoby.
        else
        {
            pages<<D_W_J;
            //2S sign off.
            //2S,2N minor transfer.
            if (bidOptions.fourSuitTransfers)
                pages<<D_W_J_MT;
        }

        //3C/3D slam invitation or 3C/3D game invitation.
        if ((bidOptions.oneNT3m == ONE_NT_3m_SI) || (bidOptions.oneNT3m == ONE_NT_3M_GI))
            pages<<D_W_3mSGI;
        //3C/3D pre emptive.
        else
            pages<<D_W_3mPE;

        //3H/3S slam invitation or game forcing or game invitation.
        if ((bidOptions.oneNT3M == ONE_NT_3M_SI) || (bidOptions.oneNT3M == ONE_NT_3M_GF) ||
            (bidOptions.oneNT3M == ONE_NT_3M_GI))
            pages<<D_W_3MSGI;
        //3H/3S pre emptive.
        else
            pages<<D_W_3MPE;

        //4H/4S natural.
        pages<<D_W_4MN;

        //2NT after 1/1 18-20 HCP.
        if (bidOptions.twoNT11 == TWO_NT_11_18_20)
            //New minor natural (after 1NT rebid).
             if (bidOptions.rebid1NT == REBID_1NT_NAT)
                pages<<RR_WN_1820_N;
            //New minor forcing (after 1NT rebid).
            else
                pages<<RR_WN_1820_F;
        //2NT after 1/1 17-18 HCP.
        else
            //New minor natural (after 1NT rebid).
            if (bidOptions.rebid1NT == REBID_1NT_NAT)
                pages<<RR_WN_1718_N;
            //New minor forcing (after 1NT rebid).
            else
                pages<<RR_WN_1718_F;
    }


    //Strong 1NT.
    if (bidOptions.oneNTRange != ONE_NT_RANGE_12_14)
    {
        pages<<D_S<<D_S_NO<<D_S_N2C_N<<D_S_N2D_N<<D_S_N2H2S_N<<D_B_PO_S;

        //2D/2H/2S sign off.
        if (!bidOptions.jacobyTransfers)
            pages<<D_S_SO;
        //Jacoby.
        else
        {
            pages<<D_S_J;
            //2S sign off.
            //2S,2N minor transfer.
            if (bidOptions.fourSuitTransfers)
                pages<<D_S_J_MT;
        }

        //3C/3D slam invitation or 3C/3D game invitation.
        if ((bidOptions.oneNT3m == ONE_NT_3m_SI) || (bidOptions.oneNT3m == ONE_NT_3M_GI))
            pages<<D_S_3mSGI;
        //3C/3D pre emptive
        else
          pages<<D_S_3mPE;

        //3H/3S slam invitation or game forcing or game invitation.
        if ((bidOptions.oneNT3M == ONE_NT_3M_SI) || (bidOptions.oneNT3M == ONE_NT_3M_GF) ||
            (bidOptions.oneNT3M == ONE_NT_3M_GI))
            pages<<D_S_3MSGI;
        //3H/3S pre emptive.
        else
            pages<<D_S_3MPE;

        //4H/4S natural.
        pages<<D_S_4MN;

        //New minor natural (after 1NT rebid).
        if (bidOptions.rebid1NT == REBID_1NT_NAT)
            pages<<RR_SN_N;
        //New minor forcing (after 1NT rebid).
        else
            pages<<RR_SN_F;
    }

    //4-major.
    if (bidOptions.majorsMode == MAJOR_FOUR)
    {
        pages<<OR4_1H_1S<<OR4_1H_2<<OR4_1S_2<<OR4_1H<<OR4_1S<<OR4_1H_1N<<OR4_1S_1N<<OR4_1H_3N_N<<OR4_1S_3N_N;
        pages<<RR4_1D1H<<RR4_1S;

        //Penalty double against overcall.
        if (!bidOptions.negativeDoubles)
            pages<<D4_OPS_1H_PD<<D4_OPS_1S_PD;
        //Negative double against overcall.
        else
            pages<<D4_OPS_1H_ND<<D4_OPS_1S_ND;

        //Strong 1NT, lower first.
        if (bidOptions.oneNTRange != ONE_NT_RANGE_12_14)
        {
            pages<<O12_4M_SN_L<<O3_4M_SN_L<<O4_4M_SN_L;
            //New minor natural (after 1NT rebid).
            if (bidOptions.rebid1NT == REBID_1NT_NAT)
                pages<<RR4_SN_N;
            //New minor forcing (after 1NT rebid).
            else
                pages<<RR4_SN_F;
        }
        //Weak 1NT, lower first.
        else
        {
            pages<<O12_4M_WN_L<<O3_4M_WN_L<<O4_4M_WN_L;
            //2NT after 1/1 18-20 HCP.
            if (bidOptions.twoNT11 == TWO_NT_11_18_20)
                //New minor natural (after 1NT rebid).
                if (bidOptions.rebid1NT == REBID_1NT_NAT)
                    pages<<RR4_WN_1820_N;
                //New minor forcing (after 1NT rebid).
                else
                    pages<<RR4_WN_1820_F;
            //2NT after 1/1 17-18 HCP.
            else
                //New minor natural (after 1NT rebid).
                if (bidOptions.rebid1NT == REBID_1NT_NAT)
                    pages<<RR4_WN_1718_N;
                //New minor forcing (after 1NT rebid).
                else
                    pages<<RR4_WN_1718_F;
        }

        //American
        if (bidOptions.bidStyle == AMERICAN_STYLE)
        {
            pages<<ORA4_1M_2N_N<<ORA4_1M_3N_N;

            //Forcing raise - minor and major.
            if (!bidOptions.limitRaises)
                pages<<A4_FF<<RR4_4N;                       //Always 4. suit natural.
            else
            {
                pages<<ORE45_1M_2N_J;                       //Always Jacoby 2NT.
                pages<<A45_L_S<<OR_SPLINTER<<RR_S;          //Always Splinter.
                pages<<AE4_1H_2NF_S<<AE4_1S_2NF_S;
                pages<<RR4_4F_1D1H<<RR4_4F_1S<<RR_4F;       //Always 4. suit forcing.
            }
        }

        //European (always simple limit raise).
        else
        {
            pages<<RR4_4F_1D1H<<RR4_4F_1S;          //Always 4. suit forcing).

            //Simple limit raise only.
             if (!bidOptions.jacoby2NT)
             {
                 //Splinter.
                 if (bidOptions.splinterbids)
                    pages<<E4_1H_L_S<<E4_1S_L_S;
                 else
                    pages<<E4_1H_SL<<E4_1S_SL;
             }

             //Simple limit raise + 2NT major raise forcing (Jacoby 2NT).
             else
                 pages<<AE4_1H_2NF_S<<AE4_1S_2NF_S;
        }
    }

    //5-major.
    if (bidOptions.majorsMode == MAJOR_FIVE)
    {
        pages<<OR5_1H_1S<<OR5_1H_2<<OR5_1S_2<<OR5_1H<<OR5_1S<<OR5_1H_1N<<OR5_1S_1N<<OR5_1H_3N_N<<OR5_1S_3N_N;
        pages<<RR5_1D1H<<RR5_1S;

        //Penalty double against overcall.
        if (!bidOptions.negativeDoubles)
            pages<<D5_OPS_1H_PD<<D5_OPS_1S_PD;
        //Negative double against overcall.
        else
            pages<<D5_OPS_1H_ND<<D5_OPS_1S_ND;

        //Strong 1NT.
        if (bidOptions.oneNTRange != ONE_NT_RANGE_12_14)
        {
            pages<<O12_5M_SN<<O3_5M_SN<<O4_5M_SN;
            //New minor natural (after 1NT rebid).
            if (bidOptions.rebid1NT == REBID_1NT_NAT)
                pages<<RR5_SN_N;
            //New minor forcing (after 1NT rebid).
            else
                pages<<RR5_SN_F;
        }
        //Weak 1NT.
        else
        {
            pages<<O12_5M_WN<<O3_5M_WN<<O4_5M_WN;
            //2NT after 1/1 18-20 HCP.
            if (bidOptions.twoNT11 == TWO_NT_11_18_20)
                //New minor natural (after 1NT rebid).
                if (bidOptions.rebid1NT == REBID_1NT_NAT)
                    pages<<RR5_WN_1820_N;
                //New minor forcing (after 1NT rebid).
                else
                    pages<<RR5_WN_1820_F;
            //2NT after 1/1 17-18 HCP.
            else
                //New minor natural (after 1NT rebid).
                if (bidOptions.rebid1NT == REBID_1NT_NAT)
                    pages<<RR5_WN_1718_N;
                //New minor forcing (after 1NT rebid).
                else
                    pages<<RR5_WN_1718_F;
        }


        //American
        if (bidOptions.bidStyle == AMERICAN_STYLE)
        {
            pages<<ORA5_1M_2N_N<<ORA5_1M_3N_N;

            //Forcing raise - minor and major.
            if (!bidOptions.limitRaises)
                pages<<A5_FF<<RR5_4N;                           //Always 4. suit natural.
            else
            {
                if (bidOptions.jacoby2NT)
                {
                    pages<<ORE45_1M_2N_J;                       //AlwaysJacoby 2NT.
                    pages<<A45_L_S<<OR_SPLINTER<<RR_S;          //Always Splinter.
                    pages<<AE5_1H_2NF_S<<AE5_1S_2NF_S;
                    pages<<RR5_4F_1D1H<<RR5_4F_1S<<RR_4F;       //Always 4. suit forcing.
                }
                else
                {
                    pages<<A5_SL;
                }
            }
        }

        //European (always simple limit raise).
        else
        {
            pages<<RR5_4F_1D1H<<RR5_4F_1S;          //Always 4. suit forcing).

            //Simple limit raise only.
             if (!bidOptions.jacoby2NT)
             {
                 //Splinter.
                 if (bidOptions.splinterbids)
                    pages<<E5_1H_L_S<<E5_1S_L_S;
                 else
                    pages<<E5_1H_SL<<E5_1S_SL;
             }

             //Simple limit raise + 2NT major raise forcing (Jacoby 2NT).
             else
                 pages<<AE5_1H_2NF_S<<AE5_1S_2NF_S;
        }
    }

    //1NT 12-14 HCP.
    if (bidOptions.oneNTRange == ONE_NT_RANGE_12_14)
    {
        pages<<O12_1N_12_14<<O3_1N_12_14<<O4_1N_12_14<<R_1214<<OR_1214;

        //Stayman forcing.
        if ((bidOptions.stayman == STAYMAN_3R_F) || (bidOptions.stayman == STAYMAN_4R))
            pages<<R_1214_SF<<OR_1214_SF_1<<OR_1214_SF_2;
        //Stayman not forcing.
        else
            pages<<R_1214_SN<<OR_1214_SN_1<<OR_1214_SN_2;

        //2D/2H/2S sign off.
        if (!bidOptions.jacobyTransfers)
            pages<<R_1214_SO<<OR_1214_SO;
        //Jacoby.
        else
        {
            pages<<OR_1214_J;

            //2S sign off.
            if (!bidOptions.fourSuitTransfers)
                pages<<R_1214_J_SO<<OR_1214_J_SO;
            //2S,2N minor transfer.
            else
                pages<<R_1214_J_MT<<OR_1214_J_MT;
        }

        //3C/3D slam invitation.
        if (bidOptions.oneNT3m == ONE_NT_3m_SI)
            pages<<R_1214_3mSI<<OR_1214_3mSI;
        //3C/3D game invitation.
        else if (bidOptions.oneNT3m == ONE_NT_3M_GI)
            pages<<R_1214_3mGI<<OR_1214_3mGI;
        //3C/3D pre emptive.
          else
            pages<<R_1214_3mPE<<OR_1214_3mPE;

        //3H/3S slam invitation.
        if (bidOptions.oneNT3M == ONE_NT_3M_SI)
            pages<<R_1214_3MSI<<OR_1214_3MSI;
        //3H/3S game forcing.
        else if (bidOptions.oneNT3M == ONE_NT_3M_GF)
            pages<<R_1214_3MGF<<OR_1214_3MGF;
        //3H/3S pre emptive.
        else if (bidOptions.oneNT3M == ONE_NT_3M_PE)
            pages<<R_1214_3MPE<<OR_1214_3MPE;
        //3H/3S game invitation.
        else
            pages<<R_1214_3MGI<<OR_1214_3MGI;

        //4H/4S natural.
        pages<<R_1214_4MN;
    }

    //1NT 16-18 HCP.
    else if (bidOptions.oneNTRange == ONE_NT_RANGE_16_18)
    {
        pages<<O_1N_16_18<<R_1618<<OR_1618;

        //Stayman forcing.
        if ((bidOptions.stayman == STAYMAN_3R_F) || (bidOptions.stayman == STAYMAN_4R))
            pages<<R_1618_SF<<OR_1618_SF_1<<OR_1618_SF_2;
        //Stayman not forcing.
        else
            pages<<R_1618_SN<<OR_1618_SN_1<<OR_1618_SN_2;

        //2D/2H/2S sign off.
        if (!bidOptions.jacobyTransfers)
            pages<<R_1618_SO<<OR_1618_SO;
        //Jacoby.
        else
        {
            pages<<OR_1618_J;

            //2S sign off.
            if (!bidOptions.fourSuitTransfers)
                pages<<R_1618_J_SO<<OR_1618_J_SO;
            //2S,2N minor transfer.
            else
                pages<<R_1618_J_MT<<OR_1618_J_MT;
        }

        //3C/3D slam invitation.
        if (bidOptions.oneNT3m == ONE_NT_3m_SI)
            pages<<R_1618_3mSI<<OR_1618_3mSI;
        //3C/3D game invitation.
        else if (bidOptions.oneNT3m == ONE_NT_3M_GI)
            pages<<R_1618_3mGI<<OR_1618_3mGI;
        //3C/3D pre emptive.
        else
            pages<<R_1618_3mPE<<OR_1618_3mPE;

        //3H/3S slam invitation.
        if (bidOptions.oneNT3M == ONE_NT_3M_SI)
            pages<<R_1618_3MSI<<OR_1618_3MSI;
        //3H/3S game forcing
        else if (bidOptions.oneNT3M == ONE_NT_3M_GF)
            pages<<R_1618_3MGF<<OR_1618_3MGF;
        //3H/3S pre emptive.
        else if (bidOptions.oneNT3M == ONE_NT_3M_PE)
            pages<<R_1618_3MPE<<OR_1618_3MPE;
        //3H/3S game invitation.
        else
            pages<<R_1618_3MGI<<OR_1618_3MGI;

        //4H/4S natural.
        pages<<R_1618_4MN;
    }

    //15-17 HCP.
    if (bidOptions.oneNTRange == ONE_NT_RANGE_15_17)
    {
        pages<<O_1N_15_17<<R_1517<<OR_1517;

        //Stayman forcing.
        if ((bidOptions.stayman == STAYMAN_3R_F) || (bidOptions.stayman == STAYMAN_4R))
            pages<<R_1517_SF<<OR_1517_SF_1<<OR_1517_SF_2;
        //Stayman not forcing.
        else
            pages<<R_1517_SN<<OR_1517_SN_1<<OR_1517_SN_2;

        //2D/2H/2S sign off.
        if (!bidOptions.jacobyTransfers)
            pages<<R_1517_SO<<OR_1517_SO;
        //Jacoby.
        else
        {
            pages<<OR_1517_J;

            //2S sign off.
            if (!bidOptions.fourSuitTransfers)
                pages<<R_1517_J_SO<<OR_1517_J_SO;
            //2S,2N minor transfer.
            else
                pages<<R_1517_J_MT<<OR_1517_J_MT;
        }

        //3C/3D slam invitation.
        if (bidOptions.oneNT3m == ONE_NT_3m_SI)
            pages<<R_1517_3mSI<<OR_1517_3mSI;
        //3C/3D game invitation.
        else if (bidOptions.oneNT3m == ONE_NT_3M_GI)
            pages<<R_1517_3mGI<<OR_1517_3mGI;
        //3C/3D pre emptive.
        else
            pages<<R_1517_3mPE<<OR_1517_3mPE;

        //3H/3S slam invitation.
        if (bidOptions.oneNT3M == ONE_NT_3M_SI)
            pages<<R_1517_3MSI<<OR_1517_3MSI;
        //3H/3S game forcing
        else if (bidOptions.oneNT3M == ONE_NT_3M_GF)
            pages<<R_1517_3MGF<<OR_1517_3MGF;
        //3H/3S pre emptive.
        else if (bidOptions.oneNT3M == ONE_NT_3M_PE)
            pages<<R_1517_3MPE<<OR_1517_3MPE;
        //3H/3S game invitation.
        else
            pages<<R_1517_3MGI<<OR_1517_3MGI;

        //4H/4S natural.
        pages<<R_1517_4MN;
    }

    //Strong 2C. Weak 2D, 2H, 2S.
    if (bidOptions.twoBidsMode != S2_NATURAL)
    {
        pages<<O_S2_C<<A_S2_C;
        //Weak 2D, 6-11 HCP. //Weak 2H/2S, 6-11 HCP.
        pages<<O12_W2D_6_11<<O3_W2D_6_11<<O4_W2D_6_11;
        pages<<O12_W2H2S_6_11<<O3_W2H2S_6_11<<O4_W2H2S_6_11;
        pages<<A_W2D<<A_W2H2S<<A_W2DX_NAT<<A_W2H2SX_NAT;

        //Ogust after weak 2D, new suit not forcing. //Ogust after weak 2H/2S, new suit not forcing.
        if ((bidOptions.twoBidsMode == W2_OGUST) && (bidOptions.w2NewSuit == W2_NEW_SUIT_NF))
            pages<<A_W2D_O_NF<<A_W2H2S_O_NF;
        //Ogust after weak 2D, forcing. //Ogust after weak 2H/2S, forcing.
        else if ((bidOptions.twoBidsMode == W2_OGUST) && (bidOptions.w2NewSuit == W2_NEW_SUIT_F))
              pages<<A_W2D_O_F<<A_W2H2S_O_F;
        //Feature after weak 2D, not forcing. //Feature after weak 2H/2S, not forcing.
        else if ((bidOptions.twoBidsMode == W2_FEATURE) && (bidOptions.w2NewSuit == W2_NEW_SUIT_NF))
            pages<<A_W2D_F_NF<<A_W2H2S_F_NF;
        //Feature after weak 2D, forcing. //Feature after weak 2H/2S, forcing.
        else if ((bidOptions.twoBidsMode == W2_FEATURE) && (bidOptions.w2NewSuit == W2_NEW_SUIT_NF))
              pages<<A_W2D_F_F<<A_W2H2S_F_F;
    }

    //Strong two (2C, 2D, 2H, 2S).
    else
        pages<<O_S2_CDHS<<A_S2_CDHS;

    //2NT 20-21 HCP.
    if (bidOptions.twoNTRange == TWO_NT_RANGE_20_21)
    {
        pages<<O_2N_20_21<<A_2N_20_21<<A_2N_20_21_L;
        //Non Texas.
        pages<<A_2N_20_21_NT;
        //3C Stayman. 3D, 3H, 3S Natural.
        pages<<A_2N_20_21_SN;
        //3C Stayman. Later bids.
        pages<<A_2N_20_21_SL;
        //3D, 3H, 3S Natural. Later bids.
        pages<<A_2N_20_21_NL;
    }
    //2NT 21-22 HCP.
    else if (bidOptions.twoNTRange == TWO_NT_RANGE_21_22)
    {
        pages<<O_2N_21_22<<A_2N_21_22<<A_2N_21_22_L;
        //Non Texas.
        pages<<A_2N_21_22_NT;
        //3C Stayman. 3D, 3H, 3S Natural.
        pages<<A_2N_21_22_SN;
        //3C Stayman. Later bids.
        pages<<A_2N_21_22_SL;
        //3D, 3H, 3S Natural. Later bids.
        pages<<A_2N_21_22_NL;
    }
    //2NT 22-24 HCP.
    else if (bidOptions.twoNTRange == TWO_NT_RANGE_22_24)
    {
        pages<<O_2N_22_24<<A_2N_22_24<<A_2N_22_24_L;
        //Non Texas.
        pages<<A_2N_22_24_NT;
        //3C Stayman. 3D, 3H, 3S Natural.
        pages<<A_2N_22_24_SN;
        //3C Stayman. Later bids.
        pages<<A_2N_22_24_SL;
        //3D, 3H, 3S Natural. Later bids.
        pages<<A_2N_22_24_NL;
    }

    //2NT after 1/1 18-21 HCP
    if (bidOptions.twoNT11 == TWO_NT_11_18_20)
        pages<<RR_2N_11_1821;
    //2NT after 1/1 17-18 HCP
    else
        pages<<RR_2N_11_1718;

    //1NT 12-14HCP
    if (bidOptions.oneNTRange == ONE_NT_RANGE_12_14)
        pages<<RR45_2N_21_1214;
    //2NT after 2/1 15-18 HCP
    if (bidOptions.twoNT21 == TWO_NT_21_15_18)
        pages<<RR45_2N_21_1518;
    //2NT after 2/1 12-14 HCP
    else
        pages<<RR45_2N_21_1214;

    //After 2C-2D-2NT opening.
    //22-24 HCP.
    if (true)
    {
        pages<<A_2C2D2N_22_24;
        //Non Texas.
        pages<<A_2C2D2N_22_24_NT;
        //3C Stayman. 3D, 3H, 3S Natural.
        pages<<A_2C2D2N_22_24_SN;
        //3C Stayman. Later bids.
        pages<<A_2C2D2N_22_24_SL;
        //3D, 3H, 3S Natural. Later bids.
        pages<<A_2C2D2N_22_24_NL;
    }
    //After 2C-2D-2NT opening.
    //23-24 HCP.
    else
    {
        pages<<A_2C2D2N_23_24;
        //Non Texas.
        pages<<A_2C2D2N_23_24_NT;
        //3C Stayman. 3D, 3H, 3S Natural.
        pages<<A_2C2D2N_23_24_SN;
        //3C Stayman. Later bids.
        pages<<A_2C2D2N_23_24_SL;
        //3D, 3H, 3S Natural. Later bids.
        pages<<A_2C2D2N_23_24_NL;
    }

    //3. level.
    pages<<O12_3LEVEL<<O3_3LEVEL<<O4_3LEVEL<<A_3LEVEL;

    //3NT 25-27 HCP.
    if (bidOptions.threeNTRange == THREE_NT_RANGE_25_27)
        pages<<O_3N_25_27<<A_3N_25_27;
    //3NT gambling (1., 2. and 3. seat). //3NT gambling (4. seat).
    else if (bidOptions.threeNTRange == GAMBLING_3_NT)
        pages<<O123_3N_GMBL<<A_3N_GMBL<<O4_3N_GMBL;
    //3NT Acol.
    else if (bidOptions.threeNTRange == GAMBLING_3_NT_ACOL)
        pages<<O_3N_ACOL<<A_3N_ACOL;


    //Rules***************************************************************************************
    rules<<0;

    if (bidOptions.majorsMode == MAJOR_FOUR)
        rules<<O_1H_4<<O_1S_4;          //1H and 1S promises 4 card suit.
    else
        rules<<O_1H_5<<O_1S_5;          //1H and 1S promises 5 card suit.

    rules<<H5_2N<<O_LOWER;              //Open lower suit first.

    if (bidOptions.oneNTRange == ONE_NT_RANGE_12_14)
    {
        rules<<WK1N;                    //Weak 1NT.
        if (bidOptions.twoNT11 == TWO_NT_11_17_18)
            rules<<WK1N_R_NF;           //Weak 1NT, and 1NT after 1/1: 15-16 HCP
        else
            rules<<WK1N_R_GF;           //Weak 1NT, and 1NT after 1/1: 15-18 HCP
    }
    else
        rules<<ST1N;                    //Strong 1NT.

    if (bidOptions.open1NTWeakMajor)
        rules<<O_1N_WM;                 //Open 1NT  with weak major.

    if (((bidOptions.bidStyle == AMERICAN_STYLE) && (bidOptions.limitRaises &&
                     ((bidOptions.majorsMode == MAJOR_FOUR) || (bidOptions.jacoby2NT)))) ||
         ((bidOptions.bidStyle == EUROPEAN_STYLE) &&
                     (bidOptions.jacoby2NT || bidOptions.splinterbids)))
          rules<<SPL_O<<SPL_D;          //Splinter offensive and defensive side.
      else
          rules<<SPL_N;                 //No splinter.

    if (bidOptions.twoNT11 == TWO_NT_11_18_20)
        rules<<OR_11_2N_GF;             //2N after 1/1: 18-20 HCP.
    else
        rules<<OR_11_2N_NF;             //2N after 1/1: 17-18 HCP.

    if ((bidOptions.oneNTRange == ONE_NT_RANGE_12_14) || (bidOptions.twoNT21 == TWO_NT_21_15_18))
        rules<<OR_21_2N_GF;             //2N after 2/1: 15-18 HCP.
    else
        rules<<OR_21_2N_NF;             //2N after 2/1: 12-14 HCP.

    if (bidOptions.twoNT11 == TWO_NT_11_18_20)
        rules<<OR_11_3N_LM;             //3N after 1/1: long minor.
    else
        rules<<OR_11_3N_N;              //3N after 1/1: 19+ HCP

    if (((bidOptions.bidStyle == AMERICAN_STYLE) && (bidOptions.limitRaises &&
                     ((bidOptions.majorsMode == MAJOR_FOUR) || (bidOptions.jacoby2NT)))) ||
         ((bidOptions.bidStyle == EUROPEAN_STYLE) && (bidOptions.jacoby2NT)))
        rules<<R_M_2N_J;              //Jacoby 2NT major raise.

    if (bidOptions.oneNT3m == ONE_NT_3m_SI)
      rules<<R_m_1N_S;                //1N-3C/3D Slam invitation.
    else
      rules<<R_m_1N_G;                //1N-3C/3D Game invitation or preemptive.

//    if (true)
          rules<<R_2C_2NT_2224;         //2NT after 2C: 22-24 HCP.
//    else
//        rules<<R_2C_2NT_2324;         //2NT after 2C: 23-24 HCP.

    if (bidOptions.threeLevel == SOUND_THREE_LEVEL)
        rules<<S_3L_P_NV<<S_3L_P_V;   //Sound 3 level preempt not vul/vul.
    else if (bidOptions.threeLevel == LIGHT_THREE_LEVEL)
        rules<<L_3L_P_NV<<L_3L_P_V;   //Light 3 level preempt not vul/vul.
    else
        rules<<S_3L_P_V<<L_3L_P_NV;   //sound/light 3 level preempt vul/not vul.

//    if (bidOptions.overcallJR == O_JR_LIM)
          rules<<R_JR_LIM;              //Opponent overcalled: responder jump raise is invitational.
//    else
//        rules<<R_JR_PRE;              //Opponent overcalled: responder jump raise is preemptive.

//    if (bidOptions.overcall == O_CONSTRUCTIVE)
          rules<<D_CON;                 //Constructive overcall (raise is invitational).
//    else
//        rules<<D_AGR;                 //Aggressive overcall (raise is preemptive).

//    if (bidOptions.overcall4C)
//        rules<<D_OVR_4;               //Four card overcall (good).

    if (bidOptions.jumpOvercalls != JUMP_OVERCALL_STRONG)
        rules<<D_J_OWK;
    else
        rules<<D_J_OIM;

    rules<<GHES_OFF;                  //Ghestem convention is off.
    rules<<CBS_OFF;                   //Checkback Stayman is off.

    if (bidOptions.twoCtwoD == TWO_C_TWO_D_NEGATIVE)
        rules<<R_2C_2D_N;
    else
        rules<<R_2C_2D_W;

    if ((bidOptions.stayman == STAYMAN_3R_F) || (bidOptions.stayman == STAYMAN_3R_NF))
        rules<<R_1N_S;                //Stayman.
    else
        rules<<R_1N_4R_S;             //4-response Stayman.

    if (!bidOptions.fourSuitTransfers)
          rules<<R_1N_2N_N;             //1N-2N natural.
    else
        rules<<R_1N_2N_T;             //1N-2N transfer.

//    rules<<R_2N_3M;               //2N-3M: natural forcing.

//    if (bidOptions.redouble == REDOUBLE_ESC)
//        rules<<RD_ESC;
//    else if (bidOptions.redouble == REDOUBLE_PEND);
//        rules<<RD_PND;

//    if (bidOptions.takeOutOff == TAKEOUT_STD)
          rules<<STD_O;             //(mostly doubles) offensive side.
//    else
//        rules<<T_OUT_D_O;         //Takeout offensive side (unclear what takeout rules).

//    if (bidOptions.takeOutDef == TAKEOUT_STD)
          rules<<STD_D;             //(mostly doubles) defensive side.
//    else
//        rules<<T_OUT_D_D;         //Takeout defensive side (unclear what takeout rules).
}

