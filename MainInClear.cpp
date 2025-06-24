// ============================================
// メイン制御　クリア [Main.Title.cpp]
// Author: 西窪 統吉桜	Date: 2023/07/07
// ============================================
#include "MainInClear.h"
#include "Fade.h"
#include "texture.h"
#include "sprite.h"
#include "OnGameData.h"
#include "input.h"
#include "Xinput.h"
#include "sound.h"
#include "DirectWrite.h"

FontData* clearData;
DirectWrite* clearWrite;

void MainClear::SetUp()
{
	CleanRequest();

	clearData = new FontData();
	clearData->Color = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
	clearData->fontSize = 60;
	clearData->fontWeight = DWRITE_FONT_WEIGHT_NORMAL;

	clearWrite = new DirectWrite(clearData);
	clearWrite->Init();
}

void MainClear::CleanUp()
{
	delete clearData;

	clearWrite->Release();
	delete clearWrite;
}

void MainClear::OnUpdate()
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

void MainClear::OnDraw()
{
	clearData->Color = D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f);
	clearData->fontSize = 150;
	clearWrite->SetFont(clearData);
	clearWrite->DrawString("ゲームクリア！", D3DXVECTOR2(600.0f, 400), D2D1_DRAW_TEXT_OPTIONS_NONE);

	clearData->Color = D2D1::ColorF(0.7f, 0.3f, 0.0f, 1.0f);
	clearData->fontSize = 60;
	clearWrite->SetFont(clearData);
	clearWrite->DrawString("Bを押してタイトルへ", D3DXVECTOR2(750.0f, 600), D2D1_DRAW_TEXT_OPTIONS_NONE);

	clearData->Color = D2D1::ColorF(0.7f, 0.3f, 0.0f, 1.0f);
	clearWrite->SetFont(clearData);
	clearWrite->DrawString("Aを押して次のステージへ", D3DXVECTOR2(750.0f, 700), D2D1_DRAW_TEXT_OPTIONS_NONE);
}
