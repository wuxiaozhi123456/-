#include "stdafx.h"
#include <complex>
using namespace std;
#define PI 3.1415926535

BITMAPINFO* lpBitsInfo = NULL;
// 指向正变换和反变换的数据
BITMAPINFO* lpDIB_FT = NULL;
BITMAPINFO* lpDIB_IFT = NULL;
// 指向频域数据
complex<double>* gFD = NULL;
// 指向均值滤波后的数据
BITMAPINFO* filt_lpBitsInfo = NULL;
BOOL LoadBmpFile(char *BmpFileName)
{
	FILE* fp;
	if(NULL == (fp = fopen(BmpFileName,"rb")))
		return false;

	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	
	fread(&bf,14,1,fp);
	fread(&bi,40,1,fp);

	int lineBytes = (bi.biWidth * bi.biBitCount + 31)/32 * 4;
	int imgSize = lineBytes * bi.biHeight;

	int numColors;
	if (bi.biClrUsed != 0)
		numColors = bi.biClrUsed;
	else
	{
		switch(bi.biBitCount)
		{
		case 1:
			numColors = 2;
			break;
		case 4:
			numColors = 16;
			break;
		case 8:
			numColors = 256;
			break;
		case 24:
			numColors = 0;
			break;
		}
	}

	int size = 40 + numColors * 4 + imgSize;
	if (NULL == (lpBitsInfo = (BITMAPINFO*)malloc(size)))
		return false;

	fseek(fp, 14, SEEK_SET);
	fread(lpBitsInfo, size, 1, fp);
	lpBitsInfo->bmiHeader.biClrUsed = numColors;

	fclose(fp);
	return true;
}

BOOL IsGray();
void gray2();
void gray3();
void gray4();

// 16色、24位真彩和256色彩色转灰度图像
void gray()
{
	if(NULL == lpBitsInfo)
		return;
	int biBitCount = lpBitsInfo->bmiHeader.biBitCount;
	// 过滤2值图像
	if(1 == biBitCount)
		return;
	// 如果是16色图像，调用gray4()转灰度
	if(4 == biBitCount)
		gray4();
	// 如果是8位图像并且不是灰度图像，那么为256色彩色，调用gray3()转灰度
	else if(8 == biBitCount && !IsGray())
		gray3();
	// 如果是24位真彩图像，调用gray2()转灰度
	else if(24 == biBitCount)
		gray2();
}

// 24位真彩转灰度
void gray2()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	/*
	// 第一种方法，虚假的修改
	int i,j;
	BYTE *R,*G,*B,avg;
	
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			B = lpBits + LineBytes * (h - 1 - i) + j * 3;
			G = B + 1;
			R = G + 1;
			avg = (*R + *G + *B)/3;
			*R = * G = *B = avg;
		}
	}
	*/

	// 第二种方法，真实的修改

	// 计算灰度图像实际位图数据每行的字节数
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// 计算灰度图像的内存量
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// 给灰度图像分配内存
	BITMAPINFO* gray_lpBitsInfo = (BITMAPINFO*)malloc(size);
	// 填写信息头和改写信息头
	memcpy(gray_lpBitsInfo,lpBitsInfo,40);
	gray_lpBitsInfo->bmiHeader.biBitCount = 8;
	gray_lpBitsInfo->bmiHeader.biClrUsed = 256;
	// 填写调色板
	int i,j;
	for(i = 0;i < 256;i++)
	{
		gray_lpBitsInfo->bmiColors[i].rgbBlue = i;
		gray_lpBitsInfo->bmiColors[i].rgbGreen = i;
		gray_lpBitsInfo->bmiColors[i].rgbRed = i;
		// 保留字
		gray_lpBitsInfo->bmiColors[i].rgbReserved = 0; 
	}

	// 填写实际位图数据

	BYTE *R,*G,*B,*pixel,avg;
	// 实际位图数据的起始点
	BYTE* gray_lpBits = (BYTE*)&gray_lpBitsInfo->bmiColors[gray_lpBitsInfo->bmiHeader.biClrUsed];

	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			B = lpBits + LineBytes * (h - 1 - i) + j * 3;
			G = B + 1;
			R = G + 1;
			avg = (*R + *G + *B)/3;
			pixel = gray_lpBits + gray_LineBytes * (h - 1 - i) + j;
			*pixel = avg;
		}
	}
	// 更新数据
	free(lpBitsInfo);
	lpBitsInfo = gray_lpBitsInfo;
}


