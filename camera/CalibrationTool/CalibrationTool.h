// CalibrationTool.h : main header file for the CALIBRATIONTOOL application
//

#if !defined(AFX_CALIBRATIONTOOL_H__03C48FF0_6DE3_453D_A205_E51C26F4BEE5__INCLUDED_)
#define AFX_CALIBRATIONTOOL_H__03C48FF0_6DE3_453D_A205_E51C26F4BEE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCalibrationToolApp:
// See CalibrationTool.cpp for the implementation of this class
//

class CCalibrationToolApp : public CWinApp
{
public:
	CCalibrationToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalibrationToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCalibrationToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBRATIONTOOL_H__03C48FF0_6DE3_453D_A205_E51C26F4BEE5__INCLUDED_)
