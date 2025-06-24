#pragma once

#include "GameObject.h"
#include "PlayerGame.h"
#include "ColiderPool.h"
#include "Colider2D.h"
#include "Damaged.h"
#include "PlayerGoal.h"

class Player : public GameObject
{
public:
	Player(D3DXVECTOR2 pos, D3DXVECTOR2 size) 
	{
		transform.SetPos(pos);
		transform.SetSize(size);

		// コンポーネント追加
		componentPool.Add(new ColiderPool());
		componentPool.Add(new Damaged());

		// コリジョン追加
		// 体
		ColiderPool* pool = GetComponent<ColiderPool>();
		pool->Add(Colider2D(&transform,
			D3DXVECTOR2(0.0f, 5.0f),
			D3DXVECTOR2(transform.GetSize().x / 2, transform.GetSize().y * 0.9f)
		));

		// 右グラブ判定
		pool->Add(Colider2D(&transform,
			D3DXVECTOR2(30.0f, 0.0f),
			D3DXVECTOR2(18.5f, 20.0f)
		));

		// 左グラブ判定
		pool->Add(Colider2D(&transform,
			D3DXVECTOR2(-30.0f, 0.0f),
			D3DXVECTOR2(18.5f, 20.0f)
		));

		stateMachine.SceneRegister(inGameKey, std::make_shared<PlayerGame>(inGameKey, this));
		stateMachine.SceneRegister(inGoalKey, std::make_shared<PlayerGoal>(inGoalKey, this));
	}

	void Start() override
	{
		// 開始シーンの指定
		stateMachine.SetStartState(inGameKey);
	}

	void Update() override
	{
		stateMachine.Update();
	}

	void Draw() override
	{
		stateMachine.Draw();
	}

	void Goal()
	{
		stateMachine.SelectState(inGoalKey);
	}

	// 無理やりの実装
	void NonInput(int frame) {
		nonInputFrame = frame;
	}

	void SetNotJump(bool set) {
		notJump = set;
	}

	bool NotJump() {
		return notJump;
	}

	void SetGround() { setGround = true; }
	bool IsSetGround() { 
		bool buf = setGround;
		setGround = false;
		return buf; 
	}

	bool shouldInput() {
		--nonInputFrame;
		return nonInputFrame < 0;
	}

private:
	const std::string inGameKey = "Game";
	const std::string inGoalKey = "Goal";
	int nonInputFrame = 0;
	bool notJump = false;
	bool setGround = false;
};