// 256色图像转灰度图像
void gray3()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	/*
	// 第一种方法，直接修改调色板的数据	
	int i,r,g,b,avg;
	for(i = 0;i < 256;i++)
	{
		b = lpBitsInfo->bmiColors[i].rgbBlue;
		g = lpBitsInfo->bmiColors[i].rgbGreen;
		r = lpBitsInfo->bmiColors[i].rgbRed;
		avg = (b + g + r)/3;
		lpBitsInfo->bmiColors[i].rgbBlue = avg;
		lpBitsInfo->bmiColors[i].rgbGreen = avg;
		lpBitsInfo->bmiColors[i].rgbRed = avg;
	}	
	*/
	
	// 第二种，真实修改为灰度图像

	// 计算灰度图像实际位图数据每行的字节数
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// 计算灰度图像的内存量
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// 给灰度图像分配内存
	BITMAPINFO* gray_lpBitsInfo = (BITMAPINFO*)malloc(size);
	// 填写信息头和改写信息头
	memcpy(gray_lpBitsInfo,lpBitsInfo,40);
	// 填写调色板
	int i,j;
	for(i = 0;i < 256;i++)
	{
		gray_lpBitsInfo->bmiColors[i].rgbBlue = i;
		gray_lpBitsInfo->bmiColors[i].rgbGreen = i;
		gray_lpBitsInfo->bmiColors[i].rgbRed = i;
		// 保留字
		gray_lpBitsInfo->bmiColors[i].rgbReserved = 0; 
	}

	// 填写实际位图数据

	BYTE *pixel,avg,b,g,r,index;
	// 实际位图数据的起始点
	BYTE* gray_lpBits = (BYTE*)&gray_lpBitsInfo->bmiColors[gray_lpBitsInfo->bmiHeader.biClrUsed];
		
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			index = *(lpBits + LineBytes * (h - 1 - i) + j);
			b = lpBitsInfo->bmiColors[index].rgbBlue;
			g = lpBitsInfo->bmiColors[index].rgbGreen;
			r = lpBitsInfo->bmiColors[index].rgbRed;
			avg = (b + g + r)/3;
			pixel = gray_lpBits + gray_LineBytes * (h - 1 - i) + j;
			*pixel = avg;
		}
	}
	// 更新数据
	free(lpBitsInfo);
	lpBitsInfo = gray_lpBitsInfo;
}
// 16色转灰度
void gray4()
{
	/*
	// 类似于256色转灰度的第一种方法
	// 直接修改调色板的数据
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	int i,r,g,b,avg;
	for(i = 0;i < 256;i++)
	{
		b = lpBitsInfo->bmiColors[i].rgbBlue;
		g = lpBitsInfo->bmiColors[i].rgbGreen;
		r = lpBitsInfo->bmiColors[i].rgbRed;
		avg = (b + g + r)/3;
		lpBitsInfo->bmiColors[i].rgbBlue = avg;
		lpBitsInfo->bmiColors[i].rgbGreen = avg;
		lpBitsInfo->bmiColors[i].rgbRed = avg;
	}
	*/

	// 第二种方法，真实修改为灰度图像
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	// 计算灰度图像实际位图数据每行的字节数
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// 计算灰度图像的内存量
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// 给灰度图像分配内存
	BITMAPINFO* gray_lpBitsInfo = (BITMAPINFO*)malloc(size);
	// 填写信息头和改写信息头
	memcpy(gray_lpBitsInfo,lpBitsInfo,40);
	gray_lpBitsInfo->bmiHeader.biBitCount = 8;
	gray_lpBitsInfo->bmiHeader.biClrUsed = 256;
	// 填写调色板
	int i,j;
	for(i = 0;i < 256;i++)
	{
		gray_lpBitsInfo->bmiColors[i].rgbBlue = i;
		gray_lpBitsInfo->bmiColors[i].rgbGreen = i;
		gray_lpBitsInfo->bmiColors[i].rgbRed = i;
		// 保留字
		gray_lpBitsInfo->bmiColors[i].rgbReserved = 0; 
	}

	// 填写实际位图数据
	BYTE *pixel,avg,b,g,r,index;
	// 实际位图数据的起始点
	BYTE* gray_lpBits = (BYTE*)&gray_lpBitsInfo->bmiColors[gray_lpBitsInfo->bmiHeader.biClrUsed];
		
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j / 2;
			// 如果处于高位半字
			if(j % 2 == 0)
				index = *pixel / 16;
			// 如果处于低位半字
			else
				index = *pixel % 16;
			b = lpBitsInfo->bmiColors[index].rgbBlue;
			g = lpBitsInfo->bmiColors[index].rgbGreen;
			r = lpBitsInfo->bmiColors[index].rgbRed;
			avg = (b + g + r)/3;
			pixel = gray_lpBits + gray_LineBytes * (h - 1 - i) + j;
			*pixel = avg;
		}
	}
	// 更新数据
	free(lpBitsInfo);
	lpBitsInfo = gray_lpBitsInfo;
}
// 判断一个图像是否为灰度图像
BOOL IsGray()
{
	int r, g, b;
	if (8 == lpBitsInfo->bmiHeader.biBitCount)
	{
		r = lpBitsInfo->bmiColors[128].rgbRed;
		g = lpBitsInfo->bmiColors[128].rgbGreen;
		b = lpBitsInfo->bmiColors[128].rgbBlue;
		if(r == b && r == g)
			return true;
	}
	return false;
}

