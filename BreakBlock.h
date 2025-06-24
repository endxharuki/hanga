#pragma once

#ifndef _BREAK_BLOCK_H_
#define _BREAK_BLOCK_H_

#include "GameObject.h"
#include "BreakBlockIdle.h"
#include "Colider2D.h"
#include "ColiderPool.h"
#include "Break.h"

class BreakBlock : public GameObject
{
public:
	BreakBlock(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	{
		// ������W���璆�S���W���i�[
		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		// �R���|�[�l���g�̒ǉ�
		componentPool.Add(new ColiderPool());
		componentPool.Add(new Break(this));

		// �R���W�����̒ǉ�
		GetComponent<ColiderPool>()->Add(
			Colider2D(GetTransform())
		);

		stateMachine.SceneRegister(IdleKey, std::make_shared<BreakBlockIdle>(IdleKey, this));
	}

	~BreakBlock(){
		std::list<GameObject*>& map = BlockMap::GetInstance()->GetMap(0);

		auto it = map.begin();
		while (it != map.end())
		{
			if (*it == this)
			{
				 it = map.erase(it);
			}
			else
			{
				++it;
			}
		}
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

