#include "pch.h"
#include "SPIPrinter.h"

DWORD WINAPI WFPSendEvent(LPVOID lpParam)
{
	int i = 0;
	while (true) {
		if (!g_wfs_event.empty())
		{
			std::map<HWND, WFS_EVENTS>::iterator it;
			for (it = g_wfs_event.begin(); it != g_wfs_event.end(); ++it)
			{
				if (((*it).second.dwEvent & SERVICE_EVENTS) == SERVICE_EVENTS
					|| ((*it).second.dwEvent & USER_EVENTS) == USER_EVENTS
					|| ((*it).second.dwEvent & SYSTEM_EVENTS) == SYSTEM_EVENTS
					|| ((*it).second.dwEvent & EXECUTE_EVENTS) == EXECUTE_EVENTS)
				{
					if (i % 2 == 0)
					{
						LPWFSRESULT lpWFSResult;
						// allocate WFSResult buffer
						if (WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (LPVOID*)&lpWFSResult) != WFS_SUCCESS)
						{
							return WFS_ERR_INTERNAL_ERROR;
						}
						lpWFSResult->hResult = WFS_SERVICE_EVENT;
						lpWFSResult->hService = (*it).second.hService;
						lpWFSResult->RequestID = NULL;
						lpWFSResult->u.dwEventID = WFS_SRVE_PTR_MEDIAINSERTED;
						lpWFSResult->lpBuffer = NULL;

						SendMessage((*it).first, WFS_SERVICE_EVENT, 0, (LPARAM)lpWFSResult);
					}
					else
					{
						LPWFSRESULT lpWFSResult;
						// allocate WFSResult buffer
						if (WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (LPVOID*)&lpWFSResult) != WFS_SUCCESS)
						{
							return WFS_ERR_INTERNAL_ERROR;
						}
						lpWFSResult->hResult = WFS_SERVICE_EVENT;
						lpWFSResult->hService = (*it).second.hService;
						lpWFSResult->RequestID = NULL;
						lpWFSResult->u.dwEventID = WFS_USRE_PTR_LAMPTHRESHOLD;
						lpWFSResult->lpBuffer = NULL;

						WFMAllocateMore(sizeof(DWORD), lpWFSResult, &lpWFSResult->lpBuffer);
						LPWORD lpwLampThreshold = (LPWORD)lpWFSResult->lpBuffer;
						*lpwLampThreshold = WFS_PTR_LAMPFADING;

						SendMessage((*it).first, WFS_USER_EVENT, 0, (LPARAM)lpWFSResult);
					}

				}
			}
		}
		i++;
		Sleep(10000L);
	}
	return 0;
}

DWORD WINAPI WFPOpenProcess(LPVOID lpParam)
{
	LPWFSRESULT lpWfsResult = (LPWFSRESULT)(lpParam);
	HWND hWindowReturn = (HWND)(lpWfsResult);

	SendMessage(hWindowReturn, WFS_OPEN_COMPLETE, 0, (LPARAM)lpParam);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    NAME OF THE METHOD: WFPOpen
//
//    OBJECTIVE:
//			Establishes a connection between the XFS Manager and the service provider 
//			that supports the specified service, and initiates a session.
//
//    ENTRANCE ARGUMENTS:
//			HSERVICE hService			: The service handle to be associated with the 
//										  session being opened.
//			LPSTR lpszLogicalName		: Points to a null-terminated string containing 
//										  the pre-defined logical name of a service.
//			HAPP hApp					: The application handle to be associated with 
//										  the session being opened
//			LPSTR lpszAppID				: Pointer to a null terminated string containing 
//										  the application ID; the pointer may be NULL if 
//										  the ID is not used.
//			DWORD dwTraceLevel			: See WFPSetTraceLevel.
//			DWORD dwTimeOut				: Number of milliseconds to wait for completion.
//			HWND hWnd					: The window handle which is to receive the 
//										  completion message for this request.
//			REQUESTID requestID			: Request identification number.
//			HPROVIDER hProvider			: Service provider handle supplied by the XFS 
//										  Manager – used by the service provider to 
//										  identify itself when calling the WFMReleaseDLL 
//										  function.
//			DWORD dwSPIVersionsRequired	: Specifies the range of XFS SPI versions that 
//										  the XFS Manager can support.
//			LPWFSVERSION lpSPIVersion	: Pointer to the data structure that is to receive 
//										  SPI version support information and (optionally) 
//										  other details about the SPI implementation 
//										  (returned parameter).
//			DWORD dwSrvcVersionsRequired: Service-specific interface versions required
//			LPWFSVERSION lpSrvcVersion	: Pointer to the service-specific interface 
//										  implementation information;
//
//    EXIT ARGUMENTS:
//        none.
//	 
//	 RETURNS:
//		HRESULT : WFS_OPEN_COMPLETE, WFS_ERR_CONNECTION_LOST, WFS_ERR_INTERNAL_ERROR, 
//				  WFS_ERR_INVALID_HSERVICE, WFS_ERR_INVALID_HWND, WFS_ERR_INVALID_POINTER, 
//				  WFS_ERR_INVALID_TRACELEVEL, WFS_ERR_SPI_VER_TOO_HIGH, 
//				  WFS_ERR_SPI_VER_TOO_LOW, WFS_ERR_SRVC_VER_TOO_HIGH, 
//				  WFS_ERR_SRVC_VER_TOO_LOW, WFS_ERR_VERSION_ERROR_IN_SRVC, 
//				  WFS_ERR_CANCELED, WFS_ERR_INTERNAL_ERROR, WFS_ERR_TIMEOUT, 
//				  WFS_ERR_VERSION_ERROR_IN_SRVC.
//
//    COMMENTS:
//		
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OU PODEMOS DEFINIR como __declspec (dllexport)
HRESULT WINAPI WFPOpen(HSERVICE hService, LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel, DWORD dwTimeOut, HWND hWnd, REQUESTID reqId, HPROVIDER hProvider, DWORD dwSPIVersionsRequired, LPWFSVERSION lpSPIVersion, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion)
{
	// check hWnd
	WINDOWINFO callWindow;
	callWindow.cbSize = sizeof(WINDOWINFO);
	if (hWnd == NULL || (!GetWindowInfo(hWnd, &callWindow) && (GetLastError() == ERROR_INVALID_HANDLE)))
	{
		return WFS_ERR_INVALID_HWND;;
	}

	// check pointer
	if (lpSPIVersion == NULL || lpSrvcVersion == NULL)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	// check version XFS application send

	// save hService and hProvider
	g_hProvider = hProvider;
	g_h_services[hService] = true;

	// allocate WFSResult buffer
	LPWFSRESULT lpWFSResult;
	if (WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_SHARE, (LPVOID*)&lpWFSResult) != WFS_SUCCESS)
	{
		return WFS_ERR_INTERNAL_ERROR;
	}

	lpWFSResult->RequestID = reqId;
	lpWFSResult->hService = hService;
	lpWFSResult->lpBuffer = (LPVOID)(hWnd);
	lpWFSResult->hResult = WFS_SUCCESS;

	HANDLE hOpenThread;
	DWORD dwThreadId;
	hOpenThread = CreateThread(NULL, 0, WFPOpenProcess, lpWFSResult, 0, &dwThreadId);
	return WFS_SUCCESS;
}

