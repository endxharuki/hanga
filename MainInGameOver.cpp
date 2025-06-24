// ============================================
// メイン制御　ゲームオーバー [Main.Title.cpp]
// Author: 西窪 統吉桜	Date: 2023/07/07
// ============================================
#include "MainInGameOver.h"
#include "MainInGame.h"
#include "Fade.h"
#include "texture.h"
#include "sprite.h"
#include "OnGameData.h"
#include "input.h"
#include "Xinput.h"
#include "DirectWrite.h"
#include "sound.h"

FontData* gameoverData;
DirectWrite* gameoverWrite;


void MainGameOver::SetUp()
{
	CleanRequest();
	StopSoundAll();

	gameoverData = new FontData();
	gameoverData->Color = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
	gameoverData->fontSize = 60;
	gameoverData->fontWeight = DWRITE_FONT_WEIGHT_NORMAL;

	gameoverWrite = new DirectWrite(gameoverData);
	gameoverWrite->Init();

	BG_tex = OnGameData::GetInstance()->GetFadeTex();
	gameOverTex = LoadTexture("data/TEXTURE/game_over.png");
	gameOverFontTex = LoadTexture("data/TEXTURE/game_over_re.png");
	retryTex = LoadTexture("data/TEXTURE/3_retry_re.png");
	titleTex = LoadTexture("data/TEXTURE/titlehe_re.png");
	BbuttonTex = LoadTexture("data/TEXTURE/Button_B.png");
	buttonFrame = LoadTexture((char*)"data/TEXTURE/buttonFrame.png");

	BGM = LoadSound("data/BGM/gameover.wav");

	frameCnt = 0;
	playerPos = MainInGame::player->GetTransform()->GetPos();
	isAnimeEnd = false;
	alpha = 1.0f;
	currentFrame = 0;
}

void MainGameOver::CleanUp()
{
	delete gameoverData;

	gameoverWrite->Release();
	delete gameoverWrite;
}

void MainGameOver::OnUpdate()
{
	++frameCnt;

	playerPos.y += 1.0f;

	if (!isAnimeEnd)
	{
		currentFrame = frameCnt / speed;
		gameOverUV.x = (float)(currentFrame % gameOverX * (1.0f / gameOverX));
		gameOverUV.y = (float)(currentFrame / gameOverX * (1.0f / gameOverY));
		gameOverUV.z = (float)1.0f / gameOverX;
		gameOverUV.w = (float)1.0f / gameOverY;

		if (currentFrame >= allTex)
		{
			isAnimeEnd = true;
			PlaySound(BGM, -1);
		}
	}
	else
	{
		if (IsButtonTriggered(0, BUTTON_A) || GetKeyboardTrigger(DIK_RETURN))
		{
			ControlKey(DIK_W);
			ControlKey(DIK_S);

			
		}
		if (IsButtonTriggered(0, BUTTON_DOWN) || GetKeyboardTrigger(DIK_S))
		{
			++selectNum;
			if (selectNum > 1)selectNum = 1;
		}
		else if (IsButtonTriggered(0, BUTTON_UP) || GetKeyboardTrigger(DIK_W))
		{
			--selectNum;
			if (selectNum < 0)selectNum = 0;
		}

		if (IsButtonTriggered(0, BUTTON_A) || GetKeyboardTrigger(DIK_RETURN))
		{
			if (selectNum == 0)
			{
				OnGameData::GetInstance()->SetRetry(true);
				EndMountRequest();
			}
			else if(selectNum == 1)
			{
				OnGameData::GetInstance()->SetSwapAnim(true);
				OnGameData::GetInstance()->SetBackTitle(true);
				EndMountRequest();
			}
		}

		alpha -= 0.001f;
		if (alpha < 0.0f) alpha = 0.0f;
	}
}

void MainGameOver::OnDraw()
{
	DrawSpriteColor(BG_tex,
		SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.7f);

	D3DXVECTOR2 drawPos = playerPos - Camera::GetInstance()->GetOriginPos();

	RectDrawSpriteColor(gameOverTex,
		drawPos.x, drawPos.y,
		250.0f, 250.0f,
		gameOverUV.x, gameOverUV.y,
		gameOverUV.z, gameOverUV.w,
		1.0f, 1.0f, 1.0f, alpha);

	if (isAnimeEnd)
	{
		RectDrawSpriteColor(gameOverFontTex,
			SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.25f,
			900.0f, 300.0f,
			0.0f,0.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f);

		float zoom = 1.0f;
		if (selectNum == 1)zoom = 1.2f;
		RectDrawSpriteColor(titleTex,
			SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.7f,
			450.0f*1.1f* zoom, 150.0f*1.1f* zoom,
			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f);
		zoom = 1.0f;
		if (selectNum == 0)zoom = 1.2f;
		RectDrawSpriteColor(retryTex,
			SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f,
			450.0f* zoom, 150.0f* zoom,
			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f);

		RectDrawSpriteColor(BbuttonTex,
			SCREEN_WIDTH * 0.5f-360, SCREEN_HEIGHT * 0.5f + selectNum * (SCREEN_HEIGHT * 0.2f),
			150.0f, 150.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f);
	}
}
