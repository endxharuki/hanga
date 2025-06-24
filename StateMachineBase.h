// ###############################################
// �X�e�[�g�}�V�����` [StateMachineBase.h]
// Author: ���E ���g��	Date: 2023/07/06
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

	// ���݂̃X�e�[�g�̍X�V����
	void Update()
	{
		if (mountState == NULL)
		{
			currentState->OnUpdate();

			if (concurrentMountState != NULL)
			{	// ��������������̂�����ꍇ
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
			{	// �d�˂ĕ\�����I������ꍇ
				mountState->CleanUp();
				mountState = NULL;
			}
		}

		if (currentState->RequestSceneChange())
		{	// �V�[���ڍs�̃��N�G�X�g���󂯎�����ꍇ
			prevState = stateMap[currentState->GetKeyName()];
			currentState = stateMap[currentState->GetChangeSceneKey()];
			ChangeState();
		}

		if (currentState->RequestMountScene())
		{	// �d�˂ĕ\�����郊�N�G�X�g���󂯂Ƃ����ꍇ
			std::shared_ptr<MountStateBase> state = mountMap[currentState->GetMountSceneKey()];
			if (state == mountState) return;

			mountState = state;
			mountState->SetUp();

			currentState->CompleteMountScene();
		}

		if (currentState->RequestConcurrentMountScene())
		{	// ���������Ή��̃}�E���g�V�[�����N�G�X�g���󂯎�����ꍇ
			std::shared_ptr<MountStateBase> state = mountMap[currentState->GetConcurrentMountSceneKey()];
			if (state == concurrentMountState)return;

			concurrentMountState = state;
			currentState->SetConcurrentMountScene(concurrentMountState);

			concurrentMountState->SetUp();

			currentState->CompleteConcurrentMountScene();
		}


	}

	// ���݂̃X�e�[�g�̕`�揈��
	void Draw()
	{
		currentState->OnDraw();

		// �d�˂ĕ\������ꍇ
		if (concurrentMountState != NULL) concurrentMountState->OnDraw();
		if (mountState != NULL)	mountState->OnDraw();
	};

	void Clean()
	{
		currentState->CleanUp();
	}


	// �V�[���̓o�^
	// ������: �X�e�[�g�̃L�[��, ������ �X�e�[�g�̌^
	void SceneRegister(const std::string& name, std::shared_ptr<StateBase> state)
	{
		stateMap.insert(std::make_pair(name, state));
	}

	// �}�E���g�V�[���̓o�^
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
	std::shared_ptr<MountStateBase> concurrentMountState;	// ���X�e�[�g�Ɠ��������\�ȃ}�E���g�X�e�[�g
	std::shared_ptr<MountStateBase> mount;
	std::shared_ptr<StateBase> prevState;
	std::unordered_map<std::string, std::shared_ptr<StateBase>> stateMap;
	std::unordered_map<std::string, std::shared_ptr<MountStateBase>> mountMap;
};
#endif // !_STATE_MACHINE_BASE_H_



