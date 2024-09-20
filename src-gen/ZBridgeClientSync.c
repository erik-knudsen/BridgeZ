
#include <stdlib.h>
#include <string.h>
#include "sc_types.h"
#include "ZBridgeClientSync.h"
/*! \file Implementation of the state machine 'ZBridgeClientSync'
*/

/* prototypes of all internal functions */
static sc_boolean zBridgeClientSync_check_main_region_WaitForAttemptSync_tr0_tr0(const ZBridgeClientSync* handle);
static sc_boolean zBridgeClientSync_check_main_region_WaitForConfirmSync_tr0_tr0(const ZBridgeClientSync* handle);
static sc_boolean zBridgeClientSync_check_main_region_WaitForAllSync_tr0_tr0(const ZBridgeClientSync* handle);
static void zBridgeClientSync_effect_main_region_WaitForAttemptSync_tr0(ZBridgeClientSync* handle);
static void zBridgeClientSync_effect_main_region_WaitForConfirmSync_tr0(ZBridgeClientSync* handle);
static void zBridgeClientSync_effect_main_region_WaitForAllSync_tr0(ZBridgeClientSync* handle);
static void zBridgeClientSync_enact_main_region_WaitForAttemptSync(ZBridgeClientSync* handle);
static void zBridgeClientSync_enseq_main_region_WaitForAttemptSync_default(ZBridgeClientSync* handle);
static void zBridgeClientSync_enseq_main_region__final__default(ZBridgeClientSync* handle);
static void zBridgeClientSync_enseq_main_region_WaitForConfirmSync_default(ZBridgeClientSync* handle);
static void zBridgeClientSync_enseq_main_region_WaitForAllSync_default(ZBridgeClientSync* handle);
static void zBridgeClientSync_enseq_main_region_default(ZBridgeClientSync* handle);
static void zBridgeClientSync_exseq_main_region_WaitForAttemptSync(ZBridgeClientSync* handle);
static void zBridgeClientSync_exseq_main_region__final_(ZBridgeClientSync* handle);
static void zBridgeClientSync_exseq_main_region_WaitForConfirmSync(ZBridgeClientSync* handle);
static void zBridgeClientSync_exseq_main_region_WaitForAllSync(ZBridgeClientSync* handle);
static void zBridgeClientSync_exseq_main_region(ZBridgeClientSync* handle);
static void zBridgeClientSync_react_main_region_WaitForAttemptSync(ZBridgeClientSync* handle);
static void zBridgeClientSync_react_main_region__final_(ZBridgeClientSync* handle);
static void zBridgeClientSync_react_main_region_WaitForConfirmSync(ZBridgeClientSync* handle);
static void zBridgeClientSync_react_main_region_WaitForAllSync(ZBridgeClientSync* handle);
static void zBridgeClientSync_react_main_region__entry_Default(ZBridgeClientSync* handle);
static void zBridgeClientSync_clearInEvents(ZBridgeClientSync* handle);
static void zBridgeClientSync_clearOutEvents(ZBridgeClientSync* handle);


void zBridgeClientSync_init(ZBridgeClientSync* handle)
{
		sc_integer i;
	
		for (i = 0; i < ZBRIDGECLIENTSYNC_MAX_ORTHOGONAL_STATES; ++i)
		{
			handle->stateConfVector[i] = ZBridgeClientSync_last_state;
		}
		
		
		handle->stateConfVectorPosition = 0;
	
		zBridgeClientSync_clearInEvents(handle);
		zBridgeClientSync_clearOutEvents(handle);
	
		/* Default init sequence for statechart ZBridgeClientSync */
		handle->iface.syncState = 0;
}

void zBridgeClientSync_enter(ZBridgeClientSync* handle)
{
	/* Default enter sequence for statechart ZBridgeClientSync */
	zBridgeClientSync_enseq_main_region_default(handle);
}

void zBridgeClientSync_exit(ZBridgeClientSync* handle)
{
	/* Default exit sequence for statechart ZBridgeClientSync */
	zBridgeClientSync_exseq_main_region(handle);
}

sc_boolean zBridgeClientSync_isActive(const ZBridgeClientSync* handle)
{
	sc_boolean result = bool_false;
	int i;
	
	for(i = 0; i < ZBRIDGECLIENTSYNC_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != ZBridgeClientSync_last_state;
	}
	
	return result;
}

sc_boolean zBridgeClientSync_isFinal(const ZBridgeClientSync* handle)
{
	return (handle->stateConfVector[0] == ZBridgeClientSync_main_region__final_);

}

static void zBridgeClientSync_clearInEvents(ZBridgeClientSync* handle)
{
	handle->iface.attemptSync_raised = bool_false;
	handle->iface.confirmSync_raised = bool_false;
	handle->iface.allSync_raised = bool_false;
}

static void zBridgeClientSync_clearOutEvents(ZBridgeClientSync* handle)
{
	handle->iface.sendAttemptSync_raised = bool_false;
	handle->iface.sendConfirmSync_raised = bool_false;
	handle->iface.okSync_raised = bool_false;
}

