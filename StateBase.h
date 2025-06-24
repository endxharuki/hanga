// ###############################################
// �X�e�[�g���` [StateMachineBase.h]
// Author: ���E ���g��	Date: 2023/07/06
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

	// �V�[���J�n���ɓǂ܂��
	virtual void SetUp() = 0;

	// �V�[���J�n�㖈�t���[���ǂ܂��
	virtual void OnUpdate() = 0;

	// �V�[���J�n�㖈�t���[���ǂ܂��
	virtual void OnDraw() = 0;

	// �V�[���I�����ɓǂ܂��
	virtual void CleanUp() = 0;


	std::string GetKeyName() { return key; }

	// ���̃V�[���Ɉڍs���������擾����
	bool RequestSceneChange() { return currentRequest; }

	// ���̃V�[���̃L�[�����擾����
	std::string GetChangeSceneKey() { return ChangeSceneKey; }

	// �d�˂ĕ\�����������擾����
	bool RequestMountScene() { return mountRequest; }
	bool RequestConcurrentMountScene() { return concurrentMountRequest; }
	// �d�˂ĕ\�����I�����邩�擾����
	bool ShouldEndMountScene() { return mountEndRequest; }
	void CompleteMountScene() { mountRequest = false; }
	void CompleteConcurrentMountScene() { concurrentMountRequest = false; }

	// �d�˂ĕ\������L�[�����擾����
	std::string GetMountSceneKey() { return MountSceneKey; }
	std::string GetConcurrentMountSceneKey() { return ConcurrentMountSceneKey; }

	// ������������V�[������ꍞ��
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
