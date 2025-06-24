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
		// �R���|�[�l���g�ǉ�
		componentPool.Add(new ColiderPool());
		componentPool.Add(new Damaged());

		// �R���W�����ǉ�
		// ��
		ColiderPool* pool = GetComponent<ColiderPool>();
		pool->Add(Colider2D(&transform,
			D3DXVECTOR2(0.0f, 5.0f),
			D3DXVECTOR2(45.0f, 90.0f)
		));

		// �E�O���u����
		pool->Add(Colider2D(&transform,
			D3DXVECTOR2(22.5f, 0.0f),
			D3DXVECTOR2(18.5f, 20.0f)
		));

		// ���O���u����
		pool->Add(Colider2D(&transform,
			D3DXVECTOR2(-22.5f, 0.0f),
			D3DXVECTOR2(18.5f, 20.0f)
		));

		stateMachine.SceneRegister(inGameKey, std::make_shared<PlayerGame>(inGameKey, this));
	}

	void Start() override
	{
		// �J�n�V�[���̎w��
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

