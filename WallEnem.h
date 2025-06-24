#pragma once
#include "GameObject.h"
#include "WallEnemIdle.h"
#include "ColiderPool.h"
#include "Enemmy.h"
#include "BlockMap.h"

class WallEnem : public GameObject
{
public:
	WallEnem(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	{
		componentPool.Add(new ColiderPool());
		componentPool.Add(new Enemmy());
		
		transform.SetPos(pos);
		transform.SetSize(size);

		// コリジョンの生成
		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(-size.x * 0.25f, 0.0f),
			D3DXVECTOR2(size.x * 0.5f, transform.GetSize().y)
		));

		stateMachine.SceneRegister(Idle, std::make_shared<WallEnemIdle>(Idle, this));
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