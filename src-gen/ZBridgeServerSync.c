
#include <stdlib.h>
#include <string.h>
#include "sc_types.h"
#include "ZBridgeServerSync.h"
/*! \file Implementation of the state machine 'ZBridgeServerSync'
*/

/* prototypes of all internal functions */
static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_West_WaitForAttemptSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_West_waitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_North_WaitForAttemptSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_North_waitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_East_WaitForAttemptSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_East_waitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_South_WaitForAttemptSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_South_waitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_West_WaitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_West_WaitForSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_North_WaitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_North_WaitForSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_East_WaitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_East_WaitForSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_South_WaitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_South_WaitForSync_tr0_tr0(const ZBridgeServerSync* handle);
static sc_boolean zBridgeServerSync_check_entry_AttemptSync_tr0_tr0(const ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_SynChronizeServer_West_WaitForAttemptSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_SynChronizeServer_West_waitForConfirmSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_SynChronizeServer_North_WaitForAttemptSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_SynChronizeServer_North_waitForConfirmSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_SynChronizeServer_East_WaitForAttemptSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_SynChronizeServer_East_waitForConfirmSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_SynChronizeServer_South_WaitForAttemptSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_SynChronizeServer_South_waitForConfirmSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_ClientConfirm_West_WaitForConfirmSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_ClientConfirm_West_WaitForSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_ClientConfirm_North_WaitForConfirmSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_ClientConfirm_North_WaitForSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_ClientConfirm_East_WaitForConfirmSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_ClientConfirm_East_WaitForSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_ClientConfirm_South_WaitForConfirmSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_ClientConfirm_South_WaitForSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_effect_entry_AttemptSync_tr0(ZBridgeServerSync* handle);
static void zBridgeServerSync_enact_entry_SynChronizeServer_West_waitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_enact_entry_SynChronizeServer_North_waitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_enact_entry_SynChronizeServer_East_waitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_enact_entry_SynChronizeServer_South_waitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_enact_entry_ClientConfirm_West_WaitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_enact_entry_ClientConfirm_North_WaitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_enact_entry_ClientConfirm_East_WaitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_enact_entry_ClientConfirm_South_WaitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_SynChronizeServer_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_SynChronizeServer_West_WaitForAttemptSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_SynChronizeServer_West_waitForConfirmSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_SynChronizeServer_North_WaitForAttemptSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_SynChronizeServer_North_waitForConfirmSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_SynChronizeServer_East_WaitForAttemptSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_SynChronizeServer_East_waitForConfirmSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_SynChronizeServer_South_WaitForAttemptSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_SynChronizeServer_South_waitForConfirmSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_ClientConfirm_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_ClientConfirm_West_WaitForConfirmSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_ClientConfirm_West_WaitForSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_ClientConfirm_North_WaitForConfirmSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_ClientConfirm_North_WaitForSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_ClientConfirm_East_WaitForConfirmSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_ClientConfirm_East_WaitForSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_ClientConfirm_South_WaitForConfirmSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_ClientConfirm_South_WaitForSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry__final__default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_AttemptSync_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_SynChronizeServer_West_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_SynChronizeServer_North_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_SynChronizeServer_East_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_SynChronizeServer_South_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_ClientConfirm_West_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_ClientConfirm_North_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_ClientConfirm_East_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_enseq_entry_ClientConfirm_South_default(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_SynChronizeServer(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_SynChronizeServer_West_WaitForAttemptSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_SynChronizeServer_West_waitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_SynChronizeServer_North_WaitForAttemptSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_SynChronizeServer_North_waitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_SynChronizeServer_East_WaitForAttemptSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_SynChronizeServer_East_waitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_SynChronizeServer_South_WaitForAttemptSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_SynChronizeServer_South_waitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_ClientConfirm(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_ClientConfirm_West_WaitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_ClientConfirm_West_WaitForSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_ClientConfirm_North_WaitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_ClientConfirm_North_WaitForSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_ClientConfirm_East_WaitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_ClientConfirm_East_WaitForSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_ClientConfirm_South_WaitForConfirmSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_ClientConfirm_South_WaitForSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry__final_(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_AttemptSync(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_SynChronizeServer_West(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_SynChronizeServer_North(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_SynChronizeServer_East(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_SynChronizeServer_South(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_ClientConfirm_West(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_ClientConfirm_North(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_ClientConfirm_East(ZBridgeServerSync* handle);
static void zBridgeServerSync_exseq_entry_ClientConfirm_South(ZBridgeServerSync* handle);
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
static void zBridgeServerSync_react_entry__entry_Default(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_SynChronizeServer_West__entry_Default(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_SynChronizeServer_North__entry_Default(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_SynChronizeServer_East__entry_Default(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_SynChronizeServer_South__entry_Default(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_ClientConfirm_West__entry_Default(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_ClientConfirm_North__entry_Default(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_ClientConfirm_East__entry_Default(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry_ClientConfirm_South__entry_Default(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry__sync0(ZBridgeServerSync* handle);
static void zBridgeServerSync_react_entry__sync1(ZBridgeServerSync* handle);
static void zBridgeServerSync_clearInEvents(ZBridgeServerSync* handle);
static void zBridgeServerSync_clearOutEvents(ZBridgeServerSync* handle);


void zBridgeServerSync_init(ZBridgeServerSync* handle)
{
	sc_integer i;

	for (i = 0; i < ZBRIDGESERVERSYNC_MAX_ORTHOGONAL_STATES; ++i)
	{
		handle->stateConfVector[i] = ZBridgeServerSync_last_state;
	}
	
	
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
	zBridgeServerSync_enseq_entry_default(handle);
}

void zBridgeServerSync_exit(ZBridgeServerSync* handle)
{
	/* Default exit sequence for statechart ZBridgeServerSync */
	zBridgeServerSync_exseq_entry(handle);
}

sc_boolean zBridgeServerSync_isActive(const ZBridgeServerSync* handle)
{
	sc_boolean result;
	if (handle->stateConfVector[0] != ZBridgeServerSync_last_state || handle->stateConfVector[1] != ZBridgeServerSync_last_state || handle->stateConfVector[2] != ZBridgeServerSync_last_state || handle->stateConfVector[3] != ZBridgeServerSync_last_state)
	{
		result =  bool_true;
	}
	else
	{
		result = bool_false;
	}
	return result;
}

sc_boolean zBridgeServerSync_isFinal(const ZBridgeServerSync* handle)
{
	return (handle->stateConfVector[0] == ZBridgeServerSync_entry__final_) && (handle->stateConfVector[1] == ZBridgeServerSync_last_state) && (handle->stateConfVector[2] == ZBridgeServerSync_last_state) && (handle->stateConfVector[3] == ZBridgeServerSync_last_state);

}

static void zBridgeServerSync_clearInEvents(ZBridgeServerSync* handle)
{
	handle->iface.attemptSync_raised = bool_false;
	handle->iface.continue_raised = bool_false;
	handle->iface.confirmSync_raised = bool_false;
}

static void zBridgeServerSync_clearOutEvents(ZBridgeServerSync* handle)
{
	handle->iface.sendAttemptSync_raised = bool_false;
	handle->iface.sendAttemptSyncAll_raised = bool_false;
	handle->iface.sendConfirmSync_raised = bool_false;
	handle->iface.sendAllSync_raised = bool_false;
}

void zBridgeServerSync_runCycle(ZBridgeServerSync* handle)
{
	
	zBridgeServerSync_clearOutEvents(handle);
	
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < ZBRIDGESERVERSYNC_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync :
		{
			zBridgeServerSync_react_entry_SynChronizeServer_West_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync :
		{
			zBridgeServerSync_react_entry_SynChronizeServer_West_waitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync :
		{
			zBridgeServerSync_react_entry_SynChronizeServer_North_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync :
		{
			zBridgeServerSync_react_entry_SynChronizeServer_North_waitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync :
		{
			zBridgeServerSync_react_entry_SynChronizeServer_East_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync :
		{
			zBridgeServerSync_react_entry_SynChronizeServer_East_waitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync :
		{
			zBridgeServerSync_react_entry_SynChronizeServer_South_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync :
		{
			zBridgeServerSync_react_entry_SynChronizeServer_South_waitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync :
		{
			zBridgeServerSync_react_entry_ClientConfirm_West_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync :
		{
			zBridgeServerSync_react_entry_ClientConfirm_West_WaitForSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync :
		{
			zBridgeServerSync_react_entry_ClientConfirm_North_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync :
		{
			zBridgeServerSync_react_entry_ClientConfirm_North_WaitForSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync :
		{
			zBridgeServerSync_react_entry_ClientConfirm_East_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync :
		{
			zBridgeServerSync_react_entry_ClientConfirm_East_WaitForSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync :
		{
			zBridgeServerSync_react_entry_ClientConfirm_South_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync :
		{
			zBridgeServerSync_react_entry_ClientConfirm_South_WaitForSync(handle);
			break;
		}
		case ZBridgeServerSync_entry__final_ :
		{
			zBridgeServerSync_react_entry__final_(handle);
			break;
		}
		case ZBridgeServerSync_entry_AttemptSync :
		{
			zBridgeServerSync_react_entry_AttemptSync(handle);
			break;
		}
		default:
			break;
		}
	}
	
	zBridgeServerSync_clearInEvents(handle);
}


sc_boolean zBridgeServerSync_isStateActive(const ZBridgeServerSync* handle, ZBridgeServerSyncStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case ZBridgeServerSync_entry_SynChronizeServer :
			result = (sc_boolean) (handle->stateConfVector[0] >= ZBridgeServerSync_entry_SynChronizeServer
				&& handle->stateConfVector[0] <= ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync);
			break;
		case ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync
			);
			break;
		case ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync
			);
			break;
		case ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync :
			result = (sc_boolean) (handle->stateConfVector[1] == ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync
			);
			break;
		case ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync :
			result = (sc_boolean) (handle->stateConfVector[1] == ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync
			);
			break;
		case ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync :
			result = (sc_boolean) (handle->stateConfVector[2] == ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync
			);
			break;
		case ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync :
			result = (sc_boolean) (handle->stateConfVector[2] == ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync
			);
			break;
		case ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync :
			result = (sc_boolean) (handle->stateConfVector[3] == ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync
			);
			break;
		case ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync :
			result = (sc_boolean) (handle->stateConfVector[3] == ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync
			);
			break;
		case ZBridgeServerSync_entry_ClientConfirm :
			result = (sc_boolean) (handle->stateConfVector[0] >= ZBridgeServerSync_entry_ClientConfirm
				&& handle->stateConfVector[0] <= ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync);
			break;
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync
			);
			break;
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync
			);
			break;
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync :
			result = (sc_boolean) (handle->stateConfVector[1] == ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync
			);
			break;
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync :
			result = (sc_boolean) (handle->stateConfVector[1] == ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync
			);
			break;
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync :
			result = (sc_boolean) (handle->stateConfVector[2] == ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync
			);
			break;
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync :
			result = (sc_boolean) (handle->stateConfVector[2] == ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync
			);
			break;
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync :
			result = (sc_boolean) (handle->stateConfVector[3] == ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync
			);
			break;
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync :
			result = (sc_boolean) (handle->stateConfVector[3] == ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync
			);
			break;
		case ZBridgeServerSync_entry__final_ :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeServerSync_entry__final_
			);
			break;
		case ZBridgeServerSync_entry_AttemptSync :
			result = (sc_boolean) (handle->stateConfVector[0] == ZBridgeServerSync_entry_AttemptSync
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

void zBridgeServerSyncIface_raise_attemptSync(ZBridgeServerSync* handle, sc_integer value)
{
	handle->iface.attemptSync_value = value;
	handle->iface.attemptSync_raised = bool_true;
}
void zBridgeServerSyncIface_raise_continue(ZBridgeServerSync* handle)
{
	handle->iface.continue_raised = bool_true;
}
void zBridgeServerSyncIface_raise_confirmSync(ZBridgeServerSync* handle, sc_integer value)
{
	handle->iface.confirmSync_value = value;
	handle->iface.confirmSync_raised = bool_true;
}

sc_boolean zBridgeServerSyncIface_israised_sendAttemptSync(const ZBridgeServerSync* handle)
{
	return handle->iface.sendAttemptSync_raised;
}
sc_integer zBridgeServerSyncIface_get_sendAttemptSync_value(const ZBridgeServerSync* handle)
{
	return handle->iface.sendAttemptSync_value;
}
sc_boolean zBridgeServerSyncIface_israised_sendAttemptSyncAll(const ZBridgeServerSync* handle)
{
	return handle->iface.sendAttemptSyncAll_raised;
}
sc_boolean zBridgeServerSyncIface_israised_sendConfirmSync(const ZBridgeServerSync* handle)
{
	return handle->iface.sendConfirmSync_raised;
}
sc_boolean zBridgeServerSyncIface_israised_sendAllSync(const ZBridgeServerSync* handle)
{
	return handle->iface.sendAllSync_raised;
}


/* implementations of all internal functions */

static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_West_WaitForAttemptSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return ((handle->iface.attemptSync_raised) && (handle->iface.attemptSync_value == handle->internal.W)) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_West_waitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return (handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync) && handle->iface.continue_raised) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_North_WaitForAttemptSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return ((handle->iface.attemptSync_raised) && (handle->iface.attemptSync_value == handle->internal.N)) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_North_waitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return (handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync) && handle->iface.continue_raised) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_East_WaitForAttemptSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return ((handle->iface.attemptSync_raised) && (handle->iface.attemptSync_value == handle->internal.E)) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_East_waitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return (handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync) && handle->iface.continue_raised) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_South_WaitForAttemptSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return ((handle->iface.attemptSync_raised) && (handle->iface.attemptSync_value == handle->internal.S)) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_SynChronizeServer_South_waitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return (handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync) && handle->iface.continue_raised) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_West_WaitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return (handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync) && handle->iface.continue_raised) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_West_WaitForSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return ((handle->iface.confirmSync_raised) && (handle->iface.confirmSync_value == handle->internal.W)) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_North_WaitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return (handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync) && handle->iface.continue_raised) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_North_WaitForSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return ((handle->iface.confirmSync_raised) && (handle->iface.confirmSync_value == handle->internal.N)) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_East_WaitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return (handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync) && handle->iface.continue_raised) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_East_WaitForSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return ((handle->iface.confirmSync_raised) && (handle->iface.confirmSync_value == handle->internal.E)) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_South_WaitForConfirmSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return (handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync) && handle->iface.continue_raised && zBridgeServerSync_isStateActive(handle, ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync) && handle->iface.continue_raised) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_ClientConfirm_South_WaitForSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return ((handle->iface.confirmSync_raised) && (handle->iface.confirmSync_value == handle->internal.S)) ? bool_true : bool_false;
}

static sc_boolean zBridgeServerSync_check_entry_AttemptSync_tr0_tr0(const ZBridgeServerSync* handle)
{
	return handle->iface.continue_raised;
}

static void zBridgeServerSync_effect_entry_SynChronizeServer_West_WaitForAttemptSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_SynChronizeServer_West_WaitForAttemptSync(handle);
	handle->internal.confirmWest = bool_true;
	handle->iface.sendAttemptSync_value = handle->internal.W;
	handle->iface.sendAttemptSync_raised = bool_true;
	zBridgeServerSync_enseq_entry_SynChronizeServer_West_waitForConfirmSync_default(handle);
}

static void zBridgeServerSync_effect_entry_SynChronizeServer_West_waitForConfirmSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_SynChronizeServer(handle);
	zBridgeServerSync_react_entry__sync0(handle);
}

static void zBridgeServerSync_effect_entry_SynChronizeServer_North_WaitForAttemptSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_SynChronizeServer_North_WaitForAttemptSync(handle);
	handle->internal.confirmNorth = bool_true;
	handle->iface.sendAttemptSync_value = handle->internal.N;
	handle->iface.sendAttemptSync_raised = bool_true;
	zBridgeServerSync_enseq_entry_SynChronizeServer_North_waitForConfirmSync_default(handle);
}

static void zBridgeServerSync_effect_entry_SynChronizeServer_North_waitForConfirmSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_SynChronizeServer(handle);
	zBridgeServerSync_react_entry__sync0(handle);
}

static void zBridgeServerSync_effect_entry_SynChronizeServer_East_WaitForAttemptSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_SynChronizeServer_East_WaitForAttemptSync(handle);
	handle->internal.confirmEast = bool_true;
	handle->iface.sendAttemptSync_value = handle->internal.E;
	handle->iface.sendAttemptSync_raised = bool_true;
	zBridgeServerSync_enseq_entry_SynChronizeServer_East_waitForConfirmSync_default(handle);
}

static void zBridgeServerSync_effect_entry_SynChronizeServer_East_waitForConfirmSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_SynChronizeServer(handle);
	zBridgeServerSync_react_entry__sync0(handle);
}

static void zBridgeServerSync_effect_entry_SynChronizeServer_South_WaitForAttemptSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_SynChronizeServer_South_WaitForAttemptSync(handle);
	handle->internal.confirmSouth = bool_true;
	handle->iface.sendAttemptSync_value = handle->internal.S;
	handle->iface.sendAttemptSync_raised = bool_true;
	zBridgeServerSync_enseq_entry_SynChronizeServer_South_waitForConfirmSync_default(handle);
}

static void zBridgeServerSync_effect_entry_SynChronizeServer_South_waitForConfirmSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_SynChronizeServer(handle);
	zBridgeServerSync_react_entry__sync0(handle);
}

static void zBridgeServerSync_effect_entry_ClientConfirm_West_WaitForConfirmSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_ClientConfirm(handle);
	zBridgeServerSync_react_entry__sync1(handle);
}

static void zBridgeServerSync_effect_entry_ClientConfirm_West_WaitForSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_ClientConfirm_West_WaitForSync(handle);
	handle->internal.confirmWest = bool_true;
	zBridgeServerSync_enseq_entry_ClientConfirm_West_WaitForConfirmSync_default(handle);
}

static void zBridgeServerSync_effect_entry_ClientConfirm_North_WaitForConfirmSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_ClientConfirm(handle);
	zBridgeServerSync_react_entry__sync1(handle);
}

static void zBridgeServerSync_effect_entry_ClientConfirm_North_WaitForSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_ClientConfirm_North_WaitForSync(handle);
	handle->internal.confirmNorth = bool_true;
	zBridgeServerSync_enseq_entry_ClientConfirm_North_WaitForConfirmSync_default(handle);
}

static void zBridgeServerSync_effect_entry_ClientConfirm_East_WaitForConfirmSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_ClientConfirm(handle);
	zBridgeServerSync_react_entry__sync1(handle);
}

static void zBridgeServerSync_effect_entry_ClientConfirm_East_WaitForSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_ClientConfirm_East_WaitForSync(handle);
	handle->internal.confirmEast = bool_true;
	zBridgeServerSync_enseq_entry_ClientConfirm_East_WaitForConfirmSync_default(handle);
}

static void zBridgeServerSync_effect_entry_ClientConfirm_South_WaitForConfirmSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_ClientConfirm(handle);
	zBridgeServerSync_react_entry__sync1(handle);
}

static void zBridgeServerSync_effect_entry_ClientConfirm_South_WaitForSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_ClientConfirm_South_WaitForSync(handle);
	handle->internal.confirmSouth = bool_true;
	zBridgeServerSync_enseq_entry_ClientConfirm_South_WaitForConfirmSync_default(handle);
}

static void zBridgeServerSync_effect_entry_AttemptSync_tr0(ZBridgeServerSync* handle)
{
	zBridgeServerSync_exseq_entry_AttemptSync(handle);
	handle->iface.sendAttemptSyncAll_raised = bool_true;
	zBridgeServerSync_enseq_entry_SynChronizeServer_default(handle);
}

/* Entry action for state 'waitForConfirmSync'. */
static void zBridgeServerSync_enact_entry_SynChronizeServer_West_waitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Entry action for state 'waitForConfirmSync'. */
	if ((handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) ? bool_true : bool_false == bool_true)
	{ 
		handle->iface.sendConfirmSync_raised = bool_true;
	} 
}

/* Entry action for state 'waitForConfirmSync'. */
static void zBridgeServerSync_enact_entry_SynChronizeServer_North_waitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Entry action for state 'waitForConfirmSync'. */
	if ((handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) ? bool_true : bool_false == bool_true)
	{ 
		handle->iface.sendConfirmSync_raised = bool_true;
	} 
}

/* Entry action for state 'waitForConfirmSync'. */
static void zBridgeServerSync_enact_entry_SynChronizeServer_East_waitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Entry action for state 'waitForConfirmSync'. */
	if ((handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) ? bool_true : bool_false == bool_true)
	{ 
		handle->iface.sendConfirmSync_raised = bool_true;
	} 
}

/* Entry action for state 'waitForConfirmSync'. */
static void zBridgeServerSync_enact_entry_SynChronizeServer_South_waitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Entry action for state 'waitForConfirmSync'. */
	if ((handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) ? bool_true : bool_false == bool_true)
	{ 
		handle->iface.sendConfirmSync_raised = bool_true;
	} 
}

/* Entry action for state 'WaitForConfirmSync'. */
static void zBridgeServerSync_enact_entry_ClientConfirm_West_WaitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Entry action for state 'WaitForConfirmSync'. */
	if ((handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) ? bool_true : bool_false == bool_true)
	{ 
		handle->iface.sendAllSync_raised = bool_true;
	} 
}

/* Entry action for state 'WaitForConfirmSync'. */
static void zBridgeServerSync_enact_entry_ClientConfirm_North_WaitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Entry action for state 'WaitForConfirmSync'. */
	if ((handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) ? bool_true : bool_false == bool_true)
	{ 
		handle->iface.sendAllSync_raised = bool_true;
	} 
}

