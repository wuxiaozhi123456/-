#if !defined(AFX_FFTFILTERDLG_H__DC25A0E8_CDDD_4C3C_93BF_1251B252683E__INCLUDED_)
#define AFX_FFTFILTERDLG_H__DC25A0E8_CDDD_4C3C_93BF_1251B252683E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FftFilterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFftFilterDlg dialog

class CFftFilterDlg : public CDialog
{
// Construction
public:
	CFftFilterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFftFilterDlg)
	enum { IDD = IDD_DIALOG3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFftFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFftFilterDlg)
	afx_msg void OnDiaEdit();
	afx_msg void OnDiaOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FFTFILTERDLG_H__DC25A0E8_CDDD_4C3C_93BF_1251B252683E__INCLUDED_)
