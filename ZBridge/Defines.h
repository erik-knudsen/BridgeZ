/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: Defines.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Global constant definitions.

  Platforms: Qt.
*/

/**
 * \file
 * Definition of global constants.
 */

#ifndef DEFINES_H
#define DEFINES_H

#include <QString>

//Port offset for automatic play.
const int AUTOPORTOFFSET = 10;

//Number of random deals in a random session.
const int NO_RANDOM_DEALS = 32;

//Update (enable/disable) UI (mostly menues) actions.
enum UpdateUIAction { UPDATE_UI_INITIAL, UPDATE_UI_SERVER, UPDATE_UI_CLIENT, UPDATE_UI_NEW_SESSION,
                    UPDATE_UI_NEW_DEAL, UPDATE_UI_UNDO, UPDATE_UI_REBID, UPDATE_UI_REPLAY, UPDATE_UI_SHOW_ALL,
                    UPDATE_UI_SCORE, UPDATE_UI_DELETE, UPDATE_UI_SAVE, UPDATE_UI_SAVEAS, UPDATE_UI_PAR,
                    UPDATE_UI_LAY_OUT_CARDS};

//Message type in network protocol.
enum MsgType { CONNECT_MSG, SEATED_MSG, RTNAMES_MSG, TEAMNAMES_MSG, RSBOARD_MSG, STARTOFBOARD_MSG, RDEALINFO_MSG,
               DEALINFO_MSG, RCARDS_MSG, CARDS_MSG, BID_MSG, RBID_MSG, ILLEGALBID_MSG, PLAYERTOLEAD_MSG,
               DUMMYTOLEAD_MSG, PLAYERPLAYS_MSG, READYFORPLAYER_MSG, READYFORDUMMY_MSG, ILLEGALCARD_MSG,
               READYFORDUMMYCARDS_MSG, DUMMYCARDS_MSG, ENDOFSESSION_MSG, UNKNOWN_MSG, UNDOBID_MSG, UNDOTRICK_MSG,
               REBID_MSG, REPLAY_MSG, ATTEMPT_SYNCHRONIZE_MSG, CONFIRM_SYNCHRONIZE_MSG, ALL_SYNCHRONIZED_MSG,
               ORIGINAL_PBN_START_MSG, PLAYED_PBN_START_MSG, PBN_LINE_MSG, ESCAPE_MSG};

//Deal and game types.
enum DealType { ORIGINAL_DEAL, RANDOM_DEAL };               /**< The two types of game sets. */
enum GameType { ORIGINAL_GAME, PLAYED_GAME, AUTO_GAME };    /**< The three types of games in a game set. */

//Honor card values
const int NINE = 7;
const int TEN =   8;
const int JACK =  9;
const int QUEEN = 10;
const int KING =  11;
const int ACE =   12;

//Seat names (indexed by enum Seat).
const char * const SEAT_NAMES[4] =
{
    QT_TRANSLATE_NOOP("defines", "West"),
    QT_TRANSLATE_NOOP("defines", "North"),
    QT_TRANSLATE_NOOP("defines", "East"),
    QT_TRANSLATE_NOOP("defines", "South")
};

//Auto seat names prefix.
const QString AUTO_SEAT_NAME_PREFIX = "AUTO_";

//Suit names (indexed by enum Suit).
const char * const SUIT_NAMES[5] =
{
    QT_TRANSLATE_NOOP("defines", "C"),
    QT_TRANSLATE_NOOP("defines", "D"),
    QT_TRANSLATE_NOOP("defines", "H"),
    QT_TRANSLATE_NOOP("defines", "S"),
    QT_TRANSLATE_NOOP("defines", "NT")
};

//Team names (indexed by enum Team).
const char * const TEAM_NAMES[4] =
{
    QT_TRANSLATE_NOOP("defines", "Neither"),
    QT_TRANSLATE_NOOP("defines", "North-South"),
    QT_TRANSLATE_NOOP("defines", "East-West"),
    QT_TRANSLATE_NOOP("defines", "All")
};

//Suit, position and vulnerability. MUST NOT be changed (used as index).
enum Suit { ANY=-1, CLUBS=0, DIAMONDS=1, HEARTS=2, SPADES=3, NOTRUMP=4 };
enum Position { LEFT_POS=0, TOP_POS=1, RIGHT_POS=2, BOTTOM_POS=3};
enum Team { NONE=-1, NEITHER=0, NORTH_SOUTH=1, EAST_WEST=2, BOTH=3 };

