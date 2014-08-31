
#include <stdlib.h>
#include <string.h>
#include "sc_types.h"
#include "ZBridgeServer.h"
/*! \file Implementation of the state machine 'ZBridgeServer'
*/

// prototypes of all internal functions

static void zBridgeServer_enact_SequenceImpl(ZBridgeServer* handle);
static void zBridgeServer_exact_SequenceImpl(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_West_Seated(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_West_TeamNames(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_West_StartOfBoard(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_West_Connect(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_North_Seated(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_North_TeamNames(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_North_StartOfBoard(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_North_Connect(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_East_Seated(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_East_TeamNames(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_East_StartOfBoard(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_East_Connect(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_South_Seated(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_South_TeamNames(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_South_StartOfBoard(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Connect_South_Connect(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Deal_West_Info(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Deal_West_Cards(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Deal_West_Sync(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Deal_North_Info(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Deal_North_Cards(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Deal_North_Sync(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Deal_East_Info(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Deal_East_Cards(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Deal_East_Sync(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Deal_South_Info(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Deal_South_Cards(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Deal_South_Sync(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Bidding_West_Wait(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Bidding_West_Sync(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Bidding_North_Wait(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Bidding_North_Sync(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Bidding_East_Wait(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Bidding_East_Sync(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Bidding_South_Wait(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Bidding_South_Sync(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Playing_West_Wait(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Playing_West_Sync(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Playing_North_Wait(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Playing_North_Sync(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Playing_East_Wait(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Playing_East_Sync(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Playing_South_Wait(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Playing_South_Sync(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Exit1(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Exit2(ZBridgeServer* handle);
static void zBridgeServer_react_entry__Exit3(ZBridgeServer* handle);
static void zBridgeServer_react_entry__WaitLeader(ZBridgeServer* handle);
static void zBridgeServer_react_entry__SyncSB(ZBridgeServer* handle);
static void zBridgeServer_react_entry__SyncAuction(ZBridgeServer* handle);
static void zBridgeServer_react_entry__SyncPlay(ZBridgeServer* handle);
static void zBridgeServer_react_entry__SyncLeader(ZBridgeServer* handle);
static void zBridgeServer_react_entry__SyncReplay(ZBridgeServer* handle);
static void zBridgeServer_react_entry___sync0(ZBridgeServer* handle);
static void zBridgeServer_react_entry___sync1(ZBridgeServer* handle);
static void zBridgeServer_react_entry___sync2(ZBridgeServer* handle);
static void zBridgeServer_react_entry___sync3(ZBridgeServer* handle);
static void zBridgeServer_clearInEvents(ZBridgeServer* handle);
static void zBridgeServer_clearOutEvents(ZBridgeServer* handle);


void zBridgeServer_init(ZBridgeServer* handle)
{
	int i;

	for (i = 0; i < ZBRIDGESERVER_MAX_ORTHOGONAL_STATES; ++i)
		handle->stateConfVector[i] = ZBridgeServer_last_state;
	
	
	handle->stateConfVectorPosition = 0;

	zBridgeServer_clearInEvents(handle);
	zBridgeServer_clearOutEvents(handle);

	/* Default init sequence for statechart ZBridgeServer */
	handle->internal.BID_NONE = - 1;
	handle->internal.BID_PASS = 0;
	handle->internal.BID_DOUBLE = 36;
	handle->internal.BID_REDOUBLE = 37;
	handle->internal.W = 0;
	handle->internal.N = 1;
	handle->internal.E = 2;
	handle->internal.S = 3;
	handle->internal.REBID = - 1;
	handle->internal.REPLAY = - 2;
	handle->internal.CT = 1;
	handle->internal.PT = 2;
	handle->internal.westConnected = bool_false;
	handle->internal.northConnected = bool_false;
	handle->internal.eastConnected = bool_false;
	handle->internal.southConnected = bool_false;
	handle->internal.westRTNames = bool_false;
	handle->internal.northRTNames = bool_false;
	handle->internal.eastRTNames = bool_false;
	handle->internal.southRTNames = bool_false;
	handle->internal.westRSBoard = bool_false;
	handle->internal.northRSBoard = bool_false;
	handle->internal.eastRSBoard = bool_false;
	handle->internal.southRSBoard = bool_false;
	handle->internal.westRSBid = bool_false;
	handle->internal.northRSBid = bool_false;
	handle->internal.eastRSBid = bool_false;
	handle->internal.southRSBid = bool_false;
	handle->internal.westBid = bool_false;
	handle->internal.northBid = bool_false;
	handle->internal.eastBid = bool_false;
	handle->internal.southBid = bool_false;
	handle->internal.noBoards = 0;
	handle->internal.westRCard = bool_false;
	handle->internal.northRCard = bool_false;
	handle->internal.eastRCard = bool_false;
	handle->internal.southRCard = bool_false;
	handle->internal.curBidder = 0;
	handle->internal.firstBidRound = bool_false;
	handle->internal.lastBidder = 0;
	handle->internal.noPasses = 0;
	handle->internal.leader = 0;
	handle->internal.playNo = 0;
	handle->iface.noOfBoards = 0;
	handle->iface.name = "";
	handle->iface.nsTeamName = "";
	handle->iface.ewTeamName = "";
	handle->iface.dealer = 0;
	handle->iface.bidVal = 0;
	handle->iface.bidder = 0;
	handle->iface.lastBid = 0;
	handle->iface.bidDouble = 0;
	handle->iface.declarer = 0;
	handle->iface.dummy = 0;
	handle->iface.player = 0;
	handle->iface.noTrick = 0;
	handle->iface.cardVal = 0;

}

void zBridgeServer_enter(ZBridgeServer* handle)
{
	/* Default enter sequence for statechart ZBridgeServer */
	zBridgeServer_enact_SequenceImpl(handle);
	/* 'default' enter sequence for region entry  */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state Connect */
	/* 'default' enter sequence for region West */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state Connect */
	handle->stateConfVector[0] = ZBridgeServer_entry__Connect_West_Connect;
	handle->stateConfVectorPosition = 0;
	/* 'default' enter sequence for region North */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state Connect */
	handle->stateConfVector[1] = ZBridgeServer_entry__Connect_North_Connect;
	handle->stateConfVectorPosition = 1;
	/* 'default' enter sequence for region East */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state Connect */
	handle->stateConfVector[2] = ZBridgeServer_entry__Connect_East_Connect;
	handle->stateConfVectorPosition = 2;
	/* 'default' enter sequence for region South */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state Connect */
	handle->stateConfVector[3] = ZBridgeServer_entry__Connect_South_Connect;
	handle->stateConfVectorPosition = 3;
}

void zBridgeServer_exit(ZBridgeServer* handle)
{
	/* Default exit sequence for statechart ZBridgeServer */
	/* Default exit sequence for region entry  */
	/* Handle exit of all possible states (of entry ) at position 0... */
	switch(handle->stateConfVector[ 0 ]) {
		case ZBridgeServer_entry__Connect_West_Seated : {
			/* Default exit sequence for state Seated */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__Connect_West_TeamNames : {
			/* Default exit sequence for state TeamNames */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__Connect_West_StartOfBoard : {
			/* Default exit sequence for state StartOfBoard */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__Connect_West_Connect : {
			/* Default exit sequence for state Connect */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__Deal_West_Info : {
			/* Default exit sequence for state Info */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__Deal_West_Cards : {
			/* Default exit sequence for state Cards */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__Deal_West_Sync : {
			/* Default exit sequence for state Sync */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__Bidding_West_Wait : {
			/* Default exit sequence for state Wait */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			/* Exit action for state 'Wait'. */
			handle->internal.westBid = bool_true;
			break;
		}
		case ZBridgeServer_entry__Bidding_West_Sync : {
			/* Default exit sequence for state Sync */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__Playing_West_Wait : {
			/* Default exit sequence for state Wait */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			/* Exit action for state 'Wait'. */
			handle->internal.westRCard = bool_true;
			break;
		}
		case ZBridgeServer_entry__Playing_West_Sync : {
			/* Default exit sequence for state Sync */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__Exit1 : {
			/* Default exit sequence for state Exit1 */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__Exit2 : {
			/* Default exit sequence for state Exit2 */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__Exit3 : {
			/* Default exit sequence for state Exit3 */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__WaitLeader : {
			/* Default exit sequence for state WaitLeader */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__SyncSB : {
			/* Default exit sequence for state SyncSB */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__SyncAuction : {
			/* Default exit sequence for state SyncAuction */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__SyncPlay : {
			/* Default exit sequence for state SyncPlay */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__SyncLeader : {
			/* Default exit sequence for state SyncLeader */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServer_entry__SyncReplay : {
			/* Default exit sequence for state SyncReplay */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		default: break;
	}
	/* Handle exit of all possible states (of entry ) at position 1... */
	switch(handle->stateConfVector[ 1 ]) {
		case ZBridgeServer_entry__Connect_North_Seated : {
			/* Default exit sequence for state Seated */
			handle->stateConfVector[1] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case ZBridgeServer_entry__Connect_North_TeamNames : {
			/* Default exit sequence for state TeamNames */
			handle->stateConfVector[1] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case ZBridgeServer_entry__Connect_North_StartOfBoard : {
			/* Default exit sequence for state StartOfBoard */
			handle->stateConfVector[1] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case ZBridgeServer_entry__Connect_North_Connect : {
			/* Default exit sequence for state Connect */
			handle->stateConfVector[1] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case ZBridgeServer_entry__Deal_North_Info : {
			/* Default exit sequence for state Info */
			handle->stateConfVector[1] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case ZBridgeServer_entry__Deal_North_Cards : {
			/* Default exit sequence for state Cards */
			handle->stateConfVector[1] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case ZBridgeServer_entry__Deal_North_Sync : {
			/* Default exit sequence for state Sync */
			handle->stateConfVector[1] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case ZBridgeServer_entry__Bidding_North_Wait : {
			/* Default exit sequence for state Wait */
			handle->stateConfVector[1] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 1;
			/* Exit action for state 'Wait'. */
			handle->internal.northBid = bool_true;
			break;
		}
		case ZBridgeServer_entry__Bidding_North_Sync : {
			/* Default exit sequence for state Sync */
			handle->stateConfVector[1] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case ZBridgeServer_entry__Playing_North_Wait : {
			/* Default exit sequence for state Wait */
			handle->stateConfVector[1] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 1;
			/* Exit action for state 'Wait'. */
			handle->internal.northRCard = bool_true;
			break;
		}
		case ZBridgeServer_entry__Playing_North_Sync : {
			/* Default exit sequence for state Sync */
			handle->stateConfVector[1] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		default: break;
	}
	/* Handle exit of all possible states (of entry ) at position 2... */
	switch(handle->stateConfVector[ 2 ]) {
		case ZBridgeServer_entry__Connect_East_Seated : {
			/* Default exit sequence for state Seated */
			handle->stateConfVector[2] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 2;
			break;
		}
		case ZBridgeServer_entry__Connect_East_TeamNames : {
			/* Default exit sequence for state TeamNames */
			handle->stateConfVector[2] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 2;
			break;
		}
		case ZBridgeServer_entry__Connect_East_StartOfBoard : {
			/* Default exit sequence for state StartOfBoard */
			handle->stateConfVector[2] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 2;
			break;
		}
		case ZBridgeServer_entry__Connect_East_Connect : {
			/* Default exit sequence for state Connect */
			handle->stateConfVector[2] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 2;
			break;
		}
		case ZBridgeServer_entry__Deal_East_Info : {
			/* Default exit sequence for state Info */
			handle->stateConfVector[2] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 2;
			break;
		}
		case ZBridgeServer_entry__Deal_East_Cards : {
			/* Default exit sequence for state Cards */
			handle->stateConfVector[2] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 2;
			break;
		}
		case ZBridgeServer_entry__Deal_East_Sync : {
			/* Default exit sequence for state Sync */
			handle->stateConfVector[2] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 2;
			break;
		}
		case ZBridgeServer_entry__Bidding_East_Wait : {
			/* Default exit sequence for state Wait */
			handle->stateConfVector[2] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 2;
			/* Exit action for state 'Wait'. */
			handle->internal.eastBid = bool_true;
			break;
		}
		case ZBridgeServer_entry__Bidding_East_Sync : {
			/* Default exit sequence for state Sync */
			handle->stateConfVector[2] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 2;
			break;
		}
		case ZBridgeServer_entry__Playing_East_Wait : {
			/* Default exit sequence for state Wait */
			handle->stateConfVector[2] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 2;
			/* Exit action for state 'Wait'. */
			handle->internal.eastRCard = bool_true;
			break;
		}
		case ZBridgeServer_entry__Playing_East_Sync : {
			/* Default exit sequence for state Sync */
			handle->stateConfVector[2] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 2;
			break;
		}
		default: break;
	}
	/* Handle exit of all possible states (of entry ) at position 3... */
	switch(handle->stateConfVector[ 3 ]) {
		case ZBridgeServer_entry__Connect_South_Seated : {
			/* Default exit sequence for state Seated */
			handle->stateConfVector[3] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 3;
			break;
		}
		case ZBridgeServer_entry__Connect_South_TeamNames : {
			/* Default exit sequence for state TeamNames */
			handle->stateConfVector[3] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 3;
			break;
		}
		case ZBridgeServer_entry__Connect_South_StartOfBoard : {
			/* Default exit sequence for state StartOfBoard */
			handle->stateConfVector[3] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 3;
			break;
		}
		case ZBridgeServer_entry__Connect_South_Connect : {
			/* Default exit sequence for state Connect */
			handle->stateConfVector[3] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 3;
			break;
		}
		case ZBridgeServer_entry__Deal_South_Info : {
			/* Default exit sequence for state Info */
			handle->stateConfVector[3] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 3;
			break;
		}
		case ZBridgeServer_entry__Deal_South_Cards : {
			/* Default exit sequence for state Cards */
			handle->stateConfVector[3] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 3;
			break;
		}
		case ZBridgeServer_entry__Deal_South_Sync : {
			/* Default exit sequence for state Sync */
			handle->stateConfVector[3] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 3;
			break;
		}
		case ZBridgeServer_entry__Bidding_South_Wait : {
			/* Default exit sequence for state Wait */
			handle->stateConfVector[3] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 3;
			/* Exit action for state 'Wait'. */
			handle->internal.southBid = bool_true;
			break;
		}
		case ZBridgeServer_entry__Bidding_South_Sync : {
			/* Default exit sequence for state Sync */
			handle->stateConfVector[3] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 3;
			break;
		}
		case ZBridgeServer_entry__Playing_South_Wait : {
			/* Default exit sequence for state Wait */
			handle->stateConfVector[3] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 3;
			/* Exit action for state 'Wait'. */
			handle->internal.southRCard = bool_true;
			break;
		}
		case ZBridgeServer_entry__Playing_South_Sync : {
			/* Default exit sequence for state Sync */
			handle->stateConfVector[3] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 3;
			break;
		}
		default: break;
	}
	zBridgeServer_exact_SequenceImpl(handle);
}

static void zBridgeServer_clearInEvents(ZBridgeServer* handle) {
	handle->iface.newSession_raised = bool_false;
	handle->iface.connect_raised = bool_false;
	handle->iface.rTNames_raised = bool_false;
	handle->iface.rSBoard_raised = bool_false;
	handle->iface.continue_raised = bool_false;
	handle->iface.rDealInfo_raised = bool_false;
	handle->iface.rCards_raised = bool_false;
	handle->iface.newDeal_raised = bool_false;
	handle->iface.undo_raised = bool_false;
	handle->iface.bid_raised = bool_false;
	handle->iface.rBid_raised = bool_false;
	handle->iface.playerPlays_raised = bool_false;
	handle->iface.readyForPlayer_raised = bool_false;
	handle->iface.readyForDummy_raised = bool_false;
	handle->iface.readyForDummyCards_raised = bool_false;
	handle->iface.newLeader_raised = bool_false;
	handle->iface.allSync_raised = bool_false;
}

static void zBridgeServer_clearOutEvents(ZBridgeServer* handle) {
	handle->iface.seated_raised = bool_false;
	handle->iface.teamNames_raised = bool_false;
	handle->iface.newDealClients_raised = bool_false;
	handle->iface.startOfBoard_raised = bool_false;
	handle->iface.startOfBoardDelayed_raised = bool_false;
	handle->iface.dealInfo_raised = bool_false;
	handle->iface.cards_raised = bool_false;
	handle->iface.bidDone_raised = bool_false;
	handle->iface.bidInfo_raised = bool_false;
	handle->iface.undoBid_raised = bool_false;
	handle->iface.undoPlay_raised = bool_false;
	handle->iface.playerToLead_raised = bool_false;
	handle->iface.dummyToLead_raised = bool_false;
	handle->iface.sendPlayerPlays_raised = bool_false;
	handle->iface.dummyCards_raised = bool_false;
	handle->iface.getLeader_raised = bool_false;
	handle->iface.undoTrick_raised = bool_false;
	handle->iface.synchronize_raised = bool_false;
	handle->iface.endOfSession_raised = bool_false;
}

void zBridgeServer_runCycle(ZBridgeServer* handle) {
	
	zBridgeServer_clearOutEvents(handle);
	
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < ZBRIDGESERVER_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++) {
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition]) {
		case ZBridgeServer_entry__Connect_West_Seated : {
			zBridgeServer_react_entry__Connect_West_Seated(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_West_TeamNames : {
			zBridgeServer_react_entry__Connect_West_TeamNames(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_West_StartOfBoard : {
			zBridgeServer_react_entry__Connect_West_StartOfBoard(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_West_Connect : {
			zBridgeServer_react_entry__Connect_West_Connect(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_North_Seated : {
			zBridgeServer_react_entry__Connect_North_Seated(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_North_TeamNames : {
			zBridgeServer_react_entry__Connect_North_TeamNames(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_North_StartOfBoard : {
			zBridgeServer_react_entry__Connect_North_StartOfBoard(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_North_Connect : {
			zBridgeServer_react_entry__Connect_North_Connect(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_East_Seated : {
			zBridgeServer_react_entry__Connect_East_Seated(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_East_TeamNames : {
			zBridgeServer_react_entry__Connect_East_TeamNames(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_East_StartOfBoard : {
			zBridgeServer_react_entry__Connect_East_StartOfBoard(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_East_Connect : {
			zBridgeServer_react_entry__Connect_East_Connect(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_South_Seated : {
			zBridgeServer_react_entry__Connect_South_Seated(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_South_TeamNames : {
			zBridgeServer_react_entry__Connect_South_TeamNames(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_South_StartOfBoard : {
			zBridgeServer_react_entry__Connect_South_StartOfBoard(handle);
			break;
		}
		case ZBridgeServer_entry__Connect_South_Connect : {
			zBridgeServer_react_entry__Connect_South_Connect(handle);
			break;
		}
		case ZBridgeServer_entry__Deal_West_Info : {
			zBridgeServer_react_entry__Deal_West_Info(handle);
			break;
		}
		case ZBridgeServer_entry__Deal_West_Cards : {
			zBridgeServer_react_entry__Deal_West_Cards(handle);
			break;
		}
		case ZBridgeServer_entry__Deal_West_Sync : {
			zBridgeServer_react_entry__Deal_West_Sync(handle);
			break;
		}
		case ZBridgeServer_entry__Deal_North_Info : {
			zBridgeServer_react_entry__Deal_North_Info(handle);
			break;
		}
		case ZBridgeServer_entry__Deal_North_Cards : {
			zBridgeServer_react_entry__Deal_North_Cards(handle);
			break;
		}
		case ZBridgeServer_entry__Deal_North_Sync : {
			zBridgeServer_react_entry__Deal_North_Sync(handle);
			break;
		}
		case ZBridgeServer_entry__Deal_East_Info : {
			zBridgeServer_react_entry__Deal_East_Info(handle);
			break;
		}
		case ZBridgeServer_entry__Deal_East_Cards : {
			zBridgeServer_react_entry__Deal_East_Cards(handle);
			break;
		}
		case ZBridgeServer_entry__Deal_East_Sync : {
			zBridgeServer_react_entry__Deal_East_Sync(handle);
			break;
		}
		case ZBridgeServer_entry__Deal_South_Info : {
			zBridgeServer_react_entry__Deal_South_Info(handle);
			break;
		}
		case ZBridgeServer_entry__Deal_South_Cards : {
			zBridgeServer_react_entry__Deal_South_Cards(handle);
			break;
		}
		case ZBridgeServer_entry__Deal_South_Sync : {
			zBridgeServer_react_entry__Deal_South_Sync(handle);
			break;
		}
		case ZBridgeServer_entry__Bidding_West_Wait : {
			zBridgeServer_react_entry__Bidding_West_Wait(handle);
			break;
		}
		case ZBridgeServer_entry__Bidding_West_Sync : {
			zBridgeServer_react_entry__Bidding_West_Sync(handle);
			break;
		}
		case ZBridgeServer_entry__Bidding_North_Wait : {
			zBridgeServer_react_entry__Bidding_North_Wait(handle);
			break;
		}
		case ZBridgeServer_entry__Bidding_North_Sync : {
			zBridgeServer_react_entry__Bidding_North_Sync(handle);
			break;
		}
		case ZBridgeServer_entry__Bidding_East_Wait : {
			zBridgeServer_react_entry__Bidding_East_Wait(handle);
			break;
		}
		case ZBridgeServer_entry__Bidding_East_Sync : {
			zBridgeServer_react_entry__Bidding_East_Sync(handle);
			break;
		}
		case ZBridgeServer_entry__Bidding_South_Wait : {
			zBridgeServer_react_entry__Bidding_South_Wait(handle);
			break;
		}
		case ZBridgeServer_entry__Bidding_South_Sync : {
			zBridgeServer_react_entry__Bidding_South_Sync(handle);
			break;
		}
		case ZBridgeServer_entry__Playing_West_Wait : {
			zBridgeServer_react_entry__Playing_West_Wait(handle);
			break;
		}
		case ZBridgeServer_entry__Playing_West_Sync : {
			zBridgeServer_react_entry__Playing_West_Sync(handle);
			break;
		}
		case ZBridgeServer_entry__Playing_North_Wait : {
			zBridgeServer_react_entry__Playing_North_Wait(handle);
			break;
		}
		case ZBridgeServer_entry__Playing_North_Sync : {
			zBridgeServer_react_entry__Playing_North_Sync(handle);
			break;
		}
		case ZBridgeServer_entry__Playing_East_Wait : {
			zBridgeServer_react_entry__Playing_East_Wait(handle);
			break;
		}
		case ZBridgeServer_entry__Playing_East_Sync : {
			zBridgeServer_react_entry__Playing_East_Sync(handle);
			break;
		}
		case ZBridgeServer_entry__Playing_South_Wait : {
			zBridgeServer_react_entry__Playing_South_Wait(handle);
			break;
		}
		case ZBridgeServer_entry__Playing_South_Sync : {
			zBridgeServer_react_entry__Playing_South_Sync(handle);
			break;
		}
		case ZBridgeServer_entry__Exit1 : {
			zBridgeServer_react_entry__Exit1(handle);
			break;
		}
		case ZBridgeServer_entry__Exit2 : {
			zBridgeServer_react_entry__Exit2(handle);
			break;
		}
		case ZBridgeServer_entry__Exit3 : {
			zBridgeServer_react_entry__Exit3(handle);
			break;
		}
		case ZBridgeServer_entry__WaitLeader : {
			zBridgeServer_react_entry__WaitLeader(handle);
			break;
		}
		case ZBridgeServer_entry__SyncSB : {
			zBridgeServer_react_entry__SyncSB(handle);
			break;
		}
		case ZBridgeServer_entry__SyncAuction : {
			zBridgeServer_react_entry__SyncAuction(handle);
			break;
		}
		case ZBridgeServer_entry__SyncPlay : {
			zBridgeServer_react_entry__SyncPlay(handle);
			break;
		}
		case ZBridgeServer_entry__SyncLeader : {
			zBridgeServer_react_entry__SyncLeader(handle);
			break;
		}
		case ZBridgeServer_entry__SyncReplay : {
			zBridgeServer_react_entry__SyncReplay(handle);
			break;
		}
		default:
			break;
		}
	}
	
	zBridgeServer_clearInEvents(handle);
}


sc_boolean zBridgeServer_isActive(ZBridgeServer* handle, ZBridgeServerStates state) {
	switch (state) {
		case ZBridgeServer_entry__Connect : 
			return (sc_boolean) (handle->stateConfVector[0] >= ZBridgeServer_entry__Connect
				&& handle->stateConfVector[0] <= ZBridgeServer_entry__Connect_South_Connect);
		case ZBridgeServer_entry__Connect_West_Seated : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Connect_West_Seated
			);
		case ZBridgeServer_entry__Connect_West_TeamNames : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Connect_West_TeamNames
			);
		case ZBridgeServer_entry__Connect_West_StartOfBoard : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Connect_West_StartOfBoard
			);
		case ZBridgeServer_entry__Connect_West_Connect : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Connect_West_Connect
			);
		case ZBridgeServer_entry__Connect_North_Seated : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServer_entry__Connect_North_Seated
			);
		case ZBridgeServer_entry__Connect_North_TeamNames : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServer_entry__Connect_North_TeamNames
			);
		case ZBridgeServer_entry__Connect_North_StartOfBoard : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServer_entry__Connect_North_StartOfBoard
			);
		case ZBridgeServer_entry__Connect_North_Connect : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServer_entry__Connect_North_Connect
			);
		case ZBridgeServer_entry__Connect_East_Seated : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServer_entry__Connect_East_Seated
			);
		case ZBridgeServer_entry__Connect_East_TeamNames : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServer_entry__Connect_East_TeamNames
			);
		case ZBridgeServer_entry__Connect_East_StartOfBoard : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServer_entry__Connect_East_StartOfBoard
			);
		case ZBridgeServer_entry__Connect_East_Connect : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServer_entry__Connect_East_Connect
			);
		case ZBridgeServer_entry__Connect_South_Seated : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServer_entry__Connect_South_Seated
			);
		case ZBridgeServer_entry__Connect_South_TeamNames : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServer_entry__Connect_South_TeamNames
			);
		case ZBridgeServer_entry__Connect_South_StartOfBoard : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServer_entry__Connect_South_StartOfBoard
			);
		case ZBridgeServer_entry__Connect_South_Connect : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServer_entry__Connect_South_Connect
			);
		case ZBridgeServer_entry__Deal : 
			return (sc_boolean) (handle->stateConfVector[0] >= ZBridgeServer_entry__Deal
				&& handle->stateConfVector[0] <= ZBridgeServer_entry__Deal_South_Sync);
		case ZBridgeServer_entry__Deal_West_Info : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Deal_West_Info
			);
		case ZBridgeServer_entry__Deal_West_Cards : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Deal_West_Cards
			);
		case ZBridgeServer_entry__Deal_West_Sync : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Deal_West_Sync
			);
		case ZBridgeServer_entry__Deal_North_Info : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServer_entry__Deal_North_Info
			);
		case ZBridgeServer_entry__Deal_North_Cards : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServer_entry__Deal_North_Cards
			);
		case ZBridgeServer_entry__Deal_North_Sync : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServer_entry__Deal_North_Sync
			);
		case ZBridgeServer_entry__Deal_East_Info : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServer_entry__Deal_East_Info
			);
		case ZBridgeServer_entry__Deal_East_Cards : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServer_entry__Deal_East_Cards
			);
		case ZBridgeServer_entry__Deal_East_Sync : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServer_entry__Deal_East_Sync
			);
		case ZBridgeServer_entry__Deal_South_Info : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServer_entry__Deal_South_Info
			);
		case ZBridgeServer_entry__Deal_South_Cards : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServer_entry__Deal_South_Cards
			);
		case ZBridgeServer_entry__Deal_South_Sync : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServer_entry__Deal_South_Sync
			);
		case ZBridgeServer_entry__Bidding : 
			return (sc_boolean) (handle->stateConfVector[0] >= ZBridgeServer_entry__Bidding
				&& handle->stateConfVector[0] <= ZBridgeServer_entry__Bidding_South_Sync);
		case ZBridgeServer_entry__Bidding_West_Wait : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Bidding_West_Wait
			);
		case ZBridgeServer_entry__Bidding_West_Sync : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Bidding_West_Sync
			);
		case ZBridgeServer_entry__Bidding_North_Wait : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServer_entry__Bidding_North_Wait
			);
		case ZBridgeServer_entry__Bidding_North_Sync : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServer_entry__Bidding_North_Sync
			);
		case ZBridgeServer_entry__Bidding_East_Wait : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServer_entry__Bidding_East_Wait
			);
		case ZBridgeServer_entry__Bidding_East_Sync : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServer_entry__Bidding_East_Sync
			);
		case ZBridgeServer_entry__Bidding_South_Wait : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServer_entry__Bidding_South_Wait
			);
		case ZBridgeServer_entry__Bidding_South_Sync : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServer_entry__Bidding_South_Sync
			);
		case ZBridgeServer_entry__Playing : 
			return (sc_boolean) (handle->stateConfVector[0] >= ZBridgeServer_entry__Playing
				&& handle->stateConfVector[0] <= ZBridgeServer_entry__Playing_South_Sync);
		case ZBridgeServer_entry__Playing_West_Wait : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Playing_West_Wait
			);
		case ZBridgeServer_entry__Playing_West_Sync : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Playing_West_Sync
			);
		case ZBridgeServer_entry__Playing_North_Wait : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServer_entry__Playing_North_Wait
			);
		case ZBridgeServer_entry__Playing_North_Sync : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServer_entry__Playing_North_Sync
			);
		case ZBridgeServer_entry__Playing_East_Wait : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServer_entry__Playing_East_Wait
			);
		case ZBridgeServer_entry__Playing_East_Sync : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServer_entry__Playing_East_Sync
			);
		case ZBridgeServer_entry__Playing_South_Wait : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServer_entry__Playing_South_Wait
			);
		case ZBridgeServer_entry__Playing_South_Sync : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServer_entry__Playing_South_Sync
			);
		case ZBridgeServer_entry__Exit1 : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Exit1
			);
		case ZBridgeServer_entry__Exit2 : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Exit2
			);
		case ZBridgeServer_entry__Exit3 : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__Exit3
			);
		case ZBridgeServer_entry__WaitLeader : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__WaitLeader
			);
		case ZBridgeServer_entry__SyncSB : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__SyncSB
			);
		case ZBridgeServer_entry__SyncAuction : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__SyncAuction
			);
		case ZBridgeServer_entry__SyncPlay : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__SyncPlay
			);
		case ZBridgeServer_entry__SyncLeader : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__SyncLeader
			);
		case ZBridgeServer_entry__SyncReplay : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServer_entry__SyncReplay
			);
		default: return bool_false;
	}
}

void zBridgeServerIface_raise_newSession(ZBridgeServer* handle) {
	handle->iface.newSession_raised = bool_true;
}
void zBridgeServerIface_raise_connect(ZBridgeServer* handle, sc_integer value) {
	handle->iface.connect_value = value;
	handle->iface.connect_raised = bool_true;
}
void zBridgeServerIface_raise_rTNames(ZBridgeServer* handle, sc_integer value) {
	handle->iface.rTNames_value = value;
	handle->iface.rTNames_raised = bool_true;
}
void zBridgeServerIface_raise_rSBoard(ZBridgeServer* handle, sc_integer value) {
	handle->iface.rSBoard_value = value;
	handle->iface.rSBoard_raised = bool_true;
}
void zBridgeServerIface_raise_continue(ZBridgeServer* handle) {
	handle->iface.continue_raised = bool_true;
}
void zBridgeServerIface_raise_rDealInfo(ZBridgeServer* handle, sc_integer value) {
	handle->iface.rDealInfo_value = value;
	handle->iface.rDealInfo_raised = bool_true;
}
void zBridgeServerIface_raise_rCards(ZBridgeServer* handle, sc_integer value) {
	handle->iface.rCards_value = value;
	handle->iface.rCards_raised = bool_true;
}
void zBridgeServerIface_raise_newDeal(ZBridgeServer* handle) {
	handle->iface.newDeal_raised = bool_true;
}
void zBridgeServerIface_raise_undo(ZBridgeServer* handle, sc_integer value) {
	handle->iface.undo_value = value;
	handle->iface.undo_raised = bool_true;
}
void zBridgeServerIface_raise_bid(ZBridgeServer* handle, sc_integer value) {
	handle->iface.bid_value = value;
	handle->iface.bid_raised = bool_true;
}
void zBridgeServerIface_raise_rBid(ZBridgeServer* handle, sc_integer value) {
	handle->iface.rBid_value = value;
	handle->iface.rBid_raised = bool_true;
}
void zBridgeServerIface_raise_playerPlays(ZBridgeServer* handle, sc_integer value) {
	handle->iface.playerPlays_value = value;
	handle->iface.playerPlays_raised = bool_true;
}
void zBridgeServerIface_raise_readyForPlayer(ZBridgeServer* handle, sc_integer value) {
	handle->iface.readyForPlayer_value = value;
	handle->iface.readyForPlayer_raised = bool_true;
}
void zBridgeServerIface_raise_readyForDummy(ZBridgeServer* handle, sc_integer value) {
	handle->iface.readyForDummy_value = value;
	handle->iface.readyForDummy_raised = bool_true;
}
void zBridgeServerIface_raise_readyForDummyCards(ZBridgeServer* handle, sc_integer value) {
	handle->iface.readyForDummyCards_value = value;
	handle->iface.readyForDummyCards_raised = bool_true;
}
void zBridgeServerIface_raise_newLeader(ZBridgeServer* handle, sc_integer value) {
	handle->iface.newLeader_value = value;
	handle->iface.newLeader_raised = bool_true;
}
void zBridgeServerIface_raise_allSync(ZBridgeServer* handle) {
	handle->iface.allSync_raised = bool_true;
}

sc_boolean zBridgeServerIface_israised_seated(ZBridgeServer* handle) {
	return handle->iface.seated_raised;
}
sc_integer zBridgeServerIface_get_seated_value(ZBridgeServer* handle) {
	return handle->iface.seated_value;
}
sc_boolean zBridgeServerIface_israised_teamNames(ZBridgeServer* handle) {
	return handle->iface.teamNames_raised;
}
sc_boolean zBridgeServerIface_israised_newDealClients(ZBridgeServer* handle) {
	return handle->iface.newDealClients_raised;
}
sc_boolean zBridgeServerIface_israised_startOfBoard(ZBridgeServer* handle) {
	return handle->iface.startOfBoard_raised;
}
sc_boolean zBridgeServerIface_israised_startOfBoardDelayed(ZBridgeServer* handle) {
	return handle->iface.startOfBoardDelayed_raised;
}
sc_boolean zBridgeServerIface_israised_dealInfo(ZBridgeServer* handle) {
	return handle->iface.dealInfo_raised;
}
sc_integer zBridgeServerIface_get_dealInfo_value(ZBridgeServer* handle) {
	return handle->iface.dealInfo_value;
}
sc_boolean zBridgeServerIface_israised_cards(ZBridgeServer* handle) {
	return handle->iface.cards_raised;
}
sc_boolean zBridgeServerIface_israised_bidDone(ZBridgeServer* handle) {
	return handle->iface.bidDone_raised;
}
sc_integer zBridgeServerIface_get_bidDone_value(ZBridgeServer* handle) {
	return handle->iface.bidDone_value;
}
sc_boolean zBridgeServerIface_israised_bidInfo(ZBridgeServer* handle) {
	return handle->iface.bidInfo_raised;
}
sc_integer zBridgeServerIface_get_bidInfo_value(ZBridgeServer* handle) {
	return handle->iface.bidInfo_value;
}
sc_boolean zBridgeServerIface_israised_undoBid(ZBridgeServer* handle) {
	return handle->iface.undoBid_raised;
}
sc_integer zBridgeServerIface_get_undoBid_value(ZBridgeServer* handle) {
	return handle->iface.undoBid_value;
}
sc_boolean zBridgeServerIface_israised_undoPlay(ZBridgeServer* handle) {
	return handle->iface.undoPlay_raised;
}
sc_boolean zBridgeServerIface_israised_playerToLead(ZBridgeServer* handle) {
	return handle->iface.playerToLead_raised;
}
sc_integer zBridgeServerIface_get_playerToLead_value(ZBridgeServer* handle) {
	return handle->iface.playerToLead_value;
}
sc_boolean zBridgeServerIface_israised_dummyToLead(ZBridgeServer* handle) {
	return handle->iface.dummyToLead_raised;
}
sc_integer zBridgeServerIface_get_dummyToLead_value(ZBridgeServer* handle) {
	return handle->iface.dummyToLead_value;
}
sc_boolean zBridgeServerIface_israised_sendPlayerPlays(ZBridgeServer* handle) {
	return handle->iface.sendPlayerPlays_raised;
}
sc_integer zBridgeServerIface_get_sendPlayerPlays_value(ZBridgeServer* handle) {
	return handle->iface.sendPlayerPlays_value;
}
sc_boolean zBridgeServerIface_israised_dummyCards(ZBridgeServer* handle) {
	return handle->iface.dummyCards_raised;
}
sc_integer zBridgeServerIface_get_dummyCards_value(ZBridgeServer* handle) {
	return handle->iface.dummyCards_value;
}
sc_boolean zBridgeServerIface_israised_getLeader(ZBridgeServer* handle) {
	return handle->iface.getLeader_raised;
}
sc_boolean zBridgeServerIface_israised_undoTrick(ZBridgeServer* handle) {
	return handle->iface.undoTrick_raised;
}
sc_integer zBridgeServerIface_get_undoTrick_value(ZBridgeServer* handle) {
	return handle->iface.undoTrick_value;
}
sc_boolean zBridgeServerIface_israised_synchronize(ZBridgeServer* handle) {
	return handle->iface.synchronize_raised;
}
sc_boolean zBridgeServerIface_israised_endOfSession(ZBridgeServer* handle) {
	return handle->iface.endOfSession_raised;
}

sc_integer zBridgeServerIface_get_noOfBoards(ZBridgeServer* handle) {
	return handle->iface.noOfBoards;
}
void zBridgeServerIface_set_noOfBoards(ZBridgeServer* handle, sc_integer value) {
	handle->iface.noOfBoards = value;
}
sc_string zBridgeServerIface_get_name(ZBridgeServer* handle) {
	return handle->iface.name;
}
void zBridgeServerIface_set_name(ZBridgeServer* handle, sc_string value) {
	handle->iface.name = value;
}
sc_string zBridgeServerIface_get_nsTeamName(ZBridgeServer* handle) {
	return handle->iface.nsTeamName;
}
void zBridgeServerIface_set_nsTeamName(ZBridgeServer* handle, sc_string value) {
	handle->iface.nsTeamName = value;
}
sc_string zBridgeServerIface_get_ewTeamName(ZBridgeServer* handle) {
	return handle->iface.ewTeamName;
}
void zBridgeServerIface_set_ewTeamName(ZBridgeServer* handle, sc_string value) {
	handle->iface.ewTeamName = value;
}
sc_integer zBridgeServerIface_get_dealer(ZBridgeServer* handle) {
	return handle->iface.dealer;
}
void zBridgeServerIface_set_dealer(ZBridgeServer* handle, sc_integer value) {
	handle->iface.dealer = value;
}
sc_integer zBridgeServerIface_get_bidVal(ZBridgeServer* handle) {
	return handle->iface.bidVal;
}
void zBridgeServerIface_set_bidVal(ZBridgeServer* handle, sc_integer value) {
	handle->iface.bidVal = value;
}
sc_integer zBridgeServerIface_get_bidder(ZBridgeServer* handle) {
	return handle->iface.bidder;
}
void zBridgeServerIface_set_bidder(ZBridgeServer* handle, sc_integer value) {
	handle->iface.bidder = value;
}
sc_integer zBridgeServerIface_get_lastBid(ZBridgeServer* handle) {
	return handle->iface.lastBid;
}
void zBridgeServerIface_set_lastBid(ZBridgeServer* handle, sc_integer value) {
	handle->iface.lastBid = value;
}
sc_integer zBridgeServerIface_get_bidDouble(ZBridgeServer* handle) {
	return handle->iface.bidDouble;
}
void zBridgeServerIface_set_bidDouble(ZBridgeServer* handle, sc_integer value) {
	handle->iface.bidDouble = value;
}
sc_integer zBridgeServerIface_get_declarer(ZBridgeServer* handle) {
	return handle->iface.declarer;
}
void zBridgeServerIface_set_declarer(ZBridgeServer* handle, sc_integer value) {
	handle->iface.declarer = value;
}
sc_integer zBridgeServerIface_get_dummy(ZBridgeServer* handle) {
	return handle->iface.dummy;
}
void zBridgeServerIface_set_dummy(ZBridgeServer* handle, sc_integer value) {
	handle->iface.dummy = value;
}
sc_integer zBridgeServerIface_get_player(ZBridgeServer* handle) {
	return handle->iface.player;
}
void zBridgeServerIface_set_player(ZBridgeServer* handle, sc_integer value) {
	handle->iface.player = value;
}
sc_integer zBridgeServerIface_get_noTrick(ZBridgeServer* handle) {
	return handle->iface.noTrick;
}
void zBridgeServerIface_set_noTrick(ZBridgeServer* handle, sc_integer value) {
	handle->iface.noTrick = value;
}
sc_integer zBridgeServerIface_get_cardVal(ZBridgeServer* handle) {
	return handle->iface.cardVal;
}
void zBridgeServerIface_set_cardVal(ZBridgeServer* handle, sc_integer value) {
	handle->iface.cardVal = value;
}

// implementations of all internal functions

/* Entry action for statechart 'ZBridgeServer'. */
static void zBridgeServer_enact_SequenceImpl(ZBridgeServer* handle) {
}

/* Exit action for state 'ZBridgeServer'. */
static void zBridgeServer_exact_SequenceImpl(ZBridgeServer* handle) {
}

/* The reactions of state Seated. */
static void zBridgeServer_react_entry__Connect_West_Seated(ZBridgeServer* handle) {
	/* The reactions of state Seated. */
	if (handle->iface.rTNames_raised && handle->iface.rTNames_value == handle->internal.W) { 
		/* Default exit sequence for state Seated */
		handle->stateConfVector[0] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 0;
		handle->internal.westRTNames = bool_true;
		/* 'default' enter sequence for state TeamNames */
		/* Entry action for state 'TeamNames'. */
		if (handle->internal.westRTNames && handle->internal.northRTNames && handle->internal.eastRTNames && handle->internal.southRTNames) { 
			handle->iface.teamNames_raised = bool_true;
		} 
		handle->stateConfVector[0] = ZBridgeServer_entry__Connect_West_TeamNames;
		handle->stateConfVectorPosition = 0;
	} 
}

/* The reactions of state TeamNames. */
static void zBridgeServer_react_entry__Connect_West_TeamNames(ZBridgeServer* handle) {
	/* The reactions of state TeamNames. */
	if (handle->iface.rSBoard_raised && handle->iface.rSBoard_value == handle->internal.W) { 
		/* Default exit sequence for state TeamNames */
		handle->stateConfVector[0] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 0;
		handle->internal.westRSBoard = bool_true;
		/* 'default' enter sequence for state StartOfBoard */
		/* Entry action for state 'StartOfBoard'. */
		if (handle->internal.westRSBoard && handle->internal.northRSBoard && handle->internal.eastRSBoard && handle->internal.southRSBoard) { 
			handle->iface.startOfBoard_raised = bool_true;
		} 
		handle->stateConfVector[0] = ZBridgeServer_entry__Connect_West_StartOfBoard;
		handle->stateConfVectorPosition = 0;
	} 
}

/* The reactions of state StartOfBoard. */
static void zBridgeServer_react_entry__Connect_West_StartOfBoard(ZBridgeServer* handle) {
	/* The reactions of state StartOfBoard. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Connect_North_StartOfBoard) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Connect_East_StartOfBoard) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Connect_South_StartOfBoard) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Connect */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Connect_West_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Connect_West_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Connect_West_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Connect_West_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Connect_North_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Connect_North_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Connect_North_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Connect_North_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Connect_East_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Connect_East_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Connect_East_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Connect_East_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Connect_South_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Connect_South_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Connect_South_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Connect_South_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync0(handle);
	} 
}

/* The reactions of state Connect. */
static void zBridgeServer_react_entry__Connect_West_Connect(ZBridgeServer* handle) {
	/* The reactions of state Connect. */
	if (handle->iface.connect_raised && handle->iface.connect_value == handle->internal.W) { 
		/* Default exit sequence for state Connect */
		handle->stateConfVector[0] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 0;
		handle->iface.ewTeamName = handle->iface.name;
		handle->iface.seated_value = handle->internal.W;
		handle->iface.seated_raised = bool_true;
		/* 'default' enter sequence for state Seated */
		handle->stateConfVector[0] = ZBridgeServer_entry__Connect_West_Seated;
		handle->stateConfVectorPosition = 0;
	} 
}

/* The reactions of state Seated. */
static void zBridgeServer_react_entry__Connect_North_Seated(ZBridgeServer* handle) {
	/* The reactions of state Seated. */
	if (handle->iface.rTNames_raised && handle->iface.rTNames_value == handle->internal.N) { 
		/* Default exit sequence for state Seated */
		handle->stateConfVector[1] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 1;
		handle->internal.northRTNames = bool_true;
		/* 'default' enter sequence for state TeamNames */
		/* Entry action for state 'TeamNames'. */
		if (handle->internal.westRTNames && handle->internal.northRTNames && handle->internal.eastRTNames && handle->internal.southRTNames) { 
			handle->iface.teamNames_raised = bool_true;
		} 
		handle->stateConfVector[1] = ZBridgeServer_entry__Connect_North_TeamNames;
		handle->stateConfVectorPosition = 1;
	} 
}

/* The reactions of state TeamNames. */
static void zBridgeServer_react_entry__Connect_North_TeamNames(ZBridgeServer* handle) {
	/* The reactions of state TeamNames. */
	if (handle->iface.rSBoard_raised && handle->iface.rSBoard_value == handle->internal.N) { 
		/* Default exit sequence for state TeamNames */
		handle->stateConfVector[1] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 1;
		handle->internal.northRSBoard = bool_true;
		/* 'default' enter sequence for state StartOfBoard */
		/* Entry action for state 'StartOfBoard'. */
		if (handle->internal.westRSBoard && handle->internal.northRSBoard && handle->internal.eastRSBoard && handle->internal.southRSBoard) { 
			handle->iface.startOfBoard_raised = bool_true;
		} 
		handle->stateConfVector[1] = ZBridgeServer_entry__Connect_North_StartOfBoard;
		handle->stateConfVectorPosition = 1;
	} 
}

/* The reactions of state StartOfBoard. */
static void zBridgeServer_react_entry__Connect_North_StartOfBoard(ZBridgeServer* handle) {
	/* The reactions of state StartOfBoard. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Connect_West_StartOfBoard) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Connect_East_StartOfBoard) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Connect_South_StartOfBoard) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Connect */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Connect_West_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Connect_West_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Connect_West_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Connect_West_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Connect_North_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Connect_North_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Connect_North_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Connect_North_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Connect_East_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Connect_East_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Connect_East_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Connect_East_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Connect_South_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Connect_South_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Connect_South_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Connect_South_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync0(handle);
	} 
}

/* The reactions of state Connect. */
static void zBridgeServer_react_entry__Connect_North_Connect(ZBridgeServer* handle) {
	/* The reactions of state Connect. */
	if (handle->iface.connect_raised && handle->iface.connect_value == handle->internal.N) { 
		/* Default exit sequence for state Connect */
		handle->stateConfVector[1] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 1;
		handle->iface.nsTeamName = handle->iface.name;
		handle->iface.seated_value = handle->internal.N;
		handle->iface.seated_raised = bool_true;
		/* 'default' enter sequence for state Seated */
		handle->stateConfVector[1] = ZBridgeServer_entry__Connect_North_Seated;
		handle->stateConfVectorPosition = 1;
	} 
}

/* The reactions of state Seated. */
static void zBridgeServer_react_entry__Connect_East_Seated(ZBridgeServer* handle) {
	/* The reactions of state Seated. */
	if (handle->iface.rTNames_raised && handle->iface.rTNames_value == handle->internal.E) { 
		/* Default exit sequence for state Seated */
		handle->stateConfVector[2] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 2;
		handle->internal.eastRTNames = bool_true;
		/* 'default' enter sequence for state TeamNames */
		/* Entry action for state 'TeamNames'. */
		if (handle->internal.westRTNames && handle->internal.northRTNames && handle->internal.eastRTNames && handle->internal.southRTNames) { 
			handle->iface.teamNames_raised = bool_true;
		} 
		handle->stateConfVector[2] = ZBridgeServer_entry__Connect_East_TeamNames;
		handle->stateConfVectorPosition = 2;
	} 
}

/* The reactions of state TeamNames. */
static void zBridgeServer_react_entry__Connect_East_TeamNames(ZBridgeServer* handle) {
	/* The reactions of state TeamNames. */
	if (handle->iface.rSBoard_raised && handle->iface.rSBoard_value == handle->internal.E) { 
		/* Default exit sequence for state TeamNames */
		handle->stateConfVector[2] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 2;
		handle->internal.eastRSBoard = bool_true;
		/* 'default' enter sequence for state StartOfBoard */
		/* Entry action for state 'StartOfBoard'. */
		if (handle->internal.westRSBoard && handle->internal.northRSBoard && handle->internal.eastRSBoard && handle->internal.southRSBoard) { 
			handle->iface.startOfBoard_raised = bool_true;
		} 
		handle->stateConfVector[2] = ZBridgeServer_entry__Connect_East_StartOfBoard;
		handle->stateConfVectorPosition = 2;
	} 
}

/* The reactions of state StartOfBoard. */
static void zBridgeServer_react_entry__Connect_East_StartOfBoard(ZBridgeServer* handle) {
	/* The reactions of state StartOfBoard. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Connect_West_StartOfBoard) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Connect_North_StartOfBoard) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Connect_South_StartOfBoard) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Connect */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Connect_West_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Connect_West_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Connect_West_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Connect_West_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Connect_North_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Connect_North_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Connect_North_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Connect_North_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Connect_East_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Connect_East_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Connect_East_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Connect_East_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Connect_South_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Connect_South_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Connect_South_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Connect_South_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync0(handle);
	} 
}

/* The reactions of state Connect. */
static void zBridgeServer_react_entry__Connect_East_Connect(ZBridgeServer* handle) {
	/* The reactions of state Connect. */
	if (handle->iface.connect_raised && handle->iface.connect_value == handle->internal.E) { 
		/* Default exit sequence for state Connect */
		handle->stateConfVector[2] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 2;
		handle->iface.ewTeamName = handle->iface.name;
		handle->iface.seated_value = handle->internal.E;
		handle->iface.seated_raised = bool_true;
		/* 'default' enter sequence for state Seated */
		handle->stateConfVector[2] = ZBridgeServer_entry__Connect_East_Seated;
		handle->stateConfVectorPosition = 2;
	} 
}

/* The reactions of state Seated. */
static void zBridgeServer_react_entry__Connect_South_Seated(ZBridgeServer* handle) {
	/* The reactions of state Seated. */
	if (handle->iface.rTNames_raised && handle->iface.rTNames_value == handle->internal.S) { 
		/* Default exit sequence for state Seated */
		handle->stateConfVector[3] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 3;
		handle->internal.southRTNames = bool_true;
		/* 'default' enter sequence for state TeamNames */
		/* Entry action for state 'TeamNames'. */
		if (handle->internal.westRTNames && handle->internal.northRTNames && handle->internal.eastRTNames && handle->internal.southRTNames) { 
			handle->iface.teamNames_raised = bool_true;
		} 
		handle->stateConfVector[3] = ZBridgeServer_entry__Connect_South_TeamNames;
		handle->stateConfVectorPosition = 3;
	} 
}

/* The reactions of state TeamNames. */
static void zBridgeServer_react_entry__Connect_South_TeamNames(ZBridgeServer* handle) {
	/* The reactions of state TeamNames. */
	if (handle->iface.rSBoard_raised && handle->iface.rSBoard_value == handle->internal.S) { 
		/* Default exit sequence for state TeamNames */
		handle->stateConfVector[3] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 3;
		handle->internal.southRSBoard = bool_true;
		/* 'default' enter sequence for state StartOfBoard */
		/* Entry action for state 'StartOfBoard'. */
		if (handle->internal.westRSBoard && handle->internal.northRSBoard && handle->internal.eastRSBoard && handle->internal.southRSBoard) { 
			handle->iface.startOfBoard_raised = bool_true;
		} 
		handle->stateConfVector[3] = ZBridgeServer_entry__Connect_South_StartOfBoard;
		handle->stateConfVectorPosition = 3;
	} 
}

/* The reactions of state StartOfBoard. */
static void zBridgeServer_react_entry__Connect_South_StartOfBoard(ZBridgeServer* handle) {
	/* The reactions of state StartOfBoard. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Connect_West_StartOfBoard) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Connect_North_StartOfBoard) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Connect_East_StartOfBoard) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Connect */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Connect_West_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Connect_West_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Connect_West_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Connect_West_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Connect_North_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Connect_North_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Connect_North_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Connect_North_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Connect_East_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Connect_East_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Connect_East_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Connect_East_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Connect_South_Seated : {
				/* Default exit sequence for state Seated */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Connect_South_TeamNames : {
				/* Default exit sequence for state TeamNames */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Connect_South_StartOfBoard : {
				/* Default exit sequence for state StartOfBoard */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Connect_South_Connect : {
				/* Default exit sequence for state Connect */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync0(handle);
	} 
}

/* The reactions of state Connect. */
static void zBridgeServer_react_entry__Connect_South_Connect(ZBridgeServer* handle) {
	/* The reactions of state Connect. */
	if (handle->iface.connect_raised && handle->iface.connect_value == handle->internal.S) { 
		/* Default exit sequence for state Connect */
		handle->stateConfVector[3] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 3;
		handle->iface.nsTeamName = handle->iface.name;
		handle->iface.seated_value = handle->internal.S;
		handle->iface.seated_raised = bool_true;
		/* 'default' enter sequence for state Seated */
		handle->stateConfVector[3] = ZBridgeServer_entry__Connect_South_Seated;
		handle->stateConfVectorPosition = 3;
	} 
}

/* The reactions of state Info. */
static void zBridgeServer_react_entry__Deal_West_Info(ZBridgeServer* handle) {
	/* The reactions of state Info. */
	if (handle->iface.rDealInfo_raised && handle->iface.rDealInfo_value == handle->internal.W) { 
		/* Default exit sequence for state Info */
		handle->stateConfVector[0] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 0;
		handle->iface.dealInfo_value = handle->internal.W;
		handle->iface.dealInfo_raised = bool_true;
		/* 'default' enter sequence for state Cards */
		handle->stateConfVector[0] = ZBridgeServer_entry__Deal_West_Cards;
		handle->stateConfVectorPosition = 0;
	} 
}

/* The reactions of state Cards. */
static void zBridgeServer_react_entry__Deal_West_Cards(ZBridgeServer* handle) {
	/* The reactions of state Cards. */
	if (handle->iface.rCards_raised && handle->iface.rCards_value == handle->internal.W) { 
		/* Default exit sequence for state Cards */
		handle->stateConfVector[0] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 0;
		handle->internal.westRSBid = bool_true;
		/* 'default' enter sequence for state Sync */
		/* Entry action for state 'Sync'. */
		if (handle->internal.westRSBid && handle->internal.northRSBid && handle->internal.eastRSBid && handle->internal.southRSBid) { 
			handle->iface.cards_raised = bool_true;
		} 
		handle->stateConfVector[0] = ZBridgeServer_entry__Deal_West_Sync;
		handle->stateConfVectorPosition = 0;
	} 
}

/* The reactions of state Sync. */
static void zBridgeServer_react_entry__Deal_West_Sync(ZBridgeServer* handle) {
	/* The reactions of state Sync. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Deal_North_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Deal_East_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Deal_South_Sync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Deal */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Deal_West_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Deal_West_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Deal_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Deal_North_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Deal_North_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Deal_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Deal_East_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Deal_East_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Deal_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Deal_South_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Deal_South_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Deal_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync1(handle);
	} 
}

/* The reactions of state Info. */
static void zBridgeServer_react_entry__Deal_North_Info(ZBridgeServer* handle) {
	/* The reactions of state Info. */
	if (handle->iface.rDealInfo_raised && handle->iface.rDealInfo_value == handle->internal.N) { 
		/* Default exit sequence for state Info */
		handle->stateConfVector[1] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 1;
		handle->iface.dealInfo_value = handle->internal.N;
		handle->iface.dealInfo_raised = bool_true;
		/* 'default' enter sequence for state Cards */
		handle->stateConfVector[1] = ZBridgeServer_entry__Deal_North_Cards;
		handle->stateConfVectorPosition = 1;
	} 
}

/* The reactions of state Cards. */
static void zBridgeServer_react_entry__Deal_North_Cards(ZBridgeServer* handle) {
	/* The reactions of state Cards. */
	if (handle->iface.rCards_raised && handle->iface.rCards_value == handle->internal.N) { 
		/* Default exit sequence for state Cards */
		handle->stateConfVector[1] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 1;
		handle->internal.northRSBid = bool_true;
		/* 'default' enter sequence for state Sync */
		/* Entry action for state 'Sync'. */
		if (handle->internal.westRSBid && handle->internal.northRSBid && handle->internal.eastRSBid && handle->internal.southRSBid) { 
			handle->iface.cards_raised = bool_true;
		} 
		handle->stateConfVector[1] = ZBridgeServer_entry__Deal_North_Sync;
		handle->stateConfVectorPosition = 1;
	} 
}

/* The reactions of state Sync. */
static void zBridgeServer_react_entry__Deal_North_Sync(ZBridgeServer* handle) {
	/* The reactions of state Sync. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Deal_West_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Deal_East_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Deal_South_Sync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Deal */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Deal_West_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Deal_West_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Deal_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Deal_North_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Deal_North_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Deal_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Deal_East_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Deal_East_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Deal_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Deal_South_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Deal_South_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Deal_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync1(handle);
	} 
}

/* The reactions of state Info. */
static void zBridgeServer_react_entry__Deal_East_Info(ZBridgeServer* handle) {
	/* The reactions of state Info. */
	if (handle->iface.rDealInfo_raised && handle->iface.rDealInfo_value == handle->internal.E) { 
		/* Default exit sequence for state Info */
		handle->stateConfVector[2] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 2;
		handle->iface.dealInfo_value = handle->internal.E;
		handle->iface.dealInfo_raised = bool_true;
		/* 'default' enter sequence for state Cards */
		handle->stateConfVector[2] = ZBridgeServer_entry__Deal_East_Cards;
		handle->stateConfVectorPosition = 2;
	} 
}

/* The reactions of state Cards. */
static void zBridgeServer_react_entry__Deal_East_Cards(ZBridgeServer* handle) {
	/* The reactions of state Cards. */
	if (handle->iface.rCards_raised && handle->iface.rCards_value == handle->internal.E) { 
		/* Default exit sequence for state Cards */
		handle->stateConfVector[2] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 2;
		handle->internal.eastRSBid = bool_true;
		/* 'default' enter sequence for state Sync */
		/* Entry action for state 'Sync'. */
		if (handle->internal.westRSBid && handle->internal.northRSBid && handle->internal.eastRSBid && handle->internal.southRSBid) { 
			handle->iface.cards_raised = bool_true;
		} 
		handle->stateConfVector[2] = ZBridgeServer_entry__Deal_East_Sync;
		handle->stateConfVectorPosition = 2;
	} 
}

/* The reactions of state Sync. */
static void zBridgeServer_react_entry__Deal_East_Sync(ZBridgeServer* handle) {
	/* The reactions of state Sync. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Deal_West_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Deal_North_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Deal_South_Sync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Deal */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Deal_West_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Deal_West_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Deal_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Deal_North_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Deal_North_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Deal_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Deal_East_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Deal_East_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Deal_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Deal_South_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Deal_South_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Deal_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync1(handle);
	} 
}

/* The reactions of state Info. */
static void zBridgeServer_react_entry__Deal_South_Info(ZBridgeServer* handle) {
	/* The reactions of state Info. */
	if (handle->iface.rDealInfo_raised && handle->iface.rDealInfo_value == handle->internal.S) { 
		/* Default exit sequence for state Info */
		handle->stateConfVector[3] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 3;
		handle->iface.dealInfo_value = handle->internal.S;
		handle->iface.dealInfo_raised = bool_true;
		/* 'default' enter sequence for state Cards */
		handle->stateConfVector[3] = ZBridgeServer_entry__Deal_South_Cards;
		handle->stateConfVectorPosition = 3;
	} 
}

/* The reactions of state Cards. */
static void zBridgeServer_react_entry__Deal_South_Cards(ZBridgeServer* handle) {
	/* The reactions of state Cards. */
	if (handle->iface.rCards_raised && handle->iface.rCards_value == handle->internal.S) { 
		/* Default exit sequence for state Cards */
		handle->stateConfVector[3] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 3;
		handle->internal.southRSBid = bool_true;
		/* 'default' enter sequence for state Sync */
		/* Entry action for state 'Sync'. */
		if (handle->internal.westRSBid && handle->internal.northRSBid && handle->internal.eastRSBid && handle->internal.southRSBid) { 
			handle->iface.cards_raised = bool_true;
		} 
		handle->stateConfVector[3] = ZBridgeServer_entry__Deal_South_Sync;
		handle->stateConfVectorPosition = 3;
	} 
}

/* The reactions of state Sync. */
static void zBridgeServer_react_entry__Deal_South_Sync(ZBridgeServer* handle) {
	/* The reactions of state Sync. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Deal_West_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Deal_North_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Deal_East_Sync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Deal */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Deal_West_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Deal_West_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServer_entry__Deal_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Deal_North_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Deal_North_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServer_entry__Deal_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Deal_East_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Deal_East_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServer_entry__Deal_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Deal_South_Info : {
				/* Default exit sequence for state Info */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Deal_South_Cards : {
				/* Default exit sequence for state Cards */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServer_entry__Deal_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync1(handle);
	} 
}

/* The reactions of state Wait. */
static void zBridgeServer_react_entry__Bidding_West_Wait(ZBridgeServer* handle) {
	/* The reactions of state Wait. */
	if (handle->iface.undo_raised && handle->iface.undo_value >= 0) { 
		/* Default exit sequence for state Bidding */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Bidding_West_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				/* Exit action for state 'Wait'. */
				handle->internal.westBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Bidding_North_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Wait'. */
				handle->internal.northBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Bidding_East_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				/* Exit action for state 'Wait'. */
				handle->internal.eastBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Bidding_South_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				/* Exit action for state 'Wait'. */
				handle->internal.southBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		handle->internal.lastBidder = (handle->iface.undo_value + handle->iface.dealer) & 3;
		handle->iface.lastBid = handle->iface.bidVal;
		handle->iface.bidDouble = handle->internal.BID_NONE;
		handle->internal.noPasses = 0;
		handle->internal.firstBidRound = bool_false;
		handle->iface.bidder = (handle->internal.lastBidder + 1) & 3;
		handle->iface.undoBid_value = handle->iface.undo_value;
		handle->iface.undoBid_raised = bool_true;
		/* 'default' enter sequence for state Bidding */
		/* Entry action for state 'Bidding'. */
		handle->internal.westBid = bool_false;
		handle->internal.northBid = bool_false;
		handle->internal.eastBid = bool_false;
		handle->internal.southBid = bool_false;
		/* 'default' enter sequence for region West */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[0] = ZBridgeServer_entry__Bidding_West_Wait;
		handle->stateConfVectorPosition = 0;
		/* 'default' enter sequence for region North */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[1] = ZBridgeServer_entry__Bidding_North_Wait;
		handle->stateConfVectorPosition = 1;
		/* 'default' enter sequence for region East */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[2] = ZBridgeServer_entry__Bidding_East_Wait;
		handle->stateConfVectorPosition = 2;
		/* 'default' enter sequence for region South */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[3] = ZBridgeServer_entry__Bidding_South_Wait;
		handle->stateConfVectorPosition = 3;
	}  else {
		if (handle->iface.undo_raised && handle->iface.undo_value == handle->internal.REBID) { 
			/* Default exit sequence for state Bidding */
			/* Default exit sequence for region West */
			/* Handle exit of all possible states (of West) at position 0... */
			switch(handle->stateConfVector[ 0 ]) {
				case ZBridgeServer_entry__Bidding_West_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[0] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 0;
					/* Exit action for state 'Wait'. */
					handle->internal.westBid = bool_true;
					break;
				}
				case ZBridgeServer_entry__Bidding_West_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[0] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 0;
					break;
				}
				default: break;
			}
			/* Default exit sequence for region North */
			/* Handle exit of all possible states (of North) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case ZBridgeServer_entry__Bidding_North_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[1] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Wait'. */
					handle->internal.northBid = bool_true;
					break;
				}
				case ZBridgeServer_entry__Bidding_North_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[1] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* Default exit sequence for region East */
			/* Handle exit of all possible states (of East) at position 2... */
			switch(handle->stateConfVector[ 2 ]) {
				case ZBridgeServer_entry__Bidding_East_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[2] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 2;
					/* Exit action for state 'Wait'. */
					handle->internal.eastBid = bool_true;
					break;
				}
				case ZBridgeServer_entry__Bidding_East_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[2] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 2;
					break;
				}
				default: break;
			}
			/* Default exit sequence for region South */
			/* Handle exit of all possible states (of South) at position 3... */
			switch(handle->stateConfVector[ 3 ]) {
				case ZBridgeServer_entry__Bidding_South_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[3] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 3;
					/* Exit action for state 'Wait'. */
					handle->internal.southBid = bool_true;
					break;
				}
				case ZBridgeServer_entry__Bidding_South_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[3] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 3;
					break;
				}
				default: break;
			}
			/* The reactions of state null. */
			handle->iface.bidder = handle->iface.dealer;
			handle->internal.firstBidRound = bool_true;
			handle->internal.noPasses = 0;
			handle->iface.undoBid_value = handle->internal.REBID;
			handle->iface.undoBid_raised = bool_true;
			/* 'default' enter sequence for state Bidding */
			/* Entry action for state 'Bidding'. */
			handle->internal.westBid = bool_false;
			handle->internal.northBid = bool_false;
			handle->internal.eastBid = bool_false;
			handle->internal.southBid = bool_false;
			/* 'default' enter sequence for region West */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[0] = ZBridgeServer_entry__Bidding_West_Wait;
			handle->stateConfVectorPosition = 0;
			/* 'default' enter sequence for region North */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[1] = ZBridgeServer_entry__Bidding_North_Wait;
			handle->stateConfVectorPosition = 1;
			/* 'default' enter sequence for region East */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[2] = ZBridgeServer_entry__Bidding_East_Wait;
			handle->stateConfVectorPosition = 2;
			/* 'default' enter sequence for region South */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[3] = ZBridgeServer_entry__Bidding_South_Wait;
			handle->stateConfVectorPosition = 3;
		}  else {
			if (handle->iface.newDeal_raised) { 
				/* Default exit sequence for state Bidding */
				/* Default exit sequence for region West */
				/* Handle exit of all possible states (of West) at position 0... */
				switch(handle->stateConfVector[ 0 ]) {
					case ZBridgeServer_entry__Bidding_West_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[0] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 0;
						/* Exit action for state 'Wait'. */
						handle->internal.westBid = bool_true;
						break;
					}
					case ZBridgeServer_entry__Bidding_West_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[0] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 0;
						break;
					}
					default: break;
				}
				/* Default exit sequence for region North */
				/* Handle exit of all possible states (of North) at position 1... */
				switch(handle->stateConfVector[ 1 ]) {
					case ZBridgeServer_entry__Bidding_North_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[1] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'Wait'. */
						handle->internal.northBid = bool_true;
						break;
					}
					case ZBridgeServer_entry__Bidding_North_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[1] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					default: break;
				}
				/* Default exit sequence for region East */
				/* Handle exit of all possible states (of East) at position 2... */
				switch(handle->stateConfVector[ 2 ]) {
					case ZBridgeServer_entry__Bidding_East_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[2] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 2;
						/* Exit action for state 'Wait'. */
						handle->internal.eastBid = bool_true;
						break;
					}
					case ZBridgeServer_entry__Bidding_East_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[2] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 2;
						break;
					}
					default: break;
				}
				/* Default exit sequence for region South */
				/* Handle exit of all possible states (of South) at position 3... */
				switch(handle->stateConfVector[ 3 ]) {
					case ZBridgeServer_entry__Bidding_South_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[3] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 3;
						/* Exit action for state 'Wait'. */
						handle->internal.southBid = bool_true;
						break;
					}
					case ZBridgeServer_entry__Bidding_South_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[3] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 3;
						break;
					}
					default: break;
				}
				handle->iface.newDealClients_raised = bool_true;
				handle->internal.noBoards += 1;
				/* The reactions of state null. */
				if (handle->internal.noBoards == handle->iface.noOfBoards) { 
					handle->iface.endOfSession_raised = bool_true;
					/* 'default' enter sequence for state Exit1 */
					handle->stateConfVector[0] = ZBridgeServer_entry__Exit1;
					handle->stateConfVectorPosition = 0;
				}  else {
					handle->iface.synchronize_raised = bool_true;
					/* 'default' enter sequence for state SyncSB */
					handle->stateConfVector[0] = ZBridgeServer_entry__SyncSB;
					handle->stateConfVectorPosition = 0;
				}
			}  else {
				if (handle->iface.newSession_raised) { 
					/* Default exit sequence for state Bidding */
					/* Default exit sequence for region West */
					/* Handle exit of all possible states (of West) at position 0... */
					switch(handle->stateConfVector[ 0 ]) {
						case ZBridgeServer_entry__Bidding_West_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[0] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 0;
							/* Exit action for state 'Wait'. */
							handle->internal.westBid = bool_true;
							break;
						}
						case ZBridgeServer_entry__Bidding_West_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[0] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 0;
							break;
						}
						default: break;
					}
					/* Default exit sequence for region North */
					/* Handle exit of all possible states (of North) at position 1... */
					switch(handle->stateConfVector[ 1 ]) {
						case ZBridgeServer_entry__Bidding_North_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[1] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 1;
							/* Exit action for state 'Wait'. */
							handle->internal.northBid = bool_true;
							break;
						}
						case ZBridgeServer_entry__Bidding_North_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[1] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 1;
							break;
						}
						default: break;
					}
					/* Default exit sequence for region East */
					/* Handle exit of all possible states (of East) at position 2... */
					switch(handle->stateConfVector[ 2 ]) {
						case ZBridgeServer_entry__Bidding_East_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[2] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 2;
							/* Exit action for state 'Wait'. */
							handle->internal.eastBid = bool_true;
							break;
						}
						case ZBridgeServer_entry__Bidding_East_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[2] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 2;
							break;
						}
						default: break;
					}
					/* Default exit sequence for region South */
					/* Handle exit of all possible states (of South) at position 3... */
					switch(handle->stateConfVector[ 3 ]) {
						case ZBridgeServer_entry__Bidding_South_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[3] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 3;
							/* Exit action for state 'Wait'. */
							handle->internal.southBid = bool_true;
							break;
						}
						case ZBridgeServer_entry__Bidding_South_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[3] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 3;
							break;
						}
						default: break;
					}
					handle->iface.endOfSession_raised = bool_true;
					/* 'default' enter sequence for state Exit2 */
					handle->stateConfVector[0] = ZBridgeServer_entry__Exit2;
					handle->stateConfVectorPosition = 0;
				}  else {
					if (handle->iface.bid_raised && handle->iface.bid_value == handle->internal.W) { 
						/* Default exit sequence for state Wait */
						handle->stateConfVector[0] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 0;
						/* Exit action for state 'Wait'. */
						handle->internal.westBid = bool_true;
						/* 'default' enter sequence for state Sync */
						/* Entry action for state 'Sync'. */
						if (handle->internal.westBid && handle->internal.northBid && handle->internal.eastBid && handle->internal.southBid) { 
							handle->iface.bidDone_value = handle->iface.bidder;
							handle->iface.bidDone_raised = bool_true;
						} 
						handle->stateConfVector[0] = ZBridgeServer_entry__Bidding_West_Sync;
						handle->stateConfVectorPosition = 0;
					}  else {
						if (handle->iface.rBid_raised && handle->iface.rBid_value == handle->internal.W) { 
							/* Default exit sequence for state Wait */
							handle->stateConfVector[0] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 0;
							/* Exit action for state 'Wait'. */
							handle->internal.westBid = bool_true;
							/* 'default' enter sequence for state Sync */
							/* Entry action for state 'Sync'. */
							if (handle->internal.westBid && handle->internal.northBid && handle->internal.eastBid && handle->internal.southBid) { 
								handle->iface.bidDone_value = handle->iface.bidder;
								handle->iface.bidDone_raised = bool_true;
							} 
							handle->stateConfVector[0] = ZBridgeServer_entry__Bidding_West_Sync;
							handle->stateConfVectorPosition = 0;
						} 
					}
				}
			}
		}
	}
}

/* The reactions of state Sync. */
static void zBridgeServer_react_entry__Bidding_West_Sync(ZBridgeServer* handle) {
	/* The reactions of state Sync. */
	if (handle->iface.undo_raised && handle->iface.undo_value >= 0) { 
		/* Default exit sequence for state Bidding */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Bidding_West_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				/* Exit action for state 'Wait'. */
				handle->internal.westBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Bidding_North_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Wait'. */
				handle->internal.northBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Bidding_East_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				/* Exit action for state 'Wait'. */
				handle->internal.eastBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Bidding_South_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				/* Exit action for state 'Wait'. */
				handle->internal.southBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		handle->internal.lastBidder = (handle->iface.undo_value + handle->iface.dealer) & 3;
		handle->iface.lastBid = handle->iface.bidVal;
		handle->iface.bidDouble = handle->internal.BID_NONE;
		handle->internal.noPasses = 0;
		handle->internal.firstBidRound = bool_false;
		handle->iface.bidder = (handle->internal.lastBidder + 1) & 3;
		handle->iface.undoBid_value = handle->iface.undo_value;
		handle->iface.undoBid_raised = bool_true;
		/* 'default' enter sequence for state Bidding */
		/* Entry action for state 'Bidding'. */
		handle->internal.westBid = bool_false;
		handle->internal.northBid = bool_false;
		handle->internal.eastBid = bool_false;
		handle->internal.southBid = bool_false;
		/* 'default' enter sequence for region West */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[0] = ZBridgeServer_entry__Bidding_West_Wait;
		handle->stateConfVectorPosition = 0;
		/* 'default' enter sequence for region North */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[1] = ZBridgeServer_entry__Bidding_North_Wait;
		handle->stateConfVectorPosition = 1;
		/* 'default' enter sequence for region East */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[2] = ZBridgeServer_entry__Bidding_East_Wait;
		handle->stateConfVectorPosition = 2;
		/* 'default' enter sequence for region South */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[3] = ZBridgeServer_entry__Bidding_South_Wait;
		handle->stateConfVectorPosition = 3;
	}  else {
		if (handle->iface.undo_raised && handle->iface.undo_value == handle->internal.REBID) { 
			/* Default exit sequence for state Bidding */
			/* Default exit sequence for region West */
			/* Handle exit of all possible states (of West) at position 0... */
			switch(handle->stateConfVector[ 0 ]) {
				case ZBridgeServer_entry__Bidding_West_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[0] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 0;
					/* Exit action for state 'Wait'. */
					handle->internal.westBid = bool_true;
					break;
				}
				case ZBridgeServer_entry__Bidding_West_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[0] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 0;
					break;
				}
				default: break;
			}
			/* Default exit sequence for region North */
			/* Handle exit of all possible states (of North) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case ZBridgeServer_entry__Bidding_North_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[1] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Wait'. */
					handle->internal.northBid = bool_true;
					break;
				}
				case ZBridgeServer_entry__Bidding_North_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[1] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* Default exit sequence for region East */
			/* Handle exit of all possible states (of East) at position 2... */
			switch(handle->stateConfVector[ 2 ]) {
				case ZBridgeServer_entry__Bidding_East_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[2] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 2;
					/* Exit action for state 'Wait'. */
					handle->internal.eastBid = bool_true;
					break;
				}
				case ZBridgeServer_entry__Bidding_East_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[2] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 2;
					break;
				}
				default: break;
			}
			/* Default exit sequence for region South */
			/* Handle exit of all possible states (of South) at position 3... */
			switch(handle->stateConfVector[ 3 ]) {
				case ZBridgeServer_entry__Bidding_South_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[3] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 3;
					/* Exit action for state 'Wait'. */
					handle->internal.southBid = bool_true;
					break;
				}
				case ZBridgeServer_entry__Bidding_South_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[3] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 3;
					break;
				}
				default: break;
			}
			/* The reactions of state null. */
			handle->iface.bidder = handle->iface.dealer;
			handle->internal.firstBidRound = bool_true;
			handle->internal.noPasses = 0;
			handle->iface.undoBid_value = handle->internal.REBID;
			handle->iface.undoBid_raised = bool_true;
			/* 'default' enter sequence for state Bidding */
			/* Entry action for state 'Bidding'. */
			handle->internal.westBid = bool_false;
			handle->internal.northBid = bool_false;
			handle->internal.eastBid = bool_false;
			handle->internal.southBid = bool_false;
			/* 'default' enter sequence for region West */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[0] = ZBridgeServer_entry__Bidding_West_Wait;
			handle->stateConfVectorPosition = 0;
			/* 'default' enter sequence for region North */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[1] = ZBridgeServer_entry__Bidding_North_Wait;
			handle->stateConfVectorPosition = 1;
			/* 'default' enter sequence for region East */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[2] = ZBridgeServer_entry__Bidding_East_Wait;
			handle->stateConfVectorPosition = 2;
			/* 'default' enter sequence for region South */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[3] = ZBridgeServer_entry__Bidding_South_Wait;
			handle->stateConfVectorPosition = 3;
		}  else {
			if (handle->iface.newDeal_raised) { 
				/* Default exit sequence for state Bidding */
				/* Default exit sequence for region West */
				/* Handle exit of all possible states (of West) at position 0... */
				switch(handle->stateConfVector[ 0 ]) {
					case ZBridgeServer_entry__Bidding_West_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[0] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 0;
						/* Exit action for state 'Wait'. */
						handle->internal.westBid = bool_true;
						break;
					}
					case ZBridgeServer_entry__Bidding_West_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[0] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 0;
						break;
					}
					default: break;
				}
				/* Default exit sequence for region North */
				/* Handle exit of all possible states (of North) at position 1... */
				switch(handle->stateConfVector[ 1 ]) {
					case ZBridgeServer_entry__Bidding_North_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[1] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'Wait'. */
						handle->internal.northBid = bool_true;
						break;
					}
					case ZBridgeServer_entry__Bidding_North_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[1] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					default: break;
				}
				/* Default exit sequence for region East */
				/* Handle exit of all possible states (of East) at position 2... */
				switch(handle->stateConfVector[ 2 ]) {
					case ZBridgeServer_entry__Bidding_East_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[2] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 2;
						/* Exit action for state 'Wait'. */
						handle->internal.eastBid = bool_true;
						break;
					}
					case ZBridgeServer_entry__Bidding_East_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[2] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 2;
						break;
					}
					default: break;
				}
				/* Default exit sequence for region South */
				/* Handle exit of all possible states (of South) at position 3... */
				switch(handle->stateConfVector[ 3 ]) {
					case ZBridgeServer_entry__Bidding_South_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[3] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 3;
						/* Exit action for state 'Wait'. */
						handle->internal.southBid = bool_true;
						break;
					}
					case ZBridgeServer_entry__Bidding_South_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[3] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 3;
						break;
					}
					default: break;
				}
				handle->iface.newDealClients_raised = bool_true;
				handle->internal.noBoards += 1;
				/* The reactions of state null. */
				if (handle->internal.noBoards == handle->iface.noOfBoards) { 
					handle->iface.endOfSession_raised = bool_true;
					/* 'default' enter sequence for state Exit1 */
					handle->stateConfVector[0] = ZBridgeServer_entry__Exit1;
					handle->stateConfVectorPosition = 0;
				}  else {
					handle->iface.synchronize_raised = bool_true;
					/* 'default' enter sequence for state SyncSB */
					handle->stateConfVector[0] = ZBridgeServer_entry__SyncSB;
					handle->stateConfVectorPosition = 0;
				}
			}  else {
				if (handle->iface.newSession_raised) { 
					/* Default exit sequence for state Bidding */
					/* Default exit sequence for region West */
					/* Handle exit of all possible states (of West) at position 0... */
					switch(handle->stateConfVector[ 0 ]) {
						case ZBridgeServer_entry__Bidding_West_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[0] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 0;
							/* Exit action for state 'Wait'. */
							handle->internal.westBid = bool_true;
							break;
						}
						case ZBridgeServer_entry__Bidding_West_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[0] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 0;
							break;
						}
						default: break;
					}
					/* Default exit sequence for region North */
					/* Handle exit of all possible states (of North) at position 1... */
					switch(handle->stateConfVector[ 1 ]) {
						case ZBridgeServer_entry__Bidding_North_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[1] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 1;
							/* Exit action for state 'Wait'. */
							handle->internal.northBid = bool_true;
							break;
						}
						case ZBridgeServer_entry__Bidding_North_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[1] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 1;
							break;
						}
						default: break;
					}
					/* Default exit sequence for region East */
					/* Handle exit of all possible states (of East) at position 2... */
					switch(handle->stateConfVector[ 2 ]) {
						case ZBridgeServer_entry__Bidding_East_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[2] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 2;
							/* Exit action for state 'Wait'. */
							handle->internal.eastBid = bool_true;
							break;
						}
						case ZBridgeServer_entry__Bidding_East_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[2] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 2;
							break;
						}
						default: break;
					}
					/* Default exit sequence for region South */
					/* Handle exit of all possible states (of South) at position 3... */
					switch(handle->stateConfVector[ 3 ]) {
						case ZBridgeServer_entry__Bidding_South_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[3] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 3;
							/* Exit action for state 'Wait'. */
							handle->internal.southBid = bool_true;
							break;
						}
						case ZBridgeServer_entry__Bidding_South_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[3] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 3;
							break;
						}
						default: break;
					}
					handle->iface.endOfSession_raised = bool_true;
					/* 'default' enter sequence for state Exit2 */
					handle->stateConfVector[0] = ZBridgeServer_entry__Exit2;
					handle->stateConfVectorPosition = 0;
				}  else {
					if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Bidding_North_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Bidding_East_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Bidding_South_Sync) && handle->iface.continue_raised) { 
						/* Default exit sequence for state Bidding */
						/* Default exit sequence for region West */
						/* Handle exit of all possible states (of West) at position 0... */
						switch(handle->stateConfVector[ 0 ]) {
							case ZBridgeServer_entry__Bidding_West_Wait : {
								/* Default exit sequence for state Wait */
								handle->stateConfVector[0] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 0;
								/* Exit action for state 'Wait'. */
								handle->internal.westBid = bool_true;
								break;
							}
							case ZBridgeServer_entry__Bidding_West_Sync : {
								/* Default exit sequence for state Sync */
								handle->stateConfVector[0] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 0;
								break;
							}
							default: break;
						}
						/* Default exit sequence for region North */
						/* Handle exit of all possible states (of North) at position 1... */
						switch(handle->stateConfVector[ 1 ]) {
							case ZBridgeServer_entry__Bidding_North_Wait : {
								/* Default exit sequence for state Wait */
								handle->stateConfVector[1] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 1;
								/* Exit action for state 'Wait'. */
								handle->internal.northBid = bool_true;
								break;
							}
							case ZBridgeServer_entry__Bidding_North_Sync : {
								/* Default exit sequence for state Sync */
								handle->stateConfVector[1] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 1;
								break;
							}
							default: break;
						}
						/* Default exit sequence for region East */
						/* Handle exit of all possible states (of East) at position 2... */
						switch(handle->stateConfVector[ 2 ]) {
							case ZBridgeServer_entry__Bidding_East_Wait : {
								/* Default exit sequence for state Wait */
								handle->stateConfVector[2] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 2;
								/* Exit action for state 'Wait'. */
								handle->internal.eastBid = bool_true;
								break;
							}
							case ZBridgeServer_entry__Bidding_East_Sync : {
								/* Default exit sequence for state Sync */
								handle->stateConfVector[2] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 2;
								break;
							}
							default: break;
						}
						/* Default exit sequence for region South */
						/* Handle exit of all possible states (of South) at position 3... */
						switch(handle->stateConfVector[ 3 ]) {
							case ZBridgeServer_entry__Bidding_South_Wait : {
								/* Default exit sequence for state Wait */
								handle->stateConfVector[3] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 3;
								/* Exit action for state 'Wait'. */
								handle->internal.southBid = bool_true;
								break;
							}
							case ZBridgeServer_entry__Bidding_South_Sync : {
								/* Default exit sequence for state Sync */
								handle->stateConfVector[3] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 3;
								break;
							}
							default: break;
						}
						zBridgeServer_react_entry___sync2(handle);
					} 
				}
			}
		}
	}
}

