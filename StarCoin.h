#pragma once

#ifndef _STAR_COIN_H_
#define _STAR_COIN_H_

#include "GameObject.h"
#include "Colider2D.h"
#include "StarCoinIdle.h"

class StarCoin : public GameObject
{
public:
	StarCoin(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	{
		// コンポーネントの追加
		componentPool.Add(new ColiderPool());

		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			transform.GetSize(),
			"Coin"
		));

		stateMachine.SceneRegister(IdleKey, std::make_shared<StarCoinIdle>(IdleKey, this));
	}

	~StarCoin()
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

