
// Hardplace AMU-1000.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CHardplaceAMU1000App:
// See Hardplace AMU-1000.cpp for the implementation of this class
//

class CHardplaceAMU1000App : public CWinApp
{
public:
	CHardplaceAMU1000App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CHardplaceAMU1000App theApp;
