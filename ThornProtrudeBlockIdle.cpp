#include "ThornProtrudeBlockIdle.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "OnGameData.h"
#include "ColiderPool.h"
#include "MainInGame.h"
#include "Damaged.h"

void ThornProtrudeBlockIdle::SetUp()
{
	CleanRequest();

	blockTex[0] = LoadTexture((char*)"data/TEXTURE/trap.png");
	blockTex[1] = LoadTexture((char*)"data/TEXTURE/trap_R.png");
	thornTex[0] = LoadTexture((char*)"data/TEXTURE/17_toge.png");
	thornTex[1] = LoadTexture((char*)"data/TEXTURE/toge_hanten.png");

	pool = obj->GetComponent<ColiderPool>();

	drawMode = 0;

	firstPos = obj->GetTransform()->GetPos();
}

void ThornProtrudeBlockIdle::CleanUp()
{

}

void ThornProtrudeBlockIdle::OnUpdate()
{
	protrudeCount++;
	if (protrudeCount > PROTRUDE_FIN)//終わり
	{
		protrudeCount = 0;
	}
	else if (protrudeCount > PROTRUDE_SHRINK_FIRST)//縮む
	{
		Shrink();
	}
	else if (protrudeCount > PROTRUDE_KEEP_FIRST)//伸びたまま
	{
		obj->GetComponent<ColiderPool>()->GetColider()[1].SetPos(obj->GetTransform()->GetPos().x, firstPos.y - obj->GetTransform()->GetSize().y * 1.0f+obj->GetTransform()->GetSize().y * 0.1f);
	}
	else if (protrudeCount > PROTRUDE_STRETCH_FIRST)//伸びる
	{
		Stretch();
	}
	else if (protrudeCount > PROTRUDE_LITTLE_KEEP_FIRST)//ちょっと伸びたまま
	{
		obj->GetComponent<ColiderPool>()->GetColider()[1].SetPos(obj->GetTransform()->GetPos().x, firstPos.y - obj->GetTransform()->GetSize().y * 0.19f + obj->GetTransform()->GetSize().y * 0.1f);
	}
	else if (protrudeCount > PROTRUDE_LITTLE_STRETCH_FIRST)//ちょっと伸びる
	{
		LittleStretch();
	}
	else if (protrudeCount > 0)//待機
	{
		obj->GetComponent<ColiderPool>()->GetColider()[1].SetPos(obj->GetTransform()->GetPos().x, firstPos.y - obj->GetTransform()->GetSize().y * 0.0f + obj->GetTransform()->GetSize().y * 0.1f);
	}
	
	
	
	

	ColidPlayer();
	ColidObject();
}

void ThornProtrudeBlockIdle::OnDraw()
{
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;

	// カメラの移動
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	Camera* camera = Camera::GetInstance();
	D3DXVECTOR2 drawPos = D3DXVECTOR2(pos.x - camera->GetOriginPos().x, pos.y - camera->GetOriginPos().y);
	D3DXVECTOR2 drawFirstPos = D3DXVECTOR2(firstPos.x - camera->GetOriginPos().x, firstPos.y - camera->GetOriginPos().y);

	DrawSprite(thornTex[drawMode],
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		size.x / CHIP_X_SIZE, size.y / CHIP_Y_SIZE
	);
	
	DrawSprite(blockTex[drawMode],
		drawFirstPos.x, drawFirstPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		size.x / CHIP_X_SIZE, size.y / CHIP_Y_SIZE
	);
}

