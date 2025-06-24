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
#include "Camera.h"
#include <string.h>

FontData* selectData;
DirectWrite* selectWrite;

static int selectBGM;

void MainStageSelect::SetUp()
{
	CleanRequest();
	StopSoundAll();
	AllControlKey();
	AllControlPad();

	selectData = new FontData();
	selectData->Color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);
	selectData->fontSize = 60;
	selectData->fontWeight = DWRITE_FONT_WEIGHT_NORMAL;

	selectWrite = new DirectWrite(selectData);
	selectWrite->Init();

	OnGameData::GetInstance()->InitData();
	Camera::GetInstance()->InitCamera();

	stageSelect = 1;

	BG[0] = LoadTexture((char*)"data/TEXTURE/StageSelect_BG.png");


	Stage[0] = LoadTexture((char*)"data/TEXTURE/stage1.png");
	Stage[1] = LoadTexture((char*)"data/TEXTURE/stage2.png");

	select[0] = LoadTexture((char*)"data/TEXTURE/3_1st.png");
	select[1] = LoadTexture((char*)"data/TEXTURE/3_2nd.png");
	select[2] = LoadTexture((char*)"data/TEXTURE/3_3rd.png");
	select[3] = LoadTexture((char*)"data/TEXTURE/3_×.png");

	B_buttonTex = LoadTexture((char*)"data/TEXTURE/Button_B.png");

	idleTex[0] = LoadTexture((char*)"data/TEXTURE/PLAYER/idle.png");
	idleTex[1] = LoadTexture((char*)"data/TEXTURE/PLAYER/idle.png");
	runTex[0] = LoadTexture((char*)"data/TEXTURE/PLAYER/run.png");
	runTex[1] = LoadTexture((char*)"data/TEXTURE/PLAYER/run.png");
	enemy = LoadTexture((char*)"data/TEXTURE/hitotume_front.png");

	mojisize[0] = 100.0f;
	mojisize[1] = 0.0f;

	mojiPos = 0.0f;

	rgb[0] = 0.0f;
	rgb[1] = 0.3f;
	rgb[2] = 0.0f;

	back = 0.0f;
	side = 1.0f;

	isRun = false;;
	isIdle = true;

	playerPos = { 250.0f, 850.0f };

	level = 1;

	selectBGM = LoadSound((char*)"data/BGM/title.wav");
	SetVolume(selectBGM, 0.45f);

	PlaySound(selectBGM, -1);
}

void MainStageSelect::CleanUp()
{
	delete selectData;

	selectWrite->Release();
	delete selectWrite;
}

void MainStageSelect::OnUpdate()
{
	if (isRun == false) {
		if (GetKeyboardPress(DIK_A) || IsSwitchTriggerd(0, BUTTON_LEFT) || GetLeftStick(0).x < 0.0f)
		{
			if (stageSelect <= STAGE_MIN)return;
			if (isRun == true)return;
			isIdle = false;
			isRun = true;
			back = 1.0f;
			side = -1.0f;

		}
		if (GetKeyboardPress(DIK_D) || IsSwitchTriggerd(0, BUTTON_RIGHT) || GetLeftStick(0).x > 0.0f)
		{
			if (stageSelect >= STAGE_MAX)return;
			if (isRun == true)return;
			isRun = true;
			isIdle = false;
			back = 0.0f;
			side = 1.0f;
		}
	}
	if (isRun == false) {
		if (IsButtonTriggered(0, BUTTON_A) || (GetKeyboardPress(DIK_SPACE)))//決定
		{
			rgb[2] = 0.2;
			mojiPos = 50.0f;

			OnGameData::GetInstance()->SetStageNum(stageSelect);
			ChangeRequest("Game");
		}
	}
	if (isRun) {

		mojisize[0] = 0.0f;
		mojisize[1] = 0.0f;

		rgb[0] = 0.3f;
		rgb[1] = 0.3f;


		rU = (currentRunTex % 5) * (1.0f / (5));
		rV = (currentRunTex / 5) * (1.0f / 2);
		rUW = 1.0f / 5;
		rVH = 1.0f / 2;

		if (currentRunTex >= 7) {
			currentRunTex = 0;
		}

		//プレイヤーの走るモーションを少しゆっくり
		if (count[0] == 3)
		{
			++currentRunTex;
			count[0] = 0;
		}
		count[0]++;

		//どのステージを選んでるかでプレイヤーの動きを変える
		if (stageSelect == 1) {
			MoveFirst();
		}
		if (stageSelect == 2) {
			MoveSecond();
		}
		if (stageSelect == 3) {
			MoveThird();
		}
		if (stageSelect == 4) {
			MoveForce();
		}

	}
	else
	{
		count[0] = 0;
		currentRunTex = 0;
	}
	// 待機アニメーション処理
	if (isIdle)
	{
		iUV.x = (currentIdleTex % 5) * (1.0f / 5);
		iUV.y = (currentIdleTex / 5) * (1.0f / 2);
		iUV.z = 1.0f / 5;
		iUV.w = 1.0f / 2;

		++currentIdleTex;


		if (stageSelect == 1)
		{
			mojisize[0] = 100.0f;
			rgb[0] = 0.0f;
		}
		if (stageSelect == 2)
		{
			mojisize[1] = 100.0f;
			rgb[1] = 0.0f;
		}
	}
	else
	{
		count[1] = 0;
		currentIdleTex = 0;
	}

	
	if (IsButtonTriggered(0, BUTTON_B) || (GetKeyboardTrigger(DIK_ESCAPE)))
	{
		ChangeRequest("Title");
	}
}

