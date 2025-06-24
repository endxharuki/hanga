// ###############################################
// �Q�[���I�u�W�F�N�g���` [GameObject.h]
// Author: ���E ���g��	Date: 2023/07/06
// ###############################################

#pragma once

#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "main.h"
#include "StateMachineBase.h"
#include "Component.h"
#include "ComponentPool.h"
#include "Transform2D.h"
#include "Colider2D.h"


class GameObject
{
public:
	GameObject() {}
	~GameObject() {}

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;


	Transform2D* GetTransform() { return &transform; }

	template<typename T>
	T* GetComponent() {
		std::list<Component*>& pool = componentPool.GetPool();

		for (Component* component : pool)
		{
			if (component->IsType(typeid(T)))
				return static_cast<T*>(component);
		}
		return nullptr;
	}

protected:
	StateMachineBase stateMachine;		// �I�u�W�F�N�g�̏�ԊǗ�

	// �R���|�[�l���g��
	Transform2D		transform;			// �I�u�W�F�N�g�̈ʒu�Ȃǂ̊Ǘ��A�Ăяo���₷���悤�O�ɂ���
	ComponentPool componentPool;			// �R���|�[�l���g���Ǘ�
};

#endif // !_GAME_OBJECT_H_