//Bids (these values MUST NOT be changed they are used as index in some places).
enum Bids {
    BID_NONE=-1,
    BID_PASS=0,
    BID_1C= 1, BID_1D= 2, BID_1H= 3, BID_1S= 4, BID_1NT= 5,
    BID_2C= 6, BID_2D= 7, BID_2H= 8, BID_2S= 9, BID_2NT=10,
    BID_3C=11, BID_3D=12, BID_3H=13, BID_3S=14, BID_3NT=15,
    BID_4C=16, BID_4D=17, BID_4H=18, BID_4S=19, BID_4NT=20,
    BID_5C=21, BID_5D=22, BID_5H=23, BID_5S=24, BID_5NT=25,
    BID_6C=26, BID_6D=27, BID_6H=28, BID_6S=29, BID_6NT=30,
    BID_7C=31, BID_7D=32, BID_7H=33, BID_7S=34, BID_7NT=35,
    BID_DOUBLE=36, BID_REDOUBLE=37, BID_PLAYER=38, BID_BLANK=39
};

//Card values (CLUBS, DIAMONDS, HEARTS, SPADES).
const int SUIT_INTERVAL[4][2] = {
    { 13, 25},
    {  0, 12},
    { 26, 38},
    { 39, 51}
                               };

//Card faces indexed by card values.
const QString CARD_FACES[52] = {
    ":/newPrefix/resources/15.png",
    ":/newPrefix/resources/16.png",
    ":/newPrefix/resources/17.png",
    ":/newPrefix/resources/18.png",
    ":/newPrefix/resources/19.png",
    ":/newPrefix/resources/20.png",
    ":/newPrefix/resources/21.png",
    ":/newPrefix/resources/22.png",
    ":/newPrefix/resources/23.png",
    ":/newPrefix/resources/24.png",
    ":/newPrefix/resources/25.png",
    ":/newPrefix/resources/26.png",
    ":/newPrefix/resources/14.png",

    ":/newPrefix/resources/2.png",
    ":/newPrefix/resources/3.png",
    ":/newPrefix/resources/4.png",
    ":/newPrefix/resources/5.png",
    ":/newPrefix/resources/6.png",
    ":/newPrefix/resources/7.png",
    ":/newPrefix/resources/8.png",
    ":/newPrefix/resources/9.png",
    ":/newPrefix/resources/10.png",
    ":/newPrefix/resources/11.png",
    ":/newPrefix/resources/12.png",
    ":/newPrefix/resources/13.png",
    ":/newPrefix/resources/1.png",

    ":/newPrefix/resources/28.png",
    ":/newPrefix/resources/29.png",
    ":/newPrefix/resources/30.png",
    ":/newPrefix/resources/31.png",
    ":/newPrefix/resources/32.png",
    ":/newPrefix/resources/33.png",
    ":/newPrefix/resources/34.png",
    ":/newPrefix/resources/35.png",
    ":/newPrefix/resources/36.png",
    ":/newPrefix/resources/37.png",
    ":/newPrefix/resources/38.png",
    ":/newPrefix/resources/39.png",
    ":/newPrefix/resources/27.png",

    ":/newPrefix/resources/41.png",
    ":/newPrefix/resources/42.png",
    ":/newPrefix/resources/43.png",
    ":/newPrefix/resources/44.png",
    ":/newPrefix/resources/45.png",
    ":/newPrefix/resources/46.png",
    ":/newPrefix/resources/47.png",
    ":/newPrefix/resources/48.png",
    ":/newPrefix/resources/49.png",
    ":/newPrefix/resources/50.png",
    ":/newPrefix/resources/51.png",
    ":/newPrefix/resources/52.png",
    ":/newPrefix/resources/40.png"
};