// 访问像素点函数
void pixel(int i, int j, char* str)
{
	if(NULL == lpBitsInfo)
		return;

	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	if(i >= h || j >= w)
		return;

	BYTE *pixel, bv, r, g, b;
	// 定义索引
	BYTE index;
	switch(lpBitsInfo->bmiHeader.biBitCount)
	{
	case 1:
		bv = *(lpBits + LineBytes * (h - 1 - i) + j / 8) & (1 << (7 - j % 8));
		if(0 == bv)
			strcpy(str, "背景点");
		else
			strcpy(str, "前景点");
		break;
	case 4:
		pixel = lpBits + LineBytes * (h - 1 - i) + j / 2;
		// 如果处于高位半字
		if(j % 2 == 0)
			index = *pixel / 16;
		// 如果处于低位半字
		else
			index = *pixel % 16;
		r = lpBitsInfo->bmiColors[index].rgbRed;
		g = lpBitsInfo->bmiColors[index].rgbGreen;
		b = lpBitsInfo->bmiColors[index].rgbBlue;
		sprintf(str, "RGB(%d,%d,%d)", r, g, b);
		break;
	case 8:
		pixel = lpBits + LineBytes * (h - 1 - i) + j;
		if(IsGray())
			sprintf(str, "亮度值: %d", *pixel);
		else
		{
			r = lpBitsInfo->bmiColors[*pixel].rgbRed;
			g = lpBitsInfo->bmiColors[*pixel].rgbGreen;
			b = lpBitsInfo->bmiColors[*pixel].rgbBlue;
			sprintf(str, "RGB(%d,%d,%d)", r, g, b);
		}
		break;
	case 24:
		BYTE *R,*G,*B;
		B = lpBits + LineBytes * (h - 1 - i) + j * 3;
		G = B + 1;
		R = G + 1;
		sprintf(str, "RGB(%d,%d,%d)", *R, *G, *B);
		break;
	}
}
// 灰度图像选择H[0]为直方图数据
// 彩色图像中，R为H[0],G为H[1],B为H[2]
DWORD H[3][256];
void Histogram1(); // 16色的直方图计算函数
void Histogram2(); // 256灰度的直方图计算函数
void Histogram3(); // 256彩色的直方图计算函数
void Histogram4(); // 24位真彩的直方图计算函数
void Histogram()
{
	// 对指针情况进行过滤
	if(NULL == lpBitsInfo)
		return;
	// 对二值图像进行过滤
	else if(1 == lpBitsInfo->bmiHeader.biBitCount)
		return;
	// 16色图像的直方图计算函数映射
	else if(4 == lpBitsInfo->bmiHeader.biBitCount)
		Histogram1();
	// 256灰度图像的直方图计算函数映射
	else if(8 == lpBitsInfo->bmiHeader.biBitCount && IsGray())
		Histogram2();
	// 256彩色图像的直方图计算函数映射
	else if(8 == lpBitsInfo->bmiHeader.biBitCount && !IsGray())
		Histogram3();
	// 24位真彩图像的直方图计算函数映射
	else if(24 == lpBitsInfo->bmiHeader.biBitCount)
		Histogram4();
}
// 16色图像的直方图计算函数
void Histogram1()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// 初始化直方图元素为0
	int i,j;
	for(i = 0;i < 3;i++)
	{
		for(j = 0;j < 256;j++)
			H[i][j] = 0;
	}
	// 计算直方图
	BYTE *pixel,b,g,r,index;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j / 2;
			// 如果处于高位半字
			if(j % 2 == 0)
				index = *pixel / 16;
			// 如果处于低位半字
			else
				index = *pixel % 16;
			b = lpBitsInfo->bmiColors[index].rgbBlue;
			g = lpBitsInfo->bmiColors[index].rgbGreen;
			r = lpBitsInfo->bmiColors[index].rgbRed;
			H[0][r]++;
			H[1][g]++;
			H[2][b]++;
		}
	}
}
// 256灰度的直方图计算函数
void Histogram2()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// 只使用到直方图数组的第一个通道数组
	int i,j;
	BYTE* pixel;
	for(i = 0;i < 256;i++)
	{
		H[0][i] = 0;
	}
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h -1 -i) + j;
			H[0][*pixel]++;
		}
	}
}
// 256彩色图像的直方图计算函数
void Histogram3()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	// 初始化直方图元素为0
	int i,j;
	for(i = 0;i < 3;i++)
	{
		for(j = 0;j < 256;j++)
			H[i][j] = 0;
	}
	// 读取实际数据,计算直方图数组
	int index,r,g,b;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			index = *(lpBits + LineBytes * (h - 1 - i) + j);
			b = lpBitsInfo->bmiColors[index].rgbBlue;
			g = lpBitsInfo->bmiColors[index].rgbGreen;
			r = lpBitsInfo->bmiColors[index].rgbRed;
			H[0][r]++;
			H[1][g]++;
			H[2][b]++;
		}
	}
}
void Histogram4()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	// 初始化直方图元素为0
	int i,j;
	for(i = 0;i < 3;i++)
	{
		for(j = 0;j < 256;j++)
			H[i][j] = 0;
	}
	// 读取实际数据，计算直方图数组
	BYTE *R,*G,*B;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			B = lpBits + LineBytes * (h - 1 - i) + j * 3;
			G = B + 1;
			R = G + 1;
			H[0][*R]++;
			H[1][*G]++;
			H[2][*B]++;
		}
	}
}

// 线性计算函数
int cal(int x,float a,float b)
{
	int y = (int)(a * x + b + 0.5);
	if(y < 0)
		y = 0;
	else if(y > 255)
		y = 255;
	return y;
}
void LineTrans1(float a,float b);
void LineTrans2(float a,float b);
void LineTrans3(float a,float b);
void LineTrans4(float a,float b);
void LineTrans (float a,float b)
{
	// 对指针情况进行过滤
	if(NULL == lpBitsInfo)
		return;
	// 对二值图像进行过滤
	else if(1 == lpBitsInfo->bmiHeader.biBitCount)
		return;
	// 16色图像的线性计算函数映射
	else if(4 == lpBitsInfo->bmiHeader.biBitCount)
		LineTrans1(a,b);
	// 256灰度图像的线性计算函数映射
	else if(8 == lpBitsInfo->bmiHeader.biBitCount && IsGray())
		LineTrans2(a,b);
	// 256彩色图像的线性计算函数映射
	else if(8 == lpBitsInfo->bmiHeader.biBitCount && !IsGray())
		LineTrans3(a,b);
	// 24位真彩图像的线性计算函数映射
	else if(24 == lpBitsInfo->bmiHeader.biBitCount)
		LineTrans4(a,b);
}

