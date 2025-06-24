// ============================================
// ステージセレクト制御　クリア [Main.StageSelect.cpp]
// Author: 多田　タケル	Date: 2023/12/08
// ============================================
#include "MainInStageSelect.h"
#include "Fade.h"
#include "texture.h"
#include "sprite.h"
#include "OnGameData.h"
#include "input.h"
#include "Xinput.h"
#include "sound.h"
#include "DirectWrite.h"
#include <string.h>

FontData* selectData;
DirectWrite* selectWrite;

void MainStageSelect::SetUp()
{
	CleanRequest();

	selectData = new FontData();
	selectData->Color = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
	selectData->fontSize = 60;
	selectData->fontWeight = DWRITE_FONT_WEIGHT_NORMAL;

	selectWrite = new DirectWrite(selectData);
	selectWrite->Init();

	stageSelect = 1;
}

void MainStageSelect::CleanUp()
{
	delete selectData;

	selectWrite->Release();
	delete selectWrite;
}

void MainStageSelect::OnUpdate()
{
	if (GetKeyboardPress(DIK_A) || IsSwitchTriggerd(0, BUTTON_LEFT))
	{
		if (stageSelect <= STAGE_MIN)return;
		stageSelect--;
	}
	if (GetKeyboardPress(DIK_D) || IsSwitchTriggerd(0, BUTTON_RIGHT))
	{
		if (stageSelect >= STAGE_MAX)return;
		stageSelect++;
	}

	if (IsButtonTriggered(0, BUTTON_A) || (GetKeyboardTrigger(DIK_SPACE)))//決定
	{
		OnGameData::GetInstance()->SetStageNum(stageSelect);
		ChangeRequest("Game");
	}
	/*
	if (IsButtonTriggered(0, BUTTON_B) || (GetKeyboardTrigger(DIK_ESCAPE)))
	{
		ChangeRequest("Title");
	}*/
}

void MainStageSelect::OnDraw()
{
	selectData->Color = D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f);
	selectData->fontSize = 150;
	selectWrite->SetFont(selectData);
	selectWrite->DrawString("ステージ:"+std::to_string(stageSelect), D3DXVECTOR2(600.0f, 400), D2D1_DRAW_TEXT_OPTIONS_NONE);
	/*
	selectData->Color = D2D1::ColorF(0.7f, 0.3f, 0.0f, 1.0f);
	selectData->fontSize = 60;
	selectWrite->SetFont(selectData);
	selectWrite->DrawString("Bを押してタイトルへ", D3DXVECTOR2(750.0f, 600), D2D1_DRAW_TEXT_OPTIONS_NONE);
	*/
	selectData->Color = D2D1::ColorF(0.7f, 0.3f, 0.0f, 1.0f);
	selectData->fontSize = 60;
	selectWrite->SetFont(selectData);
	selectWrite->DrawString("Aを押して決定", D3DXVECTOR2(750.0f, 700), D2D1_DRAW_TEXT_OPTIONS_NONE);
}
