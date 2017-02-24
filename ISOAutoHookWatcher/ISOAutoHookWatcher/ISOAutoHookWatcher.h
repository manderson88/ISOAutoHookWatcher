// HookWatcher.h : main header file for the HookWatcher DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "Common\Include\WinHttpClient.h"

// CHookWatcherApp
// See HookWatcher.cpp for the implementation of this class
//

class CISOAutoHookWatcherApp : public CWinApp
{
public:
	CISOAutoHookWatcherApp();

// Overrides
public:
	virtual BOOL InitInstance();

	//virtual BOOL CheckRootFolder(long leafFolderID);

	DECLARE_MESSAGE_MAP()
};
