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
#include "ObjectPool.h"
#include "Player.h"
#include "DirectWrite.h"
#include "BlockMap.h"
#include "BulletEnemmy.h"


class MainInGame : public StateBase
{
public:
	MainInGame(std::string _key)
		: StateBase(_key) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

	static ObjectPool objectPool;	// layer1
	static ObjectPool objectPool2;	// layer2

	static Player* player;
private:
	BlockMap* map = BlockMap::GetInstance();

	// 1フレーム前のスワップ状況を保持
	bool prevHasSwap;
	int drawMode = 0;
	


	unsigned int GameBG;
};

#endif // !_MAIN_TITLE_H_

