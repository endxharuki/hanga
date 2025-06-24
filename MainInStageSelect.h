// ###############################################
// ステージセレクト制御　クリア [MainStageSelect.h]
// Author: タダ タケル	Date: 2023/12/08
// ###############################################

#pragma once
#ifndef _MAIN_STAGE_SELECT_H_
#define _MAIN_STAGE_SELECT_H_

#include <iostream>
#include "StateBase.h"
#include "main.h"

#define STAGE_MAX 4
#define STAGE_MIN 1

class MainStageSelect : public StateBase
{
public:
	MainStageSelect(std::string _key)
		: StateBase(_key) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

	void MoveFirst();
	void MoveSecond();
	void MoveThird();
	void MoveForce();

private:

	int stageSelect;
	int BG[3];
	int Stage[4];
	int select[4];
	int enemy = 0;

	int level = 0;

	unsigned int idleTex[2];
	unsigned int runTex[2];
	unsigned int B_buttonTex;

	float rU, rV, rUW, rVH;
	// アニメーション用
	int currentRunTex = 0;
	float count[2] = { 0.0f,0.0f };
	float side = 0.0f;
	float back = 0.0f;

	//歩きアニメーション用
	D3DXVECTOR4 iUV;

	int currentIdleTex = 0;

	bool isRun = false;
	bool isIdle = false;

	float mojisize[4];
	float rgb[4];

	float mojiPos;

	D3DXVECTOR2 playerPos;

};

#endif // !_MAIN_TITLE_H_