DWORD WINAPI WFPCloseProcess(LPVOID lpParam)
{
	LPWFSRESULT lpWfsResult = (LPWFSRESULT)(lpParam);
	HWND hWindowReturn = (HWND)(lpWfsResult);

	SendMessage(hWindowReturn, WFS_CLOSE_COMPLETE, 0, (LPARAM)lpParam);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    NAME OF THE METHOD: WFPClose
//
//    OBJECTIVE: 
//			Terminates a session between the XFS Manager and the specified service provider.
//
//    ENTRANCE ARGUMENTS:
//			HSERVICE hService	:	Handle to the service provider.
//			HWND hWnd			:	The window handle which is to receive the completion 
//									message for this request.
//			REQUESTID requestID		:	Request identification number.
//
//    EXIT ARGUMENTS:
//        none.
//	 
//	 RETURNS:
//		HRESULT : WFS_ERR_CONNECTION_LOST, WFS_ERR_INTERNAL_ERROR, WFS_ERR_INVALID_HSERVICE, 
//				  WFS_ERR_INVALID_HWND, WFS_ERR_CANCELED, WFS_ERR_INTERNAL_ERROR
//
//    COMMENTS:
//		
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI WFPClose(HSERVICE hService, HWND hWnd, REQUESTID reqId)
{
	// check hService
	if (hService == NULL || g_h_services.find(hService) == g_h_services.end())
	{
		return WFS_ERR_INVALID_HSERVICE;
	}

	// Check if hService has pending commands to be executed and wait them


	// check if LOCK and UNLOCK provider
	WaitForSingleObject(g_lock_mutex, INFINITE);
	g_lock_state = UNLOCKED;
	g_lock_hservice = NULL;
	ReleaseMutex(g_lock_mutex);

	// remove hService from list
	g_h_services.erase(hService);

	// allocate WFSResult buffer
	LPWFSRESULT lpWFSResult;
	if (WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_SHARE, (LPVOID*)&lpWFSResult) != WFS_SUCCESS)
	{
		return WFS_ERR_INTERNAL_ERROR;
	}

	lpWFSResult->RequestID = reqId;
	lpWFSResult->hService = hService;
	lpWFSResult->lpBuffer = (LPVOID)(hWnd);
	lpWFSResult->hResult = WFS_SUCCESS;

	HANDLE hCloseThread;
	DWORD dwThreadId;
	hCloseThread = CreateThread(NULL, 0, WFPCloseProcess, lpWFSResult, 0, &dwThreadId);

	return WFS_SUCCESS;
}


