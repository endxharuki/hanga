// ============================================
// ���C������@�Q�[���V�[�� [MainInGame.cpp]
// Author: ���E ���g��	Date: 2023/07/07
// ============================================
#include "MainInGame.h"
#include "OnGameData.h"
#include "PlayerBlockGenerator.h"
#include "input.h"
#include "Fade.h"
#include "UI.h"
#include "sound.h"
#include "sprite.h"
#include "texture.h"
#include "BlockMap.h"
ObjectPool MainInGame::objectPool = ObjectPool();
ObjectPool MainInGame::objectPool2 = ObjectPool();

Player* MainInGame::player = new Player();

static int gameBGM;

static int BG[2];

void MainInGame::SetUp()
{
	CleanRequest();

	// �Q�[���V�[�����L�f�[�^����������
	OnGameData::GetInstance()->InitData();
	OnGameData::GetInstance()->LoadGameTexture();

	BG[0] = LoadTexture((char*)"data/TEXTURE/White.png");
	BG[1] = LoadTexture((char*)"data/TEXTURE/White.png");

	// �}�b�v�Ǎ�����
	map->InitMap(OnGameData::GetInstance()->GetStageNum());
	map->StartMap();

	// �e�I�u�W�F�N�g�̏���������
	objectPool.Start();
	objectPool2.Start();

	prevHasSwap = OnGameData::GetInstance()->HasSwap();

	OnGameData::GetInstance()->SetLayerAlpha(1.0f);

	//	�v���C���[������
	player->Start();
	OnGameData::GetInstance()->AddPlayerLife();
	OnGameData::GetInstance()->AddPlayerLife();
	OnGameData::GetInstance()->AddPlayerLife();
	OnGameData::GetInstance()->GetStarCoinNum();

	UI::GetInstance()->Start();
}

void MainInGame::CleanUp()
{
	map->UnInitMap();
	objectPool.UnInit();
	objectPool2.UnInit();
}

void MainInGame::OnUpdate()
{
	if (OnGameData::GetInstance()->GetIsClear())
	{	// �N���A��Ԃֈڍs
		ChangeRequest("Clear");
	}

	if (OnGameData::GetInstance()->GetIsGameOver())
	{
		ChangeRequest("GameOver");
	}

	if (prevHasSwap != OnGameData::GetInstance()->HasSwap())
	{
		drawMode = (drawMode + 1) % 2;
		prevHasSwap = !prevHasSwap;
	}

	map->UpdateMap();

	objectPool.OnUpdate();
	if (OnGameData::GetInstance()->GetCurrentLayer() >= 1) objectPool2.OnUpdate();

	player->Update();

	UI::GetInstance()->Update();
}

void MainInGame::OnDraw()
{
	// �w�i�`��
	DrawSpriteLeftTop(BG[drawMode],
		0.0f, 0.0f,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f,
		1.0f, 1.0f
	);

	map->DrawMap();
	if (OnGameData::GetInstance()->GetCurrentLayer() >= 1) map->DrawMap2();

	objectPool.OnDraw();
	if (OnGameData::GetInstance()->GetCurrentLayer() >= 1) objectPool2.OnDraw();

	player->Draw();

	map->DrawLayer2();

	UI::GetInstance()->Draw();
}