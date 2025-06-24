#pragma once

#ifndef _PERMATION_BLOCK_H_
#define _PERMATION_BLOCK_H_

#include "GameObject.h"
#include "PermationBlockIdle.h"
#include "Colider2D.h"
#include "ColiderPool.h"

class PermationBlock : public GameObject
{
public:
	PermationBlock(D3DXVECTOR2 pos, D3DXVECTOR2 size, int type)
	{
		// ������W���璆�S���W���i�[
		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		// �R���|�[�l���g�̒ǉ�
		componentPool.Add(new ColiderPool());

		stateMachine.SceneRegister(IdleKey, std::make_shared<PermationBlockIdle>(IdleKey, this, type));
	}

	~PermationBlock()
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

