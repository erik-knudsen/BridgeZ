
#ifndef ZBRIDGECLIENTSYNC_H_
#define ZBRIDGECLIENTSYNC_H_

#include "sc_types.h"
		
#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'ZBridgeClientSync'.
*/


/*! Enumeration of all states */ 
typedef enum
{
	ZBridgeClientSync_last_state,
	ZBridgeClientSync_main_region_WaitForAttemptSync,
	ZBridgeClientSync_main_region__final_,
	ZBridgeClientSync_main_region_WaitForConfirmSync,
	ZBridgeClientSync_main_region_WaitForAllSync
} ZBridgeClientSyncStates;

/*! Type definition of the data structure for the ZBridgeClientSyncIface interface scope. */
typedef struct
{
	sc_boolean attemptSync_raised;
	sc_boolean sendAttemptSync_raised;
	sc_boolean confirmSync_raised;
	sc_boolean sendConfirmSync_raised;
	sc_boolean allSync_raised;
	sc_boolean okSync_raised;
	sc_integer syncState;
} ZBridgeClientSyncIface;


/*! Define dimension of the state configuration vector for orthogonal states. */
#define ZBRIDGECLIENTSYNC_MAX_ORTHOGONAL_STATES 1

/*! Define indices of states in the StateConfVector */
#define SCVI_ZBRIDGECLIENTSYNC_MAIN_REGION_WAITFORATTEMPTSYNC 0
#define SCVI_ZBRIDGECLIENTSYNC_MAIN_REGION__FINAL_ 0
#define SCVI_ZBRIDGECLIENTSYNC_MAIN_REGION_WAITFORCONFIRMSYNC 0
#define SCVI_ZBRIDGECLIENTSYNC_MAIN_REGION_WAITFORALLSYNC 0

/*! 
 * Type definition of the data structure for the ZBridgeClientSync state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
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
extern sc_boolean zBridgeClientSyncIface_israised_sendAttemptSync(const ZBridgeClientSync* handle);

/*! Raises the in event 'confirmSync' that is defined in the default interface scope. */ 
extern void zBridgeClientSyncIface_raise_confirmSync(ZBridgeClientSync* handle);

/*! Checks if the out event 'sendConfirmSync' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientSyncIface_israised_sendConfirmSync(const ZBridgeClientSync* handle);

/*! Raises the in event 'allSync' that is defined in the default interface scope. */ 
extern void zBridgeClientSyncIface_raise_allSync(ZBridgeClientSync* handle);

/*! Checks if the out event 'okSync' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeClientSyncIface_israised_okSync(const ZBridgeClientSync* handle);

/*! Gets the value of the variable 'syncState' that is defined in the default interface scope. */ 
extern sc_integer zBridgeClientSyncIface_get_syncState(const ZBridgeClientSync* handle);
/*! Sets the value of the variable 'syncState' that is defined in the default interface scope. */ 
extern void zBridgeClientSyncIface_set_syncState(ZBridgeClientSync* handle, sc_integer value);

/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean zBridgeClientSync_isActive(const ZBridgeClientSync* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean zBridgeClientSync_isFinal(const ZBridgeClientSync* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean zBridgeClientSync_isStateActive(const ZBridgeClientSync* handle, ZBridgeClientSyncStates state);



#ifdef __cplusplus
}
#endif 

#endif /* ZBRIDGECLIENTSYNC_H_ */
