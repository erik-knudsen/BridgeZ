/*Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CBidDBDefine.h
  Developers: eak

  Revision History:
  1-dec-2016 eak: Original

  Abstract: Defines for bidding database.

  Platforms: Qt.

*/

#ifndef CBIDDBDEFINE_H
#define CBIDDBDEFINE_H

#include <QtGlobal>
#include <QSet>

#include "cbidoptiondoc.h"

//Definition of pages**********************************************************
//Opening bids---------------------------------------------------------------
//Open 1. and 2. seat.
const qint16 O12_4M_SN_L = 1;      //4-major, strong 1NT, lower first.
const qint16 O12_4M_WN_L = 2;      //4-major, weak 1NT, lower first:
const qint16 O12_5M_SN = 3;        //5-major, strong 1NT.
const qint16 O12_5M_WN = 1004;     //5-major, weak 1NT.
const qint16 O12_W2D_6_11 = 4;     //Weak 2D, 6-11 HCP.
const qint16 O12_W2H2S_6_11 = 5;   //Weak 2H/2S, 6-11 HCP.
const qint16 O12_1N_12_14 = 6;     //1NT 12-14 HCP.
const qint16 O12_3LEVEL = 7;       //3. level.
//Open 3. seat.
const qint16 O3_4M_SN_L = 10;      //4-major, strong 1NT, lower first.
const qint16 O3_4M_WN_L = 11;      //4-major, weak 1NT, lower first.
const qint16 O3_5M_SN = 12;        //5-major, strong 1NT.
const qint16 O3_5M_WN = 13;        //5-major, weak 1NT.
const qint16 O3_W2D_6_11 = 14;     //Weak 2D, 6-11 HCP.
const qint16 O3_W2H2S_6_11 = 15;   //Weak 2H/2S, 6-11 HCP.
const qint16 O3_1N_12_14 = 16;     //1NT 12-14 HCP.
const qint16 O3_3LEVEL = 17;       //3. level.
//Open 4. seat.
const qint16 O4_4M_SN_L = 20;      //4-major, strong 1NT, lower first.
const qint16 O4_4M_WN_L = 21;      //4-major, weak 1NT, lower first.
const qint16 O4_5M_SN = 22;        //5-major, strong 1NT.
const qint16 O4_5M_WN = 23;        //5-major, weak 1NT.
const qint16 O4_W2D_6_11 = 24;     //Weak 2D, 6-11 HCP.
const qint16 O4_W2H2S_6_11 = 25;   //Weak 2H/2S, 6-11 HCP.
const qint16 O4_1N_12_14 = 26;     //1NT 12-14 HCP.
const qint16 O4_3LEVEL = 27;       //3. level.
//Open any seat.
const qint16 O_1N_16_18 = 30;      //1NT 16-18 HCP.
const qint16 O_1N_15_17 = 31;      //1NT 15-17 HCP.
const qint16 O_S2_CDHS = 35;       //Strong two (2C, 2D, 2H, 2S).
const qint16 O_S2_C = 36;          //Strong 2C.
const qint16 O_3N_25_27 = 37;      //3NT 25-27 HCP.
const qint16 O123_3N_GMBL = 38;    //3NT gambling (1., 2. and 3. seat).
const qint16 O4_3N_GMBL = 39;      //3NT gambling (4. seat).
const qint16 O_3N_ACOL = 40;       //3NT Acol.
const qint16 O_2N_20_21 = 41;      //2NT 20-21 HCP.
const qint16 O_2N_21_22 = 42;      //2NT 21-22 HCP.
const qint16 O_2N_22_24 = 43;      //2NT 22-24 HCP.

//After 2 (C, D, H, S)level opening----------------------------------------------
const qint16 A_S2_CDHS = 50;       //After strong 2 opening.
const qint16 A_S2_C = 51;          //After strong 2C opening.
const qint16 A_W2D = 52;           //After weak 2D opening.
const qint16 A_W2H2S = 53;         //After weak 2H/2S opening.
const qint16 A_W2D_O_NF = 1090;    //Ogust after weak 2D, not forcing.
const qint16 A_W2D_F_NF = 1053;    //Feature after weak 2D, not forcing.
const qint16 A_W2D_O_F = 1091;     //Ogust after weak 2D, forcing.
const qint16 A_W2D_F_F = 1054;     //Feature after weak 2D, forcing.
const qint16 A_W2H2S_O_NF = 1092;  //Ogust after weak 2H/2S, not forcing.
const qint16 A_W2H2S_F_NF = 1055;  //Feature after weak 2H/2S, not forcing.
const qint16 A_W2H2S_O_F = 1093;   //Ogust after weak 2H/2S, forcing.
const qint16 A_W2H2S_F_F = 1056;   //Feature after weak 2H/2S, forcing.
const qint16 A_W2DX_NAT = 54;      //After weak 2D opening and intervening takeout double.
const qint16 A_W2H2SX_NAT = 55;    //After weak 2H/2S opening and intervening takeout double.

//After 2NT opening--------------------------------------------------------------
//20-21 HCP.
const qint16 A_2N_20_21 = 60;      //After opening.
const qint16 A_2N_20_21_SN = 61;   //3C Stayman. 3D, 3H, 3S Natural.
const qint16 A_2N_20_21_NT = 62;   //Non Texas.
const qint16 A_2N_20_21_L = 63;    //Later bids.
const qint16 A_2N_20_21_NL =64;    //3D, 3H, 3S Natural. Later bids.
const qint16 A_2N_20_21_SL = 1064; //3C Stayman. Later bids.
//21-22 HCP.
const qint16 A_2N_21_22 = 65;      //After opening.
const qint16 A_2N_21_22_SN = 66;   //3C Stayman. 3D, 3H, 3S Natural.
const qint16 A_2N_21_22_NT = 67;   //Non Texas.
const qint16 A_2N_21_22_L = 68;    //Later bids.
const qint16 A_2N_21_22_NL =69;    //3D, 3H, 3S Natural. Later bids.
const qint16 A_2N_21_22_SL = 1069; //3C Stayman. Later bids.
//22-24 HCP.
const qint16 A_2N_22_24 = 70;      //After opening.
const qint16 A_2N_22_24_SN = 71;   //3C Stayman. 3D, 3H, 3S Natural.
const qint16 A_2N_22_24_NT = 72;   //Non Texas.
const qint16 A_2N_22_24_L = 73;    //Later bids.
const qint16 A_2N_22_24_NL =74;    //3D, 3H, 3S Natural. Later bids.
const qint16 A_2N_22_24_SL = 1074; //3C Stayman. Later bids.
//After 2C-2D-2NT opening.
//22-24 HCP.
const qint16 A_2C2D2N_22_24 = 80;  //After opening.
const qint16 A_2C2D2N_22_24_SN = 81;//3C Stayman. 3D, 3H, 3S Natural.
const qint16 A_2C2D2N_22_24_NT = 82;//Non Texas.
const qint16 A_2C2D2N_22_24_NL = 83;//3D, 3H, 3S Natural. Later bids.
const qint16 A_2C2D2N_22_24_SL = 1083;//3C Stayman. Later bids.
//23-24 HCP.
const qint16 A_2C2D2N_23_24 = 84;  //After opening.
const qint16 A_2C2D2N_23_24_SN = 85;//3C Stayman. 3D, 3H, 3S Natural.
const qint16 A_2C2D2N_23_24_NT = 86;//Non Texas.
const qint16 A_2C2D2N_23_24_NL = 87;//3D, 3H, 3S Natural. Later bids.
const qint16 A_2C2D2N_23_24_SL = 1087;//3C Stayman. Later bids.

//After 3NT opening---------------------------------------------------------------
const qint16 A_3N_25_27 = 90;      //After 3NT 25-27 HCP.
const qint16 A_3N_GMBL = 91;       //After 3NT gambling.
const qint16 A_3N_ACOL = 92;       //After 3NT ACOL.

