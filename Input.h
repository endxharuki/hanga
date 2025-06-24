#pragma once

#include "main.h"

#define MAX_PAD_NUM (4)

void InitInputPad();
void UpdateInputPad();

D3DXVECTOR2 GetRightInputStick(const int padNo);
D3DXVECTOR2 GetLeftInputStick(const int padNo);