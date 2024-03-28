// wyz258View.cpp : implementation of the CWyz258View class
//

#include "stdafx.h"
#include "wyz258.h"

#include "wyz258Doc.h"
#include "wyz258View.h"
#include "HistogramDlg.h"
#include "LineTransDlg.h"
#include "FftFilterDlg.h"
#include "FftBatterFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWyz258View

IMPLEMENT_DYNCREATE(CWyz258View, CScrollView)

BEGIN_MESSAGE_MAP(CWyz258View, CScrollView)
	//{{AFX_MSG_MAP(CWyz258View)
	ON_COMMAND(ID_GRAY, OnGray)
	ON_UPDATE_COMMAND_UI(ID_GRAY, OnUpdateGray)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_HISTOGRAMDLG, OnHistogramdlg)
	ON_UPDATE_COMMAND_UI(ID_HISTOGRAMDLG, OnUpdateHistogramdlg)
	ON_COMMAND(ID_LINETRANS, OnLinetrans)
	ON_UPDATE_COMMAND_UI(ID_LINETRANS, OnUpdateLinetrans)
	ON_COMMAND(ID_EQUALIZE, OnEqualize)
	ON_UPDATE_COMMAND_UI(ID_EQUALIZE, OnUpdateEqualize)
	ON_COMMAND(ID_FT, OnFt)
	ON_UPDATE_COMMAND_UI(ID_FT, OnUpdateFt)
	ON_COMMAND(ID_IFT, OnIft)
	ON_UPDATE_COMMAND_UI(ID_IFT, OnUpdateIft)
	ON_COMMAND(ID_IFFT, OnIfft)
	ON_UPDATE_COMMAND_UI(ID_IFFT, OnUpdateIfft)
	ON_COMMAND(ID_FFT, OnFft)
	ON_UPDATE_COMMAND_UI(ID_FFT, OnUpdateFft)
	ON_COMMAND(ID_MEAN, OnMean)
	ON_UPDATE_COMMAND_UI(ID_MEAN, OnUpdateMean)
	ON_COMMAND(ID_MEDIAN, OnMedian)
	ON_UPDATE_COMMAND_UI(ID_MEDIAN, OnUpdateMedian)
	ON_COMMAND(ID_RSET, OnRset)
	ON_COMMAND(ID_GRADIENT, OnGradient)
	ON_UPDATE_COMMAND_UI(ID_GRADIENT, OnUpdateGradient)
	ON_COMMAND(ID_LAPLACIAN, OnLaplacian)
	ON_UPDATE_COMMAND_UI(ID_LAPLACIAN, OnUpdateLaplacian)
	ON_COMMAND(ID_IDEA, OnIdea)
	ON_UPDATE_COMMAND_UI(ID_IDEA, OnUpdateIdea)
	ON_COMMAND(ID_GAUSS, OnGauss)
	ON_UPDATE_COMMAND_UI(ID_GAUSS, OnUpdateGauss)
	ON_COMMAND(ID_BUTTER, OnButter)
	ON_UPDATE_COMMAND_UI(ID_BUTTER, OnUpdateButter)
	ON_UPDATE_COMMAND_UI(ID_RSET, OnUpdateRset)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWyz258View construction/destruction

CWyz258View::CWyz258View()
{
	// TODO: add construction code here

}

CWyz258View::~CWyz258View()
{
}

