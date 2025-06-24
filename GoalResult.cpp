// ============================================
// メイン制御　ゴールリザルトマウントシーン [GoalResult.cpp]
// Author: 西窪 統吉桜	Date: 2023/08/23
// ============================================
#include "GoalResult.h"
#include "MainInGame.h"

#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "sound.h"
#include "XInput.h"
#include "sound.h"

#define MOVE_FRAME (50)

#define TEX_RAW (5)
#define TEX_COLUMN (5) 
#define DIGIT_DISTANCE (100.0f)
#define DIGIT_NUM (2)

void GoalResult::SetUp()
{
	CleanRequest();
	StopSoundAll();


	font = new FontData;
	font->fontSize = 94.0f;
	font->fontWeight = DWRITE_FONT_WEIGHT_BOLD;
	font->Color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);

	write = new DirectWrite(font);
	write->Init();


	// 初期化
	onGameData = OnGameData::GetInstance();

	frameCnt = 0;
	backBoardTex = LoadTexture((char*)"data/TEXTURE/GOAL/haikei_re.png");
	boardHeadTex = LoadTexture((char*)"data/TEXTURE/GOAL/haikei_maki_re.png");
	starCoinTex = LoadTexture((char*)"data/TEXTURE/11_Starcoin.png");
	stamp[0] = LoadTexture((char*)"data/TEXTURE/GOAL/zumi_sutanpu.png");
	stamp[1] = LoadTexture((char*)"data/TEXTURE/GOAL/mitassei.png");

	runkTex[0] = LoadTexture((char*)"data/TEXTURE/GOAL/5_Result_runk_ka.png");
	runkTex[1] = LoadTexture((char*)"data/TEXTURE/GOAL/5_Result_runk_yoi.png");
	runkTex[2] = LoadTexture((char*)"data/TEXTURE/GOAL/5_Result_runk_yuu.png");
	runkTex[3] = LoadTexture((char*)"data/TEXTURE/GOAL/5_Result_runk_syuu.png");

	clearStrTex = LoadTexture((char*)"data/TEXTURE/GOAL/IMG_0469.PNG");

	clearSe = LoadSound((char*)"data/SE/goal.wav");
	fireSe = LoadSound((char*)"data/SE/World/fire.wav");
	dodonSe = LoadSound((char*)"data/SE/RESULT/dodon.wav");
	scroolSe = LoadSound((char*)"data/SE/RESULT/makimono.wav");
	stampSe = LoadSound((char*)"data/SE/RESULT/pon.wav");

	BGM = LoadSound((char*)"data/BGM/gameclear.wav");
	SetVolume(BGM, 0.6f);

	isAnimEnd = false;
	moveEnd = false;
	printCoin = false;
	printStr = false;
	printScore = false;
	printEnemy = false;
	resultEnd = false;
	coinOverFirst = true;
	enemyOverFirst = true;
	cardFirst[0] = true;
	cardFirst[1] = true;
	cardFirst[2] = true;

	goalAnimStart = MainInGame::goal->GetTransform()->GetPos();
	goalAnimStart.x -= 85.0f;
	goalAnimStart.y = goalAnimStart.y - MainInGame::goal->GetTransform()->GetSize().y * 0.5f + MainInGame::player->GetTransform()->GetSize().y * 0.5f;

	headPos = { SCREEN_WIDTH * 0.5f, -300.0f };
	boardY = 0.0f;

	targetCoinNum = onGameData->GetTargetCoinNum();
	getCoinNum = onGameData->GetCoinNum();
	drawCoinNum = 0;

	targetEnemy = onGameData->GetTargetEnemyNum();
	hitEnemy = onGameData->GetHitEnemyCnt();
	drawEnemy = 0;

	getCardNum = onGameData->GetStarCoinNum();

	score = onGameData->GetScore();
	drawScore = 0;
	bonus = 0;
	moveCnt = 0;

	// スタンプの描画種類決定
	coinTarget = getCoinNum >= targetCoinNum ? 0: 1;
	enemyTarget = hitEnemy >= targetEnemy ? 0 : 1;

	cardAlpha[0] = 0.0f;
	cardAlpha[1] = 0.0f;
	cardAlpha[2] = 0.0f;
	stampAlpha[0] = 0.0f;
	stampAlpha[1] = 0.0f;
	runkAlpha = 0.0f;
	runkRot = 0.0f;
	runk = 0;

	PlaySound(clearSe, 0);
}

