#include "stdafx.h"
#include <complex>
using namespace std;
#define PI 3.1415926535

BITMAPINFO* lpBitsInfo = NULL;
// ָ�����任�ͷ��任������
BITMAPINFO* lpDIB_FT = NULL;
BITMAPINFO* lpDIB_IFT = NULL;
// ָ��Ƶ������
complex<double>* gFD = NULL;
// ָ���ֵ�˲��������
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

// 16ɫ��24λ��ʺ�256ɫ��ɫת�Ҷ�ͼ��
void gray()
{
	if(NULL == lpBitsInfo)
		return;
	int biBitCount = lpBitsInfo->bmiHeader.biBitCount;
	// ����2ֵͼ��
	if(1 == biBitCount)
		return;
	// �����16ɫͼ�񣬵���gray4()ת�Ҷ�
	if(4 == biBitCount)
		gray4();
	// �����8λͼ���Ҳ��ǻҶ�ͼ����ôΪ256ɫ��ɫ������gray3()ת�Ҷ�
	else if(8 == biBitCount && !IsGray())
		gray3();
	// �����24λ���ͼ�񣬵���gray2()ת�Ҷ�
	else if(24 == biBitCount)
		gray2();
}

// 24λ���ת�Ҷ�
void gray2()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	/*
	// ��һ�ַ�������ٵ��޸�
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

	// �ڶ��ַ�������ʵ���޸�

	// ����Ҷ�ͼ��ʵ��λͼ����ÿ�е��ֽ���
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// ����Ҷ�ͼ����ڴ���
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// ���Ҷ�ͼ������ڴ�
	BITMAPINFO* gray_lpBitsInfo = (BITMAPINFO*)malloc(size);
	// ��д��Ϣͷ�͸�д��Ϣͷ
	memcpy(gray_lpBitsInfo,lpBitsInfo,40);
	gray_lpBitsInfo->bmiHeader.biBitCount = 8;
	gray_lpBitsInfo->bmiHeader.biClrUsed = 256;
	// ��д��ɫ��
	int i,j;
	for(i = 0;i < 256;i++)
	{
		gray_lpBitsInfo->bmiColors[i].rgbBlue = i;
		gray_lpBitsInfo->bmiColors[i].rgbGreen = i;
		gray_lpBitsInfo->bmiColors[i].rgbRed = i;
		// ������
		gray_lpBitsInfo->bmiColors[i].rgbReserved = 0; 
	}

	// ��дʵ��λͼ����

	BYTE *R,*G,*B,*pixel,avg;
	// ʵ��λͼ���ݵ���ʼ��
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
	// ��������
	free(lpBitsInfo);
	lpBitsInfo = gray_lpBitsInfo;
}


// 256ɫͼ��ת�Ҷ�ͼ��
void gray3()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	/*
	// ��һ�ַ�����ֱ���޸ĵ�ɫ�������	
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
	
	// �ڶ��֣���ʵ�޸�Ϊ�Ҷ�ͼ��

	// ����Ҷ�ͼ��ʵ��λͼ����ÿ�е��ֽ���
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// ����Ҷ�ͼ����ڴ���
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// ���Ҷ�ͼ������ڴ�
	BITMAPINFO* gray_lpBitsInfo = (BITMAPINFO*)malloc(size);
	// ��д��Ϣͷ�͸�д��Ϣͷ
	memcpy(gray_lpBitsInfo,lpBitsInfo,40);
	// ��д��ɫ��
	int i,j;
	for(i = 0;i < 256;i++)
	{
		gray_lpBitsInfo->bmiColors[i].rgbBlue = i;
		gray_lpBitsInfo->bmiColors[i].rgbGreen = i;
		gray_lpBitsInfo->bmiColors[i].rgbRed = i;
		// ������
		gray_lpBitsInfo->bmiColors[i].rgbReserved = 0; 
	}

	// ��дʵ��λͼ����

	BYTE *pixel,avg,b,g,r,index;
	// ʵ��λͼ���ݵ���ʼ��
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
	// ��������
	free(lpBitsInfo);
	lpBitsInfo = gray_lpBitsInfo;
}
// 16ɫת�Ҷ�
void gray4()
{
	/*
	// ������256ɫת�Ҷȵĵ�һ�ַ���
	// ֱ���޸ĵ�ɫ�������
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

	// �ڶ��ַ�������ʵ�޸�Ϊ�Ҷ�ͼ��
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	// ����Ҷ�ͼ��ʵ��λͼ����ÿ�е��ֽ���
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// ����Ҷ�ͼ����ڴ���
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// ���Ҷ�ͼ������ڴ�
	BITMAPINFO* gray_lpBitsInfo = (BITMAPINFO*)malloc(size);
	// ��д��Ϣͷ�͸�д��Ϣͷ
	memcpy(gray_lpBitsInfo,lpBitsInfo,40);
	gray_lpBitsInfo->bmiHeader.biBitCount = 8;
	gray_lpBitsInfo->bmiHeader.biClrUsed = 256;
	// ��д��ɫ��
	int i,j;
	for(i = 0;i < 256;i++)
	{
		gray_lpBitsInfo->bmiColors[i].rgbBlue = i;
		gray_lpBitsInfo->bmiColors[i].rgbGreen = i;
		gray_lpBitsInfo->bmiColors[i].rgbRed = i;
		// ������
		gray_lpBitsInfo->bmiColors[i].rgbReserved = 0; 
	}

	// ��дʵ��λͼ����
	BYTE *pixel,avg,b,g,r,index;
	// ʵ��λͼ���ݵ���ʼ��
	BYTE* gray_lpBits = (BYTE*)&gray_lpBitsInfo->bmiColors[gray_lpBitsInfo->bmiHeader.biClrUsed];
		
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j / 2;
			// ������ڸ�λ����
			if(j % 2 == 0)
				index = *pixel / 16;
			// ������ڵ�λ����
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
	// ��������
	free(lpBitsInfo);
	lpBitsInfo = gray_lpBitsInfo;
}
// �ж�һ��ͼ���Ƿ�Ϊ�Ҷ�ͼ��
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

// �������ص㺯��
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
	// ��������
	BYTE index;
	switch(lpBitsInfo->bmiHeader.biBitCount)
	{
	case 1:
		bv = *(lpBits + LineBytes * (h - 1 - i) + j / 8) & (1 << (7 - j % 8));
		if(0 == bv)
			strcpy(str, "������");
		else
			strcpy(str, "ǰ����");
		break;
	case 4:
		pixel = lpBits + LineBytes * (h - 1 - i) + j / 2;
		// ������ڸ�λ����
		if(j % 2 == 0)
			index = *pixel / 16;
		// ������ڵ�λ����
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
			sprintf(str, "����ֵ: %d", *pixel);
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
// �Ҷ�ͼ��ѡ��H[0]Ϊֱ��ͼ����
// ��ɫͼ���У�RΪH[0],GΪH[1],BΪH[2]
DWORD H[3][256];
void Histogram1(); // 16ɫ��ֱ��ͼ���㺯��
void Histogram2(); // 256�Ҷȵ�ֱ��ͼ���㺯��
void Histogram3(); // 256��ɫ��ֱ��ͼ���㺯��
void Histogram4(); // 24λ��ʵ�ֱ��ͼ���㺯��
void Histogram()
{
	// ��ָ��������й���
	if(NULL == lpBitsInfo)
		return;
	// �Զ�ֵͼ����й���
	else if(1 == lpBitsInfo->bmiHeader.biBitCount)
		return;
	// 16ɫͼ���ֱ��ͼ���㺯��ӳ��
	else if(4 == lpBitsInfo->bmiHeader.biBitCount)
		Histogram1();
	// 256�Ҷ�ͼ���ֱ��ͼ���㺯��ӳ��
	else if(8 == lpBitsInfo->bmiHeader.biBitCount && IsGray())
		Histogram2();
	// 256��ɫͼ���ֱ��ͼ���㺯��ӳ��
	else if(8 == lpBitsInfo->bmiHeader.biBitCount && !IsGray())
		Histogram3();
	// 24λ���ͼ���ֱ��ͼ���㺯��ӳ��
	else if(24 == lpBitsInfo->bmiHeader.biBitCount)
		Histogram4();
}
// 16ɫͼ���ֱ��ͼ���㺯��
void Histogram1()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// ��ʼ��ֱ��ͼԪ��Ϊ0
	int i,j;
	for(i = 0;i < 3;i++)
	{
		for(j = 0;j < 256;j++)
			H[i][j] = 0;
	}
	// ����ֱ��ͼ
	BYTE *pixel,b,g,r,index;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j / 2;
			// ������ڸ�λ����
			if(j % 2 == 0)
				index = *pixel / 16;
			// ������ڵ�λ����
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
// 256�Ҷȵ�ֱ��ͼ���㺯��
void Histogram2()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// ֻʹ�õ�ֱ��ͼ����ĵ�һ��ͨ������
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
// 256��ɫͼ���ֱ��ͼ���㺯��
void Histogram3()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	// ��ʼ��ֱ��ͼԪ��Ϊ0
	int i,j;
	for(i = 0;i < 3;i++)
	{
		for(j = 0;j < 256;j++)
			H[i][j] = 0;
	}
	// ��ȡʵ������,����ֱ��ͼ����
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
	// ��ʼ��ֱ��ͼԪ��Ϊ0
	int i,j;
	for(i = 0;i < 3;i++)
	{
		for(j = 0;j < 256;j++)
			H[i][j] = 0;
	}
	// ��ȡʵ�����ݣ�����ֱ��ͼ����
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

// ���Լ��㺯��
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
	// ��ָ��������й���
	if(NULL == lpBitsInfo)
		return;
	// �Զ�ֵͼ����й���
	else if(1 == lpBitsInfo->bmiHeader.biBitCount)
		return;
	// 16ɫͼ������Լ��㺯��ӳ��
	else if(4 == lpBitsInfo->bmiHeader.biBitCount)
		LineTrans1(a,b);
	// 256�Ҷ�ͼ������Լ��㺯��ӳ��
	else if(8 == lpBitsInfo->bmiHeader.biBitCount && IsGray())
		LineTrans2(a,b);
	// 256��ɫͼ������Լ��㺯��ӳ��
	else if(8 == lpBitsInfo->bmiHeader.biBitCount && !IsGray())
		LineTrans3(a,b);
	// 24λ���ͼ������Լ��㺯��ӳ��
	else if(24 == lpBitsInfo->bmiHeader.biBitCount)
		LineTrans4(a,b);
}

// 16ɫ��ɫ�����Լ��㺯��
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
// 256�Ҷȵ����Լ��㺯��
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
// 256��ɫ�����Լ��㺯��
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
// 24λ��ʵ����Լ��㺯��
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
	// ��ָ��������й���
	if(NULL == lpBitsInfo)
		return;
	// �Զ�ֵͼ����й���
	else if(1 == lpBitsInfo->bmiHeader.biBitCount)
		return;
	// 16ɫͼ���ֱ��ͼ���⻯����ӳ��
	else if(4 == lpBitsInfo->bmiHeader.biBitCount)
		Equalize1();
	// 256�Ҷ�ͼ���ֱ��ͼ���⻯����ӳ��
	else if(8 == lpBitsInfo->bmiHeader.biBitCount && IsGray())
		Equalize2();
	// 256��ɫͼ���ֱ��ͼ���⻯����ӳ��
	else if(8 == lpBitsInfo->bmiHeader.biBitCount && !IsGray())
		Equalize3();
	// 24λ���ͼ���ֱ��ͼ���⻯����ӳ��
	else if(24 == lpBitsInfo->bmiHeader.biBitCount)
		Equalize4();
}

// 16ɫֱ��ͼ���⻯
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

	// ��ÿһ��ͨ�����м���ӳ������Ĵ���
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
	// ����ӳ��ֵ�ı��ɫ�������ͨ����ֵ
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
// 256ɫ�Ҷ�ֱ��ͼ���⻯
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
		// �����ۼ�ֵ
		temp = 0;
		for(j = 0;j <= i;j++)
			temp += H[0][j];
		// ���㹫ʽ��(int)((�ۼ�ֵ/�����ص���)*�������ֵ+0.5)
		Map[i] = (int)(temp * 255 / (w * h) + 0.5);
	}
	// ������ֵ����ӳ��ı䴦��
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h -1 -i) + j;
			*pixel = Map[*pixel];
		}
	}
}
// 256ɫ��ɫֱ��ͼ���⻯
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
	// ��ÿһ��ͨ�����м���ӳ������Ĵ���
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
	// ����ӳ��ֵ�ı��ɫ�������ͨ����ֵ
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
// 24λ���ͼ��ֱ��ͼ���⻯
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

	// ��ÿһ��ͨ�����м���ӳ������Ĵ���
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

	// ����ӳ��ֵ�ı��������ݵ�����ͨ����ֵ
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


// ʱ��ռ�ת��ΪƵ��ռ�
void FT(complex<double>* TD,complex<double>* FD,int m)
{
	int x,u;
	double angle;
	// һ��Ƶ��ռ��ֵ��Ҫ����ʱ��ռ��ֵ������
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

// Ƶ��ռ�תΪʱ��ռ�
void IFT(complex<double>*FD,complex<double>*TD,int m)
{
	int x,u;
	double angle;
	// һ��ʱ��ռ��ֵ��Ҫ����Ƶ��ռ��ֵ������
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

	// ��ʱ������ת��Ϊ������ʽ�洢
	int i,j;
	BYTE* pixel;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h -i - 1) + j;
			// ����pow(-1,i+j)��ʵ��ת���������任
			TD[w * i + j] = complex<double>(*pixel * pow(-1,i + j),0);
		}
	}
	// ˮƽ����������Ҷ�仯
	for(i = 0;i < h;i++)
		FT(&TD[i * w],&FD[w * i],w);

	// ��ת�ò���
	for(i = 0;i < h;i++)
		for(j = 0;j < w;j++)
			TD[h * j + i] = FD[w * i + j];

	// ��ֱ����������Ҷ�仯
	for(i = 0;i < w;i++)
		FT(&TD[h * i],&FD[h * i],h);

	// ����ͼ������
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
			// �������ֵ,�Ŵ����ֵ������ֵ
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

// ����Ҷ���任
void IFourier() {
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	complex<double>* TD = new complex<double>[w*h];
	complex<double>* FD = new complex<double>[w*h];

	int i,j;
	BYTE * pixel;
	// ת��
	for (i=0;i<h;i++) {
		for(j = 0; j < w ; j++) {
			FD[w * i + j] = gFD[i + h * j];
		}
	}
	// ˮƽ������������Ҷ���任
	for (i = 0; i < h; i++) {
		IFT(&FD[w * i],&TD[w * i],w);
	}
	// ת��
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			FD[h * j + i] = TD[w * i + j];
		}
	}
	// ��ֱ������������Ҷ�任
	for (i = 0; i < w; i++) {
		IFT(&FD[h * i], &TD[h * i], h);
	}

	LONG size = 40 + 1024 + LineBytes * h;

	lpDIB_IFT = (BITMAPINFO*)malloc(size);
	if (NULL == lpDIB_IFT) return;
	memcpy(lpDIB_IFT,lpBitsInfo,size);

	lpBits = (BYTE*)&lpDIB_IFT->bmiColors[lpDIB_IFT->bmiHeader.biClrUsed];
	// ������ֵת��Ϊʵ��ֵ�������
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
	// ���㸶��Ҷ�任����
	LONG count = 1 << r;
	// �����Ȩϵ��
	int i;
	double angle;
	complex<double>* W = new complex<double>[count / 2];
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	// ��ʱ���д��X1
	complex<double>* X1 = new complex<double>[count];
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// ���õ����㷨���п��ٸ���Ҷ�任�����ΪƵ��ֵX2
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
	
	// ����������λ�������У�
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
	
	// �ͷ��ڴ�
	delete W;
	delete X1;
	delete X2;
}

void FFourier()
{
	//ͼ��Ŀ�Ⱥ͸߶�
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	//ָ��ͼ������ָ��
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ�����������������
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ�����������������
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// �����ڴ�
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	BYTE* pixel;
	
	for(i = 0; i < FFT_h; i++)  // ��
	{
		for(j = 0; j < FFT_w; j++)  // ��
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// ��ʱ��ֵ
			TD[j + FFT_w * i] = complex<double>(*pixel* pow(-1,i+j), 0);
		}
	}
	
	for(i = 0; i < FFT_h; i++)
	{
		// ��y������п��ٸ���Ҷ�任
		FFT(&TD[FFT_w * i], &FD[FFT_w * i], wp);
	}
	
	// �����м�任���
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			TD[i + FFT_h * j] = FD[j + FFT_w * i];
		}
	}
	
	for(i = 0; i < FFT_w; i++)
	{
		// ��x������п��ٸ���Ҷ�任
		FFT(&TD[i * FFT_h], &FD[i * FFT_h], hp);
	}

	//����Ƶ��ͼ��
	//ΪƵ��ͼ������ڴ�
	LONG size = 40 + 1024 + LineBytes * height;
	lpDIB_FT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_FT)
		return;
	memcpy(lpDIB_FT, lpBitsInfo, size);

	//ָ��Ƶ��ͼ������ָ��
	lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];

	double temp;
	for(i = 0; i < FFT_h; i++) // ��
	{
		for(j = 0; j < FFT_w; j++) // ��
		{
			// ����Ƶ�׷���
			temp = sqrt(FD[j * FFT_h + i].real() * FD[j * FFT_h + i].real() + 
				        FD[j * FFT_h + i].imag() * FD[j * FFT_h + i].imag()) *2000;
			
			// �ж��Ƿ񳬹�255
			if (temp > 255)
			{
				// ���ڳ����ģ�ֱ������Ϊ255
				temp = 255;
			}
			
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// ����Դͼ��
			*pixel = (BYTE)(temp);
		}
	}

	delete TD;
	gFD = FD;
}

//IFFT���任
void IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	// ����Ҷ�任����
	LONG	count;
	// ���㸶��Ҷ�任����
	count = 1 << r;

	// ������������洢��
	complex<double> * X = new complex<double>[count];
	// ��Ƶ���д��X
	memcpy(X, FD, sizeof(complex<double>) * count);
	
	// ����
	for(int i = 0; i < count; i++)
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	
	// ���ÿ��ٸ���Ҷ�任
	FFT(X, TD, r);
	
	// ��ʱ���Ĺ���
	for(i = 0; i < count; i++)
		TD[i] = complex<double> (TD[i].real() * count, -TD[i].imag() * count);
	
	// �ͷ��ڴ�
	delete X;
}

void IFFourier()
{
	//ͼ��Ŀ�Ⱥ͸߶�
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ�����������������
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ�����������������
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// �����ڴ�
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	for(i = 0; i < FFT_h; i++)  // ��
		for(j = 0; j < FFT_w; j++)  // ��
			FD[j + FFT_w * i] = gFD[i + FFT_h*j];
	
	// ��ˮƽ������п��ٸ���Ҷ�任
	for(i = 0; i < FFT_h; i++)
		IFFT(&FD[FFT_w * i], &TD[FFT_w * i], wp);
	
	// �����м�任���
	for(i = 0; i < FFT_h; i++)
		for(j = 0; j < FFT_w; j++)
			FD[i + FFT_h * j] = TD[j + FFT_w * i];
	
	// �ش�ֱ������п��ٸ���Ҷ�任
	for(i = 0; i < FFT_w; i++)
		IFFT(&FD[i * FFT_h], &TD[i * FFT_h], hp);

	//Ϊ���任ͼ������ڴ�
	LONG size = 40 + 1024 + LineBytes * height;

	lpDIB_IFT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_IFT)
		return;
	memcpy(lpDIB_IFT, lpBitsInfo, size);

	//ָ�򷴱任ͼ������ָ��
	BYTE* lpBits = (BYTE*)&lpDIB_IFT->bmiColors[256];
	BYTE* pixel;
	double temp;
	for(i = 0; i < FFT_h; i++) // ��
	{
		for(j = 0; j < FFT_w; j++) // ��
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

	// ɾ����ʱ����
	delete FD;
	delete TD;
	delete gFD;
	gFD = NULL;
}

// ��ֵ�˲���ģ������
BYTE Mean(BYTE *pixel,int width,int height)
{
	// ÿ�еĳ���
	int w = lpBitsInfo->bmiHeader.biWidth;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	// ������Ԫ�صĺ�
	int i,j,sum;
	BYTE mean;
	sum = 0;
	for(i = - height / 2;i < height - height / 2;i++)
		for(j = - width / 2;j < width - width / 2;j++)
			sum += *(pixel + i * LineBytes + j);
	// ���ֵ
	mean = BYTE((sum * 1.0) / (height * width) + 0.5);
	// �ı�����ֵΪƽ��ֵ
	return mean;
}
// ��ֵ�˲�
// ģ���Ĭ�ϴ�СΪ3*3
void MeanF(int width = 3,int height = 3)
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	// ����Ҷ�ͼ��ʵ��λͼ����ÿ�е��ֽ���
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// ����Ҷ�ͼ����ڴ���
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// ����ǵ�һ���˲���û�г�ʼ�����˲�����ͼ
	if(NULL == filt_lpBitsInfo)
	{
		// �����ʼ�ڴ�
		filt_lpBitsInfo = (BITMAPINFO*)malloc(size);
		// ���ƻҶ�ͼ������
		memcpy(filt_lpBitsInfo,lpBitsInfo,size);
	}
	// ����ͼ��
	BITMAPINFO* filt_lpBitsInfo_copy = (BITMAPINFO*)malloc(size);
	memcpy(filt_lpBitsInfo_copy,filt_lpBitsInfo,size);
	/*
	// ����ɽ���ģ������ķ�Χ
	int filt_w  = w  - ( w  % width );
	int filt_h  = h  - ( h  % height );*/
	// ���㸴�ƵĻҶ�ͼ���λͼ���ݵ���ʼ��
	BYTE* filt_lpBits = (BYTE*)&filt_lpBitsInfo->bmiColors[filt_lpBitsInfo->bmiHeader.biClrUsed];
	BYTE* filt_lpBits_copy = (BYTE*)&filt_lpBitsInfo_copy->bmiColors[filt_lpBitsInfo_copy->bmiHeader.biClrUsed];
	int i,j;
	BYTE* pixel,* filt_pixel;
	// �Ը��Ƶ�ͼ����о�ֵ�˲�
	for(i = height / 2;i < h - height / 2;i++)
	{
		for(j = width / 2;j < w - width / 2;j++)
		{
			pixel = filt_lpBits_copy + LineBytes * (h -1 -i) + j;
			filt_pixel = filt_lpBits + LineBytes * (h -1 -i) + j;
			*filt_pixel = Mean(pixel,width,height);
		}
	}
	// �ͷ��ڴ�
	free(filt_lpBitsInfo_copy);
}

