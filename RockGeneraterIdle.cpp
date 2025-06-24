#include "RockGeneraterIdle.h"
#include "RockGenerater.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "OnGameData.h"
#include "ColiderPool.h"
#include "Player.h"
#include "MainInGame.h"
#include "BlockMap.h"

void RockGeneraterIdle::SetUp()
{
	CleanRequest();

	blockTex[0] = LoadTexture((char*)"data/TEXTURE/RockHole.png");
	blockTex[1] = LoadTexture((char*)"data/TEXTURE/RockHole2.png");

	dirtTex[0] = LoadTexture((char*)"data/TEXTURE/soil_2.png");
	dirtTex[1] = LoadTexture((char*)"data/TEXTURE/soil_2_R.png");

	dir = obj->IsLeftDir() ? -1.0f : 1.0f;

	drawMode = 0;

	generateFirstTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

void RockGeneraterIdle::CleanUp()
{

}

void RockGeneraterIdle::OnUpdate()
{
	if (fabsf(MainInGame::player->GetTransform()->GetPos().x - obj->GetTransform()->GetPos().x) > 3000) return;

	if (generateMode == 0)
	{
		if (rock == nullptr) Generate();
	}
	if (generateMode == 1)
	{
		generateCount++;
		if (generateCount > GENERATE_INTERVAL_COUNT) MultiGenerate();
	}
	if (generateMode == 2)
	{
		std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
		std::chrono::milliseconds nowTime = std::chrono::duration_cast<std::chrono::milliseconds>(p.time_since_epoch());
		if (nowTime.count() - generateFirstTime.count() > GENERATE_INTERVAL_TIME) MultiGenerate();
	}
}

void RockGeneraterIdle::OnDraw()
{
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;

	// カメラの移動
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	Camera* camera = Camera::GetInstance();
	D3DXVECTOR2 drawPos = D3DXVECTOR2(pos.x - camera->GetOriginPos().x, pos.y - camera->GetOriginPos().y);

	// 壁の追加
	D3DXVECTOR2 blockPos = { drawPos.x, drawPos.y - size.y * 0.25f};

	DrawSprite(dirtTex[drawMode],
		blockPos.x, blockPos.y,
		CHIP_X_SIZE, CHIP_Y_SIZE,
		0.0f, 0.0f,
		1.0f, 1.0f);

	DrawSprite(dirtTex[drawMode],
		blockPos.x, blockPos.y + CHIP_Y_SIZE,
		CHIP_X_SIZE, CHIP_Y_SIZE,
		0.0f, 0.0f,
		1.0f, 1.0f);

	DrawSprite(blockTex[drawMode],
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		dir * -1.0f, 1.0f
	);
}

void RockGeneraterIdle::Generate()
{
	Transform2D* transform = obj->GetTransform();
	Player* player = MainInGame::player;
	D3DXVECTOR2 playerPos = player->GetTransform()->GetPos();
	D3DXVECTOR2 playerSize = player->GetTransform()->GetSize();

	float height = transform->GetPos().y;

	// プレイヤーの位置の確認
	if ( fabsf(playerPos.x - transform->GetPos().x) <= 1300.0f )
	{
		D3DXVECTOR2 rockPos = D3DXVECTOR2(transform->GetPos().x + (75.0f * dir), height);
		float rockDir = -1.0f * (OnGameData::GetInstance()->HasSwap() ? 1.0f : 1.0f);

		// 弾の生成
		Rock* generate = new Rock(&rock, rockPos, rockSize, rockVel * dir);
		rock = generate;
		rock->Start();

		MainInGame::objectPool.Add(rock);
	}
}

void RockGeneraterIdle::MultiGenerate()
{
	Transform2D* transform = obj->GetTransform();
	Player* player = MainInGame::player;
	D3DXVECTOR2 playerPos = player->GetTransform()->GetPos();
	D3DXVECTOR2 playerSize = player->GetTransform()->GetSize();

	float height = transform->GetPos().y;

	// プレイヤーの位置の確認
	if (fabsf(playerPos.x - transform->GetPos().x) <= 1100.0f)
	{
		D3DXVECTOR2 rockPos = D3DXVECTOR2(transform->GetPos().x + (75.0f * dir), height);
		float rockDir = -1.0f * (OnGameData::GetInstance()->HasSwap() ? 1.0f : 1.0f);

		// 弾の生成
		Rock* generate = new Rock(&rock, rockPos, rockSize, rockVel * dir);
		rock = generate;
		rock->Start();

		MainInGame::objectPool.Add(rock);

		std::chrono::system_clock::time_point p = std::chrono::system_clock::now();

		generateFirstTime = std::chrono::duration_cast<std::chrono::milliseconds>(p.time_since_epoch());
		generateCount = 0;
	}
}

