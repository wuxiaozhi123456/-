// wyz258.h : main header file for the WYZ258 application
//

#if !defined(AFX_WYZ258_H__B5A16DDF_AE70_4167_AFB1_EC8775C7CB6F__INCLUDED_)
#define AFX_WYZ258_H__B5A16DDF_AE70_4167_AFB1_EC8775C7CB6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWyz258App:
// See wyz258.cpp for the implementation of this class
//

class CWyz258App : public CWinApp
{
public:
	CWyz258App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWyz258App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWyz258App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WYZ258_H__B5A16DDF_AE70_4167_AFB1_EC8775C7CB6F__INCLUDED_)
