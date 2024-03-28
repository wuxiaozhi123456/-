// wyz258Doc.h : interface of the CWyz258Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WYZ258DOC_H__15F498B6_0BB8_4D03_A41E_45679731A924__INCLUDED_)
#define AFX_WYZ258DOC_H__15F498B6_0BB8_4D03_A41E_45679731A924__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWyz258Doc : public CDocument
{
protected: // create from serialization only
	CWyz258Doc();
	DECLARE_DYNCREATE(CWyz258Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWyz258Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWyz258Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWyz258Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WYZ258DOC_H__15F498B6_0BB8_4D03_A41E_45679731A924__INCLUDED_)
