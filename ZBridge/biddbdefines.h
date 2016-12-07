/*Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: BidDBDefines.h
  Developers: eak

  Revision History:
  1-dec-2016 eak: Original

  Abstract: Defines for bidding database.

  Platforms: Qt.

*/

#ifndef BIDDBDEFINES
#define BIDDBDEFINES

//Opening bids---------------------------------------------------------------
//Open 1. and 2. seat.
const int O12_4M_SN_L = 1;      //4-major, strong 1NT, lower first.
const int O12_4M_WN_L = 2;      //4-major, weak 1NT, lower first:
const int O12_5M_SN = 3;        //5-major, strong 1NT.
const int O12_5M_WN = 1004;     //5-major, weak 1NT.
const int O12_W2D_6_11 = 4;     //Weak 2D, 6-11 HCP.
const int O12_W2H2S_6_11 = 5;   //Weak 2H/2S, 6-11 HCP.
const int O12_1N_12_14 = 6;     //1NT 12-14 HCP.
const int O12_3LEVEL = 7;       //3. level.
//Open 3. seat.
const int O3_4M_SN_L = 10;      //4-major, strong 1NT, lower first.
const int O3_4M_WN_L = 11;      //4-major, weak 1NT, lower first.
const int O3_5M_SN = 12;        //5-major, strong 1NT.
const int O3_5M_WN = 13;        //5-major, weak 1NT.
const int O3_W2D_6_11 = 14;     //Weak 2D, 6-11 HCP.
const int O3_W2H2S_6_11 = 15;   //Weak 2H/2S, 6-11 HCP.
const int O3_1N_12_14 = 16;     //1NT 12-14 HCP.
const int O3_3LEVEL = 17;       //3. level.
//Open 4. seat.
const int O4_4M_SN_L = 20;      //4-major, strong 1NT, lower first.
const int O4_4M_WN_L = 21;      //4-major, weak 1NT, lower first.
const int O4_5M_SN = 22;        //5-major, strong 1NT.
const int O4_5M_WN = 23;        //5-major, weak 1NT.
const int O4_W2D_6_11 = 24;     //Weak 2D, 6-11 HCP.
const int O4_W2H2S_6_11 = 25;   //Weak 2H/2S, 6-11 HCP.
const int O4_1N_12_14 = 26;     //1NT 12-14 HCP.
const int O4_3LEVEL = 27;       //3. level.
//Open any seat.
const int O_1N_16_18 = 30;      //1NT 16-18 HCP.
const int O_1N_15_17 = 31;      //1NT 15-17 HCP.
const int O_S2_CDHS = 35;       //Strong two (2C, 2D, 2H, 2S).
const int O_S2_C = 36;          //Strong 2C.
const int O_3N_25_27 = 37;      //3NT 25-27 HCP.
const int O123_3N_GMBL = 38;    //3NT gambling (1., 2. and 3. seat).
const int O4_3N_GMBL = 39;      //3NT gambling (4. seat).
const int O_3N_ACOL = 40;       //3NT Acol.
const int O_2N_20_21 = 41;      //2NT 20-21 HCP.
const int O_2N_21_22 = 42;      //2NT 21-22 HCP.
const int O_2N_22_24 = 43;      //2NT 22-24 HCP.

//After 2 (C, D, H, S)level opening----------------------------------------------
const int A_S2_CDHS = 50;       //After strong 2 opening.
const int A_S2_C = 51;          //After strong 2C opening.
const int A_W2D = 52;           //After weak 2D opening.
const int A_W2H2S = 53;         //After weak 2H/2S opening.
const int A_W2D_O_NF = 1090;    //Ogust after weak 2D, not forcing.
const int A_W2D_F_NF = 1053;    //Feature after weak 2D, not forcing.
const int A_W2D_O_F = 1091;     //Ogust after weak 2D, forcing.
const int A_W2D_F_F = 1054;     //Feature after weak 2D, forcing.
const int A_W2H2S_O_NF = 1092;  //Ogust after weak 2H/2S, not forcing.
const int A_W2H2S_F_NF = 1055;  //Feature after weak 2H/2S, not forcing.
const int A_W2H2S_O_F = 1093;   //Ogust after weak 2H/2S, forcing.
const int A_W2H2S_F_F = 1056;   //Feature after weak 2H/2S, forcing.
const int A_W2DX_NAT = 54;      //After weak 2D opening and intervening takeout double.
const int A_W2H2SX_NAT = 55;    //After weak 2H/2S opening and intervening takeout double.

