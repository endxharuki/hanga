#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"
#include "Player.h"

class Goal;
class GoalLight : public StateBase
{
public:
	GoalLight(std::string _key, Goal* _obj)
		: StateBase(_key), obj(_obj) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:

	unsigned int tex;
	int frameCnt = 0;
	int texCnt = 0;
	D3DXVECTOR4 uv;

	Goal* obj;
	Transform2D* transform;

	Player* p_player;
	ColiderPool* colidPool;
};


