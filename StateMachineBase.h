// ###############################################
// ステートマシン雛形 [StateMachineBase.h]
// Author: 西窪 統吉桜	Date: 2023/07/06
// ###############################################

#pragma once

#ifndef _STATE_MACHINE_BASE_H_
#define _STATE_MACHINE_BASE_H_

#include "StateBase.h"
#include "MountStateBase.h"
#include <unordered_map>
#include <memory>


class StateMachineBase
{
public:
	StateMachineBase() {}

	// 現在のステートの更新処理
	void Update()
	{
		if (mountState == NULL)
		{
			currentState->OnUpdate();

			if (concurrentMountState != NULL)
			{	// 同時処理するものがある場合
				concurrentMountState->OnUpdate();

				if (concurrentMountState->ShouldEndMountScene())
				{
					concurrentMountState->CleanUp();

					concurrentMountState = NULL;
					currentState->SetConcurrentMountScene(NULL);
				}
			}
		}
		else
		{
			mountState->OnUpdate();

			if (mountState->ShouldEndMountScene())
			{	// 重ねて表示を終了する場合
				mountState->CleanUp();
				mountState = NULL;
			}
		}

		if (currentState->RequestSceneChange())
		{	// シーン移行のリクエストを受け取った場合
			prevState = stateMap[currentState->GetKeyName()];
			currentState = stateMap[currentState->GetChangeSceneKey()];
			ChangeState();
		}

		if (currentState->RequestMountScene())
		{	// 重ねて表示するリクエストを受けとった場合
			std::shared_ptr<MountStateBase> state = mountMap[currentState->GetMountSceneKey()];
			if (state == mountState) return;

			mountState = state;
			mountState->SetUp();

			currentState->CompleteMountScene();
		}

		if (currentState->RequestConcurrentMountScene())
		{	// 同時処理対応のマウントシーンリクエストを受け取った場合
			std::shared_ptr<MountStateBase> state = mountMap[currentState->GetConcurrentMountSceneKey()];
			if (state == concurrentMountState)return;

			concurrentMountState = state;
			currentState->SetConcurrentMountScene(concurrentMountState);

			concurrentMountState->SetUp();

			currentState->CompleteConcurrentMountScene();
		}


	}

	// 現在のステートの描画処理
	void Draw()
	{
		currentState->OnDraw();

		// 重ねて表示する場合
		if (concurrentMountState != NULL) concurrentMountState->OnDraw();
		if (mountState != NULL)	mountState->OnDraw();
	};

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

	// マウントシーンの登録
	void MountSceneRegister(const std::string& name, std::shared_ptr<MountStateBase> mountState)
	{
		mountMap.insert(std::make_pair(name, mountState));
	}

	void SetStartState(const std::string setName)
	{
		auto set = stateMap.find(setName);
		if (set == stateMap.end()) return;

		currentState = set->second;
		currentState->SetUp();
	}

	void SelectState(const std::string& set) {
		prevState = stateMap[currentState->GetKeyName()];
		currentState = stateMap[set];
		ChangeState();
	}


private:

	void ChangeState()
	{
		prevState->CleanUp();
		currentState->SetUp();
	};

	std::shared_ptr<StateBase> currentState;
	std::shared_ptr<MountStateBase> mountState;
	std::shared_ptr<MountStateBase> concurrentMountState;	// 基底ステートと同時処理可能なマウントステート
	std::shared_ptr<MountStateBase> mount;
	std::shared_ptr<StateBase> prevState;
	std::unordered_map<std::string, std::shared_ptr<StateBase>> stateMap;
	std::unordered_map<std::string, std::shared_ptr<MountStateBase>> mountMap;
};
#endif // !_STATE_MACHINE_BASE_H_



