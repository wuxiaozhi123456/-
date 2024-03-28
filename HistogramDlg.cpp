// HistogramDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wyz258.h"
#include "HistogramDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistogramDlg dialog


CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHistogramDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistogramDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistogramDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialog)
	//{{AFX_MSG_MAP(CHistogramDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistogramDlg message handlers
extern DWORD H[3][256];
extern BITMAPINFO* lpBitsInfo;
void Histogram();
BOOL IsGray();

BOOL CHistogramDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	Histogram();

	// 设置对话框标题
    SetWindowText(_T("直方图显示"));

	if(8 == lpBitsInfo->bmiHeader.biBitCount && IsGray())
	{
		// 如果为灰度图像，则将对话框的大小设置小些
		// 设置新的宽度和高度
		int newWidth = 300;  // 以像素为单位
		int newHeight = 300;

		// 获取当前对话框的位置
		CRect rect;
		GetWindowRect(&rect);

		// 计算新的左上角坐标
		int newX = rect.left;
		int newY = rect.top;

		// 设置新的右下角坐标
		int newRight = newX + newWidth;
		int newBottom = newY + newHeight;

		// 使用SetWindowPos函数设置对话框的位置和大小
		SetWindowPos(NULL, newX, newY, newWidth, newHeight, SWP_NOZORDER | SWP_NOMOVE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHistogramDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// 如果为256灰度图像，那么只用使用直方图数组的第一通道
	if(8 == lpBitsInfo->bmiHeader.biBitCount && IsGray())
	{
		int i;
		DWORD max = 0;
		for(i = 0;i < 256;i++)
		{
			if (H[0][i] > max)
				max = H[0][i];
		}
		dc.Rectangle(20,20,277,221);
		dc.TextOut(100,235,"灰度通道");
		for(i = 0;i < 256;i++)
		{
			dc.MoveTo(i+20,220);
			dc.LineTo(i+20,220 - (int)(H[0][i] * 200/max));
		}
	}
	else
	{
		int i,j;
		DWORD max = 0;
		for(i = 0;i < 3;i++)
			for(j = 0;j < 256;j++)
				if(H[i][j] > max)
					max = H[i][j];
		// 外框参数
		int XY[3][4] = {{20,20,277,221},{287,20,544,221},{554,20,811,221}};
		// 画出R,G,B三个直方图的外框
		dc.Rectangle(XY[0][0],XY[0][1],XY[0][2],XY[0][3]);
		dc.Rectangle(XY[1][0],XY[1][1],XY[1][2],XY[1][3]);
		dc.Rectangle(XY[2][0],XY[2][1],XY[2][2],XY[2][3]);
		for(i = 0;i < 3;i++)
		{
			int px = 100;
			if(0 == i)
				dc.TextOut(px+XY[i][0],235,"红色通道");
			else if(1 == i)
				dc.TextOut(px+XY[i][0],235,"绿色通道");
			else if(2 == i)
				dc.TextOut(px+XY[i][0],235,"蓝色通道");
			for(j = 0;j < 256;j++)
			{
				dc.MoveTo(j+XY[i][0],220);
				dc.LineTo(j+XY[i][0],220 - (int)(H[i][j] * 200/max));	
			}
		}
	}
	// Do not call CDialog::OnPaint() for painting messages
}
