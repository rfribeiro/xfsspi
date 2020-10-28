#pragma once
#include <xfsptr.h>
#include <xfsspi.h>
#include <map>
#include <deque>

// -----------------------------
// services variables
static HPROVIDER g_hProvider = NULL;
static std::map<HSERVICE, bool> g_h_services;

struct WFS_EVENTS {
	DWORD dwEvent;
	HSERVICE hService;
	DWORD evento;
};

static std::map<HWND, WFS_EVENTS> g_wfs_event;

//-------------------------------
// Command queue
struct WFS_MSG {
	HWND hWnd;
	LPWFSRESULT lpWFSResult;
	LPVOID lpDataReceived;
	BOOL bCancelled;
};

static std::deque<WFS_MSG*> g_wfs_msg_queue;
static HANDLE g_wfs_queue_mutex = NULL;
static HANDLE hExecuteThread = NULL;

//--------------------------------
// Serial acess mutex
static HANDLE g_serial_mutex;

//-------------------------------
// Lock variables
#define UNLOCKED 0
#define LOCK_PENDING 1
#define LOCKED 2

static int g_lock_state = UNLOCKED;
static HSERVICE g_lock_hservice = NULL;
static HANDLE g_lock_mutex;

