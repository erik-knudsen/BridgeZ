
#include <stdlib.h>
#include <string.h>
#include "sc_types.h"
#include "ZBridgeServerSync.h"
/*! \file Implementation of the state machine 'ZBridgeServerSync'
*/

// prototypes of all internal functions

static void zBridgeServerSync_enact_SequenceImpl(ZBridgeServerSync* handle);
static void zBridgeServerSync_exact_SequenceImpl(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_SynChronizeServer_West_WaitForAttemptSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_SynChronizeServer_West_waitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_SynChronizeServer_North_WaitForAttemptSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_SynChronizeServer_North_waitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_SynChronizeServer_East_WaitForAttemptSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_SynChronizeServer_East_waitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_SynChronizeServer_South_WaitForAttemptSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_SynChronizeServer_South_waitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_ClientConfirm_West_WaitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_ClientConfirm_West_WaitForSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_ClientConfirm_North_WaitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_ClientConfirm_North_WaitForSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_ClientConfirm_East_WaitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_ClientConfirm_East_WaitForSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_ClientConfirm_South_WaitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_ClientConfirm_South_WaitForSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry__final_(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_AttemptSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry__sync0(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry__sync1(ZBridgeServerSync* handle);
static void zBridgeServerSync_clearInEvents(ZBridgeServerSync* handle);
static void zBridgeServerSync_clearOutEvents(ZBridgeServerSync* handle);


void zBridgeServerSync_init(ZBridgeServerSync* handle)
{
	int i;

	for (i = 0; i < ZBRIDGESERVERSYNC_MAX_ORTHOGONAL_STATES; ++i)
		handle->stateConfVector[i] = ZBridgeServerSync_last_state;
	
	
	handle->stateConfVectorPosition = 0;

	zBridgeServerSync_clearInEvents(handle);
	zBridgeServerSync_clearOutEvents(handle);

	/* Default init sequence for statechart ZBridgeServerSync */
	handle->internal.W = 0;
	handle->internal.N = 1;
	handle->internal.E = 2;
	handle->internal.S = 3;
	handle->internal.confirmWest = bool_false;
	handle->internal.confirmNorth = bool_false;
	handle->internal.confirmEast = bool_false;
	handle->internal.confirmSouth = bool_false;

}

void zBridgeServerSync_enter(ZBridgeServerSync* handle)
{
	/* Default enter sequence for statechart ZBridgeServerSync */
	zBridgeServerSync_enact_SequenceImpl(handle);
	/* 'default' enter sequence for region entry */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state AttemptSync */
	handle->stateConfVector[0] = ZBridgeServerSync_entry_AttemptSync;
	handle->stateConfVectorPosition = 0;
}

void zBridgeServerSync_exit(ZBridgeServerSync* handle)
{
	/* Default exit sequence for statechart ZBridgeServerSync */
	/* Default exit sequence for region entry */
	/* Handle exit of all possible states (of entry) at position 0... */
	switch(handle->stateConfVector[ 0 ]) {
		case ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync : {
			/* Default exit sequence for state WaitForAttemptSync */
			handle->stateConfVector[0] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync : {
			/* Default exit sequence for state waitForConfirmSync */
			handle->stateConfVector[0] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync : {
			/* Default exit sequence for state WaitForConfirmSync */
			handle->stateConfVector[0] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync : {
			/* Default exit sequence for state WaitForSync */
			handle->stateConfVector[0] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServerSync_entry__final_ : {
			/* Default exit sequence for final state. */
			handle->stateConfVector[0] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case ZBridgeServerSync_entry_AttemptSync : {
			/* Default exit sequence for state AttemptSync */
			handle->stateConfVector[0] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		default: break;
	}
	/* Handle exit of all possible states (of entry) at position 1... */
	switch(handle->stateConfVector[ 1 ]) {
		case ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync : {
			/* Default exit sequence for state WaitForAttemptSync */
			handle->stateConfVector[1] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync : {
			/* Default exit sequence for state waitForConfirmSync */
			handle->stateConfVector[1] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync : {
			/* Default exit sequence for state WaitForConfirmSync */
			handle->stateConfVector[1] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync : {
			/* Default exit sequence for state WaitForSync */
			handle->stateConfVector[1] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		default: break;
	}
	/* Handle exit of all possible states (of entry) at position 2... */
	switch(handle->stateConfVector[ 2 ]) {
		case ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync : {
			/* Default exit sequence for state WaitForAttemptSync */
			handle->stateConfVector[2] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 2;
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync : {
			/* Default exit sequence for state waitForConfirmSync */
			handle->stateConfVector[2] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 2;
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync : {
			/* Default exit sequence for state WaitForConfirmSync */
			handle->stateConfVector[2] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 2;
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync : {
			/* Default exit sequence for state WaitForSync */
			handle->stateConfVector[2] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 2;
			break;
		}
		default: break;
	}
	/* Handle exit of all possible states (of entry) at position 3... */
	switch(handle->stateConfVector[ 3 ]) {
		case ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync : {
			/* Default exit sequence for state WaitForAttemptSync */
			handle->stateConfVector[3] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 3;
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync : {
			/* Default exit sequence for state waitForConfirmSync */
			handle->stateConfVector[3] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 3;
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync : {
			/* Default exit sequence for state WaitForConfirmSync */
			handle->stateConfVector[3] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 3;
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync : {
			/* Default exit sequence for state WaitForSync */
			handle->stateConfVector[3] = ZBridgeServerSync_last_state;
			handle->stateConfVectorPosition = 3;
			break;
		}
		default: break;
	}
	zBridgeServerSync_exact_SequenceImpl(handle);
}

static void zBridgeServerSync_clearInEvents(ZBridgeServerSync* handle) {
	handle->iface.attemptSync_raised = bool_false;
	handle->iface.continue_raised = bool_false;
	handle->iface.confirmSync_raised = bool_false;
}

static void zBridgeServerSync_clearOutEvents(ZBridgeServerSync* handle) {
	handle->iface.sendAttemptSync_raised = bool_false;
	handle->iface.sendAttemptSyncAll_raised = bool_false;
	handle->iface.sendConfirmSync_raised = bool_false;
	handle->iface.sendAllSync_raised = bool_false;
}

void zBridgeServerSync_runCycle(ZBridgeServerSync* handle) {
	
	zBridgeServerSync_clearOutEvents(handle);
	
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < ZBRIDGESERVERSYNC_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++) {
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition]) {
		case ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync : {
			zBridgeServerSync_react_entry_SynChronizeServer_West_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync : {
			zBridgeServerSync_react_entry_SynChronizeServer_West_waitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync : {
			zBridgeServerSync_react_entry_SynChronizeServer_North_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync : {
			zBridgeServerSync_react_entry_SynChronizeServer_North_waitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync : {
			zBridgeServerSync_react_entry_SynChronizeServer_East_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync : {
			zBridgeServerSync_react_entry_SynChronizeServer_East_waitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync : {
			zBridgeServerSync_react_entry_SynChronizeServer_South_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync : {
			zBridgeServerSync_react_entry_SynChronizeServer_South_waitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync : {
			zBridgeServerSync_react_entry_ClientConfirm_West_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync : {
			zBridgeServerSync_react_entry_ClientConfirm_West_WaitForSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync : {
			zBridgeServerSync_react_entry_ClientConfirm_North_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync : {
			zBridgeServerSync_react_entry_ClientConfirm_North_WaitForSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync : {
			zBridgeServerSync_react_entry_ClientConfirm_East_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync : {
			zBridgeServerSync_react_entry_ClientConfirm_East_WaitForSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync : {
			zBridgeServerSync_react_entry_ClientConfirm_South_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync : {
			zBridgeServerSync_react_entry_ClientConfirm_South_WaitForSync(handle);
			break;
		}
		case ZBridgeServerSync_entry__final_ : {
			zBridgeServerSync_react_entry__final_(handle);
			break;
		}
		case ZBridgeServerSync_entry_AttemptSync : {
			zBridgeServerSync_react_entry_AttemptSync(handle);
			break;
		}
		default:
			break;
		}
	}
	
	zBridgeServerSync_clearInEvents(handle);
}


sc_boolean zBridgeServerSync_isActive(ZBridgeServerSync* handle, ZBridgeServerSyncStates state) {
	switch (state) {
		case ZBridgeServerSync_entry_SynChronizeServer : 
			return (sc_boolean) (handle->stateConfVector[0] >= ZBridgeServerSync_entry_SynChronizeServer
				&& handle->stateConfVector[0] <= ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync);
		case ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync
			);
		case ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync
			);
		case ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync
			);
		case ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync
			);
		case ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync
			);
		case ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync
			);
		case ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync
			);
		case ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync
			);
		case ZBridgeServerSync_entry_ClientConfirm : 
			return (sc_boolean) (handle->stateConfVector[0] >= ZBridgeServerSync_entry_ClientConfirm
				&& handle->stateConfVector[0] <= ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync);
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync
			);
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync
			);
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync
			);
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync : 
			return (sc_boolean) (handle->stateConfVector[1] == ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync
			);
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync
			);
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync : 
			return (sc_boolean) (handle->stateConfVector[2] == ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync
			);
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync
			);
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync : 
			return (sc_boolean) (handle->stateConfVector[3] == ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync
			);
		case ZBridgeServerSync_entry__final_ : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServerSync_entry__final_
			);
		case ZBridgeServerSync_entry_AttemptSync : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeServerSync_entry_AttemptSync
			);
		default: return bool_false;
	}
}

void zBridgeServerSyncIface_raise_attemptSync(ZBridgeServerSync* handle, sc_integer value) {
	handle->iface.attemptSync_value = value;
	handle->iface.attemptSync_raised = bool_true;
}
void zBridgeServerSyncIface_raise_continue(ZBridgeServerSync* handle) {
	handle->iface.continue_raised = bool_true;
}
void zBridgeServerSyncIface_raise_confirmSync(ZBridgeServerSync* handle, sc_integer value) {
	handle->iface.confirmSync_value = value;
	handle->iface.confirmSync_raised = bool_true;
}

sc_boolean zBridgeServerSyncIface_israised_sendAttemptSync(ZBridgeServerSync* handle) {
	return handle->iface.sendAttemptSync_raised;
}
sc_integer zBridgeServerSyncIface_get_sendAttemptSync_value(ZBridgeServerSync* handle) {
	return handle->iface.sendAttemptSync_value;
}
sc_boolean zBridgeServerSyncIface_israised_sendAttemptSyncAll(ZBridgeServerSync* handle) {
	return handle->iface.sendAttemptSyncAll_raised;
}
sc_boolean zBridgeServerSyncIface_israised_sendConfirmSync(ZBridgeServerSync* handle) {
	return handle->iface.sendConfirmSync_raised;
}
sc_boolean zBridgeServerSyncIface_israised_sendAllSync(ZBridgeServerSync* handle) {
	return handle->iface.sendAllSync_raised;
}


// implementations of all internal functions

/* Entry action for statechart 'ZBridgeServerSync'. */
static void zBridgeServerSync_enact_SequenceImpl(ZBridgeServerSync* handle) {
}

/* Exit action for state 'ZBridgeServerSync'. */
static void zBridgeServerSync_exact_SequenceImpl(ZBridgeServerSync* handle) {
}

/* The reactions of state WaitForAttemptSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_West_WaitForAttemptSync(ZBridgeServerSync* handle) {
	/* The reactions of state WaitForAttemptSync. */
	if (handle->iface.attemptSync_raised && handle->iface.attemptSync_value == handle->internal.W) { 
		/* Default exit sequence for state WaitForAttemptSync */
		handle->stateConfVector[0] = ZBridgeServerSync_last_state;
		handle->stateConfVectorPosition = 0;
		handle->internal.confirmWest = bool_true;
		handle->iface.sendAttemptSync_value = handle->internal.W;
		handle->iface.sendAttemptSync_raised = bool_true;
		/* 'default' enter sequence for state waitForConfirmSync */
		/* Entry action for state 'waitForConfirmSync'. */
		if (handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) { 
			handle->iface.sendConfirmSync_raised = bool_true;
		} 
		handle->stateConfVector[0] = ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync;
		handle->stateConfVectorPosition = 0;
	} 
}

/* The reactions of state waitForConfirmSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_West_waitForConfirmSync(ZBridgeServerSync* handle) {
	/* The reactions of state waitForConfirmSync. */
	if (handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state SynChronizeServer */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServerSync_react_entry__sync0(handle);
	} 
}

/* The reactions of state WaitForAttemptSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_North_WaitForAttemptSync(ZBridgeServerSync* handle) {
	/* The reactions of state WaitForAttemptSync. */
	if (handle->iface.attemptSync_raised && handle->iface.attemptSync_value == handle->internal.N) { 
		/* Default exit sequence for state WaitForAttemptSync */
		handle->stateConfVector[1] = ZBridgeServerSync_last_state;
		handle->stateConfVectorPosition = 1;
		handle->internal.confirmNorth = bool_true;
		handle->iface.sendAttemptSync_value = handle->internal.N;
		handle->iface.sendAttemptSync_raised = bool_true;
		/* 'default' enter sequence for state waitForConfirmSync */
		/* Entry action for state 'waitForConfirmSync'. */
		if (handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) { 
			handle->iface.sendConfirmSync_raised = bool_true;
		} 
		handle->stateConfVector[1] = ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync;
		handle->stateConfVectorPosition = 1;
	} 
}

/* The reactions of state waitForConfirmSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_North_waitForConfirmSync(ZBridgeServerSync* handle) {
	/* The reactions of state waitForConfirmSync. */
	if (handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state SynChronizeServer */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServerSync_react_entry__sync0(handle);
	} 
}

/* The reactions of state WaitForAttemptSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_East_WaitForAttemptSync(ZBridgeServerSync* handle) {
	/* The reactions of state WaitForAttemptSync. */
	if (handle->iface.attemptSync_raised && handle->iface.attemptSync_value == handle->internal.E) { 
		/* Default exit sequence for state WaitForAttemptSync */
		handle->stateConfVector[2] = ZBridgeServerSync_last_state;
		handle->stateConfVectorPosition = 2;
		handle->internal.confirmEast = bool_true;
		handle->iface.sendAttemptSync_value = handle->internal.E;
		handle->iface.sendAttemptSync_raised = bool_true;
		/* 'default' enter sequence for state waitForConfirmSync */
		/* Entry action for state 'waitForConfirmSync'. */
		if (handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) { 
			handle->iface.sendConfirmSync_raised = bool_true;
		} 
		handle->stateConfVector[2] = ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync;
		handle->stateConfVectorPosition = 2;
	} 
}

/* The reactions of state waitForConfirmSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_East_waitForConfirmSync(ZBridgeServerSync* handle) {
	/* The reactions of state waitForConfirmSync. */
	if (handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state SynChronizeServer */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServerSync_react_entry__sync0(handle);
	} 
}

/* The reactions of state WaitForAttemptSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_South_WaitForAttemptSync(ZBridgeServerSync* handle) {
	/* The reactions of state WaitForAttemptSync. */
	if (handle->iface.attemptSync_raised && handle->iface.attemptSync_value == handle->internal.S) { 
		/* Default exit sequence for state WaitForAttemptSync */
		handle->stateConfVector[3] = ZBridgeServerSync_last_state;
		handle->stateConfVectorPosition = 3;
		handle->internal.confirmSouth = bool_true;
		handle->iface.sendAttemptSync_value = handle->internal.S;
		handle->iface.sendAttemptSync_raised = bool_true;
		/* 'default' enter sequence for state waitForConfirmSync */
		/* Entry action for state 'waitForConfirmSync'. */
		if (handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) { 
			handle->iface.sendConfirmSync_raised = bool_true;
		} 
		handle->stateConfVector[3] = ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync;
		handle->stateConfVectorPosition = 3;
	} 
}

/* The reactions of state waitForConfirmSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_South_waitForConfirmSync(ZBridgeServerSync* handle) {
	/* The reactions of state waitForConfirmSync. */
	if (handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state SynChronizeServer */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync : {
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync : {
				/* Default exit sequence for state waitForConfirmSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServerSync_react_entry__sync0(handle);
	} 
}

/* The reactions of state WaitForConfirmSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_West_WaitForConfirmSync(ZBridgeServerSync* handle) {
	/* The reactions of state WaitForConfirmSync. */
	if (handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state ClientConfirm */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServerSync_react_entry__sync1(handle);
	} 
}

/* The reactions of state WaitForSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_West_WaitForSync(ZBridgeServerSync* handle) {
	/* The reactions of state WaitForSync. */
	if (handle->iface.confirmSync_raised && handle->iface.confirmSync_value == handle->internal.W) { 
		/* Default exit sequence for state WaitForSync */
		handle->stateConfVector[0] = ZBridgeServerSync_last_state;
		handle->stateConfVectorPosition = 0;
		handle->internal.confirmWest = bool_true;
		/* 'default' enter sequence for state WaitForConfirmSync */
		/* Entry action for state 'WaitForConfirmSync'. */
		if (handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) { 
			handle->iface.sendAllSync_raised = bool_true;
		} 
		handle->stateConfVector[0] = ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync;
		handle->stateConfVectorPosition = 0;
	} 
}

/* The reactions of state WaitForConfirmSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_North_WaitForConfirmSync(ZBridgeServerSync* handle) {
	/* The reactions of state WaitForConfirmSync. */
	if (handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state ClientConfirm */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServerSync_react_entry__sync1(handle);
	} 
}

/* The reactions of state WaitForSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_North_WaitForSync(ZBridgeServerSync* handle) {
	/* The reactions of state WaitForSync. */
	if (handle->iface.confirmSync_raised && handle->iface.confirmSync_value == handle->internal.N) { 
		/* Default exit sequence for state WaitForSync */
		handle->stateConfVector[1] = ZBridgeServerSync_last_state;
		handle->stateConfVectorPosition = 1;
		handle->internal.confirmNorth = bool_true;
		/* 'default' enter sequence for state WaitForConfirmSync */
		/* Entry action for state 'WaitForConfirmSync'. */
		if (handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) { 
			handle->iface.sendAllSync_raised = bool_true;
		} 
		handle->stateConfVector[1] = ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync;
		handle->stateConfVectorPosition = 1;
	} 
}

/* The reactions of state WaitForConfirmSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_East_WaitForConfirmSync(ZBridgeServerSync* handle) {
	/* The reactions of state WaitForConfirmSync. */
	if (handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state ClientConfirm */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServerSync_react_entry__sync1(handle);
	} 
}

/* The reactions of state WaitForSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_East_WaitForSync(ZBridgeServerSync* handle) {
	/* The reactions of state WaitForSync. */
	if (handle->iface.confirmSync_raised && handle->iface.confirmSync_value == handle->internal.E) { 
		/* Default exit sequence for state WaitForSync */
		handle->stateConfVector[2] = ZBridgeServerSync_last_state;
		handle->stateConfVectorPosition = 2;
		handle->internal.confirmEast = bool_true;
		/* 'default' enter sequence for state WaitForConfirmSync */
		/* Entry action for state 'WaitForConfirmSync'. */
		if (handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) { 
			handle->iface.sendAllSync_raised = bool_true;
		} 
		handle->stateConfVector[2] = ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync;
		handle->stateConfVectorPosition = 2;
	} 
}

/* The reactions of state WaitForConfirmSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_South_WaitForConfirmSync(ZBridgeServerSync* handle) {
	/* The reactions of state WaitForConfirmSync. */
	if (handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isActive(handle, ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync) && handle->iface.continue_raised) { 
		/* Default exit sequence for state ClientConfirm */
		/* Default exit sequence for region West */
		/* Handle exit of all possible states (of West) at position 0... */
		switch(handle->stateConfVector[ 0 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[0] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region North */
		/* Handle exit of all possible states (of North) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[1] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region East */
		/* Handle exit of all possible states (of East) at position 2... */
		switch(handle->stateConfVector[ 2 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[2] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 2;
				break;
			}
			default: break;
		}
		/* Default exit sequence for region South */
		/* Handle exit of all possible states (of South) at position 3... */
		switch(handle->stateConfVector[ 3 ]) {
			case ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync : {
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			case ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync : {
				/* Default exit sequence for state WaitForSync */
				handle->stateConfVector[3] = ZBridgeServerSync_last_state;
				handle->stateConfVectorPosition = 3;
				break;
			}
			default: break;
		}
		zBridgeServerSync_react_entry__sync1(handle);
	} 
}

/* The reactions of state WaitForSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_South_WaitForSync(ZBridgeServerSync* handle) {
	/* The reactions of state WaitForSync. */
	if (handle->iface.confirmSync_raised && handle->iface.confirmSync_value == handle->internal.S) { 
		/* Default exit sequence for state WaitForSync */
		handle->stateConfVector[3] = ZBridgeServerSync_last_state;
		handle->stateConfVectorPosition = 3;
		handle->internal.confirmSouth = bool_true;
		/* 'default' enter sequence for state WaitForConfirmSync */
		/* Entry action for state 'WaitForConfirmSync'. */
		if (handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) { 
			handle->iface.sendAllSync_raised = bool_true;
		} 
		handle->stateConfVector[3] = ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync;
		handle->stateConfVectorPosition = 3;
	} 
}

/* The reactions of state null. */
static void zBridgeServerSync_react_entry__final_(ZBridgeServerSync* handle) {
	/* The reactions of state null. */
}

/* The reactions of state AttemptSync. */
static void zBridgeServerSync_react_entry_AttemptSync(ZBridgeServerSync* handle) {
	/* The reactions of state AttemptSync. */
	if (handle->iface.continue_raised) { 
		/* Default exit sequence for state AttemptSync */
		handle->stateConfVector[0] = ZBridgeServerSync_last_state;
		handle->stateConfVectorPosition = 0;
		handle->iface.sendAttemptSyncAll_raised = bool_true;
		/* 'default' enter sequence for state SynChronizeServer */
		/* 'default' enter sequence for region West */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state WaitForAttemptSync */
		handle->stateConfVector[0] = ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync;
		handle->stateConfVectorPosition = 0;
		/* 'default' enter sequence for region North */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state WaitForAttemptSync */
		handle->stateConfVector[1] = ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync;
		handle->stateConfVectorPosition = 1;
		/* 'default' enter sequence for region East */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state WaitForAttemptSync */
		handle->stateConfVector[2] = ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync;
		handle->stateConfVectorPosition = 2;
		/* 'default' enter sequence for region South */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state WaitForAttemptSync */
		handle->stateConfVector[3] = ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync;
		handle->stateConfVectorPosition = 3;
	} 
}

/* The reactions of state null. */
static void zBridgeServerSync_react_entry__sync0(ZBridgeServerSync* handle) {
	/* The reactions of state null. */
	handle->internal.confirmWest = bool_false;
	handle->internal.confirmNorth = bool_false;
	handle->internal.confirmEast = bool_false;
	handle->internal.confirmSouth = bool_false;
	/* 'default' enter sequence for state ClientConfirm */
	/* 'default' enter sequence for region West */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state WaitForSync */
	handle->stateConfVector[0] = ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync;
	handle->stateConfVectorPosition = 0;
	/* 'default' enter sequence for region North */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state WaitForSync */
	handle->stateConfVector[1] = ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync;
	handle->stateConfVectorPosition = 1;
	/* 'default' enter sequence for region East */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state WaitForSync */
	handle->stateConfVector[2] = ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync;
	handle->stateConfVectorPosition = 2;
	/* 'default' enter sequence for region South */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state WaitForSync */
	handle->stateConfVector[3] = ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync;
	handle->stateConfVectorPosition = 3;
}

/* The reactions of state null. */
static void zBridgeServerSync_react_entry__sync1(ZBridgeServerSync* handle) {
	/* The reactions of state null. */
	/* Default enter sequence for state null */
	handle->stateConfVector[0] = ZBridgeServerSync_entry__final_;
	handle->stateConfVectorPosition = 0;
}


