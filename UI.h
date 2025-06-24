#pragma once

#include "GameObject.h"
#include "UI_IDLE.h"

class UI : public GameObject
{
public:
	static UI* GetInstance()
	{
		if (Instance == nullptr)
			Instance = new UI();
		return Instance;
	}

	void Start()override
	{
		stateMachine.SetStartState(IdleKey);
	}
	
	void Update() override
	{
		stateMachine.Update();
	}

	void Draw() override
	{
		stateMachine.Draw();
	}

private:
	static UI* Instance;
	UI() 
	{
		stateMachine.SceneRegister(IdleKey, std::make_shared<UI_Idle>(IdleKey));
	}

	const std::string IdleKey;
};