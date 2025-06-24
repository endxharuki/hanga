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

	//�ړ��̐؂�ւ�
	bool moveSide = true;

	//�n�ʔ���
	bool isGround = false;

	// ���]�p
	bool prevHasSwap;
	
	//�����o���p
	FontData* debugData;
	DirectWrite* write;

	D3DXVECTOR2 prevColidPos;
};

#endif
