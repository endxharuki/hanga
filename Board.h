#pragma once
#ifndef _BOARD_H_
#define _BOARD_H_

#include "GameObject.h"
#include "Colider2D.h"
#include "BoardIdle.h"

class  Board : public GameObject
{
public:
	Board(D3DXVECTOR2 pos, D3DXVECTOR2 size, int tex)
	{
		_tex = tex;

		// コンポーネントの追加
		componentPool.Add(new ColiderPool());

		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		GetComponent<ColiderPool>()->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			transform.GetSize(),
			"Board"
		));

		stateMachine.SceneRegister(IdleKey, std::make_shared<BoardIdle>(IdleKey, this));
	}

	~Board()
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

	const int GetTex() { return _tex; }

private:
	const std::string IdleKey = "Idle";
	int _tex = -1;
};

#endif // !_LAND_BLOCK_H_