// ��ֵ�˲�ģ��
BYTE Median(BYTE *pixel,int width,int height)
{
	// ÿ�еĳ���
	int w = lpBitsInfo->bmiHeader.biWidth;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	int size = width * height;
	int i,j,median;
	BYTE* arr = (BYTE*)malloc(size);
	for(i = -height / 2;i < height - height / 2;i++)
		for(j = -width / 2;j < width - width / 2;j++)
			*(arr + (i + height / 2) * width + (j + width / 2)) = *(pixel + i * LineBytes + j);
	// ð������
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
	// ������λ��
	median = arr[size / 2];
	free(arr);
	return median;
}
// ��ֵ�˲�
// ģ���Ĭ�ϴ�СΪ3*3
void MedF(int width = 3,int height = 3)
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	// ����Ҷ�ͼ��ʵ��λͼ����ÿ�е��ֽ���
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// ����Ҷ�ͼ����ڴ���
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// ����ǵ�һ���˲���û�г�ʼ�����˲�����ͼ
	if(NULL == filt_lpBitsInfo)
	{
		// �����ʼ�ڴ�
		filt_lpBitsInfo = (BITMAPINFO*)malloc(size);
		// ���ƻҶ�ͼ������
		memcpy(filt_lpBitsInfo,lpBitsInfo,size);
	}
	// ����ͼ��
	BITMAPINFO* filt_lpBitsInfo_copy = (BITMAPINFO*)malloc(size);
	memcpy(filt_lpBitsInfo_copy,filt_lpBitsInfo,size);
	/*
	// ����ɽ���ģ������ķ�Χ
	int filt_w  = w  - ( w  % width );
	int filt_h  = h  - ( h  % height );
	*/
	// ���㸴�ƵĻҶ�ͼ���λͼ���ݵ���ʼ��
	BYTE* filt_lpBits = (BYTE*)&filt_lpBitsInfo->bmiColors[filt_lpBitsInfo->bmiHeader.biClrUsed];
	BYTE* filt_lpBits_copy = (BYTE*)&filt_lpBitsInfo_copy->bmiColors[filt_lpBitsInfo_copy->bmiHeader.biClrUsed];
	int i,j;
	BYTE* pixel,* filt_pixel;
	// �Ը��Ƶ�ͼ����о�ֵ�˲�
	
	for(i = height / 2;i < h - height / 2;i++)
	{
		for(j = width / 2;j < w - width / 2;j++)
		{
			pixel = filt_lpBits_copy + LineBytes * (h -1 -i) + j;
			filt_pixel = filt_lpBits + LineBytes * (h -1 -i) + j;
			*filt_pixel = Median(pixel,width,height);
		}
	}
	// �ͷ��ڴ�
	free(filt_lpBitsInfo_copy);
}

