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

class MainGameOver : public MountStateBase
{
public:
	MainGameOver(std::string _key)
		: MountStateBase(_key){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:

	int frameCnt = 0;

	unsigned int BG_tex;
	unsigned int retryButton_tex;
	unsigned int returnButton_tex;
	unsigned int gameOverTex;
	unsigned int gameOverFontTex;
	unsigned int retryTex;
	unsigned int titleTex;
	unsigned int BbuttonTex;
	unsigned int buttonFrame;
	const int gameOverX = 5;
	const int gameOverY = 3;
	const int allTex = 11;
	int currentFrame = 0;
	const int speed = 5;
	float alpha = 1.0f;
	bool isSelect = false;
	int selectNum = 0;

	// BGM
	unsigned int BGM;

	bool isAnimeEnd = false;

	D3DXVECTOR4 gameOverUV;

	D3DXVECTOR2 buttonPos[2];
	D3DXVECTOR2 buttonSize;
	D3DXVECTOR2 playerPos;

	int currentButtonPos = 0;
};

#endif // !_MAIN_TITLE_H_