//Bid icons.
const QString BIDS[41] = {
    ":/newPrefix/resources/ico_pass.ico",
    ":/newPrefix/resources/ico_1c.ico",
    ":/newPrefix/resources/ico_1d.ico",
    ":/newPrefix/resources/ico_1h.ico",
    ":/newPrefix/resources/ico_1s.ico",
    ":/newPrefix/resources/ico_1nt.ico",
    ":/newPrefix/resources/ico_2c.ico",
    ":/newPrefix/resources/ico_2d.ico",
    ":/newPrefix/resources/ico_2h.ico",
    ":/newPrefix/resources/ico_2s.ico",
    ":/newPrefix/resources/ico_2nt.ico",
    ":/newPrefix/resources/ico_3c.ico",
    ":/newPrefix/resources/ico_3d.ico",
    ":/newPrefix/resources/ico_3h.ico",
    ":/newPrefix/resources/ico_3s.ico",
    ":/newPrefix/resources/ico_3nt.ico",
    ":/newPrefix/resources/ico_4c.ico",
    ":/newPrefix/resources/ico_4d.ico",
    ":/newPrefix/resources/ico_4h.ico",
    ":/newPrefix/resources/ico_4s.ico",
    ":/newPrefix/resources/ico_4nt.ico",
    ":/newPrefix/resources/ico_5c.ico",
    ":/newPrefix/resources/ico_5d.ico",
    ":/newPrefix/resources/ico_5h.ico",
    ":/newPrefix/resources/ico_5s.ico",
    ":/newPrefix/resources/ico_5nt.ico",
    ":/newPrefix/resources/ico_6c.ico",
    ":/newPrefix/resources/ico_6d.ico",
    ":/newPrefix/resources/ico_6h.ico",
    ":/newPrefix/resources/ico_6s.ico",
    ":/newPrefix/resources/ico_6nt.ico",
    ":/newPrefix/resources/ico_7c.ico",
    ":/newPrefix/resources/ico_7d.ico",
    ":/newPrefix/resources/ico_7h.ico",
    ":/newPrefix/resources/ico_7s.ico",
    ":/newPrefix/resources/ico_7nt.ico",
    ":/newPrefix/resources/ico_double.ico",
    ":/newPrefix/resources/ico_redouble.ico",
    ":/newPrefix/resources/PlayerBidPrompt.ico",
    ":/newPrefix/resources/BlankBidPrompt.ico"
};

//Sync states (must be the same as in the Yakindu client synchronization statechart).
//These values are used to determine whether there should be a pause and if so, which kind
//of pause (show a button etc.).
const int SS = 1;       //Start of Board synchronization (before new deal starts).
const int SA = 2;       //Start of Auction synchronization (before auction starts).
const int SP = 3;       //Start of Play synchronization (before play starts).
const int SL = 4;       //Start of Leader synchronization (before next trick starts).

//Button ids.
const int BUTTON_NONE = 0;
const int BUTTON_AUCTION = SA;
const int BUTTON_PLAY = SP;
const int BUTTON_LEADER = SL;
const int BUTTON_DEAL = SS;
const int BUTTON_DD = 10;

//UNDO values (must be the same as in the Yakindu synchronization statecharts.
//These values are used as undo values to determine the undo type (rebid, replay, complete trick or partial trick).
const int REBID = -1;
const int REPLAY = -2;
const int CT = 1;
const int PT = 2;

//------------------------- Seat options. ---------------------------
enum Seat { NO_SEAT=-1, WEST_SEAT=0, NORTH_SEAT=1, EAST_SEAT=2, SOUTH_SEAT=3};
enum Actor { MANUAL_ACTOR = 0, AUTO_ACTOR = 1, REMOTE_ACTOR = 2 };
enum Role { SERVER_ROLE = 0, CLIENT_ROLE = 1,STANDALONE_ROLE = 2 };
enum Protocol { BASIC_PROTOCOL = 18, ADVANCED_PROTOCOL = 20 };

//-------------------------- Bid/Play Options --------------------
//Bid configuration file name.
const QString BIDOPTIONFILENAME = "BidOption.bid";

const int NSBIDOPTIONSINDEX = 0;
const int EWBIDOPTIONSINDEX = 1;
const int MAXOPENINGLEADS = 20;

const int NSOPENINGLEADSUIT = 0;
const int EWOPENINGLEADSUIT = 1;
const int NSOPENINGLEADNT = 2;
const int EWOPENINGLEADNT = 3;

//Bid modes.
const int AMERICAN_STYLE = 0;
const int EUROPEAN_STYLE = 1;

//Major modes.
const int MAJOR_FOUR = 0;
const int MAJOR_FIVE = 1;

//Stayman types.
const int STAYMAN_OFF = 0;
const int STAYMAN_3R_F = 1;
const int STAYMAN_3R_NF = 2;
const int STAYMAN_4R = 3;

//2-Bid modes.
const int S2_NATURAL = 0;
const int W2_NATURAL = 1;
const int W2_OGUST = 2;
const int W2_FEATURE = 3;

//Weak two new suit.
const int W2_NEW_SUIT_NF = 0;
const int W2_NEW_SUIT_F = 1;

//2C Open values.
const int OPEN_VALUE_20 = 20;
const int OPEN_VALUE_21 = 21;
const int OPEN_VALUE_22 = 22;
const int OPEN_VALUE_23 = 23;

//After strong two club.
const int TWO_C_TWO_D_NEGATIVE = 0;
const int TWO_C_TWO_D_WAITING = 1;

//NT Range.
const int ONE_NT_RANGE_12_14 = 0;
const int ONE_NT_RANGE_15_17 = 1;
const int ONE_NT_RANGE_16_18 = 2;

