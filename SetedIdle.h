#pragma once

#ifndef _SETED_IDLE_H_
#define _SETED_IDLE_H_
#include "StateBase.h"
#include "GameObject.h"

class SetedIdle : public StateBase
{
public:
	SetedIdle(std::string _key, GameObject* _obj, bool* _shouldPrint)
		: StateBase(_key), obj(_obj), shouldPrint(_shouldPrint) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	GameObject* obj;
	unsigned int block_tex;

	bool* shouldPrint;
};

#endif