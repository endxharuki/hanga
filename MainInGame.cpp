// ============================================
// メイン制御　ゲームシーン [MainInGame.cpp]
// Author: 西窪 統吉桜	Date: 2023/07/07
// ============================================
#include "MainInGame.h"
#include "input.h"
#include "Fade.h"
#include "UI.h"
#include "sound.h"
#include "sprite.h"
#include "texture.h"
#include "BlockMap.h"
#include "Camera.h"
#include "sound.h"
#include "XInput.h"

#include <chrono>
// 処理速度計測用
static std::chrono::high_resolution_clock::time_point start;
static std::chrono::high_resolution_clock::time_point end;

// font
static FontData* debugData;
static DirectWrite* write;

ObjectPool MainInGame::objectPool = ObjectPool();
Player* MainInGame::player;
Goal* MainInGame::goal;
CheckPoint* MainInGame::checkPoint;

EffectManager MainInGame::effectManager;

static int gameBGM;

static int BG[2];

void MainInGame::SetUp()
{
	CleanRequest();
	StopSoundAll();

	AllControlKey();
	AllControlPad();

	Camera::GetInstance()->InitCamera();
	Camera::GetInstance()->SetLeftLimit(0);
	Camera::GetInstance()->SetPos(D3DXVECTOR2(0.0f, 0.0f));

	// ゲームシーン共有データ初期化処理
	data->InitData();
	data->LoadGameTexture();
	data->SetCtrlUser(true);

	BG[0] = LoadTexture((char*)"data/TEXTURE/yama.png");
	BG[1] = LoadTexture((char*)"data/TEXTURE/yama_hanten2.png");

	frontMountainTex[0] = LoadTexture((char*)"data/TEXTURE/mountain1.PNG");
	frontMountainTex[1] = LoadTexture((char*)"data/TEXTURE/mountain1_w.PNG");
	backMountainTex[0] = LoadTexture((char*)"data/TEXTURE/mountain2.PNG");
	backMountainTex[1] = LoadTexture((char*)"data/TEXTURE/mountain2_w.PNG");

	frontCloudTex[0] = LoadTexture((char*)"data/TEXTURE/cloud1_re.png");
	frontCloudTex[1] = LoadTexture((char*)"data/TEXTURE/white.png");

	skyTex[0] = LoadTexture((char*)"data/TEXTURE/sky.PNG");
	skyTex[1] = LoadTexture((char*)"data/TEXTURE/sky_White.PNG");
	
	BGM = LoadSound((char*)"data/BGM/world.wav");
	SetVolume(BGM, 0.55f);

	// マップ読込処理
	map->InitMap(data->GetStageNum());
	map->StartMap();
	effectManager.Clear();

	// 各オブジェクトの初期化処理
	objectPool.Start();

	prevHasSwap = data->HasSwap();

	data->SetLayerAlpha(1.0f);

	//	プレイヤー初期化
	player->Start();
	playerTransform = player->GetTransform();

	if (data->IsCheckPoint() && checkPoint)
	{
		playerTransform->SetPos(checkPoint->GetTransform()->GetPos());
	}

	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();
	frontCloudUV = { cameraPos.x / 7000.0f + addCloudU, 1.0f };
	backMountainUV = { cameraPos.x / 6000.0f, 0.0f };
	frontMountainUV = { cameraPos.x / 2500.0f, 0.0f };

	UI::GetInstance()->Start();

	// フォント設定
	debugData = new FontData();
	debugData->fontSize = 30;
	debugData->fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
	debugData->Color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);

	write = new DirectWrite(debugData);
	write->Init();

	addCloudU = 0.0f;
	isFirstClear = false;
	isFirstGameOver = false;

	PlaySound(BGM, -1);
}

void MainInGame::CleanUp()
{
	map->UnInitMap();
	objectPool.UnInit();
	effectManager.Clear();

	write->Release();
	write = NULL;
	delete debugData;

	delete player;

	StopSound(BGM);
}

void MainInGame::OnUpdate()
{
	start = std::chrono::high_resolution_clock::now();

	// アニメーションをする
	if (data->IsSwapAnim())
	{
		Camera::GetInstance()->SwapAnimationUpdate();
		return;
	}

	if (data->IsRetry())
	{
		ChangeRequest("Game");
		return;
	}
	if (data->IsBack())
	{
		ChangeRequest("Select");
		data->SetIsCheckPoint(false);
		return;
	}

	if (data->IsBackTitle())
	{
		ChangeRequest("Title");
		return;
	}

	if (data->GetIsClear() && !isFirstClear)
	{
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
		if (data->HasSwap()) {
			// 版画状態なら一度版画状態を抜ける
			data->SetSwapAnim(true);
			return;
		}

		ConcurrentMountRequest("GoalResultMount");
		isFirstClear = true;
	}

	if (data->GetIsGameOver() && !isFirstGameOver)
	{
		MountRequest("GameOverMount");
		isFirstGameOver = true;
		return;
	}

	if (data->IsSwapAnim())return;

	map->UpdateMap();

	objectPool.OnUpdate();

	player->Update();

	effectManager.OnUpdate();

	// テクスチャ計算
	addCloudU += 0.0005f;

	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();
	frontCloudUV = { cameraPos.x / 7000.0f + addCloudU, 1.0f };
	backMountainUV = { cameraPos.x / 5000.0f, 0.0f };
	frontMountainUV = { cameraPos.x / 2500.0f, 0.0f };

	UI::GetInstance()->Update();
	end = std::chrono::high_resolution_clock::now();
}

void MainInGame::OnDraw()
{
	drawMode = data->HasSwap() ? 1 : 0;

	// 背景描画
	DrawSpriteLeftTop(skyTex[drawMode],
		0.0f, 0.0f,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f,
		1.0f, 1.0f);

	DrawSpriteLeftTop(frontCloudTex[drawMode],
		0.0f, 0.0f,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		frontCloudUV.x, frontCloudUV.y,
		1.0f, 1.0f);

	DrawSpriteLeftTop(backMountainTex[drawMode],
		0.0f, 0.0f,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		backMountainUV.x, backMountainUV.y,
		1.0f, 1.0f);

	DrawSpriteLeftTop(frontMountainTex[drawMode],
		0.0f, 0.0f,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		frontMountainUV.x, frontMountainUV.y,
		1.0f, 1.0f);

	// オブジェクト描画
	if (data->IsObjEnable())
	{
		map->DrawMap(0);

		effectManager.OnDraw();

		objectPool.OnDraw();

		player->Draw();

		map->DrawMap(1);
		map->DrawMap(2);
	}

	if (data->IsSwapAnim() && data->IsSwapAnimStart())
	{
		Camera::GetInstance()->SwapAnimationDraw();
	}

	if (!data->IsSwapAnim())
	{	// 版画切り替えアニメーション時に描画しない
		UI::GetInstance()->Draw();
	}
}