DWORD WINAPI WFPLockProcess(LPVOID lpParam)
{
	LPWFSRESULT lpWfsResult = (LPWFSRESULT)(lpParam);
	HWND hWindowReturn = (HWND)(lpWfsResult);

	WaitForSingleObject(g_lock_mutex, INFINITE);

	g_lock_state = LOCKED;
	g_lock_hservice = lpWfsResult->hService;

	ReleaseMutex(g_lock_mutex);

	SendMessage(hWindowReturn, WFS_LOCK_COMPLETE, 0, (LPARAM)lpParam);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    NAME OF THE METHOD: WFPLock
//
//    OBJECTIVE: 
//         Establishes exclusive control by the calling application over the specified 
//		   service.
//
//    ENTRANCE ARGUMENTS:
//			HSERVICE hService		: Handle to the service provider.
//			DWORD dwTimeOut			: Number of milliseconds to wait for completion 
//									  (WFS_INDEFINITE_WAIT to specify a request that 
//									  will wait until completion).
//			HWND hWnd				: The window handle.
//			REQUESTID requestID		: Request identification number.
//
//    EXIT ARGUMENTS:
//        none.
//	 
//	 RETURNS:
//		HRESULT : WFS_OPEN_COMPLETE, WFS_ERR_CONNECTION_LOST, WFS_ERR_INTERNAL_ERROR, 
//				  WFS_ERR_INVALID_HSERVICE, WFS_ERR_INVALID_HWND, WFS_ERR_CANCELED, 
//				  WFS_ERR_DEV_NOT_READY, WFS_ERR_HARDWARE_ERROR, WFS_ERR_INTERNAL_ERROR, 
//				  WFS_ERR_SOFTWARE_ERROR, WFS_ERR_TIMEOUT.
//
//    COMMENTS:
//		
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI WFPLock(HSERVICE hService, DWORD dwTimeOut, HWND hWnd, REQUESTID reqId)
{
	// check hService
	if (hService == NULL || g_h_services.find(hService) == g_h_services.end())
	{
		return WFS_ERR_INVALID_HSERVICE;
	}

	// check if device is already locked state
	WaitForSingleObject(g_lock_mutex, INFINITE);
	
	if (g_lock_state == LOCKED || g_lock_state == LOCK_PENDING)
	{
		ReleaseMutex(g_lock_mutex);
		return WFS_ERR_LOCKED;
	}

	g_lock_state = LOCK_PENDING;

	ReleaseMutex(g_lock_mutex);

	// allocate WFSResult buffer
	LPWFSRESULT lpWFSResult;
	if (WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_SHARE, (LPVOID*)&lpWFSResult) != WFS_SUCCESS)
	{
		return WFS_ERR_INTERNAL_ERROR;
	}

	lpWFSResult->RequestID = reqId;
	lpWFSResult->hService = hService;
	lpWFSResult->lpBuffer = (LPVOID)(hWnd);
	lpWFSResult->hResult = WFS_SUCCESS;

	HANDLE hCloseThread;
	DWORD dwThreadId;
	hCloseThread = CreateThread(NULL, 0, WFPLockProcess, lpWFSResult, 0, &dwThreadId);

	return WFS_SUCCESS;
}


DWORD WINAPI WFPUnLockProcess(LPVOID lpParam)
{
	LPWFSRESULT lpWfsResult = (LPWFSRESULT)(lpParam);
	HWND hWindowReturn = (HWND)(lpWfsResult);

	WaitForSingleObject(g_lock_mutex, INFINITE);
	g_lock_state = UNLOCKED;
	g_lock_hservice = NULL;
	ReleaseMutex(g_lock_mutex);

	SendMessage(hWindowReturn, WFS_UNLOCK_COMPLETE, 0, (LPARAM)lpParam);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    NAME OF THE METHOD: WFPUnlock
//
//    OBJECTIVE: 
//         Releases a service that has been locked by a previous WFPLock function.
//
//    ENTRANCE ARGUMENTS:
//			HSERVICE hService		: Handle to the service provider
//			HWND hWnd				: The window handle which is to receive the completion 
//									  message for this request.
//			REQUESTID requestID		: Request identification number.
//
//    EXIT ARGUMENTS:
//        none.
//	 
//	 RETURNS:
//		HRESULT : WFS_UNLOCK_COMPLETE, WFS_ERR_CONNECTION_LOST, WFS_ERR_INTERNAL_ERROR, 
//				  WFS_ERR_INVALID_HSERVICE, WFS_ERR_INVALID_HWND, WFS_ERR_CANCELED, 
//				  WFS_ERR_INTERNAL_ERROR, WFS_ERR_NOT_LOCKED.
//
//    COMMENTS:
//		
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI WFPUnlock(HSERVICE hService, HWND hWnd, REQUESTID reqId)
{
	// check hService
	if (hService == NULL || g_h_services.find(hService) == g_h_services.end())
	{
		return WFS_ERR_INVALID_HSERVICE;
	}

	WaitForSingleObject(g_lock_mutex, INFINITE);
	if (g_lock_state == UNLOCKED)
	{
		ReleaseMutex(g_lock_mutex);
		return WFS_ERR_LOCKED;
	}
	ReleaseMutex(g_lock_mutex);

	// allocate WFSResult buffer
	LPWFSRESULT lpWFSResult;
	if (WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_SHARE, (LPVOID*)&lpWFSResult) != WFS_SUCCESS)
	{
		return WFS_ERR_INTERNAL_ERROR;
	}

	lpWFSResult->RequestID = reqId;
	lpWFSResult->hService = hService;
	lpWFSResult->lpBuffer = (LPVOID)(hWnd);
	lpWFSResult->hResult = WFS_SUCCESS;

	HANDLE hCloseThread;
	DWORD dwThreadId;
	hCloseThread = CreateThread(NULL, 0, WFPUnLockProcess, lpWFSResult, 0, &dwThreadId);

	return WFS_SUCCESS;
}