//After pre emptive opening-------------------------------------------------------
const qint16 A_3LEVEL = 100;       //

//After 1 level opening (1C, 1D, 1H, 1S)--------------------------------------------------
//Non Splinter.
const qint16 A4_FF = 110;          //American 4-major, major/minor forcing/forcing raise.
const qint16 A5_FF = 111;          //American 5-major, major/minor forcing/forcing raise.
const qint16 A5_SF = 112;          //American 5-major, major/minor simple limit/forcing raise.
const qint16 A5_SL = 113;          //American 5-major, major/minor simple limit/limit raise.
const qint16 E45_SL = 114;         //European 4/5-major, major/minor simple limit/limit raise.
const qint16 E4_1H_SL = 115;       //European 4-major (1H), major/minor simple limit/limit raise.
const qint16 E4_1S_SL = 116;       //European 4-major (1S), major/minor simple limit/limit raise.
const qint16 E5_1H_SL = 117;       //European 5-major (1H), major/minor simple limit/limit raise.
const qint16 E5_1S_SL = 118;       //European 5-major (1S), major/minor simple limit/limit raise.
//Splinter.
const qint16 A45_L_S = 119;        //American 4/5 major, minor limit raise, splinter.
const qint16 E45_L_S = 120;        //European 4/5 major, minor limit raise, splinter.
const qint16 E4_1H_L_S = 121;      //European 4 major (1H), major limit raise, splinter.
const qint16 E4_1S_L_S = 122;      //European 4 major (1S), major limit raise, splinter.
const qint16 E5_1H_L_S = 123;      //European 5 major (1H), major limit raise, splinter.
const qint16 E5_1S_L_S = 124;      //European 5 major (1S), major limit raise, splinter.
const qint16 AE4_1H_2NF_S = 125;   //4 major (1H), major raise 2NT forcing, splinter.
const qint16 AE4_1S_2NF_S = 126;   //4 major (1S), major raise 2NT forcing, splinter.
const qint16 AE5_1H_2NF_S = 127;   //5 major (1H), major raise 2NT forcing, splinter.
const qint16 AE5_1S_2NF_S = 128;   //5 major (1S), major raise 2NT forcing, splinter.
const qint16 A4_3NF_S = 129;       //American 4 major, major raise 3NT forcing, splinter.
const qint16 A5_3NF_S = 130;       //American 5 major, major raise 3NT forcing, splinter.
//Openers rebid.
const qint16 OR_COM_1 = 131;       //Common sequences.
const qint16 OR_COM_2 = 135;       //Common sequences.
const qint16 OR4_1H_1S = 136;      //4 major (open 1H, response 1S).
const qint16 OR5_1H_1S = 137;      //5 major (open 1H, response 1S).
const qint16 OR4_1H_2 = 138;       //4 major (open 1H, response 2x).
const qint16 OR4_1S_2 = 139;       //4 major (open 1S, response 2x).
const qint16 OR5_1H_2 = 140;       //5 major (open 1H, response 2x).
const qint16 OR5_1S_2 = 141;       //5 major (open 1S, response 2x).
const qint16 OR4_1H = 142;         //4 major (open 1H).
const qint16 OR4_1S = 144;         //4 major (open 1S).
const qint16 OR5_1H = 146;         //5 major (open 1H).
const qint16 OR5_1S = 148;         //5 major (open 1S).
const qint16 OR4_1H_1N = 150;      //4 major (open 1H, response 1N).
const qint16 OR4_1S_1N = 151;      //4 major (open 1S, response 1N).
const qint16 OR5_1H_1N = 152;      //5 major (open 1H, response 1N).
const qint16 OR5_1S_1N = 153;      //5 major (open 1S, response 1N).
const qint16 OR_1m_2m_FL = 154;    //Open 1 minor, response 2 minor, raise forcing/limit.
const qint16 OR_1m_3m_F = 155;     //Open 1 minor, response 3 minor, raise forcing.
const qint16 OR_1m_3m_L = 156;     //Open 1 minor, response 3 minor, raise limit.
const qint16 ORE_1C_1N = 1156;     //Open European 1C, response 1N.
const qint16 ORA_1C_1N = 157;      //Open American 1C, response 1N.
const qint16 ORE_1m_2N = 158;      //Open European 1 minor, response 2N.
const qint16 ORA_1m_2N = 159;      //Open American 1 minor, response 2N.
const qint16 ORE_1m_3N = 160;      //Open European 1 minor, response 3N.
const qint16 ORA_1m_3N = 161;      //Open American 1 minor, response 3N.
const qint16 OR_1M_3M_F = 162;     //Open 1 major, response 3 major, raise forcing.
const qint16 OR_1M_3M_L = 163;     //Open 1 major, response 3 major, raise limit.
const qint16 OR_SPLINTER = 164;    //Splinter raise (from 1C/1D/1H/1S).
const qint16 ORA4_1M_2N_N = 165;   //American 4 major. Open 1 major, response 2N. Natural raise.
const qint16 ORA5_1M_2N_N = 166;   //American 5 major. Open 1 major, response 2N. Natural raise.
const qint16 ORE45_1M_2N_N = 167;  //European 4/5 major. Open 1 major, response 2N. Natural raise.
const qint16 ORE45_1M_2N_J = 168;  //Open 1 major, response 2N. Jacoby 2NT raise.
const qint16 ORA4_1M_3N_N = 169;   //American 4 major. Open 1 major, response 3N. Natural raise.
const qint16 ORA5_1M_3N_N = 170;   //American 5 major. Open 1 major, response 3N. Natural raise.
const qint16 ORA_1M_3N_F = 171;    //American. Open 1 major, response 3N. 3N forcing raise.
const qint16 OR4_1H_3N_N = 172;    //4 major. Open 1H, response 3N. Natural raise.
const qint16 OR4_1S_3N_N = 173;    //4 major. Open 1S, response 3N. Natural raise.
const qint16 OR5_1H_3N_N = 174;    //5 major. Open 1H, response 3N. Natural raise.
const qint16 OR5_1S_3N_N = 175;    //5 major. Open 1S, response 3N. Natural raise.
//Responders rebid or later bids.
const qint16 RR_COM = 176;         //Common sequences.
const qint16 RR4_1D1H = 178;       //4 major. Open 1D/1H.
const qint16 RR4_1S = 179;         //4 major. Open 1S.
const qint16 RR5_1D1H = 180;       //5 major. Open 1D/1H.
const qint16 RR5_1S = 181;         //5 major. Open 1S.
const qint16 RR_3N_F = 182;        //3NT forcing raise.
const qint16 RR_2N_11_1821 = 183;  //2NT after 1/1: 18-21 HCP.
const qint16 RR_2N_11_1718 = 185;  //2NT after 1/1: 17-18 HCP.
const qint16 RR45_2N_21_1518 = 187;//2NT after 2/1: 15-18 HCP.
const qint16 RR45_2N_21_1214 = 190;//2NT after 2/1: 12-14 HCP.
const qint16 RR_4N = 191;          //4. suit natural.
const qint16 RR4_4N = 192;         //4 major. 4. suit natural.
const qint16 RR5_4N = 193;         //5 major. 4. suit natural.
const qint16 RR_4F = 194;          //4. suit forcing.
const qint16 RR4_4F_1D1H = 195;    //4 major. 4. suit forcing. Open 1D/1H.
const qint16 RR4_4F_1S = 196;      //4 major. 4. suit forcing. Open 1S.
const qint16 RR5_4F_1D1H = 197;    //5 major. 4. suit forcing. Open 1D/1H.
const qint16 RR5_4F_1S = 198;      //5 major. 4. suit forcing. Open 1S.
const qint16 RR_S = 199;           //Splinter raise.
const qint16 RR_WN_1820_N = 200;   //Weak 1NT, 2NT after 1/1: 18-20, new minor natural.
const qint16 RR_WN_1820_F = 201;   //Weak 1NT, 2NT after 1/1: 18-20, new minor forcing.
const qint16 RR_WN_1718_N = 202;   //Weak 1NT, 2NT after 1/1: 17-18, new minor natural.
const qint16 RR_WN_1718_F = 203;   //Weak 1NT, 2NT after 1/1: 17-18, new minor forcing.
const qint16 RR4_WN_1820_N = 204;  //4 major. Weak 1NT, 2NT after 1/1: 18-20, new minor natural.
const qint16 RR4_WN_1820_F = 205;  //4 major. Weak 1NT, 2NT after 1/1: 18-20, new minor forcing.
const qint16 RR4_WN_1718_N = 206;  //4 major. Weak 1NT, 2NT after 1/1: 17-18, new minor natural.
const qint16 RR4_WN_1718_F = 207;  //4 major. Weak 1NT, 2NT after 1/1: 17-18, new minor forcing.
const qint16 RR5_WN_1820_N = 208;  //5 major. Weak 1NT, 2NT after 1/1: 18-20, new minor natural.
const qint16 RR5_WN_1820_F = 209;  //5 major. Weak 1NT, 2NT after 1/1: 18-20, new minor forcing.
const qint16 RR5_WN_1718_N = 211;  //5 major. Weak 1NT, 2NT after 1/1: 17-18, new minor natural.
const qint16 RR5_WN_1718_F = 212;  //5 major. Weak 1NT, 2NT after 1/1: 17-18, new minor forcing.
const qint16 RR_SN_N = 213;        //Strong 1NT, new minor natural.
const qint16 RR_SN_F = 214;        //Strong 1NT, new minor forcing.
const qint16 RR4_SN_N = 215;       //4 major. Strong 1NT, new minor natural.
const qint16 RR4_SN_F = 216;       //4 major. Strong 1NT, new minor forcing.
const qint16 RR5_SN_N = 217;       //5 major. Strong 1NT, new minor natural.
const qint16 RR5_SN_F = 218;       //5 major. Strong 1NT, new minor forcing.

