// ###############################################
// メイン制御　クリア [MainGoal.h]
// Author: 西窪 統吉桜	Date: 2023/07/06
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
	void PlayerMove();

	unsigned int BG_tex;
	unsigned int overBG_tex;
	unsigned int retryButton_tex;
	unsigned int returnButton_tex;

	bool isPlayerMove;


	D3DXVECTOR2 buttonPos[2];
	D3DXVECTOR2 buttonSize;
	D3DXVECTOR2 goalAnimStart;	// ゴールアニメーションの開始地点

	int currentButtonPos = 0;
	int frameCnt = 0;
	int rank = 0;
};

#endif // !_MAIN_TITLE_H_

