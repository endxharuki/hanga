#pragma once
#include "GameObject.h"
#include "WallEnemIdle.h"
#include "ColiderPool.h"
#include "Enemmy.h"
#include "BlockMap.h"

class WallEnem : public GameObject
{
public:
	WallEnem(D3DXVECTOR2 pos)
	{
		componentPool.Add(new ColiderPool());
		componentPool.Add(new Enemmy());
		
		transform.SetPos(0.0f, MAP_HEIGHT / 2);
		transform.SetSize(150.0f, MAP_HEIGHT);

		// コリジョンの生成です
		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(140.0f, transform.GetSize().y)
		));

		stateMachine.SceneRegister(Idle, std::make_shared<WallEnemIdle>(Idle, this, pos));
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