//After 2NT opening--------------------------------------------------------------
//20-21 HCP.
const int A_2N_20_21 = 60;      //After opening.
const int A_2N_20_21_SN = 61;   //3C Stayman. 3D, 3H, 3S Natural.
const int A_2N_20_21_NT = 62;   //Non Texas.
const int A_2N_20_21_L = 63;    //Later bids.
const int A_2N_20_21_NL =64;    //3D, 3H, 3S Natural. Later bids.
const int A_2N_20_21_SL = 1064; //3C Stayman. Later bids.
//21-22 HCP.
const int A_2N_21_22 = 65;      //After opening.
const int A_2N_21_22_SN = 66;   //3C Stayman. 3D, 3H, 3S Natural.
const int A_2N_21_22_NT = 67;   //Non Texas.
const int A_2N_21_22_L = 68;    //Later bids.
const int A_2N_21_22_NL =69;    //3D, 3H, 3S Natural. Later bids.
const int A_2N_21_22_SL = 1069; //3C Stayman. Later bids.
//22-24 HCP.
const int A_2N_22_24 = 70;      //After opening.
const int A_2N_22_24_SN = 71;   //3C Stayman. 3D, 3H, 3S Natural.
const int A_2N_22_24_NT = 22;   //Non Texas.
const int A_2N_22_24_L = 73;    //Later bids.
const int A_2N_22_24_NL =74;    //3D, 3H, 3S Natural. Later bids.
const int A_2N_22_24_SL = 1074; //3C Stayman. Later bids.
//After 2C-2D-2NT opening.
//22-24 HCP.
const int A_2C2D2N_22_24 = 80;  //After opening.
const int A_2C2D2N_22_24_SN = 81;//3C Stayman. 3D, 3H, 3S Natural.
const int A_2C2D2N_22_24_NT = 82;//Non Texas.
const int A_2C2D2N_22_24_NL = 83;//3D, 3H, 3S Natural. Later bids.
const int A_2C2D2N_22_24_SL = 1083;//3C Stayman. Later bids.
//23-24 HCP.
const int A_2C2D2N_23_24 = 84;  //After opening.
const int A_2C2D2N_23_24_SN = 85;//3C Stayman. 3D, 3H, 3S Natural.
const int A_2C2D2N_23_24_NT = 86;//Non Texas.
const int A_2C2D2N_23_24_NL = 87;//3D, 3H, 3S Natural. Later bids.
const int A_2C2D2N_23_24_SL = 1087;//3C Stayman. Later bids.

//After 3NT opening---------------------------------------------------------------
const int A_3N_25_27 = 90;      //After 3NT 25-27 HCP.
const int A_3N_GMBL = 91;       //After 3NT gambling.
const int A_3N_ACOL = 92;       //After 3NT ACOL.

//After pre emptive opening-------------------------------------------------------
const int A_3LEVEL = 100;       //

