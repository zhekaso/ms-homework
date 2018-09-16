#include "stdafx.h"
#include "Lab.h"

#include "msg_callbacks.h"

#define MAX_STATIC_STR_LEN 100

struct ClockAngles
{
	int hours;
	int minutes;
	int seconds;
	float Hangle;
	float Mangle;
	float Sangle;
} HMSangles;

static unsigned long ref_cnt = 0;

static HPEN hPenR;
static HPEN hPenG;
static HPEN hPenHourHand;
static HPEN hPenMinuteHand;
static HPEN hPenSecondHand;

static HBRUSH hBrushR;
static HBRUSH hBrushB;

static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
static void DrawClock(ProgramState *PState, HWND hWnd, HDC hdc, float Hangle, float Mangle, float Sangle);

static void CountCurentAngles(ClockAngles *HMSangles);
static void AddOneSecond(ClockAngles *HMSangles);

static ProgramState *PState;

void f(HWND hwnd, UINT uMsg, UINT_PTR timerId, DWORD dwTime)
{
	AddOneSecond(&HMSangles);
	PState->SendMsgRedrawToBrothersForce(0);
}

void InitInstruments(ProgramState *PStateLocal)
{
	if (ref_cnt)
		goto exit;

	hPenR = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	hPenG = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));

	hPenHourHand = CreatePen(PS_SOLID, 20, RGB(139, 69, 19));
	hPenMinuteHand = CreatePen(PS_SOLID, 10, RGB(47, 79, 79));
	hPenSecondHand = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	hBrushR = CreateSolidBrush(RGB(255, 0, 0));
	hBrushB = CreateSolidBrush(RGB(0, 255, 0));

	PState = PStateLocal;
	CountCurentAngles(&HMSangles);
	SetTimer(0, 1, 1000, f);

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

	DeleteObject(hPenHourHand);
	DeleteObject(hPenMinuteHand);
	DeleteObject(hPenSecondHand);

	DeleteObject(hBrushR);
	DeleteObject(hBrushB);
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

WINDOW_MSG_CALLBACK(Timer_cb)
{
	AddOneSecond(&HMSangles);
	InvalidateRect(hWnd, NULL, true);
	PState->SendMsgRedrawToBrothers(hWnd);
	return 0;
}

WINDOW_MSG_CALLBACK(Draw_cb)
{
	HPEN hOldPen;
	HBRUSH hOldBrush;

	RECT rect;

	SIZE text_box_size;
	WCHAR strToPrint[MAX_STATIC_STR_LEN];
	const POINT *MousePos = PState->GetMousePosition();

	swprintf_s(strToPrint, L"X: %u Y:%u (%c)", MousePos->x, MousePos->y, PState->IsSendToBrother() ? '+' : '-');

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	DrawClock(PState, hWnd, hdc, HMSangles.Hangle, HMSangles.Mangle, HMSangles.Sangle);

	// Получаем размер клиентской области окна
	GetClientRect(hWnd, &rect);
	GetTextExtentPointW(hdc, strToPrint, wcslen(strToPrint), &text_box_size);
	rect.right -= 2;
	rect.bottom -= 2;
	rect.top = rect.bottom - text_box_size.cy - 2;
	rect.left = rect.right - text_box_size.cx - 2;

	hOldPen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN));
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
static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

#define BIG_CIRCLE_RADIUS_COEFF 0.75
#define MIDLE_CIRCLE_RADIUS_COEFF 0.5
#define SMALL_CIRCLE_RADIUS_COEFF 0.25
#define SMALL_SMALL_CIRCLE_RADIUS_COEFF 0.25

#define CLOCK_HAND_SCALE 1.2

