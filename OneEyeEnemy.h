#pragma once
#ifndef ONEEYEENEMY_H_
#define ONEEYEENEMY_H_

#include "GameObject.h"
#include "OneEyeEnemyIdle.h"
#include "ColiderPool.h"

class OneEyeEnemy : public GameObject
{
public:

	OneEyeEnemy(D3DXVECTOR2 pos) {

		componentPool.Add(new ColiderPool());

		transform.SetSize(80, 80);

		// コリジョンの生成
		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(60.0f, transform.GetSize().y)
		));

		stateMachine.SceneRegister(Idle, std::make_shared<OneEyeEnemyIdle>(Idle, this, pos));

	}

	void Start() override
	{
		// 開始シーンの指定
		stateMachine.SetStartState(Idle);
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

	const std::string Idle = "Idle";


};

#endif