// �ݶ���ģ������
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
// �ݶ���
void G_Sharp()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	// ����Ҷ�ͼ��ʵ��λͼ����ÿ�е��ֽ���
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// ����Ҷ�ͼ����ڴ���
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// ����ǵ�һ���˲���û�г�ʼ�����˲�����ͼ
	if(NULL == filt_lpBitsInfo)
	{
		// �����ʼ�ڴ�
		filt_lpBitsInfo = (BITMAPINFO*)malloc(size);
		// ���ƻҶ�ͼ������
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


// ������˹��ģ��
BYTE Laplacian(BYTE* pixel)
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	int lap = 5 * (*pixel) - ( *(pixel - LineBytes) + *(pixel + LineBytes) + 
			                   *(pixel - 1) + *(pixel + 1));
	return lap;
}

// ������˹��
void L_Sharp()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	// ����Ҷ�ͼ��ʵ��λͼ����ÿ�е��ֽ���
	int gray_LineBytes = (w * 8 + 31)/32 * 4;
	// ����Ҷ�ͼ����ڴ���
	int size = 40 + 256 * 4 + gray_LineBytes * h;
	// ����ǵ�һ���˲���û�г�ʼ�����˲�����ͼ
	if(NULL == filt_lpBitsInfo)
	{
		// �����ʼ�ڴ�
		filt_lpBitsInfo = (BITMAPINFO*)malloc(size);
		// ���ƻҶ�ͼ������
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
	
	// ����Ƶ������
	complex<double>* origin_FD = new complex<double>[w * h];
	int i,j;
	for(i = 0;i < w * h; i++)
		origin_FD[i] = gFD[i];

	double dis;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			// ����ĵ㵽��Ƶ��ľ���
			dis = sqrt((i - h / 2) * (i - h / 2) + (j - w / 2 ) * (j - w / 2) + 1);
			// �������0����˵���Ǹ�ͨ�˲�
			if(D > 0)
			{
				// ֱ�ӽض�
				if(dis > D)
					gFD[i * w + j] = 0;
			}
			// ���С��0����˵���ǵ�ͨ�˲�
			else
			{
				// ֱ�ӽض�
				if(dis <= -D)
					gFD[i * w + j] = 0;
			}
		}
	}

	// ���ĸ���Ҷ���任�������
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
	// ���и���Ҷ���任
	IFFourier();
	// ��ԭ����Ҷ�任�������
	delete gFD;
	gFD = origin_FD;
}

