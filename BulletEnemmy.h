#pragma once
#include "GameObject.h"
#include "BulletEnemmyIdle.h"
#include "Enemmy.h"

class BulletEnemmy : public GameObject
{
public:
	BulletEnemmy(D3DXVECTOR2 pos)
	{
		componentPool.Add(new ColiderPool());
		componentPool.Add(new Enemmy());
		
		transform.SetSize(100, 100);

		// コリジョンの生成
		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(60.0f, transform.GetSize().y)
		));

		stateMachine.SceneRegister(Idle, std::make_shared<BulletEnemmyIdle>(Idle, this, pos));
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