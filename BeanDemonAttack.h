#pragma once
#include "StateBase.h"
#include "main.h"

class BeanDemon;
class BeanDemonAttack : public StateBase
{
public:
	BeanDemonAttack(std::string _key, BeanDemon* _obj)
		: StateBase(_key), obj(_obj) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void Shoot();

	BeanDemon* obj;

	// ”½“]—p
	bool prevHasSwap;
	int drawMode = 0;

	int frameCnt = 0;
	int shootFrame;

	int shootCnt = 0;
	const int maxShootCnt = 3;
};