static void DrawClock(ProgramState *PState, HWND hWnd, HDC hdc, float Hangle, float Mangle, float Sangle)
{
	HPEN hOldPen;
	HBRUSH hOldBrush;

	float dx;
	float dy;
	Hangle *= M_PI / 180;
	Mangle *= M_PI / 180;
	Sangle *= M_PI / 180;

	RECT rect;
	GetClientRect(hWnd, &rect);

	int height = rect.bottom - rect.top;
	int widht = rect.right - rect.left;
	POINT centre = { widht / 2 , height / 2 };

	int max_radius = height > widht ? widht / 2 : height / 2;

	int big_radius = max_radius * BIG_CIRCLE_RADIUS_COEFF;
	int midle_radius = max_radius * MIDLE_CIRCLE_RADIUS_COEFF;
	int small_radius = max_radius * SMALL_CIRCLE_RADIUS_COEFF;

	if (PState->IsRed(hWnd))
		hOldPen = (HPEN)SelectObject(hdc, hPenR);
	else
		hOldPen = (HPEN)SelectObject(hdc, hPenG);

	//Обводка
	Ellipse(hdc, centre.x - big_radius, centre.y - big_radius, centre.x + big_radius, centre.y + big_radius);
	Ellipse(hdc, centre.x - midle_radius, centre.y - midle_radius, centre.x + midle_radius, centre.y + midle_radius);
	Ellipse(hdc, centre.x - small_radius, centre.y - small_radius, centre.x + small_radius, centre.y + small_radius);
	//

	//Часовая стрелка
	MoveToEx(hdc, centre.x, centre.y, NULL);

	dx = sin(Hangle) * (small_radius * CLOCK_HAND_SCALE);
	dy = cos(Hangle) * (small_radius * CLOCK_HAND_SCALE);

	SelectObject(hdc, hPenHourHand);
	LineTo(hdc, centre.x + dx, centre.y + dy);
	//

	//Минутная стрелка
	MoveToEx(hdc, centre.x, centre.y, NULL);

	dx = sin(Mangle) * (midle_radius * CLOCK_HAND_SCALE);
	dy = cos(Mangle) * (midle_radius * CLOCK_HAND_SCALE);

	SelectObject(hdc, hPenMinuteHand);
	LineTo(hdc, centre.x + dx, centre.y + dy);
	//

	//Секундная стрелка
	MoveToEx(hdc, centre.x, centre.y, NULL);

	dx = sin(Sangle) * (big_radius);
	dy = cos(Sangle) * (big_radius);

	SelectObject(hdc, hPenSecondHand);
	LineTo(hdc, centre.x + dx, centre.y + dy);
	//

	//Круг в центре
	SelectObject(hdc, hOldPen);
	if (PState->IsRed(hWnd))
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrushR);
	else
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrushB);
	float small_small_radius = small_radius * SMALL_SMALL_CIRCLE_RADIUS_COEFF;
	Ellipse(hdc, centre.x - small_small_radius, centre.y - small_small_radius, centre.x + small_small_radius, centre.y + small_small_radius);
	SelectObject(hdc, hOldBrush);
}

static void CountCurentAngles(ClockAngles *HMSangles)
{
	// 12 = 180 грудусов
	// 3 = 90 градусов
	// 6 = 0 градусов
	// 9 = 315 градусов
	time_t CurentTime_time_t;
	tm CurentTime;
	CurentTime_time_t = chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	localtime_s(&CurentTime, &CurentTime_time_t);

	HMSangles->hours = CurentTime.tm_hour;
	HMSangles->minutes = CurentTime.tm_hour;
	HMSangles->seconds = CurentTime.tm_hour;

	HMSangles->Hangle = (float)CurentTime.tm_hour / 24.0*(-720) + 180;
	HMSangles->Mangle = (float)CurentTime.tm_min / 60.0 *(-360) + 180;
	HMSangles->Sangle = (float)CurentTime.tm_sec / 60 * (-360) + 180;
}

static void AddOneSecond(ClockAngles *HMSangles)
{
	HMSangles->seconds++;

	if (HMSangles->seconds == 60)
	{
		HMSangles->seconds = 0;
		HMSangles->minutes++;
		if (HMSangles->minutes == 60)
		{
			HMSangles->minutes = 0;
			HMSangles->hours++;
			if (HMSangles->hours == 24)
			{
				HMSangles->hours = 0;
			}
		}
	}

	HMSangles->Hangle = (float)HMSangles->hours / 24.0*(-720) + 180;
	HMSangles->Mangle = (float)HMSangles->minutes / 60.0 *(-360) + 180;
	HMSangles->Sangle = (float)HMSangles->seconds / 60 * (-360) + 180;

}