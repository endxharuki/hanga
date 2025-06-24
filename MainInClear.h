// ###############################################
// ���C������@�N���A [MainGoal.h]
// Author: ���E ���g��	Date: 2023/07/06
// ###############################################

#pragma once
#ifndef _MAIN_GOAL_H_
#define _MAIN_GOAL_H_

#include <iostream>
#include "StateBase.h"
#include "main.h"

class MainClear : public StateBase
{
public:
	MainClear(std::string _key)
		: StateBase(_key){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:

	unsigned int BG_tex;
	unsigned int overBG_tex;
	unsigned int retryButton_tex;
	unsigned int returnButton_tex;


	D3DXVECTOR2 buttonPos[2];
	D3DXVECTOR2 buttonSize;

	int currentButtonPos = 0;
};

#endif // !_MAIN_TITLE_H_

