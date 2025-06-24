// ###############################################
// �X�e�[�g���` [StateMachineBase.h]
// Author: ���E ���g��	Date: 2023/07/06
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
