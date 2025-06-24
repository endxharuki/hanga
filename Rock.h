#pragma once
#include "RockIdle.h"
#include "ColiderPool.h"

class Rock : public GameObject
{
public:
	Rock(void) : Rock(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(10.0f, 10.0f), D3DXVECTOR2(0.0f, 0.0f)) {}
	Rock(D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXVECTOR2 vel)
	{
		stateMachine.SceneRegister(Idle, std::make_shared<RockIdle>(Idle, this, nullptr));

		componentPool.Add(new ColiderPool());

		// ステータス初期化
		transform.SetPos(pos);
		transform.SetSize(size);
		transform.SetVel(vel);

		// コリジョンの生成
		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(100.0f, transform.GetSize().y),
			"Rock"
		));
	}
	Rock(Rock** enemRock, D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXVECTOR2 vel)
	{
		stateMachine.SceneRegister(Idle, std::make_shared<RockIdle>(Idle, this, enemRock));

		componentPool.Add(new ColiderPool());

		// ステータス初期化
		transform.SetPos(pos);
		transform.SetSize(size);
		transform.SetVel(vel);

		// コリジョンの生成
		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(transform.GetSize().x - 10.0f, transform.GetSize().y),
			"Rock"
		));
	}

	~Rock() {}

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
