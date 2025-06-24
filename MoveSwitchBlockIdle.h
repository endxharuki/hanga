#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"

#define MOVE_SWITCH_BLOCK_VELOCITY (2.0f)
#define MOVE_SWITCH_BLOCK_LANGE (500)

class MoveSwitchBlockIdle : public StateBase
{
public:
	MoveSwitchBlockIdle(std::string _key, GameObject* _obj)
		: StateBase(_key), obj(_obj){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;
	
	

private:
	void ColidPlayer();
	void ColidObject();

	void ChangeVec();
	void Move();

	GameObject *obj;
	ColiderPool* pool;

	D3DXVECTOR2 firstPos;

	int moveVec = -1;

	unsigned int onTex[2];
	unsigned int offTex[2];
	int			drawMode;

	int moveCount = 0;
};

