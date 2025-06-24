#pragma once
#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"
#include "Player.h"

class Board;
class BoardIdle : public StateBase
{
public:
	BoardIdle(std::string _key, Board* _obj)
		: StateBase(_key), obj(_obj) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void ColidPlayer();

	Board* obj;
	Transform2D* transform;

	Player* p_player;
	ColiderPool* colidPool;

	float moveSign;

	unsigned int popUp;
	unsigned int blockTex[2];
	int drawMode;

	bool prevHasSwap;

	bool Pop = false;



};

