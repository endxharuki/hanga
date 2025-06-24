#include "UI_Idle.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "OnGameData.h"
#include  "PlayerBlockGenerator.h"
#include "BlockMap.h"
#include "Camera.h"


void UI_Idle::SetUp()
{
	CleanRequest();

	topBG_Tex = OnGameData::GetInstance()->GetTopBGTex();
	Life[0] = OnGameData::GetInstance()->GetLife1Tex();
	Life[1] = OnGameData::GetInstance()->GetLife2Tex();

	StarCoinTex[0] = LoadTexture((char*)"data/TEXTURE/StarCoin1.png");
	StarCoinTex[1] = LoadTexture((char*)"data/TEXTURE/StarCoinFrame.png");
}

void UI_Idle::CleanUp()
{

}

void UI_Idle::OnUpdate()
{
	
}


void UI_Idle::OnDraw()
{
	D3DXVECTOR2 drawPos =  Camera::GetInstance()->GetOriginPos();

	DrawSpriteColor(OnGameData::GetInstance()->GetStarCoinNum() >= 1? StarCoinTex[0] : StarCoinTex[1],
		100.0f, 100.0f,
		100.0f, 100.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);

	DrawSpriteColor(OnGameData::GetInstance()->GetStarCoinNum() >= 2 ? StarCoinTex[0] : StarCoinTex[1],
		250.0f, 100.0f,
		100.0f, 100.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);

	DrawSpriteColor(OnGameData::GetInstance()->GetStarCoinNum() >= 3 ? StarCoinTex[0] : StarCoinTex[1],
		400.0f, 100.0f,
		100.0f, 100.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);

	DrawSpriteColor(OnGameData::GetInstance()->GetPlayerLife() >= 1 ? Life[0] : Life[1],
		100.0f, 250.0f,
		100.0f, 100.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);

	DrawSpriteColor(OnGameData::GetInstance()->GetPlayerLife() >= 2 ? Life[0] : Life[1],
		250.0f, 250.0f,
		100.0f, 100.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);

	DrawSpriteColor(OnGameData::GetInstance()->GetPlayerLife() >= 3 ? Life[0] : Life[1],
		400.0f, 250.0f,
		100.0f, 100.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);


}