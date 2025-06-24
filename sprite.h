//=============================================================================
//
// �v���C���[���� [sprite.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitSprite();
void UninitSprite();

void DrawSprite(int texNo, float X, float Y, float Width, float Height, float U, float V, float UW, float VH);
void DrawSpriteLeftTop(int texNo, float X, float Y, float Width, float Height, float U, float V, float UW, float VH);
void DrawSpriteLeftTopColor(int texNo, float x, float y, float width, float height, float U, float V, float UW, float VH, float r, float g, float b, float a);
void DrawSpriteColor(int texNo, float X, float Y, float Width, float Height, float U, float V, float UW, float VH, float r, float g, float b, float a);
void DrawSpriteVtxSetColor(int texNo, D3DXVECTOR2* vtx, float U, float V, float UW, float VH, float r, float g, float b, float a);
void DrawSpriteColorRotate(int texNo, float X, float Y, float Width, float Height, float U, float V, float UW, float VH, float r, float g, float b, float a, float Rot);
void DrawSpriteColorRotateAxis(int texNo, float X, float Y, float Width, float Height, float CX, float CY, float U, float V, float UW, float VH, float r, float g, float b, float a, float Rot);