// 16色彩色的线性计算函数
void LineTrans1(float a,float b)
{
	int i;
	int R,G,B;
	for(i = 0;i < 16;i++)
	{
		R = lpBitsInfo->bmiColors[i].rgbRed;
		G = lpBitsInfo->bmiColors[i].rgbGreen;
		B = lpBitsInfo->bmiColors[i].rgbBlue;
		lpBitsInfo->bmiColors[i].rgbRed = cal(R,a,b);
		lpBitsInfo->bmiColors[i].rgbGreen = cal(G,a,b);
		lpBitsInfo->bmiColors[i].rgbBlue = cal(B,a,b);
	}
}
// 256灰度的线性计算函数
void LineTrans2(float a,float b)
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	int i,j;
	BYTE *pixel;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h -1 -i) + j;
			*pixel = cal(*pixel,a,b);
		}
	}
}
// 256彩色的线性计算函数
void LineTrans3(float a,float b)
{
	int i;
	int R,G,B;
	for(i = 0;i < 256;i++)
	{
		R = lpBitsInfo->bmiColors[i].rgbRed;
		G = lpBitsInfo->bmiColors[i].rgbGreen;
		B = lpBitsInfo->bmiColors[i].rgbBlue;
		lpBitsInfo->bmiColors[i].rgbRed = cal(R,a,b);
		lpBitsInfo->bmiColors[i].rgbGreen = cal(G,a,b);
		lpBitsInfo->bmiColors[i].rgbBlue = cal(B,a,b);
	}
}
// 24位真彩的线性计算函数
void LineTrans4(float a,float b)
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	BYTE *R,*G,*B;
	int i,j;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			B = lpBits + LineBytes * (h - 1 - i) + j * 3;
			G = B + 1;
			R = G + 1;
			*R = cal(*R,a,b);
			*G = cal(*G,a,b);
			*B = cal(*B,a,b);
		}
	}
}
void Equalize1();
void Equalize2();
void Equalize3();
void Equalize4();

void Equalize()
{
	// 对指针情况进行过滤
	if(NULL == lpBitsInfo)
		return;
	// 对二值图像进行过滤
	else if(1 == lpBitsInfo->bmiHeader.biBitCount)
		return;
	// 16色图像的直方图均衡化函数映射
	else if(4 == lpBitsInfo->bmiHeader.biBitCount)
		Equalize1();
	// 256灰度图像的直方图均衡化函数映射
	else if(8 == lpBitsInfo->bmiHeader.biBitCount && IsGray())
		Equalize2();
	// 256彩色图像的直方图均衡化函数映射
	else if(8 == lpBitsInfo->bmiHeader.biBitCount && !IsGray())
		Equalize3();
	// 24位真彩图像的直方图均衡化函数映射
	else if(24 == lpBitsInfo->bmiHeader.biBitCount)
		Equalize4();
}

// 16色直方图均衡化
void Equalize1()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	int i,j,k;
	int temp;
	BYTE Map[3][256];
	Histogram();

	// 对每一个通道进行计算映射数组的处理
	for(i = 0;i < 3;i++)
	{
		for(k = 0;k < 256;k++)
		{
			temp = 0;
			for(j = 0;j <= k;j++)
				temp += H[i][j];
			Map[i][k] = (int)(temp * 255 / (w * h) + 0.5);
		}
	}
	// 根据映射值改变调色板的三个通道的值
	int r,g,b;
	for(i = 0;i < 16;i++)
	{
		r = lpBitsInfo->bmiColors[i].rgbRed;
		g = lpBitsInfo->bmiColors[i].rgbGreen;
		b = lpBitsInfo->bmiColors[i].rgbBlue;
		lpBitsInfo->bmiColors[i].rgbRed = Map[0][r];
		lpBitsInfo->bmiColors[i].rgbGreen = Map[1][g];
		lpBitsInfo->bmiColors[i].rgbBlue = Map[2][b];
	}
}
// 256色灰度直方图均衡化
void Equalize2()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i,j;
	BYTE *pixel;
	int temp;
	BYTE Map[256];
	Histogram();

	for(i = 0;i < 256;i++)
	{
		// 计算累计值
		temp = 0;
		for(j = 0;j <= i;j++)
			temp += H[0][j];
		// 计算公式：(int)((累计值/总像素的量)*像素最大值+0.5)
		Map[i] = (int)(temp * 255 / (w * h) + 0.5);
	}
	// 对像素值进行映射改变处理
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h -1 -i) + j;
			*pixel = Map[*pixel];
		}
	}
}
// 256色彩色直方图均衡化
void Equalize3()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	int i,j,k;
	int temp;
	BYTE Map[3][256];
	Histogram();
	// 对每一个通道进行计算映射数组的处理
	for(i = 0;i < 3;i++)
	{
		for(k = 0;k < 256;k++)
		{
			temp = 0;
			for(j = 0;j <= k;j++)
				temp += H[i][j];
			Map[i][k] = (int)(temp * 255 / (w * h) + 0.5);
		}
	}
	// 根据映射值改变调色板的三个通道的值
	int r,g,b;
	for(i = 0;i < 256;i++)
	{
		r = lpBitsInfo->bmiColors[i].rgbRed;
		g = lpBitsInfo->bmiColors[i].rgbGreen;
		b = lpBitsInfo->bmiColors[i].rgbBlue;
		lpBitsInfo->bmiColors[i].rgbRed = Map[0][r];
		lpBitsInfo->bmiColors[i].rgbGreen = Map[1][g];
		lpBitsInfo->bmiColors[i].rgbBlue = Map[2][b];
	}
}
// 24位真彩图像直方图均衡化
void Equalize4()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	int i,j,k;
	int temp;
	BYTE Map[3][256];
	Histogram();

	// 对每一个通道进行计算映射数组的处理
	for(i = 0;i < 3;i++)
	{
		for(k = 0;k < 256;k++)
		{
			temp = 0;
			for(j = 0;j <= k;j++)
				temp += H[i][j];
			Map[i][k] = (int)(temp * 255 / (w * h) + 0.5);
		}
	}

	// 根据映射值改变像素数据的三个通道的值
	BYTE *R,*G,*B;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			B = lpBits + LineBytes * (h - 1 - i) + j * 3;
			G = B + 1;
			R = G + 1;
			*R = Map[0][*R];
			*G = Map[1][*G];
			*B = Map[2][*B];
		}
	}
}


