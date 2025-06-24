#pragma once
#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"
#include "sound.h"

class Rock;
class RockIdle :  public StateBase
{
public:
	RockIdle(std::string _key, GameObject* _obj, Rock** _enemRock)
		: StateBase(_key), obj(_obj), enemRock(_enemRock) {}
	~RockIdle() {
		if(enemRock) *enemRock = nullptr;
		StopSound(se);
	}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void ColidBlock();
	void ColidPlayer();
	void ColidObject();

	GameObject* obj;
	Rock** enemRock = nullptr;
	ColiderPool* pool;

	unsigned int tex[2];
	unsigned int se;

	const float eraseDistance = 400.0f;	// è¡ãéÇ≥ÇÍÇÈÇ‹Ç≈ÇÃâÊñ ògÇ©ÇÁÇÃãóó£

	// îΩì]óp
	bool prevHasSwap;
	int drawMode = 0;

	float rot = 0.0f;

	bool deleteRequest = false;

};