//After 1NT opening---------------------------------------------------------------------
//Responses.
const qint16 R_1618 = 220;         //1NT 16-18 HCP. Common.
const qint16 R_1517 = 221;         //1NT 15-17 HCP. Common.
const qint16 R_1214 = 222;         //1NT 12-14 HCP. Common.
const qint16 R_1618_SF = 226;      //1NT 16-18 HCP. Stayman forcing.
const qint16 R_1517_SF = 227;      //1NT 15-17 HCP. Stayman forcing.
const qint16 R_1214_SF = 228;      //1NT 12-14 HCP. Stayman forcing.
const qint16 R_1618_SN = 229;      //1NT 16-18 HCP. Stayman not forcing.
const qint16 R_1517_SN = 230;      //1NT 15-17 HCP. Stayman not forcing.
const qint16 R_1214_SN = 231;      //1NT 12-14 HCP. Stayman not forcing.
const qint16 R_1618_SO = 232;      //1NT 16-18 HCP. 2D/2H/2S sign off.
const qint16 R_1517_SO = 233;      //1NT 15-17 HCP. 2D/2H/2S sign off.
const qint16 R_1214_SO = 234;      //1NT 12-14 HCP. 2D/2H/2S sign off.
const qint16 R_1618_J_SO = 235;    //1NT 16-18 HCP. Jacoby. 2S sign off.
const qint16 R_1517_J_SO = 236;    //1NT 15-17 HCP. Jacoby. 2S sign off.
const qint16 R_1214_J_SO = 237;    //1NT 12-14 HCP. Jacoby. 2S sign off.
const qint16 R_1618_J_MT = 238;    //1NT 16-18 HCP. Jacoby. 2S,2N minor transfer.
const qint16 R_1517_J_MT = 239;    //1NT 15-17 HCP. Jacoby. 2S 2N minor transfer.
const qint16 R_1214_J_MT = 240;    //1NT 12-14 HCP. Jacoby. 2S 2N minor transfer.
const qint16 R_1618_3mSI = 241;    //1NT 16-18 HCP. 3C/3D slam invitation.
const qint16 R_1517_3mSI = 242;    //1NT 15-17 HCP. 3C/3D slam invitation.
const qint16 R_1214_3mSI = 243;    //1NT 12-14 HCP. 3C/3D slam invitation.
const qint16 R_1618_3mGI = 244;    //1NT 16-18 HCP. 3C/3D game invitation.
const qint16 R_1517_3mGI = 245;    //1NT 15-17 HCP. 3C/3D game invitation.
const qint16 R_1214_3mGI = 246;    //1NT 12-14 HCP. 3C/3D game invitation.
const qint16 R_1618_3mPE = 247;    //1NT 16-18 HCP. 3C/3D pre emptive.
const qint16 R_1517_3mPE = 248;    //1NT 15-17 HCP. 3C/3D pre emptive.
const qint16 R_1214_3mPE = 249;    //1NT 12-14 HCP. 3C/3D pre emptive.
const qint16 R_1618_3MSI = 250;    //1NT 16-18 HCP. 3H/3S slam invitation.
const qint16 R_1517_3MSI = 251;    //1NT 15-17 HCP. 3H/3S slam invitation.
const qint16 R_1214_3MSI = 252;    //1NT 12-14 HCP. 3H/3S slam invitation.
const qint16 R_1618_3MGF = 253;    //1NT 16-18 HCP. 3H/3S game forcing.
const qint16 R_1517_3MGF = 254;    //1NT 15-17 HCP. 3H/3S game forcing.
const qint16 R_1214_3MGF = 255;    //1NT 12-14 HCP. 3H/3S game forcing.
const qint16 R_1618_3MPE = 256;    //1NT 16-18 HCP. 3H/3S pre emptive.
const qint16 R_1517_3MPE = 257;    //1NT 15-17 HCP. 3H/3S pre emptive.
const qint16 R_1214_3MPE = 258;    //1NT 12-14 HCP. 3H/3S pre emptive.
const qint16 R_1618_3MGI = 259;    //1NT 16-18 HCP. 3H/3S game invitation.
const qint16 R_1517_3MGI = 260;    //1NT 15-17 HCP. 3H/3S game invitation.
const qint16 R_1214_3MGI = 261;    //1NT 12-14 HCP. 3H/3S game invitation.
const qint16 R_1618_4MN = 262;     //1NT 16-18 HCP. 4H/4S natural.
const qint16 R_1517_4MN = 263;     //1NT 15-17 HCP. 4H/4S natural.
const qint16 R_1214_4MN = 264;     //1NT 12-14 HCP. 4H/4S natural.
//Rebids.
const qint16 OR_1618 = 265;        //1NT 16-18 HCP. Common.
const qint16 OR_1517 = 267;        //1NT 15-17 HCP. Common.
const qint16 OR_1214 = 269;        //1NT 12-14 HCP. Common.
const qint16 OR_1618_SF_1 = 271;   //1NT 16-18 HCP. Forcing Stayman.
const qint16 OR_1517_SF_1 = 273;   //1NT 15-17 HCP. Forcing Stayman.
const qint16 OR_1214_SF_1 = 275;   //1NT 12-14 HCP. Forcing Stayman.
const qint16 OR_1618_SN_1 = 276;   //1NT 16-18 HCP. Non forcing Stayman.
const qint16 OR_1517_SN_1 = 277;   //1NT 15-17 HCP. Non forcing Stayman.
const qint16 OR_1214_SN_1 = 278;   //1NT 12-14 HCP. Non forcing Stayman.
const qint16 OR_1618_SF_2 = 279;   //1NT 16-18 HCP. Forcing Stayman.
const qint16 OR_1517_SF_2 = 280;   //1NT 15-17 HCP. Forcing Stayman.
const qint16 OR_1214_SF_2 = 281;   //1NT 12-14 HCP. Forcing Stayman.
const qint16 OR_1618_SN_2 = 282;   //1NT 16-18 HCP. Non forcing Stayman.
const qint16 OR_1517_SN_2 = 283;   //1NT 15-17 HCP. Non forcing Stayman.
const qint16 OR_1214_SN_2 = 284;   //1NT 12-14 HCP. Non forcing Stayman.
const qint16 OR_1618_SO = 285;      //1NT 16-18 HCP. 2D/2H/2S sign off.
const qint16 OR_1517_SO = 286;      //1NT 15-17 HCP. 2D/2H/2S sign off.
const qint16 OR_1214_SO = 287;      //1NT 12-14 HCP. 2D/2H/2S sign off.
const qint16 OR_1618_J = 289;       //1NT 16-18 HCP. Jacoby.
const qint16 OR_1517_J = 290;       //1NT 15-17 HCP. Jacoby.
const qint16 OR_1214_J = 291;       //1NT 12-14 HCP. Jacoby.
const qint16 OR_1618_J_SO = 292;    //1NT 16-18 HCP. Jacoby. 2S sign off.
const qint16 OR_1517_J_SO = 293;    //1NT 15-17 HCP. Jacoby. 2S sign off.
const qint16 OR_1214_J_SO = 294;    //1NT 12-14 HCP. Jacoby. 2S sign off.
const qint16 OR_1618_J_MT = 295;    //1NT 16-18 HCP. Jacoby. 2S,2N minor transfer.
const qint16 OR_1517_J_MT = 296;    //1NT 15-17 HCP. Jacoby. 2S 2N minor transfer.
const qint16 OR_1214_J_MT = 297;    //1NT 12-14 HCP. Jacoby. 2S 2N minor transfer.
const qint16 OR_1618_3mSI = 298;    //1NT 16-18 HCP. 3C/3D slam invitation.
const qint16 OR_1517_3mSI = 299;    //1NT 15-17 HCP. 3C/3D slam invitation.
const qint16 OR_1214_3mSI = 300;    //1NT 12-14 HCP. 3C/3D slam invitation.
const qint16 OR_1618_3mGI = 301;    //1NT 16-18 HCP. 3C/3D game invitation.
const qint16 OR_1517_3mGI = 302;    //1NT 15-17 HCP. 3C/3D game invitation.
const qint16 OR_1214_3mGI = 303;    //1NT 12-14 HCP. 3C/3D game invitation.
const qint16 OR_1618_3mPE = 304;    //1NT 16-18 HCP. 3C/3D pre emptive.
const qint16 OR_1517_3mPE = 305;    //1NT 15-17 HCP. 3C/3D pre emptive.
const qint16 OR_1214_3mPE = 306;    //1NT 12-14 HCP. 3C/3D pre emptive.
const qint16 OR_1618_3MSI = 307;    //1NT 16-18 HCP. 3H/3S slam invitation.
const qint16 OR_1517_3MSI = 308;    //1NT 15-17 HCP. 3H/3S slam invitation.
const qint16 OR_1214_3MSI = 309;    //1NT 12-14 HCP. 3H/3S slam invitation.
const qint16 OR_1618_3MGF = 310;    //1NT 16-18 HCP. 3H/3S game forcing.
const qint16 OR_1517_3MGF = 311;    //1NT 15-17 HCP. 3H/3S game forcing.
const qint16 OR_1214_3MGF = 312;    //1NT 12-14 HCP. 3H/3S game forcing.
const qint16 OR_1618_3MPE = 313;    //1NT 16-18 HCP. 3H/3S pre emptive.
const qint16 OR_1517_3MPE = 314;    //1NT 15-17 HCP. 3H/3S pre emptive.
const qint16 OR_1214_3MPE = 315;    //1NT 12-14 HCP. 3H/3S pre emptive.
const qint16 OR_1618_3MGI = 316;    //1NT 16-18 HCP. 3H/3S game invitation.
const qint16 OR_1517_3MGI = 317;    //1NT 15-17 HCP. 3H/3S game invitation.
const qint16 OR_1214_3MGI = 318;    //1NT 12-14 HCP. 3H/3S game invitation.
//Defenders side.
const qint16 D_S = 320;            //Strong 1NT.
const qint16 D_W = 321;            //Weak 1NT.
const qint16 D_S_SO = 322;         //Strong 1NT. 2D/2H/2S signoff.
const qint16 D_W_SO = 323;         //Weak 1NT. 2D/2H/2S signoff.
const qint16 D_S_J = 324;          //Strong 1NT. Jacoby.
const qint16 D_W_J = 325;          //Weak 1NT. Jacoby.
const qint16 D_S_J_MT = 326;       //Strong 1NT. Jacoby. 2S,2N minor transfer.
const qint16 D_W_J_MT = 327;       //Weak 1NT. Jacoby. 2S,2N minor transfer.
const qint16 D_S_3mSGI = 328;      //Strong 1NT. 3C/3D slam or game invitation.
const qint16 D_W_3mSGI = 329;      //Weak 1NT. 3C/3D slam or game invitation.
const qint16 D_S_3mPE = 330;       //Strong 1NT. 3C/3D pre emptive.
const qint16 D_W_3mPE = 331;       //Weak 1NT. 3C/3D pre emptive.
const qint16 D_S_3MSGI = 3301;     //Strong 1NT.  3H/3S slam inv. or game forcing or game inv.
const qint16 D_W_3MSGI = 3302;     //Weak 1NT.  3H/3S slam inv. or game forcing or game inv.
const qint16 D_S_3MPE = 332;       //Strong 1NT. 3H/3S pre emptive.
const qint16 D_W_3MPE = 333;       //Weak 1NT.  3H/3S pre emptive.
const qint16 D_S_4MN = 334;        //Strong 1NT. 4H/4S natural.
const qint16 D_W_4MN = 335;        //Weak 1NT. 4H/4S natural.
const qint16 D_S_NO = 336;         //Strong 1NT. Natural overcall.
const qint16 D_W_NO = 337;         //Weak 1NT. Natural overcall.
const qint16 D_S_N2C_N = 338;      //Strong 1NT. Responder over overcall natural 2C. Intervention natural.
const qint16 D_W_N2C_N = 339;      //Weak 1NT. Responder over overcall natural 2C. Intervention natural.
const qint16 D_S_N2D_N = 340;      //Strong 1NT. Responder over overcall natural 2D. Intervention natural.
const qint16 D_W_N2D_N = 341;      //Weak 1NT. Responder over overcall natural 2D. Intervention natural.
const qint16 D_S_N2H2S_N = 1341;   //Strong 1NT. Responder over overcall natural 2H/2S. Intervention natural.
const qint16 D_W_N2H2S_N = 1342;   //Weak 1NT. Responder over overcall natural 2H/2S. Intervention natural.