const int TWO_NT_RANGE_20_21 = 0;
const int TWO_NT_RANGE_21_22 = 1;
const int TWO_NT_RANGE_22_24 = 2;

const int THREE_NT_RANGE_25_27 = 0;
const int GAMBLING_3_NT = 1;
const int GAMBLING_3_NT_ACOL = 2;

//2 NT after 1/1.
const int TWO_NT_11_18_20 = 0;
const int TWO_NT_11_17_18 = 1;

//2 NT after 2/1.
const int TWO_NT_21_15_18 = 0;
const int TWO_NT_21_12_14 = 1;

//After 1NT rebid.
const int REBID_1NT_NAT = 0;
const int REBID_1NT_F = 1;

//3 minor after 1NT.
const int ONE_NT_3m_SI = 0;
const int ONE_NT_3m_GI = 1;
const int ONE_NT_3m_PE = 2;

//3 major after 1NT.
const int ONE_NT_3M_SI = 0;
const int ONE_NT_3M_GF = 1;
const int ONE_NT_3M_GI = 2;
const int ONE_NT_3M_PE = 3;

//Jump overcall types.
const int JUMP_OVERCALL_STRONG = 0;
const int JUMP_OVERCALL_WEAK_NATURAL = 1;
const int JUMP_OVERCALL_WEAK_OGUST = 2;
const int JUMP_OVERCALL_WEAK_FEATURE = 3;

//Direct cuebid.
const int MICHAELS_CUEBID = 0;
const int STRONG_CUE_BID = 1;

//Three level vulnerability.
const int SOUND_THREE_LEVEL = 0;
const int LIGHT_THREE_LEVEL = 1;
const int LIGHT_THREE_LEVEL_NV = 2;

//Signals.
const int SIGNAL_NO_SIGNAL = 0;
const int SIGNAL_COUNT_HIGH = 1;
const int SIGNAL_COUNT_LOW = 2;
const int SIGNAL_ATTITUDE_HIGH = 3;
const int SIGNAL_ATTITUDE_LOW = 4;

//Leads.
const int FOURTH_BEST = 0;
const int THIRDBEST = 1;
const int LOW_ENCOURAGING = 2;

const int NS_SUIT = 0;
const int EW_SUIT = 1;
const int NS_NT = 2;
const int EW_NT = 3;

const int XX_N_S = 0;
const int XXX_N_S = 1;
const int AKX_N_S = 2;
const int KQX_N_S = 3;
const int QJX_N_S = 4;
const int JTX_N_S = 5;
const int KQTN_N_S = 6;
const int XXXX_N_S = 7;
const int XXXXX_N_S = 8;
const int TNX_N_S = 9;
const int KJTX_N_S = 10;
const int KTNX_N_S = 11;
const int QTNX_N_S = 12;

const int XX_E_S = 0;
const int XXX_E_S = 1;
const int AKX_E_S = 2;
const int KQX_E_S = 3;
const int QJX_E_S = 4;
const int JTX_E_S = 5;
const int KQTN_E_S = 6;
const int XXXX_E_S = 7;
const int XXXXX_E_S = 8;
const int TNX_E_S = 9;
const int KJTX_E_S = 10;
const int KTNX_E_S = 11;
const int QTNX_E_S = 12;

const int XX_N_NT = 0;
const int XXX_N_NT = 1;
const int AKJX_N_NT = 2;
const int AJTN_N_NT = 3;
const int KQJX_N_NT = 4;
const int QKTX_N_NT = 5;
const int JTNX_N_NT = 6;
const int XXXX_N_NT = 7;
const int XXXXX_N_NT = 8;
const int AQJX_N_NT = 9;
const int ATNX_N_NT = 10;
const int KQTN_N_NT = 11;
const int QTNX_N_NT = 12;
const int TNXX_N_NT = 13;

const int XX_E_NT = 0;
const int XXX_E_NT = 1;
const int AKJX_E_NT = 2;
const int AJTN_E_NT = 3;
const int KQJX_E_NT = 4;
const int QKTX_E_NT = 5;
const int JTNX_E_NT = 6;
const int XXXX_E_NT = 7;
const int XXXXX_E_NT = 8;
const int AQJX_E_NT = 9;
const int ATNX_E_NT = 10;
const int KQTN_E_NT = 11;
const int QTNX_E_NT = 12;
const int TNXX_E_NT = 13;

