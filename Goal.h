#pragma once

#ifndef _GOAL_H_
#define _GOAL_H_

#include "GameObject.h"
#include "Colider2D.h"
#include "GoalIdle.h"
#include "GoalLight.h"

class Goal : public GameObject
{
public:
	Goal(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	{
		// コンポーネントの追加
		componentPool.Add(new ColiderPool());

		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			{transform.GetSize().x * 4.5f, transform.GetSize().y}
		));

		stateMachine.SceneRegister(IdleKey, std::make_shared<GoalIdle>(IdleKey, this));
		stateMachine.SceneRegister(LightKey, std::make_shared<GoalLight>(LightKey, this));
	}

	~Goal()
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

	void Light()
	{
		stateMachine.SetStartState(LightKey);
	}

private:
	const std::string IdleKey = "Idle";
	const std::string LightKey = "Light";
};

#endif // !_LAND_BLOCK_H_

