// ============================================
// メイン制御　タイトル [Main.Title.cpp]
// Author: 西窪 統吉桜	Date: 2023/07/07
// ============================================
#include "MainTitle.h"
#include "Fade.h"
#include "texture.h"
#include "sprite.h"
#include "OnGameData.h"
#include "input.h"
#include "Xinput.h"
#include "sound.h"
#include "DirectWrite.h"

FontData* titleData;
DirectWrite* titleWrite;


void MainTitle::SetUp()
{
	CleanRequest();

	titleData = new FontData();
	titleData->Color = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
	titleData->fontSize = 60;
	titleData->fontWeight = DWRITE_FONT_WEIGHT_NORMAL;

	titleWrite = new DirectWrite(titleData);
	titleWrite->Init();

}

void MainTitle::CleanUp()
{
	delete titleData;

	titleWrite->Release();
	delete titleWrite;
}

void MainTitle::OnUpdate()
{
	//シーン遷移ボタン
	if (IsButtonTriggered(0, BUTTON_A)||(GetKeyboardTrigger(DIK_SPACE)))
	{
		ChangeRequest("Select");
	}
	
}

void MainTitle::OnDraw()
{
	titleData->Color = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);
	titleWrite->SetFont(titleData);
	titleWrite->DrawString("スワップリント ！", D3DXVECTOR2(750.0f, 500), D2D1_DRAW_TEXT_OPTIONS_NONE);

	titleData->Color = D2D1::ColorF(0.7f, 0.3f, 0.0f, 1.0f);
	titleWrite->SetFont(titleData);
	titleWrite->DrawString("Aを押してスタート", D3DXVECTOR2(750.0f, 600), D2D1_DRAW_TEXT_OPTIONS_NONE);
}
