// ============================================
// メイン制御　ゲームオーバー [Main.Title.cpp]
// Author: 西窪 統吉桜	Date: 2023/07/07
// ============================================
#include "MainInGameOver.h"
#include "Fade.h"
#include "texture.h"
#include "sprite.h"
#include "OnGameData.h"
#include "input.h"
#include "Xinput.h"
#include "DirectWrite.h"

FontData* gameoverData;
DirectWrite* gameoverWrite;


void MainGameOver::SetUp()
{
	CleanRequest();

	gameoverData = new FontData();
	gameoverData->Color = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
	gameoverData->fontSize = 60;
	gameoverData->fontWeight = DWRITE_FONT_WEIGHT_NORMAL;

	gameoverWrite = new DirectWrite(gameoverData);
	gameoverWrite->Init();
}

void MainGameOver::CleanUp()
{
	delete gameoverData;

	gameoverWrite->Release();
	delete gameoverWrite;
}

void MainGameOver::OnUpdate()
{
	if (IsButtonTriggered(0, BUTTON_A))
	{
		ChangeRequest("Game");
	}
	if (IsButtonTriggered(0, BUTTON_B))
	{
		ChangeRequest("Title");
	}
}

void MainGameOver::OnDraw()
{

	gameoverData->Color = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);
	gameoverData->fontSize = 150;
	gameoverWrite->SetFont(gameoverData);
	gameoverWrite->DrawString("ゲームオーバー…", D3DXVECTOR2(550.0f, 400), D2D1_DRAW_TEXT_OPTIONS_NONE);

	gameoverData->Color = D2D1::ColorF(0.7f, 0.3f, 0.0f, 1.0f);
	gameoverData->fontSize = 60;
	gameoverWrite->SetFont(gameoverData);
	gameoverWrite->DrawString("Bを押してタイトルへ", D3DXVECTOR2(750.0f, 600), D2D1_DRAW_TEXT_OPTIONS_NONE);

	gameoverData->Color = D2D1::ColorF(0.7f, 0.3f, 0.0f, 1.0f);
	gameoverWrite->SetFont(gameoverData);
	gameoverWrite->DrawString("Aを押してリトライ", D3DXVECTOR2(750.0f, 700), D2D1_DRAW_TEXT_OPTIONS_NONE);
	
}
