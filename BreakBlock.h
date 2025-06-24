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
		// 左上座標から中心座標を格納
		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		// コンポーネントの追加
		componentPool.Add(new ColiderPool());
		componentPool.Add(new Break(this));

		// コリジョンの追加
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

