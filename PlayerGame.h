#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "DirectWrite.h"
#include "ColiderPool.h"
#include "Pushed.h"
#include "Damaged.h"

#include <chrono>

class PlayerGame: public StateBase
{
public:
	PlayerGame(std::string _key, GameObject* _obj)
		: StateBase(_key), obj(_obj) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void Jump();
	void ColidBlock();
	void ColidLayer();
	void ColidPush(GameObject* target, Pushed* pushed);
	void ColidObject();
	void ColidBlockLine();

	GameObject* obj;
	D3DXVECTOR2 prevColidPos;
	bool isDie = false;


	ColiderPool* colidPool;
	Damaged* damage;

	std::chrono::milliseconds firstSec;
	std::chrono::milliseconds prevFlash;
	bool prevDamage;


	// テクスチャ
	unsigned int idleTex[2];
	unsigned int runTex[2];
	int drawMode = 0;
	
	// 走り用UV値
	float rU, rV, rUW, rVH;

	// アニメーション用
	int currentRunTex = 0;

	// font
	FontData* debugData;
	DirectWrite* write;

	// フラグ
	bool isGround;
	bool isJump;
	bool isGrab;
	bool pushJumpKey;
	bool pushGrabKey;
	bool drawPlayer = true;

	// 反転用に前フレームの反転状況を保管
	bool prevHasSwap;
	bool requestInputSwap = false;
	bool prevInput = false;

	// ジャンプ
	int currentJumpFrame = 0;
	static const int jumpFrame = 25;
	const float maxJumpHeight = 13.0f;
	float currentJumpHeight;

	// 移動
	float moveDir = 1.0f;
	float moveSign = 1.0f;
	bool isRun = false;

};

