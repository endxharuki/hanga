#pragma once
#ifndef ONEEYEENEMY_H_
#define ONEEYEENEMY_H_

#include "GameObject.h"
#include "OneEyeEnemyIdle.h"
#include "ColiderPool.h"

class OneEyeEnemy : public GameObject
{
public:

	OneEyeEnemy(D3DXVECTOR2 pos, D3DXVECTOR2 size) {

		componentPool.Add(new ColiderPool());

		transform.SetPos(pos);
		transform.SetSize(size);

		// コリジョンの生成
		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(size.x/2, size.y)
		));

		stateMachine.SceneRegister(Idle, std::make_shared<OneEyeEnemyIdle>(Idle, this));

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