/* Entry action for state 'WaitForConfirmSync'. */
static void zBridgeServerSync_enact_entry_ClientConfirm_East_WaitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Entry action for state 'WaitForConfirmSync'. */
	if ((handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) ? bool_true : bool_false == bool_true)
	{ 
		handle->iface.sendAllSync_raised = bool_true;
	} 
}

/* Entry action for state 'WaitForConfirmSync'. */
static void zBridgeServerSync_enact_entry_ClientConfirm_South_WaitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Entry action for state 'WaitForConfirmSync'. */
	if ((handle->internal.confirmWest && handle->internal.confirmNorth && handle->internal.confirmEast && handle->internal.confirmSouth) ? bool_true : bool_false == bool_true)
	{ 
		handle->iface.sendAllSync_raised = bool_true;
	} 
}

/* 'default' enter sequence for state SynChronizeServer */
static void zBridgeServerSync_enseq_entry_SynChronizeServer_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state SynChronizeServer */
	zBridgeServerSync_enseq_entry_SynChronizeServer_West_default(handle);
	zBridgeServerSync_enseq_entry_SynChronizeServer_North_default(handle);
	zBridgeServerSync_enseq_entry_SynChronizeServer_East_default(handle);
	zBridgeServerSync_enseq_entry_SynChronizeServer_South_default(handle);
}

