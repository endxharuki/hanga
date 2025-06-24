#pragma once

#ifndef _DASH_BLOCK_H_
#define _DASH_BLOCK_H_

#include "GameObject.h"
#include "DashBlockIdle.h"
#include "Colider2D.h"
#include "ColiderPool.h"

class DashBlock : public GameObject
{
public:
	DashBlock(D3DXVECTOR2 pos, D3DXVECTOR2 size, bool set)
	{
		isRight = set;

		// ������W���璆�S���W���i�[
		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		// �R���|�[�l���g�̒ǉ�
		componentPool.Add(new ColiderPool());

		stateMachine.SceneRegister(IdleKey, std::make_shared<DashBlockIdle>(IdleKey, this, pos, size, isRight));
	}

	~DashBlock()
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

	void SetRight(bool set) { isRight = set; }

	bool IsRight() {
		return isRight;
	}

private:
	const std::string IdleKey = "Idle";
	bool isRight;
};

#endif // !_LAND_BLOCK_H_

