
#ifndef ZBRIDGESERVER_H_
#define ZBRIDGESERVER_H_

#include "sc_types.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'ZBridgeServer'.
*/

//! enumeration of all states 
typedef enum {
	ZBridgeServer_entry__Connect ,
	ZBridgeServer_entry__Connect_West_Seated ,
	ZBridgeServer_entry__Connect_West_TeamNames ,
	ZBridgeServer_entry__Connect_West_StartOfBoard ,
	ZBridgeServer_entry__Connect_West_Connect ,
	ZBridgeServer_entry__Connect_North_Seated ,
	ZBridgeServer_entry__Connect_North_TeamNames ,
	ZBridgeServer_entry__Connect_North_StartOfBoard ,
	ZBridgeServer_entry__Connect_North_Connect ,
	ZBridgeServer_entry__Connect_East_Seated ,
	ZBridgeServer_entry__Connect_East_TeamNames ,
	ZBridgeServer_entry__Connect_East_StartOfBoard ,
	ZBridgeServer_entry__Connect_East_Connect ,
	ZBridgeServer_entry__Connect_South_Seated ,
	ZBridgeServer_entry__Connect_South_TeamNames ,
	ZBridgeServer_entry__Connect_South_StartOfBoard ,
	ZBridgeServer_entry__Connect_South_Connect ,
	ZBridgeServer_entry__Deal ,
	ZBridgeServer_entry__Deal_West_Info ,
	ZBridgeServer_entry__Deal_West_Cards ,
	ZBridgeServer_entry__Deal_West_Sync ,
	ZBridgeServer_entry__Deal_North_Info ,
	ZBridgeServer_entry__Deal_North_Cards ,
	ZBridgeServer_entry__Deal_North_Sync ,
	ZBridgeServer_entry__Deal_East_Info ,
	ZBridgeServer_entry__Deal_East_Cards ,
	ZBridgeServer_entry__Deal_East_Sync ,
	ZBridgeServer_entry__Deal_South_Info ,
	ZBridgeServer_entry__Deal_South_Cards ,
	ZBridgeServer_entry__Deal_South_Sync ,
	ZBridgeServer_entry__Bidding ,
	ZBridgeServer_entry__Bidding_West_Wait ,
	ZBridgeServer_entry__Bidding_West_Sync ,
	ZBridgeServer_entry__Bidding_North_Wait ,
	ZBridgeServer_entry__Bidding_North_Sync ,
	ZBridgeServer_entry__Bidding_East_Wait ,
	ZBridgeServer_entry__Bidding_East_Sync ,
	ZBridgeServer_entry__Bidding_South_Wait ,
	ZBridgeServer_entry__Bidding_South_Sync ,
	ZBridgeServer_entry__Playing ,
	ZBridgeServer_entry__Playing_West_Wait ,
	ZBridgeServer_entry__Playing_West_Sync ,
	ZBridgeServer_entry__Playing_North_Wait ,
	ZBridgeServer_entry__Playing_North_Sync ,
	ZBridgeServer_entry__Playing_East_Wait ,
	ZBridgeServer_entry__Playing_East_Sync ,
	ZBridgeServer_entry__Playing_South_Wait ,
	ZBridgeServer_entry__Playing_South_Sync ,
	ZBridgeServer_entry__Exit1 ,
	ZBridgeServer_entry__Exit2 ,
	ZBridgeServer_entry__Exit3 ,
	ZBridgeServer_entry__WaitLeader ,
	ZBridgeServer_entry__SyncSB ,
	ZBridgeServer_entry__SyncAuction ,
	ZBridgeServer_entry__SyncPlay ,
	ZBridgeServer_entry__SyncLeader ,
	ZBridgeServer_last_state
} ZBridgeServerStates;