/* The reactions of state Wait. */
static void zBridgeServer_react_entry__Bidding_North_Wait(ZBridgeServer* handle) {
	/* The reactions of state Wait. */
	if (handle->iface.rBid_raised && handle->iface.rBid_value == handle->internal.N) { 
		/* Default exit sequence for state Wait */
		handle->stateConfVector[1] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 1;
		/* Exit action for state 'Wait'. */
		handle->internal.northBid = bool_true;
		/* 'default' enter sequence for state Sync */
		/* Entry action for state 'Sync'. */
		if (handle->internal.westBid && handle->internal.northBid && handle->internal.eastBid && handle->internal.southBid) { 
			handle->iface.bidDone_value = handle->iface.bidder;
			handle->iface.bidDone_raised = bool_true;
		} 
		handle->stateConfVector[1] = ZBridgeServer_entry__Bidding_North_Sync;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.bid_raised && handle->iface.bid_value == handle->internal.N) { 
			/* Default exit sequence for state Wait */
			handle->stateConfVector[1] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 1;
			/* Exit action for state 'Wait'. */
			handle->internal.northBid = bool_true;
			/* 'default' enter sequence for state Sync */
			/* Entry action for state 'Sync'. */
			if (handle->internal.westBid && handle->internal.northBid && handle->internal.eastBid && handle->internal.southBid) { 
				handle->iface.bidDone_value = handle->iface.bidder;
				handle->iface.bidDone_raised = bool_true;
			} 
			handle->stateConfVector[1] = ZBridgeServer_entry__Bidding_North_Sync;
			handle->stateConfVectorPosition = 1;
		} 
	}
}

