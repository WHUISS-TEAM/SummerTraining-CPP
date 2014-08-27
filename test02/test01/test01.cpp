// test01.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "test01.h"
#include <string>
#include <sstream>
#include <ctime> 
#include <cstdlib>
using std::string;
using std::ostringstream;
using std::istringstream;

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
HDC hdc,mdc;
HBITMAP hbmp;
HBITMAP fbmp;
DWORD tPre, tNow, tCheck;
BITMAP bm1, bm2;
HWND hWnd;

const int FRONT_WID = 45;
const int FRONT_HEI = 43;
int position_x = 301 - FRONT_WID;
int position_y = 135;

HBITMAP imageResource[7];
int index[10][8];

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void DrawPic(HDC hdc, int x, int y);
void TransparentBlt2(HDC hdc0, int nX0, int nY0, int nW0, int nH0, HDC hdc1, int nX1, int nY1, int nW1, int nH1, UINT Tcol);
void FrontPic(HDC hdc, int x, int y, int wid, int hei);
void loadImageResource();


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST01, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST01));

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST01));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TEST01);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//��������λ��

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, "OK!", WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
	   return FALSE;
   }

   MoveWindow(hWnd, 200, 100, 800, 700, true);//���Ը��Ĵ��ڵĴ�С
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   hdc = GetDC(hWnd);
   mdc = CreateCompatibleDC(hdc);

   hbmp = (HBITMAP)LoadImage(NULL, "./img/bg(03).bmp", IMAGE_BITMAP, 780,640, LR_LOADFROMFILE);//��һ����ֵ�ǿ��
   if (hbmp == NULL)
   {
	   MessageBox(NULL, "����ͼƬ����ʧ��!", "����", 0);
	   return FALSE;
   }
   DrawPic(hdc, 780, 640);
   
   
   loadImageResource();

   /*fbmp = (HBITMAP)LoadImage(NULL, "./img/5.bmp", IMAGE_BITMAP, FRONT_WID, FRONT_HEI, LR_LOADFROMFILE);

   
   if (fbmp == NULL)
   {
	   MessageBox(NULL, "ǰ��ͼƬ����ʧ��!", "����", 0);
	   ReleaseDC(hWnd, hdc);
	   DeleteDC(mdc);
	   DeleteObject(hbmp);
	   DeleteObject(fbmp);
	   return FALSE;
   }

   GetObject(fbmp, sizeof(BITMAP), &bm2);
   FrontPic(hdc, 301, 135,FRONT_WID,FRONT_HEI);*/
   //���̵Ŀ���ǣ�
//���̵ĸ߶��ǣ�
   return TRUE;
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		DrawPic(hdc, 780, 640);
		for (int i = 0; i < 10; i++){
			for (int j = 0; j < 8; j++){
				SelectObject(mdc, imageResource[index[i][j]]);
				position_x = position_x + FRONT_WID + 3;
				FrontPic(hdc, position_x, position_y, FRONT_WID, FRONT_HEI);
			}
			position_x = 301 - FRONT_WID;
			position_y += FRONT_HEI + 3;
		}
		//FrontPic(hdc, 301, 135, FRONT_WID, FRONT_HEI);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void DrawPic(HDC hdc, int wid, int hei)
{
	//���ϱ���ͼ
	SelectObject(mdc, hbmp);
	BitBlt(hdc, 0, 0, wid, hei, mdc, 0, 0, SRCCOPY);//����ͼƬ�����ʾ�ĳߴ�
	
}
void FrontPic(HDC hdc, int x, int y ,int wid, int hei){
	//����ǰ��ͼ
	//BitBlt(hdc, x, y, wid, hei, mdc, 0, 0, SRCCOPY);	//δ������ȥ�����������ͼЧ��
	TransparentBlt2(hdc, x, y, wid, hei, mdc, 0, 0, wid, hei, RGB(255, 255, 255));	//������ȥ�����������ͼЧ��
	//TransparentBlt3(hdc, x, y,wid, hei, mdc, 0, 0, 80, 60, RGB(255, 0, 255), 50);	//������ȥ������͸�����������ͼЧ��
	tPre = GetTickCount();
}

/*void DrawPic(HDC hdc, int x, int y){
	//����ǰ��ͼ
	SelectObject(mdc, fbmp);
	BitBlt(hdc, x, y, 80, 60, mdc, 0, 0, SRCCOPY);	//δ������ȥ�����������ͼЧ��
	//TransparentBlt2(hdc, x, y, 80, 60, mdc, 0, 0, 80, 60, RGB(255, 0, 255));	//������ȥ�����������ͼЧ��
	//TransparentBlt3(hdc, x, y, 80, 60, mdc, 0, 0, 80, 60, RGB(255, 0, 255), 50);	//������ȥ������͸�����������ͼЧ��
}*/

