// ###############################################
// �}�E���g�X�e�[�g���` [MountStateBase.h]
// Author: ���E ���g��	Date: 2023/08/23
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

	// �V�[���J�n���ɓǂ܂��
	virtual void SetUp() = 0;

	// �V�[���J�n�㖈�t���[���ǂ܂��
	virtual void OnUpdate() = 0;

	// �V�[���J�n�㖈�t���[���ǂ܂��
	virtual void OnDraw() = 0;

	// �V�[���I�����ɓǂ܂��
	virtual void CleanUp() = 0;


	std::string GetKeyName() { return key; }


	// �d�˂ĕ\�����I�����邩�擾����
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

