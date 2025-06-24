// ============================================
// メイン制御　タイトル [Main.Title.cpp]
// Author: 西窪 統吉桜	Date: 2023/07/07
// ============================================
#include "MainTitle.h"
#include "MainInGame.h"
#include "Fade.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "OnGameData.h"
#include "Xinput.h"
#include "sound.h"
#include "DirectWrite.h"
#include "Easing.h"
#include "sound.h"

FontData* titleData;
DirectWrite* titleWrite;

ObjectPool MainTitle::objectPool;

static int titleBGM;

void MainTitle::SetUp()
{
	CleanRequest();
	StopSoundAll();

	titleData = new FontData();
	titleData->Color = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
	titleData->fontSize = 60;
	titleData->fontWeight = DWRITE_FONT_WEIGHT_NORMAL;

	titleWrite = new DirectWrite(titleData);
	titleWrite->Init();
	frontMountainTex[0] = LoadTexture((char*)"data/TEXTURE/mountain1.PNG");
	frontMountainTex[1] = LoadTexture((char*)"data/TEXTURE/mountain1_w.PNG"); BG[2] = LoadTexture((char*)"data/TEXTURE/taitorukari.png");
	backMountainTex[0] = LoadTexture((char*)"data/TEXTURE/mountain2.PNG"); buttonFrame = LoadTexture((char*)"data/TEXTURE/buttonFrame.png");
	backMountainTex[1] = LoadTexture((char*)"data/TEXTURE/mountain2_w.PNG"); selectTex = LoadTexture((char*)"data/TEXTURE/Choose.png");
	BG[0] = LoadTexture((char*)"data/TEXTURE/yama.png");
	BG[1] = LoadTexture((char*)"data/TEXTURE/yama_hanten2.png");
	BG[2] = LoadTexture((char*)"data/TEXTURE/taitorukari.png");

	strTex = LoadTexture((char*)"data/TEXTURE/kaisi.png");
	buttonFrame = LoadTexture((char*)"data/TEXTURE/buttonFrame.png");
	selectTex = LoadTexture((char*)"data/TEXTURE/Choose.png");
	finishTex = LoadTexture((char*)"data/TEXTURE/finish.png");
	BbuttonTex = LoadTexture((char*)"data/TEXTURE/Button_B.png");

	nextSe = LoadSound((char*)"data/SE/titlenext.wav");
	SetVolume(nextSe, 0.45f);

	titleBGM = LoadSound((char*)"data/BGM/title.wav");
	SetVolume(titleBGM, 0.45f);

	Camera::GetInstance()->InitCamera();

	OnGameData::GetInstance()->InitData();
	OnGameData::GetInstance()->LoadGameTexture();

	mapInstance = BlockMap::GetInstance();
	mapInstance->InitMap(0);
	mapInstance->StartMap();
	objectPool.Start();

	player = MainInGame::player;
	player->Start();

	frameCnt = 0;
	logoPos = {SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5f - 100.0f};
	logoSizeEase = 0.0f;
	logoRad = 0.0f;
	logoSizeTheta = 3.1415f * 0.5f;

	// インプット切り替え
	NotControlKey(DIK_W);
	NotControlKey(DIK_A);
	NotControlKey(DIK_S);
	NotControlKey(DIK_D);
	NotControlKey(DIK_DOWN);
	NotControlKey(DIK_SPACE);
	NotControlKey(DIK_UP);
	NotControlKey(DIK_LSHIFT);

	NotControlPad(BUTTON_LEFT);
	NotControlPad(BUTTON_RIGHT);
	NotControlPad(BUTTON_B);
	NotControlPad(BUTTON_X);
	NotControlPad(BUTTON_Y);
	NotControlPad(BUTTON_R);

	isSelect = false;
	selectNum = 0;

	PlaySound(titleBGM, -1);
}

void MainTitle::CleanUp()
{
	delete titleData;

	titleWrite->Release();
	titleWrite = NULL;

	mapInstance->UnInitMap();
}