/* The reactions of state Sync. */
static void zBridgeServer_react_entry__Bidding_North_Sync(ZBridgeServer* handle) {
	/* The reactions of state Sync. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Bidding_West_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Bidding_East_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Bidding_South_Sync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Bidding */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Bidding_West_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				/* Exit action for state 'Wait'. */
				handle->internal.westBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Bidding_North_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Wait'. */
				handle->internal.northBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Bidding_East_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				/* Exit action for state 'Wait'. */
				handle->internal.eastBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Bidding_South_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				/* Exit action for state 'Wait'. */
				handle->internal.southBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync2(handle);
	} 
}

/* The reactions of state Wait. */
static void zBridgeServer_react_entry__Bidding_East_Wait(ZBridgeServer* handle) {
	/* The reactions of state Wait. */
	if (handle->iface.rBid_raised && handle->iface.rBid_value == handle->internal.E) { 
		/* Default exit sequence for state Wait */
		handle->stateConfVector[2] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 2;
		/* Exit action for state 'Wait'. */
		handle->internal.eastBid = bool_true;
		/* 'default' enter sequence for state Sync */
		/* Entry action for state 'Sync'. */
		if (handle->internal.westBid && handle->internal.northBid && handle->internal.eastBid && handle->internal.southBid) { 
			handle->iface.bidDone_value = handle->iface.bidder;
			handle->iface.bidDone_raised = bool_true;
		} 
		handle->stateConfVector[2] = ZBridgeServer_entry__Bidding_East_Sync;
		handle->stateConfVectorPosition = 2;
	}  else {
		if (handle->iface.bid_raised && handle->iface.bid_value == handle->internal.E) { 
			/* Default exit sequence for state Wait */
			handle->stateConfVector[2] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 2;
			/* Exit action for state 'Wait'. */
			handle->internal.eastBid = bool_true;
			/* 'default' enter sequence for state Sync */
			/* Entry action for state 'Sync'. */
			if (handle->internal.westBid && handle->internal.northBid && handle->internal.eastBid && handle->internal.southBid) { 
				handle->iface.bidDone_value = handle->iface.bidder;
				handle->iface.bidDone_raised = bool_true;
			} 
			handle->stateConfVector[2] = ZBridgeServer_entry__Bidding_East_Sync;
			handle->stateConfVectorPosition = 2;
		} 
	}
}

