#include "Camera.h"
#include "directxmath.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"

Camera* Camera::Instance = Camera::GetInstance();

void Camera::InitCamera()
{
	balenTex = LoadTexture((char*)"data/TEXTURE/baren.png");
	whiteTex = LoadTexture((char*)"data/TEXTURE/wasi.png");
	ssTex = 0;
	ySwap = 1.0f;

	frameCnt = 0;
}

void Camera::UpdateCamera()
{
	_isSwap = ySwap >= 0.0f ? false : true;

	D3DXMatrixOrthoOffCenterLH(&mtxProjection, 0.0f, size.x * zoom.x, size.y * zoom.y, 0.0f, 0.0f, 1.0f);

	D3DXMATRIX m;
	D3DXMATRIX projection;

	D3DXMatrixIdentity(&m);
	m(0, 0) = ySwap;	// y軸に鏡映させる

	projection = mtxProjection * m;

	// プロジェクションマトリックス設定
	SetProjectionMatrix(&projection);
}

void Camera::SetPos(D3DXVECTOR2 setPos)
{

	float leftPos = setPos.x - (size.x * zoom.x) / 2;
	float rightPos = setPos.x + (size.x * zoom.x) / 2;
	float topPos = setPos.y - (size.y * zoom.y) / 2;
	float bottomPos = setPos.y + (size.y * zoom.y) / 2;

	// x軸移動制限
	if (leftPos < leftLimit)
	{
		centerPos.x = leftLimit + (size.x * zoom.x) / 2;
	}
	else
	{
		if (rightPos > rightLimit)
		{
			centerPos.x = rightLimit - (size.x * zoom.x) / 2;
		}
		else
		{
			centerPos.x = setPos.x;
		}
	}


	// y軸移動制限
	if (topPos < 0)
	{
		centerPos.y = (size.y * zoom.y) / 2;
	}
	else
	{
		if (bottomPos > downLimit)
		{
			centerPos.y = downLimit - (size.y * zoom.y) / 2;
		}
		else
		{
			centerPos.y = setPos.y;
		}
	}

	originPos.x = centerPos.x - ((size.x * zoom.y) / 2);
	originPos.y = centerPos.y - ((size.y * zoom.y) / 2);
}

void Camera::SwapAnimationInit()
{
	frameCnt = 0;
	OnGameData::GetInstance()->SetSwapAnimStart(true);

	if (_isSwap)
	{	// 反転状態から通常に戻るなら
		OnGameData::GetInstance()->SetObjEnable(true);
		OnGameData::GetInstance()->ChangeSwap();
		AddSwap(2.0f);

		ssTex = LoadScreenShot();
		spilitTexture(ssTex, &ssR, &ssL);

		ssR_pos = { SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.5f };
		ssL_pos = { SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f };

		swapReturnAnim = true;
	}
	else
	{	// 反転状態に移行するなら
		OnGameData::GetInstance()->SetObjEnable(false);

		balenPos = {2100.0f, 250.0f};

		balenAlpha = 1.0f;
		whiteAlpha = 0.0f;

		ssTex = LoadScreenShot();
		ssAlpha = 1.0f;

		swapReturnAnim = false;
	}
}

void Camera::SwapAnimationUpdate()
{
	if (frameCnt == 0) SwapAnimationInit();
	++frameCnt;

	if (swapReturnAnim)
	{	// 反転状態から通常に戻るなら
		if (frameCnt <= 25)
		{
			ssR_pos.x += 38.4f;
			ssL_pos.x -= 38.4f;
			return;
		}

		OnGameData::GetInstance()->SetSwapAnim(false);
		OnGameData::GetInstance()->SetSwapAnimStart(false);
		frameCnt = 0;
	}
	else
	{	// 反転状態に移行するなら
		if (frameCnt <= 10)
		{
			whiteAlpha += 0.05f;
			AddZoom({-0.01f, -0.01f});
			return;
		}

		if (frameCnt <= 20)
		{
			balenPos.x -= 150.0f;
			balenPos.y += 20.0f;
			return;
		}

		if (frameCnt <= 25)
		{
			return;
		}

		if (frameCnt <= 30)
		{
			balenPos.x += 150.0f;
			balenPos.y += 30.0f;
			return;
		}

		if (frameCnt <= 35)
		{
			return;
		}

		if (frameCnt <= 45)
		{
			balenPos.x -= 200.0f;
			balenPos.y += 40.0f;
			return;
		}

		if (frameCnt <= 50) return;

		if (frameCnt <= 65)
		{
			balenAlpha = 0.0f;
			AddSwap(-0.135f);
			return;
		}

		if (frameCnt <= 70)
		{
			AddZoom({ 0.02f, 0.02f });
			return;
		}
		
		if (frameCnt == 71)
		{
			OnGameData::GetInstance()->SetObjEnable(true);
			OnGameData::GetInstance()->ChangeSwap();
		}
		
		// TODO: 透過できるようにする
		// SSがなぜか透過できないので
		/*if (frameCnt <= 65)
		{
			ssAlpha -= 0.1f;
			return;
		}*/

		OnGameData::GetInstance()->SetSwapAnim(false);
		OnGameData::GetInstance()->SetSwapAnimStart(false);
		frameCnt = 0;
	}
}

void Camera::SwapAnimationDraw()
{
	if (swapReturnAnim)
	{	// 反転状態から通常に戻るなら
		DrawSprite(ssR,
			ssR_pos.x, ssR_pos.y,
			SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT,
			0.0f, 0.0f,
			1.0f, 1.0f);

		DrawSprite(ssL,
			ssL_pos.x, ssL_pos.y,
			SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT,
			0.0f, 0.0f,
			1.0f, 1.0f);
	}
	else
	{	// 反転状態に移行するなら
		// SSの描画
		DrawSpriteLeftTopColor(ssTex,
			0.0f, 0.0f,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, ssAlpha);

		// 紙の描画
		DrawSpriteLeftTopColor(whiteTex,
			0.0f, 0.0f,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, whiteAlpha);

		// バレンの描画
		DrawSpriteColor(balenTex,
			balenPos.x, balenPos.y,
			balenSize.x, balenSize.y,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, balenAlpha);
	}
}
