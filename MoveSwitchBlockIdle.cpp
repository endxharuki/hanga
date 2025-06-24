#include "MoveSwitchBlockIdle.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "OnGameData.h"
#include "ColiderPool.h"
#include "MainInGame.h"
#include "Damaged.h"
#include "Switch.h"

void MoveSwitchBlockIdle::SetUp()
{
	CleanRequest();

	onTex[0] = LoadTexture((char*)"data/TEXTURE/25_block_red.png");
	onTex[1] = LoadTexture((char*)"data/TEXTURE/25_block_h.png");
	offTex[0] = LoadTexture((char*)"data/TEXTURE/25_block_blue.png");
	offTex[1] = LoadTexture((char*)"data/TEXTURE/25_block_h.png");

	pool = obj->GetComponent<ColiderPool>();

	drawMode = 0;

	firstPos = obj->GetTransform()->GetPos();
}

void MoveSwitchBlockIdle::CleanUp()
{

}

void MoveSwitchBlockIdle::OnUpdate()
{
	moveCount++;
	
	Move();
	ChangeVec();
	

	ColidPlayer();
	ColidObject();
}

void MoveSwitchBlockIdle::OnDraw()
{
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;

	// カメラの移動
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	Camera* camera = Camera::GetInstance();
	D3DXVECTOR2 drawPos = D3DXVECTOR2(pos.x - camera->GetOriginPos().x, pos.y - camera->GetOriginPos().y);

	if (GetCommonSwitch())
	{
		DrawSprite(onTex[drawMode],
			drawPos.x, drawPos.y,
			size.x, size.y,
			0.0f, 0.0f,
			size.x / CHIP_X_SIZE, size.y / CHIP_Y_SIZE
		);
	}
	else
	{
		DrawSprite(offTex[drawMode],
			drawPos.x, drawPos.y,
			size.x, size.y,
			0.0f, 0.0f,
			size.x / CHIP_X_SIZE, size.y / CHIP_Y_SIZE
		);
	}
	
	
	
}

void MoveSwitchBlockIdle::ColidPlayer()
{
	Player* player = MainInGame::player;

	Colider2D moveColid = obj->GetComponent<ColiderPool>()->GetColider()[0];
	Colider2D playerColid = player->GetComponent<ColiderPool>()->GetColider()[0];
	D3DXVECTOR2 pos = moveColid.GetPos();
	D3DXVECTOR2 size = moveColid.GetSize();
	D3DXVECTOR2 playerPos = playerColid.GetPos();
	D3DXVECTOR2 playerSize = playerColid.GetSize();
	ColidDir dir = moveColid.IsDirColid(playerColid);
	if (dir == UP)
	{
		player->GetTransform()->SetPos(playerPos.x, pos.y - size.y*0.5f - playerSize.y * 0.5f-5.0f);
		player->SetGround();
		player->GetTransform()->SetVel(player->GetTransform()->GetVel().x, 0.0f);
	}
	if (dir == DOWN)
	{
		player->GetTransform()->SetPos(playerPos.x, pos.y + size.y * 0.5f + playerSize.y * 0.5f+5.0f);
	}
	if (dir == LEFT)
	{
		player->GetTransform()->SetPos(pos.x - size.x * 0.5f - playerSize.x * 0.5f, playerPos.y);
	}
	if (dir == RIGHT)
	{
		player->GetTransform()->SetPos(pos.x + size.x * 0.5f + playerSize.x * 0.5f, playerPos.y);
	}
	
}

void MoveSwitchBlockIdle::ColidObject()
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
		if (targetColidPool->GetColider()[0].GetTag() == "Death")continue;
		if (targetColidPool->GetColider()[0].GetTag() == "Coin")continue;

		
		Colider2D moveColid = obj->GetComponent<ColiderPool>()->GetColider()[0];
		Colider2D targetColid = target->GetComponent<ColiderPool>()->GetColider()[0];
		D3DXVECTOR2 pos = moveColid.GetPos();
		D3DXVECTOR2 size = moveColid.GetSize();
		D3DXVECTOR2 targetPos = targetColid.GetPos();
		D3DXVECTOR2 targetSize = targetColid.GetSize();
		D3DXVECTOR2 currentPos = obj->GetTransform()->GetPos();
		ColidDir dir = moveColid.IsDirColid(targetColid);
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

void MoveSwitchBlockIdle::ChangeVec()
{
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();

	if (GetCommonSwitch())
	{
		moveVec = -1;
	}
	else
	{
		moveVec = 1;
	}
	
	if (pos.y > firstPos.y)
	{
		obj->GetTransform()->SetPos(firstPos);
	}
	if (pos.y < firstPos.y - MOVE_SWITCH_BLOCK_LANGE)
	{
		obj->GetTransform()->SetPos(firstPos.x, firstPos.y- MOVE_SWITCH_BLOCK_LANGE);
	}
}

void MoveSwitchBlockIdle::Move()
{
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();

	obj->GetTransform()->SetPos(pos.x, pos.y + MOVE_SWITCH_BLOCK_VELOCITY*moveVec);
}
