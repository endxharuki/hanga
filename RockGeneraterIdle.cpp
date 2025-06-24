#include "RockGeneraterIdle.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "OnGameData.h"
#include "ColiderPool.h"
#include "Player.h"
#include "MainInGame.h"

void RockGeneraterIdle::SetUp()
{
	CleanRequest();

	blockTex[0] = LoadTexture((char*)"data/TEXTURE/RockHole.png");
	blockTex[1] = LoadTexture((char*)"data/TEXTURE/RockHole2.png");

}

void RockGeneraterIdle::CleanUp()
{

}

void RockGeneraterIdle::OnUpdate()
{
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;

	if (rock == nullptr) Generate();
}

void RockGeneraterIdle::OnDraw()
{
	// カメラの移動
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	Camera* camera = Camera::GetInstance();
	D3DXVECTOR2 drawPos = D3DXVECTOR2(pos.x - camera->GetOriginPos().x, pos.y - camera->GetOriginPos().y);

	DrawSprite(blockTex[drawMode],
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		1.0f, 1.0f
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
	if ( fabsf(playerPos.x - transform->GetPos().x) <= 1100.0f )
	{
		D3DXVECTOR2 rockPos = D3DXVECTOR2(transform->GetPos().x + (75.0f * -1.0f), height);
		float rockDir = -1.0f * (OnGameData::GetInstance()->HasSwap() ? 1.0f : 1.0f);

		// 弾の生成
		Rock* generate = new Rock(&rock, rockPos, rockSize, rockVel * -1.0f);
		rock = generate;
		rock->Start();

		MainInGame::objectPool.Add(rock);
	}
}

