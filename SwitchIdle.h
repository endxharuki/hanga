#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"

class Switch;
class SwitchIdle : public StateBase
{
public:
	SwitchIdle(std::string _key, Switch* _obj,int _vec)
		: StateBase(_key), obj(_obj),drawVec(_vec){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

	

private:
	void ColidPlayer();
	void ColidObject();
	void ColidPlayerSwitch();
	void ColidObjectSwitch();

	void Switched();


	Switch* obj;
	ColiderPool* pool;

	D3DXVECTOR2 firstPos;

	unsigned int onTex[2];
	unsigned int offTex[2];
	unsigned int pushedTex[2];
	int			drawMode;
	int drawVec = 0;

	bool pushed = false;
	bool prevPshed = false;

	int moveCount = 0;
};

