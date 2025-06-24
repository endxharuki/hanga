// ###############################################
// マウントステート雛形 [MountStateBase.h]
// Author: 西窪 統吉桜	Date: 2023/08/23
// ###############################################

#pragma once

#ifndef _MOUNT_STATE_BASE_H_
#define _MOUNT_STATE_BASE_H_


#include <iostream>

class MountStateBase
{
public:
	MountStateBase(const std::string setKey)
		:key(setKey) {}

	~MountStateBase() {}

	// シーン開始時に読まれる
	virtual void SetUp() = 0;

	// シーン開始後毎フレーム読まれる
	virtual void OnUpdate() = 0;

	// シーン開始後毎フレーム読まれる
	virtual void OnDraw() = 0;

	// シーン終了時に読まれる
	virtual void CleanUp() = 0;


	std::string GetKeyName() { return key; }


	// 重ねて表示を終了するか取得する
	bool ShouldEndMountScene() { return mountEndRequest; }



protected:
	void EndMountRequest()
	{
		mountEndRequest = true;
	}

	void CleanRequest()
	{
		mountEndRequest = false;
	}

private:

	std::string key;


	bool mountEndRequest;


};

#endif // !_STATE_BASE_H_