// 时域空间转化为频域空间
void FT(complex<double>* TD,complex<double>* FD,int m)
{
	int x,u;
	double angle;
	// 一个频域空间的值需要所有时域空间的值来计算
	for(u = 0;u < m;u++)
	{
		FD[u] = 0;
		for(x = 0;x < m;x++)
		{
			angle = -2 * PI * u * x / m;
			FD[u] += TD[x] * complex<double>(cos(angle),sin(angle));
		}
		FD[u] /= m;
	}
}

// 频域空间转为时域空间
void IFT(complex<double>*FD,complex<double>*TD,int m)
{
	int x,u;
	double angle;
	// 一个时域空间的值需要所有频域空间的值来计算
	for(x = 0;x < m;x++)
	{
		TD[x] = 0;
		for(u = 0;u < m;u++)
		{
			angle = 2 * PI * u * x / m;
			TD[x] += FD[u] * complex<double>(cos(angle),sin(angle));		
		}
	}
}

BOOL FD_Available()
{
	return (gFD != NULL);
}

void Fourier()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	complex<double>* TD = new complex<double>[w * h];
	complex<double>* FD = new complex<double>[w * h];

	// 将时域数据转化为复数形式存储
	int i,j;
	BYTE* pixel;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h -i - 1) + j;
			// 乘以pow(-1,i+j)，实现转换后的坐标变换
			TD[w * i + j] = complex<double>(*pixel * pow(-1,i + j),0);
		}
	}
	// 水平方向做傅里叶变化
	for(i = 0;i < h;i++)
		FT(&TD[i * w],&FD[w * i],w);

	// 做转置操作
	for(i = 0;i < h;i++)
		for(j = 0;j < w;j++)
			TD[h * j + i] = FD[w * i + j];

	// 垂直方向做傅里叶变化
	for(i = 0;i < w;i++)
		FT(&TD[h * i],&FD[h * i],h);

	// 生成图像数据
	LONG size = 40 + 1024 + LineBytes * h;
	lpDIB_FT = (BITMAPINFO*)malloc(size);
	if(NULL == lpDIB_FT)
		return;
	memcpy(lpDIB_FT,lpBitsInfo,size);
	lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];

	double temp;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h -1 -i) + j;
			// 计算幅度值,放大幅度值，经验值
			temp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + 
				        FD[j * h + i].imag() * FD[j * h + i].imag()) * 2000;
			if(temp > 255)
				temp = 255;
			*pixel = (BYTE)(temp);
		}
	}
	delete TD;
	gFD = FD;
}

// 傅里叶反变换
void IFourier() {
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	complex<double>* TD = new complex<double>[w*h];
	complex<double>* FD = new complex<double>[w*h];

	int i,j;
	BYTE * pixel;
	// 转置
	for (i=0;i<h;i++) {
		for(j = 0; j < w ; j++) {
			FD[w * i + j] = gFD[i + h * j];
		}
	}
	// 水平方向上做傅里叶反变换
	for (i = 0; i < h; i++) {
		IFT(&FD[w * i],&TD[w * i],w);
	}
	// 转置
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			FD[h * j + i] = TD[w * i + j];
		}
	}
	// 垂直方向上做傅里叶变换
	for (i = 0; i < w; i++) {
		IFT(&FD[h * i], &TD[h * i], h);
	}

	LONG size = 40 + 1024 + LineBytes * h;

	lpDIB_IFT = (BITMAPINFO*)malloc(size);
	if (NULL == lpDIB_IFT) return;
	memcpy(lpDIB_IFT,lpBitsInfo,size);

	lpBits = (BYTE*)&lpDIB_IFT->bmiColors[lpDIB_IFT->bmiHeader.biClrUsed];
	// 将复数值转化为实数值，求幅度
	for(i = 0; i < h ; i++) {
		for(j = 0; j < w ; j++) {
			pixel = lpBits + LineBytes * (h-1-i) + j;
			*pixel = (BYTE)(TD[j*h+i].real() / pow(-1, i+j));
		}
	}
	delete TD;
	delete FD;
	delete gFD;
	gFD = NULL;
}

void FFT(complex<double> * TD, complex<double> * FD, int r)
{
	// 计算付立叶变换点数
	LONG count = 1 << r;
	// 计算加权系数
	int i;
	double angle;
	complex<double>* W = new complex<double>[count / 2];
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	// 将时域点写入X1
	complex<double>* X1 = new complex<double>[count];
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// 采用蝶形算法进行快速付立叶变换，输出为频域值X2
	complex<double>* X2 = new complex<double>[count]; 

	int k,j,p,size;
	complex<double>* temp;
	for (k = 0; k < r; k++)
	{
		for (j = 0; j < 1 << k; j++)
		{
			size = 1 << (r-k);
			for (i = 0; i < size/2; i++)
			{
				p = j * size;
				X2[i + p] = X1[i + p] + X1[i + p + size/2];
				X2[i + p + size/2] = (X1[i + p] - X1[i + p + size/2]) * W[i * (1<<k)];
			}
		}
		temp  = X1;
		X1 = X2;
		X2 = temp;
	}
	
	// 重新排序（码位倒序排列）
	for (j = 0; j < count; j++)
	{
		p = 0;
		for (i = 0; i < r; i++)
		{
			if (j & (1<<i))
			{
				p += 1<<(r-i-1);
			}
		}
		FD[j]=X1[p];
		FD[j] /= count;
	}
	
	// 释放内存
	delete W;
	delete X1;
	delete X2;
}

