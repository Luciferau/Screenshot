#include <afxwin.h>

/*Windows ���������Աֱ�ӷ���Ӳ����(���Ե���Ļ������һ��Ӳ���豸��
����ֱ�Ӳ�����Ļ���Ӳ���豸��
����ͨ����Ļ���Ӳ���豸�ġ��豸�����ġ���CDC, �豸����������������ĻӲ�� 
Ҳ����˵����� CDC �Ǻʹ��ڵ���ʾֱ�ӹ�����
��Ļ�ϵ�ÿһ������Ҳ����Ӧһ�� CDC,
���ڵ��豸������ CDC���൱��������ڵ����ݻ�������
������������Ĳ��������൱�ڶ����������Ļ���в���
���˴��ڱ���� CDC ֮�⣬�����Դ����Լ��� CDC��(ʹ�� CreateCompatibleDC�� 
�Լ������� CDC, ���������ڣ� 
�Ϳ��԰Ѻʹ��ڹ����� CDC�����浽�Լ������ CDC ����


hDeskTop = GetDesktopWindow(); 
hDeskTopDC = GetDC(hDeskTop); 
hMemDC = CreateCompatibleDC(hDeskTopDC);

*/

void jt() 
{
		// CWnd �� MFC �Ĵ�����ġ������ࡱ
		CWnd* desktopWnd = CWnd::GetDesktopWindow();
		//CDC �豸������(�������������롰�豸����ص�һ�����ͣ�
		CDC* desktopDC = desktopWnd->GetDC(); 
		//��ȡ���ڵĴ�С 
		CRect re; 
		desktopWnd->GetClientRect(&re);
		// CBitmap �� MFC ��һ���࣬�ṩ����λͼ�ĸ��ֹ��� 
		CBitmap bmp; //λͼ 
		//������ָ�����豸���ݵ�λͼ
		//���������洰���豸����ݵ�λͼ��ͬʱָ��λͼ�Ĵ�С
		bmp.CreateCompatibleBitmap(desktopDC, re.Width(), re.Height());
		//����һ���Լ����豸�����ģ��ʹ����豸���ݣ����ǲ��������ڣ�
		CDC myDC;
		//�Լ���������� CDC, ���������� 
		//�����豸���豸������(pdeskdc)�ͱ��浽����Լ�������CDC(memorydc)����
		myDC.CreateCompatibleDC(desktopDC);
	
		//ѡ�л��� 
		// ���Զ����豸ָ��λͼ
		myDC.SelectObject(&bmp);
		//����ͼ��
		myDC.BitBlt( 0, 0,//������Ŀ���豸��myDC���ľ��������ڵ����Ͻǵ��߼�λ�ã�x ���꣬y ���꣩ 
		re.Width(), // ����Դ��Ŀ�������ڵ��߼����
		re.Height(), // ����Դ��Ŀ�������ڵ��߼��߶�
		desktopDC, // ����Դ�豸
		0, 0, // Դ�豸��pdeskdc���ľ��������ڵ����Ͻǵ��߼�λ�ã�x ���꣬y ���꣩
		SRCCOPY); // ������ʽ��SRCCOPY���� ��Դ��������ֱ�ӿ�����Ŀ���������
		// BITMAP ��һ���ṹ�壬������ʾһ��λͼͼ��ľ�����Ϣ
		BITMAP bitmapStruct; bmp.GetBitmap(&bitmapStruct); //��ȡλͼ�ľ�����Ϣ�����浽λͼ�ṹ bit ��

			//���� ͼ���С����λ��byte��
		DWORD size = bitmapStruct.bmWidthBytes * bitmapStruct.bmHeight; //λͼͼ���С=λͼ ���*λͼ�߶� 
		//GlobalAlloc �Ӷ��з���һ����Ŀ���ֽ� 
		//���䷽ʽ GPTR�� ��ʾ����̶����ڴ棬�ҳ�ʼ��Ϊ 0������������ڴ�ĵ�ַ
		LPSTR lpdata = (LPSTR)GlobalAlloc(GPTR, size); 


		//�����Ǵ���һ�� bmp �ļ��ġ�λͼ��Ϣͷ��
		BITMAPINFOHEADER bitmapHead; //ָ����ʾ��ɫ��λ����1(�ڰ׶�ɫͼ), 4(16 ɫͼ), 8(256 ɫ), 24(���ɫͼ)
		bitmapHead.biBitCount = 24;
		bitmapHead.biClrImportant = 0; //ָ����ͼ������Ҫ����ɫ���������ֵΪ�㣬����Ϊ���� ����ɫ������Ҫ��
		bitmapHead.biCompression = BI_RGB; //λͼ��ѹ����ʽ��BI_RGB����ʾ��ѹ��
		bitmapHead.biHeight = bitmapStruct.bmHeight; //ָ��ͼ��ĸ߶ȣ���λ������ 
		bitmapHead.biWidth = bitmapStruct.bmWidth; //ָ��ͼ��Ŀ�ȣ���λ������ 

		bitmapHead.biSizeImage = size; //λͼ����ռ�õ��ֽ��� 
		bitmapHead.biPlanes = 1; // ������ 1 
		bitmapHead.biSize = sizeof(BITMAPINFOHEADER); //ָ������ṹ�ĳ��� 
		bitmapHead.biXPelsPerMeter = 0; //ָ��Ŀ���豸��ˮƽ�ֱ���
		bitmapHead.biYPelsPerMeter = 0; //ָ��Ŀ���豸�Ĵ�ֱ�ֱ���


		// ��λͼ�е����ݣ�ͬʱ���ݶ�Ӧ���豸���)����λͼ��Ϣ���Ƶ������������� 5)�� 
		GetDIBits( desktopDC->m_hDC,
		bmp, //Դλͼ 
		0, //�����Ƶ� DIB �еĵ�һ��ɨ���ߵı��
		bitmapHead.biHeight, //�����Ƶ�ɨ��������
		lpdata, //ָ��һ����������ָ�롣���������������װ�ز��� DIB ��ʽ����Ϣ 
		(BITMAPINFO*)&bitmapHead, //�� DIB �ĸ�ʽ����ɫ����˵����һ���ṹ
		DIB_RGB_COLORS); // ����ɫ����װ�� RGB ��ɫ

		BITMAPFILEHEADER bitmapFileHead; //λͼ���ļ�ͷ 
		bitmapFileHead.bfReserved1 = bitmapFileHead.bfReserved2 = 0; //����λ�� ����λ����Ϊ 0 
		//λͼ��𣬸��ݲ�ͬ�Ĳ���ϵͳ����ͬ���� Windows �У����ֶε�ֵ��Ϊ��BM��
		bitmapFileHead.bfType = ('M' << 8) | 'B'; 
		bitmapFileHead.bfSize = size + 54; //BMP ͼ���ļ��Ĵ�С(�ļ���Ϣͷ��14��+λͼ��Ϣͷ ��40����
		bitmapFileHead.bfOffBits = 54; //���ļ�ͷ��ʼ����ɫ���ݵ�ƫ����
		//д���ļ�
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
			file.Write(&bitmapFileHead, sizeof(BITMAPFILEHEADER)); //14 ���ֽ� 
			file.Write(&bitmapHead, sizeof(BITMAPINFOHEADER)); //40 ���ֽ� 
			file.Write(lpdata, size); //λͼ����ɫ����
			file.Close();
		}
		GlobalFree(lpdata);
}

int main(void)
{ 
	ShowWindow(GetConsoleWindow(), SW_MINIMIZE); //�ѿ���̨������С�� 
		Sleep(3000);
		jt(); 
		return 0;
}