//Competitive bidding after 1 in a suit opening-----------------------------------------------------
//Overcall and takeout double.
const qint16 D_C_COMMON = 350;     //1C common.
const qint16 D_D_COMMON = 351;     //1D common.
const qint16 D_H_COMMON = 352;     //1H common.
const qint16 D_S_COMMON = 353;     //1S common.
const qint16 D_C1_916 = 354;       //1C 1 level overcall 9-16 HCP.
const qint16 D_D1_916 = 355;       //1D 1 level overcall 9-16 HCP.
const qint16 D_H1_916 = 356;       //1H 1 level overcall 9-16 HCP.
const qint16 D_S1_916 = 357;       //1S 1 level overcall 9-16 HCP.
const qint16 D_C2_1116_69 = 358;   //1C 2 level simple 11-16, jump weak 6-9 HCP.
const qint16 D_D2_1116_69 = 359;   //1D 2 level simple 11-16, jump weak 6-9 HCP.
const qint16 D_H2_1116_69 = 360;   //1H 2 level simple 11-16, jump weak 6-9 HCP.
const qint16 D_S2_1116_69 = 361;   //1S 2 level simple 11-16, jump weak 6-9 HCP.
const qint16 D_C2_1116_1316 = 362; //1C 2 level simple 11-16, jump intermediate 13-16 HCP.
const qint16 D_D2_1116_1316 = 363; //1D 2 level simple 11-16, jump intermediate 13-16 HCP.
const qint16 D_H2_1116_1316 = 364; //1H 2 level simple 11-16, jump intermediate 13-16 HCP.
const qint16 D_S2_1116_1316 = 365; //1S 2 level simple 11-16, jump intermediate 13-16 HCP.
const qint16 D_C_12 = 366;         //1C takeout double minimum 12 HCP.
const qint16 D_D_12 = 367;         //1D takeout double minimum 12 HCP.
const qint16 D_H_12 = 368;         //1H takeout double minimum 12 HCP.
const qint16 D_S_12 = 369;         //1S takeout double minimum 12 HCP.
const qint16 D_C_SCB = 370;        //1C strong cue bid.
const qint16 D_D_SCB = 371;        //1D strong cue bid.
const qint16 D_H_SCB = 372;        //1H strong cue bid.
const qint16 D_S_SCB = 373;        //1S strong cue bid.
const qint16 D_C_MCB = 374;        //1C Michaels cue bid.
const qint16 D_D_MCB = 375;        //1D Michaels cue bid.
const qint16 D_H_MCB = 376;        //1H Michaels cue bid.
const qint16 D_S_MCB = 377;        //1S Michaels cue bid.
//Opening side.
const qint16 D_OPS_COMMON = 380;   //Common sequence.
const qint16 D_OPS_L = 381;        //Raise limit.
const qint16 D_OPS_SCB = 382;      //Strong cue bid.
const qint16 D_OPS_MCB = 383;      //Michaels cue bid.
const qint16 D_OPS_PD = 384;       //Penalty double against overcall.
const qint16 D_OPS_ND = 385;       //Negative double against overcall.
const qint16 D4_OPS_1H_PD = 386;   //4 major 1H, penalty double against overcall.
const qint16 D4_OPS_1S_PD = 387;   //4 major 1S, penalty double against overcall.
const qint16 D4_OPS_1H_ND = 388;   //4 major 1H, negative double against overcall.
const qint16 D4_OPS_1S_ND = 389;   //4 major 1S, negative double against overcall.
const qint16 D5_OPS_1H_PD = 390;   //5 major 1H, penalty double against overcall.
const qint16 D5_OPS_1S_PD = 391;   //5 major 1S, penalty double against overcall.
const qint16 D5_OPS_1H_ND = 392;   //5 major 1H, negative double against overcall.
const qint16 D5_OPS_1S_ND = 393;   //5 major 1S, negative double against overcall.
//Overcallers side.
const qint16 D_OVS_COMMON = 394;   //Common sequence.
const qint16 D_OVS_RDP = 395;      //Response by doublers partner.
const qint16 D_OVS_CONO = 396;     //After constructive overcall.
const qint16 D_OVS_RD_1 = 397;     //Responsive double Response over RHO's raise.
const qint16 D_OVS_RD_2 = 398;     //Responsive double. After constructive overcall.
const qint16 D_OVS_R_SCB = 399;    //Response to strong cue bid.
const qint16 D_OVS_R_MCB = 400;    //Response to Michaels cue bid.
const qint16 D_OVS_R_WJO_N = 401;  //Response to weak jump overcall. 2NT natural.
const qint16 D_OVS_R_WJO_O = 402;  //Response to weak jump overcall. 2NT Ogust.
const qint16 D_OVS_R_WJO_F = 403;  //Response to weak jump overcall. 2NT Feature.
const qint16 D_OVS_R_IJO = 404;    //Response to intermediate jump overcall.
const qint16 D_OVS_ND = 405;       //Negative double after constructive overcall.

