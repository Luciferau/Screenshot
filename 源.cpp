#include <afxwin.h>
#include <iostream>
#include <Windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>

 CBitmap bmp;
void CutScreen();
 
void print();

void CutScreen()
{
	//��˭������
	//������
	//CWnd�Ǳ�ʾ���ڵ����ȡ����͡�
	CWnd* desktopWnd = CWnd::GetDesktopWindow();
	//Ҫ���ͼ����Ҫ�ҵ�����ͼ���豸��
	//��������һ���豸
	CDC * desktopDC =  desktopWnd->GetDC();//�ҵ��豸
	//�ض�󣿣�
	CRect re;//��ʾλ�õĴ�С
	//��ȡ��������豸�Ĵ�С
	desktopWnd->GetClientRect(&re);
	//����һ��ͼƬ(λͼ)
	
	//����һ����ָ�����豸�������洰�ڣ�����ݵ�λͼ
	bmp.CreateCompatibleBitmap(desktopDC,re.Width(),re.Height());
	//����Ҫ����һ���Լ����豸
	//ר�Ż�ͼ�õ�
	CDC myDC;//�����Լ����豸
	myDC.CreateCompatibleDC(desktopDC); 
	//ѡ�����
	myDC.SelectObject(&bmp);
	//�����ݵ����ǵ�λͼ��bmp��
	myDC.BitBlt(0, 0, re.Width(), re.Height(),desktopDC,0,0,SRCCOPY);
	//׼����ͼ
	BITMAP bitmapStruct;//λͼ�ṹ��Ϣ
	bmp.GetBitmap(&bitmapStruct);
	//����λͼ��ʵ�ʴ�С
	DWORD size = bitmapStruct.bmWidthBytes * bitmapStruct.bmHeight;
	//�����ڴ棬�����λͼ��Ϣ
	 LPSTR lpdata = (LPSTR)GlobalAlloc(GPTR,size);
	 //ͼƬͷ�ļ����ļ�ͷ��14��+��Ϣ��40��ͷ��
	 BITMAPINFOHEADER bitmapHead;
	 //λ��
	 bitmapHead.biBitCount = 24;
	 bitmapHead.biClrImportant = 0;
	 bitmapHead.biCompression = BI_RGB;//��ԭɫ
	 bitmapHead.biHeight = bitmapStruct.bmHeight;
	 bitmapHead.biWidth = bitmapStruct.bmWidth;
	 bitmapHead.biSizeImage = size;
	 bitmapHead.biPlanes = 1;
	 bitmapHead.biSize = sizeof(BITMAPINFOHEADER);
	 bitmapHead.biYPelsPerMeter = 0;
	 //LPSTR lpdata = (LPSTR)GlobalAlloc(GPTR, size);
	 GetDIBits(desktopDC->m_hDC,bmp,0,bitmapHead.biHeight,lpdata,(BITMAPINFO*)&bitmapHead,DIB_RGB_COLORS);
	 BITMAPFILEHEADER bitmapFileHead;
	 bitmapFileHead.bfReserved1 = 0;
	 bitmapFileHead.bfReserved2 = 0;
	 bitmapFileHead.bfType = 'M' << 8 | 'B';//MB
	 bitmapFileHead.bfSize = 14 + 40 + size;//�ļ���С
	 bitmapFileHead.bfOffBits = 14 + 40;
	 //д�ļ�
	 //�����ļ��� 2021-12-28 21��40-50.bmp
	CFile file;
	 CString filename =" infortamtion.bmp";
	 //CFile* file;
	 if (file.Open(filename, CFile::modeCreate | CFile::modeReadWrite))
	 {
		 //д�ļ�
		 //��д�ļ���Ϣͷ
		 file.Write(&bitmapFileHead, sizeof(bitmapFileHead));
		 file.Write(&bitmapHead, sizeof(bitmapHead));
		 file.Write(lpdata, size);
		 file.Close();
	}
	 GlobalFree(lpdata);
}

void Small ()
{
	ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
	 
	Sleep(2000);
}

void print()
{
	 ShowWindow(GetConsoleWindow(),SW_NORMAL);
	 printf("cut screen succeed!");
	system("start cut_screen.bmp ");

  
}


 