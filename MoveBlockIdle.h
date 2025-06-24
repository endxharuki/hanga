#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"

#define MOVE_BLOCK_VELOCITY (2.0f)
#define MOVE_BLOCK_LANGE (400)

class MoveBlockIdle : public StateBase
{
public:
	MoveBlockIdle(std::string _key, GameObject* _obj)
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

	unsigned int blockTex[2];
	int			drawMode;

	int moveCount = 0;
};

