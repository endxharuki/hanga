#pragma once

#include "main.h"
class Camera
{
public:
	static Camera* GetInstance()
	{
		return Instance;
	};

	// 各種ゲッタ
	D3DXVECTOR2 GetOriginPos()	{ return originPos; }
	D3DXVECTOR2 GetCenterPos()	{ return centerPos; }
	D3DXVECTOR2 GetSize()		{ return size; }
	float GetLeftLimit() { return leftLimit; }
	float GetRightLimit() { return rightLimit; }

	// セッタ
	void SetPos(float x, float y) { SetPos(D3DXVECTOR2(x, y)); }
	void SetPos(D3DXVECTOR2 setPos);

	void SetLeftLimit(float x) { leftLimit = x; }
	void SetRightLimit(float x) { rightLimit = x; }

private:
	Camera():Camera(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)) {};

	Camera(D3DXVECTOR2 firstSetPos)
		: centerPos(firstSetPos)
		, size(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT))
	{ 
		originPos.x = centerPos.x - (size.x / 2);
		originPos.y = centerPos.y - (size.y / 2);
	}

	static Camera* Instance;

	D3DXVECTOR2 originPos;
	D3DXVECTOR2 centerPos;	// カメラの中央座標
	D3DXVECTOR2 size;

	// カメラの移動制限
	float leftLimit = 0;
	float rightLimit = 100000;
};