void FFourier()
{
	//图像的宽度和高度
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	//指向图像数据指针
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂数，即迭代次数
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂数，即迭代次数
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// 分配内存
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	BYTE* pixel;
	
	for(i = 0; i < FFT_h; i++)  // 行
	{
		for(j = 0; j < FFT_w; j++)  // 列
		{
			// 指向DIB第i行，第j个象素的指针
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// 给时域赋值
			TD[j + FFT_w * i] = complex<double>(*pixel* pow(-1,i+j), 0);
		}
	}
	
	for(i = 0; i < FFT_h; i++)
	{
		// 对y方向进行快速付立叶变换
		FFT(&TD[FFT_w * i], &FD[FFT_w * i], wp);
	}
	
	// 保存中间变换结果
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			TD[i + FFT_h * j] = FD[j + FFT_w * i];
		}
	}
	
	for(i = 0; i < FFT_w; i++)
	{
		// 对x方向进行快速付立叶变换
		FFT(&TD[i * FFT_h], &FD[i * FFT_h], hp);
	}

	//生成频谱图像
	//为频域图像分配内存
	LONG size = 40 + 1024 + LineBytes * height;
	lpDIB_FT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_FT)
		return;
	memcpy(lpDIB_FT, lpBitsInfo, size);

	//指向频域图像数据指针
	lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];

	double temp;
	for(i = 0; i < FFT_h; i++) // 行
	{
		for(j = 0; j < FFT_w; j++) // 列
		{
			// 计算频谱幅度
			temp = sqrt(FD[j * FFT_h + i].real() * FD[j * FFT_h + i].real() + 
				        FD[j * FFT_h + i].imag() * FD[j * FFT_h + i].imag()) *2000;
			
			// 判断是否超过255
			if (temp > 255)
			{
				// 对于超过的，直接设置为255
				temp = 255;
			}
			
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// 更新源图像
			*pixel = (BYTE)(temp);
		}
	}

	delete TD;
	gFD = FD;
}

//IFFT反变换
void IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	// 付立叶变换点数
	LONG	count;
	// 计算付立叶变换点数
	count = 1 << r;

	// 分配运算所需存储器
	complex<double> * X = new complex<double>[count];
	// 将频域点写入X
	memcpy(X, FD, sizeof(complex<double>) * count);
	
	// 求共轭
	for(int i = 0; i < count; i++)
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	
	// 调用快速付立叶变换
	FFT(X, TD, r);
	
	// 求时域点的共轭
	for(i = 0; i < count; i++)
		TD[i] = complex<double> (TD[i].real() * count, -TD[i].imag() * count);
	
	// 释放内存
	delete X;
}

void IFFourier()
{
	//图像的宽度和高度
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂数，即迭代次数
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂数，即迭代次数
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// 分配内存
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	for(i = 0; i < FFT_h; i++)  // 行
		for(j = 0; j < FFT_w; j++)  // 列
			FD[j + FFT_w * i] = gFD[i + FFT_h*j];
	
	// 沿水平方向进行快速付立叶变换
	for(i = 0; i < FFT_h; i++)
		IFFT(&FD[FFT_w * i], &TD[FFT_w * i], wp);
	
	// 保存中间变换结果
	for(i = 0; i < FFT_h; i++)
		for(j = 0; j < FFT_w; j++)
			FD[i + FFT_h * j] = TD[j + FFT_w * i];
	
	// 沿垂直方向进行快速付立叶变换
	for(i = 0; i < FFT_w; i++)
		IFFT(&FD[i * FFT_h], &TD[i * FFT_h], hp);

	//为反变换图像分配内存
	LONG size = 40 + 1024 + LineBytes * height;

	lpDIB_IFT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_IFT)
		return;
	memcpy(lpDIB_IFT, lpBitsInfo, size);

	//指向反变换图像数据指针
	BYTE* lpBits = (BYTE*)&lpDIB_IFT->bmiColors[256];
	BYTE* pixel;
	double temp;
	for(i = 0; i < FFT_h; i++) // 行
	{
		for(j = 0; j < FFT_w; j++) // 列
		{
			pixel = lpBits + LineBytes * (height - 1 - i) + j;
			temp= (TD[j*FFT_h + i].real() / pow(-1, i+j));
			if (temp < 0)
				temp = 0;
			else if (temp >255)
				temp = 255;
			*pixel = (BYTE)temp;
		}
	}

	// 删除临时变量
	delete FD;
	delete TD;
	delete gFD;
	gFD = NULL;
}

// 均值滤波的模板运算
BYTE Mean(BYTE *pixel,int width,int height)
{
	// 每行的长度
	int w = lpBitsInfo->bmiHeader.biWidth;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	// 求所有元素的和
	int i,j,sum;
	BYTE mean;
	sum = 0;
	for(i = - height / 2;i < height - height / 2;i++)
		for(j = - width / 2;j < width - width / 2;j++)
			sum += *(pixel + i * LineBytes + j);
	// 求均值
	mean = BYTE((sum * 1.0) / (height * width) + 0.5);
	// 改变像素值为平均值
	return mean;
}
// 均值滤波
// 模板的默认大小为3*3
void MeanF(int width = 3,int height = 3)
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	// 计算灰度图像实际位图数据每行的字节数
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// 计算灰度图像的内存量
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// 如果是第一次滤波，没有初始化的滤波基础图
	if(NULL == filt_lpBitsInfo)
	{
		// 分配初始内存
		filt_lpBitsInfo = (BITMAPINFO*)malloc(size);
		// 复制灰度图像数据
		memcpy(filt_lpBitsInfo,lpBitsInfo,size);
	}
	// 复制图像
	BITMAPINFO* filt_lpBitsInfo_copy = (BITMAPINFO*)malloc(size);
	memcpy(filt_lpBitsInfo_copy,filt_lpBitsInfo,size);
	/*
	// 计算可进行模板运算的范围
	int filt_w  = w  - ( w  % width );
	int filt_h  = h  - ( h  % height );*/
	// 计算复制的灰度图像的位图数据的起始点
	BYTE* filt_lpBits = (BYTE*)&filt_lpBitsInfo->bmiColors[filt_lpBitsInfo->bmiHeader.biClrUsed];
	BYTE* filt_lpBits_copy = (BYTE*)&filt_lpBitsInfo_copy->bmiColors[filt_lpBitsInfo_copy->bmiHeader.biClrUsed];
	int i,j;
	BYTE* pixel,* filt_pixel;
	// 对复制的图像进行均值滤波
	for(i = height / 2;i < h - height / 2;i++)
	{
		for(j = width / 2;j < w - width / 2;j++)
		{
			pixel = filt_lpBits_copy + LineBytes * (h -1 -i) + j;
			filt_pixel = filt_lpBits + LineBytes * (h -1 -i) + j;
			*filt_pixel = Mean(pixel,width,height);
		}
	}
	// 释放内存
	free(filt_lpBitsInfo_copy);
}