/* 'default' enter sequence for state WaitForAttemptSync */
static void zBridgeServerSync_enseq_entry_SynChronizeServer_West_WaitForAttemptSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state WaitForAttemptSync */
	handle->stateConfVector[0] = ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state waitForConfirmSync */
static void zBridgeServerSync_enseq_entry_SynChronizeServer_West_waitForConfirmSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state waitForConfirmSync */
	zBridgeServerSync_enact_entry_SynChronizeServer_West_waitForConfirmSync(handle);
	handle->stateConfVector[0] = ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state WaitForAttemptSync */
static void zBridgeServerSync_enseq_entry_SynChronizeServer_North_WaitForAttemptSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state WaitForAttemptSync */
	handle->stateConfVector[1] = ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync;
	handle->stateConfVectorPosition = 1;
}

/* 'default' enter sequence for state waitForConfirmSync */
static void zBridgeServerSync_enseq_entry_SynChronizeServer_North_waitForConfirmSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state waitForConfirmSync */
	zBridgeServerSync_enact_entry_SynChronizeServer_North_waitForConfirmSync(handle);
	handle->stateConfVector[1] = ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync;
	handle->stateConfVectorPosition = 1;
}

/* 'default' enter sequence for state WaitForAttemptSync */
static void zBridgeServerSync_enseq_entry_SynChronizeServer_East_WaitForAttemptSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state WaitForAttemptSync */
	handle->stateConfVector[2] = ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync;
	handle->stateConfVectorPosition = 2;
}

