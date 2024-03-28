#if !defined(AFX_LINETRANSDLG_H__4CBCCBF3_E61D_4777_86C9_096AE3DE2797__INCLUDED_)
#define AFX_LINETRANSDLG_H__4CBCCBF3_E61D_4777_86C9_096AE3DE2797__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineTransDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLineTransDlg dialog

class CLineTransDlg : public CDialog
{
// Construction
public:
	CLineTransDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineTransDlg)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineTransDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLineTransDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINETRANSDLG_H__4CBCCBF3_E61D_4777_86C9_096AE3DE2797__INCLUDED_)