// 中值滤波模板
BYTE Median(BYTE *pixel,int width,int height)
{
	// 每行的长度
	int w = lpBitsInfo->bmiHeader.biWidth;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	int size = width * height;
	int i,j,median;
	BYTE* arr = (BYTE*)malloc(size);
	for(i = -height / 2;i < height - height / 2;i++)
		for(j = -width / 2;j < width - width / 2;j++)
			*(arr + (i + height / 2) * width + (j + width / 2)) = *(pixel + i * LineBytes + j);
	// 冒泡排序
	for(i = size - 1;i > 0;i--)
	{
		int max = arr[i];
		int index = i;
		for(j = 0;j < i;j++)
		{
			if(arr[j] > max)
			{
				max = arr[j];
				index = j;
			}
		}
		int temp = arr[i];
		arr[i] = max;
		arr[index] = temp;
	}
	// 返回中位数
	median = arr[size / 2];
	free(arr);
	return median;
}
// 中值滤波
// 模板的默认大小为3*3
void MedF(int width = 3,int height = 3)
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	// 计算灰度图像实际位图数据每行的字节数
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// 计算灰度图像的内存量
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// 如果是第一次滤波，没有初始化的滤波基础图
	if(NULL == filt_lpBitsInfo)
	{
		// 分配初始内存
		filt_lpBitsInfo = (BITMAPINFO*)malloc(size);
		// 复制灰度图像数据
		memcpy(filt_lpBitsInfo,lpBitsInfo,size);
	}
	// 复制图像
	BITMAPINFO* filt_lpBitsInfo_copy = (BITMAPINFO*)malloc(size);
	memcpy(filt_lpBitsInfo_copy,filt_lpBitsInfo,size);
	/*
	// 计算可进行模板运算的范围
	int filt_w  = w  - ( w  % width );
	int filt_h  = h  - ( h  % height );
	*/
	// 计算复制的灰度图像的位图数据的起始点
	BYTE* filt_lpBits = (BYTE*)&filt_lpBitsInfo->bmiColors[filt_lpBitsInfo->bmiHeader.biClrUsed];
	BYTE* filt_lpBits_copy = (BYTE*)&filt_lpBitsInfo_copy->bmiColors[filt_lpBitsInfo_copy->bmiHeader.biClrUsed];
	int i,j;
	BYTE* pixel,* filt_pixel;
	// 对复制的图像进行均值滤波
	
	for(i = height / 2;i < h - height / 2;i++)
	{
		for(j = width / 2;j < w - width / 2;j++)
		{
			pixel = filt_lpBits_copy + LineBytes * (h -1 -i) + j;
			filt_pixel = filt_lpBits + LineBytes * (h -1 -i) + j;
			*filt_pixel = Median(pixel,width,height);
		}
	}
	// 释放内存
	free(filt_lpBitsInfo_copy);
}

// 梯度锐化模板如下
BYTE Gradient(BYTE* pixel)
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	int gradient = (*pixel - *(pixel + LineBytes)) * (*pixel - *(pixel + LineBytes));
 	gradient += (*pixel - *(pixel + 1)) * (*pixel - *(pixel + 1));
	gradient = (int)(sqrt(gradient) + 0.5);
	if(gradient > 255)
		gradient = 255;
	return gradient;
}
// 梯度锐化
void G_Sharp()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	// 计算灰度图像实际位图数据每行的字节数
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// 计算灰度图像的内存量
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// 如果是第一次滤波，没有初始化的滤波基础图
	if(NULL == filt_lpBitsInfo)
	{
		// 分配初始内存
		filt_lpBitsInfo = (BITMAPINFO*)malloc(size);
		// 复制灰度图像数据
		memcpy(filt_lpBitsInfo,lpBitsInfo,size);
	}
	BYTE* filt_lpBits = (BYTE*)&filt_lpBitsInfo->bmiColors[filt_lpBitsInfo->bmiHeader.biClrUsed];
	int i,j;
	BYTE* pixel,* filt_pixel;
	for(i = 1; i < h ; i++)
	{
		for(j = 0; j < w - 1 ; j++)
		{
			pixel = lpBits + LineBytes * (h-1-i) + j;
			filt_pixel = filt_lpBits + LineBytes * (h-1-i) + j;
			*filt_pixel = Gradient(pixel);
		}
	}
}


// 拉普拉斯锐化模板
BYTE Laplacian(BYTE* pixel)
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	int lap = 5 * (*pixel) - ( *(pixel - LineBytes) + *(pixel + LineBytes) + 
			                   *(pixel - 1) + *(pixel + 1));
	return lap;
}

