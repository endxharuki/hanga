#include "OnGameData.h"
#include "texture.h"

OnGameData* OnGameData::Instance = OnGameData::GetInstance();

void OnGameData::LoadGameTexture()
{
	colidTex = LoadTexture((char*)"data/TEXTURE/whiteColid.png");
	fadeTex = LoadTexture((char*)"data/TEXTURE/white.png");
	topBGTex = LoadTexture((char*)"data/TEXTURE/TopBG.png");
	Life1 = LoadTexture((char*)"data/TEXTURE/heart1.png");
	Life2 = LoadTexture((char*)"data/TEXTURE/heart2.png");
}

void OnGameData::InitData()
{
	isGameOver = false;
	isClear = false;
	hasSwap = false;

	haveStarCoinNum = 0;
	currentLayer = 0;
}