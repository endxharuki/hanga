/*==============================================================================

   テクスチャの読み込み [texture.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/

#include "texture.h"

/*------------------------------------------------------------------------------
   定数定義
------------------------------------------------------------------------------*/
#define MAX_TEXTURE_NUM 600

/*------------------------------------------------------------------------------
   構造体宣言
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
   プロトタイプ宣言
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
   グローバル変数の定義
------------------------------------------------------------------------------*/
static ID3D11ShaderResourceView*	g_pTexture[MAX_TEXTURE_NUM] = {};	//テクスチャ配列
static unsigned int g_TextureIndex = 0;					// テクスチャ配列の末尾を示すインデックス
static char g_TextureName[MAX_TEXTURE_NUM][256] = {};	// テクスチャ名バッファ

/*------------------------------------------------------------------------------
	テクスチャの読み込み
------------------------------------------------------------------------------*/
int LoadTexture(char* fileName)
{
	//読み込まれているテクスチャ名を調べて、同名のものが
	//すでに読み込まれていたらその番号を返す
	for (unsigned int i = 0; i < g_TextureIndex; i++)
	{
		//テクスチャ名を比較
		if (strcmp(g_TextureName[i], fileName) == 0)
		{
			return i;
		}
	}

	//読み込み最大数を超えていたら負の値を返す
	if (g_TextureIndex == MAX_TEXTURE_NUM)
	{
		return -1;
	}

	//ファイルからテクスチャを読み込む
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
		GetDevice(),
		fileName,
		NULL,
		NULL,
		&g_pTexture[g_TextureIndex],
		NULL);

	if (S_OK != hr)
	{
		//読み込みに失敗した場合、負の値を返す
		return -1;
	}

	//読み込んだテクスチャ名を保存する
	strcpy_s(g_TextureName[g_TextureIndex], 256, fileName);

	int retIndex = g_TextureIndex;
	
	//インデックスを一つ進める
	g_TextureIndex++;

	return retIndex;
}

int LoadScreenShot()
{
	//読み込み最大数を超えていたら負の値を返す
	if (g_TextureIndex == MAX_TEXTURE_NUM)
	{
		return -1;
	}

	g_pTexture[g_TextureIndex] = GetScreenShot();

	int retIndex = g_TextureIndex;

	//インデックスを一つ進める
	g_TextureIndex++;

	return retIndex;
}

void spilitTexture(int spilitIndex, int* out_R, int* out_L)
{
	ID3D11ShaderResourceView* SRV =  *GetTexture(spilitIndex);
	if (!SRV)return;

	int idx_r = g_TextureIndex;
	int idx_l = g_TextureIndex + 1;

	SplitTexture(SRV, &g_pTexture[idx_r], &g_pTexture[idx_l]);

	*out_R = idx_r;
	*out_L = idx_l;

	g_TextureIndex += 2;
}

void UninitTexture(void)
{
	for (unsigned int i = 0; i < g_TextureIndex; i++)
	{
		if (g_pTexture[i] != NULL)
		{
			g_pTexture[i]->Release();
			g_pTexture[i] = NULL;
		}
	}
}

/*------------------------------------------------------------------------------
	テクスチャのゲッター
------------------------------------------------------------------------------*/
ID3D11ShaderResourceView** GetTexture(int index)
{
	//indexの不正値チェック(負の値)
	if (index < 0)
		return NULL;

	//indexの不正値チェック(最大数オーバー)
	if (index >= (int)g_TextureIndex)
		return NULL;

	return &g_pTexture[index];
}
