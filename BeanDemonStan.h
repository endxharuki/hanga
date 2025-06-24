#pragma once
#include "StateBase.h"
#include "main.h"

class BeanDemon;
class BeanDemonStan : public StateBase
{
public:
	BeanDemonStan(std::string _key, BeanDemon* _obj)
		: StateBase(_key), obj(_obj) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	BeanDemon* obj;

	// ”½“]—p
	bool prevHasSwap;
	int drawMode = 0;

	int frameCnt = 0;
	int stanFrame = 180;
};



