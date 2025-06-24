#pragma once

#ifndef _MOVE_SWITCH_BLOCK_H_
#define _MOVE_SWITCH_BLOCK_H_

#include "GameObject.h"
#include "MoveSwitchBlockIdle.h"
#include "Colider2D.h"
#include "ColiderPool.h"

class MoveSwitchBlock : public GameObject
{
public:
	MoveSwitchBlock(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	{
		// ������W���璆�S���W���i�[
		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		// �R���|�[�l���g�̒ǉ�
		componentPool.Add(new ColiderPool());
		
		// �R���W�����̒ǉ�
		GetComponent<ColiderPool>()->Add(
			Colider2D(&transform,
				D3DXVECTOR2(0.0f, 0.0f),
				transform.GetSize()
		));


		stateMachine.SceneRegister(IdleKey, std::make_shared<MoveSwitchBlockIdle>(IdleKey, this));
	}
	MoveSwitchBlock(D3DXVECTOR2 pos) : MoveSwitchBlock(pos, D3DXVECTOR2(100.0f, 100.0f)) {}
	

	~MoveSwitchBlock()
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

