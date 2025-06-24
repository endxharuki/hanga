// ###############################################
// メイン制御　ゲームシーン [MainInGame.h]
// Author: 西窪 統吉桜	Date: 2023/07/06
// ###############################################

#pragma once
#ifndef _MAIN_IN_GAME_H_
#define _MAIN_IN_GAME_H_

#include <iostream>
#include <vector>
#include "GameObject.h"
#include "OnGameData.h"
#include "ObjectPool.h"
#include "Player.h"
#include "DirectWrite.h"
#include "CheckPoint.h"
#include "BlockMap.h"
#include "EffectManager.h"


class MainInGame : public StateBase
{
public:
	MainInGame(std::string _key)
		: StateBase(_key) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

	static ObjectPool objectPool;
	static Player* player;
	static EffectManager effectManager;
	static Goal* goal;
	static CheckPoint* checkPoint;
private:
	BlockMap* map = BlockMap::GetInstance();
	OnGameData* data = OnGameData::GetInstance();

	Transform2D* playerTransform;

	// 1フレーム前のスワップ状況を保持
	bool prevHasSwap;
	bool isFirstClear;
	bool isFirstGameOver;
	int drawMode = 0;
	
	int  BGM;

	// テクスチャ
	unsigned int frontMountainTex[2];
	unsigned int backMountainTex[2];
	unsigned int frontCloudTex[2];
	unsigned int skyTex[2];

	// テクスチャuv
	D3DXVECTOR2 cloudUV;
	D3DXVECTOR2 frontCloudUV;
	float addCloudU;

	D3DXVECTOR2 frontMountainUV;
	D3DXVECTOR2 backMountainUV;
};

#endif // !_MAIN_TITLE_H_