void TransparentBlt2(HDC hdc0, int nX0, int nY0, int nW0, int nH0, HDC hdc1, int nX1, int nY1, int nW1, int nH1, UINT Tcol)
{
	HBITMAP hBMP = CreateCompatibleBitmap(hdc0, nW0, nH0);
	HBITMAP mBMP = CreateBitmap(nW0, nH0, 1, 1, NULL);	//������ɫλͼ����,����ɫΪ��,������ɫΪ��
	HDC	hDC = CreateCompatibleDC(hdc0);
	HDC	mDC = CreateCompatibleDC(hdc0);
	SelectObject(hDC, hBMP);
	SelectObject(mDC, mBMP);	//�豸����mDC���浥ɫλͼ
	if ((nW0 == nW1) && (nH0 == nH1))	//��Դ�豸�������渴�Ƹ�hDC
	{
		BitBlt(hDC, 0, 0, nW0, nH0, hdc1, nX1, nY1, SRCCOPY);	//����С�����ֱ�Ӹ���
	}
	else
	{
		StretchBlt(hDC, 0, 0, nW0, nH0, hdc1, nX1, nY1, nW1, nH1, SRCCOPY);	//�������츴��
	}
	SetBkColor(hDC, Tcol);	//��hDC����ɫ���ó�Tcol��ɫ
	BitBlt(mDC, 0, 0, nW0, nH0, hDC, 0, 0, SRCCOPY);	//��hDC��λͼ���Ƹ��洢��ɫλͼ��mDC,Tcol��ɫ���ֱ�ɰ�ɫ

	SetBkColor(hDC, RGB(0, 0, 0));	//��hDC����ɫ���óɺ�ɫ
	SetTextColor(hDC, RGB(255, 255, 255));	//��hDC����(��ǰ��ɫ)���óɰ�ɫ
	BitBlt(hDC, 0, 0, nW0, nH0, mDC, 0, 0, SRCAND);	//��mDC��λͼ���Ƶ�hDC�в�����������

	SetBkColor(hdc0, RGB(255, 255, 255));	//����hdc0(Ŀ���豸����)����ɫΪ��ɫ
	SetTextColor(hdc0, RGB(0, 0, 0));	//����hdc0(Ŀ���豸����)����(��ǰ��ɫ)Ϊ��ɫ
	BitBlt(hdc0, nX0, nY0, nW0, nH0, mDC, 0, 0, SRCAND);	//��mDC�ڵ�ɫλͼ���Ƶ�hdc0(Ŀ���豸����)�в�����������
	BitBlt(hdc0, nX0, nY0, nW0, nH0, hDC, 0, 0, SRCPAINT);	//��hDC��λͼ���Ƶ�hdc0(Ŀ���豸����)�в����л�����
	DeleteDC(hDC);
	DeleteDC(mDC);
	DeleteObject(hBMP);
	DeleteObject(mBMP);
}


void loadImageResource(){
	int temp;
	enum imageResources{one = 1,two,three,four,five};
	char c[20] = "./img/";
	char a[20] = ".bmp";
	char position[20];
	 position_x = 301 - FRONT_WID;
	 position_y = 135;
	//bufdc = CreateCompatibleDC(hdc);

	for (int i = 0; i < 8; i++){
		ostringstream strout;
		istringstream strin;
		strout << c << i << a;
		strin.str(strout.str());
		strin >> position;
		imageResource[i] = (HBITMAP)LoadImage(NULL,position, IMAGE_BITMAP, FRONT_WID, FRONT_HEI, LR_LOADFROMFILE);
	}

	srand(unsigned(time(NULL)));
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 8; j++){
			temp = rand() % 8;
			index[i][j] = temp;
			//SelectObject(mdc, imageResource[temp]);
			//imageResource[i][j] = temp;
			//imageResources pic;
			//info = "./img/" + temp;
			//strout << c <<temp << a;
			//strin.str(strout.str());
			//strin >> position;
			//imageResource[i][j] = (HBITMAP)LoadImage(NULL, "./img/3.bmp", IMAGE_BITMAP, FRONT_WID, FRONT_HEI, LR_LOADFROMFILE);

			/*if (fbmp == NULL)
			{
				MessageBox(NULL, "ǰ��ͼƬ����ʧ��!", "����", 0);
				ReleaseDC(hWnd, hdc);
				DeleteDC(mdc);
				DeleteObject(hbmp);
				DeleteObject(fbmp);
				break;
			}
			else
			{
				GetObject(fbmp, sizeof(BITMAP), &bm2);
				position_x = position_x + (j - 1)*FRONT_WID;
				FrontPic(hdc, position_x, position_y, FRONT_WID, FRONT_HEI);
				ReleaseDC(hWnd, hdc);
			}*/

		}
	}

	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 8; j++){
			SelectObject(mdc, imageResource[index[i][j]]);
			position_x = position_x + FRONT_WID + 3;
			FrontPic(hdc, position_x, position_y, FRONT_WID, FRONT_HEI);
		}
		position_x = 301 - FRONT_WID;
		position_y += FRONT_HEI + 3;
	}

	ReleaseDC(hWnd, hdc);
	DeleteDC(mdc);

}