const int xX_N_S = 0;
const int Xx_N_S = 1;
const int xXX_N_S = 0;
const int XxX_N_S = 1;
const int XXx_N_S = 2;
const int aKX_N_S = 0;
const int AkX_N_S = 1;
const int AKx_N_S = 2;
const int kQX_N_S = 0;
const int KqX_N_S = 1;
const int KQx_N_S = 2;
const int qJX_N_S = 0;
const int QjX_N_S = 1;
const int QJx_N_S = 2;
const int jTX_N_S = 0;
const int JtX_N_S = 1;
const int JTx_N_S = 2;
const int kQTN_N_S = 0;
const int KqTN_N_S = 1;
const int KQtN_N_S = 2;
const int KQTn_N_S = 3;
const int xXXX_N_S = 0;
const int XxXX_N_S = 1;
const int XXxX_N_S = 2;
const int XXXx_N_S = 3;
const int xXXXX_N_S = 0;
const int XxXXX_N_S = 1;
const int XXxXX_N_S = 2;
const int XXXxX_N_S = 3;
const int XXXXx_N_S = 4;
const int tNX_N_S = 0;
const int TnX_N_S = 1;
const int TNx_N_S = 2;
const int kJTX_N_S = 0;
const int KjTX_N_S = 1;
const int KJtX_N_S = 2;
const int KJTx_N_S = 0;
const int kTNX_N_S = 0;
const int KtNX_N_S = 1;
const int KTnX_N_S = 2;
const int KTNx_N_S = 3;
const int qTNX_N_S = 0;
const int QtNX_N_S = 1;
const int QTnX_N_S = 2;
const int QTNx_N_S = 3;

const int xX_E_S = 0;
const int Xx_E_S = 1;
const int xXX_E_S = 0;
const int XxX_E_S = 1;
const int XXx_E_S = 2;
const int aKX_E_S = 0;
const int AkX_E_S = 1;
const int AKx_E_S = 2;
const int kQX_E_S = 0;
const int KqX_E_S = 1;
const int KQx_E_S = 2;
const int qJX_E_S = 0;
const int QjX_E_S = 1;
const int QJx_E_S = 2;
const int jTX_E_S = 0;
const int JtX_E_S = 1;
const int JTx_E_S = 2;
const int kQTN_E_S = 0;
const int KqTN_E_S = 1;
const int KQtN_E_S = 2;
const int KQTn_E_S = 3;
const int xXXX_E_S = 0;
const int XxXX_E_S = 1;
const int XXxX_E_S = 2;
const int XXXx_E_S = 3;
const int xXXXX_E_S = 0;
const int XxXXX_E_S = 1;
const int XXxXX_E_S = 2;
const int XXXxX_E_S = 3;
const int XXXXx_E_S = 4;
const int tNX_E_S = 0;
const int TnX_E_S = 1;
const int TNx_E_S = 2;
const int kJTX_E_S = 0;
const int KjTX_E_S = 1;
const int KJtX_E_S = 2;
const int KJTx_E_S = 0;
const int kTNX_E_S = 0;
const int KtNX_E_S = 1;
const int KTnX_E_S = 2;
const int KTNx_E_S = 3;
const int qTNX_E_S = 0;
const int QtNX_E_S = 1;
const int QTnX_E_S = 2;
const int QTNx_E_S = 3;

const int xX_N_NT = 0;
const int Xx_N_NT = 1;
const int xXX_N_NT = 0;
const int XxX_N_NT = 1;
const int XXx_N_NT = 2;
const int aKJX_N_NT = 0;
const int AkJX_N_NT = 1;
const int AKjX_N_NT = 2;
const int AKJx_N_NT = 3;
const int aJTN_N_NT = 0;
const int AjTN_N_NT = 1;
const int AJtN_N_NT = 2;
const int AJTn_N_NT = 3;
const int kQJX_N_NT = 0;
const int KqJX_N_NT = 1;
const int KQjX_N_NT = 2;
const int KQJx_N_NT = 3;
const int qKTX_N_NT = 0;
const int QkTX_N_NT = 1;
const int QKtX_N_NT = 2;
const int QKTx_N_NT = 3;
const int jTNX_N_NT = 0;
const int JtNX_N_NT = 1;
const int JTnX_N_NT = 2;
const int JTNx_N_NT = 3;
const int xXXX_N_NT = 0;
const int XxXX_N_NT = 1;
const int XXxX_N_NT = 2;
const int XXXx_N_NT = 3;
const int xXXXX_N_NT = 0;
const int XxXXX_N_NT = 1;
const int XXxXX_N_NT = 2;
const int XXXxX_N_NT = 3;
const int XXXXx_N_NT = 4;
const int aQJX_N_NT = 0;
const int AqJX_N_NT = 1;
const int AQjX_N_NT = 2;
const int AQJx_N_NT = 3;
const int aTNX_N_NT = 0;
const int AtNX_N_NT = 1;
const int ATnX_N_NT = 2;
const int ATNx_N_NT = 3;
const int kQTN_N_NT = 0;
const int KqTN_N_NT = 1;
const int KQtN_N_NT = 2;
const int KQTn_N_NT = 3;
const int qTNX_N_NT = 0;
const int QtNX_N_NT = 1;
const int QTnX_N_NT = 2;
const int QTNx_N_NT = 3;
const int tNXX_N_NT = 0;
const int TnXX_N_NT = 1;
const int TNxX_N_NT = 2;
const int TNXx_N_NT = 3;

