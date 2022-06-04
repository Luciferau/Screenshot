#include <afxwin.h>

/*Windows 不允许程序员直接访问硬件，(电脑的屏幕，就是一个硬件设备）
不能直接操作屏幕这个硬件设备，
而是通过屏幕这个硬件设备的“设备上下文”（CDC, 设备环境），来操作屏幕硬件 
也就是说，这个 CDC 是和窗口的显示直接关联的
屏幕上的每一个窗口也都对应一个 CDC,
窗口的设备上下文 CDC，相当于这个窗口的数据缓冲区，
对这个缓冲区的操作，就相当于对这个窗口屏幕进行操作
除了窗口本身的 CDC 之外，还可以创建自己的 CDC，(使用 CreateCompatibleDC） 
自己创建的 CDC, 不关联窗口， 
就可以把和窗口关联的 CDC，保存到自己定义的 CDC 中了


hDeskTop = GetDesktopWindow(); 
hDeskTopDC = GetDC(hDeskTop); 
hMemDC = CreateCompatibleDC(hDeskTopDC);

*/

void jt() 
{
		// CWnd 是 MFC 的窗口类的“祖先类”
		CWnd* desktopWnd = CWnd::GetDesktopWindow();
		//CDC 设备上下文(就是用来处理与“设备”相关的一个类型）
		CDC* desktopDC = desktopWnd->GetDC(); 
		//获取窗口的大小 
		CRect re; 
		desktopWnd->GetClientRect(&re);
		// CBitmap 是 MFC 的一个类，提供操作位图的各种功能 
		CBitmap bmp; //位图 
		//创建与指定的设备兼容的位图
		//创建于桌面窗口设备相兼容的位图，同时指定位图的大小
		bmp.CreateCompatibleBitmap(desktopDC, re.Width(), re.Height());
		//创建一个自己的设备上下文（和窗口设备兼容，但是不关联窗口）
		CDC myDC;
		//自己创建的这个 CDC, 不关联窗口 
		//窗口设备的设备上下文(pdeskdc)就保存到这个自己创建的CDC(memorydc)中了
		myDC.CreateCompatibleDC(desktopDC);
	
		//选中画笔 
		// 给自定义设备指定位图
		myDC.SelectObject(&bmp);
		//绘制图像
		myDC.BitBlt( 0, 0,//拷贝到目标设备（myDC）的矩形区域内的左上角的逻辑位置（x 坐标，y 坐标） 
		re.Width(), // 拷贝源在目标区域内的逻辑宽度
		re.Height(), // 拷贝源在目标区域内的逻辑高度
		desktopDC, // 拷贝源设备
		0, 0, // 源设备（pdeskdc）的矩形区域内的左上角的逻辑位置（x 坐标，y 坐标）
		SRCCOPY); // 拷贝方式（SRCCOPY）： 将源矩形区域直接拷贝到目标矩形区域。
		// BITMAP 是一个结构体，用来表示一个位图图像的具体信息
		BITMAP bitmapStruct; bmp.GetBitmap(&bitmapStruct); //获取位图的具体信息，保存到位图结构 bit 中

			//定义 图像大小（单位：byte）
		DWORD size = bitmapStruct.bmWidthBytes * bitmapStruct.bmHeight; //位图图像大小=位图 宽度*位图高度 
		//GlobalAlloc 从堆中分配一定数目的字节 
		//分配方式 GPTR， 表示分配固定的内存，且初始化为 0，并返回这块内存的地址
		LPSTR lpdata = (LPSTR)GlobalAlloc(GPTR, size); 


		//后面是创建一个 bmp 文件的“位图信息头”
		BITMAPINFOHEADER bitmapHead; //指定表示颜色的位数，1(黑白二色图), 4(16 色图), 8(256 色), 24(真彩色图)
		bitmapHead.biBitCount = 24;
		bitmapHead.biClrImportant = 0; //指定本图象中重要的颜色数，如果该值为零，则认为所有 的颜色都是重要的
		bitmapHead.biCompression = BI_RGB; //位图的压缩格式，BI_RGB：表示不压缩
		bitmapHead.biHeight = bitmapStruct.bmHeight; //指定图象的高度，单位是像素 
		bitmapHead.biWidth = bitmapStruct.bmWidth; //指定图象的宽度，单位是像素 

		bitmapHead.biSizeImage = size; //位图数据占用的字节数 
		bitmapHead.biPlanes = 1; // 必须是 1 
		bitmapHead.biSize = sizeof(BITMAPINFOHEADER); //指定这个结构的长度 
		bitmapHead.biXPelsPerMeter = 0; //指定目标设备的水平分辨率
		bitmapHead.biYPelsPerMeter = 0; //指定目标设备的垂直分辨率


		// 把位图中的数据（同时根据对应的设备句柄)，把位图信息复制到缓冲区（参数 5)中 
		GetDIBits( desktopDC->m_hDC,
		bmp, //源位图 
		0, //欲复制到 DIB 中的第一条扫描线的编号
		bitmapHead.biHeight, //欲复制的扫描线数量
		lpdata, //指向一个缓冲区的指针。这个缓冲区将用于装载采用 DIB 格式的信息 
		(BITMAPINFO*)&bitmapHead, //对 DIB 的格式及颜色进行说明的一个结构
		DIB_RGB_COLORS); // 在颜色表中装载 RGB 颜色

		BITMAPFILEHEADER bitmapFileHead; //位图的文件头 
		bitmapFileHead.bfReserved1 = bitmapFileHead.bfReserved2 = 0; //保留位， 这两位必须为 0 
		//位图类别，根据不同的操作系统而不同，在 Windows 中，此字段的值总为‘BM’
		bitmapFileHead.bfType = ('M' << 8) | 'B'; 
		bitmapFileHead.bfSize = size + 54; //BMP 图像文件的大小(文件信息头【14】+位图信息头 【40】）
		bitmapFileHead.bfOffBits = 54; //从文件头开始到颜色数据的偏移量
		//写入文件
		CFile file;
		CString strFileName;
		strFileName = _T("ScreenShot\\"); 
		CreateDirectory((LPCTSTR)strFileName, NULL); 
		CTime t = CTime::GetCurrentTime();
		CString tt = t.Format("%Y-%m-%d_%H-%M-%S"); 
		strFileName += tt;
		strFileName += _T(".jpg"); 
		if (file.Open((LPCTSTR)strFileName, CFile::modeCreate | CFile::modeWrite)) 
		{
			file.Write(&bitmapFileHead, sizeof(BITMAPFILEHEADER)); //14 个字节 
			file.Write(&bitmapHead, sizeof(BITMAPINFOHEADER)); //40 个字节 
			file.Write(lpdata, size); //位图的颜色数据
			file.Close();
		}
		GlobalFree(lpdata);
}

int main(void)
{ 
	ShowWindow(GetConsoleWindow(), SW_MINIMIZE); //把控制台窗口最小化 
		Sleep(3000);
		jt(); 
		return 0;
}



