// FftFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wyz258.h"
#include "FftFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFftFilterDlg dialog


CFftFilterDlg::CFftFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFftFilterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFftFilterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFftFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFftFilterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFftFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CFftFilterDlg)
	ON_EN_CHANGE(IDC_EDIT1, OnDiaEdit)
	ON_BN_CLICKED(IDOK, OnDiaOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFftFilterDlg message handlers

void CFftFilterDlg::OnDiaEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

int Diameter = 0;
void CFftFilterDlg::OnDiaOk() 
{
	// TODO: Add your control notification handler code here
	Diameter = GetDlgItemInt(IDC_EDIT1);
	CDialog::OnOK();
}
