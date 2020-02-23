// Win32AppDemo.cpp : ����Ӧ�ó������ڵ㡣
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

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
static string str;
static string cut_line;
static string my_str;
static UINT32 sys_time;
static HBITMAP g_bmp1; 
static HBITMAP g_bmp2;
static HBITMAP g_bmp_bkg;




// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK new_file_handle(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


#if 1

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

INT_PTR CALLBACK new_file_handle(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
			SetWindowText(hDlg, "����һ���Ի���!");
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK:
					MessageBox(NULL, "�û�ѡ��OK��ť", "�Ի��򵯴�", MB_OK);
					EndDialog(hDlg, IDOK);
					break;

				case IDCANCEL:
					MessageBox(NULL, "�û�ѡ��CANCEL��ť", "�Ի��򵯴�", MB_OK);
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

//lff:����һЩ�ӿؼ�
int ui_handle_create(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SetWindowText(hWnd, "����˹����"); //lff:����������ʾ����
	cut_line = "------------------ �ָ��� ------------------------";
	g_bmp1 = LoadBitmap(hInst, (LPCTSTR)IDB_BITMAP1); //����Դ��װ��bmp
	g_bmp2 = LoadBitmap(hInst, (LPCTSTR)IDB_BITMAP2);
	g_bmp_bkg = (HBITMAP)LoadImage(NULL, "bkg.bmp", IMAGE_BITMAP, 500, 373, LR_LOADFROMFILE); //���ļ���װ��bmp

	//��ť
	CreateWindowEx(0, "button", "��ť", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 10, 100, 80, 30, hWnd, (HMENU)ID_USR_BUTTON, hInst, NULL);
	//��̬��
	CreateWindowEx(0, "static", "��̬��", WS_CHILD|WS_VISIBLE|SS_SUNKEN, 10, 150, 150, 60, hWnd, (HMENU)ID_USR_STATIC, hInst, NULL); //
	//�༭��
	CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "�༭��", WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE, 10, 220, 150, 60, hWnd, (HMENU)ID_USR_EDITTEXT, hInst, NULL);

	//ͼƬ��̬��
	CreateWindowEx(0, "static", "��̬��bmp", WS_CHILD|WS_VISIBLE|SS_NOTIFY, 300, 0, 332, 332, hWnd, (HMENU)ID_USR_BMP, hInst, NULL); //SS_SUNKEN
	//����SS_BITMAP���
	HWND hWndBmp = GetDlgItem(hWnd, ID_USR_BMP);
	LONG nStyle = GetWindowLong(hWndBmp, GWL_STYLE);
	SetWindowLong(hWndBmp, GWL_STYLE, nStyle|SS_BITMAP);
	//����ͼƬ
	SendDlgItemMessage(hWnd, ID_USR_BMP, STM_SETIMAGE, IMAGE_BITMAP, (long)g_bmp1);

	SetTimer(hWnd, MY_TIMEER, 1000, NULL); //����500ms��ʱ��
	
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
	// �����˵�ѡ��:
	switch (wmId)
	{
		case ID_USR_BUTTON:
			MessageBox(hWnd, "����һ������!", "����111", MB_OK);
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
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About); //�����Ի���
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
	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 128, 128)); //�����Զ��廭��
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(220, 220, 40)); //�����Զ��廭ˢ
	
	hdc = BeginPaint(hWnd, &ps); //lff:��ȡ���ڿͻ�����Ч������豸�������(������Ļ��һ������)

	#ifdef PAINT_BKG  //����ͼƬ����
	BITMAP bminfo;
	HDC memdc = CreateCompatibleDC(hdc);  //�ڴ�DC
	GetObject(g_bmp_bkg, sizeof(bminfo), &bminfo);  //��ȡͼƬ��С��Ϣ
	SelectObject(memdc, g_bmp_bkg);		  //��g_bg_bmpѡ���ڴ�DC
	BitBlt(hdc,0,0,bminfo.bmWidth,bminfo.bmHeight,memdc,0,0,SRCCOPY);  //��ͼ��Ŀ��DC 
	DeleteDC(memdc);
	#endif
	
	SetTextColor(hdc, RGB(0, 0, 255));
	//SetBkColor(hdc, GetSysColor(COLOR_3DFACE)); //lff:�����ı�����ɫ�Ϳͻ�������ɫһ��
	//SetBkMode(hdc, TRANSPARENT);
	
	TextOut(hdc, 0, 0, str.c_str(), str.length());
	TextOut(hdc, 0, 60, cut_line.c_str(), cut_line.length());
	TextOut(hdc, 0, 80, my_str.c_str(), my_str.length());

	hOldPen = (HPEN)SelectObject(hdc, hPen); 
	MoveToEx(hdc, 0, 290, NULL);
	LineTo(hdc, 270, 290);  //����
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
	
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, 0, 295, 100, 330); //������
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
        case VK_LEFT://�����  
            break;  
			
        case VK_RIGHT:  
            break; 
			
        case VK_UP:
            break; 
			
        case VK_DOWN:
            break; 

		case VK_SPACE: //�ո��
			break;
			
        default:  
            break;
	}
	return 0;
}


int ui_handle_char(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	str += char(wParam);
	InvalidateRect(hWnd, NULL, 0); //lff:ʹ�ͻ�����Ч,windows������WM_PAINT��Ϣ(���������������Ƿ��������)
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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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

		case WM_CHAR: //lff: WM_KENDOWN��Ϣ��TranslateMessageת��ΪWM_CHAR��Ϣ
			ui_handle_char(hWnd, message, wParam, lParam);
			break;

		case WM_LBUTTONDOWN: //lff:������������Ϣ
			ui_handle_lbuttondown(hWnd, message, wParam, lParam);
			break;

		case WM_RBUTTONDOWN: //lff:����Ҽ�����
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc; //lff:�ص�����
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32APPDEMO));//�������Ͻ�ͼ����
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW); //�����
	//wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1); //������ˢ���
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW); //lff:����ɫ��Ϊ��ɫ(�Ի������ɫ)
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1); //lff:�˵���
	wcex.lpszClassName	= szWindowClass;  //lff:����������
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); //Сͼ����

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,  //lff:���������� 
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow); //��ʾ����
   UpdateWindow(hWnd); //ˢ�´��ڿͻ���

   return TRUE;
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,  //lff:��ģ���ʵ����� 
                     _In_opt_ HINSTANCE hPrevInstance, //lff:win16�������ģ�����
                     _In_ LPTSTR    lpCmdLine, //lff:�����в���
                     _In_ int       nCmdShow) //lff:�����ڳ�ʼ������ʾ��ʽ
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_STRING_RUNOOB, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32APPDEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance); //lff:ע�ᴰ����

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow)) //����������
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32APPDEMO));

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

#endif
