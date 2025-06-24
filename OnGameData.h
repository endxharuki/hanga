#pragma once

#ifndef _ON_GAME_DATA_H_
#define _ON_GAME_DATA_H_

#include "main.h"

class OnGameData
{
public:
	static OnGameData* GetInstance()
	{
		if (Instance == nullptr) Instance = new OnGameData;
		return Instance;
	}

	void LoadGameTexture();
	void InitData();

	unsigned int GetColidTex() { return colidTex; }
	unsigned int GetFadeTex() { return fadeTex; }
	unsigned int GetTopBGTex() { return topBGTex; }
	unsigned int GetLife1Tex() { return Life1; }
	unsigned int GetLife2Tex() { return Life2; }

	bool GetIsGameOver() { return isGameOver; }
	bool IsRetry() { return isRetry; }
	bool IsBack() { return isBack; }
	bool IsBackTitle() { return isBackTitle; }
	bool GetIsClear() { return isClear; }
	bool GetShouldEnd() { return shouldEnd; }

	int GetStageNum() { return currentStageNum; }

	int GetStarCoinNum() { return haveStarCoinNum; }
	void AddStarCoinNum() { haveStarCoinNum++; }

	int GetCoinNum() { return haveCoinNum; }
	void AddCoinNum(int add) { haveCoinNum += add; }

	const int GetHitEnemyCnt() const{ return hitEnemyCnt; }
	void AddHitEnemyNum() { ++hitEnemyCnt; }

	int GetScore() { return score; }
	void AddScore(int add) {
		if (score + add < 0)score = 0;
		else score += add;
	}

	int GetPlayerLife() { return playerLife; }
	void AddPlayerLife() { 
		if(playerLife + 1 <= 3)playerLife++; 
	}
	void SubPlayerLife() { playerLife--; }

	const int GetMaxScore() const { return maxScore; }
	void SetMaxScore(int set) { maxScore = set; }

	int GetCurrentLayer() { return currentLayer; }
	void AddLayer() { ++currentLayer; }
	void SubLayer() { --currentLayer; }

	int GetTargetCoinNum() { return targetCoinNum; }
	void SetTargetCoinNum(int set) { targetCoinNum = set; }

	int GetTargetEnemyNum() { return targetEnemyNum; }
	void SetTargetEnemyNum(int set) { targetEnemyNum = set; }


	float GetLayerAlpha() { return layer2Alpha; }
	void SetLayerAlpha(float set) { layer2Alpha = set; }

	void SetGameOver(bool set) { isGameOver = set; }
	void SetRetry(bool set) { isRetry = set; }
	void SetBack(bool set) { isBack = set; }
	void SetBackTitle(bool set) { isBackTitle = set; }
	void SetClear(bool set) { isClear = set; }
	void SetStageNum(int set) { currentStageNum = set; }
	void SetEnd(bool set) { shouldEnd = set; }
	void ChangeSwap() { hasSwap = !hasSwap; }

	bool HasSwap() { return hasSwap; }

	void SetSwapAnim(bool set) { swapAnim = set; }
	bool IsSwapAnim() { return swapAnim; }

	void SetObjEnable(bool set) { objEnable = set; }
	bool IsObjEnable() { return objEnable; }

	void SetSwapAnimStart(bool set) { swapAnimStart = set; }
	bool IsSwapAnimStart() { return swapAnimStart; }

	void SetCtrlUser(bool set) { isCtrlUser = set; }
	bool IsCtrlUser() { return isCtrlUser; }


	void SetIsCheckPoint(bool set) { isCheckPoint = set; }
	bool IsCheckPoint() { return isCheckPoint; }
	void SetCheckPoint(D3DXVECTOR2 pos) { checkPoint = pos; }
	const D3DXVECTOR2 GetCheckPoint() const{ return checkPoint; }

private:

	static OnGameData* Instance;
	OnGameData() {
	}

	unsigned int colidTex;
	unsigned int fadeTex;
	unsigned int topBGTex;
	unsigned int Life1;
	unsigned int Life2;
	
	int haveStarCoinNum = 0;
	int haveCoinNum = 0;
	int score = 0;
	int hitEnemyCnt = 0;
	int playerLife = 3;
	int maxScore = 0;

	// –Ú•W
	int targetCoinNum;
	int targetEnemyNum;

	int currentLayer = 0;
	float layer2Alpha = 1.0f;

	bool isGameOver = false;
	bool isRetry = false;
	bool isBack = false;
	bool isBackTitle = false;
	bool isClear = false;
	bool shouldEnd = false;
	bool hasSwap = false;
	bool swapAnim = false;
	bool swapAnimStart = false;
	bool objEnable = false;
	bool isCtrlUser = false;

	int currentStageNum = 1;

	bool isCheckPoint = false;
	D3DXVECTOR2 checkPoint = {0.0f, 0.0f};
};

#endif // !_ON_GAME_DATA_H_
