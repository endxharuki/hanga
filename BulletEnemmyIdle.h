#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "BlockMap.h"
#include "Bullet.h"

#define BULLET_ENEMY_SHOT_FRAME (180)

class BulletEnemmyIdle : public StateBase
{
public:
	BulletEnemmyIdle(std::string _key, GameObject* _obj, D3DXVECTOR2 _pos)
		: StateBase(_key), obj(_obj) {
		obj->GetTransform()->SetPos(_pos);
	}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	bool ColidBullet();
	void Shot();
	void DeathRendition();
	void ColidPlayer();

	unsigned int tex[2];
	unsigned int idleTex[2];
	unsigned int attackTex[2];
	const int idleX = 5;
	const int idleY = 2;
	const int attackY = 3;
	const int attackAll = 11;

	D3DXVECTOR4 uv;

	bool isIdle = false;

	int frameCnt = 0;
	int shotFrame = 0;
	int idleFrame = 0;

	int drawMode;

	int shotSE;

	bool prevSwap = false;

	bool isDie = false;
	float dieRot = 0.0f;
	float startPosY = 0.0f;
	int shotFrameCount = 0;

	GameObject* obj;
	Bullet* bullet = nullptr;	// 現在出ている弾を格納する

	// 弾の発射に関するステータス
	const float shotHeight = 10.0f;	// 弾をオブジェクトのどの高さで発射するか
	const D3DXVECTOR2 bulletSize = D3DXVECTOR2(CHIP_X_SIZE, CHIP_Y_SIZE);
	const D3DXVECTOR2 bulletVel = D3DXVECTOR2(5.0f, 0.0f);
	float shotDir = -1.0f;
};
