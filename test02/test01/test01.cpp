// test01.cpp : 定义应用程序的入口点。
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

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
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

// 此代码模块中包含的函数的前向声明: 
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

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST01, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST01));

	// 主消息循环: 
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
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
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

//函数声明位置

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	
   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, "OK!", WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
	   return FALSE;
   }

   MoveWindow(hWnd, 200, 100, 800, 700, true);//可以更改窗口的大小
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   hdc = GetDC(hWnd);
   mdc = CreateCompatibleDC(hdc);

   hbmp = (HBITMAP)LoadImage(NULL, "./img/bg(03).bmp", IMAGE_BITMAP, 780,640, LR_LOADFROMFILE);//第一个数值是宽度
   if (hbmp == NULL)
   {
	   MessageBox(NULL, "背景图片加载失败!", "警告", 0);
	   return FALSE;
   }
   DrawPic(hdc, 780, 640);
   
   
   loadImageResource();

   /*fbmp = (HBITMAP)LoadImage(NULL, "./img/5.bmp", IMAGE_BITMAP, FRONT_WID, FRONT_HEI, LR_LOADFROMFILE);

   
   if (fbmp == NULL)
   {
	   MessageBox(NULL, "前景图片加载失败!", "警告", 0);
	   ReleaseDC(hWnd, hdc);
	   DeleteDC(mdc);
	   DeleteObject(hbmp);
	   DeleteObject(fbmp);
	   return FALSE;
   }

   GetObject(fbmp, sizeof(BITMAP), &bm2);
   FrontPic(hdc, 301, 135,FRONT_WID,FRONT_HEI);*/
   //棋盘的宽度是：
//棋盘的高度是：
   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
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
		// 分析菜单选择: 
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
		// TODO:  在此添加任意绘图代码...
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

// “关于”框的消息处理程序。
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
	//贴上背景图
	SelectObject(mdc, hbmp);
	BitBlt(hdc, 0, 0, wid, hei, mdc, 0, 0, SRCCOPY);//设置图片最大显示的尺寸
	
}
void FrontPic(HDC hdc, int x, int y ,int wid, int hei){
	//贴上前景图
	//BitBlt(hdc, x, y, wid, hei, mdc, 0, 0, SRCCOPY);	//未经过“去背”处理的贴图效果
	TransparentBlt2(hdc, x, y, wid, hei, mdc, 0, 0, wid, hei, RGB(255, 255, 255));	//经过“去背”处理的贴图效果
	//TransparentBlt3(hdc, x, y,wid, hei, mdc, 0, 0, 80, 60, RGB(255, 0, 255), 50);	//经过“去背”和透明化处理的贴图效果
	tPre = GetTickCount();
}

/*void DrawPic(HDC hdc, int x, int y){
	//贴上前景图
	SelectObject(mdc, fbmp);
	BitBlt(hdc, x, y, 80, 60, mdc, 0, 0, SRCCOPY);	//未经过“去背”处理的贴图效果
	//TransparentBlt2(hdc, x, y, 80, 60, mdc, 0, 0, 80, 60, RGB(255, 0, 255));	//经过“去背”处理的贴图效果
	//TransparentBlt3(hdc, x, y, 80, 60, mdc, 0, 0, 80, 60, RGB(255, 0, 255), 50);	//经过“去背”和透明化处理的贴图效果
}*/

void TransparentBlt2(HDC hdc0, int nX0, int nY0, int nW0, int nH0, HDC hdc1, int nX1, int nY1, int nW1, int nH1, UINT Tcol)
{
	HBITMAP hBMP = CreateCompatibleBitmap(hdc0, nW0, nH0);
	HBITMAP mBMP = CreateBitmap(nW0, nH0, 1, 1, NULL);	//创建单色位图变量,背景色为白,其它颜色为黑
	HDC	hDC = CreateCompatibleDC(hdc0);
	HDC	mDC = CreateCompatibleDC(hdc0);
	SelectObject(hDC, hBMP);
	SelectObject(mDC, mBMP);	//设备场景mDC保存单色位图
	if ((nW0 == nW1) && (nH0 == nH1))	//把源设备场景画面复制给hDC
	{
		BitBlt(hDC, 0, 0, nW0, nH0, hdc1, nX1, nY1, SRCCOPY);	//若大小相等则直接复制
	}
	else
	{
		StretchBlt(hDC, 0, 0, nW0, nH0, hdc1, nX1, nY1, nW1, nH1, SRCCOPY);	//否则拉伸复制
	}
	SetBkColor(hDC, Tcol);	//把hDC背景色设置成Tcol颜色
	BitBlt(mDC, 0, 0, nW0, nH0, hDC, 0, 0, SRCCOPY);	//把hDC内位图复制给存储单色位图的mDC,Tcol颜色部分变成白色

	SetBkColor(hDC, RGB(0, 0, 0));	//把hDC背景色设置成黑色
	SetTextColor(hDC, RGB(255, 255, 255));	//把hDC字体(即前景色)设置成白色
	BitBlt(hDC, 0, 0, nW0, nH0, mDC, 0, 0, SRCAND);	//把mDC内位图复制到hDC中并进行与运算

	SetBkColor(hdc0, RGB(255, 255, 255));	//设置hdc0(目的设备场景)背景色为白色
	SetTextColor(hdc0, RGB(0, 0, 0));	//设置hdc0(目的设备场景)字体(即前景色)为黑色
	BitBlt(hdc0, nX0, nY0, nW0, nH0, mDC, 0, 0, SRCAND);	//把mDC内单色位图复制到hdc0(目的设备场景)中并进行与运算
	BitBlt(hdc0, nX0, nY0, nW0, nH0, hDC, 0, 0, SRCPAINT);	//把hDC内位图复制到hdc0(目的设备场景)中并进行或运算
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
				MessageBox(NULL, "前景图片加载失败!", "警告", 0);
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

