
#include <stdlib.h>
#include <string.h>
#include "sc_types.h"
#include "ZBridgeClient.h"
/*! \file Implementation of the state machine 'ZBridgeClient'
*/

/* prototypes of all internal functions */
static sc_boolean zBridgeClient_check_main_region_Connecting_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Connecting_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Seated_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_StartOfBoard_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Info_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Cards_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Bid_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Bid_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Bid_tr2_tr2(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Bid_tr3_tr3(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Bid_tr4_tr4(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Lead_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Lead_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_DummyCards_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Play_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Play_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Play_tr2_tr2(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Play_tr3_tr3(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Play_tr4_tr4(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_Play_tr5_tr5(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_WaitLeader_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncSB_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncSB_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncLeader_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncLeader_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncLeader_tr2_tr2(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncLeader_tr3_tr3(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncLeader_tr4_tr4(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncLeader_tr5_tr5(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncAuction_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncAuction_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncAuction_tr2_tr2(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncPlay_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncPlay_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncPlay_tr2_tr2(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region_SyncReplay_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_0_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_0_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_1_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_1_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_2_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_2_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_2_tr2_tr2(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_3_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_3_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_4_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_4_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_5_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_5_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_6_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_6_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_7_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_7_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_8_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_9_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_9_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_10_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_10_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_11_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_11_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_12_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_12_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_13_tr0_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_13_tr1_tr1(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_14_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_15_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_16_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_17_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_18_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_19_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_20_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_21_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_22_tr0(const ZBridgeClient* handle);
static sc_boolean zBridgeClient_check_main_region__choice_23_tr0(const ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Connecting_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Connecting_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Seated_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_StartOfBoard_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Info_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Cards_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Bid_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Bid_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Bid_tr2(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Bid_tr3(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Bid_tr4(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Lead_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Lead_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_DummyCards_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Play_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Play_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Play_tr2(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Play_tr3(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Play_tr4(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_Play_tr5(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_WaitLeader_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncSB_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncSB_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncLeader_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncLeader_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncLeader_tr2(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncLeader_tr3(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncLeader_tr4(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncLeader_tr5(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncAuction_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncAuction_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncAuction_tr2(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncPlay_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncPlay_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncPlay_tr2(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region_SyncReplay_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_0_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_0_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_1_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_1_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_2_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_2_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_2_tr2(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_3_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_3_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_4_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_4_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_5_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_5_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_6_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_6_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_7_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_7_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_8_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_9_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_9_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_10_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_10_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_11_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_11_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_12_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_12_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_13_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_13_tr1(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_14_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_15_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_16_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_17_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_18_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_19_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_20_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_21_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_22_tr0(ZBridgeClient* handle);
static void zBridgeClient_effect_main_region__choice_23_tr0(ZBridgeClient* handle);
static void zBridgeClient_enact_main_region_Connecting(ZBridgeClient* handle);
static void zBridgeClient_enact_main_region_SyncSB(ZBridgeClient* handle);
static void zBridgeClient_enact_main_region_SyncLeader(ZBridgeClient* handle);
static void zBridgeClient_enact_main_region_SyncAuction(ZBridgeClient* handle);
static void zBridgeClient_enact_main_region_SyncPlay(ZBridgeClient* handle);
static void zBridgeClient_enact_main_region_SyncReplay(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_Connecting_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region__final__default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_Seated_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_StartOfBoard_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_Info_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_Cards_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_Bid_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_Lead_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_DummyCards_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_Play_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_WaitLeader_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_SyncSB_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_SyncLeader_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_SyncAuction_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_SyncPlay_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_SyncReplay_default(ZBridgeClient* handle);
static void zBridgeClient_enseq_main_region_default(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_Connecting(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region__final_(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_Seated(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_StartOfBoard(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_Info(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_Cards(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_Bid(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_Lead(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_DummyCards(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_Play(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_WaitLeader(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_SyncSB(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_SyncLeader(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_SyncAuction(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_SyncPlay(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region_SyncReplay(ZBridgeClient* handle);
static void zBridgeClient_exseq_main_region(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Connecting(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__final_(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Seated(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_StartOfBoard(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Info(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Cards(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Bid(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Lead(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_DummyCards(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Play(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_WaitLeader(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_SyncSB(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_SyncLeader(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_SyncAuction(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_SyncPlay(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_SyncReplay(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_0(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_1(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_2(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_3(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_4(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_5(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_6(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_7(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_8(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_9(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_10(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_11(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_12(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_13(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_14(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_15(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_16(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_17(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_18(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_19(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_20(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_21(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_22(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__choice_23(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__entry_Default(ZBridgeClient* handle);
static void zBridgeClient_clearInEvents(ZBridgeClient* handle);
static void zBridgeClient_clearOutEvents(ZBridgeClient* handle);


void zBridgeClient_init(ZBridgeClient* handle)
{
	sc_integer i;

	for (i = 0; i < ZBRIDGECLIENT_MAX_ORTHOGONAL_STATES; ++i)
	{
		handle->stateConfVector[i] = ZBridgeClient_last_state;
	}
	
	
	handle->stateConfVectorPosition = 0;

	zBridgeClient_clearInEvents(handle);
	zBridgeClient_clearOutEvents(handle);

	/* Default init sequence for statechart ZBridgeClient */
	handle->internal.SS = 1;
	handle->internal.SA = 2;
	handle->internal.SP = 3;
	handle->internal.SL = 4;
	handle->internal.SR = 5;
	handle->internal.BID_NONE = -1;
	handle->internal.BID_PASS = 0;
	handle->internal.BID_DOUBLE = 36;
	handle->internal.BID_REDOUBLE = 37;
	handle->internal.REBID = -1;
	handle->internal.REPLAY = -2;
	handle->internal.CT = 1;
	handle->internal.PT = 2;
	handle->internal.curBidder = 0;
	handle->internal.firstBidRound = bool_false;
	handle->internal.noPasses = 0;
	handle->internal.lastBidder = 0;
	handle->internal.playNo = 0;
	handle->internal.firstTrick = bool_false;
	handle->iface.boardNumber = 0;
	handle->iface.vulnerability = 0;
	handle->iface.client = 0;
	handle->iface.dealer = 0;
	handle->iface.bidder = 0;
	handle->iface.bidVal = 0;
	handle->iface.lastBid = 0;
	handle->iface.bidDouble = 0;
	handle->iface.bidEnable = 0;
	handle->iface.declarer = 0;
	handle->iface.dummy = 0;
	handle->iface.leader = 0;
	handle->iface.noTrick = 0;
	handle->iface.player = 0;
	handle->iface.cardVal = 0;
	handle->iface.syncState = 0;

}

void zBridgeClient_enter(ZBridgeClient* handle)
{
	/* Default enter sequence for statechart ZBridgeClient */
	zBridgeClient_enseq_main_region_default(handle);
}

void zBridgeClient_exit(ZBridgeClient* handle)
{
	/* Default exit sequence for statechart ZBridgeClient */
	zBridgeClient_exseq_main_region(handle);
}

sc_boolean zBridgeClient_isActive(const ZBridgeClient* handle)
{
	sc_boolean result;
	if (handle->stateConfVector[0] != ZBridgeClient_last_state)
	{
		result =  bool_true;
	}
	else
	{
		result = bool_false;
	}
	return result;
}

sc_boolean zBridgeClient_isFinal(const ZBridgeClient* handle)
{
	return (handle->stateConfVector[0] == ZBridgeClient_main_region__final_);

}

static void zBridgeClient_clearInEvents(ZBridgeClient* handle)
{
	handle->iface.endOfSession_raised = bool_false;
	handle->iface.connectError_raised = bool_false;
	handle->iface.seated_raised = bool_false;
	handle->iface.teamNames_raised = bool_false;
	handle->iface.startOfBoard_raised = bool_false;
	handle->iface.dealInfo_raised = bool_false;
	handle->iface.cards_raised = bool_false;
	handle->iface.undo_raised = bool_false;
	handle->iface.bidDone_raised = bool_false;
	handle->iface.playerPlays_raised = bool_false;
	handle->iface.playerToLead_raised = bool_false;
	handle->iface.dummyToLead_raised = bool_false;
	handle->iface.dummyCards_raised = bool_false;
	handle->iface.newLeader_raised = bool_false;
	handle->iface.reStart_raised = bool_false;
	handle->iface.allSync_raised = bool_false;
}

static void zBridgeClient_clearOutEvents(ZBridgeClient* handle)
{
	handle->iface.connect_raised = bool_false;
	handle->iface.rTNames_raised = bool_false;
	handle->iface.rSBoard_raised = bool_false;
	handle->iface.rDealInfo_raised = bool_false;
	handle->iface.rCards_raised = bool_false;
	handle->iface.giveBid_raised = bool_false;
	handle->iface.rBid_raised = bool_false;
	handle->iface.bidInfo_raised = bool_false;
	handle->iface.undoBid_raised = bool_false;
	handle->iface.undoPlay_raised = bool_false;
	handle->iface.readyForDummy_raised = bool_false;
	handle->iface.clientPlays_raised = bool_false;
	handle->iface.readyForDummyCards_raised = bool_false;
	handle->iface.readyForPlayer_raised = bool_false;
	handle->iface.getLeader_raised = bool_false;
	handle->iface.undoTrick_raised = bool_false;
	handle->iface.synchronize_raised = bool_false;
}

void zBridgeClient_runCycle(ZBridgeClient* handle)
{
	
	zBridgeClient_clearOutEvents(handle);
	
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < ZBRIDGECLIENT_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case ZBridgeClient_main_region_Connecting :
		{
			zBridgeClient_react_main_region_Connecting(handle);
			break;
		}
		case ZBridgeClient_main_region__final_ :
		{
			zBridgeClient_react_main_region__final_(handle);
			break;
		}
		case ZBridgeClient_main_region_Seated :
		{
			zBridgeClient_react_main_region_Seated(handle);
			break;
		}
		case ZBridgeClient_main_region_StartOfBoard :
		{
			zBridgeClient_react_main_region_StartOfBoard(handle);
			break;
		}
		case ZBridgeClient_main_region_Info :
		{
			zBridgeClient_react_main_region_Info(handle);
			break;
		}
		case ZBridgeClient_main_region_Cards :
		{
			zBridgeClient_react_main_region_Cards(handle);
			break;
		}
		case ZBridgeClient_main_region_Bid :
		{
			zBridgeClient_react_main_region_Bid(handle);
			break;
		}
		case ZBridgeClient_main_region_Lead :
		{
			zBridgeClient_react_main_region_Lead(handle);
			break;
		}
		case ZBridgeClient_main_region_DummyCards :
		{
			zBridgeClient_react_main_region_DummyCards(handle);
			break;
		}
		case ZBridgeClient_main_region_Play :
		{
			zBridgeClient_react_main_region_Play(handle);
			break;
		}
		case ZBridgeClient_main_region_WaitLeader :
		{
			zBridgeClient_react_main_region_WaitLeader(handle);
			break;
		}
		case ZBridgeClient_main_region_SyncSB :
		{
			zBridgeClient_react_main_region_SyncSB(handle);
			break;
		}
		case ZBridgeClient_main_region_SyncLeader :
		{
			zBridgeClient_react_main_region_SyncLeader(handle);
			break;
		}
		case ZBridgeClient_main_region_SyncAuction :
		{
			zBridgeClient_react_main_region_SyncAuction(handle);
			break;
		}
		case ZBridgeClient_main_region_SyncPlay :
		{
			zBridgeClient_react_main_region_SyncPlay(handle);
			break;
		}
		case ZBridgeClient_main_region_SyncReplay :
		{
			zBridgeClient_react_main_region_SyncReplay(handle);
			break;
		}
		default:
			break;
		}
	}
	
	zBridgeClient_clearInEvents(handle);
}


sc_boolean zBridgeClient_isStateActive(const ZBridgeClient* handle, ZBridgeClientStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case ZBridgeClient_main_region_Connecting :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Connecting
			);
			break;
		case ZBridgeClient_main_region__final_ :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region__final_
			);
			break;
		case ZBridgeClient_main_region_Seated :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Seated
			);
			break;
		case ZBridgeClient_main_region_StartOfBoard :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_StartOfBoard
			);
			break;
		case ZBridgeClient_main_region_Info :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Info
			);
			break;
		case ZBridgeClient_main_region_Cards :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Cards
			);
			break;
		case ZBridgeClient_main_region_Bid :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Bid
			);
			break;
		case ZBridgeClient_main_region_Lead :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Lead
			);
			break;
		case ZBridgeClient_main_region_DummyCards :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_DummyCards
			);
			break;
		case ZBridgeClient_main_region_Play :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Play
			);
			break;
		case ZBridgeClient_main_region_WaitLeader :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_WaitLeader
			);
			break;
		case ZBridgeClient_main_region_SyncSB :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_SyncSB
			);
			break;
		case ZBridgeClient_main_region_SyncLeader :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_SyncLeader
			);
			break;
		case ZBridgeClient_main_region_SyncAuction :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_SyncAuction
			);
			break;
		case ZBridgeClient_main_region_SyncPlay :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_SyncPlay
			);
			break;
		case ZBridgeClient_main_region_SyncReplay :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_SyncReplay
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

void zBridgeClientIface_raise_endOfSession(ZBridgeClient* handle)
{
	handle->iface.endOfSession_raised = bool_true;
}
void zBridgeClientIface_raise_connectError(ZBridgeClient* handle)
{
	handle->iface.connectError_raised = bool_true;
}
void zBridgeClientIface_raise_seated(ZBridgeClient* handle)
{
	handle->iface.seated_raised = bool_true;
}
void zBridgeClientIface_raise_teamNames(ZBridgeClient* handle)
{
	handle->iface.teamNames_raised = bool_true;
}
void zBridgeClientIface_raise_startOfBoard(ZBridgeClient* handle)
{
	handle->iface.startOfBoard_raised = bool_true;
}
void zBridgeClientIface_raise_dealInfo(ZBridgeClient* handle)
{
	handle->iface.dealInfo_raised = bool_true;
}
void zBridgeClientIface_raise_cards(ZBridgeClient* handle)
{
	handle->iface.cards_raised = bool_true;
}
void zBridgeClientIface_raise_undo(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.undo_value = value;
	handle->iface.undo_raised = bool_true;
}
void zBridgeClientIface_raise_bidDone(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.bidDone_value = value;
	handle->iface.bidDone_raised = bool_true;
}
void zBridgeClientIface_raise_playerPlays(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.playerPlays_value = value;
	handle->iface.playerPlays_raised = bool_true;
}
void zBridgeClientIface_raise_playerToLead(ZBridgeClient* handle)
{
	handle->iface.playerToLead_raised = bool_true;
}
void zBridgeClientIface_raise_dummyToLead(ZBridgeClient* handle)
{
	handle->iface.dummyToLead_raised = bool_true;
}
void zBridgeClientIface_raise_dummyCards(ZBridgeClient* handle)
{
	handle->iface.dummyCards_raised = bool_true;
}
void zBridgeClientIface_raise_newLeader(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.newLeader_value = value;
	handle->iface.newLeader_raised = bool_true;
}
void zBridgeClientIface_raise_reStart(ZBridgeClient* handle)
{
	handle->iface.reStart_raised = bool_true;
}
void zBridgeClientIface_raise_allSync(ZBridgeClient* handle)
{
	handle->iface.allSync_raised = bool_true;
}

sc_boolean zBridgeClientIface_israised_connect(const ZBridgeClient* handle)
{
	return handle->iface.connect_raised;
}
sc_boolean zBridgeClientIface_israised_rTNames(const ZBridgeClient* handle)
{
	return handle->iface.rTNames_raised;
}
sc_boolean zBridgeClientIface_israised_rSBoard(const ZBridgeClient* handle)
{
	return handle->iface.rSBoard_raised;
}
sc_boolean zBridgeClientIface_israised_rDealInfo(const ZBridgeClient* handle)
{
	return handle->iface.rDealInfo_raised;
}
sc_boolean zBridgeClientIface_israised_rCards(const ZBridgeClient* handle)
{
	return handle->iface.rCards_raised;
}
sc_boolean zBridgeClientIface_israised_giveBid(const ZBridgeClient* handle)
{
	return handle->iface.giveBid_raised;
}
sc_boolean zBridgeClientIface_israised_rBid(const ZBridgeClient* handle)
{
	return handle->iface.rBid_raised;
}
sc_boolean zBridgeClientIface_israised_bidInfo(const ZBridgeClient* handle)
{
	return handle->iface.bidInfo_raised;
}
sc_integer zBridgeClientIface_get_bidInfo_value(const ZBridgeClient* handle)
{
	return handle->iface.bidInfo_value;
}
sc_boolean zBridgeClientIface_israised_undoBid(const ZBridgeClient* handle)
{
	return handle->iface.undoBid_raised;
}
sc_integer zBridgeClientIface_get_undoBid_value(const ZBridgeClient* handle)
{
	return handle->iface.undoBid_value;
}
sc_boolean zBridgeClientIface_israised_undoPlay(const ZBridgeClient* handle)
{
	return handle->iface.undoPlay_raised;
}
sc_boolean zBridgeClientIface_israised_readyForDummy(const ZBridgeClient* handle)
{
	return handle->iface.readyForDummy_raised;
}
sc_boolean zBridgeClientIface_israised_clientPlays(const ZBridgeClient* handle)
{
	return handle->iface.clientPlays_raised;
}
sc_boolean zBridgeClientIface_israised_readyForDummyCards(const ZBridgeClient* handle)
{
	return handle->iface.readyForDummyCards_raised;
}
sc_boolean zBridgeClientIface_israised_readyForPlayer(const ZBridgeClient* handle)
{
	return handle->iface.readyForPlayer_raised;
}
sc_boolean zBridgeClientIface_israised_getLeader(const ZBridgeClient* handle)
{
	return handle->iface.getLeader_raised;
}
sc_boolean zBridgeClientIface_israised_undoTrick(const ZBridgeClient* handle)
{
	return handle->iface.undoTrick_raised;
}
sc_integer zBridgeClientIface_get_undoTrick_value(const ZBridgeClient* handle)
{
	return handle->iface.undoTrick_value;
}
sc_boolean zBridgeClientIface_israised_synchronize(const ZBridgeClient* handle)
{
	return handle->iface.synchronize_raised;
}

sc_integer zBridgeClientIface_get_boardNumber(const ZBridgeClient* handle)
{
	return handle->iface.boardNumber;
}
void zBridgeClientIface_set_boardNumber(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.boardNumber = value;
}
sc_integer zBridgeClientIface_get_vulnerability(const ZBridgeClient* handle)
{
	return handle->iface.vulnerability;
}
void zBridgeClientIface_set_vulnerability(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.vulnerability = value;
}
sc_integer zBridgeClientIface_get_client(const ZBridgeClient* handle)
{
	return handle->iface.client;
}
void zBridgeClientIface_set_client(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.client = value;
}
sc_integer zBridgeClientIface_get_dealer(const ZBridgeClient* handle)
{
	return handle->iface.dealer;
}
void zBridgeClientIface_set_dealer(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.dealer = value;
}
sc_integer zBridgeClientIface_get_bidder(const ZBridgeClient* handle)
{
	return handle->iface.bidder;
}
void zBridgeClientIface_set_bidder(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.bidder = value;
}
sc_integer zBridgeClientIface_get_bidVal(const ZBridgeClient* handle)
{
	return handle->iface.bidVal;
}
void zBridgeClientIface_set_bidVal(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.bidVal = value;
}
sc_integer zBridgeClientIface_get_lastBid(const ZBridgeClient* handle)
{
	return handle->iface.lastBid;
}
void zBridgeClientIface_set_lastBid(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.lastBid = value;
}
sc_integer zBridgeClientIface_get_bidDouble(const ZBridgeClient* handle)
{
	return handle->iface.bidDouble;
}
void zBridgeClientIface_set_bidDouble(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.bidDouble = value;
}
sc_integer zBridgeClientIface_get_bidEnable(const ZBridgeClient* handle)
{
	return handle->iface.bidEnable;
}
void zBridgeClientIface_set_bidEnable(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.bidEnable = value;
}
sc_integer zBridgeClientIface_get_declarer(const ZBridgeClient* handle)
{
	return handle->iface.declarer;
}
void zBridgeClientIface_set_declarer(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.declarer = value;
}
sc_integer zBridgeClientIface_get_dummy(const ZBridgeClient* handle)
{
	return handle->iface.dummy;
}
void zBridgeClientIface_set_dummy(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.dummy = value;
}
sc_integer zBridgeClientIface_get_leader(const ZBridgeClient* handle)
{
	return handle->iface.leader;
}
void zBridgeClientIface_set_leader(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.leader = value;
}
sc_integer zBridgeClientIface_get_noTrick(const ZBridgeClient* handle)
{
	return handle->iface.noTrick;
}
void zBridgeClientIface_set_noTrick(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.noTrick = value;
}
sc_integer zBridgeClientIface_get_player(const ZBridgeClient* handle)
{
	return handle->iface.player;
}
void zBridgeClientIface_set_player(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.player = value;
}
sc_integer zBridgeClientIface_get_cardVal(const ZBridgeClient* handle)
{
	return handle->iface.cardVal;
}
void zBridgeClientIface_set_cardVal(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.cardVal = value;
}
sc_integer zBridgeClientIface_get_syncState(const ZBridgeClient* handle)
{
	return handle->iface.syncState;
}
void zBridgeClientIface_set_syncState(ZBridgeClient* handle, sc_integer value)
{
	handle->iface.syncState = value;
}

/* implementations of all internal functions */

static sc_boolean zBridgeClient_check_main_region_Connecting_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.connectError_raised;
}

static sc_boolean zBridgeClient_check_main_region_Connecting_tr1_tr1(const ZBridgeClient* handle)
{
	return handle->iface.seated_raised;
}

static sc_boolean zBridgeClient_check_main_region_Seated_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.teamNames_raised;
}

static sc_boolean zBridgeClient_check_main_region_StartOfBoard_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.startOfBoard_raised;
}

static sc_boolean zBridgeClient_check_main_region_Info_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.dealInfo_raised;
}

static sc_boolean zBridgeClient_check_main_region_Cards_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.cards_raised;
}

static sc_boolean zBridgeClient_check_main_region_Bid_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.bidDone_raised;
}

static sc_boolean zBridgeClient_check_main_region_Bid_tr1_tr1(const ZBridgeClient* handle)
{
	return ((handle->iface.undo_raised) && (handle->iface.undo_value >= 0)) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region_Bid_tr2_tr2(const ZBridgeClient* handle)
{
	return ((handle->iface.undo_raised) && (handle->iface.undo_value == handle->internal.REBID)) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region_Bid_tr3_tr3(const ZBridgeClient* handle)
{
	return handle->iface.startOfBoard_raised;
}

static sc_boolean zBridgeClient_check_main_region_Bid_tr4_tr4(const ZBridgeClient* handle)
{
	return handle->iface.reStart_raised;
}

static sc_boolean zBridgeClient_check_main_region_Lead_tr0_tr0(const ZBridgeClient* handle)
{
	return ((handle->iface.playerToLead_raised) && (handle->iface.player != handle->iface.dummy)) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region_Lead_tr1_tr1(const ZBridgeClient* handle)
{
	return ((handle->iface.dummyToLead_raised) && (handle->iface.player == handle->iface.dummy)) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region_DummyCards_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.dummyCards_raised;
}

static sc_boolean zBridgeClient_check_main_region_Play_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.playerPlays_raised;
}

static sc_boolean zBridgeClient_check_main_region_Play_tr1_tr1(const ZBridgeClient* handle)
{
	return handle->iface.startOfBoard_raised;
}

static sc_boolean zBridgeClient_check_main_region_Play_tr2_tr2(const ZBridgeClient* handle)
{
	return handle->iface.reStart_raised;
}

static sc_boolean zBridgeClient_check_main_region_Play_tr3_tr3(const ZBridgeClient* handle)
{
	return ((handle->iface.undo_raised) && (handle->iface.undo_value == handle->internal.REPLAY)) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region_Play_tr4_tr4(const ZBridgeClient* handle)
{
	return ((handle->iface.undo_raised) && (handle->iface.undo_value == handle->internal.REBID)) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region_Play_tr5_tr5(const ZBridgeClient* handle)
{
	return ((handle->iface.undo_raised) && (handle->iface.undo_value >= 0)) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region_WaitLeader_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.newLeader_raised;
}

static sc_boolean zBridgeClient_check_main_region_SyncSB_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.allSync_raised;
}

static sc_boolean zBridgeClient_check_main_region_SyncSB_tr1_tr1(const ZBridgeClient* handle)
{
	return handle->iface.reStart_raised;
}

static sc_boolean zBridgeClient_check_main_region_SyncLeader_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.allSync_raised;
}

static sc_boolean zBridgeClient_check_main_region_SyncLeader_tr1_tr1(const ZBridgeClient* handle)
{
	return handle->iface.startOfBoard_raised;
}

static sc_boolean zBridgeClient_check_main_region_SyncLeader_tr2_tr2(const ZBridgeClient* handle)
{
	return handle->iface.reStart_raised;
}

static sc_boolean zBridgeClient_check_main_region_SyncLeader_tr3_tr3(const ZBridgeClient* handle)
{
	return ((handle->iface.undo_raised) && (handle->iface.undo_value == handle->internal.REPLAY)) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region_SyncLeader_tr4_tr4(const ZBridgeClient* handle)
{
	return ((handle->iface.undo_raised) && (handle->iface.undo_value == handle->internal.REBID)) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region_SyncLeader_tr5_tr5(const ZBridgeClient* handle)
{
	return ((handle->iface.undo_raised) && (handle->iface.undo_value >= 0)) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region_SyncAuction_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.allSync_raised;
}

static sc_boolean zBridgeClient_check_main_region_SyncAuction_tr1_tr1(const ZBridgeClient* handle)
{
	return handle->iface.startOfBoard_raised;
}

static sc_boolean zBridgeClient_check_main_region_SyncAuction_tr2_tr2(const ZBridgeClient* handle)
{
	return handle->iface.reStart_raised;
}

static sc_boolean zBridgeClient_check_main_region_SyncPlay_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.allSync_raised;
}

static sc_boolean zBridgeClient_check_main_region_SyncPlay_tr1_tr1(const ZBridgeClient* handle)
{
	return handle->iface.startOfBoard_raised;
}

static sc_boolean zBridgeClient_check_main_region_SyncPlay_tr2_tr2(const ZBridgeClient* handle)
{
	return handle->iface.reStart_raised;
}

static sc_boolean zBridgeClient_check_main_region_SyncReplay_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->iface.allSync_raised;
}

static sc_boolean zBridgeClient_check_main_region__choice_0_tr0_tr0(const ZBridgeClient* handle)
{
	return (handle->iface.bidder == handle->iface.client) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region__choice_0_tr1_tr1(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_1_tr0_tr0(const ZBridgeClient* handle)
{
	return (handle->iface.bidVal == handle->internal.BID_PASS) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region__choice_1_tr1_tr1(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_2_tr0_tr0(const ZBridgeClient* handle)
{
	return (handle->internal.firstBidRound && (handle->internal.noPasses == 4)) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region__choice_2_tr1_tr1(const ZBridgeClient* handle)
{
	return (!handle->internal.firstBidRound && (handle->internal.noPasses == 3)) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region__choice_2_tr2_tr2(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_3_tr0_tr0(const ZBridgeClient* handle)
{
	return ((handle->iface.bidVal == handle->internal.BID_DOUBLE) || (handle->iface.bidVal == handle->internal.BID_REDOUBLE)) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region__choice_3_tr1_tr1(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_4_tr0_tr0(const ZBridgeClient* handle)
{
	return (((handle->iface.client == handle->iface.player) && (handle->iface.player != handle->iface.dummy)) || ((handle->iface.client == handle->iface.declarer) && (handle->iface.player == handle->iface.dummy))) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region__choice_4_tr1_tr1(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_5_tr0_tr0(const ZBridgeClient* handle)
{
	return handle->internal.firstTrick;
}

static sc_boolean zBridgeClient_check_main_region__choice_5_tr1_tr1(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_6_tr0_tr0(const ZBridgeClient* handle)
{
	return (handle->iface.noTrick == 13) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region__choice_6_tr1_tr1(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_7_tr0_tr0(const ZBridgeClient* handle)
{
	return (handle->internal.playNo == 0) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region__choice_7_tr1_tr1(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_8_tr0(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_9_tr0_tr0(const ZBridgeClient* handle)
{
	return (handle->internal.playNo < 4) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region__choice_9_tr1_tr1(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_10_tr1_tr1(const ZBridgeClient* handle)
{
	return (handle->iface.player == handle->iface.dummy) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region__choice_10_tr0_tr0(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_11_tr0_tr0(const ZBridgeClient* handle)
{
	return ((handle->internal.curBidder == ((handle->iface.client + 1) & 3)) || (handle->internal.curBidder == ((handle->iface.client + 3) & 3))) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region__choice_11_tr1_tr1(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_12_tr1_tr1(const ZBridgeClient* handle)
{
	return ((handle->iface.bidVal == handle->internal.BID_DOUBLE) && ((handle->internal.curBidder == ((handle->iface.client + 1) & 3)) || (handle->internal.curBidder == ((handle->iface.client + 3 & 3))))) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region__choice_12_tr0_tr0(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_13_tr0_tr0(const ZBridgeClient* handle)
{
	return (handle->iface.client != handle->iface.dummy) ? bool_true : bool_false;
}

static sc_boolean zBridgeClient_check_main_region__choice_13_tr1_tr1(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_14_tr0(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_15_tr0(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_16_tr0(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_17_tr0(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_18_tr0(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_19_tr0(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_20_tr0(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_21_tr0(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_22_tr0(const ZBridgeClient* handle)
{
	return bool_true;
}

static sc_boolean zBridgeClient_check_main_region__choice_23_tr0(const ZBridgeClient* handle)
{
	return bool_true;
}

static void zBridgeClient_effect_main_region_Connecting_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Connecting(handle);
	zBridgeClient_enseq_main_region__final__default(handle);
}

static void zBridgeClient_effect_main_region_Connecting_tr1(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Connecting(handle);
	handle->iface.rTNames_raised = bool_true;
	zBridgeClient_enseq_main_region_Seated_default(handle);
}

static void zBridgeClient_effect_main_region_Seated_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Seated(handle);
	handle->iface.rSBoard_raised = bool_true;
	zBridgeClient_enseq_main_region_StartOfBoard_default(handle);
}

static void zBridgeClient_effect_main_region_StartOfBoard_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_StartOfBoard(handle);
	handle->iface.rDealInfo_raised = bool_true;
	zBridgeClient_enseq_main_region_Info_default(handle);
}

static void zBridgeClient_effect_main_region_Info_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Info(handle);
	handle->iface.rCards_raised = bool_true;
	zBridgeClient_enseq_main_region_Cards_default(handle);
}

static void zBridgeClient_effect_main_region_Cards_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Cards(handle);
	handle->iface.bidder = handle->iface.dealer;
	handle->internal.firstBidRound = bool_true;
	handle->iface.lastBid = handle->internal.BID_NONE;
	handle->internal.noPasses = 0;
	handle->iface.synchronize_raised = bool_true;
	zBridgeClient_enseq_main_region_SyncAuction_default(handle);
}

static void zBridgeClient_effect_main_region_Bid_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Bid(handle);
	handle->iface.bidVal = handle->iface.bidDone_value;
	handle->internal.curBidder = handle->iface.bidder;
	handle->iface.bidder += 1;
	handle->iface.bidder &= 3;
	zBridgeClient_react_main_region__choice_1(handle);
}

static void zBridgeClient_effect_main_region_Bid_tr1(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Bid(handle);
	handle->internal.lastBidder = (handle->iface.undo_value + handle->iface.dealer) & 3;
	handle->iface.lastBid = handle->iface.bidVal;
	handle->iface.bidDouble = handle->internal.BID_NONE;
	handle->internal.noPasses = 0;
	handle->iface.bidder = (handle->internal.lastBidder + 1) & 3;
	handle->iface.undoBid_value = handle->iface.undo_value;
	handle->iface.undoBid_raised = bool_true;
	zBridgeClient_react_main_region__choice_0(handle);
}

static void zBridgeClient_effect_main_region_Bid_tr2(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Bid(handle);
	zBridgeClient_react_main_region__choice_22(handle);
}

static void zBridgeClient_effect_main_region_Bid_tr3(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Bid(handle);
	handle->iface.synchronize_raised = bool_true;
	zBridgeClient_enseq_main_region_SyncSB_default(handle);
}

static void zBridgeClient_effect_main_region_Bid_tr4(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Bid(handle);
	zBridgeClient_react_main_region__choice_17(handle);
}

static void zBridgeClient_effect_main_region_Lead_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Lead(handle);
	zBridgeClient_react_main_region__choice_8(handle);
}

static void zBridgeClient_effect_main_region_Lead_tr1(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Lead(handle);
	zBridgeClient_react_main_region__choice_8(handle);
}

static void zBridgeClient_effect_main_region_DummyCards_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_DummyCards(handle);
	zBridgeClient_react_main_region__choice_4(handle);
}

static void zBridgeClient_effect_main_region_Play_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Play(handle);
	handle->iface.cardVal = handle->iface.playerPlays_value;
	handle->internal.playNo += 1;
	handle->iface.player = (handle->iface.leader + handle->internal.playNo) & 3;
	zBridgeClient_react_main_region__choice_5(handle);
}

static void zBridgeClient_effect_main_region_Play_tr1(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Play(handle);
	zBridgeClient_react_main_region__choice_14(handle);
}

static void zBridgeClient_effect_main_region_Play_tr2(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Play(handle);
	zBridgeClient_react_main_region__choice_15(handle);
}

static void zBridgeClient_effect_main_region_Play_tr3(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Play(handle);
	zBridgeClient_react_main_region__choice_20(handle);
}

static void zBridgeClient_effect_main_region_Play_tr4(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Play(handle);
	zBridgeClient_react_main_region__choice_21(handle);
}

static void zBridgeClient_effect_main_region_Play_tr5(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_Play(handle);
	handle->iface.undoTrick_value = handle->internal.PT;
	handle->iface.undoTrick_raised = bool_true;
	zBridgeClient_react_main_region__choice_23(handle);
}

static void zBridgeClient_effect_main_region_WaitLeader_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_WaitLeader(handle);
	handle->iface.leader = handle->iface.newLeader_value;
	handle->iface.player = handle->iface.leader;
	handle->internal.playNo = 0;
	handle->iface.synchronize_raised = bool_true;
	zBridgeClient_enseq_main_region_SyncLeader_default(handle);
}

static void zBridgeClient_effect_main_region_SyncSB_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncSB(handle);
	zBridgeClient_enseq_main_region_StartOfBoard_default(handle);
}

static void zBridgeClient_effect_main_region_SyncSB_tr1(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncSB(handle);
	zBridgeClient_react_main_region__choice_19(handle);
}

static void zBridgeClient_effect_main_region_SyncLeader_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncLeader(handle);
	zBridgeClient_react_main_region__choice_6(handle);
}

static void zBridgeClient_effect_main_region_SyncLeader_tr1(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncLeader(handle);
	zBridgeClient_react_main_region__choice_14(handle);
}

static void zBridgeClient_effect_main_region_SyncLeader_tr2(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncLeader(handle);
	zBridgeClient_react_main_region__choice_15(handle);
}

static void zBridgeClient_effect_main_region_SyncLeader_tr3(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncLeader(handle);
	zBridgeClient_react_main_region__choice_20(handle);
}

static void zBridgeClient_effect_main_region_SyncLeader_tr4(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncLeader(handle);
	zBridgeClient_react_main_region__choice_21(handle);
}

static void zBridgeClient_effect_main_region_SyncLeader_tr5(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncLeader(handle);
	handle->iface.noTrick -= 1;
	handle->iface.undoTrick_value = handle->internal.CT;
	handle->iface.undoTrick_raised = bool_true;
	zBridgeClient_react_main_region__choice_23(handle);
}

static void zBridgeClient_effect_main_region_SyncAuction_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncAuction(handle);
	zBridgeClient_react_main_region__choice_0(handle);
}

static void zBridgeClient_effect_main_region_SyncAuction_tr1(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncAuction(handle);
	handle->iface.synchronize_raised = bool_true;
	zBridgeClient_enseq_main_region_SyncSB_default(handle);
}

static void zBridgeClient_effect_main_region_SyncAuction_tr2(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncAuction(handle);
	zBridgeClient_react_main_region__choice_18(handle);
}

static void zBridgeClient_effect_main_region_SyncPlay_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncPlay(handle);
	handle->iface.bidInfo_value = handle->iface.leader;
	handle->iface.bidInfo_raised = bool_true;
	zBridgeClient_react_main_region__choice_4(handle);
}

static void zBridgeClient_effect_main_region_SyncPlay_tr1(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncPlay(handle);
	handle->iface.synchronize_raised = bool_true;
	zBridgeClient_enseq_main_region_SyncSB_default(handle);
}

static void zBridgeClient_effect_main_region_SyncPlay_tr2(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncPlay(handle);
	zBridgeClient_react_main_region__choice_16(handle);
}

static void zBridgeClient_effect_main_region_SyncReplay_tr0(ZBridgeClient* handle)
{
	zBridgeClient_exseq_main_region_SyncReplay(handle);
	zBridgeClient_react_main_region__choice_6(handle);
}

static void zBridgeClient_effect_main_region__choice_0_tr0(ZBridgeClient* handle)
{
	handle->iface.giveBid_raised = bool_true;
	zBridgeClient_enseq_main_region_Bid_default(handle);
}

static void zBridgeClient_effect_main_region__choice_0_tr1(ZBridgeClient* handle)
{
	handle->iface.rBid_raised = bool_true;
	zBridgeClient_enseq_main_region_Bid_default(handle);
}

static void zBridgeClient_effect_main_region__choice_1_tr0(ZBridgeClient* handle)
{
	handle->internal.noPasses += 1;
	zBridgeClient_react_main_region__choice_2(handle);
}

static void zBridgeClient_effect_main_region__choice_1_tr1(ZBridgeClient* handle)
{
	handle->internal.firstBidRound = bool_false;
	handle->internal.noPasses = 0;
	zBridgeClient_react_main_region__choice_3(handle);
}

static void zBridgeClient_effect_main_region__choice_2_tr0(ZBridgeClient* handle)
{
	handle->iface.synchronize_raised = bool_true;
	zBridgeClient_enseq_main_region_SyncSB_default(handle);
}

static void zBridgeClient_effect_main_region__choice_2_tr1(ZBridgeClient* handle)
{
	handle->iface.declarer = handle->internal.lastBidder;
	handle->iface.dummy = (handle->iface.declarer + 2) & 3;
	handle->internal.firstTrick = bool_true;
	handle->iface.noTrick = 0;
	handle->iface.leader = (handle->iface.declarer + 1) & 3;
	handle->internal.playNo = 0;
	handle->iface.player = handle->iface.leader;
	handle->iface.synchronize_raised = bool_true;
	zBridgeClient_enseq_main_region_SyncPlay_default(handle);
}

static void zBridgeClient_effect_main_region__choice_2_tr2(ZBridgeClient* handle)
{
	zBridgeClient_react_main_region__choice_0(handle);
}

static void zBridgeClient_effect_main_region__choice_3_tr0(ZBridgeClient* handle)
{
	handle->iface.bidDouble = handle->iface.bidVal;
	zBridgeClient_react_main_region__choice_12(handle);
}

static void zBridgeClient_effect_main_region__choice_3_tr1(ZBridgeClient* handle)
{
	handle->internal.lastBidder = handle->internal.curBidder;
	handle->iface.lastBid = handle->iface.bidVal;
	handle->iface.bidDouble = handle->internal.BID_NONE;
	zBridgeClient_react_main_region__choice_11(handle);
}

static void zBridgeClient_effect_main_region__choice_4_tr0(ZBridgeClient* handle)
{
	zBridgeClient_react_main_region__choice_7(handle);
}

static void zBridgeClient_effect_main_region__choice_4_tr1(ZBridgeClient* handle)
{
	zBridgeClient_react_main_region__choice_10(handle);
}

static void zBridgeClient_effect_main_region__choice_5_tr0(ZBridgeClient* handle)
{
	handle->internal.firstTrick = bool_false;
	zBridgeClient_react_main_region__choice_13(handle);
}

static void zBridgeClient_effect_main_region__choice_5_tr1(ZBridgeClient* handle)
{
	zBridgeClient_react_main_region__choice_9(handle);
}

static void zBridgeClient_effect_main_region__choice_6_tr0(ZBridgeClient* handle)
{
	zBridgeClient_react_main_region__choice_14(handle);
}

static void zBridgeClient_effect_main_region__choice_6_tr1(ZBridgeClient* handle)
{
	zBridgeClient_react_main_region__choice_4(handle);
}

static void zBridgeClient_effect_main_region__choice_7_tr0(ZBridgeClient* handle)
{
	zBridgeClient_enseq_main_region_Lead_default(handle);
}

static void zBridgeClient_effect_main_region__choice_7_tr1(ZBridgeClient* handle)
{
	zBridgeClient_react_main_region__choice_8(handle);
}

static void zBridgeClient_effect_main_region__choice_8_tr0(ZBridgeClient* handle)
{
	handle->iface.clientPlays_raised = bool_true;
	zBridgeClient_enseq_main_region_Play_default(handle);
}

static void zBridgeClient_effect_main_region__choice_9_tr0(ZBridgeClient* handle)
{
	zBridgeClient_react_main_region__choice_4(handle);
}

static void zBridgeClient_effect_main_region__choice_9_tr1(ZBridgeClient* handle)
{
	handle->iface.noTrick += 1;
	handle->iface.getLeader_raised = bool_true;
	zBridgeClient_enseq_main_region_WaitLeader_default(handle);
}

static void zBridgeClient_effect_main_region__choice_10_tr1(ZBridgeClient* handle)
{
	handle->iface.readyForDummy_raised = bool_true;
	zBridgeClient_enseq_main_region_Play_default(handle);
}

static void zBridgeClient_effect_main_region__choice_10_tr0(ZBridgeClient* handle)
{
	handle->iface.readyForPlayer_raised = bool_true;
	zBridgeClient_enseq_main_region_Play_default(handle);
}

static void zBridgeClient_effect_main_region__choice_11_tr0(ZBridgeClient* handle)
{
	handle->iface.bidEnable = handle->internal.BID_DOUBLE;
	zBridgeClient_react_main_region__choice_0(handle);
}

static void zBridgeClient_effect_main_region__choice_11_tr1(ZBridgeClient* handle)
{
	handle->iface.bidEnable = handle->internal.BID_NONE;
	zBridgeClient_react_main_region__choice_0(handle);
}

static void zBridgeClient_effect_main_region__choice_12_tr1(ZBridgeClient* handle)
{
	handle->iface.bidEnable = handle->internal.BID_REDOUBLE;
	zBridgeClient_react_main_region__choice_0(handle);
}

static void zBridgeClient_effect_main_region__choice_12_tr0(ZBridgeClient* handle)
{
	handle->iface.bidEnable = handle->internal.BID_NONE;
	zBridgeClient_react_main_region__choice_0(handle);
}

static void zBridgeClient_effect_main_region__choice_13_tr0(ZBridgeClient* handle)
{
	handle->iface.readyForDummyCards_raised = bool_true;
	zBridgeClient_enseq_main_region_DummyCards_default(handle);
}

static void zBridgeClient_effect_main_region__choice_13_tr1(ZBridgeClient* handle)
{
	zBridgeClient_react_main_region__choice_9(handle);
}

static void zBridgeClient_effect_main_region__choice_14_tr0(ZBridgeClient* handle)
{
	handle->iface.synchronize_raised = bool_true;
	zBridgeClient_enseq_main_region_SyncSB_default(handle);
}

static void zBridgeClient_effect_main_region__choice_15_tr0(ZBridgeClient* handle)
{
	zBridgeClient_react_main_region__choice_16(handle);
}

static void zBridgeClient_effect_main_region__choice_16_tr0(ZBridgeClient* handle)
{
	zBridgeClient_react_main_region__choice_17(handle);
}

static void zBridgeClient_effect_main_region__choice_17_tr0(ZBridgeClient* handle)
{
	zBridgeClient_react_main_region__choice_18(handle);
}

static void zBridgeClient_effect_main_region__choice_18_tr0(ZBridgeClient* handle)
{
	zBridgeClient_react_main_region__choice_19(handle);
}

static void zBridgeClient_effect_main_region__choice_19_tr0(ZBridgeClient* handle)
{
	zBridgeClient_enseq_main_region_Connecting_default(handle);
}

static void zBridgeClient_effect_main_region__choice_20_tr0(ZBridgeClient* handle)
{
	handle->internal.firstTrick = bool_true;
	handle->iface.noTrick = 0;
	handle->iface.leader = (handle->iface.declarer + 1) & 3;
	handle->internal.playNo = 0;
	handle->iface.player = handle->iface.leader;
	handle->iface.undoTrick_value = handle->internal.REPLAY;
	handle->iface.undoTrick_raised = bool_true;
	handle->iface.synchronize_raised = bool_true;
	zBridgeClient_enseq_main_region_SyncReplay_default(handle);
}

static void zBridgeClient_effect_main_region__choice_21_tr0(ZBridgeClient* handle)
{
	handle->iface.undoPlay_raised = bool_true;
	zBridgeClient_react_main_region__choice_22(handle);
}

static void zBridgeClient_effect_main_region__choice_22_tr0(ZBridgeClient* handle)
{
	handle->iface.bidder = handle->iface.dealer;
	handle->internal.firstBidRound = bool_true;
	handle->iface.lastBid = handle->internal.BID_NONE;
	handle->internal.noPasses = 0;
	handle->iface.undoBid_value = handle->internal.REBID;
	handle->iface.undoBid_raised = bool_true;
	zBridgeClient_react_main_region__choice_0(handle);
}

static void zBridgeClient_effect_main_region__choice_23_tr0(ZBridgeClient* handle)
{
	handle->iface.leader = handle->iface.undo_value;
	handle->iface.player = handle->iface.leader;
	handle->internal.playNo = 0;
	handle->iface.synchronize_raised = bool_true;
	zBridgeClient_enseq_main_region_SyncLeader_default(handle);
}

/* Entry action for state 'Connecting'. */
static void zBridgeClient_enact_main_region_Connecting(ZBridgeClient* handle)
{
	/* Entry action for state 'Connecting'. */
	handle->iface.connect_raised = bool_true;
}

/* Entry action for state 'SyncSB'. */
static void zBridgeClient_enact_main_region_SyncSB(ZBridgeClient* handle)
{
	/* Entry action for state 'SyncSB'. */
	handle->iface.syncState = handle->internal.SS;
}

/* Entry action for state 'SyncLeader'. */
static void zBridgeClient_enact_main_region_SyncLeader(ZBridgeClient* handle)
{
	/* Entry action for state 'SyncLeader'. */
	handle->iface.syncState = handle->internal.SL;
}

/* Entry action for state 'SyncAuction'. */
static void zBridgeClient_enact_main_region_SyncAuction(ZBridgeClient* handle)
{
	/* Entry action for state 'SyncAuction'. */
	handle->iface.syncState = handle->internal.SA;
}

/* Entry action for state 'SyncPlay'. */
static void zBridgeClient_enact_main_region_SyncPlay(ZBridgeClient* handle)
{
	/* Entry action for state 'SyncPlay'. */
	handle->iface.syncState = handle->internal.SP;
}

/* Entry action for state 'SyncReplay'. */
static void zBridgeClient_enact_main_region_SyncReplay(ZBridgeClient* handle)
{
	/* Entry action for state 'SyncReplay'. */
	handle->iface.syncState = handle->internal.SR;
}

/* 'default' enter sequence for state Connecting */
static void zBridgeClient_enseq_main_region_Connecting_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state Connecting */
	zBridgeClient_enact_main_region_Connecting(handle);
	handle->stateConfVector[0] = ZBridgeClient_main_region_Connecting;
	handle->stateConfVectorPosition = 0;
}

/* Default enter sequence for state null */
static void zBridgeClient_enseq_main_region__final__default(ZBridgeClient* handle)
{
	/* Default enter sequence for state null */
	handle->stateConfVector[0] = ZBridgeClient_main_region__final_;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Seated */
static void zBridgeClient_enseq_main_region_Seated_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state Seated */
	handle->stateConfVector[0] = ZBridgeClient_main_region_Seated;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state StartOfBoard */
static void zBridgeClient_enseq_main_region_StartOfBoard_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state StartOfBoard */
	handle->stateConfVector[0] = ZBridgeClient_main_region_StartOfBoard;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Info */
static void zBridgeClient_enseq_main_region_Info_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state Info */
	handle->stateConfVector[0] = ZBridgeClient_main_region_Info;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Cards */
static void zBridgeClient_enseq_main_region_Cards_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state Cards */
	handle->stateConfVector[0] = ZBridgeClient_main_region_Cards;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Bid */
static void zBridgeClient_enseq_main_region_Bid_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state Bid */
	handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Lead */
static void zBridgeClient_enseq_main_region_Lead_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state Lead */
	handle->stateConfVector[0] = ZBridgeClient_main_region_Lead;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state DummyCards */
static void zBridgeClient_enseq_main_region_DummyCards_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state DummyCards */
	handle->stateConfVector[0] = ZBridgeClient_main_region_DummyCards;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Play */
static void zBridgeClient_enseq_main_region_Play_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state Play */
	handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state WaitLeader */
static void zBridgeClient_enseq_main_region_WaitLeader_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state WaitLeader */
	handle->stateConfVector[0] = ZBridgeClient_main_region_WaitLeader;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state SyncSB */
static void zBridgeClient_enseq_main_region_SyncSB_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state SyncSB */
	zBridgeClient_enact_main_region_SyncSB(handle);
	handle->stateConfVector[0] = ZBridgeClient_main_region_SyncSB;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state SyncLeader */
static void zBridgeClient_enseq_main_region_SyncLeader_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state SyncLeader */
	zBridgeClient_enact_main_region_SyncLeader(handle);
	handle->stateConfVector[0] = ZBridgeClient_main_region_SyncLeader;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state SyncAuction */
static void zBridgeClient_enseq_main_region_SyncAuction_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state SyncAuction */
	zBridgeClient_enact_main_region_SyncAuction(handle);
	handle->stateConfVector[0] = ZBridgeClient_main_region_SyncAuction;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state SyncPlay */
static void zBridgeClient_enseq_main_region_SyncPlay_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state SyncPlay */
	zBridgeClient_enact_main_region_SyncPlay(handle);
	handle->stateConfVector[0] = ZBridgeClient_main_region_SyncPlay;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state SyncReplay */
static void zBridgeClient_enseq_main_region_SyncReplay_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for state SyncReplay */
	zBridgeClient_enact_main_region_SyncReplay(handle);
	handle->stateConfVector[0] = ZBridgeClient_main_region_SyncReplay;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
static void zBridgeClient_enseq_main_region_default(ZBridgeClient* handle)
{
	/* 'default' enter sequence for region main region */
	zBridgeClient_react_main_region__entry_Default(handle);
}

/* Default exit sequence for state Connecting */
static void zBridgeClient_exseq_main_region_Connecting(ZBridgeClient* handle)
{
	/* Default exit sequence for state Connecting */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for final state. */
static void zBridgeClient_exseq_main_region__final_(ZBridgeClient* handle)
{
	/* Default exit sequence for final state. */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state Seated */
static void zBridgeClient_exseq_main_region_Seated(ZBridgeClient* handle)
{
	/* Default exit sequence for state Seated */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state StartOfBoard */
static void zBridgeClient_exseq_main_region_StartOfBoard(ZBridgeClient* handle)
{
	/* Default exit sequence for state StartOfBoard */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state Info */
static void zBridgeClient_exseq_main_region_Info(ZBridgeClient* handle)
{
	/* Default exit sequence for state Info */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state Cards */
static void zBridgeClient_exseq_main_region_Cards(ZBridgeClient* handle)
{
	/* Default exit sequence for state Cards */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state Bid */
static void zBridgeClient_exseq_main_region_Bid(ZBridgeClient* handle)
{
	/* Default exit sequence for state Bid */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state Lead */
static void zBridgeClient_exseq_main_region_Lead(ZBridgeClient* handle)
{
	/* Default exit sequence for state Lead */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state DummyCards */
static void zBridgeClient_exseq_main_region_DummyCards(ZBridgeClient* handle)
{
	/* Default exit sequence for state DummyCards */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state Play */
static void zBridgeClient_exseq_main_region_Play(ZBridgeClient* handle)
{
	/* Default exit sequence for state Play */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state WaitLeader */
static void zBridgeClient_exseq_main_region_WaitLeader(ZBridgeClient* handle)
{
	/* Default exit sequence for state WaitLeader */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state SyncSB */
static void zBridgeClient_exseq_main_region_SyncSB(ZBridgeClient* handle)
{
	/* Default exit sequence for state SyncSB */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state SyncLeader */
static void zBridgeClient_exseq_main_region_SyncLeader(ZBridgeClient* handle)
{
	/* Default exit sequence for state SyncLeader */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state SyncAuction */
static void zBridgeClient_exseq_main_region_SyncAuction(ZBridgeClient* handle)
{
	/* Default exit sequence for state SyncAuction */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state SyncPlay */
static void zBridgeClient_exseq_main_region_SyncPlay(ZBridgeClient* handle)
{
	/* Default exit sequence for state SyncPlay */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state SyncReplay */
static void zBridgeClient_exseq_main_region_SyncReplay(ZBridgeClient* handle)
{
	/* Default exit sequence for state SyncReplay */
	handle->stateConfVector[0] = ZBridgeClient_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
static void zBridgeClient_exseq_main_region(ZBridgeClient* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of ZBridgeClient.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case ZBridgeClient_main_region_Connecting :
		{
			zBridgeClient_exseq_main_region_Connecting(handle);
			break;
		}
		case ZBridgeClient_main_region__final_ :
		{
			zBridgeClient_exseq_main_region__final_(handle);
			break;
		}
		case ZBridgeClient_main_region_Seated :
		{
			zBridgeClient_exseq_main_region_Seated(handle);
			break;
		}
		case ZBridgeClient_main_region_StartOfBoard :
		{
			zBridgeClient_exseq_main_region_StartOfBoard(handle);
			break;
		}
		case ZBridgeClient_main_region_Info :
		{
			zBridgeClient_exseq_main_region_Info(handle);
			break;
		}
		case ZBridgeClient_main_region_Cards :
		{
			zBridgeClient_exseq_main_region_Cards(handle);
			break;
		}
		case ZBridgeClient_main_region_Bid :
		{
			zBridgeClient_exseq_main_region_Bid(handle);
			break;
		}
		case ZBridgeClient_main_region_Lead :
		{
			zBridgeClient_exseq_main_region_Lead(handle);
			break;
		}
		case ZBridgeClient_main_region_DummyCards :
		{
			zBridgeClient_exseq_main_region_DummyCards(handle);
			break;
		}
		case ZBridgeClient_main_region_Play :
		{
			zBridgeClient_exseq_main_region_Play(handle);
			break;
		}
		case ZBridgeClient_main_region_WaitLeader :
		{
			zBridgeClient_exseq_main_region_WaitLeader(handle);
			break;
		}
		case ZBridgeClient_main_region_SyncSB :
		{
			zBridgeClient_exseq_main_region_SyncSB(handle);
			break;
		}
		case ZBridgeClient_main_region_SyncLeader :
		{
			zBridgeClient_exseq_main_region_SyncLeader(handle);
			break;
		}
		case ZBridgeClient_main_region_SyncAuction :
		{
			zBridgeClient_exseq_main_region_SyncAuction(handle);
			break;
		}
		case ZBridgeClient_main_region_SyncPlay :
		{
			zBridgeClient_exseq_main_region_SyncPlay(handle);
			break;
		}
		case ZBridgeClient_main_region_SyncReplay :
		{
			zBridgeClient_exseq_main_region_SyncReplay(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state Connecting. */
static void zBridgeClient_react_main_region_Connecting(ZBridgeClient* handle)
{
	/* The reactions of state Connecting. */
	if (zBridgeClient_check_main_region_Connecting_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_Connecting_tr0(handle);
	}  else
	{
		if (zBridgeClient_check_main_region_Connecting_tr1_tr1(handle) == bool_true)
		{ 
			zBridgeClient_effect_main_region_Connecting_tr1(handle);
		} 
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__final_(ZBridgeClient* handle)
{
	/* The reactions of state null. */
}

/* The reactions of state Seated. */
static void zBridgeClient_react_main_region_Seated(ZBridgeClient* handle)
{
	/* The reactions of state Seated. */
	if (zBridgeClient_check_main_region_Seated_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_Seated_tr0(handle);
	} 
}

/* The reactions of state StartOfBoard. */
static void zBridgeClient_react_main_region_StartOfBoard(ZBridgeClient* handle)
{
	/* The reactions of state StartOfBoard. */
	if (zBridgeClient_check_main_region_StartOfBoard_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_StartOfBoard_tr0(handle);
	} 
}

/* The reactions of state Info. */
static void zBridgeClient_react_main_region_Info(ZBridgeClient* handle)
{
	/* The reactions of state Info. */
	if (zBridgeClient_check_main_region_Info_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_Info_tr0(handle);
	} 
}

/* The reactions of state Cards. */
static void zBridgeClient_react_main_region_Cards(ZBridgeClient* handle)
{
	/* The reactions of state Cards. */
	if (zBridgeClient_check_main_region_Cards_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_Cards_tr0(handle);
	} 
}

/* The reactions of state Bid. */
static void zBridgeClient_react_main_region_Bid(ZBridgeClient* handle)
{
	/* The reactions of state Bid. */
	if (zBridgeClient_check_main_region_Bid_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_Bid_tr0(handle);
	}  else
	{
		if (zBridgeClient_check_main_region_Bid_tr1_tr1(handle) == bool_true)
		{ 
			zBridgeClient_effect_main_region_Bid_tr1(handle);
		}  else
		{
			if (zBridgeClient_check_main_region_Bid_tr2_tr2(handle) == bool_true)
			{ 
				zBridgeClient_effect_main_region_Bid_tr2(handle);
			}  else
			{
				if (zBridgeClient_check_main_region_Bid_tr3_tr3(handle) == bool_true)
				{ 
					zBridgeClient_effect_main_region_Bid_tr3(handle);
				}  else
				{
					if (zBridgeClient_check_main_region_Bid_tr4_tr4(handle) == bool_true)
					{ 
						zBridgeClient_effect_main_region_Bid_tr4(handle);
					} 
				}
			}
		}
	}
}

/* The reactions of state Lead. */
static void zBridgeClient_react_main_region_Lead(ZBridgeClient* handle)
{
	/* The reactions of state Lead. */
	if (zBridgeClient_check_main_region_Lead_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_Lead_tr0(handle);
	}  else
	{
		if (zBridgeClient_check_main_region_Lead_tr1_tr1(handle) == bool_true)
		{ 
			zBridgeClient_effect_main_region_Lead_tr1(handle);
		} 
	}
}

/* The reactions of state DummyCards. */
static void zBridgeClient_react_main_region_DummyCards(ZBridgeClient* handle)
{
	/* The reactions of state DummyCards. */
	if (zBridgeClient_check_main_region_DummyCards_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_DummyCards_tr0(handle);
	} 
}

/* The reactions of state Play. */
static void zBridgeClient_react_main_region_Play(ZBridgeClient* handle)
{
	/* The reactions of state Play. */
	if (zBridgeClient_check_main_region_Play_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_Play_tr0(handle);
	}  else
	{
		if (zBridgeClient_check_main_region_Play_tr1_tr1(handle) == bool_true)
		{ 
			zBridgeClient_effect_main_region_Play_tr1(handle);
		}  else
		{
			if (zBridgeClient_check_main_region_Play_tr2_tr2(handle) == bool_true)
			{ 
				zBridgeClient_effect_main_region_Play_tr2(handle);
			}  else
			{
				if (zBridgeClient_check_main_region_Play_tr3_tr3(handle) == bool_true)
				{ 
					zBridgeClient_effect_main_region_Play_tr3(handle);
				}  else
				{
					if (zBridgeClient_check_main_region_Play_tr4_tr4(handle) == bool_true)
					{ 
						zBridgeClient_effect_main_region_Play_tr4(handle);
					}  else
					{
						if (zBridgeClient_check_main_region_Play_tr5_tr5(handle) == bool_true)
						{ 
							zBridgeClient_effect_main_region_Play_tr5(handle);
						} 
					}
				}
			}
		}
	}
}

/* The reactions of state WaitLeader. */
static void zBridgeClient_react_main_region_WaitLeader(ZBridgeClient* handle)
{
	/* The reactions of state WaitLeader. */
	if (zBridgeClient_check_main_region_WaitLeader_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_WaitLeader_tr0(handle);
	} 
}

/* The reactions of state SyncSB. */
static void zBridgeClient_react_main_region_SyncSB(ZBridgeClient* handle)
{
	/* The reactions of state SyncSB. */
	if (zBridgeClient_check_main_region_SyncSB_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_SyncSB_tr0(handle);
	}  else
	{
		if (zBridgeClient_check_main_region_SyncSB_tr1_tr1(handle) == bool_true)
		{ 
			zBridgeClient_effect_main_region_SyncSB_tr1(handle);
		} 
	}
}

/* The reactions of state SyncLeader. */
static void zBridgeClient_react_main_region_SyncLeader(ZBridgeClient* handle)
{
	/* The reactions of state SyncLeader. */
	if (zBridgeClient_check_main_region_SyncLeader_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_SyncLeader_tr0(handle);
	}  else
	{
		if (zBridgeClient_check_main_region_SyncLeader_tr1_tr1(handle) == bool_true)
		{ 
			zBridgeClient_effect_main_region_SyncLeader_tr1(handle);
		}  else
		{
			if (zBridgeClient_check_main_region_SyncLeader_tr2_tr2(handle) == bool_true)
			{ 
				zBridgeClient_effect_main_region_SyncLeader_tr2(handle);
			}  else
			{
				if (zBridgeClient_check_main_region_SyncLeader_tr3_tr3(handle) == bool_true)
				{ 
					zBridgeClient_effect_main_region_SyncLeader_tr3(handle);
				}  else
				{
					if (zBridgeClient_check_main_region_SyncLeader_tr4_tr4(handle) == bool_true)
					{ 
						zBridgeClient_effect_main_region_SyncLeader_tr4(handle);
					}  else
					{
						if (zBridgeClient_check_main_region_SyncLeader_tr5_tr5(handle) == bool_true)
						{ 
							zBridgeClient_effect_main_region_SyncLeader_tr5(handle);
						} 
					}
				}
			}
		}
	}
}

/* The reactions of state SyncAuction. */
static void zBridgeClient_react_main_region_SyncAuction(ZBridgeClient* handle)
{
	/* The reactions of state SyncAuction. */
	if (zBridgeClient_check_main_region_SyncAuction_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_SyncAuction_tr0(handle);
	}  else
	{
		if (zBridgeClient_check_main_region_SyncAuction_tr1_tr1(handle) == bool_true)
		{ 
			zBridgeClient_effect_main_region_SyncAuction_tr1(handle);
		}  else
		{
			if (zBridgeClient_check_main_region_SyncAuction_tr2_tr2(handle) == bool_true)
			{ 
				zBridgeClient_effect_main_region_SyncAuction_tr2(handle);
			} 
		}
	}
}

/* The reactions of state SyncPlay. */
static void zBridgeClient_react_main_region_SyncPlay(ZBridgeClient* handle)
{
	/* The reactions of state SyncPlay. */
	if (zBridgeClient_check_main_region_SyncPlay_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_SyncPlay_tr0(handle);
	}  else
	{
		if (zBridgeClient_check_main_region_SyncPlay_tr1_tr1(handle) == bool_true)
		{ 
			zBridgeClient_effect_main_region_SyncPlay_tr1(handle);
		}  else
		{
			if (zBridgeClient_check_main_region_SyncPlay_tr2_tr2(handle) == bool_true)
			{ 
				zBridgeClient_effect_main_region_SyncPlay_tr2(handle);
			} 
		}
	}
}

/* The reactions of state SyncReplay. */
static void zBridgeClient_react_main_region_SyncReplay(ZBridgeClient* handle)
{
	/* The reactions of state SyncReplay. */
	if (zBridgeClient_check_main_region_SyncReplay_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region_SyncReplay_tr0(handle);
	} 
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_0(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	if (zBridgeClient_check_main_region__choice_0_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region__choice_0_tr0(handle);
	}  else
	{
		zBridgeClient_effect_main_region__choice_0_tr1(handle);
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_1(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	if (zBridgeClient_check_main_region__choice_1_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region__choice_1_tr0(handle);
	}  else
	{
		zBridgeClient_effect_main_region__choice_1_tr1(handle);
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_2(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	if (zBridgeClient_check_main_region__choice_2_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region__choice_2_tr0(handle);
	}  else
	{
		if (zBridgeClient_check_main_region__choice_2_tr1_tr1(handle) == bool_true)
		{ 
			zBridgeClient_effect_main_region__choice_2_tr1(handle);
		}  else
		{
			zBridgeClient_effect_main_region__choice_2_tr2(handle);
		}
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_3(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	if (zBridgeClient_check_main_region__choice_3_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region__choice_3_tr0(handle);
	}  else
	{
		zBridgeClient_effect_main_region__choice_3_tr1(handle);
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_4(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	if (zBridgeClient_check_main_region__choice_4_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region__choice_4_tr0(handle);
	}  else
	{
		zBridgeClient_effect_main_region__choice_4_tr1(handle);
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_5(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	if (zBridgeClient_check_main_region__choice_5_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region__choice_5_tr0(handle);
	}  else
	{
		zBridgeClient_effect_main_region__choice_5_tr1(handle);
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_6(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	if (zBridgeClient_check_main_region__choice_6_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region__choice_6_tr0(handle);
	}  else
	{
		zBridgeClient_effect_main_region__choice_6_tr1(handle);
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_7(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	if (zBridgeClient_check_main_region__choice_7_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region__choice_7_tr0(handle);
	}  else
	{
		zBridgeClient_effect_main_region__choice_7_tr1(handle);
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_8(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	zBridgeClient_effect_main_region__choice_8_tr0(handle);
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_9(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	if (zBridgeClient_check_main_region__choice_9_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region__choice_9_tr0(handle);
	}  else
	{
		zBridgeClient_effect_main_region__choice_9_tr1(handle);
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_10(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	if (zBridgeClient_check_main_region__choice_10_tr1_tr1(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region__choice_10_tr1(handle);
	}  else
	{
		zBridgeClient_effect_main_region__choice_10_tr0(handle);
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_11(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	if (zBridgeClient_check_main_region__choice_11_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region__choice_11_tr0(handle);
	}  else
	{
		zBridgeClient_effect_main_region__choice_11_tr1(handle);
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_12(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	if (zBridgeClient_check_main_region__choice_12_tr1_tr1(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region__choice_12_tr1(handle);
	}  else
	{
		zBridgeClient_effect_main_region__choice_12_tr0(handle);
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_13(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	if (zBridgeClient_check_main_region__choice_13_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClient_effect_main_region__choice_13_tr0(handle);
	}  else
	{
		zBridgeClient_effect_main_region__choice_13_tr1(handle);
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_14(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	zBridgeClient_effect_main_region__choice_14_tr0(handle);
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_15(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	zBridgeClient_effect_main_region__choice_15_tr0(handle);
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_16(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	zBridgeClient_effect_main_region__choice_16_tr0(handle);
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_17(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	zBridgeClient_effect_main_region__choice_17_tr0(handle);
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_18(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	zBridgeClient_effect_main_region__choice_18_tr0(handle);
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_19(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	zBridgeClient_effect_main_region__choice_19_tr0(handle);
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_20(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	zBridgeClient_effect_main_region__choice_20_tr0(handle);
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_21(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	zBridgeClient_effect_main_region__choice_21_tr0(handle);
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_22(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	zBridgeClient_effect_main_region__choice_22_tr0(handle);
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__choice_23(ZBridgeClient* handle)
{
	/* The reactions of state null. */
	zBridgeClient_effect_main_region__choice_23_tr0(handle);
}

/* Default react sequence for initial entry  */
static void zBridgeClient_react_main_region__entry_Default(ZBridgeClient* handle)
{
	/* Default react sequence for initial entry  */
	zBridgeClient_enseq_main_region_Connecting_default(handle);
}


