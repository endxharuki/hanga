#pragma once

#include "StateBase.h"
#include "main.h"
#include <string>

class Player;
class PlayerGoal : public StateBase
{
public:
	PlayerGoal(std::string _key, Player* _obj)
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

	Player* obj;

};