const int xX_E_NT = 0;
const int Xx_E_NT = 01;
const int xXX_E_NT = 0;
const int XxX_E_NT = 1;
const int XXx_E_NT = 2;
const int aKJX_E_NT = 0;
const int AkJX_E_NT = 1;
const int AKjX_E_NT = 2;
const int AKJx_E_NT = 3;
const int aJTN_E_NT = 0;
const int AjTN_E_NT = 1;
const int AJtN_E_NT = 2;
const int AJTn_E_NT = 3;
const int kQJX_E_NT = 0;
const int KqJX_E_NT = 1;
const int KQjX_E_NT = 2;
const int KQJx_E_NT = 3;
const int qKTX_E_NT = 0;
const int QkTX_E_NT = 1;
const int QKtX_E_NT = 2;
const int QKTx_E_NT = 3;
const int jTNX_E_NT = 0;
const int JtNX_E_NT = 1;
const int JTnX_E_NT = 2;
const int JTNx_E_NT = 3;
const int xXXX_E_NT = 0;
const int XxXX_E_NT = 1;
const int XXxX_E_NT = 2;
const int XXXx_E_NT = 3;
const int xXXXX_E_NT = 0;
const int XxXXX_E_NT = 1;
const int XXxXX_E_NT = 2;
const int XXXxX_E_NT = 3;
const int XXXXx_E_NT = 4;
const int aQJX_E_NT = 0;
const int AqJX_E_NT = 1;
const int AQjX_E_NT = 2;
const int AQJx_E_NT = 3;
const int aTNX_E_NT = 0;
const int AtNX_E_NT = 1;
const int ATnX_E_NT = 2;
const int ATNx_E_NT = 3;
const int kQTN_E_NT = 0;
const int KqTN_E_NT = 1;
const int KQtN_E_NT = 2;
const int KQTn_E_NT = 3;
const int qTNX_E_NT = 0;
const int QtNX_E_NT = 1;
const int QTnX_E_NT = 2;
const int QTNx_E_NT = 3;
const int tNXX_E_NT = 0;
const int TnXX_E_NT = 1;
const int TNxX_E_NT = 2;
const int TNXx_E_NT = 3;

//---------------------------- Deal Options -------------------
const int MAX_SUIT_HCP = 10;
const int MAX_TOT_HCP = 37;
const int MAX_TOT_DP = 9;
const int MAX_SUIT_SIZE = 13;

//------------------------- Display options. ------------------------
//Suits.
const int SHCD_SUIT_SEQUENCE = 0;
const int SHDC_SUIT_SEQUENCE = 1;
const int DCHS_SUIT_SEQUENCE = 2;
const int CDHS_SUIT_SEQUENCE = 3;
const int HSDC_SUIT_SEQUENCE = 4;

//Cards.
const int DEFAULT_ANIMATION_GRANULARITY = 9;

//Card Backs.
const int CARD_BACK_0 = 0;
const int CARD_BACK_1 = 1;
const int CARD_BACK_2 = 2;
const int CARD_BACK_3 = 3;
const int CARD_BACK_4 = 4;
const int CARD_BACK_5 = 5;
const int CARD_BACK_6 = 6;
//Card back names indexed by above index.
const QString CARD_BACK_NAME[6] = { ":/newPrefix/resources/cardback1.png",
                                    ":/newPrefix/resources/cardback22.png",
                                    ":/newPrefix/resources/cardback3.png",
                                    ":/newPrefix/resources/cardback5.png",
                                    ":/newPrefix/resources/cardback11.png",
                                    ":/newPrefix/resources/cardback2.png"
                                   };
//Misc.
const int TILE_BITMAP_MODE = 0;
const int CENTER_BITMAP_MODE = 1;

//----------------------------- Game Options. ------------------------
//Interface.
const int LEVEL_1_TRACE_LEVEL = 0;
const int LEVEL_2_TRACE_LEVEL = 1;
const int LEVEL_3_TRACE_LEVEL = 2;
const int LEVEL_4_TRACE_LEVEL = 3;
const int LEVEL_5_TRACE_LEVEL = 4;

//Counting.
const int GOREN_VALUATION_MODE = 0;
const int GINSBURG_VALUATION_MODE = 1;
const int CUSTOM_VALUATION_MODE = 3;

