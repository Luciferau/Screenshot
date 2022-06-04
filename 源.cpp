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
	//截谁？？？
	//截桌面
	//CWnd是表示窗口的祖先“类型”
	CWnd* desktopWnd = CWnd::GetDesktopWindow();
	//要想截图，想要找到被截图“设备”
	//窗口中有一个设备
	CDC * desktopDC =  desktopWnd->GetDC();//找到设备
	//截多大？？
	CRect re;//表示位置的大小
	//获取窗桌面口设备的大小
	desktopWnd->GetClientRect(&re);
	//定义一个图片(位图)
	
	//创建一个和指定“设备”（桌面窗口）相兼容的位图
	bmp.CreateCompatibleBitmap(desktopDC,re.Width(),re.Height());
	//还需要创建一个自己的设备
	//专门画图用的
	CDC myDC;//创建自己的设备
	myDC.CreateCompatibleDC(desktopDC); 
	//选择对象
	myDC.SelectObject(&bmp);
	//拷数据到我们的位图（bmp）
	myDC.BitBlt(0, 0, re.Width(), re.Height(),desktopDC,0,0,SRCCOPY);
	//准备画图
	BITMAP bitmapStruct;//位图结构信息
	bmp.GetBitmap(&bitmapStruct);
	//计算位图的实际大小
	DWORD size = bitmapStruct.bmWidthBytes * bitmapStruct.bmHeight;
	//分配内存，来存放位图信息
	 LPSTR lpdata = (LPSTR)GlobalAlloc(GPTR,size);
	 //图片头文件（文件头【14】+信息【40】头）
	 BITMAPINFOHEADER bitmapHead;
	 //位数
	 bitmapHead.biBitCount = 24;
	 bitmapHead.biClrImportant = 0;
	 bitmapHead.biCompression = BI_RGB;//三原色
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
	 bitmapFileHead.bfSize = 14 + 40 + size;//文件大小
	 bitmapFileHead.bfOffBits = 14 + 40;
	 //写文件
	 //定义文件名 2021-12-28 21：40-50.bmp
	CFile file;
	 CString filename =" infortamtion.bmp";
	 //CFile* file;
	 if (file.Open(filename, CFile::modeCreate | CFile::modeReadWrite))
	 {
		 //写文件
		 //先写文件信息头
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


 