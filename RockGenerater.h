#pragma once

#ifndef _ROCK_GENERATOR_H_
#define _ROCK_GENERATOR_H_

#include "GameObject.h"
#include "RockGeneraterIdle.h"
#include "Colider2D.h"
#include "ColiderPool.h"

class RockGenerater : public GameObject
{
public:
	RockGenerater(D3DXVECTOR2 pos, D3DXVECTOR2 size, bool left)
	{
		// 左上座標から中心座標を格納
		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		isLeft = left;

		stateMachine.SceneRegister(IdleKey, std::make_shared<RockGeneraterIdle>(IdleKey, this, left));
	}

	~RockGenerater(){}

	void Start() override
	{
		// 開始シーンの指定
		stateMachine.SetStartState(IdleKey);
	}

	void Update() override
	{
		stateMachine.Update();
	}

	void Draw() override
	{
		stateMachine.Draw();
	}

	bool IsLeftDir() { return isLeft; }

private:
	const std::string IdleKey = "Idle";
	
	bool isLeft = false;
};

#endif