//Other competitive auctions-------------------------------------------------------------
//Reopening auction and overcall in indirect position.
const qint16 D_RO_COMMON = 406;    //Reopening auction.
const qint16 D_O_IND = 407;        //Overcall in indirect position.
const qint16 D_mLF = 408;          //Minor limit/forcing raise.
const qint16 D_MCB_OFF = 409;      //Michaels cue bid off.
const qint16 D_MCB_ON = 410;       //Michaels cue bid on.
const qint16 D_SNT_OFF = 411;      //Sandwich NT off.
const qint16 D_WJO_SNT_OFF = 412;  //Weak jump overcall, sandwich NT off.
const qint16 D_IJO_SNT_OFF = 413;  //intermediate jump overcall, sandwich NT off.
//Balancing, passed hand overcall.
const qint16 D_B_PO_COMMON = 414;  //Common.
const qint16 D_B_PO_S = 415;       //Strong 1NT.
const qint16 D_B_PO_W = 416;       //Weak 1NT.
//Rebid by opener and later auction.
const qint16 D_RB_COMMON_1 = 417;  //Common.
const qint16 D_RB_SD_OFF = 418;    //Support double off.
const qint16 D_RB_COMMON_2 = 419;  //Common.
const qint16 D_RB_mLF = 420;       //Minor limit/forcing raise.
const qint16 D_RB_ACO = 421;       //After constructive overcall.
//Maximal overcall double related sequences.
const qint16 D_RB_MO = 422;        //Rebid by opener and responder.
const qint16 D_RB_MO_OFF = 423;    //Maximal overcall double off.

//Definition of rule ids*******************************************************************
//1 in a suit opening.
//Basic settings are 13+ HCP for balanced hand and 11+ HCP for unbalanced hand.
//Additional settings.
const qint16 OB_12G = 1;           //Balanced and 12+ good HCP.
const qint16 OB_12 = 2;            //Balanced and 12+ HCP.
const qint16 OB_11G = 3;           //Balanced and 11+ good HCP.
const qint16 OUB_10 = 4;           //Balanced and 10+ HCP.
const qint16 OUB_9 = 5;            //Balanced and 9+ HCP.

const qint16 O_1H_4 = 6;           //1H promises 4 card suit.
const qint16 O_1H_5 = 7;           //1H promises 5 card suit.
const qint16 O_1S_4 = 8;           //1S promises 4 card suit.
const qint16 O_1S_5 = 9;           //1S promises 5 card suit.

const qint16 H5_2N = 44;           //Two Over One Natural.
const qint16 H5_2G = 43;           //Two Over One Game Force.

const qint16 O_PREP = 30;          //Prepared opening.
const qint16 O_MAJOR = 31;         //Open major suit first.
const qint16 O_LOWER = 32;         //Open lower suit first.

//Others.
const qint16 ST1N = 27;            //Strong 1NT.
const qint16 WK1N = 28;            //Weak 1NT.
const qint16 O_1N_WM = 13;         //Open 1NT with weak major.

const qint16 WK1N_R_GF = 33;       //Weak 1NT and 1NT after 1/1: 15-18 HCP.
const qint16 WK1N_R_NF = 34;       //Weak 1NT and 1NT after 1/1: 15-16 HCP.

const qint16 SPL_O = 29;           //Splinter offensive side.
const qint16 SPL_D = 22;           //Splinter defensive side.
const qint16 SPL_N = 42;           //No splinter.

const qint16 OR_11_2N_GF = 35;     //2N after 1/1: 18+ HCP (GF).
const qint16 OR_11_2N_NF = 36;     //2N after 1/1: 17-18 HCP (NF).
const qint16 OR_21_2N_GF = 40;     //2N after 2/1: 15-18 HCP (GF).
const qint16 OR_21_2N_NF = 41;     //2N after 2/1: 12-14 HCP (NF).

const qint16 OR_11_3N_N = 37;      //3N after 1/1: 19+ HCP (natural).
const qint16 OR_11_3N_LM = 39;     //3N after 1/1: Long minor.
const qint16 OR_11_3N_A = 38;      //3N after 1/1: Balanced major raise.