DWORD WINAPI WFPRegisterProcess(LPVOID lpParam)
{
	LPWFSRESULT lpWfsResult = (LPWFSRESULT)(lpParam);
	HWND hWindowReturn = (HWND)(lpWfsResult->lpBuffer);
	lpWfsResult->lpBuffer = NULL;

	SendMessage(hWindowReturn, WFS_REGISTER_COMPLETE, 0, (LPARAM)lpWfsResult);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    NAME OF THE METHOD: WFPRegister
//
//    OBJECTIVE:
//			Enables event monitoring for the specified service by the specified hWndReg.
//
//    ENTRANCE ARGUMENTS:
//			HSERVICE hService		: Handle to the service provider as returned by 
//									  WFSOpen or WFSAsyncOpen.
//			DORD dwEventClass		: The class(es) of events for which the application 
//									  is registering.
//			HWND hWndReg			: The window handle which is to be registered to 
//									  receive the specified messages.
//			HWND hWnd				: The window handle which is to receive the completion 
//									  message for this request.
//			REQUESTID requestID		: The request identifier for request to be canceled, 
//									  as returned by the original function call.
//
//    EXIT ARGUMENTS:
//        none.
//	 
//	 RETURNS:
//		HRESULT : WFS_SUCCESS, WFS_ERR_CANCELED, WFS_ERR_CONNECTION_LOST, 
//				  WFS_ERR_INTERNAL_ERROR, WFS_ERR_INVALID_EVENT_CLASS, 
//				  WFS_ERR_INVALID_HSERVICE, WFS_ERR_INVALID_HWNDREG, WFS_ERR_NOT_STARTED.
//
//    COMMENTS: 
//		
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI WFPRegister(HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, REQUESTID reqId)
{
	if (hService == NULL || g_h_services.find(hService) == g_h_services.end())
		return WFS_ERR_INVALID_HSERVICE;

	if ((dwEventClass & SERVICE_EVENTS) != SERVICE_EVENTS
		&& (dwEventClass & USER_EVENTS) != USER_EVENTS
		&& (dwEventClass & SYSTEM_EVENTS) != SYSTEM_EVENTS
		&& (dwEventClass & EXECUTE_EVENTS) != EXECUTE_EVENTS)
		return WFS_ERR_USER_ERROR;



	if (g_wfs_event.find(hWndReg) != g_wfs_event.end())
	{
		// enable event bits
		g_wfs_event[hWndReg].dwEvent |= dwEventClass;
	}
	else
	{
		WFS_EVENTS wfsEvent;
		wfsEvent.hService = hService;
		wfsEvent.dwEvent = dwEventClass;
		g_wfs_event[hWndReg] = wfsEvent;
	}

	// allocate WFSResult buffer
	LPWFSRESULT lpWFSResult;
	if (WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_SHARE, (LPVOID*)&lpWFSResult) != WFS_SUCCESS)
	{
		return WFS_ERR_INTERNAL_ERROR;
	}

	lpWFSResult->RequestID = reqId;
	lpWFSResult->hService = hService;
	lpWFSResult->lpBuffer = hWnd;
	lpWFSResult->u.dwCommandCode = dwEventClass;

	HANDLE hGetInfoThread;
	DWORD dwThreadId;
	hGetInfoThread = CreateThread(NULL, 0, WFPRegisterProcess, lpWFSResult, 0, &dwThreadId);

	return WFS_SUCCESS;
}


DWORD WINAPI WFPDeRegisterProcess(LPVOID lpParam)
{
	LPWFSRESULT lpWfsResult = (LPWFSRESULT)(lpParam);
	HWND hWindowReturn = (HWND)(lpWfsResult->lpBuffer);
	lpWfsResult->lpBuffer = NULL;

	SendMessage(hWindowReturn, WFS_DEREGISTER_COMPLETE, 0, (LPARAM)lpWfsResult);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    NAME OF THE METHOD: WFPDeregister
//
//    OBJECTIVE: 
//         Discontinues monitoring of the specified message class(es) from the specified 
//		   service provider, by the specified hWndReg (or all hWnd's).
//
//    ENTRANCE ARGUMENTS:
//			HSERVICE hService	: Handle to the service provider
//			DWORD dwEventClass	: The class(es) of messages from which the application is 
//								  deregistering.
//			HWND hWndReg		: The window to which notification messages are posted
//			HWND hWnd			: The window handle which is to receive the completion 
//								  message for this request.
//			REQUESTID requestID		: Request identification number.
//
//    EXIT ARGUMENTS:
//			none.
//	 
//	 RETURNS:
//			HRESULT : WFS_DEREGISTER_COMPLETE, WFS_ERR_CONNECTION_LOST, 
//					  WFS_ERR_INTERNAL_ERROR, WFS_ERR_INVALID_EVENT_CLASS, 
//					  WFS_ERR_INVALID_HSERVICE, WFS_ERR_INVALID_HWND, 
//					  WFS_ERR_INVALID_HWNDREG, WFS_ERR_NOT_REGISTERED, FS_ERR_CANCELED
//
//    COMMENTS:
//		
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI WFPDeregister(HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, REQUESTID reqId)
{
	if (hWndReg == NULL) {
		g_wfs_event.clear();
	}
	else if (g_wfs_event.find(hWndReg) != g_wfs_event.end())
	{
		// enable event bits
		g_wfs_event[hWndReg].dwEvent &= (~dwEventClass);

		if (g_wfs_event[hWndReg].dwEvent == 0)
			g_wfs_event.erase(hWndReg);
	}
	else
	{
		return WFS_ERR_INVALID_HWNDREG;
	}

	// allocate WFSResult buffer
	LPWFSRESULT lpWFSResult;
	if (WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_SHARE, (LPVOID*)&lpWFSResult) != WFS_SUCCESS)
	{
		return WFS_ERR_INTERNAL_ERROR;
	}

	lpWFSResult->RequestID = reqId;
	lpWFSResult->hService = hService;
	lpWFSResult->lpBuffer = hWnd;
	lpWFSResult->u.dwCommandCode = dwEventClass;

	HANDLE hGetInfoThread;
	DWORD dwThreadId;
	hGetInfoThread = CreateThread(NULL, 0, WFPDeRegisterProcess, lpWFSResult, 0, &dwThreadId);
	return WFS_SUCCESS;
}


