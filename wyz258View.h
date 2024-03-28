// wyz258View.h : interface of the CWyz258View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WYZ258VIEW_H__1D6FD8A6_B80B_49DC_9462_1F9C385FBD91__INCLUDED_)
#define AFX_WYZ258VIEW_H__1D6FD8A6_B80B_49DC_9462_1F9C385FBD91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWyz258View : public CScrollView
{
protected: // create from serialization only
	CWyz258View();
	DECLARE_DYNCREATE(CWyz258View)

// Attributes
public:
	CWyz258Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWyz258View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWyz258View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWyz258View)
	afx_msg void OnGray();
	afx_msg void OnUpdateGray(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHistogramdlg();
	afx_msg void OnUpdateHistogramdlg(CCmdUI* pCmdUI);
	afx_msg void OnLinetrans();
	afx_msg void OnUpdateLinetrans(CCmdUI* pCmdUI);
	afx_msg void OnEqualize();
	afx_msg void OnUpdateEqualize(CCmdUI* pCmdUI);
	afx_msg void OnFt();
	afx_msg void OnUpdateFt(CCmdUI* pCmdUI);
	afx_msg void OnIft();
	afx_msg void OnUpdateIft(CCmdUI* pCmdUI);
	afx_msg void OnIfft();
	afx_msg void OnUpdateIfft(CCmdUI* pCmdUI);
	afx_msg void OnFft();
	afx_msg void OnUpdateFft(CCmdUI* pCmdUI);
	afx_msg void OnMean();
	afx_msg void OnUpdateMean(CCmdUI* pCmdUI);
	afx_msg void OnMedian();
	afx_msg void OnUpdateMedian(CCmdUI* pCmdUI);
	afx_msg void OnRset();
	afx_msg void OnGradient();
	afx_msg void OnUpdateGradient(CCmdUI* pCmdUI);
	afx_msg void OnLaplacian();
	afx_msg void OnUpdateLaplacian(CCmdUI* pCmdUI);
	afx_msg void OnIdea();
	afx_msg void OnUpdateIdea(CCmdUI* pCmdUI);
	afx_msg void OnGauss();
	afx_msg void OnUpdateGauss(CCmdUI* pCmdUI);
	afx_msg void OnButter();
	afx_msg void OnUpdateButter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRset(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in wyz258View.cpp
inline CWyz258Doc* CWyz258View::GetDocument()
   { return (CWyz258Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WYZ258VIEW_H__1D6FD8A6_B80B_49DC_9462_1F9C385FBD91__INCLUDED_)