/* The reactions of state Sync. */
static void zBridgeServer_react_entry__Bidding_East_Sync(ZBridgeServer* handle) {
	/* The reactions of state Sync. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Bidding_West_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Bidding_North_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Bidding_South_Sync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Bidding */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Bidding_West_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				/* Exit action for state 'Wait'. */
				handle->internal.westBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Bidding_North_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Wait'. */
				handle->internal.northBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Bidding_East_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				/* Exit action for state 'Wait'. */
				handle->internal.eastBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Bidding_South_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				/* Exit action for state 'Wait'. */
				handle->internal.southBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync2(handle);
	} 
}

/* The reactions of state Wait. */
static void zBridgeServer_react_entry__Bidding_South_Wait(ZBridgeServer* handle) {
	/* The reactions of state Wait. */
	if (handle->iface.rBid_raised && handle->iface.rBid_value == handle->internal.S) { 
		/* Default exit sequence for state Wait */
		handle->stateConfVector[3] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 3;
		/* Exit action for state 'Wait'. */
		handle->internal.southBid = bool_true;
		/* 'default' enter sequence for state Sync */
		/* Entry action for state 'Sync'. */
		if (handle->internal.westBid && handle->internal.northBid && handle->internal.eastBid && handle->internal.southBid) { 
			handle->iface.bidDone_value = handle->iface.bidder;
			handle->iface.bidDone_raised = bool_true;
		} 
		handle->stateConfVector[3] = ZBridgeServer_entry__Bidding_South_Sync;
		handle->stateConfVectorPosition = 3;
	}  else {
		if (handle->iface.bid_raised && handle->iface.bid_value == handle->internal.S) { 
			/* Default exit sequence for state Wait */
			handle->stateConfVector[3] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 3;
			/* Exit action for state 'Wait'. */
			handle->internal.southBid = bool_true;
			/* 'default' enter sequence for state Sync */
			/* Entry action for state 'Sync'. */
			if (handle->internal.westBid && handle->internal.northBid && handle->internal.eastBid && handle->internal.southBid) { 
				handle->iface.bidDone_value = handle->iface.bidder;
				handle->iface.bidDone_raised = bool_true;
			} 
			handle->stateConfVector[3] = ZBridgeServer_entry__Bidding_South_Sync;
			handle->stateConfVectorPosition = 3;
		} 
	}
}

