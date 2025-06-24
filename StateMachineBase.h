// ###############################################
// ステートマシン雛形 [StateMachineBase.h]
// Author: 西窪 統吉桜	Date: 2023/07/06
// ###############################################

#pragma once

#ifndef _STATE_MACHINE_BASE_H_
#define _STATE_MACHINE_BASE_H_

#include "StateBase.h"
#include <unordered_map>
#include <memory>


class StateMachineBase
{
public:
	StateMachineBase() {}
	~StateMachineBase() {}

	// 現在のステートの更新処理
	void Update()
	{
		if (currentState->RequestSceneChange())
		{
			prevState = stateMap[currentState->GetKeyName()];
			currentState = stateMap[currentState->GetChangeSceneKey()];
			ChangeState();
		}

		currentState->OnUpdate();
	}

	// 現在のステートの描画処理
	void Draw()
	{
		currentState->OnDraw();
	};

	// 終了時のみ使用
	void Clean()
	{
		currentState->CleanUp();
	}

	// シーンの登録
	// 第一引数: ステートのキー名, 第二引数 ステートの型
	void SceneRegister(const std::string& name, std::shared_ptr<StateBase> state)
	{
		stateMap.insert(std::make_pair(name, state));
	}

	void SetStartState(const std::string setName)
	{
		auto set = stateMap.find(setName);
		if (set == stateMap.end()) return;

		currentState = set->second;
		currentState->SetUp();
	}

private:

	void ChangeState()
	{
		prevState->CleanUp();
		currentState->SetUp();
	};

	std::shared_ptr<StateBase> currentState;
	std::shared_ptr<StateBase> prevState;
	std::unordered_map<std::string, std::shared_ptr<StateBase>> stateMap;
};
#endif // !_STATE_MACHINE_BASE_H_