BOOL CWyz258View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWyz258View drawing
extern BITMAPINFO* lpBitsInfo;
// 指向正变换和反变换的数据
extern BITMAPINFO* lpDIB_FT;
extern BITMAPINFO* lpDIB_IFT;
// 指向均值滤波后的数据
extern BITMAPINFO* filt_lpBitsInfo;
void CWyz258View::OnDraw(CDC* pDC)
{
	CWyz258Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if(NULL == lpBitsInfo)
		return;
	void* lpBits = &lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	StretchDIBits(pDC->GetSafeHdc(),
		0, 0, lpBitsInfo->bmiHeader.biWidth, lpBitsInfo->bmiHeader.biHeight,
		0, 0, lpBitsInfo->bmiHeader.biWidth, lpBitsInfo->bmiHeader.biHeight,
		lpBits, lpBitsInfo,
		DIB_RGB_COLORS,
		SRCCOPY);

	if(lpDIB_FT)
	{
		lpBits=&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];
		StretchDIBits(pDC->GetSafeHdc(),
		600,0,lpDIB_FT->bmiHeader.biWidth,lpDIB_FT->bmiHeader.biHeight,
		0,0,lpDIB_FT->bmiHeader.biWidth,lpDIB_FT->bmiHeader.biHeight,
		lpBits,lpDIB_FT,
		DIB_RGB_COLORS,
		SRCCOPY);
	}
	if(lpDIB_IFT) 
	{
		lpBits = &lpDIB_IFT->bmiColors[lpDIB_IFT->bmiHeader.biClrUsed];
		StretchDIBits(pDC->GetSafeHdc(),
			0, 600, lpDIB_IFT->bmiHeader.biWidth, lpDIB_IFT->bmiHeader.biHeight,
			0, 0, lpDIB_IFT->bmiHeader.biWidth, lpDIB_IFT->bmiHeader.biHeight,
			lpBits, lpDIB_IFT,
			DIB_RGB_COLORS,
			SRCCOPY);
	}
	if(filt_lpBitsInfo)
	{
		lpBits=&filt_lpBitsInfo->bmiColors[filt_lpBitsInfo->bmiHeader.biClrUsed];
		StretchDIBits(pDC->GetSafeHdc(),
		600,0,filt_lpBitsInfo->bmiHeader.biWidth,filt_lpBitsInfo->bmiHeader.biHeight,
		0,  0,filt_lpBitsInfo->bmiHeader.biWidth,filt_lpBitsInfo->bmiHeader.biHeight,
		lpBits,filt_lpBitsInfo,
		DIB_RGB_COLORS,
		SRCCOPY);
	}
}

void CWyz258View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	// 设置滚动条
	sizeTotal.cx = sizeTotal.cy = 1200;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CWyz258View printing

BOOL CWyz258View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWyz258View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWyz258View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWyz258View diagnostics

#ifdef _DEBUG
void CWyz258View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWyz258View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWyz258Doc* CWyz258View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWyz258Doc)));
	return (CWyz258Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWyz258View message handlers
void gray();
void gray2();
void CWyz258View::OnGray() 
{
	// TODO: Add your command handler code here
	if(1 == lpBitsInfo->bmiHeader.biBitCount)
	{
		MessageBox("2值图像无法进行灰度化处理!!!");
		return;
	}
	gray();
	Invalidate();
}

void CWyz258View::OnUpdateGray(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL);
}

void pixel(int i, int j, char* str);
void CWyz258View::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	char xy[100];
	memset(xy, 0, 100);
	sprintf(xy, "x:%d y:%d ", point.x, point.y);

	char rgb[100];
	memset(rgb, 0, 100);
	pixel(point.y, point.x, rgb);

	strcat(xy, rgb);

	((CFrameWnd*)GetParent())->SetMessageText(xy);
	CScrollView::OnMouseMove(nFlags, point);
}

void CWyz258View::OnHistogramdlg() 
{
	// TODO: Add your command handler code here
	if(1 == lpBitsInfo->bmiHeader.biBitCount)
	{
		MessageBox("2值图像没有直方图!!!");
		return;
	}
	CHistogramDlg dlg;
	dlg.DoModal();
}
void CWyz258View::OnUpdateHistogramdlg(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL);
}


extern double gradient;
extern double intercept;
void LineTrans(float,float);
void CWyz258View::OnLinetrans() 
{
	// TODO: Add your command handler code here
	if(1 == lpBitsInfo->bmiHeader.biBitCount)
	{
		MessageBox("2值图像无法进行线性运算!!!");
		return;
	}
	CLineTransDlg dlg;
	dlg.DoModal();
	
	LineTrans(gradient,intercept);
	Invalidate();
}
BOOL IsGray();
void CWyz258View::OnUpdateLinetrans(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL);
}

// 直方图均衡化
void Equalize();
void CWyz258View::OnEqualize() 
{
	// TODO: Add your command handler code here
	if(1 == lpBitsInfo->bmiHeader.biBitCount)
	{
		MessageBox("2值图像无法进行直方图均衡化!!!");
		return;
	}
	Equalize();
	Invalidate();
}

void CWyz258View::OnUpdateEqualize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL);
}

