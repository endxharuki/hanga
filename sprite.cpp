//=============================================================================
//
// スプライト処理 [sprite.cpp]
// Author : 
//
//=============================================================================
#include "sprite.h"
#include "renderer.h"
#include "texture.h"
#include "OnGameData.h"
#include "Camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX 4	//必要な頂点の数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;	// 頂点情報
static OnGameData* g_GameData = OnGameData::GetInstance();
static Camera* g_Camera = Camera::GetInstance();



//=============================================================================
// 初期化処理
//=============================================================================
void InitSprite(void)
{
	ID3D11Device *pDevice = GetDevice();

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// マスクリソース設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(1, 1, &g_VertexBuffer, &stride, &offset);

	unsigned int mask = LoadTexture((char*)"data/TEXTURE/mask.png");
	GetDeviceContext()->PSSetShaderResources(1, 1, GetTexture(mask));
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSprite(void)
{
	// 頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// スプライトデータ設定
// 座標・サイズ・UV指定
//=============================================================================
//引数
//texNo : テクスチャの識別番号（texture.h, texture.cpp のLoadTexture関数の戻り値）
//X     : X座標（スプライトの中心点）
//Y		: Y座標（スプライトの中心点）
//Width : 横幅
//Height: 高さ
//U		: UV値始点
//V		: UV値始点
//UW	: UV値横幅
//VH	: UV値高さ
void DrawSprite(int texNo, float X, float Y, float Width, float Height, float U, float V, float UW, float VH)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;
	hh = Height * 0.5f;

	

	if (g_Camera->IsSwap())
	{
		// 頂点０番（左上の頂点）
		vertex[1].Position = D3DXVECTOR3(X - hw, Y - hh, 0.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(U, V);

		// 頂点１番（右上の頂点）
		vertex[0].Position = D3DXVECTOR3(X + hw, Y - hh, 0.0f);
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(U + UW, V);

		// 頂点２番（左下の頂点）
		vertex[3].Position = D3DXVECTOR3(X - hw, Y + hh, 0.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(U, V + VH);

		// 頂点３番（右下の頂点）
		vertex[2].Position = D3DXVECTOR3(X + hw, Y + hh, 0.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(U + UW, V + VH);
	}
	else
	{
		// 頂点０番（左上の頂点）
		vertex[0].Position = D3DXVECTOR3(X - hw, Y - hh, 0.0f);
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(U, V);

		// 頂点１番（右上の頂点）
		vertex[1].Position = D3DXVECTOR3(X + hw, Y - hh, 0.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(U + UW, V);

		// 頂点２番（左下の頂点）
		vertex[2].Position = D3DXVECTOR3(X - hw, Y + hh, 0.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(U, V + VH);

		// 頂点３番（右下の頂点）
		vertex[3].Position = D3DXVECTOR3(X + hw, Y + hh, 0.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(U + UW, V + VH);
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(texNo));

	// ポリゴン描画
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}

//=============================================================================
// スプライトデータ設定（左上指定）
// 座標・サイズ・UV指定
//=============================================================================
//引数
//texNo : テクスチャの識別番号（texture.h, texture.cpp のLoadTexture関数の戻り値）
//X     : X座標（スプライトの左上）
//Y		: Y座標（スプライトの左上）
//Width : 横幅
//Height: 高さ
//U		: UV値始点
//V		: UV値始点
//UW	: UV値横幅
//VH	: UV値高さ
void DrawSpriteLeftTop(int texNo, float X, float Y, float Width, float Height, float U, float V, float UW, float VH)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;


	if (g_Camera->IsSwap())
	{
		// 左上を原点として設定するプログラム
		vertex[1].Position = D3DXVECTOR3(X, Y, 0.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(U, V);

		vertex[0].Position = D3DXVECTOR3(X + Width, Y, 0.0f);
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(U + UW, V);

		vertex[3].Position = D3DXVECTOR3(X, Y + Height, 0.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(U, V + VH);

		vertex[2].Position = D3DXVECTOR3(X + Width, Y + Height, 0.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(U + UW, V + VH);
	}
	else
	{
		// 左上を原点として設定するプログラム
		vertex[0].Position = D3DXVECTOR3(X, Y, 0.0f);
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(U, V);

		vertex[1].Position = D3DXVECTOR3(X + Width, Y, 0.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(U + UW, V);

		vertex[2].Position = D3DXVECTOR3(X, Y + Height, 0.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(U, V + VH);

		vertex[3].Position = D3DXVECTOR3(X + Width, Y + Height, 0.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(U + UW, V + VH);
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(texNo));

	// ポリゴン描画
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}

void DrawSpriteLeftTopColor(int texNo, float x, float y, float width, float height, float U, float V, float UW, float VH, float r, float g, float b, float a)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	D3DXCOLOR color = D3DXCOLOR(r, g, b, a);

	// 左上を原点として設定するプログラム
	vertex[0].Position = D3DXVECTOR3(x, y, 0.0f);
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = D3DXVECTOR2(U, V);

	vertex[1].Position = D3DXVECTOR3(x + width, y, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = D3DXVECTOR2(U + UW, V);

	vertex[2].Position = D3DXVECTOR3(x, y + height, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = D3DXVECTOR2(U, V + VH);

	vertex[3].Position = D3DXVECTOR3(x + width, y + height, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = D3DXVECTOR2(U + UW, V + VH);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(texNo));

	// ポリゴン描画
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}


//=============================================================================
// スプライトデータ設定
// 座標・サイズ・UV指定・頂点色
//=============================================================================
//引数
//texNo : テクスチャの識別番号（texture.h, texture.cpp のLoadTexture関数の戻り値）
//X     : X座標（スプライトの中心点）
//Y		: Y座標（スプライトの中心点）
//Width : 横幅
//Height: 高さ
//U		: UV値始点
//V		: UV値始点
//UW	: UV値横幅
//VH	: UV値高さ
//color : 頂点の色（RGBA）
void DrawSpriteColor(int texNo, float X, float Y, float Width, float Height,
		float U, float V, float UW, float VH,
		float r, float g, float b, float a)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;
	hh = Height * 0.5f;

	D3DXCOLOR Color(r, g, b, a);

	if (g_Camera->IsSwap())
	{
		// 頂点０番（左上の頂点）
		vertex[1].Position = D3DXVECTOR3(X - hw, Y - hh, 0.0f);
		vertex[1].Diffuse = Color;
		vertex[1].TexCoord = D3DXVECTOR2(U, V);

		// 頂点１番（右上の頂点）
		vertex[0].Position = D3DXVECTOR3(X + hw, Y - hh, 0.0f);
		vertex[0].Diffuse = Color;
		vertex[0].TexCoord = D3DXVECTOR2(U + UW, V);

		// 頂点２番（左下の頂点）
		vertex[3].Position = D3DXVECTOR3(X - hw, Y + hh, 0.0f);
		vertex[3].Diffuse = Color;
		vertex[3].TexCoord = D3DXVECTOR2(U, V + VH);

		// 頂点３番（右下の頂点）
		vertex[2].Position = D3DXVECTOR3(X + hw, Y + hh, 0.0f);
		vertex[2].Diffuse = Color;
		vertex[2].TexCoord = D3DXVECTOR2(U + UW, V + VH);
	}
	else
	{
		// 頂点０番（左上の頂点）
		vertex[0].Position = D3DXVECTOR3(X - hw, Y - hh, 0.0f);
		vertex[0].Diffuse = Color;
		vertex[0].TexCoord = D3DXVECTOR2(U, V);

		// 頂点１番（右上の頂点）
		vertex[1].Position = D3DXVECTOR3(X + hw, Y - hh, 0.0f);
		vertex[1].Diffuse = Color;
		vertex[1].TexCoord = D3DXVECTOR2(U + UW, V);

		// 頂点２番（左下の頂点）
		vertex[2].Position = D3DXVECTOR3(X - hw, Y + hh, 0.0f);
		vertex[2].Diffuse = Color;
		vertex[2].TexCoord = D3DXVECTOR2(U, V + VH);

		// 頂点３番（右下の頂点）
		vertex[3].Position = D3DXVECTOR3(X + hw, Y + hh, 0.0f);
		vertex[3].Diffuse = Color;
		vertex[3].TexCoord = D3DXVECTOR2(U + UW, V + VH);
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(texNo));

	// ポリゴン描画
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}


//=============================================================================
// スプライトデータ設定
// 頂点座標指定・UV指定・頂点色
//=============================================================================
//引数
//texNo : テクスチャの識別番号（texture.h, texture.cpp のLoadTexture関数の戻り値）
//vtx	: 短径を構成する頂点
//U		: UV値始点
//V		: UV値始点
//UW	: UV値横幅
//VH	: UV値高さ
//color : 頂点の色（RGBA）
void DrawSpriteVtxSetColor(int texNo, D3DXVECTOR2* vtx,
	float U, float V, float UW, float VH,
	float r, float g, float b, float a)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	D3DXCOLOR Color(r, g, b, a);

	if (g_Camera->IsSwap())
	{
		// 頂点０番（左上の頂点）
		vertex[1].Position = D3DXVECTOR3(vtx[0].x, vtx[0].y, 0.0f);
		vertex[1].Diffuse = Color;
		vertex[1].TexCoord = D3DXVECTOR2(U, V);

		// 頂点１番（右上の頂点）
		vertex[0].Position = D3DXVECTOR3(vtx[1].x, vtx[1].y, 0.0f);
		vertex[0].Diffuse = Color;
		vertex[0].TexCoord = D3DXVECTOR2(U + UW, V);

		// 頂点２番（左下の頂点）
		vertex[3].Position = D3DXVECTOR3(vtx[2].x, vtx[2].y, 0.0f);
		vertex[3].Diffuse = Color;
		vertex[3].TexCoord = D3DXVECTOR2(U, V + VH);

		// 頂点３番（右下の頂点）
		vertex[2].Position = D3DXVECTOR3(vtx[3].x, vtx[3].y, 0.0f);
		vertex[2].Diffuse = Color;
		vertex[2].TexCoord = D3DXVECTOR2(U + UW, V + VH);
	}
	else
	{
		// 頂点０番（左上の頂点）
		vertex[0].Position = D3DXVECTOR3(vtx[0].x, vtx[0].y, 0.0f);
		vertex[0].Diffuse = Color;
		vertex[0].TexCoord = D3DXVECTOR2(U, V);

		// 頂点１番（右上の頂点）
		vertex[1].Position = D3DXVECTOR3(vtx[1].x, vtx[1].y, 0.0f);
		vertex[1].Diffuse = Color;
		vertex[1].TexCoord = D3DXVECTOR2(U + UW, V);

		// 頂点２番（左下の頂点）
		vertex[2].Position = D3DXVECTOR3(vtx[2].x, vtx[2].y, 0.0f);
		vertex[2].Diffuse = Color;
		vertex[2].TexCoord = D3DXVECTOR2(U, V + VH);

		// 頂点３番（右下の頂点）
		vertex[3].Position = D3DXVECTOR3(vtx[3].x, vtx[3].y, 0.0f);
		vertex[3].Diffuse = Color;
		vertex[3].TexCoord = D3DXVECTOR2(U + UW, V + VH);
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(texNo));

	// ポリゴン描画
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}


//=============================================================================
// スプライトデータ設定
// 座標・サイズ・UV指定・頂点色・回転角度
//=============================================================================
//引数
//texNo : テクスチャの識別番号（texture.h, texture.cpp のLoadTexture関数の戻り値）
//X     : X座標（スプライトの中心点）
//Y		: Y座標（スプライトの中心点）
//Width : 横幅
//Height: 高さ
//U		: UV値始点
//V		: UV値始点
//UW	: UV値横幅
//VH	: UV値高さ
//color : 頂点の色（RGBA）
//Rot	: 回転角度（ラジアン値）
void DrawSpriteColorRotate(int texNo, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	float r, float g, float b, float a, float Rot)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;
	hh = Height * 0.5f;

	if (g_Camera->IsSwap())
	{
		// 座標変換
		vertex[1].Position.x = (-hw) * cosf(Rot) - (-hh) * sinf(Rot) + X;
		vertex[1].Position.y = (-hw) * sinf(Rot) + (-hh) * cosf(Rot) + Y;
		vertex[0].Position.x = (hw)*cosf(Rot) - (-hh) * sinf(Rot) + X;
		vertex[0].Position.y = (hw)*sinf(Rot) + (-hh) * cosf(Rot) + Y;
		vertex[3].Position.x = (-hw) * cosf(Rot) - (hh)*sinf(Rot) + X;
		vertex[3].Position.y = (-hw) * sinf(Rot) + (hh)*cosf(Rot) + Y;
		vertex[2].Position.x = (hw)*cosf(Rot) - (hh)*sinf(Rot) + X;
		vertex[2].Position.y = (hw)*sinf(Rot) + (hh)*cosf(Rot) + Y;
	}
	else
	{
		// 座標変換
		vertex[0].Position.x = (-hw) * cosf(Rot) - (-hh) * sinf(Rot) + X;
		vertex[0].Position.y = (-hw) * sinf(Rot) + (-hh) * cosf(Rot) + Y;
		vertex[1].Position.x = (hw)*cosf(Rot) - (-hh) * sinf(Rot) + X;
		vertex[1].Position.y = (hw)*sinf(Rot) + (-hh) * cosf(Rot) + Y;
		vertex[2].Position.x = (-hw) * cosf(Rot) - (hh)*sinf(Rot) + X;
		vertex[2].Position.y = (-hw) * sinf(Rot) + (hh)*cosf(Rot) + Y;
		vertex[3].Position.x = (hw)*cosf(Rot) - (hh)*sinf(Rot) + X;
		vertex[3].Position.y = (hw)*sinf(Rot) + (hh)*cosf(Rot) + Y;
	}

	D3DXCOLOR Color(r, g, b, a);
	vertex[0].Diffuse = Color;
	vertex[1].Diffuse = Color;
	vertex[2].Diffuse = Color;
	vertex[3].Diffuse = Color;

	vertex[0].TexCoord = D3DXVECTOR2(U, V);
	vertex[1].TexCoord = D3DXVECTOR2(U + UW, V);
	vertex[2].TexCoord = D3DXVECTOR2(U, V + VH);
	vertex[3].TexCoord = D3DXVECTOR2(U + UW, V + VH);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(texNo));

	// ポリゴン描画
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}

//=============================================================================
// スプライトデータ設定
// 座標・サイズ・UV指定・頂点色・回転角度
//=============================================================================
//引数
//texNo : テクスチャの識別番号（texture.h, texture.cpp のLoadTexture関数の戻り値）
//X     : X座標（スプライトの中心点）
//Y		: Y座標（スプライトの中心点）
//Width : 横幅
//Height: 高さ
//CX	: 回転軸座標X
//CY	: 回転軸座標Y
//U		: UV値始点
//V		: UV値始点
//UW	: UV値横幅
//VH	: UV値高さ
//color : 頂点の色（RGBA）
//Rot	: 回転角度（ラジアン値）
void DrawSpriteColorRotateAxis(int texNo, float X, float Y, float Width, float Height, float CX, float CY,
	float U, float V, float UW, float VH,
	float r, float g, float b, float a, float Rot)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;
	hh = Height * 0.5f;

	//※CX,CYが回転軸と考える
	//回転軸を(0,0)の原点に移動するため、回転軸の座標の分だけ矩形全体の座標をマイナスし、その状態で回転を行う

	// 座標変換
	vertex[0].Position.x = (-CX - hw) * cosf(Rot) - (-CY - hh) * sinf(Rot);
	vertex[0].Position.y = (-CX - hw) * sinf(Rot) + (-CY - hh) * cosf(Rot);
	vertex[1].Position.x = (-CX + hw) * cosf(Rot) - (-CY - hh) * sinf(Rot);
	vertex[1].Position.y = (-CX + hw) * sinf(Rot) + (-CY - hh) * cosf(Rot);
	vertex[2].Position.x = (-CX - hw) * cosf(Rot) - (-CY + hh) * sinf(Rot);
	vertex[2].Position.y = (-CX - hw) * sinf(Rot) + (-CY + hh) * cosf(Rot);
	vertex[3].Position.x = (-CX + hw) * cosf(Rot) - (-CY + hh) * sinf(Rot);
	vertex[3].Position.y = (-CX + hw) * sinf(Rot) + (-CY + hh) * cosf(Rot);

	//回転の変換が終わった状態で回転軸の座標分プラスし、原点に戻す。さらに表示座標（X,Y）まで移動させる
	vertex[0].Position.x += CX + X;
	vertex[0].Position.y += CY + Y;
	vertex[1].Position.x += CX + X;
	vertex[1].Position.y += CY + Y;
	vertex[2].Position.x += CX + X;
	vertex[2].Position.y += CY + Y;
	vertex[3].Position.x += CX + X;
	vertex[3].Position.y += CY + Y;

	if (g_Camera->IsSwap())
	{
		D3DXVECTOR3 buf = vertex[1].Position;
		vertex[1].Position = vertex[0].Position;
		vertex[0].Position = buf;
		buf = vertex[3].Position;
		vertex[3].Position = vertex[2].Position;
		vertex[2].Position = buf;
	}

	D3DXCOLOR Color(r, g, b, a);
	vertex[0].Diffuse = Color;
	vertex[1].Diffuse = Color;
	vertex[2].Diffuse = Color;
	vertex[3].Diffuse = Color;

	vertex[0].TexCoord = D3DXVECTOR2(U, V);
	vertex[1].TexCoord = D3DXVECTOR2(U + UW, V);
	vertex[2].TexCoord = D3DXVECTOR2(U, V + VH);
	vertex[3].TexCoord = D3DXVECTOR2(U + UW, V + VH);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(texNo));

	// ポリゴン描画
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}



//=============================================================================
// スプライトデータUI用設定
// 座標・サイズ・UV指定・頂点色
//=============================================================================
//引数
//texNo : テクスチャの識別番号（texture.h, texture.cpp のLoadTexture関数の戻り値）
//X     : X座標（スプライトの中心点）
//Y		: Y座標（スプライトの中心点）
//Width : 横幅
//Height: 高さ
//U		: UV値始点
//V		: UV値始点
//UW	: UV値横幅
//VH	: UV値高さ
//color : 頂点の色（RGBA）
void RectDrawSpriteColor(int texNo, float X, float Y, float Width, float Height, float U, float V, float UW, float VH, float r, float g, float b, float a)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	D3DXVECTOR2 screenSize = g_Camera->GetSize();
	D3DXVECTOR2 zoom = g_Camera->GetZoom();
	// ズーム量が増減しても見える位置を固定にする
	X = X * zoom.x;
	Y = Y * zoom.y;

	// ズーム量が増減しても見える大きさを固定にする
	Width = Width * zoom.x;
	Height = Height * zoom.y;

	float hw, hh;
	hw = Width * 0.5f;
	hh = Height * 0.5f;

	D3DXCOLOR Color(r, g, b, a);

	if (g_Camera->IsSwap())
	{
		// 反転作業
		X = (X - (screenSize.x * zoom.x) / 2) * -2 + X;

		// 頂点０番（左上の頂点）
		vertex[1].Position = D3DXVECTOR3(X - hw, Y - hh, 0.0f);
		vertex[1].Diffuse = Color;
		vertex[1].TexCoord = D3DXVECTOR2(U + UW, V);

		// 頂点１番（右上の頂点）
		vertex[0].Position = D3DXVECTOR3(X + hw, Y - hh, 0.0f);
		vertex[0].Diffuse = Color;
		vertex[0].TexCoord = D3DXVECTOR2(U, V);

		// 頂点２番（左下の頂点）
		vertex[3].Position = D3DXVECTOR3(X - hw, Y + hh, 0.0f);
		vertex[3].Diffuse = Color;
		vertex[3].TexCoord = D3DXVECTOR2(U + UW, V + VH);

		// 頂点３番（右下の頂点）
		vertex[2].Position = D3DXVECTOR3(X + hw, Y + hh, 0.0f);
		vertex[2].Diffuse = Color;
		vertex[2].TexCoord = D3DXVECTOR2(U, V + VH);
	}
	else
	{
		// 頂点０番（左上の頂点）
		vertex[0].Position = D3DXVECTOR3(X - hw, Y - hh, 0.0f);
		vertex[0].Diffuse = Color;
		vertex[0].TexCoord = D3DXVECTOR2(U, V);

		// 頂点１番（右上の頂点）
		vertex[1].Position = D3DXVECTOR3(X + hw, Y - hh, 0.0f);
		vertex[1].Diffuse = Color;
		vertex[1].TexCoord = D3DXVECTOR2(U + UW, V);

		// 頂点２番（左下の頂点）
		vertex[2].Position = D3DXVECTOR3(X - hw, Y + hh, 0.0f);
		vertex[2].Diffuse = Color;
		vertex[2].TexCoord = D3DXVECTOR2(U, V + VH);

		// 頂点３番（右下の頂点）
		vertex[3].Position = D3DXVECTOR3(X + hw, Y + hh, 0.0f);
		vertex[3].Diffuse = Color;
		vertex[3].TexCoord = D3DXVECTOR2(U + UW, V + VH);
	}
	

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(texNo));

	// ポリゴン描画
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}
