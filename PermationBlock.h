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
		// 左上座標から中心座標を格納
		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		// コンポーネントの追加
		componentPool.Add(new ColiderPool());

		stateMachine.SceneRegister(IdleKey, std::make_shared<PermationBlockIdle>(IdleKey, this, type));
	}

	~PermationBlock()
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

