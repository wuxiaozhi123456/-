// FftBatterFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wyz258.h"
#include "FftBatterFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFftBatterFilterDlg dialog


CFftBatterFilterDlg::CFftBatterFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFftBatterFilterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFftBatterFilterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFftBatterFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFftBatterFilterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFftBatterFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CFftBatterFilterDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFftBatterFilterDlg message handlers

int Diameter2 = 0;
int Nparmer2 = 2;
void CFftBatterFilterDlg::OnOK() 
{
	// TODO: Add extra validation here
	Diameter2 =  GetDlgItemInt(IDC_EDIT1);
	Nparmer2  =  GetDlgItemInt(IDC_EDIT2);
	CDialog::OnOK();
}