/* The reactions of state Sync. */
static void zBridgeServer_react_entry__Bidding_South_Sync(ZBridgeServer* handle) {
	/* The reactions of state Sync. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Bidding_West_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Bidding_North_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Bidding_East_Sync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Bidding */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Bidding_West_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				/* Exit action for state 'Wait'. */
				handle->internal.westBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Bidding_North_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Wait'. */
				handle->internal.northBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Bidding_East_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				/* Exit action for state 'Wait'. */
				handle->internal.eastBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Bidding_South_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				/* Exit action for state 'Wait'. */
				handle->internal.southBid = bool_true;
				break;
			}
			case ZBridgeServer_entry__Bidding_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync2(handle);
	} 
}

/* The reactions of state Wait. */
static void zBridgeServer_react_entry__Playing_West_Wait(ZBridgeServer* handle) {
	/* The reactions of state Wait. */
	if (handle->iface.undo_raised && handle->iface.undo_value == handle->internal.REBID) { 
		/* Default exit sequence for state Playing */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Playing_West_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				/* Exit action for state 'Wait'. */
				handle->internal.westRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Playing_North_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Wait'. */
				handle->internal.northRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Playing_East_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				/* Exit action for state 'Wait'. */
				handle->internal.eastRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Playing_South_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				/* Exit action for state 'Wait'. */
				handle->internal.southRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		/* The reactions of state null. */
		handle->iface.undoPlay_raised = bool_true;
		/* The reactions of state null. */
		handle->iface.bidder = handle->iface.dealer;
		handle->internal.firstBidRound = bool_true;
		handle->internal.noPasses = 0;
		handle->iface.undoBid_value = handle->internal.REBID;
		handle->iface.undoBid_raised = bool_true;
		/* 'default' enter sequence for state Bidding */
		/* Entry action for state 'Bidding'. */
		handle->internal.westBid = bool_false;
		handle->internal.northBid = bool_false;
		handle->internal.eastBid = bool_false;
		handle->internal.southBid = bool_false;
		/* 'default' enter sequence for region West */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[0] = ZBridgeServer_entry__Bidding_West_Wait;
		handle->stateConfVectorPosition = 0;
		/* 'default' enter sequence for region North */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[1] = ZBridgeServer_entry__Bidding_North_Wait;
		handle->stateConfVectorPosition = 1;
		/* 'default' enter sequence for region East */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[2] = ZBridgeServer_entry__Bidding_East_Wait;
		handle->stateConfVectorPosition = 2;
		/* 'default' enter sequence for region South */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[3] = ZBridgeServer_entry__Bidding_South_Wait;
		handle->stateConfVectorPosition = 3;
	}  else {
		if (handle->iface.newDeal_raised) { 
			/* Default exit sequence for state Playing */
			/* Default exit sequence for region West */
			/* Handle exit of all possible states (of West) at position 0... */
			switch(handle->stateConfVector[ 0 ]) {
				case ZBridgeServer_entry__Playing_West_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[0] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 0;
					/* Exit action for state 'Wait'. */
					handle->internal.westRCard = bool_true;
					break;
				}
				case ZBridgeServer_entry__Playing_West_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[0] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 0;
					break;
				}
				default: break;
			}
			/* Default exit sequence for region North */
			/* Handle exit of all possible states (of North) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case ZBridgeServer_entry__Playing_North_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[1] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Wait'. */
					handle->internal.northRCard = bool_true;
					break;
				}
				case ZBridgeServer_entry__Playing_North_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[1] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* Default exit sequence for region East */
			/* Handle exit of all possible states (of East) at position 2... */
			switch(handle->stateConfVector[ 2 ]) {
				case ZBridgeServer_entry__Playing_East_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[2] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 2;
					/* Exit action for state 'Wait'. */
					handle->internal.eastRCard = bool_true;
					break;
				}
				case ZBridgeServer_entry__Playing_East_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[2] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 2;
					break;
				}
				default: break;
			}
			/* Default exit sequence for region South */
			/* Handle exit of all possible states (of South) at position 3... */
			switch(handle->stateConfVector[ 3 ]) {
				case ZBridgeServer_entry__Playing_South_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[3] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 3;
					/* Exit action for state 'Wait'. */
					handle->internal.southRCard = bool_true;
					break;
				}
				case ZBridgeServer_entry__Playing_South_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[3] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 3;
					break;
				}
				default: break;
			}
			/* The reactions of state null. */
			handle->iface.newDealClients_raised = bool_true;
			handle->internal.noBoards += 1;
			/* The reactions of state null. */
			if (handle->internal.noBoards == handle->iface.noOfBoards) { 
				handle->iface.endOfSession_raised = bool_true;
				/* 'default' enter sequence for state Exit1 */
				handle->stateConfVector[0] = ZBridgeServer_entry__Exit1;
				handle->stateConfVectorPosition = 0;
			}  else {
				handle->iface.synchronize_raised = bool_true;
				/* 'default' enter sequence for state SyncSB */
				handle->stateConfVector[0] = ZBridgeServer_entry__SyncSB;
				handle->stateConfVectorPosition = 0;
			}
		}  else {
			if (handle->iface.newSession_raised) { 
				/* Default exit sequence for state Playing */
				/* Default exit sequence for region West */
				/* Handle exit of all possible states (of West) at position 0... */
				switch(handle->stateConfVector[ 0 ]) {
					case ZBridgeServer_entry__Playing_West_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[0] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 0;
						/* Exit action for state 'Wait'. */
						handle->internal.westRCard = bool_true;
						break;
					}
					case ZBridgeServer_entry__Playing_West_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[0] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 0;
						break;
					}
					default: break;
				}
				/* Default exit sequence for region North */
				/* Handle exit of all possible states (of North) at position 1... */
				switch(handle->stateConfVector[ 1 ]) {
					case ZBridgeServer_entry__Playing_North_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[1] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'Wait'. */
						handle->internal.northRCard = bool_true;
						break;
					}
					case ZBridgeServer_entry__Playing_North_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[1] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					default: break;
				}
				/* Default exit sequence for region East */
				/* Handle exit of all possible states (of East) at position 2... */
				switch(handle->stateConfVector[ 2 ]) {
					case ZBridgeServer_entry__Playing_East_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[2] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 2;
						/* Exit action for state 'Wait'. */
						handle->internal.eastRCard = bool_true;
						break;
					}
					case ZBridgeServer_entry__Playing_East_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[2] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 2;
						break;
					}
					default: break;
				}
				/* Default exit sequence for region South */
				/* Handle exit of all possible states (of South) at position 3... */
				switch(handle->stateConfVector[ 3 ]) {
					case ZBridgeServer_entry__Playing_South_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[3] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 3;
						/* Exit action for state 'Wait'. */
						handle->internal.southRCard = bool_true;
						break;
					}
					case ZBridgeServer_entry__Playing_South_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[3] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 3;
						break;
					}
					default: break;
				}
				handle->iface.endOfSession_raised = bool_true;
				/* 'default' enter sequence for state Exit3 */
				handle->stateConfVector[0] = ZBridgeServer_entry__Exit3;
				handle->stateConfVectorPosition = 0;
			}  else {
				if (handle->iface.undo_raised && handle->iface.undo_value == handle->internal.REPLAY) { 
					/* Default exit sequence for state Playing */
					/* Default exit sequence for region West */
					/* Handle exit of all possible states (of West) at position 0... */
					switch(handle->stateConfVector[ 0 ]) {
						case ZBridgeServer_entry__Playing_West_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[0] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 0;
							/* Exit action for state 'Wait'. */
							handle->internal.westRCard = bool_true;
							break;
						}
						case ZBridgeServer_entry__Playing_West_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[0] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 0;
							break;
						}
						default: break;
					}
					/* Default exit sequence for region North */
					/* Handle exit of all possible states (of North) at position 1... */
					switch(handle->stateConfVector[ 1 ]) {
						case ZBridgeServer_entry__Playing_North_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[1] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 1;
							/* Exit action for state 'Wait'. */
							handle->internal.northRCard = bool_true;
							break;
						}
						case ZBridgeServer_entry__Playing_North_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[1] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 1;
							break;
						}
						default: break;
					}
					/* Default exit sequence for region East */
					/* Handle exit of all possible states (of East) at position 2... */
					switch(handle->stateConfVector[ 2 ]) {
						case ZBridgeServer_entry__Playing_East_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[2] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 2;
							/* Exit action for state 'Wait'. */
							handle->internal.eastRCard = bool_true;
							break;
						}
						case ZBridgeServer_entry__Playing_East_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[2] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 2;
							break;
						}
						default: break;
					}
					/* Default exit sequence for region South */
					/* Handle exit of all possible states (of South) at position 3... */
					switch(handle->stateConfVector[ 3 ]) {
						case ZBridgeServer_entry__Playing_South_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[3] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 3;
							/* Exit action for state 'Wait'. */
							handle->internal.southRCard = bool_true;
							break;
						}
						case ZBridgeServer_entry__Playing_South_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[3] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 3;
							break;
						}
						default: break;
					}
					/* The reactions of state null. */
					handle->iface.noTrick = 0;
					handle->internal.leader = (handle->iface.declarer + 1) & 3;
					handle->iface.player = handle->internal.leader;
					handle->internal.playNo = 0;
					handle->iface.undoTrick_value = handle->internal.REPLAY;
					handle->iface.undoTrick_raised = bool_true;
					handle->iface.synchronize_raised = bool_true;
					/* 'default' enter sequence for state SyncReplay */
					handle->stateConfVector[0] = ZBridgeServer_entry__SyncReplay;
					handle->stateConfVectorPosition = 0;
				}  else {
					if (handle->iface.undo_raised && handle->iface.undo_value >= 0) { 
						/* Default exit sequence for state Playing */
						/* Default exit sequence for region West */
						/* Handle exit of all possible states (of West) at position 0... */
						switch(handle->stateConfVector[ 0 ]) {
							case ZBridgeServer_entry__Playing_West_Wait : {
								/* Default exit sequence for state Wait */
								handle->stateConfVector[0] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 0;
								/* Exit action for state 'Wait'. */
								handle->internal.westRCard = bool_true;
								break;
							}
							case ZBridgeServer_entry__Playing_West_Sync : {
								/* Default exit sequence for state Sync */
								handle->stateConfVector[0] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 0;
								break;
							}
							default: break;
						}
						/* Default exit sequence for region North */
						/* Handle exit of all possible states (of North) at position 1... */
						switch(handle->stateConfVector[ 1 ]) {
							case ZBridgeServer_entry__Playing_North_Wait : {
								/* Default exit sequence for state Wait */
								handle->stateConfVector[1] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 1;
								/* Exit action for state 'Wait'. */
								handle->internal.northRCard = bool_true;
								break;
							}
							case ZBridgeServer_entry__Playing_North_Sync : {
								/* Default exit sequence for state Sync */
								handle->stateConfVector[1] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 1;
								break;
							}
							default: break;
						}
						/* Default exit sequence for region East */
						/* Handle exit of all possible states (of East) at position 2... */
						switch(handle->stateConfVector[ 2 ]) {
							case ZBridgeServer_entry__Playing_East_Wait : {
								/* Default exit sequence for state Wait */
								handle->stateConfVector[2] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 2;
								/* Exit action for state 'Wait'. */
								handle->internal.eastRCard = bool_true;
								break;
							}
							case ZBridgeServer_entry__Playing_East_Sync : {
								/* Default exit sequence for state Sync */
								handle->stateConfVector[2] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 2;
								break;
							}
							default: break;
						}
						/* Default exit sequence for region South */
						/* Handle exit of all possible states (of South) at position 3... */
						switch(handle->stateConfVector[ 3 ]) {
							case ZBridgeServer_entry__Playing_South_Wait : {
								/* Default exit sequence for state Wait */
								handle->stateConfVector[3] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 3;
								/* Exit action for state 'Wait'. */
								handle->internal.southRCard = bool_true;
								break;
							}
							case ZBridgeServer_entry__Playing_South_Sync : {
								/* Default exit sequence for state Sync */
								handle->stateConfVector[3] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 3;
								break;
							}
							default: break;
						}
						handle->iface.undoTrick_value = handle->internal.PT;
						handle->iface.undoTrick_raised = bool_true;
						/* The reactions of state null. */
						handle->internal.leader = handle->iface.undo_value;
						handle->iface.player = handle->internal.leader;
						handle->internal.playNo = 0;
						handle->iface.synchronize_raised = bool_true;
						/* 'default' enter sequence for state SyncLeader */
						handle->stateConfVector[0] = ZBridgeServer_entry__SyncLeader;
						handle->stateConfVectorPosition = 0;
					}  else {
						if (handle->iface.playerPlays_raised && handle->iface.playerPlays_value == handle->internal.W) { 
							/* Default exit sequence for state Wait */
							handle->stateConfVector[0] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 0;
							/* Exit action for state 'Wait'. */
							handle->internal.westRCard = bool_true;
							/* 'default' enter sequence for state Sync */
							/* Entry action for state 'Sync'. */
							if (handle->internal.westRCard && handle->internal.northRCard && handle->internal.eastRCard && handle->internal.southRCard) { 
								handle->iface.sendPlayerPlays_value = handle->iface.player;
								handle->iface.sendPlayerPlays_raised = bool_true;
							} 
							handle->stateConfVector[0] = ZBridgeServer_entry__Playing_West_Sync;
							handle->stateConfVectorPosition = 0;
						}  else {
							if (handle->iface.readyForPlayer_raised && (handle->iface.readyForPlayer_value == handle->internal.W)) { 
								/* Default exit sequence for state Wait */
								handle->stateConfVector[0] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 0;
								/* Exit action for state 'Wait'. */
								handle->internal.westRCard = bool_true;
								/* 'default' enter sequence for state Sync */
								/* Entry action for state 'Sync'. */
								if (handle->internal.westRCard && handle->internal.northRCard && handle->internal.eastRCard && handle->internal.southRCard) { 
									handle->iface.sendPlayerPlays_value = handle->iface.player;
									handle->iface.sendPlayerPlays_raised = bool_true;
								} 
								handle->stateConfVector[0] = ZBridgeServer_entry__Playing_West_Sync;
								handle->stateConfVectorPosition = 0;
							}  else {
								if (handle->iface.readyForDummy_raised && handle->iface.readyForDummy_value == handle->internal.W) { 
									/* Default exit sequence for state Wait */
									handle->stateConfVector[0] = ZBridgeServer_last_state;
									handle->stateConfVectorPosition = 0;
									/* Exit action for state 'Wait'. */
									handle->internal.westRCard = bool_true;
									/* 'default' enter sequence for state Sync */
									/* Entry action for state 'Sync'. */
									if (handle->internal.westRCard && handle->internal.northRCard && handle->internal.eastRCard && handle->internal.southRCard) { 
										handle->iface.sendPlayerPlays_value = handle->iface.player;
										handle->iface.sendPlayerPlays_raised = bool_true;
									} 
									handle->stateConfVector[0] = ZBridgeServer_entry__Playing_West_Sync;
									handle->stateConfVectorPosition = 0;
								}  else {
									if (handle->iface.readyForDummyCards_raised && handle->iface.readyForDummyCards_value == handle->internal.W) { 
										/* Default exit sequence for state Wait */
										handle->stateConfVector[0] = ZBridgeServer_last_state;
										handle->stateConfVectorPosition = 0;
										/* Exit action for state 'Wait'. */
										handle->internal.westRCard = bool_true;
										handle->iface.dummyCards_value = handle->internal.W;
										handle->iface.dummyCards_raised = bool_true;
										/* 'default' enter sequence for state Wait */
										/* Entry action for state 'Wait'. */
										handle->internal.westRCard = bool_false;
										handle->stateConfVector[0] = ZBridgeServer_entry__Playing_West_Wait;
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

/* The reactions of state Sync. */
static void zBridgeServer_react_entry__Playing_West_Sync(ZBridgeServer* handle) {
	/* The reactions of state Sync. */
	if (handle->iface.undo_raised && handle->iface.undo_value == handle->internal.REBID) { 
		/* Default exit sequence for state Playing */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Playing_West_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				/* Exit action for state 'Wait'. */
				handle->internal.westRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Playing_North_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Wait'. */
				handle->internal.northRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Playing_East_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				/* Exit action for state 'Wait'. */
				handle->internal.eastRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Playing_South_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				/* Exit action for state 'Wait'. */
				handle->internal.southRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		/* The reactions of state null. */
		handle->iface.undoPlay_raised = bool_true;
		/* The reactions of state null. */
		handle->iface.bidder = handle->iface.dealer;
		handle->internal.firstBidRound = bool_true;
		handle->internal.noPasses = 0;
		handle->iface.undoBid_value = handle->internal.REBID;
		handle->iface.undoBid_raised = bool_true;
		/* 'default' enter sequence for state Bidding */
		/* Entry action for state 'Bidding'. */
		handle->internal.westBid = bool_false;
		handle->internal.northBid = bool_false;
		handle->internal.eastBid = bool_false;
		handle->internal.southBid = bool_false;
		/* 'default' enter sequence for region West */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[0] = ZBridgeServer_entry__Bidding_West_Wait;
		handle->stateConfVectorPosition = 0;
		/* 'default' enter sequence for region North */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[1] = ZBridgeServer_entry__Bidding_North_Wait;
		handle->stateConfVectorPosition = 1;
		/* 'default' enter sequence for region East */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[2] = ZBridgeServer_entry__Bidding_East_Wait;
		handle->stateConfVectorPosition = 2;
		/* 'default' enter sequence for region South */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[3] = ZBridgeServer_entry__Bidding_South_Wait;
		handle->stateConfVectorPosition = 3;
	}  else {
		if (handle->iface.newDeal_raised) { 
			/* Default exit sequence for state Playing */
			/* Default exit sequence for region West */
			/* Handle exit of all possible states (of West) at position 0... */
			switch(handle->stateConfVector[ 0 ]) {
				case ZBridgeServer_entry__Playing_West_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[0] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 0;
					/* Exit action for state 'Wait'. */
					handle->internal.westRCard = bool_true;
					break;
				}
				case ZBridgeServer_entry__Playing_West_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[0] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 0;
					break;
				}
				default: break;
			}
			/* Default exit sequence for region North */
			/* Handle exit of all possible states (of North) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case ZBridgeServer_entry__Playing_North_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[1] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Wait'. */
					handle->internal.northRCard = bool_true;
					break;
				}
				case ZBridgeServer_entry__Playing_North_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[1] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* Default exit sequence for region East */
			/* Handle exit of all possible states (of East) at position 2... */
			switch(handle->stateConfVector[ 2 ]) {
				case ZBridgeServer_entry__Playing_East_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[2] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 2;
					/* Exit action for state 'Wait'. */
					handle->internal.eastRCard = bool_true;
					break;
				}
				case ZBridgeServer_entry__Playing_East_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[2] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 2;
					break;
				}
				default: break;
			}
			/* Default exit sequence for region South */
			/* Handle exit of all possible states (of South) at position 3... */
			switch(handle->stateConfVector[ 3 ]) {
				case ZBridgeServer_entry__Playing_South_Wait : {
					/* Default exit sequence for state Wait */
					handle->stateConfVector[3] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 3;
					/* Exit action for state 'Wait'. */
					handle->internal.southRCard = bool_true;
					break;
				}
				case ZBridgeServer_entry__Playing_South_Sync : {
					/* Default exit sequence for state Sync */
					handle->stateConfVector[3] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 3;
					break;
				}
				default: break;
			}
			/* The reactions of state null. */
			handle->iface.newDealClients_raised = bool_true;
			handle->internal.noBoards += 1;
			/* The reactions of state null. */
			if (handle->internal.noBoards == handle->iface.noOfBoards) { 
				handle->iface.endOfSession_raised = bool_true;
				/* 'default' enter sequence for state Exit1 */
				handle->stateConfVector[0] = ZBridgeServer_entry__Exit1;
				handle->stateConfVectorPosition = 0;
			}  else {
				handle->iface.synchronize_raised = bool_true;
				/* 'default' enter sequence for state SyncSB */
				handle->stateConfVector[0] = ZBridgeServer_entry__SyncSB;
				handle->stateConfVectorPosition = 0;
			}
		}  else {
			if (handle->iface.newSession_raised) { 
				/* Default exit sequence for state Playing */
				/* Default exit sequence for region West */
				/* Handle exit of all possible states (of West) at position 0... */
				switch(handle->stateConfVector[ 0 ]) {
					case ZBridgeServer_entry__Playing_West_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[0] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 0;
						/* Exit action for state 'Wait'. */
						handle->internal.westRCard = bool_true;
						break;
					}
					case ZBridgeServer_entry__Playing_West_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[0] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 0;
						break;
					}
					default: break;
				}
				/* Default exit sequence for region North */
				/* Handle exit of all possible states (of North) at position 1... */
				switch(handle->stateConfVector[ 1 ]) {
					case ZBridgeServer_entry__Playing_North_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[1] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'Wait'. */
						handle->internal.northRCard = bool_true;
						break;
					}
					case ZBridgeServer_entry__Playing_North_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[1] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					default: break;
				}
				/* Default exit sequence for region East */
				/* Handle exit of all possible states (of East) at position 2... */
				switch(handle->stateConfVector[ 2 ]) {
					case ZBridgeServer_entry__Playing_East_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[2] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 2;
						/* Exit action for state 'Wait'. */
						handle->internal.eastRCard = bool_true;
						break;
					}
					case ZBridgeServer_entry__Playing_East_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[2] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 2;
						break;
					}
					default: break;
				}
				/* Default exit sequence for region South */
				/* Handle exit of all possible states (of South) at position 3... */
				switch(handle->stateConfVector[ 3 ]) {
					case ZBridgeServer_entry__Playing_South_Wait : {
						/* Default exit sequence for state Wait */
						handle->stateConfVector[3] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 3;
						/* Exit action for state 'Wait'. */
						handle->internal.southRCard = bool_true;
						break;
					}
					case ZBridgeServer_entry__Playing_South_Sync : {
						/* Default exit sequence for state Sync */
						handle->stateConfVector[3] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 3;
						break;
					}
					default: break;
				}
				handle->iface.endOfSession_raised = bool_true;
				/* 'default' enter sequence for state Exit3 */
				handle->stateConfVector[0] = ZBridgeServer_entry__Exit3;
				handle->stateConfVectorPosition = 0;
			}  else {
				if (handle->iface.undo_raised && handle->iface.undo_value == handle->internal.REPLAY) { 
					/* Default exit sequence for state Playing */
					/* Default exit sequence for region West */
					/* Handle exit of all possible states (of West) at position 0... */
					switch(handle->stateConfVector[ 0 ]) {
						case ZBridgeServer_entry__Playing_West_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[0] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 0;
							/* Exit action for state 'Wait'. */
							handle->internal.westRCard = bool_true;
							break;
						}
						case ZBridgeServer_entry__Playing_West_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[0] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 0;
							break;
						}
						default: break;
					}
					/* Default exit sequence for region North */
					/* Handle exit of all possible states (of North) at position 1... */
					switch(handle->stateConfVector[ 1 ]) {
						case ZBridgeServer_entry__Playing_North_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[1] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 1;
							/* Exit action for state 'Wait'. */
							handle->internal.northRCard = bool_true;
							break;
						}
						case ZBridgeServer_entry__Playing_North_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[1] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 1;
							break;
						}
						default: break;
					}
					/* Default exit sequence for region East */
					/* Handle exit of all possible states (of East) at position 2... */
					switch(handle->stateConfVector[ 2 ]) {
						case ZBridgeServer_entry__Playing_East_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[2] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 2;
							/* Exit action for state 'Wait'. */
							handle->internal.eastRCard = bool_true;
							break;
						}
						case ZBridgeServer_entry__Playing_East_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[2] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 2;
							break;
						}
						default: break;
					}
					/* Default exit sequence for region South */
					/* Handle exit of all possible states (of South) at position 3... */
					switch(handle->stateConfVector[ 3 ]) {
						case ZBridgeServer_entry__Playing_South_Wait : {
							/* Default exit sequence for state Wait */
							handle->stateConfVector[3] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 3;
							/* Exit action for state 'Wait'. */
							handle->internal.southRCard = bool_true;
							break;
						}
						case ZBridgeServer_entry__Playing_South_Sync : {
							/* Default exit sequence for state Sync */
							handle->stateConfVector[3] = ZBridgeServer_last_state;
							handle->stateConfVectorPosition = 3;
							break;
						}
						default: break;
					}
					/* The reactions of state null. */
					handle->iface.noTrick = 0;
					handle->internal.leader = (handle->iface.declarer + 1) & 3;
					handle->iface.player = handle->internal.leader;
					handle->internal.playNo = 0;
					handle->iface.undoTrick_value = handle->internal.REPLAY;
					handle->iface.undoTrick_raised = bool_true;
					handle->iface.synchronize_raised = bool_true;
					/* 'default' enter sequence for state SyncReplay */
					handle->stateConfVector[0] = ZBridgeServer_entry__SyncReplay;
					handle->stateConfVectorPosition = 0;
				}  else {
					if (handle->iface.undo_raised && handle->iface.undo_value >= 0) { 
						/* Default exit sequence for state Playing */
						/* Default exit sequence for region West */
						/* Handle exit of all possible states (of West) at position 0... */
						switch(handle->stateConfVector[ 0 ]) {
							case ZBridgeServer_entry__Playing_West_Wait : {
								/* Default exit sequence for state Wait */
								handle->stateConfVector[0] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 0;
								/* Exit action for state 'Wait'. */
								handle->internal.westRCard = bool_true;
								break;
							}
							case ZBridgeServer_entry__Playing_West_Sync : {
								/* Default exit sequence for state Sync */
								handle->stateConfVector[0] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 0;
								break;
							}
							default: break;
						}
						/* Default exit sequence for region North */
						/* Handle exit of all possible states (of North) at position 1... */
						switch(handle->stateConfVector[ 1 ]) {
							case ZBridgeServer_entry__Playing_North_Wait : {
								/* Default exit sequence for state Wait */
								handle->stateConfVector[1] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 1;
								/* Exit action for state 'Wait'. */
								handle->internal.northRCard = bool_true;
								break;
							}
							case ZBridgeServer_entry__Playing_North_Sync : {
								/* Default exit sequence for state Sync */
								handle->stateConfVector[1] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 1;
								break;
							}
							default: break;
						}
						/* Default exit sequence for region East */
						/* Handle exit of all possible states (of East) at position 2... */
						switch(handle->stateConfVector[ 2 ]) {
							case ZBridgeServer_entry__Playing_East_Wait : {
								/* Default exit sequence for state Wait */
								handle->stateConfVector[2] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 2;
								/* Exit action for state 'Wait'. */
								handle->internal.eastRCard = bool_true;
								break;
							}
							case ZBridgeServer_entry__Playing_East_Sync : {
								/* Default exit sequence for state Sync */
								handle->stateConfVector[2] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 2;
								break;
							}
							default: break;
						}
						/* Default exit sequence for region South */
						/* Handle exit of all possible states (of South) at position 3... */
						switch(handle->stateConfVector[ 3 ]) {
							case ZBridgeServer_entry__Playing_South_Wait : {
								/* Default exit sequence for state Wait */
								handle->stateConfVector[3] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 3;
								/* Exit action for state 'Wait'. */
								handle->internal.southRCard = bool_true;
								break;
							}
							case ZBridgeServer_entry__Playing_South_Sync : {
								/* Default exit sequence for state Sync */
								handle->stateConfVector[3] = ZBridgeServer_last_state;
								handle->stateConfVectorPosition = 3;
								break;
							}
							default: break;
						}
						handle->iface.undoTrick_value = handle->internal.PT;
						handle->iface.undoTrick_raised = bool_true;
						/* The reactions of state null. */
						handle->internal.leader = handle->iface.undo_value;
						handle->iface.player = handle->internal.leader;
						handle->internal.playNo = 0;
						handle->iface.synchronize_raised = bool_true;
						/* 'default' enter sequence for state SyncLeader */
						handle->stateConfVector[0] = ZBridgeServer_entry__SyncLeader;
						handle->stateConfVectorPosition = 0;
					}  else {
						if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Playing_North_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Playing_East_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Playing_South_Sync) && handle->iface.continue_raised) { 
							/* Default exit sequence for state Playing */
							/* Default exit sequence for region West */
							/* Handle exit of all possible states (of West) at position 0... */
							switch(handle->stateConfVector[ 0 ]) {
								case ZBridgeServer_entry__Playing_West_Wait : {
									/* Default exit sequence for state Wait */
									handle->stateConfVector[0] = ZBridgeServer_last_state;
									handle->stateConfVectorPosition = 0;
									/* Exit action for state 'Wait'. */
									handle->internal.westRCard = bool_true;
									break;
								}
								case ZBridgeServer_entry__Playing_West_Sync : {
									/* Default exit sequence for state Sync */
									handle->stateConfVector[0] = ZBridgeServer_last_state;
									handle->stateConfVectorPosition = 0;
									break;
								}
								default: break;
							}
							/* Default exit sequence for region North */
							/* Handle exit of all possible states (of North) at position 1... */
							switch(handle->stateConfVector[ 1 ]) {
								case ZBridgeServer_entry__Playing_North_Wait : {
									/* Default exit sequence for state Wait */
									handle->stateConfVector[1] = ZBridgeServer_last_state;
									handle->stateConfVectorPosition = 1;
									/* Exit action for state 'Wait'. */
									handle->internal.northRCard = bool_true;
									break;
								}
								case ZBridgeServer_entry__Playing_North_Sync : {
									/* Default exit sequence for state Sync */
									handle->stateConfVector[1] = ZBridgeServer_last_state;
									handle->stateConfVectorPosition = 1;
									break;
								}
								default: break;
							}
							/* Default exit sequence for region East */
							/* Handle exit of all possible states (of East) at position 2... */
							switch(handle->stateConfVector[ 2 ]) {
								case ZBridgeServer_entry__Playing_East_Wait : {
									/* Default exit sequence for state Wait */
									handle->stateConfVector[2] = ZBridgeServer_last_state;
									handle->stateConfVectorPosition = 2;
									/* Exit action for state 'Wait'. */
									handle->internal.eastRCard = bool_true;
									break;
								}
								case ZBridgeServer_entry__Playing_East_Sync : {
									/* Default exit sequence for state Sync */
									handle->stateConfVector[2] = ZBridgeServer_last_state;
									handle->stateConfVectorPosition = 2;
									break;
								}
								default: break;
							}
							/* Default exit sequence for region South */
							/* Handle exit of all possible states (of South) at position 3... */
							switch(handle->stateConfVector[ 3 ]) {
								case ZBridgeServer_entry__Playing_South_Wait : {
									/* Default exit sequence for state Wait */
									handle->stateConfVector[3] = ZBridgeServer_last_state;
									handle->stateConfVectorPosition = 3;
									/* Exit action for state 'Wait'. */
									handle->internal.southRCard = bool_true;
									break;
								}
								case ZBridgeServer_entry__Playing_South_Sync : {
									/* Default exit sequence for state Sync */
									handle->stateConfVector[3] = ZBridgeServer_last_state;
									handle->stateConfVectorPosition = 3;
									break;
								}
								default: break;
							}
							zBridgeServer_react_entry___sync3(handle);
						} 
					}
				}
			}
		}
	}
}