//After 1 level opening (1C, 1D, 1H, 1S)--------------------------------------------------
//Non Splinter.
const int A4_FF = 110;          //American 4-major, major/minor forcing/forcing raise.
const int A5_FF = 111;          //American 5-major, major/minor forcing/forcing raise.
const int A5_SF = 112;          //American 5-major, major/minor simple limit/forcing raise.
const int A5_SL = 113;          //American 5-major, major/minor simple limit/limit raise.
const int E45_SL = 114;         //European 4/5-major, major/minor simple limit/limit raise.
const int E4_1H_SL = 115;       //European 4-major (1H), major/minor simple limit/limit raise.
const int E4_1S_SL = 116;       //European 4-major (1S), major/minor simple limit/limit raise.
const int E5_1H_SL = 117;       //European 5-major (1H), major/minor simple limit/limit raise.
const int E5_1S_SL = 118;       //European 5-major (1S), major/minor simple limit/limit raise.
//Splinter.
const int A45_L_S = 119;        //American 4/5 major, minor limit raise, splinter.
const int E45_L_S = 120;        //European 4/5 major, minor limit raise, splinter.
const int E4_1H_L_S = 121;      //European 4 major (1H), major limit raise, splinter.
const int E4_1S_L_S = 122;      //European 4 major (1S), major limit raise, splinter.
const int E5_1H_L_S = 123;      //European 5 major (1H), major limit raise, splinter.
const int E5_1S_L_S = 124;      //European 5 major (1S), major limit raise, splinter.
const int AE4_1H_2NF_S = 125;   //4 major (1H), major raise 2NT forcing, splinter.
const int AE4_1S_2NF_S = 126;   //4 major (1S), major raise 2NT forcing, splinter.
const int AE5_1H_2NF_S = 127;   //5 major (1H), major raise 2NT forcing, splinter.
const int AE5_1S_2NF_S = 128;   //5 major (1S), major raise 2NT forcing, splinter.
const int A4_3NF_S = 129;       //American 4 major, major raise 3NT forcing, splinter.
const int A5_3NF_S = 130;       //American 5 major, major raise 3NT forcing, splinter.
//Openers rebid.
const int OR_COM_1 = 131;       //Common sequences.
const int OR_COM_2 = 135;       //Common sequences.
const int OR4_1H_1S = 136;      //4 major (open 1H, response 1S).
const int OR5_1H_1S = 137;      //5 major (open 1H, response 1S).
const int OR4_1H_2 = 138;       //4 major (open 1H, response 2x).
const int OR4_1S_2 = 139;       //4 major (open 1S, response 2x).
const int OR5_1H_2 = 140;       //5 major (open 1H, response 2x).
const int OR5_1S_2 = 141;       //5 major (open 1S, response 2x).
const int OR4_1H = 142;         //4 major (open 1H).
const int OR4_1S = 144;         //4 major (open 1S).
const int OR5_1H = 146;         //5 major (open 1H).
const int OR5_1S = 148;         //5 major (open 1S).
const int OR4_1H_1N = 150;      //4 major (open 1H, response 1N).
const int OR4_1S_1N = 151;      //4 major (open 1S, response 1N).
const int OR5_1H_1N = 152;      //5 major (open 1H, response 1N).
const int OR5_1H_1N = 153;      //5 major (open 1S, response 1N).
const int OR_1m_2m_FL = 154;    //Open 1 minor, response 2 minor, raise forcing/limit.
const int OR_1m_3m_F = 155;     //Open 1 minor, response 3 minor, raise forcing.
const int OR_1m_3m_L = 156;     //Open 1 minor, response 3 minor, raise limit.
const int ORE_1C_1N = 1156;     //Open European 1C, response 1N.
const int ORA_1C_1N = 157;      //Open American 1C, response 1N.
const int ORE_1m_2N = 158;      //Open European 1 minor, response 2N.
const int ORA_1m_2N = 159;      //Open American 1 minor, response 2N.
const int ORE_1m_3N = 160;      //Open European 1 minor, response 3N.
const int ORA_1m_3N = 161;      //Open American 1 minor, response 3N.
const int OR_1M_3M_F = 162;     //Open 1 major, response 3 major, raise forcing.
const int OR_1M_3M_L = 163;     //Open 1 major, response 3 major, raise limit.
const int OR_SPLINTER = 164;    //Splinter raise (from 1C/1D/1H/1S).
const int ORA4_1M_2N_N = 165;   //American 4 major. Open 1 major, response 2N. Natural raise.
const int ORA5_1M_2N_N = 166;   //American 5 major. Open 1 major, response 2N. Natural raise.
const int ORE45_1M_2N_N = 167;  //European 4/5 major. Open 1 major, response 2N. Natural raise.
const int ORE45_1M_2N_J = 168;  //Open 1 major, response 2N. Jacoby 2NT raise.
const int ORA4_1M_3N_N = 169;   //American 4 major. Open 1 major, response 3N. Natural raise.
const int ORA5_1M_3N_N = 170;   //American 5 major. Open 1 major, response 3N. Natural raise.
const int ORA_1M_3N_F = 171;    //American. Open 1 major, response 3N. 3N forcing raise.
const int OR4_1H_3N_N = 172;    //4 major. Open 1H, response 3N. Natural raise.
const int OR4_1S_3N_N = 173;    //4 major. Open 1S, response 3N. Natural raise.
const int OR5_1H_3N_N = 174;    //5 major. Open 1H, response 3N. Natural raise.
const int OR5_1S_3N_N = 175;    //5 major. Open 1S, response 3N. Natural raise.
//Responders rebid or later bids.
const int RR_COM = 176;         //Common sequences.
const int RR4_1D1H = 178;       //4 major. Open 1D/1H.
const int RR4_1S = 179;         //4 major. Open 1S.
const int RR5_1D1H = 180;       //5 major. Open 1D/1H.
const int RR5_1S = 181;         //5 major. Open 1S.
const int RR_3N_F = 182;        //3NT forcing raise.
const int RR_2N_11_1821 = 183;  //2NT after 1/1: 18-21 HCP.
const int RR_2N_11_1718 = 185;  //2NT after 1/1: 17-18 HCP.
const int RR45_2N_21_1518 = 187;//2NT after 1/1: 18-21 HCP.
const int RR45_2N_21_1214 = 190;//2NT after 1/1: 17-18 HCP.
const int RR_4N = 191;          //4. suit natural.
const int RR4_4N = 192;         //4 major. 4. suit natural.
const int RR5_4N = 193;         //5 major. 4. suit natural.
const int RR_4F = 194;          //4. suit forcing.
const int RR4_4F_1D1H = 195;    //4 major. 4. suit forcing. Open 1D/1H.
const int RR4_4F_1S = 196;      //4 major. 4. suit forcing. Open 1S.
const int RR5_4F_1D1H = 197;    //5 major. 4. suit forcing. Open 1D/1H.
const int RR5_4F_1S = 198;      //5 major. 4. suit forcing. Open 1S.
const int RR_S = 199;           //Splinter raise.
const int RR_WN_1820_N = 200;   //Weak 1NT, 2NT after 1/1: 18-20, new minor natural.
const int RR_WN_1820_F = 201;   //Weak 1NT, 2NT after 1/1: 18-20, new minor forcing.
const int RR_WN_1718_N = 202;   //Weak 1NT, 2NT after 1/1: 17-18, new minor natural.
const int RR_WN_1718_F = 203;   //Weak 1NT, 2NT after 1/1: 17-18, new minor forcing.
const int RR4_WN_1820_N = 204;  //4 major. Weak 1NT, 2NT after 1/1: 17-20, new minor natural.
const int RR4_WN_1820_F = 205;  //4 major. Weak 1NT, 2NT after 1/1: 18-20, new minor forcing.
const int RR4_WN_1718_N = 206;  //4 major. Weak 1NT, 2NT after 1/1: 17-18, new minor natural.
const int RR4_WN_1718_F = 207;  //4 major. Weak 1NT, 2NT after 1/1: 17-18, new minor forcing.
const int RR5_WN_1820_N = 208;  //5 major. Weak 1NT, 2NT after 1/1: 17-20, new minor natural.
const int RR5_WN_1820_F = 209;  //5 major. Weak 1NT, 2NT after 1/1: 18-20, new minor forcing.
const int RR5_WN_1718_N = 211;  //5 major. Weak 1NT, 2NT after 1/1: 17-18, new minor natural.
const int RR5_WN_1718_F = 212;  //5 major. Weak 1NT, 2NT after 1/1: 17-18, new minor forcing.
const int RR_SN_N = 213;        //Strong 1NT, new minor natural.
const int RR_SN_F = 214;        //Strong 1NT, new minor forcing.
const int RR4_SN_N = 215;       //4 major. Strong 1NT, new minor natural.
const int RR4_SN_F = 216;       //4 major. Strong 1NT, new minor forcing.
const int RR5_SN_N = 217;       //5 major. Strong 1NT, new minor natural.
const int RR5_SN_F = 218;       //5 major. Strong 1NT, new minor forcing.