const qint16 R_M_2N_S = 26;        //Stenberg 2NT major raise.
const qint16 R_M_2N_J = 25;        //Jacoby 2NT major raise.

const qint16 R_m_1N_S = 48;        //1N-3C/3D Slam invitation.
const qint16 R_m_1N_G = 49;        //1N-3C/3D Game invitation or preemptive.

const qint16 R_2C_2NT_2224 = 18;   //2NT after 2C: 22-24 HCP.
const qint16 R_2C_2NT_2324 = 21;   //2NT after 2C: 23-24 HCP.

const qint16 S_3L_P_NV = 14;       //Sound 3 level preempt when not vulnerable.
const qint16 S_3L_P_V = 16;        //Sound 3 level preempt when vulnerable.
const qint16 L_3L_P_NV = 15;       //Light 3 level preempt when not vulnerable.
const qint16 L_3L_P_V = 17;        //Light 3 level preempt when vulnerable.

const qint16 R_JR_LIM = 54;        //Open overcalled: responder jump raise is invitational.
const qint16 R_JR_PRE = 55;        //Open overcalled: responder jump raise is preemptive.

const qint16 D_CON = 56;           //Constructive overcall (raise is invitational).
const qint16 D_AGR = 57;           //Aggressive overcall (raise is preemptive).

const qint16 D_OVR_4 = 52;         //Four card overcall (good).

const qint16 D_J_OWK = 59;         //Jump overcall is weak.
const qint16 D_J_OIM = 60;         //Jump overcall is intermediate.

const qint16 GHES_OFF = 53;        //Ghestem convention is off.
const qint16 CBS_OFF = 61;         //Checkback Stayman is off.

const qint16 R_2C_2D_N = 19;       //2C-2D: negative.
const qint16 R_2C_2D_W = 20;       //2C-2D: waiting.

const qint16 R_1N_S = 47;          //Stayman.
const qint16 R_1N_4R_S = 56;       //4-response Stayman.

const qint16 R_1N_2N_N = 45;       //1N-2N natural.
const qint16 R_1N_2N_T = 46;       //1N-2N transfer.

const qint16 R_2N_3M = 23;         //2N-3M: natural forcing.

const qint16 RD_ESC = 51;          //Redouble escape.
const qint16 RD_PND = 12;          //Redouble pending.

const qint16 T_OUT_D_O = 24;       //Takeout offensive side (unclear what takeout rules).
const qint16 T_OUT_D_D = 11;       //Takeout defensive side (unclear what takeout rules).
const qint16 STD_O = 58;           //(mostly doubles) offensive side (instead of T_OUT_D_O).
const qint16 STD_D = 10;           //(mostly doubles) defensive side (instead of T_OUT_D_D).