void FFT_BATTER_Filter(int D,int N)
{
	int w = lpDIB_FT->bmiHeader.biWidth;
	int h = lpDIB_FT->bmiHeader.biHeight;
	int LineBytes = (w * lpDIB_FT->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];
	
	// ���Ƹ���Ҷ�仯������
	complex<double>* origin_FD = new complex<double>[w * h];
	int i,j;
	for(i = 0;i < w * h; i++)
		origin_FD[i] = gFD[i];

	double dis;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			// ����ĵ㵽��Ƶ��ľ���
			dis = sqrt((i - h / 2) * (i - h / 2) + (j - w / 2 ) * (j - w / 2) + 1);
			// �������0������Ե�ͨ�˲�������
			if(D > 0)
			{
				gFD[i * w + j] = gFD[i * w + j] * (1 / (1 + pow(dis / D,2 * N)));
			}
			// ���С�ڵ���0������Ը�ͨ�˲�������
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

// ��˹�˲���
void FFT_GAUSS_Filter(int D)
{
	int w = lpDIB_FT->bmiHeader.biWidth;
	int h = lpDIB_FT->bmiHeader.biHeight;
	int LineBytes = (w * lpDIB_FT->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];
	
	// ���Ƹ���Ҷ�仯������
	complex<double>* origin_FD = new complex<double>[w * h];
	int i,j;
	for(i = 0;i < w * h; i++)
		origin_FD[i] = gFD[i];

	double dis;
	for(i = 0;i < h;i++)
	{
		for(j = 0;j < w;j++)
		{
			// ����õ㵽��Ƶ�����
			dis = sqrt((i - h / 2) * (i - h / 2) + (j - w / 2 ) * (j - w / 2) + 1);
			// �������0������Ե�ͨ�˲�������
			if(D > 0)
			{
				gFD[i * w + j] = gFD[i * w + j] * exp(-pow(dis,2) / (2 * pow(D,2)));
			}
			// ���С�ڵ���0������Ը�ͨ�˲�������
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
