// ###############################################
// ステート雛形 [StateMachineBase.h]
// Author: 西窪 統吉桜	Date: 2023/07/06
// ###############################################

#pragma once

#ifndef _STATE_BASE_H_
#define _STATE_BASE_H_

#include <iostream>
#include "MountStateBase.h"

class StateBase
{
public:
	StateBase(const std::string setKey)
		:key(setKey) {}

	~StateBase() {}

	// シーン開始時に読まれる
	virtual void SetUp() = 0;

	// シーン開始後毎フレーム読まれる
	virtual void OnUpdate() = 0;

	// シーン開始後毎フレーム読まれる
	virtual void OnDraw() = 0;

	// シーン終了時に読まれる
	virtual void CleanUp() = 0;


	std::string GetKeyName() { return key; }

	// 次のシーンに移行したいか取得する
	bool RequestSceneChange() { return currentRequest; }

	// 次のシーンのキー名を取得する
	std::string GetChangeSceneKey() { return ChangeSceneKey; }

	// 重ねて表示したいか取得する
	bool RequestMountScene() { return mountRequest; }
	bool RequestConcurrentMountScene() { return concurrentMountRequest; }
	// 重ねて表示を終了するか取得する
	bool ShouldEndMountScene() { return mountEndRequest; }
	void CompleteMountScene() { mountRequest = false; }
	void CompleteConcurrentMountScene() { concurrentMountRequest = false; }

	// 重ねて表示するキー名を取得する
	std::string GetMountSceneKey() { return MountSceneKey; }
	std::string GetConcurrentMountSceneKey() { return ConcurrentMountSceneKey; }

	// 同時処理するシーンを入れ込む
	void SetConcurrentMountScene(std::shared_ptr<MountStateBase> setScene) { currentConcurrentMountScene = setScene; }

protected:
	void ChangeRequest(const std::string nextKey)
	{
		ChangeSceneKey = nextKey;
		currentRequest = true;
	}

	void MountRequest(const std::string mountKey)
	{
		MountSceneKey = mountKey;
		mountRequest = true;
	}

	void ConcurrentMountRequest(const std::string mountKey)
	{
		ConcurrentMountSceneKey = mountKey;
		concurrentMountRequest = true;
	}

	void EndMountRequest()
	{
		mountEndRequest = true;
	}

	void CleanRequest()
	{
		ChangeSceneKey = "";
		currentRequest = false;
		mountRequest = false;
		mountEndRequest = false;
	}

	std::shared_ptr<MountStateBase> GetConcurrentMountScene() { return currentConcurrentMountScene; }

private:

	std::string key;

	bool		currentRequest;
	std::string ChangeSceneKey;

	bool mountRequest;
	bool concurrentMountRequest = false;
	bool mountEndRequest;
	std::string MountSceneKey;
	std::string ConcurrentMountSceneKey;

	std::shared_ptr<MountStateBase> currentConcurrentMountScene;

};

#endif // !_STATE_BASE_H_