void MainTitle::OnUpdate()
{
	// アニメーションをする
	if (OnGameData::GetInstance()->IsSwapAnim())
	{
		Camera::GetInstance()->SwapAnimationUpdate();
		return;
	}

	++frameCnt;

	//シーン遷移ボタン
	if ( frameCnt > 120 && !isSelect && (IsButtonTriggered(0, BUTTON_A)||GetKeyboardTrigger(DIK_RETURN) ))
	{
		ControlKey(DIK_W);
		ControlKey(DIK_S);
		isSelect = true;
		PlaySound(nextSe, 0);

		OnGameData::GetInstance()->SetSwapAnim(true);
		return;
	}

	if (isSelect && (IsButtonTriggered(0, BUTTON_DOWN) || GetKeyboardTrigger(DIK_S)))
	{
		++selectNum;
		if (selectNum > 1)selectNum = 1;
	}
	else if (isSelect && (IsButtonTriggered(0, BUTTON_UP) || GetKeyboardTrigger(DIK_W)))
	{
		--selectNum;
		if (selectNum < 0)selectNum = 0;
	}

	if (isSelect && (IsButtonTriggered(0, BUTTON_A) || GetKeyboardTrigger(DIK_RETURN)) && selectNum == 0)
	{
		AllControlPad();
		ChangeRequest("Select");
	}

	if (isSelect && (IsButtonTriggered(0, BUTTON_A) || GetKeyboardTrigger(DIK_RETURN)) && selectNum == 1)
	{
		OnGameData::GetInstance()->SetEnd(true);
	}

	if (frameCnt <= 90)
	{
		if (frameCnt <= 30)logoRad += 6.283f / 30.0f;
		logoSizeEase = easeOutBack((float)frameCnt / 90);
	}

	if (frameCnt >= 91 && frameCnt <= 100)
	{
		logoPos.y -= 22;
	}

	if (frameCnt >= 120)
	{
		logoSizeTheta += 0.02f;
		logoSizeEase = 1.0f - cosf(logoSizeTheta) * 0.1f;

		if (logoSizeTheta >= 3.1415f * 2.0f)logoSizeTheta = 0.0f;

		PlayerMove(frameCnt - 120);
	}
	objectPool.OnUpdate();
	mapInstance->UpdateMap();
	player->Update();

	// TODO: ちゃんとしたカメラの固定を作る
	Camera::GetInstance()->SetPos(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
}

void MainTitle::OnDraw()
{
	drawMode = data->HasSwap() ? 1 : 0;
	DrawSpriteLeftTop(BG[drawMode], 0.0f, 0.0f, 1980.0f, 1080.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	DrawSpriteLeftTop(backMountainTex[drawMode], 0.0f, 0.0f, 1980.0f, 1080.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	DrawSpriteLeftTop(frontMountainTex[drawMode], 0.0f, 0.0f, 1980.0f, 1080.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	//DrawSpriteLeftTopColor(BG[1], 0.0f, 0.0f, 1980.0f, 1080.0f, 0.0f, 0.0f, 1.0f, 1.0f,0.0f,0.0f,0.0f,0.5);
	drawMode = data->HasSwap() ? -1 : 1;
	if (OnGameData::GetInstance()->IsObjEnable())
	{
		objectPool.OnDraw();
		mapInstance->DrawMap(0);
		player->Draw();

		D3DXVECTOR2 size = logoSize * logoSizeEase;
		DrawSpriteColorRotate(BG[2],
			logoPos.x, logoPos.y,
			size.x, size.y,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f, logoRad);
	}

	if (OnGameData::GetInstance()->IsSwapAnim() && OnGameData::GetInstance()->IsSwapAnimStart())
	{
		Camera::GetInstance()->SwapAnimationDraw();
	}

	if (!OnGameData::GetInstance()->IsSwapAnim())
	{
		if (!isSelect)
		{
			DrawSprite(strTex,
				SCREEN_WIDTH * 0.5f, 900.0f,
				600.0f, 200.0f,
				0.0f, 0.0f,
				1.0f, 1.0f);
			DrawSprite(BbuttonTex,
				530.0f, 900.0f,
				100.0f, 100.0f,
				0.0f, 0.0f,
				1.0f, 1.0f);
		}
		else
		{
			//選択範囲
			/*
			DrawSpriteColor(buttonFrame,
				buttonStart.x, buttonStart.y + buttonDistance.y * selectNum,
				buttonSize.x + 35.0f, buttonSize.y + 30.0f,
				0.0f, 0.0f,
				1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f);
				*/
			float zoom = 1.0f;

			if (selectNum == 0)zoom = 1.2f;
			// 選択ボタン
			/*
			DrawSprite(buttonFrame,
				buttonStart.x, buttonStart.y + buttonDistance.y * 0,
				buttonSize.x + 20.0f, buttonSize.y + 20.0f,
				0.0f, 0.0f,
				1.0f, 1.0f);
				*/
			DrawSprite(selectTex,
				buttonStart.x, buttonStart.y + buttonDistance.y * 0,
				buttonSize.x*0.9f* zoom, buttonSize.y*1.3f* zoom,
				0.0f, 0.0f,
				1.0f* drawMode, 1.0f);
			zoom = 1.0f;
			if (selectNum == 1)zoom = 1.2f;
			//終了
			/*
			DrawSprite(buttonFrame,
				buttonStart.x, buttonStart.y + buttonDistance.y * 1,
				buttonSize.x + 20.0f, buttonSize.y + 20.0f,
				0.0f, 0.0f,
				1.0f, 1.0f);
				*/
			DrawSprite(finishTex,
				buttonStart.x, buttonStart.y + buttonDistance.y * 1,
				buttonSize.x*0.7f* zoom, buttonSize.y*0.8f* zoom,
				0.0f, 0.0f,
				1.0f* drawMode, 1.0f);

			//Aボタン
			DrawSprite(BbuttonTex,
				buttonStart.x - buttonDistance.x * drawMode, buttonStart.y + buttonDistance.y*selectNum,
				buttonSize.y * 0.9f, buttonSize.y * 0.9f,
				0.0f, 0.0f,
				1.0f * drawMode, 1.0f);
		}
	}
}

void MainTitle::PlayerMove(int frameCnt)
{
	int cnt = frameCnt % 270;
	int move = 0;

	if (OnGameData::GetInstance()->HasSwap())
	{
		move = DIK_A;
	}
	else
	{
		move = DIK_D;
	}

	if (cnt <= 100)
	{
		if (cnt >= 35 && cnt <= 50)VirtualInputPress(DIK_SPACE);

		VirtualInputPress(DIK_LSHIFT);
		VirtualInputPress(move);
		return;
	}

	if (cnt <= 165 && cnt >= 120)
	{
		if(cnt % 15 != 0)VirtualInputPress(DIK_SPACE);
		return;
	}

	if (cnt <= 230 && cnt >= 170)
	{
		if(cnt >= 180)VirtualInputPress(DIK_SPACE);
		VirtualInputPress(DIK_LSHIFT);
		VirtualInputPress(move);
		return;
	}

	if (cnt == 231)
	{
		player->GetTransform()->SetPos(0.0f, 800.0f);
		return;
	}

	if (cnt <= 255 && cnt >= 232)
	{
		VirtualInputPress(DIK_LSHIFT);
		VirtualInputPress(move);
		return;
	}

	if (cnt == 256)
	{
		player->GetTransform()->SetPos(240.0f, 800.0f);
	}

}
