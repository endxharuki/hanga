#pragma once

#include "main.h"
#include <d3dx9.h>
#include "renderer.h"
#include "OnGameData.h"
class Camera
{
public:
	static Camera* GetInstance()
	{
		if (!Instance) Instance = new Camera();
		return Instance;
	};

	void InitCamera();

	// カメラの更新
	void UpdateCamera();

	// 各種ゲッタ
	D3DXVECTOR2 GetOriginPos()	{ return originPos; }
	D3DXVECTOR2 GetCenterPos()	{ return centerPos; }
	D3DXVECTOR2 GetSize()		{ return size; }
	D3DXVECTOR2 GetZoom() { return zoom; }
	float GetLeftLimit() { return leftLimit; }
	float GetRightLimit() { return rightLimit; }
	float GetDownLimit() { return downLimit; }

	// セッタ
	void SetPos(float x, float y) { SetPos(D3DXVECTOR2(x, y)); }
	void SetPos(D3DXVECTOR2 setPos);

	void SetZoom(float x, float y) { SetZoom(D3DXVECTOR2(x, y)); }
	void SetZoom(D3DXVECTOR2 setZoom) { 
		if (setZoom.x <= 0.1f) setZoom.x = 0.1f;
		if (setZoom.y <= 0.1f) setZoom.y = 0.1f;
		zoom = setZoom; }
	void AddZoom(D3DXVECTOR2 add) {
		if (add.x + zoom.x <= 0.1f || add.y + zoom.y <= 0.1f)return;
		zoom += add;
	}

	void SetLeftLimit(float x) { leftLimit = x; }
	void SetRightLimit(float x) { rightLimit = x; }
	void SetDownLimit(float y) { downLimit = y; }

	void AddSwap(float add) {
		if (add + ySwap < -1.0f) ySwap = -1.0f;
		else if (add + ySwap > 1.0f)ySwap = 1.0f;
		else ySwap += add;
	}

	bool IsSwap() { return _isSwap; }

	void SwapAnimationInit();
	void SwapAnimationUpdate();
	void SwapAnimationDraw();

private:
	Camera():Camera(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)) {};

	Camera(D3DXVECTOR2 firstSetPos)
		: centerPos(firstSetPos)
		, size(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT))
	{ 
		originPos.x = centerPos.x - (size.x / 2);
		originPos.y = centerPos.y - (size.y / 2);

		gameData = OnGameData::GetInstance();
	}

	static Camera* Instance;

	D3DXMATRIX			mtxProjection;	// プロジェクションマトリックス

	D3DXVECTOR2 originPos;
	D3DXVECTOR2 centerPos;	// カメラの中央座標
	D3DXVECTOR2 size;
	D3DXVECTOR2 zoom = D3DXVECTOR2(1.0f, 1.0f);	// カメラのズーム値 : 1.0fが通常の画面サイズとする

	// カメラの移動制限
	float leftLimit = 0;
	float rightLimit = 100000;
	float downLimit = 0;

	float ySwap = 1;	// 鏡映値(1.0f ~ -1.0f)

	bool _isSwap = false;

	OnGameData* gameData;

	// アニメーション用
	int frameCnt = 0;

	int balenTex;
	int whiteTex;
	int ssTex;	// スクリーンショット格納
	int ssR;	// スクリーンショット右
	int ssL;	// スクリーンショット左
	D3DXVECTOR2 ssR_pos = { 0.0f, 0.0f };
	D3DXVECTOR2 ssL_pos = { 0.0f, 0.0f };
	D3DXVECTOR2 balenPos = {0.0f, 0.0f};
	const D3DXVECTOR2 balenSize = {250.0f, 250.0f};

	bool swapReturnAnim = false;

	float balenAlpha = 0.0f;
	float whiteAlpha = 0.0f;
	float ssAlpha = 0.0f;
};

