// wyz258Doc.cpp : implementation of the CWyz258Doc class
//

#include "stdafx.h"
#include "wyz258.h"

#include "wyz258Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWyz258Doc

IMPLEMENT_DYNCREATE(CWyz258Doc, CDocument)

BEGIN_MESSAGE_MAP(CWyz258Doc, CDocument)
	//{{AFX_MSG_MAP(CWyz258Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWyz258Doc construction/destruction

CWyz258Doc::CWyz258Doc()
{
	// TODO: add one-time construction code here

}

CWyz258Doc::~CWyz258Doc()
{
}

BOOL CWyz258Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWyz258Doc serialization

void CWyz258Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWyz258Doc diagnostics

#ifdef _DEBUG
void CWyz258Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWyz258Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWyz258Doc commands
BOOL LoadBmpFile(char *BmpFileName);
BOOL CWyz258Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	LoadBmpFile((char*)lpszPathName);
	return TRUE;
}
