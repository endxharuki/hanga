// ###############################################
// ステート雛形 [StateMachineBase.h]
// Author: 西窪 統吉桜	Date: 2023/07/06
// ###############################################

#pragma once

#ifndef _STATE_BASE_H_
#define _STATE_BASE_H_

#include <iostream>

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

protected:
	void ChangeRequest(const std::string nextKey)
	{
		ChangeSceneKey = nextKey;
		currentRequest = true;
	}

	void CleanRequest()
	{
		ChangeSceneKey = "";
		currentRequest = false;
	}

private:

	std::string key;

	bool		currentRequest;
	std::string ChangeSceneKey;

};

#endif // !_STATE_BASE_H_
