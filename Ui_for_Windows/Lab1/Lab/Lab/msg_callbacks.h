#pragma once
#include "ProgramState.h"

void InitInstruments(ProgramState *PState);
void ReleseInstuments();

#define WINDOW_MSG_CALLBACK(func_name) int func_name(ProgramState * PState, HWND hWnd, WPARAM wParam, LPARAM lParam)

WINDOW_MSG_CALLBACK(Draw_cb);
WINDOW_MSG_CALLBACK(MouseMove_cb);
WINDOW_MSG_CALLBACK(MenuActivation_cb);
WINDOW_MSG_CALLBACK(LeftButtonUp_cb);
WINDOW_MSG_CALLBACK(Timer_cb);