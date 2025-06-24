#pragma once
#include "StateBase.h"
#include "GameObject.h"

class Bullet;
class BulletIdle :  public StateBase
{
public:
	BulletIdle(std::string _key, GameObject* _obj, Bullet** _enemBullet)
		: StateBase(_key), obj(_obj), enemBullet(_enemBullet) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void ColidBlock();
	void ColidPlayer();

	GameObject* obj;
	Bullet** enemBullet = nullptr;

	bool deleteRequest = false;

	unsigned int tex[2];

	const float eraseDistance = 200.0f;	// ���������܂ł̉�ʘg����̋���

	// ���]�p
	bool prevHasSwap;
	int drawMode = 0;

};

