#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"
#include "Pushed.h"

class WoodBoxIdle : public StateBase
{
public:
	WoodBoxIdle(std::string _key, GameObject* _obj)
		: StateBase(_key), obj(_obj) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void ColidBox();
	void ColidPush(GameObject* target, Pushed* push);
	void ColidMap();
	void ColidPlayer();

	GameObject* obj;
	Transform2D* transform;
	ColiderPool* colidPool;

	float moveSign;

	unsigned int blockTex[2];
	int drawMode;

	bool prevHasSwap;
};


