#include "UI_Idle.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "OnGameData.h"
#include "BlockMap.h"
#include "Camera.h"

#define SCORE_DIGIT 6
#define SCORE_MAX_FRAME_X 6					
#define SCORE_MAX_FRAME_Y 2					
#define SCORE_MAX_FRAME_W (1.0f / SCORE_MAX_FRAME_X)
#define SCORE_MAX_FRAME_H (1.0f / SCORE_MAX_FRAME_Y)


void UI_Idle::SetUp()
{
	CleanRequest();

	topBG_Tex = OnGameData::GetInstance()->GetTopBGTex();
	Life[3] = LoadTexture((char*)"data/TEXTURE/HP.PNG");
	Life[2] = LoadTexture((char*)"data/TEXTURE/HP2.PNG");
	Life[1] = LoadTexture((char*)"data/TEXTURE/HP1.PNG");
	Life[0] = LoadTexture((char*)"data/TEXTURE/HP0.PNG");

	ScoreTex = LoadTexture((char*)"data/TEXTURE/number.PNG");
	CoinTex = LoadTexture((char*)"data/TEXTURE/coin_2.PNG");
	MulTex = LoadTexture((char*)"data/TEXTURE/3_~.PNG");

	StarCoinTex[0] = LoadTexture((char*)"data/TEXTURE/11_Starcoin.PNG");
	StarCoinTex[1] = LoadTexture((char*)"data/TEXTURE/11_Starcoin_ten.PNG");

	font = new FontData();
	font->Color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);
	font->fontSize = 40;
	font->fontStyle = DWRITE_FONT_STYLE_OBLIQUE;

	write = new DirectWrite(font);
	write->Init();

}

void UI_Idle::CleanUp()
{
	write->Release();
	write = NULL;
	delete font;
}

void UI_Idle::OnUpdate()
{
	
}


void UI_Idle::OnDraw()
{
	D3DXVECTOR2 drawPos =  Camera::GetInstance()->GetOriginPos();

	RectDrawSpriteColor(OnGameData::GetInstance()->GetStarCoinNum() >= 1? StarCoinTex[0] : StarCoinTex[1],
		400.0f, 130.0f,
		150.0f, 150.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);

	RectDrawSpriteColor(OnGameData::GetInstance()->GetStarCoinNum() >= 2 ? StarCoinTex[0] : StarCoinTex[1],
		520.0f, 130.0f,
		150.0f, 150.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);

	RectDrawSpriteColor(OnGameData::GetInstance()->GetStarCoinNum() >= 3 ? StarCoinTex[0] : StarCoinTex[1],
		640.0f, 130.0f,
		150.0f, 150.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);

	RectDrawSpriteColor(Life[OnGameData::GetInstance()->GetPlayerLife()],
		150.0f, 150.0f,
		250.0f, 250.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);

	//int score = OnGameData::GetInstance()->GetScore();
	//write->DrawString(std::to_string(score), D3DXVECTOR2(1700.0f, 100.0f), D2D1_DRAW_TEXT_OPTIONS_NONE);
	
	int score = OnGameData::GetInstance()->GetScore();
	int currentDigit = 0;
	int a = 100000;
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		if (score / a == 0)currentDigit++;
		else break;
		a /= 10;
	}
	if (SCORE_DIGIT == currentDigit)currentDigit--;
	D3DXVECTOR2 drowPos;
	D3DXVECTOR2 drowSize = {70.0f,80.0};
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		int number;
		number = score % 10;
		score /= 10;
		drowPos.x = SCREEN_WIDTH - 100.0f - drowSize.x * i;
		drowPos.y = 70.0f;
		if (i < SCORE_DIGIT - currentDigit)
		{
			RectDrawSpriteColor(ScoreTex, drowPos.x, drowPos.y, drowSize.x, drowSize.y,
				number % SCORE_MAX_FRAME_X * SCORE_MAX_FRAME_W,
				number / SCORE_MAX_FRAME_X * SCORE_MAX_FRAME_H,
				SCORE_MAX_FRAME_W,
				SCORE_MAX_FRAME_H,
				1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	score = OnGameData::GetInstance()->GetCoinNum();
	currentDigit = 0;
	a = 100000;
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		if (score / a == 0)currentDigit++;
		else break;
		a /= 10;
	}
	if (SCORE_DIGIT == currentDigit)currentDigit--;
	drowPos.x = SCREEN_WIDTH - 100.0f - drowSize.x * (SCORE_DIGIT - currentDigit)+drowSize.x*0.25f;
	drowPos.y = 150.0f;
	RectDrawSpriteColor(MulTex, drowPos.x, drowPos.y, drowSize.x*0.5f, drowSize.y * 0.5f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
	drowPos.x = SCREEN_WIDTH - 100.0f - drowSize.x * (SCORE_DIGIT - currentDigit+1)+drowSize.x * 0.3f;
	drowPos.y = 150.0f;
	RectDrawSpriteColor(CoinTex, drowPos.x, drowPos.y, drowSize.x, drowSize.y,
		0.0f,0.0f,1.0f,1.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		int number;
		number = score % 10;
		score /= 10;
		drowPos.x = SCREEN_WIDTH - 100.0f - drowSize.x * i;
		drowPos.y = 150.0f;
		if (i < SCORE_DIGIT - currentDigit)
		{
			RectDrawSpriteColor(ScoreTex, drowPos.x, drowPos.y, drowSize.x, drowSize.y,
				number % SCORE_MAX_FRAME_X * SCORE_MAX_FRAME_W,
				number / SCORE_MAX_FRAME_X * SCORE_MAX_FRAME_H,
				SCORE_MAX_FRAME_W,
				SCORE_MAX_FRAME_H,
				1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
}