/* The reactions of state Wait. */
static void zBridgeServer_react_entry__Playing_North_Wait(ZBridgeServer* handle) {
	/* The reactions of state Wait. */
	if (handle->iface.playerPlays_raised && handle->iface.playerPlays_value == handle->internal.N) { 
		/* Default exit sequence for state Wait */
		handle->stateConfVector[1] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 1;
		/* Exit action for state 'Wait'. */
		handle->internal.northRCard = bool_true;
		/* 'default' enter sequence for state Sync */
		/* Entry action for state 'Sync'. */
		if (handle->internal.westRCard && handle->internal.northRCard && handle->internal.eastRCard && handle->internal.southRCard) { 
			handle->iface.sendPlayerPlays_value = handle->iface.player;
			handle->iface.sendPlayerPlays_raised = bool_true;
		} 
		handle->stateConfVector[1] = ZBridgeServer_entry__Playing_North_Sync;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.readyForPlayer_raised && handle->iface.readyForPlayer_value == handle->internal.N) { 
			/* Default exit sequence for state Wait */
			handle->stateConfVector[1] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 1;
			/* Exit action for state 'Wait'. */
			handle->internal.northRCard = bool_true;
			/* 'default' enter sequence for state Sync */
			/* Entry action for state 'Sync'. */
			if (handle->internal.westRCard && handle->internal.northRCard && handle->internal.eastRCard && handle->internal.southRCard) { 
				handle->iface.sendPlayerPlays_value = handle->iface.player;
				handle->iface.sendPlayerPlays_raised = bool_true;
			} 
			handle->stateConfVector[1] = ZBridgeServer_entry__Playing_North_Sync;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.readyForDummy_raised && handle->iface.readyForDummy_value == handle->internal.N) { 
				/* Default exit sequence for state Wait */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Wait'. */
				handle->internal.northRCard = bool_true;
				/* 'default' enter sequence for state Sync */
				/* Entry action for state 'Sync'. */
				if (handle->internal.westRCard && handle->internal.northRCard && handle->internal.eastRCard && handle->internal.southRCard) { 
					handle->iface.sendPlayerPlays_value = handle->iface.player;
					handle->iface.sendPlayerPlays_raised = bool_true;
				} 
				handle->stateConfVector[1] = ZBridgeServer_entry__Playing_North_Sync;
				handle->stateConfVectorPosition = 1;
			}  else {
				if (handle->iface.readyForDummyCards_raised && handle->iface.readyForDummyCards_value == handle->internal.N) { 
					/* Default exit sequence for state Wait */
					handle->stateConfVector[1] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Wait'. */
					handle->internal.northRCard = bool_true;
					handle->iface.dummyCards_value = handle->internal.N;
					handle->iface.dummyCards_raised = bool_true;
					/* 'default' enter sequence for state Wait */
					/* Entry action for state 'Wait'. */
					handle->internal.northRCard = bool_false;
					handle->stateConfVector[1] = ZBridgeServer_entry__Playing_North_Wait;
					handle->stateConfVectorPosition = 1;
				} 
			}
		}
	}
}

