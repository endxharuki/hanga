#pragma once

#ifndef _LAND_BLOCK_H_
#define _LAND_BLOCK_H_

#include "GameObject.h"
#include "LandBlockIdle.h"
#include "Colider2D.h"
#include "ColiderPool.h"

class LandBlock : public GameObject
{
public:
	LandBlock(D3DXVECTOR2 pos, D3DXVECTOR2 size, int type)
	{
		// ������W���璆�S���W���i�[
		transform.SetPos(pos.x + size.x / 2, pos.y + size.y / 2);
		transform.SetSize(size);

		// �R���|�[�l���g�̒ǉ�
		componentPool.Add(new ColiderPool());

		stateMachine.SceneRegister(IdleKey, std::make_shared<LandBlockIdle>(IdleKey, this, type));
	}

	~LandBlock()
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