/*
//Pages for bid database -----------------------------------------------------------
//Common.
    OR_COM_1 OR_COM_2 RR_COM
    OR_1m_2m_FL
    D_C_COMMON D_D_COMMON D_H_COMMON D_S_COMMON
    D_C1_916 D_D1_916 D_H1_916 D_S1_916
    D_C_12 D_D_12 D_H_12 D_S_12
    D_OPS_COMMON
    D_OVS_COMMON D_OVS_RDP D_OVS_CONO D_OVS_RD_1 D_OVS_RD_2
    D_RO_COMMON D_O_IND D_mLF
    D_SNT_OFF
    D_B_PO_COMMON
    D_RB_COMMON_1 D_RB_SD_OFF D_RB_COMMON_2 D_RB_mLF D_RB_ACO
    D_RB_MO D_RB_MO_OFF

    //Weak jump overcall (6-9 HCP)
        D_C2_1116_69 D_D2_1116_69 D_H2_1116_69 D_S2_1116_69
        D_WJO_SNT_OFF
        //2NT Natural.
            D_OVS_R_WJO_N
        //2NT Ogust.
            D_OVS_R_WJO_O
        //2NT Feature.
            D_OVS_R_WJO_F
    //intermediate jump overcall (13-16 HCP)
        D_C2_1116_1316 D_D2_1116_1316 D_H2_1116_1316 D_S2_1116_1316
        D_OVS_R_IJO
        D_IJO_SNT_OFF

    //Strong cuebid.
        D_C_SCB D_D_SCB D_H_SCB D_S_SCB
        D_OPS_SCB
        D_OVS_R_SCB
        D_MCB_OFF
    //Michaels cue bid.
        D_C_MCB D_D_MCB D_H_MCB D_S_MCB
        D_OPS_MCB
        D_OVS_R_MCB
        D_MCB_ON

    //Penalty double against overcall.
        D_OPS_PD
    //Negative double against overcall.
        D_OPS_ND
        D_OVS_ND

    //American
        ORA_1C_1N ORA_1m_2N ORA_1m_3N

        //Forcing raise - minor and major.
            OR_1m_3m_F  OR_1M_3M_F
            RR_4N       (Always 4. suit non forcing).
        //Simple limit raise - minor and major.
            OR_1m_3m_L OR_1M_3M_L
            D_OPS_L
    //European.
        ORE_1C_1N ORE_1m_2N ORE_1m_3N
        RR_4F           (Always 4. suit forcing).
        //Always simple Limit raise - minor and major.
        OR_1m_3m_L OR_1M_3M_L
        D_OPS_L

        //Simple limit raise only.
            ORE45_1M_2N_N

            //Splinter.
                E45_L_S OR_SPLINTER
            //Non Splinter.
                E45_SL
        //Simple limit raise + 2NT major raise forcing (Jacoby 2NT).
            ORE45_1M_2N_J E45_L_S OR_SPLINTER       (Always Splinter).

    //Weak 1NT.
        D_W
        D_W_NO D_W_N2C_N D_W_N2D_N D_W_N2H2S_N
        D_B_PO_W

        //2D/2H/2S sign off.
            D_W_SO
        //Jacoby.
            D_W_J
            //2S sign off.
            //2S,2N minor transfer.
                D_W_J_MT

        //3C/3D slam invitation or 3C/3D game invitation.
            D_W_3mSGI
        //3C/3D pre emptive.
            D_W_3mPE

        //3H/3S slam invitation or game forcing or game invitation.
            D_W_3MSGI
        //3H/3S pre emptive.
            D_W_3MPE

        //4H/4S natural.
            D_W_4MN

        //2NT after 1/1 18-20 HCP.
            //New minor natural (after 1NT rebid).
                RR_WN_1820_N
            //New minor forcing (after 1NT rebid).
                RR_WN_1820_F
        //2NT after 1/1 17-18 HCP.
            //New minor natural (after 1NT rebid).
                RR_WN_1718_N
            //New minor forcing (after 1NT rebid).
                RR_WN_1718_F

    //Strong 1NT.
        D_S
        D_S_NO D_S_N2C_N D_S_N2D_N D_S_N2H2S_N
        D_B_PO_S

        //2D/2H/2S sign off.
            D_S_SO
        //Jacoby.
            D_S_J
            //2S sign off.
            //2S,2N minor transfer.
                D_S_J_MT

        //3C/3D slam invitation or 3C/3D game invitation.
            D_S_3mSGI
        //3C/3D pre emptive
            D_S_3mPE

        //3H/3S slam invitation or game forcing or game invitation.
            D_S_3MSGI
        //3H/3S pre emptive.
            D_S_3MPE

        //4H/4S natural.
            D_S_4MN

        //New minor natural (after 1NT rebid).
            RR_SN_N
        //New minor forcing (after 1NT rebid).
            RR_SN_F


//4-major.
    OR4_1H_1S OR4_1H_2 OR4_1S_2 OR4_1H OR4_1S OR4_1H_1N OR4_1S_1N OR4_1H_3N_N OR4_1S_3N_N
    RR4_1D1H RR4_1S

    //Penalty double against overcall.
        D4_OPS_1H_PD D4_OPS_1S_PD
    //Negative double against overcall.
        D4_OPS_1H_ND D4_OPS_1S_ND

    //Strong 1NT, lower first.
        O12_4M_SN_L O3_4M_SN_L O4_4M_SN_L
        //New minor natural (after 1NT rebid).
            RR4_SN_N
        //New minor forcing (after 1NT rebid).
            RR4_SN_F
    //Weak 1NT, lower first.
        O12_4M_WN_L O3_4M_WN_L O4_4M_WN_L
        //2NT after 1/1 18-20 HCP.
            //New minor natural (after 1NT rebid).
                RR4_WN_1820_N
            //New minor forcing (after 1NT rebid).
                RR4_WN_1820_F
        //2NT after 1/1 17-18 HCP.
            //New minor natural (after 1NT rebid).
                RR4_WN_1718_N
            //New minor forcing (after 1NT rebid).
                RR4_WN_1718_F

    //American.
        ORA4_1M_2N_N ORA4_1M_3N_N
        //Forcing raise - (minor and major).
            A4_FF RR4_4N        (Always 4. suit natural).
        //Simple limit raise - (minor and major).
            ORE45_1M_2N_J       (Always Jacoby 2NT).
            A45_L_S OR_SPLINTER (Always Splinter)
            AE4_1H_2NF_S AE4_1S_2NF_S
            RR4_4F_1D1H RR_4F    (Always 4. suit forcing).
    //European.
    //Always simple limit raise - minor and major.
        RR4_4F_1D1H RR4_4F_1S           (Always 4. suit forcing).

        //Simple limit raise only.
            E4_1H_L_S E4_1S_L_S

            //Non Splinter.
                E4_1H_SL E4_1S_SL
        //Simple limit raise + 2NT major raise forcing (Jacoby 2NT).
            AE4_1H_2NF_S AE4_1S_2NF_S


//5-major.
    OR5_1H_1S OR5_1H_2 OR5_1S_2 OR5_1H OR5_1S OR5_1H_1N OR5_1H_1N OR5_1H_3N_N OR5_1S_3N_N
    RR5_1D1H RR5_1S

    //Penalty double against overcall.
        D5_OPS_1H_PD D5_OPS_1S_PD
    //Negative double against overcall.
        D5_OPS_1H_ND D5_OPS_1S_ND

    //Strong 1NT.
        O12_5M_SN O3_5M_SN O4_5M_SN
        //New minor natural (after 1NT rebid).
            RR5_SN_N
        //New minor forcing (after 1NT rebid).
            RR5_SN_F
    //Weak 1NT.
        O12_5M_WN O3_5M_WN O4_5M_WN
        //2NT after 1/1 18-20 HCP.
            //New minor natural (after 1NT rebid).
                RR5_WN_1820_N
            //New minor forcing (after 1NT rebid).
                RR5_WN_1820_F
        //2NT after 1/1 17-18 HCP.
            //New minor natural (after 1NT rebid).
                RR5_WN_1718_N
            //New minor forcing (after 1NT rebid).
                RR5_WN_1718_F

    //American.
        ORA5_1M_2N_N ORA5_1M_3N_N
        //Forcing raise - (minor and major).
            A5_FF RR5_4N        (Always 4. suit natural).
        //Simple limit raise - (minor and major).
            ORE45_1M_2N_J       //Always Jacoby 2NT).
            A45_L_S OR_SPLINTER //Always Splinter.
            AE5_1H_2NF_S AE5_1S_2NF_S
            RR5_4F_1D1H RR_4F    (Always Splinter and 4. suit forcing).
    //European.
    //Always simple limit raise - minor and major.
        RR5_4F_1D1H RR5_4F_1S           (Always 4. suit forcing).

        //Simple limit raise only.
            E5_1H_L_S E5_1S_L_S

            //Non Splinter.
                E5_1H_SL E5_1S_SL
        //Simple limit raise + 2NT major raise forcing (Jacoby 2NT).
            AE5_1H_2NF_S AE5_1S_2NF_S

//1NT 12-14 HCP.
    O12_1N_12_14 O3_1N_12_14 O4_1N_12_14 R_1214 OR_1214_4MN
    //Stayman forcing.
        R_1214_SF OR_1214_SF_1 OR_1214_SF_2
    //Stayman not forcing.
        R_1214_SN OR_1214_SN_1 OR_1214_SN_2

    //2D/2H/2S sign off.
        R_1214_SO OR_1214_SO
    //Jacoby.
        OR_1214_J
        //2S sign off.
            R_1214_J_SO OR_1214_J_SO
        //2S,2N minor transfer.
            R_1214_J_MT OR_1214_J_MT

    //3C/3D slam invitation.
        R_1214_3mSI OR_1214_3mSI
    //3C/3D game invitation.
        R_1214_3mGI OR_1214_3mGI
    //3C/3D pre emptive.
        R_1214_3mPE OR_1214_3mPE

    //3H/3S slam invitation.
        R_1214_3MSI OR_1214_3MSI
    //3H/3S game forcing.
        R_1214_3MGF OR_1214_3MGF
    //3H/3S pre emptive.
        R_1214_3MGPE OR_1214_3MPE
    //3H/3S game invitation.
        R_1214_3MGI OR_1214_3MGI

    //4H/4S natural.
        R_1214_4MN
//1NT 16-18 HCP.
    O_1N_16_18 R_1618 OR_1618_4MN
    //Stayman forcing.
        R_1618_SF OR_1618_SF_1 OR_1618_SF_2
    //Stayman not forcing.
        R_1618_SN OR_1618_SN_1 OR_1618_SN_2

    //2D/2H/2S sign off.
        R_1618_SO OR_1618_SO
    //Jacoby.
        OR_1618_J
        //2S sign off.
            R_1618_J_SO OR_1618_J_SO
        //2S,2N minor transfer.
            R_1618_J_MT OR_1618_J_MT

    //3C/3D slam invitation.
        R_1618_3mSI OR_1618_3mSI
    //3C/3D game invitation.
        R_1618_3mGI OR_1618_3mGI
    //3C/3D pre emptive.
        R_1618_3mPE OR_1618_3mPE

    //3H/3S slam invitation.
        R_1618_3MSI OR_1618_3MSI
    //3H/3S game forcing
        R_1618_3MGF OR_1618_3MGF
    //3H/3S pre emptive.
        R_1618_3MGPE OR_1618_3MPE
    //3H/3S game invitation.
        R_1618_3MGI OR_1618_3MGI

    //4H/4S natural.
        R_1618_4MN
//15-17 HCP.
    O_1N_15_17 R_1517 OR_1517_4MN
    //Stayman forcing.
        R_1517_SF OR_1517_SF_1 OR_1517_SF_2
    //Stayman not forcing.
        R_1517_SN OR_1517_SN_1 OR_1517_SN_2

    //2D/2H/2S sign off.
        R_1517_SO OR_1517_SO
    //Jacoby.
        OR_1517_J
        //2S sign off.
            R_1517_J_SO OR_1517_J_SO
        //2S,2N minor transfer.
            R_1517_J_MT OR_1517_J_MT

    //3C/3D slam invitation.
        R_1517_3mSI OR_1517_3mSI
    //3C/3D game invitation.
        R_1517_3mGI OR_1517_3mGI
    //3C/3D pre emptive.
        R_1517_3mPE OR_1517_3mPE

    //3H/3S slam invitation.
        R_1517_3MSI OR_1517_3MSI
    //3H/3S game forcing
        R_1517_3MGF OR_1517_3MGF
    //3H/3S pre emptive.
        R_1517_3MGPE OR_1517_3MPE
    //3H/3S game invitation.
        R_1517_3MGI OR_1517_3MGI

    //4H/4S natural.
        R_1517_4MN

//Strong 2C.
    O_S2_C A_S2_C
    //Weak 2D, 6-11 HCP. //Weak 2H/2S, 6-11 HCP.
    O12_W2D_6_11 O3_W2D_6_11 O4_W2D_6_11
    O12_W2H2S_6_11 O3_W2H2S_6_11 O4_W2H2S_6_11
    A_W2D A_W2H2S A_W2DX_NAT A_W2H2SX_NAT

    //Ogust after weak 2D, not forcing. //Ogust after weak 2H/2S, not forcing.
        A_W2D_O_NF A_W2H2S_O_NF
    //Ogust after weak 2D, forcing. //Ogust after weak 2H/2S, forcing.
        A_W2D_O_F A_W2H2S_O_F
    //Feature after weak 2D, not forcing. //Feature after weak 2H/2S, not forcing.
        A_W2D_F_NF A_W2H2S_F_NF
    //Feature after weak 2D, forcing. //Feature after weak 2H/2S, forcing.
        A_W2D_F_F A_W2H2S_F_F
//Strong two (2C, 2D, 2H, 2S).
    O_S2_CDHS A_S2_CDHS

//2NT 20-21 HCP.
    O_2N_20_21 A_2N_20_21 A_2N_20_21_L
    //Non Texas.
    A_2N_20_21_NT
    //3C Stayman. 3D, 3H, 3S Natural.
    A_2N_20_21_SN
    //3C Stayman. Later bids.
    A_2N_20_21_SL
    //3D, 3H, 3S Natural. Later bids.
    A_2N_20_21_NL
//2NT 21-22 HCP.
    O_2N_21_22 A_2N_21_22 A_2N_21_22_L
    //Non Texas.
    A_2N_21_22_NT
    //3C Stayman. 3D, 3H, 3S Natural.
    A_2N_21_22_SN
    //3C Stayman. Later bids.
    A_2N_21_22_SL
    //3D, 3H, 3S Natural. Later bids.
    A_2N_21_22_NL
//2NT 22-24 HCP.
    O_2N_22_24 A_2N_22_24 A_2N_22_24_L
    //Non Texas.
    A_2N_22_24_NT
    //3C Stayman. 3D, 3H, 3S Natural.
    A_2N_22_24_SN
    //3C Stayman. Later bids.
    A_2N_22_24_SL
    //3D, 3H, 3S Natural. Later bids.
    A_2N_22_24_NL

//2NT after 1/1 18-21 HCP
    RR_2N_11_1821
//2NT after 1/1 17-18 HCP
    RR_2N_11_1718

//1NT 12-14 HCP
    RR45_2N_21_1518
//2NT after 2/1 15-18 HCP
    RR45_2N_21_1518
//2NT after 2/1 12-14 HCP
    RR45_2N_21_1214

//After 2C-2D-2NT opening.
//22-24 HCP.
    A_2C2D2N_22_24
    //Non Texas.
    A_2C2D2N_22_24_NT
    //3C Stayman. 3D, 3H, 3S Natural.
    A_2C2D2N_22_24_SN
    //3C Stayman. Later bids.
    A_2C2D2N_22_24_SL
    //3D, 3H, 3S Natural. Later bids.
    A_2C2D2N_22_24_NL
//After 2C-2D-2NT opening.
//23-24 HCP.
    A_2C2D2N_23_24
    //Non Texas.
    A_2C2D2N_23_24_NT
    //3C Stayman. 3D, 3H, 3S Natural.
    A_2C2D2N_23_24_SN
    //3C Stayman. Later bids.
    A_2C2D2N_23_24_SL
    //3D, 3H, 3S Natural. Later bids.
    A_2C2D2N_23_24_NL

//3. level.
    O12_3LEVEL O3_3LEVEL O4_3LEVEL A_3LEVEL

//3NT 25-27 HCP.
    O_3N_25_27 A_3N_25_27
//3NT gambling (1., 2. and 3. seat). //3NT gambling (4. seat).
    O123_3N_GMBL A_3N_GMBL
    O4_3N_GMBL
//3NT Acol.
    O_3N_ACOL A_3N_ACOL


//Rules ids for bid database --------------------------------------------------------
//4-major.
    O_1H_4 O_1S_4                       //1H and 1S promises 4 card suit.
//5-major.
    O_1H_5 O_1S_5                       //1H and 1S promises 5 card suit.

O_LOWER                                 //Open lower suit first.

//Weak 1NT.
    WK1N
    //1NT after 1/1: 15-16 HCP.
        WK1N_R_NF;                      //1NT after 1/1: 15-16 HCP
    //1NT after 1/1: 15-18 HCP.
        WK1N_R_GF;                      //1NT after 1/1: 15-18 HCP
//Strong 1NT.
    ST1N
    //Open 1NT  with weak major.
        O_1N_WM

    //Splinter.
        SPL_O<<SPL_D                    //Splinter offensive and defensive side.
    //No splinter.
        SPL_N                           //No splinter.

//2N after 1/1: 18-20 HCP.
    OR_11_2N_GF
//2N after 1/1: 17-18 HCP.
    OR_11_2N_NF

//2N after 2/1: 15-18 HCP.
    OR_21_2N_GF
//2N after 2/1: 12-14 HCP.
    OR_21_2N_NF

//3N after 1/1: long minor.
    OR_11_3N_LM
//3N after 1/1: 19+ HCP
    OR_11_3N_N

//Jacoby 2NT major raise
    R_M_2N_J

//1N-3C/3D Slam invitation.
    R_m_1N_S
//1N-3C/3D Game invitation or preemptive.
    R_m_1N_G

//2NT after 2C: 22-24 HCP.
    R_2C_2NT_2224
//2NT after 2C: 23-24 HCP.
    R_2C_2NT_2324;

//Sound 3 level preempt not vul/vul.
    S_3L_P_NV S_3L_P_V
//Light 3 level preempt not vul/vul.
    L_3L_P_NV L_3L_P_V
//sound/light 3 level preempt vul/not vul.
    S_3L_P_V L_3L_P_NV

//Opponent overcalled: responder jump raise is invitational.
    R_JR_LIM
//Opponent overcalled: responder jump raise is preemptive.
    R_JR_PRE

//Constructive overcall (raise is invitational).
    D_CON
//Aggressive overcall (raise is preemptive).
    D_AGR

//Four card overcall (good).
    D_OVR_4

//Jump overcall weak.
    D_J_OWK
//Jump overcall intermediate.
    D_J_OIM

GHES_OFF;                  //Ghestem convention is off.
CBS_OFF;                   //Checkback Stayman is off.

//2C-2D negative.
    R_2C_2D_N
//2C-2D waiting.
    R_2C_2D_W

//3 response Stayman.
    R_1N_S
//4 response Stayman.
    R_1N_4R_S

//1N-2N natural.
    R_1N_2N_N
//1N-2N transfer.
    R_1N_2N_T

//2N-3M natural forcing.
    R_2N_3M

//Redouble escape.
    RD_ESC
//Redouble pending.
    RD_PND

//Takeout standard offensive side.
    STD_O
//Takeout ??? offensive side.
    T_OUT_D_O

//Takeout standard defensive side.
    STD_D
//Takeout ??? defensive side.
    T_OUT_D_D
*/

/**
 * @brief This class calculates pages and rule ids for a given set of bid options.
 *
 * The pages and rule ids are as defined by the constants above. The calculations are
 * hardcoded in this class.
 */
class CBidDBDefine
{
public:
    CBidDBDefine(CBidOptionDoc &nsBidOptions, CBidOptionDoc &ewBidOptions);
    QSet<qint16> &getPages(Seat seat);
    QSet<qint16> &getRules(Seat seat);

private:
    void calcIds(CBidOptionDoc &bidOptions, QSet<qint16> &pages, QSet<qint16> &rules);
    QSet<qint16> nsPages;
    QSet<qint16> ewPages;
    QSet<qint16> nsRules;
    QSet<qint16> ewRules;
};

#endif // CBIDDBDEFINE_H