//After 1NT opening---------------------------------------------------------------------
//Responses.
const int R_1618 = 220;         //1NT 16-18 HCP. Common.
const int R_1517 = 221;         //1NT 15-17 HCP. Common.
const int R_1214 = 222;         //1NT 12-14 HCP. Common.
const int R_1618_SF = 226;      //1NT 16-18 HCP. Stayman forcing.
const int R_1517_SF = 227;      //1NT 15-17 HCP. Stayman forcing.
const int R_1214_SF = 228;      //1NT 12-14 HCP. Stayman forcing.
const int R_1618_SN = 229;      //1NT 16-18 HCP. Stayman not forcing.
const int R_1517_SN = 230;      //1NT 15-17 HCP. Stayman not forcing.
const int R_1214_SN = 231;      //1NT 12-14 HCP. Stayman not forcing.
const int R_1618_SO = 232;      //1NT 16-18 HCP. 2D/2H/2S sign off.
const int R_1517_SO = 233;      //1NT 15-17 HCP. 2D/2H/2S sign off.
const int R_1214_SO = 234;      //1NT 12-14 HCP. 2D/2H/2S sign off.
const int R_1618_J_SO = 235;    //1NT 16-18 HCP. Jacoby. 2S sign off.
const int R_1517_J_SO = 236;    //1NT 15-17 HCP. Jacoby. 2S sign off.
const int R_1214_J_SO = 237;    //1NT 12-14 HCP. Jacoby. 2S sign off.
const int R_1618_J_MT = 238;    //1NT 16-18 HCP. Jacoby. 2S,2N minor transfer.
const int R_1517_J_MT = 239;    //1NT 15-17 HCP. Jacoby. 2S 2N minor transfer.
const int R_1214_J_MT = 240;    //1NT 12-14 HCP. Jacoby. 2S 2N minor transfer.
const int R_1618_3mST = 241;    //1NT 16-18 HCP. 3C/3D slam try.
const int R_1517_3mST = 242;    //1NT 15-17 HCP. 3C/3D slam try.
const int R_1214_3mST = 243;    //1NT 12-14 HCP. 3C/3D slam try.
const int R_1618_3mGT = 244;    //1NT 16-18 HCP. 3C/3D game try.
const int R_1517_3mGT = 245;    //1NT 15-17 HCP. 3C/3D game try.
const int R_1214_3mGT = 246;    //1NT 12-14 HCP. 3C/3D game try.
const int R_1618_3mPE = 247;    //1NT 16-18 HCP. 3C/3D pre emptive.
const int R_1517_3mPE = 248;    //1NT 15-17 HCP. 3C/3D pre emptive.
const int R_1214_3mPE = 249;    //1NT 12-14 HCP. 3C/3D pre emptive.
const int R_1618_3MST = 250;    //1NT 16-18 HCP. 3H/3S slam try.
const int R_1517_3MST = 251;    //1NT 15-17 HCP. 3H/3S slam try.
const int R_1214_3MST = 252;    //1NT 12-14 HCP. 3H/3S slam try.
const int R_1618_3MGF = 253;    //1NT 16-18 HCP. 3H/3S game forcing.
const int R_1517_3MGF = 254;    //1NT 15-17 HCP. 3H/3S game forcing.
const int R_1214_3MGF = 255;    //1NT 12-14 HCP. 3H/3S game forcing.
const int R_1618_3MGF = 256;    //1NT 16-18 HCP. 3H/3S pre emptive.
const int R_1517_3MGF = 257;    //1NT 15-17 HCP. 3H/3S pre emptive.
const int R_1214_3MGF = 258;    //1NT 12-14 HCP. 3H/3S pre emptive.
const int R_1618_3MGF = 259;    //1NT 16-18 HCP. 3H/3S game invitation.
const int R_1517_3MGF = 260;    //1NT 15-17 HCP. 3H/3S game invitation.
const int R_1214_3MGF = 261;    //1NT 12-14 HCP. 3H/3S game invitation.
const int R_1618_4MN = 262;     //1NT 16-18 HCP. 4H/4S natural.
const int R_1517_4MN = 263;     //1NT 15-17 HCP. 4H/4S natural.
const int R_1214_4MN = 264;     //1NT 12-14 HCP. 4H/4S natural.
//Rebids.
const int OR_1618_4MN = 265;    //1NT 16-18 HCP. Common.
const int OR_1517_4MN = 267;    //1NT 15-17 HCP. Common.
const int OR_1214_4MN = 269;    //1NT 12-14 HCP. Common.
const int OR_1618_SF_1 = 271;   //1NT 16-18 HCP. Forcing Stayman.
const int OR_1517_SF_1 = 273;   //1NT 15-17 HCP. Forcing Stayman.
const int OR_1214_SF_1 = 275;   //1NT 12-14 HCP. Forcing Stayman.
const int OR_1618_SN_1 = 276;   //1NT 16-18 HCP. Non forcing Stayman.
const int OR_1517_SN_1 = 277;   //1NT 15-17 HCP. Non forcing Stayman.
const int OR_1214_SN_1 = 278;   //1NT 12-14 HCP. Non forcing Stayman.
const int OR_1618_SF_2 = 279;   //1NT 16-18 HCP. Forcing Stayman.
const int OR_1517_SF_2 = 280;   //1NT 15-17 HCP. Forcing Stayman.
const int OR_1214_SF_2 = 281;   //1NT 12-14 HCP. Forcing Stayman.
const int OR_1618_SN_2 = 282;   //1NT 16-18 HCP. Non forcing Stayman.
const int OR_1517_SN_2 = 283;   //1NT 15-17 HCP. Non forcing Stayman.
const int OR_1214_SN_2 = 284;   //1NT 12-14 HCP. Non forcing Stayman.
const int OR_1618_SO = 285;      //1NT 16-18 HCP. 2D/2H/2S sign off.
const int OR_1517_SO = 286;      //1NT 15-17 HCP. 2D/2H/2S sign off.
const int OR_1214_SO = 287;      //1NT 12-14 HCP. 2D/2H/2S sign off.
const int OR_1618_J = 289;       //1NT 16-18 HCP. Jacoby.
const int OR_1517_J = 290;       //1NT 15-17 HCP. Jacoby.
const int OR_1214_J = 291;       //1NT 12-14 HCP. Jacoby.
const int OR_1618_J_SO = 292;    //1NT 16-18 HCP. Jacoby. 2S sign off.
const int OR_1517_J_SO = 293;    //1NT 15-17 HCP. Jacoby. 2S sign off.
const int OR_1214_J_SO = 294;    //1NT 12-14 HCP. Jacoby. 2S sign off.
const int OR_1618_J_MT = 295;    //1NT 16-18 HCP. Jacoby. 2S,2N minor transfer.
const int OR_1517_J_MT = 296;    //1NT 15-17 HCP. Jacoby. 2S 2N minor transfer.
const int OR_1214_J_MT = 297;    //1NT 12-14 HCP. Jacoby. 2S 2N minor transfer.
const int OR_1618_3mST = 298;    //1NT 16-18 HCP. 3C/3D slam try.
const int OR_1517_3mST = 299;    //1NT 15-17 HCP. 3C/3D slam try.
const int OR_1214_3mST = 300;    //1NT 12-14 HCP. 3C/3D slam try.
const int OR_1618_3mGT = 301;    //1NT 16-18 HCP. 3C/3D game try.
const int OR_1517_3mGT = 302;    //1NT 15-17 HCP. 3C/3D game try.
const int OR_1214_3mGT = 303;    //1NT 12-14 HCP. 3C/3D game try.
const int OR_1618_3mPE = 304;    //1NT 16-18 HCP. 3C/3D pre emptive.
const int OR_1517_3mPE = 305;    //1NT 15-17 HCP. 3C/3D pre emptive.
const int OR_1214_3mPE = 306;    //1NT 12-14 HCP. 3C/3D pre emptive.
const int OR_1618_3MST = 307;    //1NT 16-18 HCP. 3H/3S slam try.
const int OR_1517_3MST = 308;    //1NT 15-17 HCP. 3H/3S slam try.
const int OR_1214_3MST = 309;    //1NT 12-14 HCP. 3H/3S slam try.
const int OR_1618_3MGF = 310;    //1NT 16-18 HCP. 3H/3S game forcing.
const int OR_1517_3MGF = 311;    //1NT 15-17 HCP. 3H/3S game forcing.
const int OR_1214_3MGF = 312;    //1NT 12-14 HCP. 3H/3S game forcing.
const int OR_1618_3MPE = 313;    //1NT 16-18 HCP. 3H/3S pre emptive.
const int OR_1517_3MPE = 314;    //1NT 15-17 HCP. 3H/3S pre emptive.
const int OR_1214_3MPE = 315;    //1NT 12-14 HCP. 3H/3S pre emptive.
const int OR_1618_3MGI = 316;    //1NT 16-18 HCP. 3H/3S game invitation.
const int OR_1517_3MGI = 317;    //1NT 15-17 HCP. 3H/3S game invitation.
const int OR_1214_3MGI = 318;    //1NT 12-14 HCP. 3H/3S game invitation.
//Defenders side.
const int D_S = 320;            //Strong 1NT.
const int D_W = 321;            //Weak 1NT.
const int D_S_SO = 322;         //Strong 1NT. 2D/2H/2S signoff.
const int D_W_SO = 323;         //Weak 1NT. 2D/2H/2S signoff.
const int D_S_J = 324;          //Strong 1NT. Jacoby.
const int D_W_J = 325;          //Weak 1NT. Jacoby.
const int D_S_J_MT = 326;       //Strong 1NT. Jacoby. 2S,2N minor transfer.
const int D_W_J_MT = 327;       //Weak 1NT. Jacoby. 2S,2N minor transfer.
const int D_S_3mSGT = 328;      //Strong 1NT. 3C/3D slam or game try.
const int D_W_3mSGT = 329;      //Weak 1NT. 3C/3D slam or game try.
const int D_S_3mPE3M = 330;     //Strong 1NT. 3C/3D pre emptive. 3H/3S slam try or game forcing or game inv.
const int D_W_3mPE3M = 331;     //Weak 1NT. 3C/3D pre emptive. 3H/3S slam try or game forcing or game inv.
const int D_S_3MPE = 332;       //Strong 1NT. 3H/3S pre emptive.
const int D_W_3MPE = 333;       //Weak 1NT.  3H/3S pre emptive.
const int D_S_J_MT = 332;       //Strong 1NT. 3C/3D slam try.
const int D_W_J_MT = 333;       //Weak 1NT. 3C/3D slam try.
const int D_S_4MN = 334;        //Strong 1NT. 4H/4S natural.
const int D_W_4MN = 335;        //Weak 1NT. 4H/4S natural.
const int D_S_NO = 336;         //Strong 1NT. Natural overcall.
const int D_W_NO = 337;         //Weak 1NT. Natural overcall.
const int D_S_N2C_N = 338;      //Strong 1NT. Responder over overcall natural 2C.Intervention natural.
const int D_W_N2C_N = 339;      //Weak 1NT. Responder over overcall natural 2C.Intervention natural.
const int D_S_N2D_N = 340;      //Strong 1NT. Responder over overcall natural 2D.Intervention natural.
const int D_W_N2D_N = 341;      //Weak 1NT. Responder over overcall natural 2D.Intervention natural.
const int D_S_N2H2S_N = 1341;   //Strong 1NT. Responder over overcall natural 2H/2S. Intervention natural.
const int D_W_N2H2S_N = 1342;   //Weak 1NT. Responder over overcall natural 2H/2S. Intervention natural.

