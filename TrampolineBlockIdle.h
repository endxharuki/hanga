#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"

class TrampolineBlockIdle : public StateBase
{
public:
	TrampolineBlockIdle(std::string _key, GameObject* _obj, D3DXVECTOR2 _pos, D3DXVECTOR2 _size, int _boundDirection)
		: StateBase(_key), obj(_obj), setPos(_pos), setSize(_size), boundDirection(_boundDirection){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	GameObject *obj;

	D3DXVECTOR2 setPos;
	D3DXVECTOR2 setSize;

	ColiderPool* pool;

	int boundDirection;	// 跳ねる向き  0左1右  未2両
	float moveSign;

	unsigned int blockTex[2];
	int drawMode;

	bool prevHasSwap;
};