void zBridgeClientSync_runCycle(ZBridgeClientSync* handle)
{
	
	zBridgeClientSync_clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < ZBRIDGECLIENTSYNC_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case ZBridgeClientSync_main_region_WaitForAttemptSync:
		{
			zBridgeClientSync_react_main_region_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeClientSync_main_region__final_:
		{
			zBridgeClientSync_react_main_region__final_(handle);
			break;
		}
		case ZBridgeClientSync_main_region_WaitForConfirmSync:
		{
			zBridgeClientSync_react_main_region_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeClientSync_main_region_WaitForAllSync:
		{
			zBridgeClientSync_react_main_region_WaitForAllSync(handle);
			break;
		}
		default:
			break;
		}
	}
	
	zBridgeClientSync_clearInEvents(handle);
}


sc_boolean zBridgeClientSync_isStateActive(const ZBridgeClientSync* handle, ZBridgeClientSyncStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case ZBridgeClientSync_main_region_WaitForAttemptSync :
			result = (sc_boolean) (handle->stateConfVector[SCVI_ZBRIDGECLIENTSYNC_MAIN_REGION_WAITFORATTEMPTSYNC] == ZBridgeClientSync_main_region_WaitForAttemptSync
			);
			break;
		case ZBridgeClientSync_main_region__final_ :
			result = (sc_boolean) (handle->stateConfVector[SCVI_ZBRIDGECLIENTSYNC_MAIN_REGION__FINAL_] == ZBridgeClientSync_main_region__final_
			);
			break;
		case ZBridgeClientSync_main_region_WaitForConfirmSync :
			result = (sc_boolean) (handle->stateConfVector[SCVI_ZBRIDGECLIENTSYNC_MAIN_REGION_WAITFORCONFIRMSYNC] == ZBridgeClientSync_main_region_WaitForConfirmSync
			);
			break;
		case ZBridgeClientSync_main_region_WaitForAllSync :
			result = (sc_boolean) (handle->stateConfVector[SCVI_ZBRIDGECLIENTSYNC_MAIN_REGION_WAITFORALLSYNC] == ZBridgeClientSync_main_region_WaitForAllSync
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

void zBridgeClientSyncIface_raise_attemptSync(ZBridgeClientSync* handle)
{
	handle->iface.attemptSync_raised = bool_true;
}
void zBridgeClientSyncIface_raise_confirmSync(ZBridgeClientSync* handle)
{
	handle->iface.confirmSync_raised = bool_true;
}
void zBridgeClientSyncIface_raise_allSync(ZBridgeClientSync* handle)
{
	handle->iface.allSync_raised = bool_true;
}

sc_boolean zBridgeClientSyncIface_israised_sendAttemptSync(const ZBridgeClientSync* handle)
{
	return handle->iface.sendAttemptSync_raised;
}
sc_boolean zBridgeClientSyncIface_israised_sendConfirmSync(const ZBridgeClientSync* handle)
{
	return handle->iface.sendConfirmSync_raised;
}
sc_boolean zBridgeClientSyncIface_israised_okSync(const ZBridgeClientSync* handle)
{
	return handle->iface.okSync_raised;
}

sc_integer zBridgeClientSyncIface_get_syncState(const ZBridgeClientSync* handle)
{
	return handle->iface.syncState;
}
void zBridgeClientSyncIface_set_syncState(ZBridgeClientSync* handle, sc_integer value)
{
	handle->iface.syncState = value;
}

/* implementations of all internal functions */

static sc_boolean zBridgeClientSync_check_main_region_WaitForAttemptSync_tr0_tr0(const ZBridgeClientSync* handle)
{
	return handle->iface.attemptSync_raised;
}

static sc_boolean zBridgeClientSync_check_main_region_WaitForConfirmSync_tr0_tr0(const ZBridgeClientSync* handle)
{
	return handle->iface.confirmSync_raised;
}

static sc_boolean zBridgeClientSync_check_main_region_WaitForAllSync_tr0_tr0(const ZBridgeClientSync* handle)
{
	return handle->iface.allSync_raised;
}

static void zBridgeClientSync_effect_main_region_WaitForAttemptSync_tr0(ZBridgeClientSync* handle)
{
	zBridgeClientSync_exseq_main_region_WaitForAttemptSync(handle);
	handle->iface.sendAttemptSync_raised = bool_true;
	zBridgeClientSync_enseq_main_region_WaitForConfirmSync_default(handle);
}

static void zBridgeClientSync_effect_main_region_WaitForConfirmSync_tr0(ZBridgeClientSync* handle)
{
	zBridgeClientSync_exseq_main_region_WaitForConfirmSync(handle);
	handle->iface.sendConfirmSync_raised = bool_true;
	zBridgeClientSync_enseq_main_region_WaitForAllSync_default(handle);
}

static void zBridgeClientSync_effect_main_region_WaitForAllSync_tr0(ZBridgeClientSync* handle)
{
	zBridgeClientSync_exseq_main_region_WaitForAllSync(handle);
	handle->iface.okSync_raised = bool_true;
	zBridgeClientSync_enseq_main_region__final__default(handle);
}

/* Entry action for state 'WaitForAttemptSync'. */
static void zBridgeClientSync_enact_main_region_WaitForAttemptSync(ZBridgeClientSync* handle)
{
	/* Entry action for state 'WaitForAttemptSync'. */
	handle->iface.sendAttemptSync_raised = bool_true;
}

/* 'default' enter sequence for state WaitForAttemptSync */
static void zBridgeClientSync_enseq_main_region_WaitForAttemptSync_default(ZBridgeClientSync* handle)
{
	/* 'default' enter sequence for state WaitForAttemptSync */
	zBridgeClientSync_enact_main_region_WaitForAttemptSync(handle);
	handle->stateConfVector[0] = ZBridgeClientSync_main_region_WaitForAttemptSync;
	handle->stateConfVectorPosition = 0;
}

/* Default enter sequence for state null */
static void zBridgeClientSync_enseq_main_region__final__default(ZBridgeClientSync* handle)
{
	/* Default enter sequence for state null */
	handle->stateConfVector[0] = ZBridgeClientSync_main_region__final_;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state WaitForConfirmSync */
static void zBridgeClientSync_enseq_main_region_WaitForConfirmSync_default(ZBridgeClientSync* handle)
{
	/* 'default' enter sequence for state WaitForConfirmSync */
	handle->stateConfVector[0] = ZBridgeClientSync_main_region_WaitForConfirmSync;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state WaitForAllSync */
static void zBridgeClientSync_enseq_main_region_WaitForAllSync_default(ZBridgeClientSync* handle)
{
	/* 'default' enter sequence for state WaitForAllSync */
	handle->stateConfVector[0] = ZBridgeClientSync_main_region_WaitForAllSync;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
static void zBridgeClientSync_enseq_main_region_default(ZBridgeClientSync* handle)
{
	/* 'default' enter sequence for region main region */
	zBridgeClientSync_react_main_region__entry_Default(handle);
}

/* Default exit sequence for state WaitForAttemptSync */
static void zBridgeClientSync_exseq_main_region_WaitForAttemptSync(ZBridgeClientSync* handle)
{
	/* Default exit sequence for state WaitForAttemptSync */
	handle->stateConfVector[0] = ZBridgeClientSync_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for final state. */
static void zBridgeClientSync_exseq_main_region__final_(ZBridgeClientSync* handle)
{
	/* Default exit sequence for final state. */
	handle->stateConfVector[0] = ZBridgeClientSync_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state WaitForConfirmSync */
static void zBridgeClientSync_exseq_main_region_WaitForConfirmSync(ZBridgeClientSync* handle)
{
	/* Default exit sequence for state WaitForConfirmSync */
	handle->stateConfVector[0] = ZBridgeClientSync_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state WaitForAllSync */
static void zBridgeClientSync_exseq_main_region_WaitForAllSync(ZBridgeClientSync* handle)
{
	/* Default exit sequence for state WaitForAllSync */
	handle->stateConfVector[0] = ZBridgeClientSync_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
static void zBridgeClientSync_exseq_main_region(ZBridgeClientSync* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of ZBridgeClientSync.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case ZBridgeClientSync_main_region_WaitForAttemptSync :
		{
			zBridgeClientSync_exseq_main_region_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeClientSync_main_region__final_ :
		{
			zBridgeClientSync_exseq_main_region__final_(handle);
			break;
		}
		case ZBridgeClientSync_main_region_WaitForConfirmSync :
		{
			zBridgeClientSync_exseq_main_region_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeClientSync_main_region_WaitForAllSync :
		{
			zBridgeClientSync_exseq_main_region_WaitForAllSync(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state WaitForAttemptSync. */
static void zBridgeClientSync_react_main_region_WaitForAttemptSync(ZBridgeClientSync* handle)
{
	/* The reactions of state WaitForAttemptSync. */
	if (zBridgeClientSync_check_main_region_WaitForAttemptSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClientSync_effect_main_region_WaitForAttemptSync_tr0(handle);
	} 
}

/* The reactions of state null. */
static void zBridgeClientSync_react_main_region__final_(ZBridgeClientSync* handle)
{
}

/* The reactions of state WaitForConfirmSync. */
static void zBridgeClientSync_react_main_region_WaitForConfirmSync(ZBridgeClientSync* handle)
{
	/* The reactions of state WaitForConfirmSync. */
	if (zBridgeClientSync_check_main_region_WaitForConfirmSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClientSync_effect_main_region_WaitForConfirmSync_tr0(handle);
	} 
}

/* The reactions of state WaitForAllSync. */
static void zBridgeClientSync_react_main_region_WaitForAllSync(ZBridgeClientSync* handle)
{
	/* The reactions of state WaitForAllSync. */
	if (zBridgeClientSync_check_main_region_WaitForAllSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeClientSync_effect_main_region_WaitForAllSync_tr0(handle);
	} 
}

/* Default react sequence for initial entry  */
static void zBridgeClientSync_react_main_region__entry_Default(ZBridgeClientSync* handle)
{
	/* Default react sequence for initial entry  */
	zBridgeClientSync_enseq_main_region_WaitForAttemptSync_default(handle);
}


