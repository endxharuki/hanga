#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"
#include "Player.h"

class CheckPointIdle : public StateBase
{
public:
	CheckPointIdle(std::string _key, GameObject* _obj)
		: StateBase(_key), obj(_obj) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void ColidPlayer();

	GameObject* obj;
	Transform2D* transform;

	Player* p_player;
	ColiderPool* colidPool;

	unsigned int checkPointIdle[2];
	unsigned int checkPointActive[2];
	unsigned int se;
	int drawMode;
	bool isCheckPointed = false;
};

