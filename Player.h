#pragma once

#include "GameObject.h"
#include "PlayerGame.h"
#include "ColiderPool.h"
#include "Colider2D.h"
#include "Damaged.h"

class Player : public GameObject
{
public:
	Player() 
	{
		// コンポーネント追加
		componentPool.Add(new ColiderPool());
		componentPool.Add(new Damaged());

		// コリジョン追加
		// 体
		ColiderPool* pool = GetComponent<ColiderPool>();
		pool->Add(Colider2D(&transform,
			D3DXVECTOR2(0.0f, 5.0f),
			D3DXVECTOR2(45.0f, 90.0f)
		));

		// 右グラブ判定
		pool->Add(Colider2D(&transform,
			D3DXVECTOR2(22.5f, 0.0f),
			D3DXVECTOR2(18.5f, 20.0f)
		));

		// 左グラブ判定
		pool->Add(Colider2D(&transform,
			D3DXVECTOR2(-22.5f, 0.0f),
			D3DXVECTOR2(18.5f, 20.0f)
		));

		stateMachine.SceneRegister(inGameKey, std::make_shared<PlayerGame>(inGameKey, this));
	}

	void Start() override
	{
		// 開始シーンの指定
		stateMachine.SetStartState(inGameKey);
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
	const std::string inGameKey = "Game";
};

