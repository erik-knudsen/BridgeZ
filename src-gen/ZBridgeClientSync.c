
#include <stdlib.h>
#include <string.h>
#include "sc_types.h"
#include "ZBridgeClientSync.h"
/*! \file Implementation of the state machine 'ZBridgeClientSync'
*/

// prototypes of all internal functions

static void zBridgeClientSync_entryaction(ZBridgeClientSync* handle);
static void zBridgeClientSync_exitaction(ZBridgeClientSync* handle);
static void zBridgeClientSync_react_main_region_WaitForAttemptSync(ZBridgeClientSync* handle);
static void zBridgeClientSync_react_main_region__final_0(ZBridgeClientSync* handle);
static void zBridgeClientSync_react_main_region_WaitForConfirmSync(ZBridgeClientSync* handle);
static void zBridgeClientSync_react_main_region_WaitForAllSync(ZBridgeClientSync* handle);
static void clearInEvents(ZBridgeClientSync* handle);
static void clearOutEvents(ZBridgeClientSync* handle);


void zBridgeClientSync_init(ZBridgeClientSync* handle)
{
	int i;

	for (i = 0; i < ZBRIDGECLIENTSYNC_MAX_ORTHOGONAL_STATES; ++i)
		handle->stateConfVector[i] = ZBridgeClientSync_last_state;
	
	
	handle->stateConfVectorPosition = 0;

clearInEvents(handle);
clearOutEvents(handle);

	// TODO: initialize all events ...

	{
		/* Default init sequence for statechart ZBridgeClientSync */
		handle->iface.syncState = 0;
	}

}

void zBridgeClientSync_enter(ZBridgeClientSync* handle)
{
	{
		/* Default enter sequence for statechart ZBridgeClientSync */
		zBridgeClientSync_entryaction(handle);
		{
			/* 'default' enter sequence for region main region */
			{
				/* Default react sequence for initial entry  */
				{
					/* 'default' enter sequence for state WaitForAttemptSync */
					{
						/* Entry action for state 'WaitForAttemptSync'. */
						handle->iface.sendAttemptSync_raised = bool_true;
					}
					handle->stateConfVector[0] = ZBridgeClientSync_main_region_WaitForAttemptSync;
					handle->stateConfVectorPosition = 0;
				}
			}
		}
	}
}

