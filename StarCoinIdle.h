#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"
#include "Player.h"

class StarCoinIdle : public StateBase
{
public:
	StarCoinIdle(std::string _key, GameObject* _obj)
		: StateBase(_key), obj(_obj){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void ColidPlayer();

	GameObject *obj;
	Transform2D* transform;

	Player* p_player;
	ColiderPool* colidPool;

	float moveSign;

	unsigned int blockTex[2];
	int drawMode;

	bool prevHasSwap;
};

