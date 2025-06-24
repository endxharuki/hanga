#pragma once

#include "GameObject.h"
#include "SetedIdle.h"


class SetedBlock : public GameObject
{
public:
	SetedBlock(D3DXVECTOR2 setPos, D3DXVECTOR2 setSize) 
	{
		transform.SetPos(setPos);
		transform.SetSize(setSize);

		stateMachine.SceneRegister(IdleKey, std::make_shared<SetedIdle>(IdleKey, this, &shouldPrint));
	}

	void Start() override
	{
		// 開始シーンの指定
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

	bool GetPrint() { return shouldPrint; }
	void SetPrint(bool set) { shouldPrint = set; }


private:
	const std::string IdleKey = "SetedIdle";
	bool shouldPrint = true;	// 描画するべきかどうか
	
};

