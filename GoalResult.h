// ###############################################
// メイン制御　ゴールリザルトマウントシーン [GoalResult.h]
// Author: 西窪 統吉桜	Date: 2024/01/19
// ###############################################

#pragma once
#ifndef _GOAL_RESULT_H_
#define _GOAL_RESULT_H_

#include <vector>
#include "MountStateBase.h"
#include "OnGameData.h"
#include "DirectWrite.h"

class GoalResult : public MountStateBase
{
public:
	GoalResult(std::string _key)
		: MountStateBase(_key) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void MovePlayer();
	OnGameData* onGameData;

	FontData* font;
	DirectWrite* write;

	int frameCnt = 0;
	int moveCnt = 0;
	unsigned int boardHeadTex;
	unsigned int backBoardTex;
	unsigned int starCoinTex;

	unsigned int clearStrTex;

	// ランク描画
	unsigned int runkTex[4];
	int runk;
	float runkAlpha;
	float runkRot;

	// スタンプ描画
	unsigned int stamp[2];
	float stampAlpha[2];

	// SE
	unsigned int clearSe;
	unsigned int fireSe;
	unsigned int dodonSe;
	unsigned int stampSe;
	unsigned int scroolSe;

	// BGM
	unsigned int BGM;

	float cardAlpha[3];	// 取得お札のアルファ値

	int targetCoinNum = 0;
	int getCoinNum = 0;
	int drawCoinNum = 0;
	int targetEnemy = 0;
	int hitEnemy = 0;
	int drawEnemy = 0;
	int getCardNum = 0;

	D3DXVECTOR2 headPos;

	float boardY;

	bool isAnimEnd = false;
	bool moveEnd = false;
	bool printStr = false;
	bool printCoin = false;
	bool printEnemy = false;
	bool printScore = false;
	bool resultEnd = false;

	const int targetOverScore = 2000;
	const int cardGetScore = 2100 / 3;

	int coinTarget = 0;
	int enemyTarget = 0;

	int score = 0;
	int drawScore = 0;
	int bonus = 0;

	// ボーナスの付与が最初であるか
	bool coinOverFirst = false;
	bool enemyOverFirst = false;
	bool cardFirst[3];

	D3DXVECTOR2 goalAnimStart;	// ゴールアニメーションの開始地点

};

#endif // !_MAIN_TITLE_H_
