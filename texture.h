#pragma once


#include "renderer.h"

int LoadTexture(char* fileName);
int LoadScreenShot();
void spilitTexture(int spilitIndex, int* out_R, int* out_L);
void UninitTexture(void);

ID3D11ShaderResourceView** GetTexture(int index);
