#include "OnGameData.h"
#include "texture.h"

OnGameData* OnGameData::Instance = OnGameData::GetInstance();

void OnGameData::LoadGameTexture()
{
	colidTex = LoadTexture((char*)"data/TEXTURE/whiteColid.png");
	fadeTex = LoadTexture((char*)"data/TEXTURE/white.png");
	topBGTex = LoadTexture((char*)"data/TEXTURE/yama.png");
	Life1 = LoadTexture((char*)"data/TEXTURE/heart1.png");
	Life2 = LoadTexture((char*)"data/TEXTURE/heart2.png");
}

void OnGameData::InitData()
{
	isGameOver = false;
	isClear = false;
	isRetry = false;
	isBack = false;
	isBackTitle = false;
	hasSwap = false;
	swapAnim = false;
	objEnable = true;
	swapAnimStart = false;
	isCtrlUser = false;

	haveStarCoinNum = 0;
	haveCoinNum = 0;
	currentLayer = 0;
	hitEnemyCnt = 0;
	score = 0;
	maxScore = 0;
	targetCoinNum = 0;
	targetEnemyNum = 0;
	playerLife = 3;
}