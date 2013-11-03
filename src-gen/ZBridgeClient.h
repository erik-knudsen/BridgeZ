
#ifndef ZBRIDGECLIENT_H_
#define ZBRIDGECLIENT_H_

#include "sc_types.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'ZBridgeClient'.
*/

//! enumeration of all states 
typedef enum {
	ZBridgeClient_main_region_Connecting ,
	ZBridgeClient_main_region__final_ ,
	ZBridgeClient_main_region_Seated ,
	ZBridgeClient_main_region_StartOfBoard ,
	ZBridgeClient_main_region_Info ,
	ZBridgeClient_main_region_Cards ,
	ZBridgeClient_main_region_Bid ,
	ZBridgeClient_main_region_Lead ,
	ZBridgeClient_main_region_DummyCards ,
	ZBridgeClient_main_region_Play ,
	ZBridgeClient_main_region_WaitLeader ,
	ZBridgeClient_main_region_Exit1 ,
	ZBridgeClient_main_region_Exit2 ,
	ZBridgeClient_last_state
} ZBridgeClientStates;

//! Type definition of the data structure for the ZBridgeClientInternal interface scope.
typedef struct {
	sc_integer  BID_NONE;
	sc_integer  BID_PASS;
	sc_integer  BID_DOUBLE;
	sc_integer  BID_REDOUBLE;
	sc_integer  curBidder;
	sc_boolean  firstBidRound;
	sc_integer  noPasses;
	sc_integer  lastBidder;
	sc_integer  leader;
	sc_integer  playNo;
	sc_boolean  firstTrick;
} ZBridgeClientInternal;

//! Type definition of the data structure for the ZBridgeClientIface interface scope.
typedef struct {
	sc_boolean endOfSession_raised;
	sc_boolean connect_raised;
	sc_boolean connectError_raised;
	sc_boolean seated_raised;
	sc_boolean rTNames_raised;
	sc_boolean teamNames_raised;
	sc_boolean rSBoard_raised;
	sc_boolean startOfBoard_raised;
	sc_boolean rDealInfo_raised;
	sc_boolean dealInfo_raised;
	sc_boolean rCards_raised;
	sc_boolean cards_raised;
	sc_boolean undo_raised;
	sc_integer  undo_value;
	sc_boolean giveBid_raised;
	sc_boolean rBid_raised;
	sc_boolean bidDone_raised;
	sc_integer  bidDone_value;
	sc_boolean bidInfo_raised;
	sc_integer  bidInfo_value;
	sc_boolean undoBid_raised;
	sc_integer  undoBid_value;
	sc_boolean readyForDummy_raised;
	sc_boolean clientPlays_raised;
	sc_boolean playerPlays_raised;
	sc_integer  playerPlays_value;
	sc_boolean readyForDummyCards_raised;
	sc_boolean readyForPlayer_raised;
	sc_boolean playerToLead_raised;
	sc_boolean dummyToLead_raised;
	sc_boolean dummyCards_raised;
	sc_boolean getLeader_raised;
	sc_boolean newLeader_raised;
	sc_integer  newLeader_value;
	sc_boolean undoTrick_raised;
	sc_integer  undoTrick_value;
	sc_integer  boardNumber;
	sc_integer  vulnerability;
	sc_integer  client;
	sc_integer  dealer;
	sc_integer  bidder;
	sc_integer  bidVal;
	sc_integer  lastBid;
	sc_integer  bidDouble;
	sc_integer  bidEnable;
	sc_integer  declarer;
	sc_integer  dummy;
	sc_integer  noTrick;
	sc_integer  player;
	sc_integer  cardVal;
} ZBridgeClientIface;


//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
#define ZBRIDGECLIENT_MAX_ORTHOGONAL_STATES 1

