// ###############################################
// �X�e�[�g�}�V�����` [StateMachineBase.h]
// Author: ���E ���g��	Date: 2023/07/06
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

	// ���݂̃X�e�[�g�̍X�V����
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

	// ���݂̃X�e�[�g�̕`�揈��
	void Draw()
	{
		currentState->OnDraw();
	};

	// �I�����̂ݎg�p
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