void GoalResult::CleanUp()
{
	write->Release();
	write = NULL;
	delete font;
}

void GoalResult::OnUpdate()
{
	if (!moveEnd)
	{
		MovePlayer();
		return;
	}

	++frameCnt;

	if (!isAnimEnd)
	{
		if (frameCnt >= 60 && frameCnt <= 120)
		{
			D3DXVECTOR2 playerPos = MainInGame::player->GetTransform()->GetPos();
			MainInGame::player->GetTransform()->SetPos(playerPos.x + 0.4f, playerPos.y);
		}

		if (frameCnt >= 160 && frameCnt <= 160)
		{
			MainInGame::goal->Light();
			PlaySound(fireSe, 0);
		}

		if (frameCnt >= 180)
		{
			isAnimEnd = true;
			frameCnt = 0;
		}
	}
	else if(!resultEnd)
	{
		if (frameCnt <= 60)
		{
			headPos.y += 5.5f;
			return;
		}
		if (frameCnt >= 60 && frameCnt <= 90)
		{
			if (frameCnt == 61)PlaySound(scroolSe, 0);
			boardY += 26.25f;
			return;
		}
		
		printStr = true;
		printScore = true;

		if (frameCnt >= 100 && frameCnt <= 130)
		{
			float add = (float)getCoinNum / 30.0f * (frameCnt - 100);
			drawCoinNum = add;
			printCoin = true;
			return;
		}
		drawCoinNum = getCoinNum;

		if (frameCnt >= 140 && frameCnt <= 170)
		{
			float add = (float)hitEnemy / 30.0f * (frameCnt - 140);
			drawEnemy = add;
			printEnemy = true;
			return;
		}
		drawEnemy = hitEnemy;
		
		if (frameCnt >= 200 && frameCnt <= 210)
		{
			if (coinTarget == 0 && coinOverFirst)
			{
				coinOverFirst = false;
				bonus += targetOverScore;
				PlaySound(stampSe, 0);
			}

			stampAlpha[0] += 1.0f / 10.0f;
			return;
		}


		if (frameCnt <= 220 && frameCnt >= 210)
		{
			if (enemyTarget == 0 && enemyOverFirst)
			{
				enemyOverFirst = false;
				bonus += targetOverScore;
				PlaySound(stampSe, 0);
			}

			stampAlpha[1] += 1.0f / 10.0f;
			return;
		}

		if (frameCnt <= 240 && frameCnt >= 230)
		{
			if (getCardNum < 1)return;
			if (cardFirst[0])
			{
				cardFirst[0] = false;
				bonus += cardGetScore;
				PlaySound(stampSe, 0);
			}
			cardAlpha[0] += 0.1f;
			return;
		}

		if (frameCnt <= 250 && frameCnt >= 240)
		{
			if (getCardNum < 2)return;
			if (cardFirst[1])
			{
				cardFirst[1] = false;
				bonus += cardGetScore;
				PlaySound(stampSe, 0);
			}

			cardAlpha[1] += 0.1f;
			return;
		}

		if (frameCnt == 251)
		{
			// ランクの設定
			int maxScore = OnGameData::GetInstance()->GetMaxScore();
			if (score + bonus >= maxScore * 0.75f)runk = 3;
			else if (score + bonus >= maxScore * 0.6f)runk = 2;
			else if (score + bonus >= maxScore * 0.4f)runk = 1;
			else runk = 0;
		}

		if (frameCnt <= 260 && frameCnt >= 250)
		{
			if (getCardNum < 3)return;
			if (cardFirst[2])
			{
				cardFirst[2] = false;
				bonus += cardGetScore;
				PlaySound(stampSe, 0);
			}

			cardAlpha[2] += 0.1f;
			return;
		}

		if (frameCnt <= 305 && frameCnt >= 290)
		{
			if (frameCnt == 302) PlaySound(dodonSe, 0);
			runkAlpha += 1.0f / 15.0f;
			runkRot -= 0.01f;
			return;
		}

		if (frameCnt >= 310)
		{
			resultEnd = true;
			PlaySound(BGM, -1);
		}
	}

	if (resultEnd)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			OnGameData::GetInstance()->SetBack(true);
			EndMountRequest();
			return;
		}
	}
}