//Competitive bidding after 1 in a suit opening-----------------------------------------------------
//Overcall and takeout double.
const int D_C_COMMON = 350;     //1C common.
const int D_D_COMMON = 351;     //1D common.
const int D_H_COMMON = 352;     //1H common.
const int D_S_COMMON = 353;     //1S common.
const int D_C1_916 = 354;       //1C 1 level overcall 9-16 HCP.
const int D_D1_916 = 355;       //1D 1 level overcall 9-16 HCP.
const int D_H1_916 = 356;       //1H 1 level overcall 9-16 HCP.
const int D_S1_916 = 357;       //1S 1 level overcall 9-16 HCP.
const int D_C2_1116_69 = 358;   //1C 2 level simple 11-16, jump weak 6-9 HCP.
const int D_D2_1116_69 = 359;   //1D 2 level simple 11-16, jump weak 6-9 HCP.
const int D_H2_1116_69 = 360;   //1H 2 level simple 11-16, jump weak 6-9 HCP.
const int D_S2_1116_69 = 361;   //1S 2 level simple 11-16, jump weak 6-9 HCP.
const int D_C2_1116_1316 = 362; //1C 2 level simple 11-16, jump intermediate 13-16 HCP.
const int D_D2_1116_1316 = 363; //1D 2 level simple 11-16, jump intermediate 13-16 HCP.
const int D_H2_1116_1316 = 364; //1H 2 level simple 11-16, jump intermediate 13-16 HCP.
const int D_S2_1116_1316 = 365; //1S 2 level simple 11-16, jump intermediate 13-16 HCP.
const int D_C_12 = 366;         //1C takeout double minimum 12 HCP.
const int D_D_12 = 367;         //1D takeout double minimum 12 HCP.
const int D_H_12 = 368;         //1H takeout double minimum 12 HCP.
const int D_S_12 = 369;         //1S takeout double minimum 12 HCP.
const int D_C_SCB = 370;        //1C strong cue bid.
const int D_D_SCB = 371;        //1D strong cue bid.
const int D_H_SCB = 372;        //1H strong cue bid.
const int D_S_SCB = 373;        //1S strong cue bid.
const int D_C_MCB = 374;        //1C Michaels cue bid.
const int D_D_MCB = 375;        //1D Michaels cue bid.
const int D_H_MCB = 376;        //1H Michaels cue bid.
const int D_S_MCB = 377;        //1S Michaels cue bid.
//Opening side.
const int D_OPS_COMMON = 380;   //Common sequence.
const int D_OPS_L = 381;        //Raise limit.
const int D_OPS_SCB = 382;      //Strong cue bid.
const int D_OPS_MCB = 383;      //Michaels cue bid.
const int D_OPS_PD = 384;       //Penalty double against overcall.
const int D_OPS_ND = 385;       //Negative double against overcall.
const int D4_OPS_1H_PD = 386;   //4 major 1H, penalty double against overcall.
const int D4_OPS_1S_PD = 387;   //4 major 1S, penalty double against overcall.
const int D4_OPS_1H_ND = 388;   //4 major 1H, negative double against overcall.
const int D4_OPS_1S_ND = 389;   //4 major 1S, negative double against overcall.
const int D5_OPS_1H_PD = 390;   //5 major 1H, penalty double against overcall.
const int D5_OPS_1S_PD = 391;   //5 major 1S, penalty double against overcall.
const int D5_OPS_1H_ND = 392;   //5 major 1H, negative double against overcall.
const int D5_OPS_1S_ND = 393;   //5 major 1S, negative double against overcall.
//Overcallers side.
const int D_OVS_COMMON = 394;   //Common sequence.
const int D_OVS_RDP = 395;      //Response by doublers partner.
const int D_OVS_CONO = 396;     //After constructive overcall.
const int D_OVS_RD_1 = 397;     //Responsive double Response over RHO's raise.
const int D_OVS_RD_2 = 398;     //Responsive double. After constructive overcall.
const int D_OVS_R_SCB = 399;    //Response to strong cue bid.
const int D_OVS_R_MCB = 400;    //Response to Michaels cue bid.
const int D_OVS_R_WJO_N = 401;  //Response to weak jump overcall. 2NT natural.
const int D_OVS_R_WJO_O = 402;  //Response to weak jump overcall. 2NT Ogust.
const int D_OVS_R_WJO_F = 403;  //Response to weak jump overcall. 2NT Feature.
const int D_OVS_R_IJO = 404;    //Response to intermediate jump overcall.
const int D_OVS_ND = 405;       //Negative double after constructive overcall.