/* 'default' enter sequence for state waitForConfirmSync */
static void zBridgeServerSync_enseq_entry_SynChronizeServer_East_waitForConfirmSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state waitForConfirmSync */
	zBridgeServerSync_enact_entry_SynChronizeServer_East_waitForConfirmSync(handle);
	handle->stateConfVector[2] = ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync;
	handle->stateConfVectorPosition = 2;
}

/* 'default' enter sequence for state WaitForAttemptSync */
static void zBridgeServerSync_enseq_entry_SynChronizeServer_South_WaitForAttemptSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state WaitForAttemptSync */
	handle->stateConfVector[3] = ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync;
	handle->stateConfVectorPosition = 3;
}

/* 'default' enter sequence for state waitForConfirmSync */
static void zBridgeServerSync_enseq_entry_SynChronizeServer_South_waitForConfirmSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state waitForConfirmSync */
	zBridgeServerSync_enact_entry_SynChronizeServer_South_waitForConfirmSync(handle);
	handle->stateConfVector[3] = ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync;
	handle->stateConfVectorPosition = 3;
}

/* 'default' enter sequence for state ClientConfirm */
static void zBridgeServerSync_enseq_entry_ClientConfirm_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state ClientConfirm */
	zBridgeServerSync_enseq_entry_ClientConfirm_West_default(handle);
	zBridgeServerSync_enseq_entry_ClientConfirm_North_default(handle);
	zBridgeServerSync_enseq_entry_ClientConfirm_East_default(handle);
	zBridgeServerSync_enseq_entry_ClientConfirm_South_default(handle);
}

