#include "BreakBlockIdle.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "OnGameData.h"
#include "ColiderPool.h"
#include "Break.h"
#include "EffectManager.h"

void BreakBlockIdle::SetUp()
{
	CleanRequest();

	blockTex[0] = LoadTexture((char*)"data/TEXTURE/broken_wall.png");
	blockTex[1] = LoadTexture((char*)"data/TEXTURE/broken_wall_R.png");

	breakTex[0] = LoadTexture((char*)"data/TEXTURE/broken_wall_SPR.png");
	breakTex[1] = LoadTexture((char*)"data/TEXTURE/broken_wall_R_SPR.png");

	EffectObj effect;
	effect.tex[0] = breakTex[0];
	effect.tex[1] = breakTex[1];
	effect.size = obj->GetTransform()->GetSize();
	effect.speed = 10;
	effect.max_x = 4;
	effect.max_y = 1;
	effect.allFrame = 4;

	obj->GetComponent<Break>()->SetEffect(effect);

	drawMode = 0;
}

void BreakBlockIdle::CleanUp()
{
	
}

void BreakBlockIdle::OnUpdate()
{
	ColidPlayer();
	ColidObject();
}

void BreakBlockIdle::OnDraw()
{
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;

	// カメラの移動
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	Camera* camera = Camera::GetInstance();
	D3DXVECTOR2 drawPos = D3DXVECTOR2(pos.x - camera->GetOriginPos().x, pos.y - camera->GetOriginPos().y);

	DrawSprite( blockTex[drawMode],
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		size.x / CHIP_X_SIZE, size.y / CHIP_Y_SIZE
	);
}

void BreakBlockIdle::ColidPlayer()
{
	Player* player = MainInGame::player;
	Colider2D colid = obj->GetComponent<ColiderPool>()->GetColider()[0];
	Colider2D playerColid = player->GetComponent<ColiderPool>()->GetColider()[0];

	D3DXVECTOR2 pos = colid.GetPos();
	D3DXVECTOR2 size = colid.GetSize();

	D3DXVECTOR2 playerPos = playerColid.GetPos();
	D3DXVECTOR2 playerSize = playerColid.GetSize();

	ColidDir dir = colid.IsDirColid(playerColid);

	if (dir == DOWN)
	{	// ブロックの上に衝突
		player->GetTransform()->SetPos(playerPos.x, pos.y + size.y * 0.5f + playerSize.y * 0.5f);
	}
	else if (dir == UP)
	{	// ブロックの下に衝突
		player->SetGround();
		player->GetTransform()->SetVel(player->GetTransform()->GetVel().x, 0.0f);
		player->GetTransform()->SetPos(playerPos.x, pos.y - size.y * 0.5f - playerSize.y * 0.5f);

	}
	else if (dir == RIGHT)
	{	// ブロックの左に衝突
		player->GetTransform()->SetPos(pos.x + size.x * 0.5f + playerSize.x * 0.5f, playerPos.y);
	}
	else if (dir == LEFT)
	{	// ブロックの右に衝突
		player->GetTransform()->SetPos(pos.x - size.x * 0.5f - playerSize.x * 0.5f, playerPos.y);
	}
}


void BreakBlockIdle::ColidObject()
{

	// オブジェクトとの衝突判定
	std::list<GameObject*>* pool = MainInGame::objectPool.GetPool();

	//オブジェクト
	for (GameObject* target : *pool)
	{
		if (target == nullptr) continue;
		if (obj == target) continue;


		ColiderPool* targetColidPool = target->GetComponent<ColiderPool>();
		if (targetColidPool == nullptr) continue;
		if (targetColidPool->GetColider()[0].GetTag() == "Rock")continue;


		Colider2D colid = obj->GetComponent<ColiderPool>()->GetColider()[0];
		Colider2D targetColid = target->GetComponent<ColiderPool>()->GetColider()[0];
		D3DXVECTOR2 pos = colid.GetPos();
		D3DXVECTOR2 size = colid.GetSize();
		D3DXVECTOR2 targetPos = targetColid.GetPos();
		D3DXVECTOR2 targetSize = targetColid.GetSize();
		D3DXVECTOR2 currentPos = obj->GetTransform()->GetPos();

		ColidDir dir = colid.IsDirColid(targetColid);
		if (dir == UP)
		{
			target->GetTransform()->SetPos(targetPos.x, pos.y - size.y * 0.5f - targetSize.y * 0.5f);
		}
		if (dir == DOWN)
		{
			target->GetTransform()->SetPos(targetPos.x, pos.y + size.y * 0.5f + targetSize.y * 0.5f);
		}
		if (dir == LEFT)
		{
			target->GetTransform()->SetPos(pos.x - size.x * 0.5f - targetSize.x * 0.5f, targetPos.y);
		}
		if (dir == RIGHT)
		{
			target->GetTransform()->SetPos(pos.x + size.x * 0.5f + targetSize.x * 0.5f, targetPos.y);
		}

	}

}