void MainStageSelect::OnDraw()
{
	DrawSpriteLeftTop(BG[0], 0.0f, 0.0f, 1980.0f, 1080.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	if (stageSelect != 4)DrawSpriteColor(select[stageSelect - 1], 1600.0f, 110.0f, 280.0f, 100.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	else {
		DrawSpriteColor(select[stageSelect - 1], 1500.0f, 110.0f, 100.0f, 100.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		DrawSpriteColor(select[stageSelect - 1], 1600.0f, 110.0f, 100.0f, 100.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		DrawSpriteColor(select[stageSelect - 1], 1700.0f, 110.0f, 100.0f, 100.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	}


	if (isRun) {
		DrawSpriteColor(runTex[0],
			playerPos.x, playerPos.y,
			100.0f, 100.0f,
			rU + (rUW * back), rV,
			rUW * side, rVH,
			1.0f, 1.0f, 1.0f, 1.0f);
	}
	if (isIdle) {
		DrawSpriteColor(idleTex[0],
			playerPos.x, playerPos.y,
			100.0f, 100.0f,
			iUV.x + (iUV.z * back), iUV.y,
			iUV.z * side, iUV.w,
			1.0f, 1.0f, 1.0f, 1.0f);
	}

	if (isIdle) {
		/*if (stageSelect == 1) {

			DrawSprite(Stage[0], 350.0f, 450.0f, 600.0f, 350.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		}
		if (stageSelect == 2) {

			DrawSprite(Stage[1], 1300.0f, 230.0f, 600.0f, 350.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		}*/
	}
	selectData->Color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);
	selectData->fontSize = 75;
	selectData->fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
	selectWrite->SetFont(selectData);
	selectWrite->DrawString("難易度"/*+std::to_string(stageSelect)*/, D3DXVECTOR2(120.0f, 980), D2D1_DRAW_TEXT_OPTIONS_NONE);
	DrawSpriteColor(select[3], 400.0f, 1040.0f, 80.0f, 80.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	float posx = 0.0f;

	for (int i = 0; i < level; i++) {
		DrawSpriteColor(enemy, 500.0f + posx, 1030.0f, 80.0f, 60.0, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f);
		posx += 50.0f;
	}
	/*
	selectData->Color = D2D1::ColorF(0.7f, 0.3f, 0.0f, 1.0f);
	selectData->fontSize = 60;
	selectWrite->SetFont(selectData);
	selectWrite->DrawString("Bを押してタイトルへ", D3DXVECTOR2(750.0f, 600), D2D1_DRAW_TEXT_OPTIONS_NONE);
	*/
	selectData->Color = D2D1::ColorF(0.0, 0.0, 0.0f, 1.0f);
	selectData->fontSize = 60;
	selectData->fontWeight = DWRITE_FONT_WEIGHT_BOLD;
	selectWrite->SetFont(selectData);
	selectWrite->DrawString("を押して決定", D3DXVECTOR2(750.0f, 700 + mojiPos), D2D1_DRAW_TEXT_OPTIONS_NONE);

	DrawSpriteColor(B_buttonTex,
		690.0f, 740.0f,
		100.0f, 100.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
}

void MainStageSelect::MoveFirst()
{
	if (playerPos.x <= 420.0f) {
		playerPos.x += 5.0f;
	}
	else if (playerPos.y >= 750.0f) {
		playerPos.y -= 5.0f;
	}
	else if (playerPos.x <= 780.0f) {
		playerPos.x += 5.0f;
		playerPos.y -= 2.0f;
	}
	else if (playerPos.y >= 520.0f) {
		playerPos.y -= 5.0f;
	}
	else if (playerPos.y <= 520.0f) {
		isRun = false;
		isIdle = true;
		stageSelect++;
		level = 2;
	}

}

void MainStageSelect::MoveSecond()
{
	if (back == 0.0f) {

		if (playerPos.x <= 850.0f) {
			playerPos.x += 5.0f;
		}
		else if (playerPos.x <= 960.0f) {
			playerPos.x += 4.0f;
			playerPos.y += 5.0f;
		}
		else if (playerPos.x <= 1040.0f) {
			playerPos.x += 4.0f;
			playerPos.y -= 2.0f;
		}
		else if (playerPos.x <= 1070.0f) {
			playerPos.x += 2.0f;
			playerPos.y += 5.0f;
		}
		else if (playerPos.x <= 1160.0f) {
			playerPos.x += 5.0f;
			playerPos.y -= 2.0f;
		}
		else if (playerPos.x <= 1430.0f) {
			playerPos.x += 5.0f;
			playerPos.y += 2.5f;
		}
		else if (playerPos.x >= 1430.0f) {
			isRun = false;
			isIdle = true;
			stageSelect++;
			level = 3;
		}
	}
	if (back == 1.0f) {

		if (playerPos.y <= 750.0f - 144.0f) {
			playerPos.y += 5.0f;
		}
		else if (playerPos.x >= 420.0f) {
			playerPos.x -= 5.0f;
			playerPos.y += 2.0f;
		}
		else if (playerPos.y <= 850.0f) {
			playerPos.y += 5.0f;
		}
		else if (playerPos.x >= 250.0f) {
			playerPos.x -= 5.0f;
		}
		else if (playerPos.x <= 250.0f) {
			isRun = false;
			isIdle = true;
			stageSelect--;
			level = 1;
		}
	}

}

void MainStageSelect::MoveThird()
{
	if (back == 0.0f) {
		if (playerPos.x <= 1500.0f) {
			playerPos.x += 5.0f;
			playerPos.y -= 5.0f;
		}
		else if (playerPos.x <= 1560.0f) {
			playerPos.x += 5.0f;
			playerPos.y += 5.0f;
		}
		else if (playerPos.x <= 1730.0f) {
			playerPos.x += 2.0f;
			playerPos.y -= 5.0f;
		}
		else if (playerPos.x <= 1800.0f) {
			playerPos.x += 5.0f;
		}
		else if (playerPos.x >= 1800.0f) {
			isRun = false;
			isIdle = true;
			stageSelect++;
			level = 6;
		}

	}
	if (back == 1.0f) {

		if (playerPos.x >= 1160.0f) {
			playerPos.x -= 5.0f;
			playerPos.y -= 2.5f;
		}
		else if (playerPos.x >= 1070.0f) {
			playerPos.x -= 5.0f;
			playerPos.y += 2.0f;
		}
		else if (playerPos.x >= 1040.0f) {
			playerPos.x -= 2.0f;
			playerPos.y -= 5.0f;
		}
		else if (playerPos.x >= 960.0f) {
			playerPos.x -= 4.0f;
			playerPos.y += 2.0f;
		}
		else if (playerPos.x >= 850.0f) {
			playerPos.x -= 4.0f;
			playerPos.y -= 5.0f;
		}
		else if (playerPos.x >= 780.0f) {
			playerPos.x -= 5.0f;
		}
		else if (playerPos.x <= 780.0f) {
			isRun = false;
			isIdle = true;
			stageSelect--;
			level = 2;
		}
	}


}

void MainStageSelect::MoveForce()
{

	if (playerPos.x >= 1730.0f) {
		playerPos.x -= 5.0f;
	}
	else if (playerPos.x >= 1560.0f) {
		playerPos.x -= 2.0f;
		playerPos.y += 5.0f;
	}
	else if (playerPos.x >= 1500.0f) {
		playerPos.x -= 5.0f;
		playerPos.y -= 5.0f;
	}
	else if (playerPos.x >= 1430.0f) {
		playerPos.x -= 5.0f;
		playerPos.y += 5.0f;
	}
	else if (playerPos.x <= 1430.0f) {
		isRun = false;
		isIdle = true;
		stageSelect--;
		level = 3;
	}

}