/* The reactions of state Sync. */
static void zBridgeServer_react_entry__Playing_North_Sync(ZBridgeServer* handle) {
	/* The reactions of state Sync. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Playing_West_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Playing_East_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Playing_South_Sync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Playing */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Playing_West_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				/* Exit action for state 'Wait'. */
				handle->internal.westRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Playing_North_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Wait'. */
				handle->internal.northRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Playing_East_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				/* Exit action for state 'Wait'. */
				handle->internal.eastRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Playing_South_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				/* Exit action for state 'Wait'. */
				handle->internal.southRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync3(handle);
	} 
}

/* The reactions of state Wait. */
static void zBridgeServer_react_entry__Playing_East_Wait(ZBridgeServer* handle) {
	/* The reactions of state Wait. */
	if (handle->iface.playerPlays_raised && handle->iface.playerPlays_value == handle->internal.E) { 
		/* Default exit sequence for state Wait */
		handle->stateConfVector[2] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 2;
		/* Exit action for state 'Wait'. */
		handle->internal.eastRCard = bool_true;
		/* 'default' enter sequence for state Sync */
		/* Entry action for state 'Sync'. */
		if (handle->internal.westRCard && handle->internal.northRCard && handle->internal.eastRCard && handle->internal.southRCard) { 
			handle->iface.sendPlayerPlays_value = handle->iface.player;
			handle->iface.sendPlayerPlays_raised = bool_true;
		} 
		handle->stateConfVector[2] = ZBridgeServer_entry__Playing_East_Sync;
		handle->stateConfVectorPosition = 2;
	}  else {
		if (handle->iface.readyForPlayer_raised && handle->iface.readyForPlayer_value == handle->internal.E) { 
			/* Default exit sequence for state Wait */
			handle->stateConfVector[2] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 2;
			/* Exit action for state 'Wait'. */
			handle->internal.eastRCard = bool_true;
			/* 'default' enter sequence for state Sync */
			/* Entry action for state 'Sync'. */
			if (handle->internal.westRCard && handle->internal.northRCard && handle->internal.eastRCard && handle->internal.southRCard) { 
				handle->iface.sendPlayerPlays_value = handle->iface.player;
				handle->iface.sendPlayerPlays_raised = bool_true;
			} 
			handle->stateConfVector[2] = ZBridgeServer_entry__Playing_East_Sync;
			handle->stateConfVectorPosition = 2;
		}  else {
			if (handle->iface.readyForDummy_raised && handle->iface.readyForDummy_value == handle->internal.E) { 
				/* Default exit sequence for state Wait */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				/* Exit action for state 'Wait'. */
				handle->internal.eastRCard = bool_true;
				/* 'default' enter sequence for state Sync */
				/* Entry action for state 'Sync'. */
				if (handle->internal.westRCard && handle->internal.northRCard && handle->internal.eastRCard && handle->internal.southRCard) { 
					handle->iface.sendPlayerPlays_value = handle->iface.player;
					handle->iface.sendPlayerPlays_raised = bool_true;
				} 
				handle->stateConfVector[2] = ZBridgeServer_entry__Playing_East_Sync;
				handle->stateConfVectorPosition = 2;
			}  else {
				if (handle->iface.readyForDummyCards_raised && handle->iface.readyForDummyCards_value == handle->internal.E) { 
					/* Default exit sequence for state Wait */
					handle->stateConfVector[2] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 2;
					/* Exit action for state 'Wait'. */
					handle->internal.eastRCard = bool_true;
					handle->iface.dummyCards_value = handle->internal.E;
					handle->iface.dummyCards_raised = bool_true;
					/* 'default' enter sequence for state Wait */
					/* Entry action for state 'Wait'. */
					handle->internal.eastRCard = bool_false;
					handle->stateConfVector[2] = ZBridgeServer_entry__Playing_East_Wait;
					handle->stateConfVectorPosition = 2;
				} 
			}
		}
	}
}

/* The reactions of state Sync. */
static void zBridgeServer_react_entry__Playing_East_Sync(ZBridgeServer* handle) {
	/* The reactions of state Sync. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Playing_West_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Playing_North_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Playing_South_Sync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Playing */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Playing_West_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				/* Exit action for state 'Wait'. */
				handle->internal.westRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Playing_North_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Wait'. */
				handle->internal.northRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Playing_East_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				/* Exit action for state 'Wait'. */
				handle->internal.eastRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Playing_South_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				/* Exit action for state 'Wait'. */
				handle->internal.southRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync3(handle);
	} 
}

/* The reactions of state Wait. */
static void zBridgeServer_react_entry__Playing_South_Wait(ZBridgeServer* handle) {
	/* The reactions of state Wait. */
	if (handle->iface.playerPlays_raised && handle->iface.playerPlays_value == handle->internal.S) { 
		/* Default exit sequence for state Wait */
		handle->stateConfVector[3] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 3;
		/* Exit action for state 'Wait'. */
		handle->internal.southRCard = bool_true;
		/* 'default' enter sequence for state Sync */
		/* Entry action for state 'Sync'. */
		if (handle->internal.westRCard && handle->internal.northRCard && handle->internal.eastRCard && handle->internal.southRCard) { 
			handle->iface.sendPlayerPlays_value = handle->iface.player;
			handle->iface.sendPlayerPlays_raised = bool_true;
		} 
		handle->stateConfVector[3] = ZBridgeServer_entry__Playing_South_Sync;
		handle->stateConfVectorPosition = 3;
	}  else {
		if (handle->iface.readyForPlayer_raised && handle->iface.readyForPlayer_value == handle->internal.S) { 
			/* Default exit sequence for state Wait */
			handle->stateConfVector[3] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 3;
			/* Exit action for state 'Wait'. */
			handle->internal.southRCard = bool_true;
			/* 'default' enter sequence for state Sync */
			/* Entry action for state 'Sync'. */
			if (handle->internal.westRCard && handle->internal.northRCard && handle->internal.eastRCard && handle->internal.southRCard) { 
				handle->iface.sendPlayerPlays_value = handle->iface.player;
				handle->iface.sendPlayerPlays_raised = bool_true;
			} 
			handle->stateConfVector[3] = ZBridgeServer_entry__Playing_South_Sync;
			handle->stateConfVectorPosition = 3;
		}  else {
			if (handle->iface.readyForDummy_raised && handle->iface.readyForDummy_value == handle->internal.S) { 
				/* Default exit sequence for state Wait */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				/* Exit action for state 'Wait'. */
				handle->internal.southRCard = bool_true;
				/* 'default' enter sequence for state Sync */
				/* Entry action for state 'Sync'. */
				if (handle->internal.westRCard && handle->internal.northRCard && handle->internal.eastRCard && handle->internal.southRCard) { 
					handle->iface.sendPlayerPlays_value = handle->iface.player;
					handle->iface.sendPlayerPlays_raised = bool_true;
				} 
				handle->stateConfVector[3] = ZBridgeServer_entry__Playing_South_Sync;
				handle->stateConfVectorPosition = 3;
			}  else {
				if (handle->iface.readyForDummyCards_raised && handle->iface.readyForDummyCards_value == handle->internal.S) { 
					/* Default exit sequence for state Wait */
					handle->stateConfVector[3] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 3;
					/* Exit action for state 'Wait'. */
					handle->internal.southRCard = bool_true;
					handle->iface.dummyCards_value = handle->internal.S;
					handle->iface.dummyCards_raised = bool_true;
					/* 'default' enter sequence for state Wait */
					/* Entry action for state 'Wait'. */
					handle->internal.southRCard = bool_false;
					handle->stateConfVector[3] = ZBridgeServer_entry__Playing_South_Wait;
					handle->stateConfVectorPosition = 3;
				} 
			}
		}
	}
}