/* 'default' enter sequence for state WaitForConfirmSync */
static void zBridgeServerSync_enseq_entry_ClientConfirm_West_WaitForConfirmSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state WaitForConfirmSync */
	zBridgeServerSync_enact_entry_ClientConfirm_West_WaitForConfirmSync(handle);
	handle->stateConfVector[0] = ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state WaitForSync */
static void zBridgeServerSync_enseq_entry_ClientConfirm_West_WaitForSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state WaitForSync */
	handle->stateConfVector[0] = ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state WaitForConfirmSync */
static void zBridgeServerSync_enseq_entry_ClientConfirm_North_WaitForConfirmSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state WaitForConfirmSync */
	zBridgeServerSync_enact_entry_ClientConfirm_North_WaitForConfirmSync(handle);
	handle->stateConfVector[1] = ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync;
	handle->stateConfVectorPosition = 1;
}

/* 'default' enter sequence for state WaitForSync */
static void zBridgeServerSync_enseq_entry_ClientConfirm_North_WaitForSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state WaitForSync */
	handle->stateConfVector[1] = ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync;
	handle->stateConfVectorPosition = 1;
}

/* 'default' enter sequence for state WaitForConfirmSync */
static void zBridgeServerSync_enseq_entry_ClientConfirm_East_WaitForConfirmSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state WaitForConfirmSync */
	zBridgeServerSync_enact_entry_ClientConfirm_East_WaitForConfirmSync(handle);
	handle->stateConfVector[2] = ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync;
	handle->stateConfVectorPosition = 2;
}

/* 'default' enter sequence for state WaitForSync */
static void zBridgeServerSync_enseq_entry_ClientConfirm_East_WaitForSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state WaitForSync */
	handle->stateConfVector[2] = ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync;
	handle->stateConfVectorPosition = 2;
}

/* 'default' enter sequence for state WaitForConfirmSync */
static void zBridgeServerSync_enseq_entry_ClientConfirm_South_WaitForConfirmSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state WaitForConfirmSync */
	zBridgeServerSync_enact_entry_ClientConfirm_South_WaitForConfirmSync(handle);
	handle->stateConfVector[3] = ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync;
	handle->stateConfVectorPosition = 3;
}

/* 'default' enter sequence for state WaitForSync */
static void zBridgeServerSync_enseq_entry_ClientConfirm_South_WaitForSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state WaitForSync */
	handle->stateConfVector[3] = ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync;
	handle->stateConfVectorPosition = 3;
}

/* Default enter sequence for state null */
static void zBridgeServerSync_enseq_entry__final__default(ZBridgeServerSync* handle)
{
	/* Default enter sequence for state null */
	handle->stateConfVector[0] = ZBridgeServerSync_entry__final_;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state AttemptSync */
static void zBridgeServerSync_enseq_entry_AttemptSync_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for state AttemptSync */
	handle->stateConfVector[0] = ZBridgeServerSync_entry_AttemptSync;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region entry */
static void zBridgeServerSync_enseq_entry_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for region entry */
	zBridgeServerSync_react_entry__entry_Default(handle);
}

/* 'default' enter sequence for region West */
static void zBridgeServerSync_enseq_entry_SynChronizeServer_West_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for region West */
	zBridgeServerSync_react_entry_SynChronizeServer_West__entry_Default(handle);
}

/* 'default' enter sequence for region North */
static void zBridgeServerSync_enseq_entry_SynChronizeServer_North_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for region North */
	zBridgeServerSync_react_entry_SynChronizeServer_North__entry_Default(handle);
}

/* 'default' enter sequence for region East */
static void zBridgeServerSync_enseq_entry_SynChronizeServer_East_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for region East */
	zBridgeServerSync_react_entry_SynChronizeServer_East__entry_Default(handle);
}

/* 'default' enter sequence for region South */
static void zBridgeServerSync_enseq_entry_SynChronizeServer_South_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for region South */
	zBridgeServerSync_react_entry_SynChronizeServer_South__entry_Default(handle);
}

/* 'default' enter sequence for region West */
static void zBridgeServerSync_enseq_entry_ClientConfirm_West_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for region West */
	zBridgeServerSync_react_entry_ClientConfirm_West__entry_Default(handle);
}

/* 'default' enter sequence for region North */
static void zBridgeServerSync_enseq_entry_ClientConfirm_North_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for region North */
	zBridgeServerSync_react_entry_ClientConfirm_North__entry_Default(handle);
}

/* 'default' enter sequence for region East */
static void zBridgeServerSync_enseq_entry_ClientConfirm_East_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for region East */
	zBridgeServerSync_react_entry_ClientConfirm_East__entry_Default(handle);
}

/* 'default' enter sequence for region South */
static void zBridgeServerSync_enseq_entry_ClientConfirm_South_default(ZBridgeServerSync* handle)
{
	/* 'default' enter sequence for region South */
	zBridgeServerSync_react_entry_ClientConfirm_South__entry_Default(handle);
}

/* Default exit sequence for state SynChronizeServer */
static void zBridgeServerSync_exseq_entry_SynChronizeServer(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state SynChronizeServer */
	zBridgeServerSync_exseq_entry_SynChronizeServer_West(handle);
	zBridgeServerSync_exseq_entry_SynChronizeServer_North(handle);
	zBridgeServerSync_exseq_entry_SynChronizeServer_East(handle);
	zBridgeServerSync_exseq_entry_SynChronizeServer_South(handle);
}