void ProcessGetInfoStatus(LPWFSRESULT wfs_result)
{
	HRESULT res = WFMAllocateMore(sizeof(WFSPTRSTATUS), wfs_result, &wfs_result->lpBuffer);
	if (res != WFS_SUCCESS)
	{
		wfs_result->hResult = WFS_ERR_INTERNAL_ERROR;
	}
	else
	{
		LPWFSPTRSTATUS lpStatus = (LPWFSPTRSTATUS)wfs_result->lpBuffer;

		// init data
		lpStatus->fwDevice = WFS_PTR_DEVPOWEROFF;
		lpStatus->fwMedia = WFS_PTR_MEDIAUNKNOWN;
		lpStatus->fwPaper[WFS_PTR_SUPPLYUPPER] = WFS_PTR_PAPERUNKNOWN;
		lpStatus->fwPaper[WFS_PTR_SUPPLYLOWER] = WFS_PTR_PAPERNOTSUPP;
		lpStatus->fwPaper[WFS_PTR_SUPPLYEXTERNAL] = WFS_PTR_PAPERNOTSUPP;
		lpStatus->fwPaper[WFS_PTR_SUPPLYAUX] = WFS_PTR_PAPERNOTSUPP;
		lpStatus->fwPaper[WFS_PTR_SUPPLYAUX2] = WFS_PTR_PAPERNOTSUPP;
		lpStatus->fwPaper[WFS_PTR_SUPPLYPARK] = WFS_PTR_PAPERNOTSUPP;
		lpStatus->fwToner = WFS_PTR_TONERUNKNOWN;
		lpStatus->fwInk = WFS_PTR_INKNOTSUPP;
		lpStatus->fwLamp = WFS_PTR_LAMPNOTSUPP;
		lpStatus->lppRetractBins = NULL;
		lpStatus->usMediaOnStacker = 0;
		lpStatus->lpszExtra = NULL;
		std::fill_n(lpStatus->dwGuidLights, WFS_PTR_GUIDLIGHTS_SIZE, WFS_PTR_GUIDANCE_NOT_AVAILABLE);
		lpStatus->wDevicePosition = WFS_PTR_DEVICEPOSNOTSUPP;
		lpStatus->usPowerSaveRecoveryTime = 0;
		std::fill_n(lpStatus->wPaperType, WFS_PTR_SUPPLYSIZE, WFS_PTR_PAPERTYPEUNKNOWN);
		lpStatus->wPaperType[WFS_PTR_SUPPLYUPPER] = WFS_PTR_PAPERSINGLESIDED;
		lpStatus->wAntiFraudModule = WFS_PTR_AFMNOTSUPP;
		lpStatus->wBlackMarkMode = WFS_PTR_BLACKMARKDETECTIONNOTSUPP;

		// get information from physical device

		// fill the status on the structure
		lpStatus->fwDevice = WFS_PTR_DEVONLINE;
		lpStatus->fwPaper[WFS_PTR_SUPPLYUPPER] = WFS_PTR_PAPERFULL;
	}
}

void ProcessGetInfoCapabilities(LPWFSRESULT wfs_result)
{
	HRESULT res = WFMAllocateMore(sizeof(WFSPTRCAPS), wfs_result, &wfs_result->lpBuffer);
	if (res != WFS_SUCCESS)
	{
		wfs_result->hResult = WFS_ERR_INTERNAL_ERROR;
	}
	else
	{
		LPWFSPTRCAPS lpCaps = (LPWFSPTRCAPS)wfs_result->lpBuffer;

		lpCaps->wClass = WFS_SERVICE_CLASS_PTR;
		lpCaps->fwType = WFS_PTR_TYPERECEIPT;
		lpCaps->bCompound = FALSE;
		lpCaps->wResolution = WFS_PTR_RESMED;
		lpCaps->fwReadForm = 0;
		lpCaps->fwWriteForm = 0;
		lpCaps->fwExtents = 0;
		lpCaps->fwControl = 0;
		lpCaps->usMaxMediaOnStacker = 0;
		lpCaps->bAcceptMedia = FALSE;
		lpCaps->bMultiPage = FALSE;
		lpCaps->fwPaperSources = WFS_PTR_PAPERUPPER;
		lpCaps->bMediaTaken = FALSE;
		lpCaps->usRetractBins = 0;
		lpCaps->lpusMaxRetract = NULL;
		lpCaps->fwImageType = 0;
		lpCaps->fwFrontImageColorFormat = 0;
		lpCaps->fwBackImageColorFormat = 0;
		lpCaps->fwCodelineFormat = 0;
		lpCaps->fwImageSource = 0;
		lpCaps->fwCharSupport = WFS_PTR_ASCII | WFS_PTR_UNICODE;
		lpCaps->bDispensePaper = TRUE;
		lpCaps->lpszExtra = NULL;
		std::fill_n(lpCaps->dwGuidLights, WFS_PTR_GUIDLIGHTS_SIZE, WFS_PTR_GUIDANCE_NOT_AVAILABLE);
		lpCaps->lpszWindowsPrinter = NULL;
		lpCaps->bMediaPresented = FALSE;
		lpCaps->usAutoRetractPeriod = 0;
		lpCaps->bRetractToTransport = FALSE;
		lpCaps->bPowerSaveControl = FALSE;
		lpCaps->fwCoercivityType = WFS_PTR_COERCIVITYNOTSUPP;
		lpCaps->fwControlPassbook = WFS_PTR_PBKCTRLNOTSUPP;
		lpCaps->wPrintSides = WFS_PTR_PRINTSIDESSINGLE;
		lpCaps->bAntiFraudModule = FALSE;
		lpCaps->dwControlEx = WFS_PTR_CTRLEJECT | WFS_PTR_CTRLCUT;
		lpCaps->bBlackMarkModeSupported = FALSE;
		lpCaps->lpdwSynchronizableCommands = NULL;
	}
}

