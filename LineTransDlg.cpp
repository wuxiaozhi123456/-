// LineTransDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wyz258.h"
#include "LineTransDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineTransDlg dialog


CLineTransDlg::CLineTransDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineTransDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineTransDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLineTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineTransDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineTransDlg, CDialog)
	//{{AFX_MSG_MAP(CLineTransDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineTransDlg message handlers
// 斜率a和截距b
double gradient = 1;
double intercept = 0;
void CLineTransDlg::OnOK() 
{
	// TODO: Add extra validation here
	char c1[10];
	char c2[10];
	GetDlgItemText(IDC_GRADIENT, c1, 10);
	GetDlgItemText(IDC_INTERCEPT, c2, 10);
	gradient = atof(c1);
	intercept = atof(c2);
	if(0 == gradient)
	{
		gradient = 1;
		MessageBox("斜率不能为0!!!");
	}
	CDialog::OnOK();
}