//Scoring.
enum Forcing { NON_FORCING=0, FORCING=1, GAME_FORCING=2, MUST_PASS=3};
enum Vulnerability { VUL_II=0, VUL_NI=1, VUL_YI=2, VUL_NN=3, VUL_NY=4, VUL_YN=5, VUL_YY=6 };
enum ScoringMethod { IMP=0, MP=1, RUBBER=2, PRACTICE=3, NOSCORE=4, FORSCORE=5};

//----------------------------- Wizard. -------------------------------
//Help Level.
const int BEGIN_HELP_LEVEL = 0;
const int BASIC_HELP_LEVEL = 1;
const int MINIMUM_HELP_LEVEL = 2;

//Bidding Style.
const int NATURAL_BIDDING_STYLE = 0;
const int GOREN_BIDDING_STYLE = 1;
const int STANDARD_AMERICAN_BASIC_BIDDING_STYLE = 2;
const int STANDARD_AMERICAN_YELLOW_CARD_BIDDING_STYLE = 3;
const int ACOL_BIDDING_STYLE = 4;

//------------------------- Play View definitions ----------------------
const int CARD_VER_SIZE = 96;
const int TB_CARD_VER_SIZE = CARD_VER_SIZE + 2;
const int LR_CARD_VER_SIZE = 4 * TB_CARD_VER_SIZE;
const int CENTER_VER_SIZE = LR_CARD_VER_SIZE;
const int INF_VER_SIZE = CENTER_VER_SIZE;
const int SCENE_VER_SIZE = CENTER_VER_SIZE + TB_CARD_VER_SIZE + TB_CARD_VER_SIZE;

const int CARD_HOR_SIZE = 71;
const int CENTER_HOR_SIZE = 290;
const int LR_CARD_HOR_SIZE = (CENTER_VER_SIZE + 4 * TB_CARD_VER_SIZE  - CENTER_HOR_SIZE) >> 1;
const int TB_CARD_HOR_SIZE = CENTER_HOR_SIZE + LR_CARD_HOR_SIZE + LR_CARD_HOR_SIZE - TB_CARD_VER_SIZE;
const int INF_HOR_SIZE = 180;
const int SCENE_HOR_SIZE = CENTER_HOR_SIZE + LR_CARD_HOR_SIZE + LR_CARD_HOR_SIZE + INF_HOR_SIZE;

const int BID_HOR_SIZE = 18;
const int BID_VER_SIZE = 12;




// Shortcuts (Macros)
#define pMAINFRAME		 CMainframe::Instance()
#define pDOC			 CZBridgeDoc::Instance()

#define	ABS(x)			 ((x>=0)?x:-x)

//Suit is CLUBS or DIAMONDS or Hearts ot SPADES. Face in suit ranges from 0 to 12 (inclusive), where
//0 corresponds to 2 in the suit and 12 corresponds to ace in the suit.
#define IS_CLUBS(card)          ((SUIT_INTERVAL[CLUBS][0] <= card) && (card <= SUIT_INTERVAL[CLUBS][1]))
#define IS_DIAMONDS(card)       ((SUIT_INTERVAL[DIAMONDS][0] <= card) && (card <= SUIT_INTERVAL[DIAMONDS][1]))
#define IS_HEARTS(card)         ((SUIT_INTERVAL[HEARTS][0] <= card) && (card <= SUIT_INTERVAL[HEARTS][1]))
#define IS_SPADES(card)         ((SUIT_INTERVAL[SPADES][0] <= card) && (card <= SUIT_INTERVAL[SPADES][1]))
#define	MAKE_CARD(suit, face)   ((suit >= CLUBS) && (suit <= SPADES) && (face >= 0) && (face <= 12)) ?\
                                 (SUIT_INTERVAL[suit][0] + face) : -1
#define	CARD_SUIT(card)         (IS_CLUBS(card) ? CLUBS :\
                                 IS_DIAMONDS(card) ? DIAMONDS :\
                                  IS_HEARTS(card) ? HEARTS :\
                                   IS_SPADES(card) ? SPADES : ANY)
#define	CARD_FACE(card)         (IS_CLUBS(card) ? (card - SUIT_INTERVAL[CLUBS][0]) :\
                                 IS_DIAMONDS(card) ? (card - SUIT_INTERVAL[DIAMONDS][0]) :\
                                  IS_HEARTS(card) ? (card - SUIT_INTERVAL[HEARTS][0]) :\
                                   IS_SPADES(card) ? (card - SUIT_INTERVAL[SPADES][0]) : -1)

