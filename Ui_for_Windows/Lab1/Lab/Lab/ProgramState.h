#pragma once

#include <map>
using namespace std;

#define MAX_LOADSTRING 100

class ProgramState
{
private:
	POINT MousePos;
	map<HWND, bool> XcolorRed;
	bool SendToBrother = true;
public:
	HINSTANCE hInst;                                // текущий экземпляр
	WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
	WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

	void MouseMove(POINT *new_pos);
	void AddWindow(HWND id, bool is_color_red = true);
	void RemoveWindow(HWND id);
	void SendMsgToBrothers(HWND id_src, UINT message, WPARAM wParam, LPARAM lParam);
	void SendMsgRedrawToBrothers(HWND id_src = 0);
	void SendMsgRedrawToBrothersForce(HWND id_src = 0);
	bool IsRed(HWND id);
	void ToogleFeatureResend();
	void ToogleColor(HWND id);
	const POINT* GetMousePosition();
	bool IsSendToBrother();
	ProgramState();
	~ProgramState();
};