void zBridgeClientSync_exit(ZBridgeClientSync* handle)
{
	{
		/* Default exit sequence for statechart ZBridgeClientSync */
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of main region) at position 0... */
			switch(handle->stateConfVector[ 0 ]) {
				case ZBridgeClientSync_main_region_WaitForAttemptSync : {
					{
						/* Default exit sequence for state WaitForAttemptSync */
						handle->stateConfVector[0] = ZBridgeClientSync_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClientSync_main_region__final_ : {
					{
						/* Default exit sequence for final state. */
						handle->stateConfVector[0] = ZBridgeClientSync_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClientSync_main_region_WaitForConfirmSync : {
					{
						/* Default exit sequence for state WaitForConfirmSync */
						handle->stateConfVector[0] = ZBridgeClientSync_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				case ZBridgeClientSync_main_region_WaitForAllSync : {
					{
						/* Default exit sequence for state WaitForAllSync */
						handle->stateConfVector[0] = ZBridgeClientSync_last_state;
						handle->stateConfVectorPosition = 0;
					}
					break;
				}
				default: break;
			}
		}
		zBridgeClientSync_exitaction(handle);
	}
}

static void clearInEvents(ZBridgeClientSync* handle) {
	handle->iface.attemptSync_raised = bool_false;
	handle->iface.confirmSync_raised = bool_false;
	handle->iface.allSync_raised = bool_false;
}

static void clearOutEvents(ZBridgeClientSync* handle) {
	handle->iface.sendAttemptSync_raised = bool_false;
	handle->iface.sendConfirmSync_raised = bool_false;
	handle->iface.okSync_raised = bool_false;
}

void zBridgeClientSync_runCycle(ZBridgeClientSync* handle) {
	
	clearOutEvents(handle);
	
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < ZBRIDGECLIENTSYNC_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++) {
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition]) {
		case ZBridgeClientSync_main_region_WaitForAttemptSync : {
			zBridgeClientSync_react_main_region_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeClientSync_main_region__final_ : {
			zBridgeClientSync_react_main_region__final_0(handle);
			break;
		}
		case ZBridgeClientSync_main_region_WaitForConfirmSync : {
			zBridgeClientSync_react_main_region_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeClientSync_main_region_WaitForAllSync : {
			zBridgeClientSync_react_main_region_WaitForAllSync(handle);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
}


sc_boolean zBridgeClientSync_isActive(ZBridgeClientSync* handle, ZBridgeClientSyncStates state) {
	switch (state) {
		case ZBridgeClientSync_main_region_WaitForAttemptSync : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClientSync_main_region_WaitForAttemptSync
			);
		case ZBridgeClientSync_main_region__final_ : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClientSync_main_region__final_
			);
		case ZBridgeClientSync_main_region_WaitForConfirmSync : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClientSync_main_region_WaitForConfirmSync
			);
		case ZBridgeClientSync_main_region_WaitForAllSync : 
			return (sc_boolean) (handle->stateConfVector[0] == ZBridgeClientSync_main_region_WaitForAllSync
			);
		default: return bool_false;
	}
}

void zBridgeClientSyncIface_raise_attemptSync(ZBridgeClientSync* handle) {
	handle->iface.attemptSync_raised = bool_true;
}
void zBridgeClientSyncIface_raise_confirmSync(ZBridgeClientSync* handle) {
	handle->iface.confirmSync_raised = bool_true;
}
void zBridgeClientSyncIface_raise_allSync(ZBridgeClientSync* handle) {
	handle->iface.allSync_raised = bool_true;
}

sc_boolean zBridgeClientSyncIface_israised_sendAttemptSync(ZBridgeClientSync* handle) {
	return handle->iface.sendAttemptSync_raised;
}
sc_boolean zBridgeClientSyncIface_israised_sendConfirmSync(ZBridgeClientSync* handle) {
	return handle->iface.sendConfirmSync_raised;
}
sc_boolean zBridgeClientSyncIface_israised_okSync(ZBridgeClientSync* handle) {
	return handle->iface.okSync_raised;
}

sc_integer zBridgeClientSyncIface_get_syncState(ZBridgeClientSync* handle) {
	return handle->iface.syncState;
}
void zBridgeClientSyncIface_set_syncState(ZBridgeClientSync* handle, sc_integer value) {
	handle->iface.syncState = value;
}
		
// implementations of all internal functions

/* Entry action for statechart 'ZBridgeClientSync'. */
static void zBridgeClientSync_entryaction(ZBridgeClientSync* handle) {
	{
		/* Entry action for statechart 'ZBridgeClientSync'. */
	}
}

/* Exit action for state 'ZBridgeClientSync'. */
static void zBridgeClientSync_exitaction(ZBridgeClientSync* handle) {
	{
		/* Exit action for state 'ZBridgeClientSync'. */
	}
}

/* The reactions of state WaitForAttemptSync. */
static void zBridgeClientSync_react_main_region_WaitForAttemptSync(ZBridgeClientSync* handle) {
	{
		/* The reactions of state WaitForAttemptSync. */
		if (handle->iface.attemptSync_raised) { 
			{
				/* Default exit sequence for state WaitForAttemptSync */
				handle->stateConfVector[0] = ZBridgeClientSync_last_state;
				handle->stateConfVectorPosition = 0;
			}
			handle->iface.sendAttemptSync_raised = bool_true;
			{
				/* 'default' enter sequence for state WaitForConfirmSync */
				handle->stateConfVector[0] = ZBridgeClientSync_main_region_WaitForConfirmSync;
				handle->stateConfVectorPosition = 0;
			}
		} 
	}
}

/* The reactions of state null. */
static void zBridgeClientSync_react_main_region__final_0(ZBridgeClientSync* handle) {
	{
		/* The reactions of state null. */
	}
}

/* The reactions of state WaitForConfirmSync. */
static void zBridgeClientSync_react_main_region_WaitForConfirmSync(ZBridgeClientSync* handle) {
	{
		/* The reactions of state WaitForConfirmSync. */
		if (handle->iface.confirmSync_raised) { 
			{
				/* Default exit sequence for state WaitForConfirmSync */
				handle->stateConfVector[0] = ZBridgeClientSync_last_state;
				handle->stateConfVectorPosition = 0;
			}
			handle->iface.sendConfirmSync_raised = bool_true;
			{
				/* 'default' enter sequence for state WaitForAllSync */
				handle->stateConfVector[0] = ZBridgeClientSync_main_region_WaitForAllSync;
				handle->stateConfVectorPosition = 0;
			}
		} 
	}
}

/* The reactions of state WaitForAllSync. */
static void zBridgeClientSync_react_main_region_WaitForAllSync(ZBridgeClientSync* handle) {
	{
		/* The reactions of state WaitForAllSync. */
		if (handle->iface.allSync_raised) { 
			{
				/* Default exit sequence for state WaitForAllSync */
				handle->stateConfVector[0] = ZBridgeClientSync_last_state;
				handle->stateConfVectorPosition = 0;
			}
			handle->iface.okSync_raised = bool_true;
			{
				/* Default enter sequence for state null */
				handle->stateConfVector[0] = ZBridgeClientSync_main_region__final_;
				handle->stateConfVectorPosition = 0;
			}
		} 
	}
}


