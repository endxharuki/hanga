#pragma once
#ifndef ONEEYEENEMYIDLE_H_
#define ONEEYEENEMYIDLE_H_
#include "StateBase.h"
#include "GameObject.h"
#include "DirectWrite.h"
#include "ColiderPool.h"

class OneEyeEnemyIdle : public StateBase
{
public:

	OneEyeEnemyIdle(std::string _key,GameObject* _obj,D3DXVECTOR2 pos) 
		: StateBase(_key),obj(_obj) 
	{
		obj->GetTransform()->SetPos(pos);
	}
	~OneEyeEnemyIdle(){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;



private:

	void ColidBlock();
	void ColidPlayer();
	void ColidBox();

	unsigned int tex[2];
	int drawMode;

	bool prevSwap = false;

	bool isDie = false;

	GameObject* obj;
	ColiderPool* colidPool;

	bool deleteRequest = false;

	//移動の切り替え
	bool moveSide = true;

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