DWORD WINAPI WFPGetInfoProcess(LPVOID lpParam)
{
	LPWFSRESULT lpWfsResult = (LPWFSRESULT)(lpParam);
	HWND hWindowReturn = (HWND)(lpWfsResult->lpBuffer);

	if (lpWfsResult->u.dwCommandCode == WFS_INF_PTR_CAPABILITIES)
	{
		ProcessGetInfoCapabilities(lpWfsResult);
		lpWfsResult->hResult = WFS_SUCCESS;
	}
	else if (lpWfsResult->u.dwCommandCode == WFS_INF_PTR_STATUS)
	{
		ProcessGetInfoStatus(lpWfsResult);
		lpWfsResult->hResult = WFS_SUCCESS;
	}

	SendMessage(hWindowReturn, WFS_GETINFO_COMPLETE, 0, (LPARAM)lpParam);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    NAME OF THE METHOD: WFPGetInfo
//
//    OBJECTIVE: 
//         Retrieves several types of information from the specified service provider.
//
//    ENTRANCE ARGUMENTS:
//			HSERVICE hService		: Handle to the service provider.
//			DWORD dwCategory		: Specifies the category of the query.
//			LPVOID lpQueryDetails	: Pointer to the data structure to be passed to the 
//									  service provider, containing further details to 
//									  make the query more precise, e.g.,  a form name.
//			DWORD dwTimeOut			: Number of milliseconds to wait for completion.
//			HWND hWnd				: The window handle which is to receive the completion 
//									  message for this request.
//			REQUESTID requestID		: Request identification number.
//
//    EXIT ARGUMENTS:
//        none.
//	 
//	 RETURNS:
//		HRESULT : WFS_GETINFO_COMPLETE, WFS_ERR_CONNECTION_LOST, WFS_ERR_INTERNAL_ERROR, 
//				  WFS_ERR_INVALID_CATEGORY, WFS_ERR_INVALID_HSERVICE, 
//				  WFS_ERR_INVALID_HWND, WFS_ERR_INVALID_POINTER, WFS_ERR_UNSUPP_CATEGORY, 
//				  WFS_ERR_CANCELED, WFS_ERR_DEV_NOT_READY, WFS_ERR_HARDWARE_ERROR, 
//				  WFS_ERR_INTERNAL_ERROR, WFS_ERR_INVALID_DATA, WFS_ERR_SOFTWARE_ERROR, 
//				  WFS_ERR_TIMEOUT, WFS_ERR_USER_ERROR, WFS_ERR_UNSUPP_DATA.
//
//    COMMENTS:
//		
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI WFPGetInfo(HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, HWND hWnd, REQUESTID reqId)
{
	// check hService
	if (hService == NULL || g_h_services.find(hService) == g_h_services.end())
	{
		return WFS_ERR_INVALID_HSERVICE;
	}

	// allocate WFSResult buffer
	LPWFSRESULT lpWFSResult;
	if (WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_SHARE | WFS_MEM_ZEROINIT, (LPVOID*)&lpWFSResult) != WFS_SUCCESS)
	{
		return WFS_ERR_INTERNAL_ERROR;
	}

	lpWFSResult->RequestID = reqId;
	lpWFSResult->hService = hService;
	lpWFSResult->lpBuffer = (LPVOID)(hWnd);
	lpWFSResult->u.dwCommandCode = dwCategory;


	HANDLE hCloseThread;
	DWORD dwThreadId;
	hCloseThread = CreateThread(NULL, 0, WFPGetInfoProcess, lpWFSResult, 0, &dwThreadId);

	return WFS_SUCCESS;
}


void WFPExecutePrint(LPWFSRESULT wfs_result, LPVOID data)
{
	LPWFSPTRRAWDATA lpData = (LPWFSPTRRAWDATA)data;

	wfs_result->hResult = WFS_SUCCESS;
	wfs_result->lpBuffer = NULL;

	if (lpData->lpbData != NULL)
	{
		free(lpData->lpbData);
	}

	if (lpData != NULL)
	{
		free(lpData);
	}

}

void WFPExecuteControlMediaCommand(LPWFSRESULT wfs_result, LPVOID data)
{
	LPWORD lpMediaControl = (LPWORD)data;

	wfs_result->hResult = WFS_SUCCESS;
	wfs_result->lpBuffer = NULL;

	if (lpMediaControl != NULL)
	{
		free(lpMediaControl);
	}
}

DWORD WINAPI WFPExecuteProcess(LPVOID lpParam)
{
	WFS_MSG *msg = (WFS_MSG*)(lpParam);
	LPWFSRESULT lpWfsResult = msg->lpWFSResult;
	HWND hWindowReturn = (HWND)msg->hWnd;

	if (lpWfsResult->u.dwCommandCode == WFS_CMD_PTR_RAW_DATA)
	{
		WFPExecutePrint(lpWfsResult, msg->lpDataReceived);
	}
	else if (lpWfsResult->u.dwCommandCode == WFS_CMD_PTR_CONTROL_MEDIA)
	{
		WFPExecuteControlMediaCommand(lpWfsResult, msg->lpDataReceived);
	}
	free(msg);

	SendMessage(hWindowReturn, WFS_EXECUTE_COMPLETE, 0, (LPARAM)lpWfsResult);
	return 0;
}

