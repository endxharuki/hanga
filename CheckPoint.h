#pragma once

#include "main.h"
#include "GameObject.h"
#include "ColiderPool.h"
#include "CheckPointIdle.h"

class CheckPoint : public GameObject
{
public:
	CheckPoint(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	{
		// コンポーネントの追加
		componentPool.Add(new ColiderPool());

		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			transform.GetSize(),
			"NOT"
		));

		stateMachine.SceneRegister(IdleKey, std::make_shared<CheckPointIdle>(IdleKey, this));
	}
	~CheckPoint() = default;

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
