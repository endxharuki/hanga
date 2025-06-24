#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"

class ThornBlockIdle : public StateBase
{
public:
	ThornBlockIdle(std::string _key, GameObject* _obj)
		: StateBase(_key), obj(_obj){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void ColidPlayer();

	GameObject *obj;
	ColiderPool* pool;

	unsigned int blockTex[2];
	int			drawMode;
};

