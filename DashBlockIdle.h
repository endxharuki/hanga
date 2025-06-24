#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"

class DashBlock;
class DashBlockIdle : public StateBase
{
public:
	DashBlockIdle(std::string _key, DashBlock* _obj, D3DXVECTOR2 _pos, D3DXVECTOR2 _size, int _isRight)
		: StateBase(_key), obj(_obj), setPos(_pos), setSize(_size){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	DashBlock* obj;

	D3DXVECTOR2 setPos;
	D3DXVECTOR2 setSize;

	ColiderPool* pool;

	float moveSign;

	unsigned int blockTex[2];
	int drawMode;

	bool prevHasSwap;
};