/* Default exit sequence for state WaitForAttemptSync */
static void zBridgeServerSync_exseq_entry_SynChronizeServer_West_WaitForAttemptSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state WaitForAttemptSync */
	handle->stateConfVector[0] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state waitForConfirmSync */
static void zBridgeServerSync_exseq_entry_SynChronizeServer_West_waitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state waitForConfirmSync */
	handle->stateConfVector[0] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state WaitForAttemptSync */
static void zBridgeServerSync_exseq_entry_SynChronizeServer_North_WaitForAttemptSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state WaitForAttemptSync */
	handle->stateConfVector[1] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 1;
}

/* Default exit sequence for state waitForConfirmSync */
static void zBridgeServerSync_exseq_entry_SynChronizeServer_North_waitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state waitForConfirmSync */
	handle->stateConfVector[1] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 1;
}

/* Default exit sequence for state WaitForAttemptSync */
static void zBridgeServerSync_exseq_entry_SynChronizeServer_East_WaitForAttemptSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state WaitForAttemptSync */
	handle->stateConfVector[2] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 2;
}

/* Default exit sequence for state waitForConfirmSync */
static void zBridgeServerSync_exseq_entry_SynChronizeServer_East_waitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state waitForConfirmSync */
	handle->stateConfVector[2] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 2;
}

/* Default exit sequence for state WaitForAttemptSync */
static void zBridgeServerSync_exseq_entry_SynChronizeServer_South_WaitForAttemptSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state WaitForAttemptSync */
	handle->stateConfVector[3] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 3;
}

/* Default exit sequence for state waitForConfirmSync */
static void zBridgeServerSync_exseq_entry_SynChronizeServer_South_waitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state waitForConfirmSync */
	handle->stateConfVector[3] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 3;
}

/* Default exit sequence for state ClientConfirm */
static void zBridgeServerSync_exseq_entry_ClientConfirm(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state ClientConfirm */
	zBridgeServerSync_exseq_entry_ClientConfirm_West(handle);
	zBridgeServerSync_exseq_entry_ClientConfirm_North(handle);
	zBridgeServerSync_exseq_entry_ClientConfirm_East(handle);
	zBridgeServerSync_exseq_entry_ClientConfirm_South(handle);
}

/* Default exit sequence for state WaitForConfirmSync */
static void zBridgeServerSync_exseq_entry_ClientConfirm_West_WaitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state WaitForConfirmSync */
	handle->stateConfVector[0] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state WaitForSync */
static void zBridgeServerSync_exseq_entry_ClientConfirm_West_WaitForSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state WaitForSync */
	handle->stateConfVector[0] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state WaitForConfirmSync */
static void zBridgeServerSync_exseq_entry_ClientConfirm_North_WaitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state WaitForConfirmSync */
	handle->stateConfVector[1] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 1;
}

/* Default exit sequence for state WaitForSync */
static void zBridgeServerSync_exseq_entry_ClientConfirm_North_WaitForSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state WaitForSync */
	handle->stateConfVector[1] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 1;
}

/* Default exit sequence for state WaitForConfirmSync */
static void zBridgeServerSync_exseq_entry_ClientConfirm_East_WaitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state WaitForConfirmSync */
	handle->stateConfVector[2] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 2;
}

/* Default exit sequence for state WaitForSync */
static void zBridgeServerSync_exseq_entry_ClientConfirm_East_WaitForSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state WaitForSync */
	handle->stateConfVector[2] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 2;
}

/* Default exit sequence for state WaitForConfirmSync */
static void zBridgeServerSync_exseq_entry_ClientConfirm_South_WaitForConfirmSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state WaitForConfirmSync */
	handle->stateConfVector[3] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 3;
}

/* Default exit sequence for state WaitForSync */
static void zBridgeServerSync_exseq_entry_ClientConfirm_South_WaitForSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state WaitForSync */
	handle->stateConfVector[3] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 3;
}

