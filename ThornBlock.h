#pragma once

#ifndef _THORN_BLOCK_H_
#define _THORN_BLOCK_H_

#include "GameObject.h"
#include "ThornBlockIdle.h"
#include "Colider2D.h"
#include "ColiderPool.h"

class ThornBlock : public GameObject
{
public:
	ThornBlock(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	{
		// 左上座標から中心座標を格納
		transform.SetPos(pos.x + size.x / 2, pos.y + size.y / 2);
		transform.SetSize(size);

		// コンポーネントの追加
		componentPool.Add(new ColiderPool());

		// コリジョンの追加
		GetComponent<ColiderPool>()->Add(
			Colider2D(&transform,
				D3DXVECTOR2(0.0f, 0.0f),
				transform.GetSize(),
				"NOT"
		));

		// 針のダメージ判定
		GetComponent<ColiderPool>()->Add(
			Colider2D(&transform,
				D3DXVECTOR2(0.0f, 15.0f),
				D3DXVECTOR2(transform.GetSize().x, transform.GetSize().y),
				"Damage")
		);

		stateMachine.SceneRegister(IdleKey, std::make_shared<ThornBlockIdle>(IdleKey, this));
	}

	~ThornBlock()
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

private:
	const std::string IdleKey = "Idle";
};

#endif // !_LAND_BLOCK_H_