/*! Type definition of the data structure for the ZBridgeClient state machine.
This data structure has to be allocated by the client code. */
typedef struct {
	ZBridgeClientStates stateConfVector[ZBRIDGECLIENT_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	ZBridgeClientInternal internal;
	ZBridgeClientIface iface;
} ZBridgeClient;

/*! Initializes the ZBridgeClient state machine data structures. Must be called before first usage.*/
extern void zBridgeClient_init(ZBridgeClient* handle);

/*! Activates the state machine */
extern void zBridgeClient_enter(ZBridgeClient* handle);

/*! Deactivates the state machine */
extern void zBridgeClient_exit(ZBridgeClient* handle);

/*! Performs a 'run to completion' step. */
extern void zBridgeClient_runCycle(ZBridgeClient* handle);


/*! Raises the in event 'endOfSession' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_endOfSession(ZBridgeClient* handle);

/*! Checks if the out event 'connect' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_connect(ZBridgeClient* handle);

/*! Raises the in event 'connectError' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_connectError(ZBridgeClient* handle);

/*! Raises the in event 'seated' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_seated(ZBridgeClient* handle);

/*! Checks if the out event 'rTNames' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_rTNames(ZBridgeClient* handle);

/*! Raises the in event 'teamNames' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_teamNames(ZBridgeClient* handle);

/*! Checks if the out event 'rSBoard' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_rSBoard(ZBridgeClient* handle);

/*! Raises the in event 'startOfBoard' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_startOfBoard(ZBridgeClient* handle);

/*! Checks if the out event 'rDealInfo' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_rDealInfo(ZBridgeClient* handle);

/*! Raises the in event 'dealInfo' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_dealInfo(ZBridgeClient* handle);

/*! Checks if the out event 'rCards' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_rCards(ZBridgeClient* handle);

/*! Raises the in event 'cards' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_cards(ZBridgeClient* handle);

/*! Raises the in event 'undo' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_undo(ZBridgeClient* handle, sc_integer value);

/*! Checks if the out event 'giveBid' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_giveBid(ZBridgeClient* handle);

/*! Checks if the out event 'rBid' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_rBid(ZBridgeClient* handle);

/*! Raises the in event 'bidDone' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_bidDone(ZBridgeClient* handle, sc_integer value);

/*! Checks if the out event 'bidInfo' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_bidInfo(ZBridgeClient* handle);

/*! Gets the value of the out event 'bidInfo' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_bidInfo_value(ZBridgeClient* handle);

/*! Checks if the out event 'undoBid' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_undoBid(ZBridgeClient* handle);

/*! Gets the value of the out event 'undoBid' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_undoBid_value(ZBridgeClient* handle);

/*! Checks if the out event 'readyForDummy' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_readyForDummy(ZBridgeClient* handle);

/*! Checks if the out event 'clientPlays' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_clientPlays(ZBridgeClient* handle);

/*! Raises the in event 'playerPlays' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_playerPlays(ZBridgeClient* handle, sc_integer value);

/*! Checks if the out event 'readyForDummyCards' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_readyForDummyCards(ZBridgeClient* handle);

/*! Checks if the out event 'readyForPlayer' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_readyForPlayer(ZBridgeClient* handle);

/*! Raises the in event 'playerToLead' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_playerToLead(ZBridgeClient* handle);

/*! Raises the in event 'dummyToLead' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_dummyToLead(ZBridgeClient* handle);

/*! Raises the in event 'dummyCards' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_dummyCards(ZBridgeClient* handle);

/*! Checks if the out event 'getLeader' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_getLeader(ZBridgeClient* handle);

/*! Raises the in event 'newLeader' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_raise_newLeader(ZBridgeClient* handle, sc_integer value);

/*! Checks if the out event 'undoTrick' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientIface_israised_undoTrick(ZBridgeClient* handle);

/*! Gets the value of the out event 'undoTrick' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_undoTrick_value(ZBridgeClient* handle);

/*! Gets the value of the variable 'boardNumber' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_boardNumber(ZBridgeClient* handle);
/*! Sets the value of the variable 'boardNumber' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_boardNumber(ZBridgeClient* handle, sc_integer value);
/*! Gets the value of the variable 'vulnerability' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_vulnerability(ZBridgeClient* handle);
/*! Sets the value of the variable 'vulnerability' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_vulnerability(ZBridgeClient* handle, sc_integer value);
/*! Gets the value of the variable 'client' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_client(ZBridgeClient* handle);
/*! Sets the value of the variable 'client' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_client(ZBridgeClient* handle, sc_integer value);
/*! Gets the value of the variable 'dealer' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_dealer(ZBridgeClient* handle);
/*! Sets the value of the variable 'dealer' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_dealer(ZBridgeClient* handle, sc_integer value);
/*! Gets the value of the variable 'bidder' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_bidder(ZBridgeClient* handle);
/*! Sets the value of the variable 'bidder' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_bidder(ZBridgeClient* handle, sc_integer value);
/*! Gets the value of the variable 'bidVal' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_bidVal(ZBridgeClient* handle);
/*! Sets the value of the variable 'bidVal' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_bidVal(ZBridgeClient* handle, sc_integer value);
/*! Gets the value of the variable 'lastBid' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_lastBid(ZBridgeClient* handle);
/*! Sets the value of the variable 'lastBid' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_lastBid(ZBridgeClient* handle, sc_integer value);
/*! Gets the value of the variable 'bidDouble' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_bidDouble(ZBridgeClient* handle);
/*! Sets the value of the variable 'bidDouble' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_bidDouble(ZBridgeClient* handle, sc_integer value);
/*! Gets the value of the variable 'bidEnable' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_bidEnable(ZBridgeClient* handle);
/*! Sets the value of the variable 'bidEnable' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_bidEnable(ZBridgeClient* handle, sc_integer value);
/*! Gets the value of the variable 'declarer' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_declarer(ZBridgeClient* handle);
/*! Sets the value of the variable 'declarer' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_declarer(ZBridgeClient* handle, sc_integer value);
/*! Gets the value of the variable 'dummy' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_dummy(ZBridgeClient* handle);
/*! Sets the value of the variable 'dummy' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_dummy(ZBridgeClient* handle, sc_integer value);
/*! Gets the value of the variable 'noTrick' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_noTrick(ZBridgeClient* handle);
/*! Sets the value of the variable 'noTrick' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_noTrick(ZBridgeClient* handle, sc_integer value);
/*! Gets the value of the variable 'player' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_player(ZBridgeClient* handle);
/*! Sets the value of the variable 'player' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_player(ZBridgeClient* handle, sc_integer value);
/*! Gets the value of the variable 'cardVal' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientIface_get_cardVal(ZBridgeClient* handle);
/*! Sets the value of the variable 'cardVal' that is defined in the default interface scope. */ 
extern void zBridgeClientIface_set_cardVal(ZBridgeClient* handle, sc_integer value);


/*! Checks if the specified state is active. */
extern sc_boolean zBridgeClient_isActive(ZBridgeClient* handle, ZBridgeClientStates state);

#ifdef __cplusplus
}
#endif 

#endif /* ZBRIDGECLIENT_H_ */
