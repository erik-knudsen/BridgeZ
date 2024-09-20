ItemisCreate (Yakindu) ver. 3.0.0 is used.

ItemisCREATE upgrade from 3.x to 4.x has been tested and works with
the following updates (but is not used due to more strict license 
conditions):

In ZBridgeClient.sgen, ZBridgeClientSync.sgen, ZBridgeServer.sgen, 
ZBridgeServerSync.sgen
add:

	feature OutEventAPI {
		getters = true
		observables = false
	}
	
In ZBridgeClient.sct, ZBridgeClientSync.sct, ZBridgeServer.sct, 
ZBridgeServerSync.sct
add:

	@CycleBased(200)
	@ParentFirstExecution
	@EventBuffering(false, true)
	
	remove all "readonly" markings of internal variables.
	
In cactorlocal.h, cactorlocal.cpp, cactorlocalauto.h, cactorlocalauto.cpp,
ctblmngrclient.cpp, ctblmngrserver.cpp, ctblmngrserverauto.cpp:
	Many changes, all just name changes due to new naming conventions
	in ItemisCREATE upgrade from 3.x to 4.x.