//Other competitive auctions-------------------------------------------------------------
//Reopening auction and overcall in indirect position.
const int D_RO_COMMON = 406;    //Reopening auction.
const int D_O_IND = 407;        //Overcall in indirect position.
const int D_mLF = 408;          //Minor limit/forcing raise.
const int D_MCB_OFF = 409;      //Michaels cue bid off.
const int D_MCB_ON = 410;       //Michaels cue bid on.
const int D_SNT_OFF = 411;      //Sandwich NT off.
const int D_WJO_SNT_OFF = 412;  //Weak jump overcall, sandwich NT off.
const int D_IJO_SNT_OFF = 413;  //Intermediate jump overcall, sandwich NT off.
//Balancing, passed hand overcall.
const int D_B_PO_COMMON = 414;  //Common.
const int D_B_PO_S = 415;       //Strong 1NT.
const int D_B_PO_W = 416;       //Weak 1NT.
//Rebid by opener and later auction.
const int D_RB_COMMON_1 = 417;  //Common.
const int D_RB_SD_OFF = 418;    //Support double off.
const int D_RB_COMMON_2 = 419;  //Common.
const int D_RB_mLF = 420;       //Minor limit/forcing raise.
const int D_RB_ACO = 421;       //After constructive overcall.
//Maximal overcall double related sequences.
const int D_RB_MO = 422;        //Rebid by opener and responder.
const int D_RB_MO_OFF = 423;    //Maximal overcall double off.

#endif // BIDDBDEFINES