// 基本傅里叶变化
void Fourier();
void CWyz258View::OnFt() 
{
	// TODO: Add your command handler code here
	Fourier();
	Invalidate();
}
void CWyz258View::OnUpdateFt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}
// 基本傅里叶变化的反变化
void IFourier();
void CWyz258View::OnIft() 
{
	// TODO: Add your command handler code here
	IFourier();
	Invalidate();
}
BOOL FD_Available();
void CWyz258View::OnUpdateIft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FD_Available() && NULL == lpDIB_IFT);
}

// 快速傅里叶变化
void FFourier();
void CWyz258View::OnFft() 
{
	// TODO: Add your command handler code here
	if (lpDIB_FT)
	{
		free(lpDIB_FT);
		lpDIB_FT = NULL;
	}

	if (lpDIB_IFT)
	{
		free(lpDIB_IFT);
		lpDIB_IFT = NULL;
	} 

	FFourier();
	Invalidate();
}
void CWyz258View::OnUpdateFft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray()); 
}

// 快速傅里叶变化的反变化
void IFFourier();
void CWyz258View::OnIfft() 
{
	// TODO: Add your command handler code here
	if (lpDIB_IFT)
	{
		free(lpDIB_IFT);
		lpDIB_IFT = NULL;
	}
	IFFourier();
	Invalidate();
	
}
void CWyz258View::OnUpdateIfft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FD_Available() && NULL == lpDIB_IFT);
}


void MeanF(int width = 3,int height = 3);
void CWyz258View::OnMean() 
{
	// TODO: Add your command handler code here
	MeanF(3,3);
	Invalidate();
}


void CWyz258View::OnUpdateMean(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

void MedF(int width = 3,int height = 3);
void CWyz258View::OnMedian() 
{
	// TODO: Add your command handler code here
	MedF(3,3);
	Invalidate();
}

void CWyz258View::OnUpdateMedian(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

// 复位按钮
void CWyz258View::OnRset() 
{
	// TODO: Add your command handler code here
	if(lpDIB_FT != NULL)
	{
		free(lpDIB_FT);
		lpDIB_FT = NULL;
	}
	if(lpDIB_IFT != NULL)
	{
		free(lpDIB_IFT);
		lpDIB_IFT = NULL;
	}
	if(filt_lpBitsInfo != NULL)
	{
		free(filt_lpBitsInfo);
		filt_lpBitsInfo = NULL;
	}
	Invalidate();
}


void CWyz258View::OnUpdateRset(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}


// 梯度锐化
void G_Sharp();
void CWyz258View::OnGradient() 
{
	// TODO: Add your command handler code here
	G_Sharp();
	Invalidate();
}

void CWyz258View::OnUpdateGradient(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

// 拉普拉斯锐化
void L_Sharp();
void CWyz258View::OnLaplacian() 
{
	// TODO: Add your command handler code here
	L_Sharp();
	Invalidate();
}

void CWyz258View::OnUpdateLaplacian(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

extern int Diameter;
void FFT_Filter(int D);
// 理想滤波器
void CWyz258View::OnIdea() 
{
	// TODO: Add your command handler code here
	CFftFilterDlg dlg;
	dlg.DoModal();
	FFT_Filter(Diameter);
	Invalidate();
}
void CWyz258View::OnUpdateIdea(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpDIB_FT != NULL && IsGray());
}

extern int Diameter2;
extern int Nparmer2;
void FFT_BATTER_Filter(int D,int N);
// 巴特沃斯滤波器
void CWyz258View::OnButter() 
{
	CFftBatterFilterDlg dlg;
	dlg.DoModal();
	// TODO: Add your command handler code here
	FFT_BATTER_Filter(Diameter2,Nparmer2);
	Invalidate();
}

void CWyz258View::OnUpdateButter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpDIB_FT != NULL && IsGray());
}

void FFT_GAUSS_Filter(int D);
// 高斯滤波器
void CWyz258View::OnGauss() 
{
	// TODO: Add your command handler code here
	CFftFilterDlg dlg;
	dlg.DoModal();
	FFT_GAUSS_Filter(Diameter);
	Invalidate();
}

void CWyz258View::OnUpdateGauss(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpDIB_FT != NULL && IsGray());
}


