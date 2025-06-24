#pragma once

#include "StateBase.h"
#include "GameObject.h"

class BreakBlockIdle : public StateBase
{
public:
	BreakBlockIdle(std::string _key, GameObject* _obj)
		: StateBase(_key), obj(_obj){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	GameObject *obj;

	unsigned int blockTex[2];
	int			drawMode;
};

