#if !defined(AFX_FFTBATTERFILTERDLG_H__382C484E_96A4_44E5_9D2F_6DE02D89EA14__INCLUDED_)
#define AFX_FFTBATTERFILTERDLG_H__382C484E_96A4_44E5_9D2F_6DE02D89EA14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FftBatterFilterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFftBatterFilterDlg dialog

class CFftBatterFilterDlg : public CDialog
{
// Construction
public:
	CFftBatterFilterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFftBatterFilterDlg)
	enum { IDD = IDD_DIALOG4 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFftBatterFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFftBatterFilterDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FFTBATTERFILTERDLG_H__382C484E_96A4_44E5_9D2F_6DE02D89EA14__INCLUDED_)
