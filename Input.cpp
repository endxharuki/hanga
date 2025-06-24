#include "Input.h"
#include <Xinput.h>

#define STICK_STR (32768)
#define DEAD_ZONE (0.2f)	// スティックから指定％は無効判定にする

XINPUT_STATE state[MAX_PAD_NUM];

void InitInputPad() {
	for (int i = 0; i < MAX_PAD_NUM; i++)
	{
		if (XInputGetState(i, &state[i]) == ERROR_DEVICE_NOT_CONNECTED)
		{
			// エラー処理を書く
		}
	}
}

void UpdateInputPad()
{
	for (int i = 0; i < MAX_PAD_NUM; i++)
	{
		if (XInputGetState(i, &state[i]) == ERROR_DEVICE_NOT_CONNECTED)
		{
			// エラー処理を書く
		}
	}
}



D3DXVECTOR2 GetRightInputStick(const int padNo)
{
	float rx = (float)state[padNo].Gamepad.sThumbRX;
	float ry = (float)state[padNo].Gamepad.sThumbRY;

	if (rx < -(STICK_STR * DEAD_ZONE) || (STICK_STR * DEAD_ZONE) < rx) rx = rx / STICK_STR;
	else rx = 0.0f;

	if (ry < -(STICK_STR * DEAD_ZONE) || (STICK_STR * DEAD_ZONE) < ry) ry = ry / STICK_STR;
	else ry = 0.0f;

	return D3DXVECTOR2(rx, ry);
}

D3DXVECTOR2 GetLeftInputStick(const int padNo)
{
	float lx = (float)state[padNo].Gamepad.sThumbLX;
	float ly = (float)state[padNo].Gamepad.sThumbLY;

	if (lx < -(STICK_STR * DEAD_ZONE) || (STICK_STR * DEAD_ZONE) < lx) lx = lx / STICK_STR;
	else lx = 0.0f;

	if (ly < -(STICK_STR * DEAD_ZONE) || (STICK_STR * DEAD_ZONE) < ly) ly = ly / STICK_STR;
	else ly = 0.0f;

	return D3DXVECTOR2(lx, ly);
}