/* Default exit sequence for final state. */
static void zBridgeServerSync_exseq_entry__final_(ZBridgeServerSync* handle)
{
	/* Default exit sequence for final state. */
	handle->stateConfVector[0] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state AttemptSync */
static void zBridgeServerSync_exseq_entry_AttemptSync(ZBridgeServerSync* handle)
{
	/* Default exit sequence for state AttemptSync */
	handle->stateConfVector[0] = ZBridgeServerSync_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for region entry */
static void zBridgeServerSync_exseq_entry(ZBridgeServerSync* handle)
{
	/* Default exit sequence for region entry */
	/* Handle exit of all possible states (of ZBridgeServerSync.entry) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_West_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_West_waitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_West_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_West_WaitForSync(handle);
			break;
		}
		case ZBridgeServerSync_entry__final_ :
		{
			zBridgeServerSync_exseq_entry__final_(handle);
			break;
		}
		case ZBridgeServerSync_entry_AttemptSync :
		{
			zBridgeServerSync_exseq_entry_AttemptSync(handle);
			break;
		}
		default: break;
	}
	/* Handle exit of all possible states (of ZBridgeServerSync.entry) at position 1... */
	switch(handle->stateConfVector[ 1 ])
	{
		case ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_North_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_North_waitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_North_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_North_WaitForSync(handle);
			break;
		}
		default: break;
	}
	/* Handle exit of all possible states (of ZBridgeServerSync.entry) at position 2... */
	switch(handle->stateConfVector[ 2 ])
	{
		case ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_East_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_East_waitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_East_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_East_WaitForSync(handle);
			break;
		}
		default: break;
	}
	/* Handle exit of all possible states (of ZBridgeServerSync.entry) at position 3... */
	switch(handle->stateConfVector[ 3 ])
	{
		case ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_South_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_South_waitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_South_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_South_WaitForSync(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region West */
static void zBridgeServerSync_exseq_entry_SynChronizeServer_West(ZBridgeServerSync* handle)
{
	/* Default exit sequence for region West */
	/* Handle exit of all possible states (of ZBridgeServerSync.entry.SynChronizeServer.West) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case ZBridgeServerSync_entry_SynChronizeServer_West_WaitForAttemptSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_West_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_West_waitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_West_waitForConfirmSync(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region North */
static void zBridgeServerSync_exseq_entry_SynChronizeServer_North(ZBridgeServerSync* handle)
{
	/* Default exit sequence for region North */
	/* Handle exit of all possible states (of ZBridgeServerSync.entry.SynChronizeServer.North) at position 1... */
	switch(handle->stateConfVector[ 1 ])
	{
		case ZBridgeServerSync_entry_SynChronizeServer_North_WaitForAttemptSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_North_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_North_waitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_North_waitForConfirmSync(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region East */
static void zBridgeServerSync_exseq_entry_SynChronizeServer_East(ZBridgeServerSync* handle)
{
	/* Default exit sequence for region East */
	/* Handle exit of all possible states (of ZBridgeServerSync.entry.SynChronizeServer.East) at position 2... */
	switch(handle->stateConfVector[ 2 ])
	{
		case ZBridgeServerSync_entry_SynChronizeServer_East_WaitForAttemptSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_East_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_East_waitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_East_waitForConfirmSync(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region South */
static void zBridgeServerSync_exseq_entry_SynChronizeServer_South(ZBridgeServerSync* handle)
{
	/* Default exit sequence for region South */
	/* Handle exit of all possible states (of ZBridgeServerSync.entry.SynChronizeServer.South) at position 3... */
	switch(handle->stateConfVector[ 3 ])
	{
		case ZBridgeServerSync_entry_SynChronizeServer_South_WaitForAttemptSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_South_WaitForAttemptSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_SynChronizeServer_South_waitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_SynChronizeServer_South_waitForConfirmSync(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region West */
static void zBridgeServerSync_exseq_entry_ClientConfirm_West(ZBridgeServerSync* handle)
{
	/* Default exit sequence for region West */
	/* Handle exit of all possible states (of ZBridgeServerSync.entry.ClientConfirm.West) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_West_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_West_WaitForSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_West_WaitForSync(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region North */
static void zBridgeServerSync_exseq_entry_ClientConfirm_North(ZBridgeServerSync* handle)
{
	/* Default exit sequence for region North */
	/* Handle exit of all possible states (of ZBridgeServerSync.entry.ClientConfirm.North) at position 1... */
	switch(handle->stateConfVector[ 1 ])
	{
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_North_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_North_WaitForSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_North_WaitForSync(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region East */
static void zBridgeServerSync_exseq_entry_ClientConfirm_East(ZBridgeServerSync* handle)
{
	/* Default exit sequence for region East */
	/* Handle exit of all possible states (of ZBridgeServerSync.entry.ClientConfirm.East) at position 2... */
	switch(handle->stateConfVector[ 2 ])
	{
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_East_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_East_WaitForSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_East_WaitForSync(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region South */
static void zBridgeServerSync_exseq_entry_ClientConfirm_South(ZBridgeServerSync* handle)
{
	/* Default exit sequence for region South */
	/* Handle exit of all possible states (of ZBridgeServerSync.entry.ClientConfirm.South) at position 3... */
	switch(handle->stateConfVector[ 3 ])
	{
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForConfirmSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_South_WaitForConfirmSync(handle);
			break;
		}
		case ZBridgeServerSync_entry_ClientConfirm_South_WaitForSync :
		{
			zBridgeServerSync_exseq_entry_ClientConfirm_South_WaitForSync(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state WaitForAttemptSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_West_WaitForAttemptSync(ZBridgeServerSync* handle)
{
	/* The reactions of state WaitForAttemptSync. */
	if (zBridgeServerSync_check_entry_SynChronizeServer_West_WaitForAttemptSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_SynChronizeServer_West_WaitForAttemptSync_tr0(handle);
	} 
}

/* The reactions of state waitForConfirmSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_West_waitForConfirmSync(ZBridgeServerSync* handle)
{
	/* The reactions of state waitForConfirmSync. */
	if (zBridgeServerSync_check_entry_SynChronizeServer_West_waitForConfirmSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_SynChronizeServer_West_waitForConfirmSync_tr0(handle);
	} 
}

/* The reactions of state WaitForAttemptSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_North_WaitForAttemptSync(ZBridgeServerSync* handle)
{
	/* The reactions of state WaitForAttemptSync. */
	if (zBridgeServerSync_check_entry_SynChronizeServer_North_WaitForAttemptSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_SynChronizeServer_North_WaitForAttemptSync_tr0(handle);
	} 
}

/* The reactions of state waitForConfirmSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_North_waitForConfirmSync(ZBridgeServerSync* handle)
{
	/* The reactions of state waitForConfirmSync. */
	if (zBridgeServerSync_check_entry_SynChronizeServer_North_waitForConfirmSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_SynChronizeServer_North_waitForConfirmSync_tr0(handle);
	} 
}

/* The reactions of state WaitForAttemptSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_East_WaitForAttemptSync(ZBridgeServerSync* handle)
{
	/* The reactions of state WaitForAttemptSync. */
	if (zBridgeServerSync_check_entry_SynChronizeServer_East_WaitForAttemptSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_SynChronizeServer_East_WaitForAttemptSync_tr0(handle);
	} 
}

/* The reactions of state waitForConfirmSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_East_waitForConfirmSync(ZBridgeServerSync* handle)
{
	/* The reactions of state waitForConfirmSync. */
	if (zBridgeServerSync_check_entry_SynChronizeServer_East_waitForConfirmSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_SynChronizeServer_East_waitForConfirmSync_tr0(handle);
	} 
}

/* The reactions of state WaitForAttemptSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_South_WaitForAttemptSync(ZBridgeServerSync* handle)
{
	/* The reactions of state WaitForAttemptSync. */
	if (zBridgeServerSync_check_entry_SynChronizeServer_South_WaitForAttemptSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_SynChronizeServer_South_WaitForAttemptSync_tr0(handle);
	} 
}

/* The reactions of state waitForConfirmSync. */
static void zBridgeServerSync_react_entry_SynChronizeServer_South_waitForConfirmSync(ZBridgeServerSync* handle)
{
	/* The reactions of state waitForConfirmSync. */
	if (zBridgeServerSync_check_entry_SynChronizeServer_South_waitForConfirmSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_SynChronizeServer_South_waitForConfirmSync_tr0(handle);
	} 
}

/* The reactions of state WaitForConfirmSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_West_WaitForConfirmSync(ZBridgeServerSync* handle)
{
	/* The reactions of state WaitForConfirmSync. */
	if (zBridgeServerSync_check_entry_ClientConfirm_West_WaitForConfirmSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_ClientConfirm_West_WaitForConfirmSync_tr0(handle);
	} 
}

/* The reactions of state WaitForSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_West_WaitForSync(ZBridgeServerSync* handle)
{
	/* The reactions of state WaitForSync. */
	if (zBridgeServerSync_check_entry_ClientConfirm_West_WaitForSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_ClientConfirm_West_WaitForSync_tr0(handle);
	} 
}

/* The reactions of state WaitForConfirmSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_North_WaitForConfirmSync(ZBridgeServerSync* handle)
{
	/* The reactions of state WaitForConfirmSync. */
	if (zBridgeServerSync_check_entry_ClientConfirm_North_WaitForConfirmSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_ClientConfirm_North_WaitForConfirmSync_tr0(handle);
	} 
}

/* The reactions of state WaitForSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_North_WaitForSync(ZBridgeServerSync* handle)
{
	/* The reactions of state WaitForSync. */
	if (zBridgeServerSync_check_entry_ClientConfirm_North_WaitForSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_ClientConfirm_North_WaitForSync_tr0(handle);
	} 
}

/* The reactions of state WaitForConfirmSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_East_WaitForConfirmSync(ZBridgeServerSync* handle)
{
	/* The reactions of state WaitForConfirmSync. */
	if (zBridgeServerSync_check_entry_ClientConfirm_East_WaitForConfirmSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_ClientConfirm_East_WaitForConfirmSync_tr0(handle);
	} 
}

/* The reactions of state WaitForSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_East_WaitForSync(ZBridgeServerSync* handle)
{
	/* The reactions of state WaitForSync. */
	if (zBridgeServerSync_check_entry_ClientConfirm_East_WaitForSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_ClientConfirm_East_WaitForSync_tr0(handle);
	} 
}

/* The reactions of state WaitForConfirmSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_South_WaitForConfirmSync(ZBridgeServerSync* handle)
{
	/* The reactions of state WaitForConfirmSync. */
	if (zBridgeServerSync_check_entry_ClientConfirm_South_WaitForConfirmSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_ClientConfirm_South_WaitForConfirmSync_tr0(handle);
	} 
}

/* The reactions of state WaitForSync. */
static void zBridgeServerSync_react_entry_ClientConfirm_South_WaitForSync(ZBridgeServerSync* handle)
{
	/* The reactions of state WaitForSync. */
	if (zBridgeServerSync_check_entry_ClientConfirm_South_WaitForSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_ClientConfirm_South_WaitForSync_tr0(handle);
	} 
}

/* The reactions of state null. */
static void zBridgeServerSync_react_entry__final_(ZBridgeServerSync* handle)
{
	/* The reactions of state null. */
}

/* The reactions of state AttemptSync. */
static void zBridgeServerSync_react_entry_AttemptSync(ZBridgeServerSync* handle)
{
	/* The reactions of state AttemptSync. */
	if (zBridgeServerSync_check_entry_AttemptSync_tr0_tr0(handle) == bool_true)
	{ 
		zBridgeServerSync_effect_entry_AttemptSync_tr0(handle);
	} 
}

/* Default react sequence for initial entry  */
static void zBridgeServerSync_react_entry__entry_Default(ZBridgeServerSync* handle)
{
	/* Default react sequence for initial entry  */
	zBridgeServerSync_enseq_entry_AttemptSync_default(handle);
}

/* Default react sequence for initial entry  */
static void zBridgeServerSync_react_entry_SynChronizeServer_West__entry_Default(ZBridgeServerSync* handle)
{
	/* Default react sequence for initial entry  */
	zBridgeServerSync_enseq_entry_SynChronizeServer_West_WaitForAttemptSync_default(handle);
}

/* Default react sequence for initial entry  */
static void zBridgeServerSync_react_entry_SynChronizeServer_North__entry_Default(ZBridgeServerSync* handle)
{
	/* Default react sequence for initial entry  */
	zBridgeServerSync_enseq_entry_SynChronizeServer_North_WaitForAttemptSync_default(handle);
}

/* Default react sequence for initial entry  */
static void zBridgeServerSync_react_entry_SynChronizeServer_East__entry_Default(ZBridgeServerSync* handle)
{
	/* Default react sequence for initial entry  */
	zBridgeServerSync_enseq_entry_SynChronizeServer_East_WaitForAttemptSync_default(handle);
}

/* Default react sequence for initial entry  */
static void zBridgeServerSync_react_entry_SynChronizeServer_South__entry_Default(ZBridgeServerSync* handle)
{
	/* Default react sequence for initial entry  */
	zBridgeServerSync_enseq_entry_SynChronizeServer_South_WaitForAttemptSync_default(handle);
}

/* Default react sequence for initial entry  */
static void zBridgeServerSync_react_entry_ClientConfirm_West__entry_Default(ZBridgeServerSync* handle)
{
	/* Default react sequence for initial entry  */
	zBridgeServerSync_enseq_entry_ClientConfirm_West_WaitForSync_default(handle);
}

/* Default react sequence for initial entry  */
static void zBridgeServerSync_react_entry_ClientConfirm_North__entry_Default(ZBridgeServerSync* handle)
{
	/* Default react sequence for initial entry  */
	zBridgeServerSync_enseq_entry_ClientConfirm_North_WaitForSync_default(handle);
}

/* Default react sequence for initial entry  */
static void zBridgeServerSync_react_entry_ClientConfirm_East__entry_Default(ZBridgeServerSync* handle)
{
	/* Default react sequence for initial entry  */
	zBridgeServerSync_enseq_entry_ClientConfirm_East_WaitForSync_default(handle);
}

/* Default react sequence for initial entry  */
static void zBridgeServerSync_react_entry_ClientConfirm_South__entry_Default(ZBridgeServerSync* handle)
{
	/* Default react sequence for initial entry  */
	zBridgeServerSync_enseq_entry_ClientConfirm_South_WaitForSync_default(handle);
}

/* The reactions of state null. */
static void zBridgeServerSync_react_entry__sync0(ZBridgeServerSync* handle)
{
	/* The reactions of state null. */
	handle->internal.confirmWest = bool_false;
	handle->internal.confirmNorth = bool_false;
	handle->internal.confirmEast = bool_false;
	handle->internal.confirmSouth = bool_false;
	zBridgeServerSync_enseq_entry_ClientConfirm_default(handle);
}

/* The reactions of state null. */
static void zBridgeServerSync_react_entry__sync1(ZBridgeServerSync* handle)
{
	/* The reactions of state null. */
	zBridgeServerSync_enseq_entry__final__default(handle);
}