// 拉普拉斯锐化
void L_Sharp()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	// 计算灰度图像实际位图数据每行的字节数
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// 计算灰度图像的内存量
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// 如果是第一次滤波，没有初始化的滤波基础图
	if(NULL == filt_lpBitsInfo)
	{
		// 分配初始内存
		filt_lpBitsInfo = (BITMAPINFO*)malloc(size);
		// 复制灰度图像数据
		memcpy(filt_lpBitsInfo,lpBitsInfo,size);
	}
	BYTE* filt_lpBits = (BYTE*)&filt_lpBitsInfo->bmiColors[filt_lpBitsInfo->bmiHeader.biClrUsed];
	int i,j;
	BYTE* pixel,* filt_pixel;
	for(i = 1; i < h - 1; i++)
	{
		for(j = 1; j < w - 1 ; j++)
		{
			pixel = lpBits + LineBytes * (h-1-i) + j;
			filt_pixel = filt_lpBits + LineBytes * (h-1-i) + j;
			*filt_pixel = Laplacian(pixel);
		}
	}
}
void FFT_Filter(int D)
{
	int w = lpDIB_FT->bmiHeader.biWidth;
	int h = lpDIB_FT->bmiHeader.biHeight;
	int LineBytes = (w * lpDIB_FT->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];
	
	// 复制频域数据
	complex<double>* origin_FD = new complex<double>[w * h];
	int i,j;
	for(i = 0;i < w * h; i++)
		origin_FD[i] = gFD[i];

	double dis;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			// 计算改点到零频点的距离
			dis = sqrt((i - h / 2) * (i - h / 2) + (j - w / 2 ) * (j - w / 2) + 1);
			// 如果大于0，则说明是高通滤波
			if(D > 0)
			{
				// 直接截断
				if(dis > D)
					gFD[i * w + j] = 0;
			}
			// 如果小于0，则说明是低通滤波
			else
			{
				// 直接截断
				if(dis <= -D)
					gFD[i * w + j] = 0;
			}
		}
	}

	// 更改傅里叶反变换后的数据
	double temp;
	BYTE* pixel;
	for(i = 0; i < h; i++) 
	{
		for(j = 0; j < w; j++) 
		{
			temp = sqrt(gFD[j * h + i].real() * gFD[j * w + i].real() + 
				        gFD[j * h + i].imag() * gFD[j * w + i].imag()) *2000;
			if (temp > 255)
				temp = 255;
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			*pixel = (BYTE)(temp);
		}
	}
	// 进行傅里叶反变换
	IFFourier();
	// 还原傅里叶变换后的数据
	delete gFD;
	gFD = origin_FD;
}

void FFT_BATTER_Filter(int D,int N)
{
	int w = lpDIB_FT->bmiHeader.biWidth;
	int h = lpDIB_FT->bmiHeader.biHeight;
	int LineBytes = (w * lpDIB_FT->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];
	
	// 复制傅里叶变化的数据
	complex<double>* origin_FD = new complex<double>[w * h];
	int i,j;
	for(i = 0;i < w * h; i++)
		origin_FD[i] = gFD[i];

	double dis;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			// 计算改点到零频点的距离
			dis = sqrt((i - h / 2) * (i - h / 2) + (j - w / 2 ) * (j - w / 2) + 1);
			// 如果大于0，则乘以低通滤波器函数
			if(D > 0)
			{
				gFD[i * w + j] = gFD[i * w + j] * (1 / (1 + pow(dis / D,2 * N)));
			}
			// 如果小于等于0，则乘以高通滤波器函数
			else
			{
				gFD[i * w + j] = gFD[i * w + j] * (1 / (1 + pow(D / dis,2 * N)));
			}
		}
	}

	double temp;
	BYTE* pixel;
	for(i = 0; i < h; i++) 
	{
		for(j = 0; j < w; j++) 
		{
			temp = sqrt(gFD[j * h + i].real() * gFD[j * w + i].real() + 
				        gFD[j * h + i].imag() * gFD[j * w + i].imag()) *2000;
			if (temp > 255)
				temp = 255;
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			*pixel = (BYTE)(temp);
		}
	}
	IFFourier();

	delete gFD;
	gFD = origin_FD;
}

// 高斯滤波器
void FFT_GAUSS_Filter(int D)
{
	int w = lpDIB_FT->bmiHeader.biWidth;
	int h = lpDIB_FT->bmiHeader.biHeight;
	int LineBytes = (w * lpDIB_FT->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];
	
	// 复制傅里叶变化的数据
	complex<double>* origin_FD = new complex<double>[w * h];
	int i,j;
	for(i = 0;i < w * h; i++)
		origin_FD[i] = gFD[i];

	double dis;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			// 计算该点到零频点距离
			dis = sqrt((i - h / 2) * (i - h / 2) + (j - w / 2 ) * (j - w / 2) + 1);
			// 如果大于0，则乘以低通滤波器函数
			if(D > 0)
			{
				gFD[i * w + j] = gFD[i * w + j] * exp(-pow(dis,2) / (2 * pow(D,2)));
			}
			// 如果小于等于0，则乘以高通滤波器函数
			else
			{
				gFD[i * w + j] = gFD[i * w + j] * (1 - exp(-pow(dis,2) / (2 * pow(D,2))));
			}
		}
	}

	double temp;
	BYTE* pixel;
	for(i = 0; i < h; i++) 
	{
		for(j = 0; j < w; j++) 
		{
			temp = sqrt(gFD[j * h + i].real() * gFD[j * w + i].real() + 
				        gFD[j * h + i].imag() * gFD[j * w + i].imag()) *2000;
			if (temp > 255)
				temp = 255;
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			*pixel = (BYTE)(temp);
		}
	}
	IFFourier();

	delete gFD;
	gFD = origin_FD;

}