/* The reactions of state Sync. */
static void zBridgeServer_react_entry__Playing_South_Sync(ZBridgeServer* handle) {
	/* The reactions of state Sync. */
	if (handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Playing_West_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Playing_North_Sync) && handle->iface.continue_raised && zBridgeServer_isActive(handle, ZBridgeServer_entry__Playing_East_Sync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state Playing */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServer_entry__Playing_West_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				/* Exit action for state 'Wait'. */
				handle->internal.westRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_West_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServer_entry__Playing_North_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Wait'. */
				handle->internal.northRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_North_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[1] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServer_entry__Playing_East_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				/* Exit action for state 'Wait'. */
				handle->internal.eastRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_East_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[2] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServer_entry__Playing_South_Wait : {
				/* Default exit sequence for state Wait */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				/* Exit action for state 'Wait'. */
				handle->internal.southRCard = bool_true;
				break;
			}
			case ZBridgeServer_entry__Playing_South_Sync : {
				/* Default exit sequence for state Sync */
				handle->stateConfVector[3] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServer_react_entry___sync3(handle);
	} 
}

/* The reactions of state Exit1. */
static void zBridgeServer_react_entry__Exit1(ZBridgeServer* handle) {
	/* The reactions of state Exit1. */
}

/* The reactions of state Exit2. */
static void zBridgeServer_react_entry__Exit2(ZBridgeServer* handle) {
	/* The reactions of state Exit2. */
}

/* The reactions of state Exit3. */
static void zBridgeServer_react_entry__Exit3(ZBridgeServer* handle) {
	/* The reactions of state Exit3. */
}

/* The reactions of state WaitLeader. */
static void zBridgeServer_react_entry__WaitLeader(ZBridgeServer* handle) {
	/* The reactions of state WaitLeader. */
	if (handle->iface.newLeader_raised) { 
		/* Default exit sequence for state WaitLeader */
		handle->stateConfVector[0] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 0;
		handle->internal.leader = handle->iface.newLeader_value;
		handle->iface.player = handle->internal.leader;
		handle->internal.playNo = 0;
		handle->iface.synchronize_raised = bool_true;
		/* 'default' enter sequence for state SyncLeader */
		handle->stateConfVector[0] = ZBridgeServer_entry__SyncLeader;
		handle->stateConfVectorPosition = 0;
	} 
}

/* The reactions of state SyncSB. */
static void zBridgeServer_react_entry__SyncSB(ZBridgeServer* handle) {
	/* The reactions of state SyncSB. */
	if (handle->iface.allSync_raised) { 
		/* Default exit sequence for state SyncSB */
		handle->stateConfVector[0] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 0;
		handle->iface.startOfBoardDelayed_raised = bool_true;
		/* 'default' enter sequence for state Deal */
		/* Entry action for state 'Deal'. */
		handle->internal.westRSBid = bool_false;
		handle->internal.northRSBid = bool_false;
		handle->internal.eastRSBid = bool_false;
		handle->internal.southRSBid = bool_false;
		/* 'default' enter sequence for region West */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Info */
		handle->stateConfVector[0] = ZBridgeServer_entry__Deal_West_Info;
		handle->stateConfVectorPosition = 0;
		/* 'default' enter sequence for region North */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Info */
		handle->stateConfVector[1] = ZBridgeServer_entry__Deal_North_Info;
		handle->stateConfVectorPosition = 1;
		/* 'default' enter sequence for region East */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Info */
		handle->stateConfVector[2] = ZBridgeServer_entry__Deal_East_Info;
		handle->stateConfVectorPosition = 2;
		/* 'default' enter sequence for region South */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Info */
		handle->stateConfVector[3] = ZBridgeServer_entry__Deal_South_Info;
		handle->stateConfVectorPosition = 3;
	} 
}

/* The reactions of state SyncAuction. */
static void zBridgeServer_react_entry__SyncAuction(ZBridgeServer* handle) {
	/* The reactions of state SyncAuction. */
	if (handle->iface.allSync_raised) { 
		/* Default exit sequence for state SyncAuction */
		handle->stateConfVector[0] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 0;
		/* 'default' enter sequence for state Bidding */
		/* Entry action for state 'Bidding'. */
		handle->internal.westBid = bool_false;
		handle->internal.northBid = bool_false;
		handle->internal.eastBid = bool_false;
		handle->internal.southBid = bool_false;
		/* 'default' enter sequence for region West */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[0] = ZBridgeServer_entry__Bidding_West_Wait;
		handle->stateConfVectorPosition = 0;
		/* 'default' enter sequence for region North */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[1] = ZBridgeServer_entry__Bidding_North_Wait;
		handle->stateConfVectorPosition = 1;
		/* 'default' enter sequence for region East */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[2] = ZBridgeServer_entry__Bidding_East_Wait;
		handle->stateConfVectorPosition = 2;
		/* 'default' enter sequence for region South */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		handle->stateConfVector[3] = ZBridgeServer_entry__Bidding_South_Wait;
		handle->stateConfVectorPosition = 3;
	}  else {
		if (handle->iface.newDeal_raised) { 
			/* Default exit sequence for state SyncAuction */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			handle->iface.newDealClients_raised = bool_true;
			handle->internal.noBoards += 1;
			/* The reactions of state null. */
			if (handle->internal.noBoards == handle->iface.noOfBoards) { 
				handle->iface.endOfSession_raised = bool_true;
				/* 'default' enter sequence for state Exit1 */
				handle->stateConfVector[0] = ZBridgeServer_entry__Exit1;
				handle->stateConfVectorPosition = 0;
			}  else {
				handle->iface.synchronize_raised = bool_true;
				/* 'default' enter sequence for state SyncSB */
				handle->stateConfVector[0] = ZBridgeServer_entry__SyncSB;
				handle->stateConfVectorPosition = 0;
			}
		} 
	}
}

/* The reactions of state SyncPlay. */
static void zBridgeServer_react_entry__SyncPlay(ZBridgeServer* handle) {
	/* The reactions of state SyncPlay. */
	if (handle->iface.allSync_raised) { 
		/* Default exit sequence for state SyncPlay */
		handle->stateConfVector[0] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 0;
		handle->iface.bidInfo_value = handle->internal.leader;
		handle->iface.bidInfo_raised = bool_true;
		/* The reactions of state null. */
		if (handle->internal.leader == handle->iface.dummy) { 
			handle->iface.dummyToLead_value = handle->iface.declarer;
			handle->iface.dummyToLead_raised = bool_true;
			/* 'default' enter sequence for state Playing */
			/* 'default' enter sequence for region West */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			/* Entry action for state 'Wait'. */
			handle->internal.westRCard = bool_false;
			handle->stateConfVector[0] = ZBridgeServer_entry__Playing_West_Wait;
			handle->stateConfVectorPosition = 0;
			/* 'default' enter sequence for region North */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			/* Entry action for state 'Wait'. */
			handle->internal.northRCard = bool_false;
			handle->stateConfVector[1] = ZBridgeServer_entry__Playing_North_Wait;
			handle->stateConfVectorPosition = 1;
			/* 'default' enter sequence for region East */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			/* Entry action for state 'Wait'. */
			handle->internal.eastRCard = bool_false;
			handle->stateConfVector[2] = ZBridgeServer_entry__Playing_East_Wait;
			handle->stateConfVectorPosition = 2;
			/* 'default' enter sequence for region South */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			/* Entry action for state 'Wait'. */
			handle->internal.southRCard = bool_false;
			handle->stateConfVector[3] = ZBridgeServer_entry__Playing_South_Wait;
			handle->stateConfVectorPosition = 3;
		}  else {
			handle->iface.playerToLead_value = handle->internal.leader;
			handle->iface.playerToLead_raised = bool_true;
			/* 'default' enter sequence for state Playing */
			/* 'default' enter sequence for region West */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			/* Entry action for state 'Wait'. */
			handle->internal.westRCard = bool_false;
			handle->stateConfVector[0] = ZBridgeServer_entry__Playing_West_Wait;
			handle->stateConfVectorPosition = 0;
			/* 'default' enter sequence for region North */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			/* Entry action for state 'Wait'. */
			handle->internal.northRCard = bool_false;
			handle->stateConfVector[1] = ZBridgeServer_entry__Playing_North_Wait;
			handle->stateConfVectorPosition = 1;
			/* 'default' enter sequence for region East */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			/* Entry action for state 'Wait'. */
			handle->internal.eastRCard = bool_false;
			handle->stateConfVector[2] = ZBridgeServer_entry__Playing_East_Wait;
			handle->stateConfVectorPosition = 2;
			/* 'default' enter sequence for region South */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			/* Entry action for state 'Wait'. */
			handle->internal.southRCard = bool_false;
			handle->stateConfVector[3] = ZBridgeServer_entry__Playing_South_Wait;
			handle->stateConfVectorPosition = 3;
		}
	}  else {
		if (handle->iface.newDeal_raised) { 
			/* Default exit sequence for state SyncPlay */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			handle->iface.newDealClients_raised = bool_true;
			handle->internal.noBoards += 1;
			/* The reactions of state null. */
			if (handle->internal.noBoards == handle->iface.noOfBoards) { 
				handle->iface.endOfSession_raised = bool_true;
				/* 'default' enter sequence for state Exit1 */
				handle->stateConfVector[0] = ZBridgeServer_entry__Exit1;
				handle->stateConfVectorPosition = 0;
			}  else {
				handle->iface.synchronize_raised = bool_true;
				/* 'default' enter sequence for state SyncSB */
				handle->stateConfVector[0] = ZBridgeServer_entry__SyncSB;
				handle->stateConfVectorPosition = 0;
			}
		} 
	}
}

/* The reactions of state SyncLeader. */
static void zBridgeServer_react_entry__SyncLeader(ZBridgeServer* handle) {
	/* The reactions of state SyncLeader. */
	if (handle->iface.allSync_raised) { 
		/* Default exit sequence for state SyncLeader */
		handle->stateConfVector[0] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 0;
		/* The reactions of state null. */
		if (handle->iface.noTrick == 13) { 
			handle->internal.noBoards += 1;
			/* The reactions of state null. */
			if (handle->internal.noBoards == handle->iface.noOfBoards) { 
				handle->iface.endOfSession_raised = bool_true;
				/* 'default' enter sequence for state Exit1 */
				handle->stateConfVector[0] = ZBridgeServer_entry__Exit1;
				handle->stateConfVectorPosition = 0;
			}  else {
				handle->iface.synchronize_raised = bool_true;
				/* 'default' enter sequence for state SyncSB */
				handle->stateConfVector[0] = ZBridgeServer_entry__SyncSB;
				handle->stateConfVectorPosition = 0;
			}
		}  else {
			/* The reactions of state null. */
			if (handle->internal.leader == handle->iface.dummy) { 
				handle->iface.dummyToLead_value = handle->iface.declarer;
				handle->iface.dummyToLead_raised = bool_true;
				/* 'default' enter sequence for state Playing */
				/* 'default' enter sequence for region West */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.westRCard = bool_false;
				handle->stateConfVector[0] = ZBridgeServer_entry__Playing_West_Wait;
				handle->stateConfVectorPosition = 0;
				/* 'default' enter sequence for region North */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.northRCard = bool_false;
				handle->stateConfVector[1] = ZBridgeServer_entry__Playing_North_Wait;
				handle->stateConfVectorPosition = 1;
				/* 'default' enter sequence for region East */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.eastRCard = bool_false;
				handle->stateConfVector[2] = ZBridgeServer_entry__Playing_East_Wait;
				handle->stateConfVectorPosition = 2;
				/* 'default' enter sequence for region South */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.southRCard = bool_false;
				handle->stateConfVector[3] = ZBridgeServer_entry__Playing_South_Wait;
				handle->stateConfVectorPosition = 3;
			}  else {
				handle->iface.playerToLead_value = handle->internal.leader;
				handle->iface.playerToLead_raised = bool_true;
				/* 'default' enter sequence for state Playing */
				/* 'default' enter sequence for region West */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.westRCard = bool_false;
				handle->stateConfVector[0] = ZBridgeServer_entry__Playing_West_Wait;
				handle->stateConfVectorPosition = 0;
				/* 'default' enter sequence for region North */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.northRCard = bool_false;
				handle->stateConfVector[1] = ZBridgeServer_entry__Playing_North_Wait;
				handle->stateConfVectorPosition = 1;
				/* 'default' enter sequence for region East */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.eastRCard = bool_false;
				handle->stateConfVector[2] = ZBridgeServer_entry__Playing_East_Wait;
				handle->stateConfVectorPosition = 2;
				/* 'default' enter sequence for region South */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.southRCard = bool_false;
				handle->stateConfVector[3] = ZBridgeServer_entry__Playing_South_Wait;
				handle->stateConfVectorPosition = 3;
			}
		}
	}  else {
		if (handle->iface.newDeal_raised) { 
			/* Default exit sequence for state SyncLeader */
			handle->stateConfVector[0] = ZBridgeServer_last_state;
			handle->stateConfVectorPosition = 0;
			/* The reactions of state null. */
			handle->iface.newDealClients_raised = bool_true;
			handle->internal.noBoards += 1;
			/* The reactions of state null. */
			if (handle->internal.noBoards == handle->iface.noOfBoards) { 
				handle->iface.endOfSession_raised = bool_true;
				/* 'default' enter sequence for state Exit1 */
				handle->stateConfVector[0] = ZBridgeServer_entry__Exit1;
				handle->stateConfVectorPosition = 0;
			}  else {
				handle->iface.synchronize_raised = bool_true;
				/* 'default' enter sequence for state SyncSB */
				handle->stateConfVector[0] = ZBridgeServer_entry__SyncSB;
				handle->stateConfVectorPosition = 0;
			}
		}  else {
			if (handle->iface.undo_raised && handle->iface.undo_value == handle->internal.REPLAY) { 
				/* Default exit sequence for state SyncLeader */
				handle->stateConfVector[0] = ZBridgeServer_last_state;
				handle->stateConfVectorPosition = 0;
				/* The reactions of state null. */
				handle->iface.noTrick = 0;
				handle->internal.leader = (handle->iface.declarer + 1) & 3;
				handle->iface.player = handle->internal.leader;
				handle->internal.playNo = 0;
				handle->iface.undoTrick_value = handle->internal.REPLAY;
				handle->iface.undoTrick_raised = bool_true;
				handle->iface.synchronize_raised = bool_true;
				/* 'default' enter sequence for state SyncReplay */
				handle->stateConfVector[0] = ZBridgeServer_entry__SyncReplay;
				handle->stateConfVectorPosition = 0;
			}  else {
				if (handle->iface.undo_raised && handle->iface.undo_value >= 0) { 
					/* Default exit sequence for state SyncLeader */
					handle->stateConfVector[0] = ZBridgeServer_last_state;
					handle->stateConfVectorPosition = 0;
					handle->iface.noTrick -= 1;
					handle->iface.undoTrick_value = handle->internal.CT;
					handle->iface.undoTrick_raised = bool_true;
					/* The reactions of state null. */
					handle->internal.leader = handle->iface.undo_value;
					handle->iface.player = handle->internal.leader;
					handle->internal.playNo = 0;
					handle->iface.synchronize_raised = bool_true;
					/* 'default' enter sequence for state SyncLeader */
					handle->stateConfVector[0] = ZBridgeServer_entry__SyncLeader;
					handle->stateConfVectorPosition = 0;
				}  else {
					if (handle->iface.undo_raised && handle->iface.undo_value == handle->internal.REBID) { 
						/* Default exit sequence for state SyncLeader */
						handle->stateConfVector[0] = ZBridgeServer_last_state;
						handle->stateConfVectorPosition = 0;
						/* The reactions of state null. */
						handle->iface.undoPlay_raised = bool_true;
						/* The reactions of state null. */
						handle->iface.bidder = handle->iface.dealer;
						handle->internal.firstBidRound = bool_true;
						handle->internal.noPasses = 0;
						handle->iface.undoBid_value = handle->internal.REBID;
						handle->iface.undoBid_raised = bool_true;
						/* 'default' enter sequence for state Bidding */
						/* Entry action for state 'Bidding'. */
						handle->internal.westBid = bool_false;
						handle->internal.northBid = bool_false;
						handle->internal.eastBid = bool_false;
						handle->internal.southBid = bool_false;
						/* 'default' enter sequence for region West */
						/* Default react sequence for initial entry  */
						/* 'default' enter sequence for state Wait */
						handle->stateConfVector[0] = ZBridgeServer_entry__Bidding_West_Wait;
						handle->stateConfVectorPosition = 0;
						/* 'default' enter sequence for region North */
						/* Default react sequence for initial entry  */
						/* 'default' enter sequence for state Wait */
						handle->stateConfVector[1] = ZBridgeServer_entry__Bidding_North_Wait;
						handle->stateConfVectorPosition = 1;
						/* 'default' enter sequence for region East */
						/* Default react sequence for initial entry  */
						/* 'default' enter sequence for state Wait */
						handle->stateConfVector[2] = ZBridgeServer_entry__Bidding_East_Wait;
						handle->stateConfVectorPosition = 2;
						/* 'default' enter sequence for region South */
						/* Default react sequence for initial entry  */
						/* 'default' enter sequence for state Wait */
						handle->stateConfVector[3] = ZBridgeServer_entry__Bidding_South_Wait;
						handle->stateConfVectorPosition = 3;
					} 
				}
			}
		}
	}
}

/* The reactions of state SyncReplay. */
static void zBridgeServer_react_entry__SyncReplay(ZBridgeServer* handle) {
	/* The reactions of state SyncReplay. */
	if (handle->iface.allSync_raised) { 
		/* Default exit sequence for state SyncReplay */
		handle->stateConfVector[0] = ZBridgeServer_last_state;
		handle->stateConfVectorPosition = 0;
		/* The reactions of state null. */
		if (handle->iface.noTrick == 13) { 
			handle->internal.noBoards += 1;
			/* The reactions of state null. */
			if (handle->internal.noBoards == handle->iface.noOfBoards) { 
				handle->iface.endOfSession_raised = bool_true;
				/* 'default' enter sequence for state Exit1 */
				handle->stateConfVector[0] = ZBridgeServer_entry__Exit1;
				handle->stateConfVectorPosition = 0;
			}  else {
				handle->iface.synchronize_raised = bool_true;
				/* 'default' enter sequence for state SyncSB */
				handle->stateConfVector[0] = ZBridgeServer_entry__SyncSB;
				handle->stateConfVectorPosition = 0;
			}
		}  else {
			/* The reactions of state null. */
			if (handle->internal.leader == handle->iface.dummy) { 
				handle->iface.dummyToLead_value = handle->iface.declarer;
				handle->iface.dummyToLead_raised = bool_true;
				/* 'default' enter sequence for state Playing */
				/* 'default' enter sequence for region West */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.westRCard = bool_false;
				handle->stateConfVector[0] = ZBridgeServer_entry__Playing_West_Wait;
				handle->stateConfVectorPosition = 0;
				/* 'default' enter sequence for region North */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.northRCard = bool_false;
				handle->stateConfVector[1] = ZBridgeServer_entry__Playing_North_Wait;
				handle->stateConfVectorPosition = 1;
				/* 'default' enter sequence for region East */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.eastRCard = bool_false;
				handle->stateConfVector[2] = ZBridgeServer_entry__Playing_East_Wait;
				handle->stateConfVectorPosition = 2;
				/* 'default' enter sequence for region South */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.southRCard = bool_false;
				handle->stateConfVector[3] = ZBridgeServer_entry__Playing_South_Wait;
				handle->stateConfVectorPosition = 3;
			}  else {
				handle->iface.playerToLead_value = handle->internal.leader;
				handle->iface.playerToLead_raised = bool_true;
				/* 'default' enter sequence for state Playing */
				/* 'default' enter sequence for region West */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.westRCard = bool_false;
				handle->stateConfVector[0] = ZBridgeServer_entry__Playing_West_Wait;
				handle->stateConfVectorPosition = 0;
				/* 'default' enter sequence for region North */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.northRCard = bool_false;
				handle->stateConfVector[1] = ZBridgeServer_entry__Playing_North_Wait;
				handle->stateConfVectorPosition = 1;
				/* 'default' enter sequence for region East */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.eastRCard = bool_false;
				handle->stateConfVector[2] = ZBridgeServer_entry__Playing_East_Wait;
				handle->stateConfVectorPosition = 2;
				/* 'default' enter sequence for region South */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				/* Entry action for state 'Wait'. */
				handle->internal.southRCard = bool_false;
				handle->stateConfVector[3] = ZBridgeServer_entry__Playing_South_Wait;
				handle->stateConfVectorPosition = 3;
			}
		}
	} 
}

/* The reactions of state null. */
static void zBridgeServer_react_entry___sync0(ZBridgeServer* handle) {
	/* The reactions of state null. */
	handle->internal.noBoards = 1;
	/* 'default' enter sequence for state Deal */
	/* Entry action for state 'Deal'. */
	handle->internal.westRSBid = bool_false;
	handle->internal.northRSBid = bool_false;
	handle->internal.eastRSBid = bool_false;
	handle->internal.southRSBid = bool_false;
	/* 'default' enter sequence for region West */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state Info */
	handle->stateConfVector[0] = ZBridgeServer_entry__Deal_West_Info;
	handle->stateConfVectorPosition = 0;
	/* 'default' enter sequence for region North */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state Info */
	handle->stateConfVector[1] = ZBridgeServer_entry__Deal_North_Info;
	handle->stateConfVectorPosition = 1;
	/* 'default' enter sequence for region East */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state Info */
	handle->stateConfVector[2] = ZBridgeServer_entry__Deal_East_Info;
	handle->stateConfVectorPosition = 2;
	/* 'default' enter sequence for region South */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state Info */
	handle->stateConfVector[3] = ZBridgeServer_entry__Deal_South_Info;
	handle->stateConfVectorPosition = 3;
}

/* The reactions of state null. */
static void zBridgeServer_react_entry___sync1(ZBridgeServer* handle) {
	/* The reactions of state null. */
	handle->iface.bidder = handle->iface.dealer;
	handle->internal.firstBidRound = bool_true;
	handle->internal.noPasses = 0;
	handle->iface.synchronize_raised = bool_true;
	/* 'default' enter sequence for state SyncAuction */
	handle->stateConfVector[0] = ZBridgeServer_entry__SyncAuction;
	handle->stateConfVectorPosition = 0;
}

/* The reactions of state null. */
static void zBridgeServer_react_entry___sync2(ZBridgeServer* handle) {
	/* The reactions of state null. */
	handle->internal.curBidder = handle->iface.bidder;
	handle->iface.bidder += 1;
	handle->iface.bidder &= 3;
	/* The reactions of state null. */
	if (handle->iface.bidVal == handle->internal.BID_PASS) { 
		handle->internal.noPasses += 1;
		/* The reactions of state null. */
		if (! handle->internal.firstBidRound && (handle->internal.noPasses == 3)) { 
			handle->iface.declarer = handle->internal.lastBidder;
			handle->iface.dummy = (handle->iface.declarer + 2) & 3;
			handle->iface.noTrick = 0;
			handle->internal.leader = (handle->iface.declarer + 1) & 3;
			handle->iface.player = handle->internal.leader;
			handle->internal.playNo = 0;
			handle->iface.synchronize_raised = bool_true;
			/* 'default' enter sequence for state SyncPlay */
			handle->stateConfVector[0] = ZBridgeServer_entry__SyncPlay;
			handle->stateConfVectorPosition = 0;
		}  else {
			if (handle->internal.firstBidRound && (handle->internal.noPasses == 4)) { 
				handle->internal.noBoards += 1;
				/* The reactions of state null. */
				if (handle->internal.noBoards == handle->iface.noOfBoards) { 
					handle->iface.endOfSession_raised = bool_true;
					/* 'default' enter sequence for state Exit1 */
					handle->stateConfVector[0] = ZBridgeServer_entry__Exit1;
					handle->stateConfVectorPosition = 0;
				}  else {
					handle->iface.synchronize_raised = bool_true;
					/* 'default' enter sequence for state SyncSB */
					handle->stateConfVector[0] = ZBridgeServer_entry__SyncSB;
					handle->stateConfVectorPosition = 0;
				}
			}  else {
				/* 'default' enter sequence for state Bidding */
				/* Entry action for state 'Bidding'. */
				handle->internal.westBid = bool_false;
				handle->internal.northBid = bool_false;
				handle->internal.eastBid = bool_false;
				handle->internal.southBid = bool_false;
				/* 'default' enter sequence for region West */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				handle->stateConfVector[0] = ZBridgeServer_entry__Bidding_West_Wait;
				handle->stateConfVectorPosition = 0;
				/* 'default' enter sequence for region North */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				handle->stateConfVector[1] = ZBridgeServer_entry__Bidding_North_Wait;
				handle->stateConfVectorPosition = 1;
				/* 'default' enter sequence for region East */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				handle->stateConfVector[2] = ZBridgeServer_entry__Bidding_East_Wait;
				handle->stateConfVectorPosition = 2;
				/* 'default' enter sequence for region South */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state Wait */
				handle->stateConfVector[3] = ZBridgeServer_entry__Bidding_South_Wait;
				handle->stateConfVectorPosition = 3;
			}
		}
	}  else {
		handle->internal.firstBidRound = bool_false;
		handle->internal.noPasses = 0;
		/* The reactions of state null. */
		if ((handle->iface.bidVal == handle->internal.BID_DOUBLE) || (handle->iface.bidVal == handle->internal.BID_REDOUBLE)) { 
			handle->iface.bidDouble = handle->iface.bidVal;
			/* 'default' enter sequence for state Bidding */
			/* Entry action for state 'Bidding'. */
			handle->internal.westBid = bool_false;
			handle->internal.northBid = bool_false;
			handle->internal.eastBid = bool_false;
			handle->internal.southBid = bool_false;
			/* 'default' enter sequence for region West */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[0] = ZBridgeServer_entry__Bidding_West_Wait;
			handle->stateConfVectorPosition = 0;
			/* 'default' enter sequence for region North */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[1] = ZBridgeServer_entry__Bidding_North_Wait;
			handle->stateConfVectorPosition = 1;
			/* 'default' enter sequence for region East */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[2] = ZBridgeServer_entry__Bidding_East_Wait;
			handle->stateConfVectorPosition = 2;
			/* 'default' enter sequence for region South */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[3] = ZBridgeServer_entry__Bidding_South_Wait;
			handle->stateConfVectorPosition = 3;
		}  else {
			handle->internal.lastBidder = handle->internal.curBidder;
			handle->iface.lastBid = handle->iface.bidVal;
			handle->iface.bidDouble = handle->internal.BID_NONE;
			/* 'default' enter sequence for state Bidding */
			/* Entry action for state 'Bidding'. */
			handle->internal.westBid = bool_false;
			handle->internal.northBid = bool_false;
			handle->internal.eastBid = bool_false;
			handle->internal.southBid = bool_false;
			/* 'default' enter sequence for region West */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[0] = ZBridgeServer_entry__Bidding_West_Wait;
			handle->stateConfVectorPosition = 0;
			/* 'default' enter sequence for region North */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[1] = ZBridgeServer_entry__Bidding_North_Wait;
			handle->stateConfVectorPosition = 1;
			/* 'default' enter sequence for region East */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[2] = ZBridgeServer_entry__Bidding_East_Wait;
			handle->stateConfVectorPosition = 2;
			/* 'default' enter sequence for region South */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state Wait */
			handle->stateConfVector[3] = ZBridgeServer_entry__Bidding_South_Wait;
			handle->stateConfVectorPosition = 3;
		}
	}
}

/* The reactions of state null. */
static void zBridgeServer_react_entry___sync3(ZBridgeServer* handle) {
	/* The reactions of state null. */
	handle->internal.playNo += 1;
	handle->iface.player = (handle->internal.leader + handle->internal.playNo) & 3;
	/* The reactions of state null. */
	if (handle->internal.playNo < 4) { 
		/* 'default' enter sequence for state Playing */
		/* 'default' enter sequence for region West */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		/* Entry action for state 'Wait'. */
		handle->internal.westRCard = bool_false;
		handle->stateConfVector[0] = ZBridgeServer_entry__Playing_West_Wait;
		handle->stateConfVectorPosition = 0;
		/* 'default' enter sequence for region North */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		/* Entry action for state 'Wait'. */
		handle->internal.northRCard = bool_false;
		handle->stateConfVector[1] = ZBridgeServer_entry__Playing_North_Wait;
		handle->stateConfVectorPosition = 1;
		/* 'default' enter sequence for region East */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		/* Entry action for state 'Wait'. */
		handle->internal.eastRCard = bool_false;
		handle->stateConfVector[2] = ZBridgeServer_entry__Playing_East_Wait;
		handle->stateConfVectorPosition = 2;
		/* 'default' enter sequence for region South */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state Wait */
		/* Entry action for state 'Wait'. */
		handle->internal.southRCard = bool_false;
		handle->stateConfVector[3] = ZBridgeServer_entry__Playing_South_Wait;
		handle->stateConfVectorPosition = 3;
	}  else {
		handle->iface.noTrick += 1;
		handle->iface.getLeader_raised = bool_true;
		/* 'default' enter sequence for state WaitLeader */
		handle->stateConfVector[0] = ZBridgeServer_entry__WaitLeader;
		handle->stateConfVectorPosition = 0;
	}
}


