// Win32AppDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Win32AppDemo.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include "my_resource.h"
using namespace std;

#define MAX_LOADSTRING   100
#define MY_TIMEER        1
//#define PAINT_BKG

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
static string str;
static string cut_line;
static string my_str;
static UINT32 sys_time;
static HBITMAP g_bmp1; 
static HBITMAP g_bmp2;
static HBITMAP g_bmp_bkg;




// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK new_file_handle(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


#if 1

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

INT_PTR CALLBACK new_file_handle(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
			SetWindowText(hDlg, "这是一个对话框!");
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK:
					MessageBox(NULL, "用户选择OK按钮", "对话框弹窗", MB_OK);
					EndDialog(hDlg, IDOK);
					break;

				case IDCANCEL:
					MessageBox(NULL, "用户选择CANCEL按钮", "对话框弹窗", MB_OK);
					EndDialog(hDlg, IDOK);
					break;

				default:
					break;
			}

		default:
			break;
	}
	return 0;
}

//lff:创建一些子控件
int ui_handle_create(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SetWindowText(hWnd, "俄罗斯方块"); //lff:窗口栏上显示标题
	cut_line = "------------------ 分割线 ------------------------";
	g_bmp1 = LoadBitmap(hInst, (LPCTSTR)IDB_BITMAP1); //从资源中装载bmp
	g_bmp2 = LoadBitmap(hInst, (LPCTSTR)IDB_BITMAP2);
	g_bmp_bkg = (HBITMAP)LoadImage(NULL, "bkg.bmp", IMAGE_BITMAP, 500, 373, LR_LOADFROMFILE); //从文件中装载bmp

	//按钮
	CreateWindowEx(0, "button", "按钮", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 10, 100, 80, 30, hWnd, (HMENU)ID_USR_BUTTON, hInst, NULL);
	//静态框
	CreateWindowEx(0, "static", "静态框", WS_CHILD|WS_VISIBLE|SS_SUNKEN, 10, 150, 150, 60, hWnd, (HMENU)ID_USR_STATIC, hInst, NULL); //
	//编辑框
	CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "编辑框", WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE, 10, 220, 150, 60, hWnd, (HMENU)ID_USR_EDITTEXT, hInst, NULL);

	//图片静态框
	CreateWindowEx(0, "static", "静态框bmp", WS_CHILD|WS_VISIBLE|SS_NOTIFY, 300, 0, 332, 332, hWnd, (HMENU)ID_USR_BMP, hInst, NULL); //SS_SUNKEN
	//设置SS_BITMAP风格
	HWND hWndBmp = GetDlgItem(hWnd, ID_USR_BMP);
	LONG nStyle = GetWindowLong(hWndBmp, GWL_STYLE);
	SetWindowLong(hWndBmp, GWL_STYLE, nStyle|SS_BITMAP);
	//设置图片
	SendDlgItemMessage(hWnd, ID_USR_BMP, STM_SETIMAGE, IMAGE_BITMAP, (long)g_bmp1);

	SetTimer(hWnd, MY_TIMEER, 1000, NULL); //设置500ms定时器
	
	return 0;
}

int ui_handle_timer(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	sys_time++;
	InvalidateRect(hWnd, NULL, 0);
	return 0;
}

int ui_handle_command(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	FILE *file = NULL;
	
	wmId	= LOWORD(wParam);
	wmEvent = HIWORD(wParam);
	// 分析菜单选择:
	switch (wmId)
	{
		case ID_USR_BUTTON:
			MessageBox(hWnd, "这是一个弹窗!", "弹窗111", MB_OK);
			break;

		case ID_USR_EDITTEXT:
			if(HIWORD(wParam) == EN_CHANGE)
			{
				char sz[256] = {0};
				GetWindowText(GetDlgItem(hWnd, ID_USR_EDITTEXT), sz, 256);
				SetWindowText(GetDlgItem(hWnd, ID_USR_STATIC), sz);
				SendDlgItemMessage(hWnd, ID_USR_BMP, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_bmp2);
			}
			break;

		case ID_USR_BMP:
			SendDlgItemMessage(hWnd, ID_USR_BMP, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_bmp2);
			break;
			
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About); //创建对话框
			break;
			
		case ID_NEW_FILE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, new_file_handle);
			break;
			
		case ID_OPEN:
			file = fopen("text.txt", "r+");
			if(!file)
			{
				cout<<"fopen err!\n";
				return 0;
			}
			fread((void*)str.c_str(), str.length(), 2, file);
			fclose(file);
			InvalidateRect(hWnd, NULL, 0);
			break;
	
		case ID_SAVE:
			file = fopen("text.txt", "w+");
			if(!file)
			{
				cout<<"fopen err!\n";
				return 0;
			}
			fwrite(str.c_str(), str.length(), 2, file);
			fclose(file);
			break;
			
		case ID_CLOSE:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		
		case ID_EXIT:
			DestroyWindow(hWnd);
			break;
			
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int ui_handle_paint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	char time[16] = {0};
	HPEN hOldPen;
	HBRUSH hOldBrush;
	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 128, 128)); //创建自定义画笔
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(220, 220, 40)); //创建自定义画刷
	
	hdc = BeginPaint(hWnd, &ps); //lff:获取窗口客户区无效区域的设备环境句柄(代表屏幕上一块区域)

	#ifdef PAINT_BKG  //绘制图片背景
	BITMAP bminfo;
	HDC memdc = CreateCompatibleDC(hdc);  //内存DC
	GetObject(g_bmp_bkg, sizeof(bminfo), &bminfo);  //获取图片大小信息
	SelectObject(memdc, g_bmp_bkg);		  //将g_bg_bmp选入内存DC
	BitBlt(hdc,0,0,bminfo.bmWidth,bminfo.bmHeight,memdc,0,0,SRCCOPY);  //贴图到目标DC 
	DeleteDC(memdc);
	#endif
	
	SetTextColor(hdc, RGB(0, 0, 255));
	//SetBkColor(hdc, GetSysColor(COLOR_3DFACE)); //lff:设置文本背景色和客户区背景色一致
	//SetBkMode(hdc, TRANSPARENT);
	
	TextOut(hdc, 0, 0, str.c_str(), str.length());
	TextOut(hdc, 0, 60, cut_line.c_str(), cut_line.length());
	TextOut(hdc, 0, 80, my_str.c_str(), my_str.length());

	hOldPen = (HPEN)SelectObject(hdc, hPen); 
	MoveToEx(hdc, 0, 290, NULL);
	LineTo(hdc, 270, 290);  //画线
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
	
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, 0, 295, 100, 330); //画矩形
	SelectObject(hdc, hOldBrush);
	DeleteObject(hOldBrush);
	
	wsprintf(time, "time: %u", sys_time);
	TextOut(hdc, 0, 300, time, strlen(time));
	
	EndPaint(hWnd, &ps);
	return 0;
}

