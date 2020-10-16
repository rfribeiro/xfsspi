#include "pch.h"
#include <xfsptr.h>
#include <xfsspi.h>


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
	return WFS_SUCCESS;
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

	return WFS_SUCCESS;
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

	return WFS_SUCCESS;
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

	return WFS_SUCCESS;
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

	return WFS_SUCCESS;
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
	return WFS_SUCCESS;
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
	return WFS_SUCCESS;
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