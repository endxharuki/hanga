#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"
#include "Rock.h"

class RockGeneraterIdle : public StateBase
{
public:
	RockGeneraterIdle(std::string _key, GameObject* _obj)
		: StateBase(_key), obj(_obj) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void Generate();

	GameObject* obj;
	Rock* rock = nullptr;

	const D3DXVECTOR2 rockVel = D3DXVECTOR2(5.0f, 0.0f);
	const D3DXVECTOR2 rockSize = D3DXVECTOR2(100.0f, 100.0f);

	unsigned int blockTex[2];
	int			drawMode;
};

