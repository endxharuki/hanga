// ###############################################
// メイン制御　ゲームオーバー [MainGoal.h]
// Author: 西窪 統吉桜	Date: 2023/07/06
// ###############################################

#pragma once
#ifndef _MAIN_GAME_OVERL_H_
#define _MAIN_GAME_OVERL_H_

#include <iostream>
#include "StateBase.h"
#include "main.h"

class MainGameOver : public StateBase
{
public:
	MainGameOver(std::string _key)
		: StateBase(_key){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:

	unsigned int BG_tex;
	unsigned int retryButton_tex;
	unsigned int returnButton_tex;
	unsigned int OverBG_tex;

	D3DXVECTOR2 buttonPos[2];
	D3DXVECTOR2 buttonSize;

	int currentButtonPos = 0;
};

#endif // !_MAIN_TITLE_H_

