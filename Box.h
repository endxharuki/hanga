#pragma once

#ifndef _BOX_H_
#define _BOX_H_

#include "GameObject.h"
#include "Colider2D.h"
#include "BoxIdle.h"
#include "PushBox.h"

class Box : public GameObject
{
public:
	Box(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	{
		// コンポーネントの追加
		componentPool.Add(new ColiderPool());
		componentPool.Add(new PushBox(this));

		transform.SetPos(pos.x + size.x / 2, pos.y + size.y / 2);
		transform.SetSize(size);

		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			transform.GetSize()
		));

		stateMachine.SceneRegister(IdleKey, std::make_shared<BoxIdle>(IdleKey, this));
	}

	~Box()
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

