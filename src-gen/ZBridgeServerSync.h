
#ifndef ZBRIDGESERVERSYNC_H_
#define ZBRIDGESERVERSYNC_H_

#include "sc_types.h"
		
#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'ZBridgeServerSync'.
*/


/*! Enumeration of all states */ 
typedef enum
{
	ZBridgeServerSync_last_state,
	ZBridgeServerSync_entry_SynChronizeServer,
	ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync,
	ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync,
	ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync,
	ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync,
	ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync,
	ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync,
	ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync,
	ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync,
	ZBridgeServerSync_entry_ClientConfirm,
	ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync,
	ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync,
	ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync,
	ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync,
	ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync,
	ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync,
	ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync,
	ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync,
	ZBridgeServerSync_entry__final_,
	ZBridgeServerSync_entry_AttemptSync
} ZBridgeServerSyncStates;

/*! Type definition of the data structure for the ZBridgeServerSyncInternal interface scope. */
typedef struct
{
	sc_integer W;
	sc_integer N;
	sc_integer E;
	sc_integer S;
	sc_boolean confirmWest;
	sc_boolean confirmNorth;
	sc_boolean confirmEast;
	sc_boolean confirmSouth;
} ZBridgeServerSyncInternal;

/*! Type definition of the data structure for the ZBridgeServerSyncIface interface scope. */
typedef struct
{
	sc_boolean attemptSync_raised;
	sc_integer attemptSync_value;
	sc_boolean continue_raised;
	sc_boolean sendAttemptSync_raised;
	sc_integer sendAttemptSync_value;
	sc_boolean sendAttemptSyncAll_raised;
	sc_boolean sendConfirmSync_raised;
	sc_boolean confirmSync_raised;
	sc_integer confirmSync_value;
	sc_boolean sendAllSync_raised;
} ZBridgeServerSyncIface;


/*! Define dimension of the state configuration vector for orthogonal states. */
#define ZBRIDGESERVERSYNC_MAX_ORTHOGONAL_STATES 4

/*! Define indices of states in the StateConfVector */
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_SYNCHRONIZESERVER 0
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_SYNCHRONIZESERVER_WEST_WAITFORATTEMPTSYNC 0
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_SYNCHRONIZESERVER_WEST_WAITFORCONFIRMSYNC 0
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_SYNCHRONIZESERVER_NORTH_WAITFORATTEMPTSYNC 1
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_SYNCHRONIZESERVER_NORTH_WAITFORCONFIRMSYNC 1
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_SYNCHRONIZESERVER_EAST_WAITFORATTEMPTSYNC 2
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_SYNCHRONIZESERVER_EAST_WAITFORCONFIRMSYNC 2
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_SYNCHRONIZESERVER_SOUTH_WAITFORATTEMPTSYNC 3
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_SYNCHRONIZESERVER_SOUTH_WAITFORCONFIRMSYNC 3
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_CLIENTCONFIRM 0
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_CLIENTCONFIRM_WEST_WAITFORCONFIRMSYNC 0
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_CLIENTCONFIRM_WEST_WAITFORSYNC 0
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_CLIENTCONFIRM_NORTH_WAITFORCONFIRMSYNC 1
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_CLIENTCONFIRM_NORTH_WAITFORSYNC 1
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_CLIENTCONFIRM_EAST_WAITFORCONFIRMSYNC 2
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_CLIENTCONFIRM_EAST_WAITFORSYNC 2
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_CLIENTCONFIRM_SOUTH_WAITFORCONFIRMSYNC 3
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_CLIENTCONFIRM_SOUTH_WAITFORSYNC 3
#define SCVI_ZBRIDGESERVERSYNC_ENTRY__FINAL_ 0
#define SCVI_ZBRIDGESERVERSYNC_ENTRY_ATTEMPTSYNC 0

/*! 
 * Type definition of the data structure for the ZBridgeServerSync state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	ZBridgeServerSyncStates stateConfVector[ZBRIDGESERVERSYNC_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	ZBridgeServerSyncInternal internal;
	ZBridgeServerSyncIface iface;
} ZBridgeServerSync;


/*! Initializes the ZBridgeServerSync state machine data structures. Must be called before first usage.*/
extern void zBridgeServerSync_init(ZBridgeServerSync* handle);

/*! Activates the state machine */
extern void zBridgeServerSync_enter(ZBridgeServerSync* handle);

/*! Deactivates the state machine */
extern void zBridgeServerSync_exit(ZBridgeServerSync* handle);

/*! Performs a 'run to completion' step. */
extern void zBridgeServerSync_runCycle(ZBridgeServerSync* handle);


/*! Raises the in event 'attemptSync' that is defined in the default interface scope. */ 
extern void zBridgeServerSyncIface_raise_attemptSync(ZBridgeServerSync* handle, sc_integer value);

/*! Raises the in event 'continue' that is defined in the default interface scope. */ 
extern void zBridgeServerSyncIface_raise_continue(ZBridgeServerSync* handle);

/*! Checks if the out event 'sendAttemptSync' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerSyncIface_israised_sendAttemptSync(const ZBridgeServerSync* handle);

/*! Gets the value of the out event 'sendAttemptSync' that is defined in the default interface scope. */ 
extern sc_integer zBridgeServerSyncIface_get_sendAttemptSync_value(const ZBridgeServerSync* handle);

/*! Checks if the out event 'sendAttemptSyncAll' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerSyncIface_israised_sendAttemptSyncAll(const ZBridgeServerSync* handle);

/*! Checks if the out event 'sendConfirmSync' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerSyncIface_israised_sendConfirmSync(const ZBridgeServerSync* handle);

/*! Raises the in event 'confirmSync' that is defined in the default interface scope. */ 
extern void zBridgeServerSyncIface_raise_confirmSync(ZBridgeServerSync* handle, sc_integer value);

/*! Checks if the out event 'sendAllSync' that is defined in the default interface scope has been raised. */ 
extern sc_boolean zBridgeServerSyncIface_israised_sendAllSync(const ZBridgeServerSync* handle);


/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean zBridgeServerSync_isActive(const ZBridgeServerSync* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean zBridgeServerSync_isFinal(const ZBridgeServerSync* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean zBridgeServerSync_isStateActive(const ZBridgeServerSync* handle, ZBridgeServerSyncStates state);



#ifdef __cplusplus
}
#endif 

#endif /* ZBRIDGESERVERSYNC_H_ */