//! Type definition of the data structure for the ZBridgeServerInternal interface scope.
typedef struct {
	sc_integer BID_NONE;
	sc_integer BID_PASS;
	sc_integer BID_DOUBLE;
	sc_integer BID_REDOUBLE;
	sc_integer W;
	sc_integer N;
	sc_integer E;
	sc_integer S;
	sc_boolean westConnected;
	sc_boolean northConnected;
	sc_boolean eastConnected;
	sc_boolean southConnected;
	sc_boolean westRTNames;
	sc_boolean northRTNames;
	sc_boolean eastRTNames;
	sc_boolean southRTNames;
	sc_boolean westRSBoard;
	sc_boolean northRSBoard;
	sc_boolean eastRSBoard;
	sc_boolean southRSBoard;
	sc_boolean westRSBid;
	sc_boolean northRSBid;
	sc_boolean eastRSBid;
	sc_boolean southRSBid;
	sc_boolean westBid;
	sc_boolean northBid;
	sc_boolean eastBid;
	sc_boolean southBid;
	sc_integer noBoards;
	sc_boolean westRCard;
	sc_boolean northRCard;
	sc_boolean eastRCard;
	sc_boolean southRCard;
	sc_integer curBidder;
	sc_boolean firstBidRound;
	sc_integer lastBidder;
	sc_integer noPasses;
	sc_integer leader;
	sc_integer playNo;
} ZBridgeServerInternal;

//! Type definition of the data structure for the ZBridgeServerIface interface scope.
typedef struct {
	sc_boolean newSession_raised;
	sc_boolean connect_raised;
	sc_integer connect_value;
	sc_boolean rTNames_raised;
	sc_integer rTNames_value;
	sc_boolean rSBoard_raised;
	sc_integer rSBoard_value;
	sc_boolean seated_raised;
	sc_integer seated_value;
	sc_boolean teamNames_raised;
	sc_boolean newDealClients_raised;
	sc_boolean startOfBoard_raised;
	sc_boolean startOfBoardDelayed_raised;
	sc_boolean continue_raised;
	sc_boolean rDealInfo_raised;
	sc_integer rDealInfo_value;
	sc_boolean dealInfo_raised;
	sc_integer dealInfo_value;
	sc_boolean rCards_raised;
	sc_integer rCards_value;
	sc_boolean cards_raised;
	sc_boolean newDeal_raised;
	sc_boolean undo_raised;
	sc_integer undo_value;
	sc_boolean bid_raised;
	sc_integer bid_value;
	sc_boolean rBid_raised;
	sc_integer rBid_value;
	sc_boolean bidDone_raised;
	sc_integer bidDone_value;
	sc_boolean bidInfo_raised;
	sc_integer bidInfo_value;
	sc_boolean undoBid_raised;
	sc_integer undoBid_value;
	sc_boolean playerPlays_raised;
	sc_integer playerPlays_value;
	sc_boolean readyForPlayer_raised;
	sc_integer readyForPlayer_value;
	sc_boolean playerToLead_raised;
	sc_integer playerToLead_value;
	sc_boolean readyForDummy_raised;
	sc_integer readyForDummy_value;
	sc_boolean dummyToLead_raised;
	sc_integer dummyToLead_value;
	sc_boolean sendPlayerPlays_raised;
	sc_integer sendPlayerPlays_value;
	sc_boolean readyForDummyCards_raised;
	sc_integer readyForDummyCards_value;
	sc_boolean dummyCards_raised;
	sc_integer dummyCards_value;
	sc_boolean getLeader_raised;
	sc_boolean newLeader_raised;
	sc_integer newLeader_value;
	sc_boolean undoTrick_raised;
	sc_integer undoTrick_value;
	sc_boolean allSync_raised;
	sc_boolean synchronize_raised;
	sc_boolean endOfSession_raised;
	sc_integer noOfBoards;
	sc_string name;
	sc_string nsTeamName;
	sc_string ewTeamName;
	sc_integer dealer;
	sc_integer bidVal;
	sc_integer bidder;
	sc_integer lastBid;
	sc_integer bidDouble;
	sc_integer declarer;
	sc_integer dummy;
	sc_integer player;
	sc_integer noTrick;
	sc_integer cardVal;
} ZBridgeServerIface;


//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
#define ZBRIDGESERVER_MAX_ORTHOGONAL_STATES 4

