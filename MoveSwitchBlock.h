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
		// 左上座標から中心座標を格納
		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		// コンポーネントの追加
		componentPool.Add(new ColiderPool());
		
		// コリジョンの追加
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

