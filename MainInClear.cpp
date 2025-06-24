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

	rank = OnGameData::GetInstance()->GetStarCoinNum();
}

void MainClear::CleanUp()
{
	delete clearData;

	clearWrite->Release();
	delete clearWrite;
}

void MainClear::OnUpdate()
{
	if (IsButtonTriggered(0, BUTTON_B) || GetKeyboardPress(DIK_B))
	{
		ChangeRequest("Select");
	}
}

void MainClear::OnDraw()
{
	clearData->Color = D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f);
	clearData->fontSize = 150;
	clearWrite->SetFont(clearData);
	clearWrite->DrawString("ゲームクリア！", D3DXVECTOR2(700.0f, 200), D2D1_DRAW_TEXT_OPTIONS_NONE);
	
	//{//ランクシステム（今はスターコイン獲得枚数）
	//	clearData->Color = D2D1::ColorF(0.7f, 0.3f, 0.0f, 1.0f);
	//	clearData->fontSize = 150;
	//	clearWrite->SetFont(clearData);
	//	clearWrite->DrawString("クリアランク", D3DXVECTOR2(720.0f, 400), D2D1_DRAW_TEXT_OPTIONS_NONE);

	//	if (rank == 3) {
	//		clearData->Color = D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f);
	//		clearData->fontSize = 150;
	//		clearWrite->SetFont(clearData);
	//		clearWrite->DrawString("S", D3DXVECTOR2(960.0f, 600), D2D1_DRAW_TEXT_OPTIONS_NONE);

	//	}

	//	if (rank == 2) {
	//		clearData->Color = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);
	//		clearData->fontSize = 150;
	//		clearWrite->SetFont(clearData);
	//		clearWrite->DrawString("A", D3DXVECTOR2(960.0f, 600), D2D1_DRAW_TEXT_OPTIONS_NONE);

	//	}

	//	if (rank == 1) {
	//		clearData->Color = D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f);
	//		clearData->fontSize = 150;
	//		clearWrite->SetFont(clearData);
	//		clearWrite->DrawString("B", D3DXVECTOR2(960.0f, 600), D2D1_DRAW_TEXT_OPTIONS_NONE);

	//	}

	//	if (rank == 0) {
	//		clearData->Color = D2D1::ColorF(0.7f, 0.3f, 0.0f, 1.0f);
	//		clearData->fontSize = 150;
	//		clearWrite->SetFont(clearData);
	//		clearWrite->DrawString("C", D3DXVECTOR2(960.0f, 600), D2D1_DRAW_TEXT_OPTIONS_NONE);

	//	}
	//}
	clearData->Color = D2D1::ColorF(0.7f, 0.3f, 0.0f, 1.0f);
	clearData->fontSize = 60;
	clearWrite->SetFont(clearData);
	clearWrite->DrawString("Bを押してタイトルへ", D3DXVECTOR2(750.0f, 1000), D2D1_DRAW_TEXT_OPTIONS_NONE);

	//clearData->Color = D2D1::ColorF(0.7f, 0.3f, 0.0f, 1.0f);
	//clearWrite->SetFont(clearData);
	//clearWrite->DrawString("Aを押して次のステージへ", D3DXVECTOR2(750.0f, 700), D2D1_DRAW_TEXT_OPTIONS_NONE);
}

void MainClear::PlayerMove()
{

}
