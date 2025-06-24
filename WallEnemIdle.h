#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "Camera.h"

class WallEnemIdle : public StateBase
{
public:
	WallEnemIdle(std::string _key, GameObject* _obj, D3DXVECTOR2 _pos)
		: StateBase(_key), obj(_obj) {
		obj->GetTransform()->SetPos(_pos);
	}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void ColidPlayer();

	unsigned int tex[2];
	int drawMode;

	bool prevSwap = false;

	bool isDie = false;

	GameObject* obj;
	Camera* camera;
};
