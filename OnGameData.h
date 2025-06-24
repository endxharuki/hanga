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
	bool GetIsClear() { return isClear; }
	bool GetShouldEnd() { return shouldEnd; }

	int GetStageNum() { return currentStageNum; }

	int GetStarCoinNum() { return haveStarCoinNum; }
	void AddStarCoinNum() { haveStarCoinNum++; }

	int GetPlayerLife() { return playerLife; }
	void AddPlayerLife() { 
		if(playerLife + 1 <= 3)playerLife++; 
	}
	void SubPlayerLife() { playerLife--; }

	int GetCurrentLayer() { return currentLayer; }
	void AddLayer() { ++currentLayer; }
	void SubLayer() { --currentLayer; }


	float GetLayerAlpha() { return layer2Alpha; }
	void SetLayerAlpha(float set) { layer2Alpha = set; }

	D3DXVECTOR2* GetFirstPos() { return firstPos; }

	void SetGameOver(bool set) { isGameOver = set; }
	void SetClear(bool set) { isClear = set; }
	void SetStageNum(int set) { currentStageNum = set; }
	void SetEnd(bool set) { shouldEnd = set; }
	void ChangeSwap() { hasSwap = !hasSwap; }

	void SG() { setGround = true; }

	// ‚â‚ß‚é
	bool SetGround() {
		if (setGround)
		{
			setGround = false;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool HasSwap() { return hasSwap; }

private:

	static OnGameData* Instance;
	OnGameData() {
		firstPos[0] = D3DXVECTOR2(120.0f, 805.0f);
		firstPos[1] = D3DXVECTOR2(120.0f, 805.0f);
		firstPos[2] = D3DXVECTOR2(120.0f, 805.0f);
	}

	unsigned int colidTex;
	unsigned int fadeTex;
	unsigned int topBGTex;
	unsigned int Life1;
	unsigned int Life2;
	
	int haveStarCoinNum = 0;
	int playerLife = 3;

	int currentLayer = 0;
	float layer2Alpha = 1.0f;

	bool isGameOver = false;
	bool isClear = false;
	bool shouldEnd = false;
	bool hasSwap = false;

	// ‚â‚ß‚é
	bool setGround = false;

	int currentStageNum = 1;

	D3DXVECTOR2 firstPos[3];
};

#endif // !_ON_GAME_DATA_H_