void ThornProtrudeBlockIdle::ColidPlayer()
{
	Player* player = MainInGame::player;

	D3DXVECTOR2 currentPos = obj->GetTransform()->GetPos();
	obj->GetTransform()->SetPos(firstPos);
	Colider2D landColid = obj->GetComponent<ColiderPool>()->GetColider()[0];
	D3DXVECTOR2 pos = landColid.GetPos();
	D3DXVECTOR2 size = landColid.GetSize();
	Colider2D playerColid = player->GetComponent<ColiderPool>()->GetColider()[0];
	D3DXVECTOR2 playerPos = player->GetTransform()->GetPos();
	D3DXVECTOR2 playerSize = player->GetTransform()->GetSize();
	playerSize.x = playerColid.GetSize().x;
	ColidDir dir = landColid.IsDirColid(playerColid);

	if (dir == UP)
	{
		player->SetGround();
		player->GetTransform()->SetVel(player->GetTransform()->GetVel().x, 0.0f);
		player->GetTransform()->SetPos(playerPos.x, pos.y - size.y * 0.5f - playerSize.y * 0.5f);
	}
	if (dir == DOWN)
	{
		player->GetTransform()->SetPos(playerPos.x, pos.y + size.y * 0.5f + playerSize.y * 0.5f);
	}
	if (dir == LEFT)
	{
		player->GetTransform()->SetPos(pos.x - size.x * 0.5f - playerSize.x * 0.5f, playerPos.y);
	}
	if (dir == RIGHT)
	{
		player->GetTransform()->SetPos(pos.x + size.x * 0.5f + playerSize.x * 0.5f, playerPos.y);
	}
	obj->GetTransform()->SetPos(currentPos);

	Colider2D thornColid = obj->GetComponent<ColiderPool>()->GetColider()[1];
	playerColid = player->GetComponent<ColiderPool>()->GetColider()[0];
	pos = thornColid.GetPos();
	size = thornColid.GetSize();
	playerPos = player->GetTransform()->GetPos();
	playerSize = player->GetTransform()->GetSize();

	dir = thornColid.IsDirColid(playerColid);

	if (dir == UP)
	{
		player->GetComponent<Damaged>()->Damage(1);
		player->SetGround();
		player->GetTransform()->SetVel(player->GetTransform()->GetVel().x, 0.0f);
		player->GetTransform()->SetPos(playerPos.x, pos.y - size.y*0.5f - playerSize.y * 0.5f);
	}
	if (dir == DOWN)
	{
		player->GetComponent<Damaged>()->Damage(1);
		player->GetTransform()->SetPos(playerPos.x, pos.y + size.y * 0.5f + playerSize.y * 0.5f);
	}
	if (dir == LEFT)
	{
		player->GetComponent<Damaged>()->Damage(1);
		player->GetTransform()->SetPos(pos.x - size.x * 0.5f - playerSize.x * 0.5f, playerPos.y);
	}
	if (dir == RIGHT)
	{
		player->GetTransform()->SetPos(pos.x + size.x * 0.5f + playerSize.x * 0.5f, playerPos.y);
	}
	
}

void ThornProtrudeBlockIdle::ColidObject()
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

		
		Colider2D thornColid = obj->GetComponent<ColiderPool>()->GetColider()[1];
		Colider2D targetColid = target->GetComponent<ColiderPool>()->GetColider()[0];
		D3DXVECTOR2 pos = thornColid.GetPos();
		D3DXVECTOR2 size = thornColid.GetSize();
		D3DXVECTOR2 targetPos = targetColid.GetPos();
		D3DXVECTOR2 targetSize = targetColid.GetSize();
		D3DXVECTOR2 currentPos = obj->GetTransform()->GetPos();
		ColidDir dir = thornColid.IsDirColid(targetColid);
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

		
		obj->GetTransform()->SetPos(firstPos);
		Colider2D landColid = obj->GetComponent<ColiderPool>()->GetColider()[0];
		pos = landColid.GetPos();
		size = landColid.GetSize();
		
		dir = landColid.IsDirColid(targetColid);
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
		obj->GetTransform()->SetPos(currentPos);

	}

}

void ThornProtrudeBlockIdle::LittleStretch()
{
	D3DXVECTOR2 pos = obj->GetComponent<ColiderPool>()->GetColider()[1].GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();

	obj->GetComponent<ColiderPool>()->GetColider()[1].SetPos(pos.x, pos.y - ((size.y*0.19f) / (PROTRUDE_LITTLE_STRETCH_DURATION)));
}

void ThornProtrudeBlockIdle::Stretch()
{
	D3DXVECTOR2 pos = obj->GetComponent<ColiderPool>()->GetColider()[1].GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();

	obj->GetComponent<ColiderPool>()->GetColider()[1].SetPos(pos.x, pos.y - ((size.y * 0.8f) / (PROTRUDE_STRETCH_DURATION)));
}

void ThornProtrudeBlockIdle::Shrink()
{
	D3DXVECTOR2 pos = obj->GetComponent<ColiderPool>()->GetColider()[1].GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	
	obj->GetComponent<ColiderPool>()->GetColider()[1].SetPos(pos.x, pos.y + (size.y / (PROTRUDE_SHRINK_DURATION)));
}