DWORD WINAPI WFPExecuteThread(LPVOID lpParam)
{
	while (TRUE)
	{
		// read data from queue
		WaitForSingleObject(g_wfs_queue_mutex, INFINITE);
		// check if queue is not empty
		if (!g_wfs_msg_queue.empty())
		{
			// remove command from queue
			WFS_MSG* msg = g_wfs_msg_queue.front();
			LPWFSRESULT lpWfsResult = msg->lpWFSResult;
			HWND hWindowReturn = (HWND)msg->hWnd;
			g_wfs_msg_queue.pop_front();
			ReleaseMutex(g_wfs_queue_mutex);

			if (msg->bCancelled == TRUE)
			{
				// command is cancelled
				msg->lpWFSResult->hResult = WFS_ERR_CANCELED;
			}
			else
			{
				if (lpWfsResult->u.dwCommandCode == WFS_CMD_PTR_RAW_DATA)
				{
					WFPExecutePrint(lpWfsResult, msg->lpDataReceived);
				}
				else if (lpWfsResult->u.dwCommandCode == WFS_CMD_PTR_CONTROL_MEDIA)
				{
					WFPExecuteControlMediaCommand(lpWfsResult, msg->lpDataReceived);
				}
			}
			//post message to XFS Manager
			SendMessage(hWindowReturn, WFS_EXECUTE_COMPLETE, NULL, (LPARAM)lpWfsResult);

			free(msg);
		}
		else
		{
			ReleaseMutex(g_wfs_queue_mutex);
		}
		Sleep(10000L);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    NAME OF THE METHOD: WFPExecute
//
//    OBJECTIVE:
//		 Sends asynchronous service class specific commands to a service provider.
//
//    ENTRANCE ARGUMENTS:
//			HSERVICE hService		: Handle to the service provider.
//			DWORD dwCommand			: Command to be executed.
//			LPVOID lpCmdData		: Pointer to the data structure to be passed.
//			DWORD dwTimeOut			: Number of milliseconds to wait for completion.
//			HWND hWnd				: The window handle which is to receive the completion 
//									  message for this request.
//			REQUESTID requestID		: Request identification number.
//
//    EXIT ARGUMENTS:
//        none.
//	 
//	 RETURNS:
//		HRESULT : WFS_EXECUTE_COMPLETE, WFS_ERR_CONNECTION_LOST, WFS_ERR_INTERNAL_ERROR, 
//				  WFS_ERR_INVALID_COMMAND, WFS_ERR_INVALID_HSERVICE, WFS_ERR_INVALID_HWND,
//				  WFS_ERR_INVALID_POINTER, WFS_ERR_UNSUPP_COMMAND, WFS_ERR_CANCELED, 
//				  WFS_ERR_DEV_NOT_READY, WFS_ERR_HARDWARE_ERROR, WFS_ERR_INVALID_DATA, 
//				  WFS_ERR_INTERNAL_ERROR, WFS_ERR_LOCKED, WFS_ERR_SOFTWARE_ERROR, 
//				  WFS_ERR_TIMEOUT, WFS_ERR_USER_ERROR, WFS_ERR_UNSUPP_DATA
//
//    COMMENTS:
//		
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI WFPExecute(HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, HWND hWnd, REQUESTID reqId)
{
	// check hService
	if (hService == NULL || g_h_services.find(hService) == g_h_services.end())
	{
		return WFS_ERR_INVALID_HSERVICE;
	}

	// check lock state
	WaitForSingleObject(g_lock_mutex, INFINITE);
	if (g_lock_state == LOCKED && g_lock_hservice != hService)
	{
		ReleaseMutex(g_lock_mutex);
		return WFS_ERR_LOCKED;
	}
	ReleaseMutex(g_lock_mutex);

	// check supported commands
	if (dwCommand == WFS_CMD_PTR_PRINT_FORM
		|| dwCommand == WFS_CMD_PTR_READ_FORM
		|| dwCommand == WFS_CMD_PTR_MEDIA_EXTENTS
		|| dwCommand == WFS_CMD_PTR_RESET_COUNT
		|| dwCommand == WFS_CMD_PTR_READ_IMAGE
		|| dwCommand == WFS_CMD_PTR_RETRACT_MEDIA
		|| dwCommand == WFS_CMD_PTR_SET_GUIDANCE_LIGHT
		|| dwCommand == WFS_CMD_PTR_PRINT_RAW_FILE
		|| dwCommand == WFS_CMD_PTR_LOAD_DEFINITION
		|| dwCommand == WFS_CMD_PTR_SUPPLY_REPLENISH
		|| dwCommand == WFS_CMD_PTR_POWER_SAVE_CONTROL
		|| dwCommand == WFS_CMD_PTR_CONTROL_PASSBOOK
		|| dwCommand == WFS_CMD_PTR_SET_BLACK_MARK_MODE
		|| dwCommand == WFS_CMD_PTR_SYNCHRONIZE_COMMAND)
	{
		return WFS_ERR_UNSUPP_COMMAND;
	}

	// allocate WFSResult buffer
	LPWFSRESULT lpWFSResult;
	if (WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_SHARE, (LPVOID*)&lpWFSResult) != WFS_SUCCESS)
	{
		return WFS_ERR_INTERNAL_ERROR;
	}

	lpWFSResult->RequestID = reqId;
	lpWFSResult->hService = hService;
	lpWFSResult->u.dwCommandCode = dwCommand;


	WFS_MSG* msgData = (WFS_MSG*)malloc(sizeof(WFS_MSG));
	   
	msgData->lpWFSResult = lpWFSResult;
	msgData->hWnd = hWnd;
	msgData->bCancelled = false;

	if (dwCommand == WFS_CMD_PTR_RAW_DATA)
	{
		LPWFSPTRRAWDATA tmpCmdData = NULL;
		LPBYTE tmpData = NULL;

		// Allocate data for command
		tmpCmdData = (LPWFSPTRRAWDATA)malloc(sizeof(WFSPTRRAWDATA));
		if (tmpCmdData != NULL)
		{
			memcpy(tmpCmdData, lpCmdData, sizeof(WFSPTRRAWDATA));
			if (tmpCmdData->ulSize > 0)
			{
				tmpData = (LPBYTE)malloc(tmpCmdData->ulSize);
				memcpy(tmpData, ((LPWFSPTRRAWDATA)lpCmdData)->lpbData, tmpCmdData->ulSize);

				((LPWFSPTRRAWDATA)tmpCmdData)->lpbData = tmpData;
			}
		}
		msgData->lpDataReceived = tmpCmdData;
	}
	else if (dwCommand == WFS_CMD_PTR_CONTROL_MEDIA)
	{
		LPWORD tmpCmdData = (LPWORD)malloc(sizeof(WORD));
		*tmpCmdData = *((LPWORD)lpCmdData);
		msgData->lpDataReceived = (LPVOID)tmpCmdData;
	}

	DWORD dwThreadId;
	//hExecuteThread = CreateThread(NULL, 0, WFPExecuteProcess, msgData, 0, &dwThreadId);
	if (hExecuteThread == NULL) 
	{
		hExecuteThread = CreateThread(NULL, 0, WFPExecuteThread, 0, 0, &dwThreadId);
	}

	// Include message on dispatch queue
	WaitForSingleObject(g_wfs_queue_mutex, INFINITE);
	g_wfs_msg_queue.push_back(msgData);
	ReleaseMutex(g_wfs_queue_mutex);

	return WFS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    NAME OF THE METHOD: WFPCancelAsyncRequest
//
//    OBJECTIVE: 
//		  Cancel an outstanding asynchronous request.
//
//    ENTRANCE ARGUMENTS:
//        HSERVICE hService	: Handle to the service as returned by WFSOpen or 
//							  WFSAsyncOpen.
//		  REQUESTID requestID : The request identifier for request to be canceled, 
//								as returned by the original function call.
//
//    EXIT ARGUMENTS:
//        none.
//	 
//	 RETURNS:
//		HRESULT : WFS_ERR_CONNECTION_LOST, WFS_ERR_INTERNAL_ERROR, 
//				  WFS_ERR_INVALID_HSERVICE, WFS_ERR_INVALID_REQ_ID.
//
//    COMMENTS:
//		
//
//////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI WFPCancelAsyncRequest(HSERVICE hService, REQUESTID reqId)
{
	// check hService
	if (hService == NULL || g_h_services.find(hService) == g_h_services.end())
	{
		return WFS_ERR_INVALID_HSERVICE;
	}

	WaitForSingleObject(g_wfs_queue_mutex, INFINITE);
	std::deque<WFS_MSG*>::iterator it;
	for (it = g_wfs_msg_queue.begin(); it != g_wfs_msg_queue.end(); ++it)
	{
		if ((*it)->lpWFSResult->hService == hService)
		{
			if ((reqId == NULL) || ((*it)->lpWFSResult->RequestID == reqId))
			{
				(*it)->bCancelled = true;
			}
		}
	}
	ReleaseMutex(g_wfs_queue_mutex);
	return WFS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    NAME OF THE METHOD: WFPSetTraceLevel
//
//    OBJECTIVE:
//			Sets the specified trace level(s) at run time, in and/or below the service 
//			provider.
//         
//
//    ENTRANCE ARGUMENTS:
//			HSERVICE hService		: Handle to the service provider.
//			DWORD dwTraceLevel		: The level(s) of tracing being requested.
//
//    EXIT ARGUMENTS:
//        none.
//	 
//	 RETURNS:
//		HRESULT : WFS_SUCCESS, WFS_ERR_CONNECTION_LOST, WFS_ERR_INTERNAL_ERROR, 
//				  WFS_ERR_INVALID_HSERVICE, WFS_ERR_INVALID_TRACELEVEL, 
//				  WFS_ERR_NOT_STARTED, WFS_ERR_OP_IN_PROGRESS.
//
//    COMMENTS:
//		
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI WFPSetTraceLevel(HSERVICE hService, DWORD dwTraceLevel)
{

	return WFS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    NAME OF THE METHOD: WFPUnloadService
//
//    OBJECTIVE: 
//			Asks the called service provider whether it is OK for the XFS Manager 
//			to unload the service provider’s DLL.
//
//    ENTRANCE ARGUMENTS:
//			none.
//
//    EXIT ARGUMENTS:
//			none.
//	 
//	 RETURNS:
//			HRESULT : WFS_ERR_NOT_OK_TO_UNLOAD.
//
//    COMMENTS:
//		
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI WFPUnloadService()
{
	return WFS_SUCCESS;
}