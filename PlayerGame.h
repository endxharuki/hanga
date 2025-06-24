#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "DirectWrite.h"
#include "ColiderPool.h"
#include "Pushed.h"
#include "Damaged.h"

#include <chrono>

class Player;
class PlayerGame: public StateBase
{
public:
	PlayerGame(std::string _key, Player* _obj)
		: StateBase(_key), obj(_obj) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void Jump();
	void Move();
	void ColidBlock();
	void ColidLayer();
	void ColidOverMap();
	void ColidPush(GameObject* target, Pushed* pushed);
	void ColidObject();

	Player* obj;
	D3DXVECTOR2 prevColidPos;
	bool isDie = false;


	ColiderPool* colidPool;

	Damaged* damage;

	std::chrono::milliseconds firstSec;
	std::chrono::milliseconds prevFlash;
	bool prevDamage;

	// つかみ案内用テクスチャ
	unsigned int X_buttonTex;
	const D3DXVECTOR2 pushAnounceSize = { 50.0f, 50.0f };
	bool inPush = false;	// 箱に触れているか

	// テクスチャ
	unsigned int idleTex[2];
	unsigned int runTex[2];
	unsigned int walkTex[2];
	unsigned int jumpTex[2];
	unsigned int pushTex[2];
	unsigned int circleTex;
	int drawMode = 0;

	// すなぼこりテクスチャ
	unsigned int dashSand[2];
	unsigned int JumpSand[2];

	unsigned int jumpSe;
	unsigned int damageSe;
	
	// 走り用UV値
	float rU, rV, rUW, rVH;
	D3DXVECTOR4 wUV, iUV, jUV, pUV;
	int currentWalkTex;
	int currentIdleTex;
	int currentJumpTex;
	int currentPushTex;

	int frameCnt;
	int dashCnt;


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
	bool notCtrl = false;

	// 反転用に前フレームの反転状況を保管
	bool prevHasSwap;
	bool requestInputSwap = false;
	bool prevInput = false;

	// ジャンプ
	int currentJumpFrame = 0;
	static const int jumpFrame = 25;
	const float maxJumpHeight = 15.0f;
	float currentJumpHeight;

	// 移動
	float moveDir = 1.0f;
	float moveSign = 1.0f;
	float pushDir = 1.0f;
	bool isRun = false;
	bool isWalk = false;
	bool isIdle = false;

	// 当たり判定用
	const int COLID_CHECK_NUM = 20;
	int colidCheckNum = 0;
	float mapRightLimit = 0.0f;
};

