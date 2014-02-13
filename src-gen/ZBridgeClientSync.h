
#ifndef ZBRIDGECLIENTSYNC_H_
#define ZBRIDGECLIENTSYNC_H_

#include "sc_types.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'ZBridgeClientSync'.
*/

//! enumeration of all states 
typedef enum {
	ZBridgeClientSync_main_region_WaitForAttemptSync ,
	ZBridgeClientSync_main_region__final_ ,
	ZBridgeClientSync_main_region_WaitForConfirmSync ,
	ZBridgeClientSync_main_region_WaitForAllSync ,
	ZBridgeClientSync_last_state
} ZBridgeClientSyncStates;

//! Type definition of the data structure for the ZBridgeClientSyncIface interface scope.
typedef struct {
	sc_boolean attemptSync_raised;
	sc_boolean sendAttemptSync_raised;
	sc_boolean confirmSync_raised;
	sc_boolean sendConfirmSync_raised;
	sc_boolean allSync_raised;
	sc_boolean okSync_raised;
	sc_integer  syncState;
} ZBridgeClientSyncIface;


//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
#define ZBRIDGECLIENTSYNC_MAX_ORTHOGONAL_STATES 1

/*! Type definition of the data structure for the ZBridgeClientSync state machine.
This data structure has to be allocated by the client code. */
typedef struct {
	ZBridgeClientSyncStates stateConfVector[ZBRIDGECLIENTSYNC_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	ZBridgeClientSyncIface iface;
} ZBridgeClientSync;

/*! Initializes the ZBridgeClientSync state machine data structures. Must be called before first usage.*/
extern void zBridgeClientSync_init(ZBridgeClientSync* handle);

/*! Activates the state machine */
extern void zBridgeClientSync_enter(ZBridgeClientSync* handle);

/*! Deactivates the state machine */
extern void zBridgeClientSync_exit(ZBridgeClientSync* handle);

/*! Performs a 'run to completion' step. */
extern void zBridgeClientSync_runCycle(ZBridgeClientSync* handle);


/*! Raises the in event 'attemptSync' that is defined in the default interface scope. */ 
extern void zBridgeClientSyncIface_raise_attemptSync(ZBridgeClientSync* handle);

/*! Checks if the out event 'sendAttemptSync' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientSyncIface_israised_sendAttemptSync(ZBridgeClientSync* handle);

/*! Raises the in event 'confirmSync' that is defined in the default interface scope. */ 
extern void zBridgeClientSyncIface_raise_confirmSync(ZBridgeClientSync* handle);

/*! Checks if the out event 'sendConfirmSync' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientSyncIface_israised_sendConfirmSync(ZBridgeClientSync* handle);

/*! Raises the in event 'allSync' that is defined in the default interface scope. */ 
extern void zBridgeClientSyncIface_raise_allSync(ZBridgeClientSync* handle);

/*! Checks if the out event 'okSync' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientSyncIface_israised_okSync(ZBridgeClientSync* handle);

/*! Gets the value of the variable 'syncState' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientSyncIface_get_syncState(ZBridgeClientSync* handle);
/*! Sets the value of the variable 'syncState' that is defined in the default interface scope. */ 
extern void zBridgeClientSyncIface_set_syncState(ZBridgeClientSync* handle, sc_integer value);


/*! Checks if the specified state is active. */
extern sc_boolean zBridgeClientSync_isActive(ZBridgeClientSync* handle, ZBridgeClientSyncStates state);

#ifdef __cplusplus
}
#endif 

#endif /* ZBRIDGECLIENTSYNC_H_ */
