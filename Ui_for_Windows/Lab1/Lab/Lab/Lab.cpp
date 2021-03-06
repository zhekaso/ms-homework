// Lab.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab.h"
#include <stdio.h>

#include "msg_callbacks.h"
#include "ProgramState.h"



ProgramState PState;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, PState.szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB, PState.szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB));
    MSG msg;

	POINT Mouse;
	GetCursorPos(&Mouse);
	PState.MouseMove(&Mouse);
	PState.SendMsgRedrawToBrothers();

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB);
    wcex.lpszClassName  = PState.szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	PState.hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(PState.szWindowClass, PState.szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   PState.AddWindow(hWnd);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int rc = 0;
    switch (message)
    {
	case WM_CREATE:
		InitInstruments(&PState);
		break;
	case WM_MOUSEMOVE:
		rc = MouseMove_cb(&PState, hWnd, wParam, lParam);
		break;
	case WM_SYSCOMMAND:
    case WM_COMMAND:
		rc = MenuActivation_cb(&PState, hWnd, wParam, lParam);
        break;
	case WM_LBUTTONUP:
		rc = LeftButtonUp_cb(&PState, hWnd, wParam, lParam);
		break;
    case WM_PAINT:
		rc = Draw_cb(&PState, hWnd, wParam, lParam);
        break;
    case WM_DESTROY:
		PState.RemoveWindow(hWnd);
		ReleseInstuments();
        break;
	case WM_TIMER:
		rc = Timer_cb(&PState, hWnd, wParam, lParam);
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
	if(rc)
		DefWindowProc(hWnd, message, wParam, lParam);
    return rc;
}