/*! Type definition of the data structure for the ZBridgeServer state machine.
This data structure has to be allocated by the client code. */
typedef struct {
	ZBridgeServerStates stateConfVector[ZBRIDGESERVER_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	ZBridgeServerInternal internal;
	ZBridgeServerIface iface;
} ZBridgeServer;

/*! Initializes the ZBridgeServer state machine data structures. Must be called before first usage.*/
extern void zBridgeServer_init(ZBridgeServer* handle);

/*! Activates the state machine */
extern void zBridgeServer_enter(ZBridgeServer* handle);

/*! Deactivates the state machine */
extern void zBridgeServer_exit(ZBridgeServer* handle);

/*! Performs a 'run to completion' step. */
extern void zBridgeServer_runCycle(ZBridgeServer* handle);


/*! Raises the in event 'newSession' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_newSession(ZBridgeServer* handle);

/*! Raises the in event 'connect' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_connect(ZBridgeServer* handle, sc_integer value);

/*! Raises the in event 'rTNames' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_rTNames(ZBridgeServer* handle, sc_integer value);

/*! Raises the in event 'rSBoard' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_rSBoard(ZBridgeServer* handle, sc_integer value);

/*! Checks if the out event 'seated' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_seated(ZBridgeServer* handle);

/*! Gets the value of the out event 'seated' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_seated_value(ZBridgeServer* handle);

/*! Checks if the out event 'teamNames' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_teamNames(ZBridgeServer* handle);

/*! Checks if the out event 'newDealClients' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_newDealClients(ZBridgeServer* handle);

/*! Checks if the out event 'startOfBoard' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_startOfBoard(ZBridgeServer* handle);

/*! Checks if the out event 'startOfBoardDelayed' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_startOfBoardDelayed(ZBridgeServer* handle);

/*! Raises the in event 'continue' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_continue(ZBridgeServer* handle);

/*! Raises the in event 'rDealInfo' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_rDealInfo(ZBridgeServer* handle, sc_integer value);

/*! Checks if the out event 'dealInfo' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_dealInfo(ZBridgeServer* handle);

/*! Gets the value of the out event 'dealInfo' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_dealInfo_value(ZBridgeServer* handle);

/*! Raises the in event 'rCards' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_rCards(ZBridgeServer* handle, sc_integer value);

/*! Checks if the out event 'cards' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_cards(ZBridgeServer* handle);

/*! Raises the in event 'newDeal' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_newDeal(ZBridgeServer* handle);

/*! Raises the in event 'undo' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_undo(ZBridgeServer* handle, sc_integer value);

/*! Raises the in event 'bid' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_bid(ZBridgeServer* handle, sc_integer value);

/*! Raises the in event 'rBid' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_rBid(ZBridgeServer* handle, sc_integer value);

/*! Checks if the out event 'bidDone' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_bidDone(ZBridgeServer* handle);

/*! Gets the value of the out event 'bidDone' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_bidDone_value(ZBridgeServer* handle);

/*! Checks if the out event 'bidInfo' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_bidInfo(ZBridgeServer* handle);

/*! Gets the value of the out event 'bidInfo' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_bidInfo_value(ZBridgeServer* handle);

/*! Checks if the out event 'undoBid' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_undoBid(ZBridgeServer* handle);

/*! Gets the value of the out event 'undoBid' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_undoBid_value(ZBridgeServer* handle);

/*! Raises the in event 'playerPlays' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_playerPlays(ZBridgeServer* handle, sc_integer value);

/*! Raises the in event 'readyForPlayer' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_readyForPlayer(ZBridgeServer* handle, sc_integer value);

/*! Checks if the out event 'playerToLead' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_playerToLead(ZBridgeServer* handle);

/*! Gets the value of the out event 'playerToLead' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_playerToLead_value(ZBridgeServer* handle);

/*! Raises the in event 'readyForDummy' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_readyForDummy(ZBridgeServer* handle, sc_integer value);

/*! Checks if the out event 'dummyToLead' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_dummyToLead(ZBridgeServer* handle);

/*! Gets the value of the out event 'dummyToLead' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_dummyToLead_value(ZBridgeServer* handle);

/*! Checks if the out event 'sendPlayerPlays' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_sendPlayerPlays(ZBridgeServer* handle);

/*! Gets the value of the out event 'sendPlayerPlays' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_sendPlayerPlays_value(ZBridgeServer* handle);

/*! Raises the in event 'readyForDummyCards' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_readyForDummyCards(ZBridgeServer* handle, sc_integer value);

/*! Checks if the out event 'dummyCards' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_dummyCards(ZBridgeServer* handle);

/*! Gets the value of the out event 'dummyCards' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_dummyCards_value(ZBridgeServer* handle);

/*! Checks if the out event 'getLeader' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_getLeader(ZBridgeServer* handle);

/*! Raises the in event 'newLeader' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_newLeader(ZBridgeServer* handle, sc_integer value);

/*! Checks if the out event 'undoTrick' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_undoTrick(ZBridgeServer* handle);

/*! Gets the value of the out event 'undoTrick' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_undoTrick_value(ZBridgeServer* handle);

/*! Raises the in event 'allSync' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_raise_allSync(ZBridgeServer* handle);

/*! Checks if the out event 'synchronize' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_synchronize(ZBridgeServer* handle);

/*! Checks if the out event 'endOfSession' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerIface_israised_endOfSession(ZBridgeServer* handle);

/*! Gets the value of the variable 'noOfBoards' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_noOfBoards(ZBridgeServer* handle);
/*! Sets the value of the variable 'noOfBoards' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_noOfBoards(ZBridgeServer* handle, sc_integer value);
/*! Gets the value of the variable 'name' that is defined in the default interface scope. */ 
extern sc_string zBridgeServerIface_get_name(ZBridgeServer* handle);
/*! Sets the value of the variable 'name' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_name(ZBridgeServer* handle, sc_string value);
/*! Gets the value of the variable 'nsTeamName' that is defined in the default interface scope. */ 
extern sc_string zBridgeServerIface_get_nsTeamName(ZBridgeServer* handle);
/*! Sets the value of the variable 'nsTeamName' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_nsTeamName(ZBridgeServer* handle, sc_string value);
/*! Gets the value of the variable 'ewTeamName' that is defined in the default interface scope. */ 
extern sc_string zBridgeServerIface_get_ewTeamName(ZBridgeServer* handle);
/*! Sets the value of the variable 'ewTeamName' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_ewTeamName(ZBridgeServer* handle, sc_string value);
/*! Gets the value of the variable 'dealer' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_dealer(ZBridgeServer* handle);
/*! Sets the value of the variable 'dealer' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_dealer(ZBridgeServer* handle, sc_integer value);
/*! Gets the value of the variable 'bidVal' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_bidVal(ZBridgeServer* handle);
/*! Sets the value of the variable 'bidVal' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_bidVal(ZBridgeServer* handle, sc_integer value);
/*! Gets the value of the variable 'bidder' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_bidder(ZBridgeServer* handle);
/*! Sets the value of the variable 'bidder' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_bidder(ZBridgeServer* handle, sc_integer value);
/*! Gets the value of the variable 'lastBid' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_lastBid(ZBridgeServer* handle);
/*! Sets the value of the variable 'lastBid' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_lastBid(ZBridgeServer* handle, sc_integer value);
/*! Gets the value of the variable 'bidDouble' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_bidDouble(ZBridgeServer* handle);
/*! Sets the value of the variable 'bidDouble' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_bidDouble(ZBridgeServer* handle, sc_integer value);
/*! Gets the value of the variable 'declarer' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_declarer(ZBridgeServer* handle);
/*! Sets the value of the variable 'declarer' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_declarer(ZBridgeServer* handle, sc_integer value);
/*! Gets the value of the variable 'dummy' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_dummy(ZBridgeServer* handle);
/*! Sets the value of the variable 'dummy' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_dummy(ZBridgeServer* handle, sc_integer value);
/*! Gets the value of the variable 'player' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_player(ZBridgeServer* handle);
/*! Sets the value of the variable 'player' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_player(ZBridgeServer* handle, sc_integer value);
/*! Gets the value of the variable 'noTrick' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_noTrick(ZBridgeServer* handle);
/*! Sets the value of the variable 'noTrick' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_noTrick(ZBridgeServer* handle, sc_integer value);
/*! Gets the value of the variable 'cardVal' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerIface_get_cardVal(ZBridgeServer* handle);
/*! Sets the value of the variable 'cardVal' that is defined in the default interface scope. */ 
extern void zBridgeServerIface_set_cardVal(ZBridgeServer* handle, sc_integer value);


/*! Checks if the specified state is active. */
extern sc_boolean zBridgeServer_isActive(ZBridgeServer* handle, ZBridgeServerStates state);

#ifdef __cplusplus
}
#endif 

#endif /* ZBRIDGESERVER_H_ */
