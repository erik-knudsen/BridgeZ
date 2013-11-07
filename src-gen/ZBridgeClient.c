
#include <stdlib.h>
#include <string.h>
#include "sc_types.h"
#include "ZBridgeClient.h"
/*! \file Implementation of the state machine 'ZBridgeClient'
*/

// prototypes of all internal functions

static void zBridgeClient_entryaction(ZBridgeClient* handle);
static void zBridgeClient_exitaction(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Connecting(ZBridgeClient* handle);
static void zBridgeClient_react_main_region__final_0(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Seated(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_StartOfBoard(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Info(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Cards(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Bid(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Lead(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_DummyCards(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Play(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_WaitLeader(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Exit1(ZBridgeClient* handle);
static void zBridgeClient_react_main_region_Exit2(ZBridgeClient* handle);
static void clearInEvents(ZBridgeClient* handle);
static void clearOutEvents(ZBridgeClient* handle);


void zBridgeClient_init(ZBridgeClient* handle)
{
	int i;

	for (i = 0; i < ZBRIDGECLIENT_MAX_ORTHOGONAL_STATES; ++i)
		handle->stateConfVector[i] = ZBridgeClient_last_state;
	
	
	handle->stateConfVectorPosition = 0;

clearInEvents(handle);
clearOutEvents(handle);

	// TODO: initialize all events ...

	{
		/* Default init sequence for statechart ZBridgeClient */
		handle->internal.BID_NONE = - 1;
		handle->internal.BID_PASS = 0;
		handle->internal.BID_DOUBLE = 36;
		handle->internal.BID_REDOUBLE = 37;
	}

}

void zBridgeClient_enter(ZBridgeClient* handle)
{
	{
		/* Default enter sequence for statechart ZBridgeClient */
		zBridgeClient_entryaction(handle);
		{
			/* 'default' enter sequence for region main region */
			{
				/* Default react sequence for initial entry  */
				{
					/* 'default' enter sequence for state Connecting */
					{
						/* Entry action for state 'Connecting'. */
						handle->iface.connect_raised = bool_true;
					}
					handle->stateConfVector[0] = ZBridgeClient_main_region_Connecting;
					handle->stateConfVectorPosition = 0;
				}
			}
		}
	}
}

void zBridgeClient_exit(ZBridgeClient* handle)
{
	{
		/* Default exit sequence for statechart ZBridgeClient */
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of main region) at position 0... */
			switch(handle->stateConfVector[ 0 ]) {
				case ZBridgeClient_main_region_Connecting : {
					{
						/* Default exit sequence for state Connecting */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClient_main_region__final_ : {
					{
						/* Default exit sequence for final state. */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClient_main_region_Seated : {
					{
						/* Default exit sequence for state Seated */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClient_main_region_StartOfBoard : {
					{
						/* Default exit sequence for state StartOfBoard */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClient_main_region_Info : {
					{
						/* Default exit sequence for state Info */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClient_main_region_Cards : {
					{
						/* Default exit sequence for state Cards */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClient_main_region_Bid : {
					{
						/* Default exit sequence for state Bid */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClient_main_region_Lead : {
					{
						/* Default exit sequence for state Lead */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClient_main_region_DummyCards : {
					{
						/* Default exit sequence for state DummyCards */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClient_main_region_Play : {
					{
						/* Default exit sequence for state Play */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClient_main_region_WaitLeader : {
					{
						/* Default exit sequence for state WaitLeader */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClient_main_region_Exit1 : {
					{
						/* Default exit sequence for state Exit1 */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClient_main_region_Exit2 : {
					{
						/* Default exit sequence for state Exit2 */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				default: break;
			}
		}
		zBridgeClient_exitaction(handle);
	}
}

static void clearInEvents(ZBridgeClient* handle) {
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
}

static void clearOutEvents(ZBridgeClient* handle) {
	handle->iface.connect_raised = bool_false;
	handle->iface.rTNames_raised = bool_false;
	handle->iface.rSBoard_raised = bool_false;
	handle->iface.rDealInfo_raised = bool_false;
	handle->iface.rCards_raised = bool_false;
	handle->iface.giveBid_raised = bool_false;
	handle->iface.rBid_raised = bool_false;
	handle->iface.bidInfo_raised = bool_false;
	handle->iface.undoBid_raised = bool_false;
	handle->iface.readyForDummy_raised = bool_false;
	handle->iface.clientPlays_raised = bool_false;
	handle->iface.readyForDummyCards_raised = bool_false;
	handle->iface.readyForPlayer_raised = bool_false;
	handle->iface.getLeader_raised = bool_false;
	handle->iface.undoTrick_raised = bool_false;
}

void zBridgeClient_runCycle(ZBridgeClient* handle) {
	
	clearOutEvents(handle);
	
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < ZBRIDGECLIENT_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++) {
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition]) {
		case ZBridgeClient_main_region_Connecting : {
			zBridgeClient_react_main_region_Connecting(handle);
			break;
		}
		case ZBridgeClient_main_region__final_ : {
			zBridgeClient_react_main_region__final_0(handle);
			break;
		}
		case ZBridgeClient_main_region_Seated : {
			zBridgeClient_react_main_region_Seated(handle);
			break;
		}
		case ZBridgeClient_main_region_StartOfBoard : {
			zBridgeClient_react_main_region_StartOfBoard(handle);
			break;
		}
		case ZBridgeClient_main_region_Info : {
			zBridgeClient_react_main_region_Info(handle);
			break;
		}
		case ZBridgeClient_main_region_Cards : {
			zBridgeClient_react_main_region_Cards(handle);
			break;
		}
		case ZBridgeClient_main_region_Bid : {
			zBridgeClient_react_main_region_Bid(handle);
			break;
		}
		case ZBridgeClient_main_region_Lead : {
			zBridgeClient_react_main_region_Lead(handle);
			break;
		}
		case ZBridgeClient_main_region_DummyCards : {
			zBridgeClient_react_main_region_DummyCards(handle);
			break;
		}
		case ZBridgeClient_main_region_Play : {
			zBridgeClient_react_main_region_Play(handle);
			break;
		}
		case ZBridgeClient_main_region_WaitLeader : {
			zBridgeClient_react_main_region_WaitLeader(handle);
			break;
		}
		case ZBridgeClient_main_region_Exit1 : {
			zBridgeClient_react_main_region_Exit1(handle);
			break;
		}
		case ZBridgeClient_main_region_Exit2 : {
			zBridgeClient_react_main_region_Exit2(handle);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
}


sc_boolean zBridgeClient_isActive(ZBridgeClient* handle, ZBridgeClientStates state) {
	switch (state) {
		case ZBridgeClient_main_region_Connecting : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Connecting
			);
		case ZBridgeClient_main_region__final_ : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region__final_
			);
		case ZBridgeClient_main_region_Seated : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Seated
			);
		case ZBridgeClient_main_region_StartOfBoard : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_StartOfBoard
			);
		case ZBridgeClient_main_region_Info : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Info
			);
		case ZBridgeClient_main_region_Cards : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Cards
			);
		case ZBridgeClient_main_region_Bid : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Bid
			);
		case ZBridgeClient_main_region_Lead : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Lead
			);
		case ZBridgeClient_main_region_DummyCards : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_DummyCards
			);
		case ZBridgeClient_main_region_Play : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Play
			);
		case ZBridgeClient_main_region_WaitLeader : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_WaitLeader
			);
		case ZBridgeClient_main_region_Exit1 : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Exit1
			);
		case ZBridgeClient_main_region_Exit2 : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClient_main_region_Exit2
			);
		default: return bool_false;
	}
}

void zBridgeClientIface_raise_endOfSession(ZBridgeClient* handle) {
	handle->iface.endOfSession_raised = bool_true;
}
void zBridgeClientIface_raise_connectError(ZBridgeClient* handle) {
	handle->iface.connectError_raised = bool_true;
}
void zBridgeClientIface_raise_seated(ZBridgeClient* handle) {
	handle->iface.seated_raised = bool_true;
}
void zBridgeClientIface_raise_teamNames(ZBridgeClient* handle) {
	handle->iface.teamNames_raised = bool_true;
}
void zBridgeClientIface_raise_startOfBoard(ZBridgeClient* handle) {
	handle->iface.startOfBoard_raised = bool_true;
}
void zBridgeClientIface_raise_dealInfo(ZBridgeClient* handle) {
	handle->iface.dealInfo_raised = bool_true;
}
void zBridgeClientIface_raise_cards(ZBridgeClient* handle) {
	handle->iface.cards_raised = bool_true;
}
void zBridgeClientIface_raise_undo(ZBridgeClient* handle, null value) {
	handle->iface.undo_value = value;
	handle->iface.undo_raised = bool_true;
}
void zBridgeClientIface_raise_bidDone(ZBridgeClient* handle, null value) {
	handle->iface.bidDone_value = value;
	handle->iface.bidDone_raised = bool_true;
}
void zBridgeClientIface_raise_playerPlays(ZBridgeClient* handle, null value) {
	handle->iface.playerPlays_value = value;
	handle->iface.playerPlays_raised = bool_true;
}
void zBridgeClientIface_raise_playerToLead(ZBridgeClient* handle) {
	handle->iface.playerToLead_raised = bool_true;
}
void zBridgeClientIface_raise_dummyToLead(ZBridgeClient* handle) {
	handle->iface.dummyToLead_raised = bool_true;
}
void zBridgeClientIface_raise_dummyCards(ZBridgeClient* handle) {
	handle->iface.dummyCards_raised = bool_true;
}
void zBridgeClientIface_raise_newLeader(ZBridgeClient* handle, null value) {
	handle->iface.newLeader_value = value;
	handle->iface.newLeader_raised = bool_true;
}

sc_boolean zBridgeClientIface_israised_connect(ZBridgeClient* handle) {
	return handle->iface.connect_raised;
}
sc_boolean zBridgeClientIface_israised_rTNames(ZBridgeClient* handle) {
	return handle->iface.rTNames_raised;
}
sc_boolean zBridgeClientIface_israised_rSBoard(ZBridgeClient* handle) {
	return handle->iface.rSBoard_raised;
}
sc_boolean zBridgeClientIface_israised_rDealInfo(ZBridgeClient* handle) {
	return handle->iface.rDealInfo_raised;
}
sc_boolean zBridgeClientIface_israised_rCards(ZBridgeClient* handle) {
	return handle->iface.rCards_raised;
}
sc_boolean zBridgeClientIface_israised_giveBid(ZBridgeClient* handle) {
	return handle->iface.giveBid_raised;
}
sc_boolean zBridgeClientIface_israised_rBid(ZBridgeClient* handle) {
	return handle->iface.rBid_raised;
}
sc_boolean zBridgeClientIface_israised_bidInfo(ZBridgeClient* handle) {
	return handle->iface.bidInfo_raised;
}
 zBridgeClientIface_get_bidInfo_value(ZBridgeClient* handle) {
	//TODO: Check if event is not raised
	return handle->iface.bidInfo_value;
}
sc_boolean zBridgeClientIface_israised_undoBid(ZBridgeClient* handle) {
	return handle->iface.undoBid_raised;
}
 zBridgeClientIface_get_undoBid_value(ZBridgeClient* handle) {
	//TODO: Check if event is not raised
	return handle->iface.undoBid_value;
}
sc_boolean zBridgeClientIface_israised_readyForDummy(ZBridgeClient* handle) {
	return handle->iface.readyForDummy_raised;
}
sc_boolean zBridgeClientIface_israised_clientPlays(ZBridgeClient* handle) {
	return handle->iface.clientPlays_raised;
}
sc_boolean zBridgeClientIface_israised_readyForDummyCards(ZBridgeClient* handle) {
	return handle->iface.readyForDummyCards_raised;
}
sc_boolean zBridgeClientIface_israised_readyForPlayer(ZBridgeClient* handle) {
	return handle->iface.readyForPlayer_raised;
}
sc_boolean zBridgeClientIface_israised_getLeader(ZBridgeClient* handle) {
	return handle->iface.getLeader_raised;
}
sc_boolean zBridgeClientIface_israised_undoTrick(ZBridgeClient* handle) {
	return handle->iface.undoTrick_raised;
}
 zBridgeClientIface_get_undoTrick_value(ZBridgeClient* handle) {
	//TODO: Check if event is not raised
	return handle->iface.undoTrick_value;
}

 zBridgeClientIface_get_boardNumber(ZBridgeClient* handle) {
	return handle->iface.boardNumber;
}
void zBridgeClientIface_set_boardNumber(ZBridgeClient* handle,  value) {
	handle->iface.boardNumber = value;
}
 zBridgeClientIface_get_vulnerability(ZBridgeClient* handle) {
	return handle->iface.vulnerability;
}
void zBridgeClientIface_set_vulnerability(ZBridgeClient* handle,  value) {
	handle->iface.vulnerability = value;
}
 zBridgeClientIface_get_client(ZBridgeClient* handle) {
	return handle->iface.client;
}
void zBridgeClientIface_set_client(ZBridgeClient* handle,  value) {
	handle->iface.client = value;
}
 zBridgeClientIface_get_dealer(ZBridgeClient* handle) {
	return handle->iface.dealer;
}
void zBridgeClientIface_set_dealer(ZBridgeClient* handle,  value) {
	handle->iface.dealer = value;
}
 zBridgeClientIface_get_bidder(ZBridgeClient* handle) {
	return handle->iface.bidder;
}
void zBridgeClientIface_set_bidder(ZBridgeClient* handle,  value) {
	handle->iface.bidder = value;
}
 zBridgeClientIface_get_bidVal(ZBridgeClient* handle) {
	return handle->iface.bidVal;
}
void zBridgeClientIface_set_bidVal(ZBridgeClient* handle,  value) {
	handle->iface.bidVal = value;
}
 zBridgeClientIface_get_lastBid(ZBridgeClient* handle) {
	return handle->iface.lastBid;
}
void zBridgeClientIface_set_lastBid(ZBridgeClient* handle,  value) {
	handle->iface.lastBid = value;
}
 zBridgeClientIface_get_bidDouble(ZBridgeClient* handle) {
	return handle->iface.bidDouble;
}
void zBridgeClientIface_set_bidDouble(ZBridgeClient* handle,  value) {
	handle->iface.bidDouble = value;
}
 zBridgeClientIface_get_bidEnable(ZBridgeClient* handle) {
	return handle->iface.bidEnable;
}
void zBridgeClientIface_set_bidEnable(ZBridgeClient* handle,  value) {
	handle->iface.bidEnable = value;
}
 zBridgeClientIface_get_declarer(ZBridgeClient* handle) {
	return handle->iface.declarer;
}
void zBridgeClientIface_set_declarer(ZBridgeClient* handle,  value) {
	handle->iface.declarer = value;
}
 zBridgeClientIface_get_dummy(ZBridgeClient* handle) {
	return handle->iface.dummy;
}
void zBridgeClientIface_set_dummy(ZBridgeClient* handle,  value) {
	handle->iface.dummy = value;
}
 zBridgeClientIface_get_noTrick(ZBridgeClient* handle) {
	return handle->iface.noTrick;
}
void zBridgeClientIface_set_noTrick(ZBridgeClient* handle,  value) {
	handle->iface.noTrick = value;
}
 zBridgeClientIface_get_player(ZBridgeClient* handle) {
	return handle->iface.player;
}
void zBridgeClientIface_set_player(ZBridgeClient* handle,  value) {
	handle->iface.player = value;
}
 zBridgeClientIface_get_cardVal(ZBridgeClient* handle) {
	return handle->iface.cardVal;
}
void zBridgeClientIface_set_cardVal(ZBridgeClient* handle,  value) {
	handle->iface.cardVal = value;
}
		
// implementations of all internal functions

/* Entry action for statechart 'ZBridgeClient'. */
static void zBridgeClient_entryaction(ZBridgeClient* handle) {
	{
		/* Entry action for statechart 'ZBridgeClient'. */
	}
}

/* Exit action for state 'ZBridgeClient'. */
static void zBridgeClient_exitaction(ZBridgeClient* handle) {
	{
		/* Exit action for state 'ZBridgeClient'. */
	}
}

/* The reactions of state Connecting. */
static void zBridgeClient_react_main_region_Connecting(ZBridgeClient* handle) {
	{
		/* The reactions of state Connecting. */
		if (handle->iface.connectError_raised) { 
			{
				/* Default exit sequence for state Connecting */
				handle->stateConfVector[0] = ZBridgeClient_last_state;
				handle->stateConfVectorPosition = 0;
			}
			{
				/* Default enter sequence for state null */
				handle->stateConfVector[0] = ZBridgeClient_main_region__final_;
				handle->stateConfVectorPosition = 0;
			}
		}  else {
			if (handle->iface.seated_raised) { 
				{
					/* Default exit sequence for state Connecting */
					handle->stateConfVector[0] = ZBridgeClient_last_state;
					handle->stateConfVectorPosition = 0;
				}
				handle->iface.rTNames_raised = bool_true;
				{
					/* 'default' enter sequence for state Seated */
					handle->stateConfVector[0] = ZBridgeClient_main_region_Seated;
					handle->stateConfVectorPosition = 0;
				}
			} 
		}
	}
}

/* The reactions of state null. */
static void zBridgeClient_react_main_region__final_0(ZBridgeClient* handle) {
	{
		/* The reactions of state null. */
	}
}

/* The reactions of state Seated. */
static void zBridgeClient_react_main_region_Seated(ZBridgeClient* handle) {
	{
		/* The reactions of state Seated. */
		if (handle->iface.teamNames_raised) { 
			{
				/* Default exit sequence for state Seated */
				handle->stateConfVector[0] = ZBridgeClient_last_state;
				handle->stateConfVectorPosition = 0;
			}
			handle->iface.rSBoard_raised = bool_true;
			{
				/* 'default' enter sequence for state StartOfBoard */
				handle->stateConfVector[0] = ZBridgeClient_main_region_StartOfBoard;
				handle->stateConfVectorPosition = 0;
			}
		} 
	}
}

/* The reactions of state StartOfBoard. */
static void zBridgeClient_react_main_region_StartOfBoard(ZBridgeClient* handle) {
	{
		/* The reactions of state StartOfBoard. */
		if (handle->iface.startOfBoard_raised) { 
			{
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[0] = ZBridgeClient_last_state;
				handle->stateConfVectorPosition = 0;
			}
			handle->iface.rDealInfo_raised = bool_true;
			{
				/* 'default' enter sequence for state Info */
				handle->stateConfVector[0] = ZBridgeClient_main_region_Info;
				handle->stateConfVectorPosition = 0;
			}
		} 
	}
}

/* The reactions of state Info. */
static void zBridgeClient_react_main_region_Info(ZBridgeClient* handle) {
	{
		/* The reactions of state Info. */
		if (handle->iface.dealInfo_raised) { 
			{
				/* Default exit sequence for state Info */
				handle->stateConfVector[0] = ZBridgeClient_last_state;
				handle->stateConfVectorPosition = 0;
			}
			handle->iface.rCards_raised = bool_true;
			{
				/* 'default' enter sequence for state Cards */
				handle->stateConfVector[0] = ZBridgeClient_main_region_Cards;
				handle->stateConfVectorPosition = 0;
			}
		} 
	}
}

/* The reactions of state Cards. */
static void zBridgeClient_react_main_region_Cards(ZBridgeClient* handle) {
	{
		/* The reactions of state Cards. */
		if (handle->iface.cards_raised) { 
			{
				/* Default exit sequence for state Cards */
				handle->stateConfVector[0] = ZBridgeClient_last_state;
				handle->stateConfVectorPosition = 0;
			}
			handle->iface.bidder = handle->iface.dealer;
			handle->internal.firstBidRound = bool_true;
			handle->iface.lastBid = handle->internal.BID_NONE;
			handle->internal.noPasses = 0;
			{
				/* The reactions of state null. */
				if (handle->iface.bidder == handle->iface.client) { 
					handle->iface.giveBid_raised = bool_true;
					{
						/* 'default' enter sequence for state Bid */
						handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
						handle->stateConfVectorPosition = 0;
					}
				}  else {
					handle->iface.rBid_raised = bool_true;
					{
						/* 'default' enter sequence for state Bid */
						handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
						handle->stateConfVectorPosition = 0;
					}
				}
			}
		}  else {
			if (handle->iface.startOfBoard_raised) { 
				{
					/* Default exit sequence for state Cards */
					handle->stateConfVector[0] = ZBridgeClient_last_state;
					handle->stateConfVectorPosition = 0;
				}
				handle->iface.rDealInfo_raised = bool_true;
				{
					/* 'default' enter sequence for state Info */
					handle->stateConfVector[0] = ZBridgeClient_main_region_Info;
					handle->stateConfVectorPosition = 0;
				}
			} 
		}
	}
}

/* The reactions of state Bid. */
static void zBridgeClient_react_main_region_Bid(ZBridgeClient* handle) {
	{
		/* The reactions of state Bid. */
		if (handle->iface.bidDone_raised) { 
			{
				/* Default exit sequence for state Bid */
				handle->stateConfVector[0] = ZBridgeClient_last_state;
				handle->stateConfVectorPosition = 0;
			}
			handle->iface.bidVal = handle->iface.bidDone_value;
			handle->internal.curBidder = handle->iface.bidder;
			handle->iface.bidder += 1;
			handle->iface.bidder &= 3;
			{
				/* The reactions of state null. */
				if (handle->iface.bidVal == handle->internal.BID_PASS) { 
					handle->internal.noPasses += 1;
					{
						/* The reactions of state null. */
						if ((handle->internal.noPasses == 4) && handle->internal.firstBidRound) { 
							{
								/* 'default' enter sequence for state Bid */
								handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
								handle->stateConfVectorPosition = 0;
							}
						}  else {
							if ((handle->internal.noPasses == 3) && ! handle->internal.firstBidRound) { 
								handle->iface.declarer = handle->internal.lastBidder;
								handle->iface.dummy = (handle->iface.declarer + 2) & 3;
								handle->internal.firstTrick = bool_true;
								handle->iface.noTrick = 0;
								handle->internal.leader = (handle->iface.declarer + 1) & 3;
								handle->internal.playNo = 0;
								handle->iface.player = handle->internal.leader;
								handle->iface.bidInfo_value = handle->internal.leader;
								handle->iface.bidInfo_raised = bool_true;
								{
									/* The reactions of state null. */
									if (((handle->iface.player == handle->iface.dummy) && (handle->iface.client == handle->iface.declarer)) || ((handle->iface.player != handle->iface.dummy) && (handle->iface.client == handle->iface.player))) { 
										{
											/* The reactions of state null. */
											if (handle->internal.playNo == 0) { 
												{
													/* 'default' enter sequence for state Lead */
													handle->stateConfVector[0] = ZBridgeClient_main_region_Lead;
													handle->stateConfVectorPosition = 0;
												}
											}  else {
												{
													/* The reactions of state null. */
													handle->iface.clientPlays_raised = bool_true;
													{
														/* 'default' enter sequence for state Play */
														handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
														handle->stateConfVectorPosition = 0;
													}
												}
											}
										}
									}  else {
										{
											/* The reactions of state null. */
											if (handle->iface.player == handle->iface.dummy) { 
												handle->iface.readyForDummy_raised = bool_true;
												{
													/* 'default' enter sequence for state Play */
													handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
													handle->stateConfVectorPosition = 0;
												}
											}  else {
												handle->iface.readyForPlayer_raised = bool_true;
												{
													/* 'default' enter sequence for state Play */
													handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
													handle->stateConfVectorPosition = 0;
												}
											}
										}
									}
								}
							}  else {
								{
									/* The reactions of state null. */
									if (handle->iface.bidder == handle->iface.client) { 
										handle->iface.giveBid_raised = bool_true;
										{
											/* 'default' enter sequence for state Bid */
											handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
											handle->stateConfVectorPosition = 0;
										}
									}  else {
										handle->iface.rBid_raised = bool_true;
										{
											/* 'default' enter sequence for state Bid */
											handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
											handle->stateConfVectorPosition = 0;
										}
									}
								}
							}
						}
					}
				}  else {
					handle->internal.firstBidRound = bool_false;
					handle->internal.noPasses = 0;
					{
						/* The reactions of state null. */
						if ((handle->iface.bidVal == handle->internal.BID_REDOUBLE) || (handle->iface.bidVal == handle->internal.BID_DOUBLE)) { 
							handle->iface.bidDouble = handle->iface.bidVal;
							{
								/* The reactions of state null. */
								if (((handle->internal.curBidder == ((handle->iface.client + 3 & 3))) || (handle->internal.curBidder == ((handle->iface.client + 1) & 3))) && (handle->iface.bidVal == handle->internal.BID_DOUBLE)) { 
									handle->iface.bidEnable = handle->internal.BID_REDOUBLE;
									{
										/* The reactions of state null. */
										if (handle->iface.bidder == handle->iface.client) { 
											handle->iface.giveBid_raised = bool_true;
											{
												/* 'default' enter sequence for state Bid */
												handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
												handle->stateConfVectorPosition = 0;
											}
										}  else {
											handle->iface.rBid_raised = bool_true;
											{
												/* 'default' enter sequence for state Bid */
												handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
												handle->stateConfVectorPosition = 0;
											}
										}
									}
								}  else {
									handle->iface.bidEnable = handle->internal.BID_NONE;
									{
										/* The reactions of state null. */
										if (handle->iface.bidder == handle->iface.client) { 
											handle->iface.giveBid_raised = bool_true;
											{
												/* 'default' enter sequence for state Bid */
												handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
												handle->stateConfVectorPosition = 0;
											}
										}  else {
											handle->iface.rBid_raised = bool_true;
											{
												/* 'default' enter sequence for state Bid */
												handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
												handle->stateConfVectorPosition = 0;
											}
										}
									}
								}
							}
						}  else {
							handle->internal.lastBidder = handle->internal.curBidder;
							handle->iface.lastBid = handle->iface.bidVal;
							handle->iface.bidDouble = handle->internal.BID_NONE;
							{
								/* The reactions of state null. */
								if ((handle->internal.curBidder == ((handle->iface.client + 3) & 3)) || (handle->internal.curBidder == ((handle->iface.client + 1) & 3))) { 
									handle->iface.bidEnable = handle->internal.BID_DOUBLE;
									{
										/* The reactions of state null. */
										if (handle->iface.bidder == handle->iface.client) { 
											handle->iface.giveBid_raised = bool_true;
											{
												/* 'default' enter sequence for state Bid */
												handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
												handle->stateConfVectorPosition = 0;
											}
										}  else {
											handle->iface.rBid_raised = bool_true;
											{
												/* 'default' enter sequence for state Bid */
												handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
												handle->stateConfVectorPosition = 0;
											}
										}
									}
								}  else {
									handle->iface.bidEnable = handle->internal.BID_NONE;
									{
										/* The reactions of state null. */
										if (handle->iface.bidder == handle->iface.client) { 
											handle->iface.giveBid_raised = bool_true;
											{
												/* 'default' enter sequence for state Bid */
												handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
												handle->stateConfVectorPosition = 0;
											}
										}  else {
											handle->iface.rBid_raised = bool_true;
											{
												/* 'default' enter sequence for state Bid */
												handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
												handle->stateConfVectorPosition = 0;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}  else {
			if (handle->iface.undo_value >= 0 && handle->iface.undo_raised) { 
				{
					/* Default exit sequence for state Bid */
					handle->stateConfVector[0] = ZBridgeClient_last_state;
					handle->stateConfVectorPosition = 0;
				}
				handle->internal.lastBidder = (handle->iface.undo_value + handle->internal.leader) & 3;
				handle->iface.lastBid = handle->iface.bidVal;
				handle->iface.bidDouble = handle->internal.BID_NONE;
				handle->internal.noPasses = 0;
				handle->iface.bidder = (handle->internal.lastBidder + 1) & 3;
				handle->iface.undoBid_value = handle->iface.undo_value;
				handle->iface.undoBid_raised = bool_true;
				{
					/* The reactions of state null. */
					if (handle->iface.bidder == handle->iface.client) { 
						handle->iface.giveBid_raised = bool_true;
						{
							/* 'default' enter sequence for state Bid */
							handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
							handle->stateConfVectorPosition = 0;
						}
					}  else {
						handle->iface.rBid_raised = bool_true;
						{
							/* 'default' enter sequence for state Bid */
							handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
							handle->stateConfVectorPosition = 0;
						}
					}
				}
			}  else {
				if (handle->iface.undo_value == - 1 && handle->iface.undo_raised) { 
					{
						/* Default exit sequence for state Bid */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					handle->iface.bidder = handle->iface.dealer;
					handle->internal.firstBidRound = bool_true;
					handle->iface.lastBid = handle->internal.BID_NONE;
					handle->internal.noPasses = 0;
					handle->iface.undoBid_value = handle->iface.undo_value;
					handle->iface.undoBid_raised = bool_true;
					{
						/* The reactions of state null. */
						if (handle->iface.bidder == handle->iface.client) { 
							handle->iface.giveBid_raised = bool_true;
							{
								/* 'default' enter sequence for state Bid */
								handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
								handle->stateConfVectorPosition = 0;
							}
						}  else {
							handle->iface.rBid_raised = bool_true;
							{
								/* 'default' enter sequence for state Bid */
								handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
								handle->stateConfVectorPosition = 0;
							}
						}
					}
				}  else {
					if (handle->iface.startOfBoard_raised) { 
						{
							/* Default exit sequence for state Bid */
							handle->stateConfVector[0] = ZBridgeClient_last_state;
							handle->stateConfVectorPosition = 0;
						}
						handle->iface.rDealInfo_raised = bool_true;
						{
							/* 'default' enter sequence for state Info */
							handle->stateConfVector[0] = ZBridgeClient_main_region_Info;
							handle->stateConfVectorPosition = 0;
						}
					}  else {
						if (handle->iface.endOfSession_raised) { 
							{
								/* Default exit sequence for state Bid */
								handle->stateConfVector[0] = ZBridgeClient_last_state;
								handle->stateConfVectorPosition = 0;
							}
							{
								/* 'default' enter sequence for state Exit1 */
								handle->stateConfVector[0] = ZBridgeClient_main_region_Exit1;
								handle->stateConfVectorPosition = 0;
							}
						} 
					}
				}
			}
		}
	}
}

/* The reactions of state Lead. */
static void zBridgeClient_react_main_region_Lead(ZBridgeClient* handle) {
	{
		/* The reactions of state Lead. */
		if (handle->iface.player != handle->iface.dummy && handle->iface.playerToLead_raised) { 
			{
				/* Default exit sequence for state Lead */
				handle->stateConfVector[0] = ZBridgeClient_last_state;
				handle->stateConfVectorPosition = 0;
			}
			{
				/* The reactions of state null. */
				handle->iface.clientPlays_raised = bool_true;
				{
					/* 'default' enter sequence for state Play */
					handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
					handle->stateConfVectorPosition = 0;
				}
			}
		}  else {
			if (handle->iface.player == handle->iface.dummy && handle->iface.dummyToLead_raised) { 
				{
					/* Default exit sequence for state Lead */
					handle->stateConfVector[0] = ZBridgeClient_last_state;
					handle->stateConfVectorPosition = 0;
				}
				{
					/* The reactions of state null. */
					handle->iface.clientPlays_raised = bool_true;
					{
						/* 'default' enter sequence for state Play */
						handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
						handle->stateConfVectorPosition = 0;
					}
				}
			} 
		}
	}
}

/* The reactions of state DummyCards. */
static void zBridgeClient_react_main_region_DummyCards(ZBridgeClient* handle) {
	{
		/* The reactions of state DummyCards. */
		if (handle->iface.dummyCards_raised) { 
			{
				/* Default exit sequence for state DummyCards */
				handle->stateConfVector[0] = ZBridgeClient_last_state;
				handle->stateConfVectorPosition = 0;
			}
			{
				/* The reactions of state null. */
				if (((handle->iface.player == handle->iface.dummy) && (handle->iface.client == handle->iface.declarer)) || ((handle->iface.player != handle->iface.dummy) && (handle->iface.client == handle->iface.player))) { 
					{
						/* The reactions of state null. */
						if (handle->internal.playNo == 0) { 
							{
								/* 'default' enter sequence for state Lead */
								handle->stateConfVector[0] = ZBridgeClient_main_region_Lead;
								handle->stateConfVectorPosition = 0;
							}
						}  else {
							{
								/* The reactions of state null. */
								handle->iface.clientPlays_raised = bool_true;
								{
									/* 'default' enter sequence for state Play */
									handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
									handle->stateConfVectorPosition = 0;
								}
							}
						}
					}
				}  else {
					{
						/* The reactions of state null. */
						if (handle->iface.player == handle->iface.dummy) { 
							handle->iface.readyForDummy_raised = bool_true;
							{
								/* 'default' enter sequence for state Play */
								handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
								handle->stateConfVectorPosition = 0;
							}
						}  else {
							handle->iface.readyForPlayer_raised = bool_true;
							{
								/* 'default' enter sequence for state Play */
								handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
								handle->stateConfVectorPosition = 0;
							}
						}
					}
				}
			}
		} 
	}
}

/* The reactions of state Play. */
static void zBridgeClient_react_main_region_Play(ZBridgeClient* handle) {
	{
		/* The reactions of state Play. */
		if (handle->iface.playerPlays_raised) { 
			{
				/* Default exit sequence for state Play */
				handle->stateConfVector[0] = ZBridgeClient_last_state;
				handle->stateConfVectorPosition = 0;
			}
			handle->iface.cardVal = handle->iface.playerPlays_value;
			handle->internal.playNo += 1;
			handle->iface.player = (handle->internal.leader + handle->internal.playNo) & 3;
			{
				/* The reactions of state null. */
				if (handle->internal.firstTrick) { 
					handle->internal.firstTrick = bool_false;
					{
						/* The reactions of state null. */
						if (handle->iface.client != handle->iface.dummy) { 
							handle->iface.readyForDummyCards_raised = bool_true;
							{
								/* 'default' enter sequence for state DummyCards */
								handle->stateConfVector[0] = ZBridgeClient_main_region_DummyCards;
								handle->stateConfVectorPosition = 0;
							}
						}  else {
							{
								/* The reactions of state null. */
								if (handle->internal.playNo < 4) { 
									{
										/* The reactions of state null. */
										if (((handle->iface.player == handle->iface.dummy) && (handle->iface.client == handle->iface.declarer)) || ((handle->iface.player != handle->iface.dummy) && (handle->iface.client == handle->iface.player))) { 
											{
												/* The reactions of state null. */
												if (handle->internal.playNo == 0) { 
													{
														/* 'default' enter sequence for state Lead */
														handle->stateConfVector[0] = ZBridgeClient_main_region_Lead;
														handle->stateConfVectorPosition = 0;
													}
												}  else {
													{
														/* The reactions of state null. */
														handle->iface.clientPlays_raised = bool_true;
														{
															/* 'default' enter sequence for state Play */
															handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
															handle->stateConfVectorPosition = 0;
														}
													}
												}
											}
										}  else {
											{
												/* The reactions of state null. */
												if (handle->iface.player == handle->iface.dummy) { 
													handle->iface.readyForDummy_raised = bool_true;
													{
														/* 'default' enter sequence for state Play */
														handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
														handle->stateConfVectorPosition = 0;
													}
												}  else {
													handle->iface.readyForPlayer_raised = bool_true;
													{
														/* 'default' enter sequence for state Play */
														handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
														handle->stateConfVectorPosition = 0;
													}
												}
											}
										}
									}
								}  else {
									handle->iface.noTrick += 1;
									handle->iface.getLeader_raised = bool_true;
									{
										/* 'default' enter sequence for state WaitLeader */
										handle->stateConfVector[0] = ZBridgeClient_main_region_WaitLeader;
										handle->stateConfVectorPosition = 0;
									}
								}
							}
						}
					}
				}  else {
					{
						/* The reactions of state null. */
						if (handle->internal.playNo < 4) { 
							{
								/* The reactions of state null. */
								if (((handle->iface.player == handle->iface.dummy) && (handle->iface.client == handle->iface.declarer)) || ((handle->iface.player != handle->iface.dummy) && (handle->iface.client == handle->iface.player))) { 
									{
										/* The reactions of state null. */
										if (handle->internal.playNo == 0) { 
											{
												/* 'default' enter sequence for state Lead */
												handle->stateConfVector[0] = ZBridgeClient_main_region_Lead;
												handle->stateConfVectorPosition = 0;
											}
										}  else {
											{
												/* The reactions of state null. */
												handle->iface.clientPlays_raised = bool_true;
												{
													/* 'default' enter sequence for state Play */
													handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
													handle->stateConfVectorPosition = 0;
												}
											}
										}
									}
								}  else {
									{
										/* The reactions of state null. */
										if (handle->iface.player == handle->iface.dummy) { 
											handle->iface.readyForDummy_raised = bool_true;
											{
												/* 'default' enter sequence for state Play */
												handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
												handle->stateConfVectorPosition = 0;
											}
										}  else {
											handle->iface.readyForPlayer_raised = bool_true;
											{
												/* 'default' enter sequence for state Play */
												handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
												handle->stateConfVectorPosition = 0;
											}
										}
									}
								}
							}
						}  else {
							handle->iface.noTrick += 1;
							handle->iface.getLeader_raised = bool_true;
							{
								/* 'default' enter sequence for state WaitLeader */
								handle->stateConfVector[0] = ZBridgeClient_main_region_WaitLeader;
								handle->stateConfVectorPosition = 0;
							}
						}
					}
				}
			}
		}  else {
			if (handle->iface.undo_value >= 0 && handle->iface.undo_raised) { 
				{
					/* Default exit sequence for state Play */
					handle->stateConfVector[0] = ZBridgeClient_last_state;
					handle->stateConfVectorPosition = 0;
				}
				handle->iface.noTrick = handle->iface.undo_value;
				handle->internal.playNo = 0;
				handle->internal.firstTrick = (handle->iface.undo_value == 0);
				handle->iface.undoTrick_value = handle->iface.noTrick;
				handle->iface.undoTrick_raised = bool_true;
				{
					/* 'default' enter sequence for state WaitLeader */
					handle->stateConfVector[0] = ZBridgeClient_main_region_WaitLeader;
					handle->stateConfVectorPosition = 0;
				}
			}  else {
				if (handle->iface.undo_value == - 1 && handle->iface.undo_raised) { 
					{
						/* Default exit sequence for state Play */
						handle->stateConfVector[0] = ZBridgeClient_last_state;
						handle->stateConfVectorPosition = 0;
					}
					handle->iface.bidder = handle->iface.dealer;
					handle->internal.firstBidRound = bool_true;
					handle->iface.lastBid = handle->internal.BID_NONE;
					handle->internal.noPasses = 0;
					handle->iface.undoBid_value = handle->iface.undo_value;
					handle->iface.undoBid_raised = bool_true;
					{
						/* The reactions of state null. */
						if (handle->iface.bidder == handle->iface.client) { 
							handle->iface.giveBid_raised = bool_true;
							{
								/* 'default' enter sequence for state Bid */
								handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
								handle->stateConfVectorPosition = 0;
							}
						}  else {
							handle->iface.rBid_raised = bool_true;
							{
								/* 'default' enter sequence for state Bid */
								handle->stateConfVector[0] = ZBridgeClient_main_region_Bid;
								handle->stateConfVectorPosition = 0;
							}
						}
					}
				}  else {
					if (handle->iface.startOfBoard_raised) { 
						{
							/* Default exit sequence for state Play */
							handle->stateConfVector[0] = ZBridgeClient_last_state;
							handle->stateConfVectorPosition = 0;
						}
						handle->iface.rDealInfo_raised = bool_true;
						{
							/* 'default' enter sequence for state Info */
							handle->stateConfVector[0] = ZBridgeClient_main_region_Info;
							handle->stateConfVectorPosition = 0;
						}
					}  else {
						if (handle->iface.endOfSession_raised) { 
							{
								/* Default exit sequence for state Play */
								handle->stateConfVector[0] = ZBridgeClient_last_state;
								handle->stateConfVectorPosition = 0;
							}
							{
								/* 'default' enter sequence for state Exit2 */
								handle->stateConfVector[0] = ZBridgeClient_main_region_Exit2;
								handle->stateConfVectorPosition = 0;
							}
						} 
					}
				}
			}
		}
	}
}

/* The reactions of state WaitLeader. */
static void zBridgeClient_react_main_region_WaitLeader(ZBridgeClient* handle) {
	{
		/* The reactions of state WaitLeader. */
		if (handle->iface.newLeader_raised) { 
			{
				/* Default exit sequence for state WaitLeader */
				handle->stateConfVector[0] = ZBridgeClient_last_state;
				handle->stateConfVectorPosition = 0;
			}
			handle->internal.leader = handle->iface.newLeader_value;
			handle->iface.player = handle->internal.leader;
			handle->internal.playNo = 0;
			{
				/* The reactions of state null. */
				if (handle->iface.noTrick == 13) { 
					{
						/* 'default' enter sequence for state StartOfBoard */
						handle->stateConfVector[0] = ZBridgeClient_main_region_StartOfBoard;
						handle->stateConfVectorPosition = 0;
					}
				}  else {
					{
						/* The reactions of state null. */
						if (((handle->iface.player == handle->iface.dummy) && (handle->iface.client == handle->iface.declarer)) || ((handle->iface.player != handle->iface.dummy) && (handle->iface.client == handle->iface.player))) { 
							{
								/* The reactions of state null. */
								if (handle->internal.playNo == 0) { 
									{
										/* 'default' enter sequence for state Lead */
										handle->stateConfVector[0] = ZBridgeClient_main_region_Lead;
										handle->stateConfVectorPosition = 0;
									}
								}  else {
									{
										/* The reactions of state null. */
										handle->iface.clientPlays_raised = bool_true;
										{
											/* 'default' enter sequence for state Play */
											handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
											handle->stateConfVectorPosition = 0;
										}
									}
								}
							}
						}  else {
							{
								/* The reactions of state null. */
								if (handle->iface.player == handle->iface.dummy) { 
									handle->iface.readyForDummy_raised = bool_true;
									{
										/* 'default' enter sequence for state Play */
										handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
										handle->stateConfVectorPosition = 0;
									}
								}  else {
									handle->iface.readyForPlayer_raised = bool_true;
									{
										/* 'default' enter sequence for state Play */
										handle->stateConfVector[0] = ZBridgeClient_main_region_Play;
										handle->stateConfVectorPosition = 0;
									}
								}
							}
						}
					}
				}
			}
		} 
	}
}

/* The reactions of state Exit1. */
static void zBridgeClient_react_main_region_Exit1(ZBridgeClient* handle) {
	{
		/* The reactions of state Exit1. */
	}
}

/* The reactions of state Exit2. */
static void zBridgeClient_react_main_region_Exit2(ZBridgeClient* handle) {
	{
		/* The reactions of state Exit2. */
	}
}


