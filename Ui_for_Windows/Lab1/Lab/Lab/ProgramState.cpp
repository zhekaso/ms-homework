#include "stdafx.h"
#include "ProgramState.h"


void ProgramState::MouseMove(POINT * new_pos)
{
	memcpy(&MousePos, new_pos, sizeof(POINT));
}

void ProgramState::AddWindow(HWND id, bool is_color_red)
{
	XcolorRed.insert({ id, is_color_red });
}

void ProgramState::RemoveWindow(HWND id)
{
	XcolorRed.erase(id);
	if (!XcolorRed.size())
		PostQuitMessage(0);
	else
		PostMessage(id, WM_CLOSE, 0, 0);
}

void ProgramState::SendMsgToBrothers(HWND id_src, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!SendToBrother)
		return;
	for (auto el : XcolorRed)
	{
		if (el.first != id_src)
		{
			SendMessage(el.first, message, wParam, lParam);
		}
	}
}

void ProgramState::SendMsgRedrawToBrothers(HWND id_src)
{
	if (!SendToBrother)
		return;
	for (auto el : XcolorRed)
	{
		if (el.first != id_src)
		{
			InvalidateRect(el.first, NULL, true);
		}
	}
}

void ProgramState::SendMsgRedrawToBrothersForce(HWND id_src)
{
	for (auto el : XcolorRed)
	{
		if (el.first != id_src)
		{
			InvalidateRect(el.first, NULL, true);
		}
	}
}

bool ProgramState::IsRed(HWND id)
{
	return XcolorRed[id];
}

void ProgramState::ToogleFeatureResend()
{
	SendToBrother = !SendToBrother;
}

void ProgramState::ToogleColor(HWND id)
{
	XcolorRed[id] = !XcolorRed[id];
}

const POINT * ProgramState::GetMousePosition()
{
	return &MousePos;
}

bool ProgramState::IsSendToBrother()
{
	return SendToBrother;
}

ProgramState::ProgramState()
{
}


ProgramState::~ProgramState()
{
}
