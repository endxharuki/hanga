#pragma once

#ifndef _COIN_H_
#define _COIN_H_

#include "GameObject.h"
#include "Colider2D.h"
#include "CoinIdle.h"

class Coin : public GameObject
{
public:
	Coin(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	{
		// �R���|�[�l���g�̒ǉ�
		componentPool.Add(new ColiderPool());

		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			transform.GetSize(),
			"Coin"
		));

		stateMachine.SceneRegister(IdleKey, std::make_shared<CoinIdle>(IdleKey, this));
	}

	~Coin()
	{

	}

	void Start() override
	{
		// �J�n�V�[���̎w��
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

