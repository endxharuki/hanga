#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "DirectWrite.h"

class UI_Idle: public StateBase
{
public:
	UI_Idle(std::string _key)
		: StateBase(_key){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:

	GameObject* obj;

	unsigned int topBG_Tex;
	unsigned int Life[4];
	unsigned int StarCoinTex[2];
	unsigned int ScoreTex;
	unsigned int CoinTex;
	unsigned int MulTex;

	FontData* font;
	DirectWrite* write;
};