int ui_handle_keydown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{  
        case VK_LEFT://左方向键  
            break;  
			
        case VK_RIGHT:  
            break; 
			
        case VK_UP:
            break; 
			
        case VK_DOWN:
            break; 

		case VK_SPACE: //空格键
			break;
			
        default:  
            break;
	}
	return 0;
}


int ui_handle_char(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	str += char(wParam);
	InvalidateRect(hWnd, NULL, 0); //lff:使客户区无效,windows将发送WM_PAINT消息(第三个参数决定是否擦除背景)
	//UpdateWindow(hWnd);
	return 0;
}

int ui_handle_lbuttondown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char szPoint[56] = {0};

	wsprintf(szPoint, "X=%d, Y=%d", LOWORD(lParam), HIWORD(lParam));
	my_str = szPoint;
	if(wParam & MK_SHIFT)
	{
		my_str = my_str + "  shift key is down";
	}
	InvalidateRect(hWnd, NULL, 0);
	return 0;
}

int ui_handle_rbuttondown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

int ui_handle_close(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	printf("close window!\n");
	KillTimer(hWnd,MY_TIMEER);
	DestroyWindow(hWnd);
	return 0;
}


//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			ui_handle_create(hWnd, message, wParam, lParam);
			break;

		case WM_TIMER:
			ui_handle_timer(hWnd, message, wParam, lParam);
			break;

		case WM_COMMAND:
			ui_handle_command(hWnd, message, wParam, lParam);
			break;

		case WM_PAINT:
			ui_handle_paint(hWnd, message, wParam, lParam);
			break;

		case WM_KEYDOWN:
			ui_handle_keydown(hWnd, message, wParam, lParam);
			break;

		case WM_CHAR: //lff: WM_KENDOWN消息被TranslateMessage转化为WM_CHAR消息
			ui_handle_char(hWnd, message, wParam, lParam);
			break;

		case WM_LBUTTONDOWN: //lff:鼠标左键按下消息
			ui_handle_lbuttondown(hWnd, message, wParam, lParam);
			break;

		case WM_RBUTTONDOWN: //lff:鼠标右键按下
			ui_handle_rbuttondown(hWnd, message, wParam, lParam);
			break;

		case WM_CLOSE:
			ui_handle_close(hWnd, message, wParam, lParam);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
			
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc; //lff:回调函数
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32APPDEMO));//窗口左上角图标句柄
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW); //光标句柄
	//wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1); //背景画刷句柄
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW); //lff:背景色设为灰色(对话框的颜色)
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1); //lff:菜单名
	wcex.lpszClassName	= szWindowClass;  //lff:窗口类名称
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); //小图标句柄

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,  //lff:创建主窗口 
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow); //显示窗口
   UpdateWindow(hWnd); //刷新窗口客户区

   return TRUE;
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,  //lff:本模块的实例句柄 
                     _In_opt_ HINSTANCE hPrevInstance, //lff:win16留下来的，无用
                     _In_ LPTSTR    lpCmdLine, //lff:命令行参数
                     _In_ int       nCmdShow) //lff:主窗口初始化的显示方式
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_STRING_RUNOOB, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32APPDEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance); //lff:注册窗口类

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow)) //创建主窗口
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32APPDEMO));

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

#endif
