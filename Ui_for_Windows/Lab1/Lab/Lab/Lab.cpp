// Lab.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab.h"
#include <stdio.h>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


#include <map>
using namespace std;

map<HWND, bool> XcolorRed;
bool SendToBrother = true;

POINT Mouse;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB, szWindowClass, MAX_LOADSTRING);
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
	GetCursorPos(&Mouse);
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
    wcex.lpszClassName  = szWindowClass;
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
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   XcolorRed[hWnd] = true;
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
	RECT rc;
    switch (message)
    {

	case WM_SYSCOMMAND:
	{
		int wmId = wParam;//= LOWORD(wParam);
		int test = IDR_MAINFRAME;
		if (wmId == IDR_MAINFRAME)
			SendToBrother =! SendToBrother;

		break;
	}

	case WM_MOUSEMOVE: {
		GetCursorPos(&Mouse);
		//ScreenToClient(hWnd, &Mouse);
		InvalidateRect(hWnd, NULL, true);
		if (SendToBrother)
		{
			for each (auto var in XcolorRed)
			{
				if (var.first != hWnd)
					SendMessage(var.first, message, wParam, lParam);
			}
		}
		break;
	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);

                break;
			case IDR_MAINFRAME:
				SendToBrother = !SendToBrother;
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
		XcolorRed[hWnd] = !XcolorRed[hWnd];
		
		GetClientRect(hWnd, &rc);
		InvalidateRect(hWnd, &rc, true);
		break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			
			// Получаем размер клиентской области окна
			GetClientRect(hWnd, &rc);

			// Создаем Pen
			HPEN hPenR = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			HPEN hPenG = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
			HPEN hOldPen;
			if (XcolorRed[hWnd])
				hOldPen = (HPEN)SelectObject(hdc, hPenR);
			else
				hOldPen = (HPEN)SelectObject(hdc, hPenG);

			// Перемещаем точку рисования в левый верхний угол окна
			MoveToEx(hdc, rc.left, rc.top, NULL);
			// Рисуем линию в правый нижний угол
			LineTo(hdc, rc.right, rc.bottom);

			// Перемещаем точку рисования в правый верхний угол окна
			MoveToEx(hdc, rc.right, rc.top, NULL);
			// Рисуем линию в левый нижний угол
			LineTo(hdc, rc.left, rc.bottom);
			WCHAR strToPrint[MAX_LOADSTRING];
			swprintf_s(strToPrint, L"X: %u Y:%u", Mouse.x, Mouse.y);
			MoveToEx(hdc, rc.right - 100, rc.bottom - 100, NULL);
			rc.top = rc.bottom - 20;
			rc.left = rc.right - 100;
			//rc.right = 100;
			//rc.bottom = 100;
			SelectObject(hdc, GetStockObject(BLACK_PEN));
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
			DrawText(hdc, strToPrint, -1, &rc, DT_TOP | DT_LEFT);
			// Выбираем старый Pen в DC (освобождаем свой Pen из DC)
			SelectObject(hdc, hOldPen);

			// Удаляем Pen
			DeleteObject(hPenG);
			DeleteObject(hPenR);


            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		XcolorRed.erase(hWnd);
        if(!XcolorRed.size())
			PostQuitMessage(0);
		else
			PostMessage(hWnd, WM_CLOSE, 0, 0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
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
