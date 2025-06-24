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
	RockGenerater(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	{
		// 左上座標から中心座標を格納
		transform.SetPos(pos.x + size.x / 2 + 20.0f, pos.y + size.y);
		transform.SetSize(size * 2);


		stateMachine.SceneRegister(IdleKey, std::make_shared<RockGeneraterIdle>(IdleKey, this));
	}

	~RockGenerater()
	{

	}

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

private:
	const std::string IdleKey = "Idle";
};

#endif


