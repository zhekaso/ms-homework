#include "stdafx.h"
#include "Lab.h"

#include "msg_callbacks.h"

#define MAX_STATIC_STR_LEN 100

static unsigned long ref_cnt = 0;

static HPEN hPenR;
static HPEN hPenG;

static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

void InitInstruments()
{
	if (ref_cnt)
		goto exit;

	hPenR = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	hPenG = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));

exit:
	ref_cnt++;
}

void ReleseInstuments()
{
	ref_cnt--;
	if (ref_cnt)
		return;
	DeleteObject(hPenG);
	DeleteObject(hPenR);
}

WINDOW_MSG_CALLBACK(MenuActivation_cb)
{
	int wmId = LOWORD(wParam);
	// Разобрать выбор в меню:
	switch (wmId)
	{
	case IDM_ABOUT:
		DialogBox(PState->hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	case IDM_SENDTOBROTHER:
		PState->ToogleFeatureResend();
		InvalidateRect(hWnd, NULL, true);
		PState->SendMsgRedrawToBrothersForce(hWnd);
	default:
		return -1;
	}
	return 0;
}

WINDOW_MSG_CALLBACK(LeftButtonUp_cb)
{
	PState->ToogleColor(hWnd);

	InvalidateRect(hWnd, NULL, true);
	return 0;
}

WINDOW_MSG_CALLBACK(Draw_cb)
{
	HPEN hOldPen;
	HBRUSH hOldBrush;

	RECT rect;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	// Получаем размер клиентской области окна
	GetClientRect(hWnd, &rect);

	if (PState->IsRed(hWnd))
		hOldPen = (HPEN)SelectObject(hdc, hPenR);
	else
		hOldPen = (HPEN)SelectObject(hdc, hPenG);

	// Перемещаем точку рисования в левый верхний угол окна
	MoveToEx(hdc, rect.left, rect.top, NULL);

	// Рисуем линию в правый нижний угол
	LineTo(hdc, rect.right, rect.bottom);

	// Перемещаем точку рисования в правый верхний угол окна
	MoveToEx(hdc, rect.right, rect.top, NULL);
	// Рисуем линию в левый нижний угол
	LineTo(hdc, rect.left, rect.bottom);

	WCHAR strToPrint[MAX_STATIC_STR_LEN];
	const POINT *MousePos = PState->GetMousePosition();
	swprintf_s(strToPrint, L"X: %u Y:%u (%c)", MousePos->x, MousePos->y, PState->IsSendToBrother() ? '+' : '-');

	SIZE text_box_size;
	GetTextExtentPointW(hdc, strToPrint, wcslen(strToPrint), &text_box_size);
	//MoveToEx(hdc, rect.right - text_box_size.cx-5, rect.bottom - text_box_size.cy-5, NULL);
	rect.right -= 2;
	rect.bottom -= 2;
	rect.top = rect.bottom - text_box_size.cy - 2;
	rect.left = rect.right - text_box_size.cx - 2;

	SelectObject(hdc, GetStockObject(BLACK_PEN));
	hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	DrawText(hdc, strToPrint, -1, &rect, DT_TOP | DT_LEFT);
	rect.left -= 2;
	rect.right += 2;
	rect.top -= 2;
	rect.bottom += 2;
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	// Выбираем старый Pen в DC
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	EndPaint(hWnd, &ps);
	return 0;
}

WINDOW_MSG_CALLBACK(MouseMove_cb)
{
	POINT Mouse;
	GetCursorPos(&Mouse);
	PState->MouseMove(&Mouse);
	//ScreenToClient(hWnd, &Mouse);
	InvalidateRect(hWnd, NULL, true);
	PState->SendMsgRedrawToBrothers(hWnd);
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