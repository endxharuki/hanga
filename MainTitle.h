// ###############################################
// メイン制御　タイトル [MainTitle.h]
// Author: 西窪 統吉桜	Date: 2023/07/06
// ###############################################

#pragma once
#ifndef _MAIN_TITLE_H_
#define _MAIN_TITLE_H_

#include <iostream>
#include "ObjectPool.h"
#include "StateBase.h"
#include "BlockMap.h"
#include "main.h"

class MainTitle : public StateBase
{
public:
	MainTitle(std::string _key)
		: StateBase(_key){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

	static ObjectPool objectPool;	// layer1

private:
	void PlayerMove(int frameCnt);
	
	int BG[3];
	unsigned int strTex;

	unsigned int nextSe;
	int buttonFrame;
	int selectTex;
	int finishTex;
	int BbuttonTex;
	int frontMountainTex[2];
	int backMountainTex[2];
	D3DXVECTOR2 buttonStart = {SCREEN_WIDTH * 0.5f, 600.0f};
	D3DXVECTOR2 buttonSize = { SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.1f };
	D3DXVECTOR2 buttonDistance = { 300.0f, 200.0f };

	int frameCnt;

	bool isSelect;
	int selectNum;

	D3DXVECTOR2 logoPos = { SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5f - 100.0f };
	D3DXVECTOR2 logoSize = { 1200.0f, 530.0f };
	float logoSizeEase = 0;
	float logoRad = 0;
	float logoSizeTheta = 0;

	OnGameData* data = OnGameData::GetInstance();
	int drawMode = 0;

	BlockMap* mapInstance;
	Player* player;
};

#endif // !_MAIN_TITLE_H_