void GoalResult::OnDraw()
{
	if (!isAnimEnd)return;
	DrawSpriteColor(boardHeadTex,
		headPos.x, headPos.y,
		1400.0f, 400.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f);

	DrawSpriteLeftTopColor(backBoardTex,
		headPos.x - 700.0f, headPos.y + 190.0f,
		1400.0f, boardY,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f);

	if (printStr)
	{
		DrawSprite(clearStrTex,
			headPos.x, headPos.y + 280.0f,
			700.0f, 200.0f,
			0.0f, 0.0f,
			1.0f, 1.0f);
	}

	if (printCoin)
	{
		write->DrawString(std::to_string(drawCoinNum) + "/" + std::to_string(targetCoinNum), D3DXVECTOR2(620.0f, 385.0f), D2D1_DRAW_TEXT_OPTIONS_NONE);
	}

	if (printEnemy)
	{
		write->DrawString(std::to_string(drawEnemy), D3DXVECTOR2(550.0f, 545.0f), D2D1_DRAW_TEXT_OPTIONS_NONE);
		write->DrawString(std::to_string(targetEnemy), D3DXVECTOR2(730.0f, 545.0f), D2D1_DRAW_TEXT_OPTIONS_NONE);
	}

	if (printScore)
	{
		drawScore = score + bonus;
		write->DrawString(std::to_string(drawScore), D3DXVECTOR2(470.0f, 820.0f), D2D1_DRAW_TEXT_OPTIONS_NONE);
	}

	DrawSpriteColor(stamp[coinTarget],
		865.0f, 455.0f,
		150.0f, 100.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, stampAlpha[0]);

	DrawSpriteColor(stamp[enemyTarget],
		865.0f, 600.0f,
		150.0f, 100.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, stampAlpha[1]);

	// 左お札
	DrawSpriteColor(starCoinTex,
		440.0f, 725.0f,
		200.0f, 150.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, cardAlpha[0]);

	// 中央お札
	DrawSpriteColor(starCoinTex,
		605.0f, 725.0f,
		200.0f, 150.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, cardAlpha[1]);

	// 右お札
	DrawSpriteColor(starCoinTex,
		760.0f, 725.0f,
		200.0f, 150.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, cardAlpha[2]);

	// ランク
	DrawSpriteColorRotate(runkTex[runk],
		1300.0f, 700.0f,
		600.0f, 500.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, runkAlpha, runkRot);

	if (resultEnd)
	{
		write->DrawString("Bでセレクトへ", D3DXVECTOR2(headPos.x - 350.0f, 950.0f), D2D1_DRAW_TEXT_OPTIONS_NONE);
	}
}

void GoalResult::MovePlayer()
{
	D3DXVECTOR2 playerPos = MainInGame::player->GetTransform()->GetPos();
	bool moveX, moveY;
	moveX = false;
	moveY = false;

	D3DXVECTOR2 move = playerPos - goalAnimStart;
	++moveCnt;

	if (moveCnt > 120)
	{	// スタック対策で強制的に移動を終わらせる
		MainInGame::player->GetTransform()->SetPos(goalAnimStart.x, playerPos.y);
		moveEnd = true;
		MainInGame::player->Goal();
		return;
	}


	// x軸の移動
	if (move.x > 7.0f)
	{
		MainInGame::player->GetTransform()->SetPos(playerPos.x - 2.0f, playerPos.y);
	}
	else if (move.x < -7.0f)
	{
		MainInGame::player->GetTransform()->SetPos(playerPos.x + 2.0f, playerPos.y);
	}
	else
	{
		moveX = true;
	}

	playerPos = MainInGame::player->GetTransform()->GetPos();

	// y軸の移動
	if (move.y > 4.0f)
	{
		MainInGame::player->GetTransform()->SetPos(playerPos.x, playerPos.y - 3.0f);
	}
	else if (move.y < -4.0f)
	{
		MainInGame::player->GetTransform()->SetPos(playerPos.x, playerPos.y + 3.0f);
	}
	else
	{
		moveY = true;
	}


	if (moveX && moveY)
	{	// 移動が完全に終了したとき
		moveEnd = true;
		MainInGame::player->Goal();
	}
}
