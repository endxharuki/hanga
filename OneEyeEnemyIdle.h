#pragma once
#ifndef ONEEYEENEMYIDLE_H_
#define ONEEYEENEMYIDLE_H_
#include "StateBase.h"
#include "GameObject.h"
#include "DirectWrite.h"
#include "ColiderPool.h"

#define ONEEYE_TEX_X (5)
#define ONEEYE_TEX_Y (2)

class OneEyeEnemyIdle : public StateBase
{
public:

	OneEyeEnemyIdle(std::string _key,GameObject* _obj) 
		: StateBase(_key),obj(_obj) {}
	~OneEyeEnemyIdle(){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;



private:

	void ColidBlock();
	void ColidPlayer();
	void ColidBox();
	void DeathRendition();

	unsigned int tex[2];
	int drawMode;
	D3DXVECTOR4 wUV;
	int walkTexCount = 0;

	int deathSE;

	bool prevSwap = false;

	bool isDie = false;

	GameObject* obj;
	ColiderPool* colidPool;

	float startPosY = 0.0f;
	
	float dieRot = 0.0f;

	bool deleteRequest = false;

	//移動の切り替え
	bool moveSide = true;
	int hitCount = 0;

	//地面判定
	bool isGround = false;

	// 反転用
	bool prevHasSwap;
	
	//文字出し用
	FontData* debugData;
	DirectWrite* write;

	D3DXVECTOR2 prevColidPos;
};

#endif
