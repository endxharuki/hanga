#pragma once
#include "BulletIdle.h"
#include "ColiderPool.h"

class Bullet : public GameObject
{
public:
	Bullet(void) : Bullet(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(10.0f, 10.0f), D3DXVECTOR2(0.0f, 0.0f)) {}
	Bullet(D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXVECTOR2 vel)
	{
		stateMachine.SceneRegister(Idle, std::make_shared<BulletIdle>(Idle, this, nullptr));

		componentPool.Add(new ColiderPool());

		// ステータス初期化
		transform.SetPos(pos);
		transform.SetSize(size);
		transform.SetVel(vel);

		// コリジョンの生成
		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(60.0f, transform.GetSize().y),
			"Bullet"
		));
	}
	Bullet(Bullet** enemBullet, D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXVECTOR2 vel)
	{
		stateMachine.SceneRegister(Idle, std::make_shared<BulletIdle>(Idle, this, enemBullet));

		componentPool.Add(new ColiderPool());

		// ステータス初期化
		transform.SetPos(pos);
		transform.SetSize(size);
		transform.SetVel(vel);

		// コリジョンの生成
		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(60.0f, transform.GetSize().y),
			"Bullet"
		));
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