#define ISMAJOR(suit)	(((suit == HEARTS) || (suit == SPADES)) ? true : false)
#define ISMINOR(suit)	(((suit == CLUBS) || (suit == DIAMONDS)) ? true : false)
#define ISNOTRUMP(suit)((suit == NOTRUMP) ? true : false)

#define MAKE_BID(suit, level) ((suit > ANY) && (suit <= NOTRUMP) && (level >= 1) && (level <= 7)) ?\
                               (Bids)(BID_PASS + (level-1)*5 + suit + 1) : BID_NONE
#define BID_SUIT(bid)  (Suit)(((bid <= BID_PASS) || (bid >= BID_DOUBLE)) ? -1 : (((bid-1) % 5)))
#define BID_LEVEL(bid) ( ((bid <= BID_PASS) || (bid >= BID_DOUBLE)) ? -1 : (((bid-1) / 5) + 1) )

#define IS_BID(bid) ((bid >= BID_1C) && (bid <= BID_7NT))
#define IS_DOUBLE_BID(bid) (bid == BID_DOUBLE)
#define IS_REDOUBLE_BID(bid) (bid == BID_REDOUBLE)
#define IS_SUIT(suit) ((suit >= CLUBS) && (suit <= SPADES))
#define IS_CARD(card) ((card >= 0) && (card <= 51))
#define IS_FACE(face) ((face >= 0) && (face <= ACE))
#define IS_POSITION(position) ((position >= LEFT_POS) && (position <= BOTTOM_POS))
#define IS_PLAYER(seat) ((seat >= WEST_SEAT) && (seat <= SOUTH_SEAT))
#define IS_TEAM(team) ((team == NORTH_SOUTH) || (team == EAST_WEST))

#define OTHER_MAJOR(suit) 	((suit == HEARTS) ? SPADES : HEARTS)
#define OTHER_MINOR(suit) 	((suit == CLUBS) ? DIAMONDS : CLUBS)

#define MIN(a,b) ( ((a) <= (b))? (a): (b) )
#define MAX(a,b) ( ((a) >= (b))? (a): (b) )


#include <QEvent>

#define WMS_UPDATE_TEXT			QEvent::Type(106 + QEvent::User)
class WMS_UPDATE_TEXT_Event : public QEvent
{
public:
    explicit WMS_UPDATE_TEXT_Event(bool get) : QEvent(WMS_UPDATE_TEXT)
    {   this->get = get; }

    bool getget()
    { return get; }

private:
     bool get;
};

#define WMS_CARD_CLICKED			QEvent::Type(107 + QEvent::User)
class CARD_CLICKED_Event : public QEvent
{
public:
    explicit CARD_CLICKED_Event(Position pos, int cardValue) : QEvent(WMS_CARD_CLICKED)
    {
        this->pos = pos;
        this->cardValue = cardValue;
    }

    Position getPos() { return pos; }
    int getCardValue() { return cardValue; }

private:
    Position pos;
    int cardValue;
};

#define WMS_HAND_CLICKED			QEvent::Type(108 + QEvent::User)
class HAND_CLICKED_Event : public QEvent
{
public:
    explicit HAND_CLICKED_Event(Position pos) : QEvent(WMS_CARD_CLICKED)
    {
       this->pos = pos;
    }

    Position getPos() { return pos; }

private:
    Position pos;
};

#define WMS_BUTTON_CLICKED          QEvent::Type(109 + QEvent::User)
class BUTTON_CLICKED_Event : public QEvent
{
public:
    explicit BUTTON_CLICKED_Event(int button) : QEvent(WMS_BUTTON_CLICKED)
    { this->button = button;}

    int getButton() { return button; }

private:
    int button;
};

#define WMS_BID_CLICKED			QEvent::Type(110 + QEvent::User)
class BID_CLICKED_Event : public QEvent
{
public:
    explicit BID_CLICKED_Event(Seat seat, Bids bid) : QEvent(WMS_BID_CLICKED)
    { this->bid = bid; }

    Seat getSeat() { return seat; }
    Bids getBid() { return bid; }

private:
    Seat seat;
    Bids bid;
};

#define WMS_UPDATE_UI_ACTION			QEvent::Type(111 + QEvent::User)
class UPDATE_UI_ACTION_Event : public QEvent
{
public:
    explicit UPDATE_UI_ACTION_Event(UpdateUIAction uiAction, bool enable = true) : QEvent(WMS_UPDATE_UI_ACTION)
    { this->uiAction = uiAction; this->enable = enable; }

    UpdateUIAction getUIAction() { return uiAction; }
    bool getEnable() { return enable; }

private:
    UpdateUIAction uiAction;
    bool enable;
};

#endif // DEFINES_H
