#include "SwitchIdle.h"
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

void SwitchIdle::SetUp()
{
	CleanRequest();

	onTex[0] = LoadTexture((char*)"data/TEXTURE/25_switch_red_re_2.png");
	onTex[1] = LoadTexture((char*)"data/TEXTURE/25_switch_h_re_2.png");
	offTex[0] = LoadTexture((char*)"data/TEXTURE/25_switch_blue_re_2.png");
	offTex[1] = LoadTexture((char*)"data/TEXTURE/25_switch_h_re_2.png");
	pushedTex[0] = LoadTexture((char*)"data/TEXTURE/25_pedestal_re_3.png");
	pushedTex[1] = LoadTexture((char*)"data/TEXTURE/25_pedestal_h_re_3.png");

	pool = obj->GetComponent<ColiderPool>();
	Transform2D* transform = obj->GetTransform();

	drawVec = obj->GetDir();

	if (drawVec == 0)
	{
		pool->Add(
			Colider2D(transform,
				D3DXVECTOR2(0.0f, transform->GetSize().y * 0.25f),
				D3DXVECTOR2(transform->GetSize().x, transform->GetSize().y * 0.5f)
			));//下
	}
	else if (drawVec == 1)
	{
		pool->Add(
			Colider2D(transform,
				D3DXVECTOR2(-transform->GetSize().x * 0.25f, 0.0f),
				D3DXVECTOR2(transform->GetSize().x * 0.5f, transform->GetSize().y)
			));//左
	}
	else if (drawVec == 2)
	{
		pool->Add(
			Colider2D(transform,
				D3DXVECTOR2(0.0f, -transform->GetSize().y * 0.25f),
				D3DXVECTOR2(transform->GetSize().x, transform->GetSize().y * 0.5f)
			));//上
	}
	else
	{
		pool->Add(
			Colider2D(transform,
				D3DXVECTOR2(transform->GetSize().x * 0.25f, 0.0f),
				D3DXVECTOR2(transform->GetSize().x * 0.5f, transform->GetSize().y)
			));//右
	}

	
	
	
	SetCommonSwitch(false);

	drawMode = 0;

	firstPos = obj->GetTransform()->GetPos();
}

void SwitchIdle::CleanUp()
{

}

void SwitchIdle::OnUpdate()
{
	pushed = false;

	ColidPlayer();
	ColidObject();

	ColidPlayerSwitch();
	ColidObjectSwitch();

	Switched();

	prevPshed = pushed;
}

void SwitchIdle::OnDraw()
{
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;

	//pool->GetColider()[1].viewColid({1.0f, 0.0f, 0.0f, 0.5f});

	// カメラの移動
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	Camera* camera = Camera::GetInstance();
	D3DXVECTOR2 drawPos = D3DXVECTOR2(pos.x - camera->GetOriginPos().x, pos.y - camera->GetOriginPos().y);

	float rot = 0.0f;
	D3DXVECTOR2 pushedLength = { 0.0f,0.0f };

	float a = 0.25f;

	if (drawVec == 0)
	{
		rot = 0.0f;
		if (pushed)pushedLength.y = size.y * a;
	}
	else if (drawVec == 1)
	{
		rot = 1.57f;
		if (pushed)pushedLength.x = -size.y * a;
	}
	else if (drawVec == 2)
	{
		rot = 3.14f;
		if (pushed)pushedLength.y = -size.y * a;
	}
	else
	{
		rot = 4.71f;
		if (pushed)pushedLength.x = size.y * a;
	}

	
	if (GetCommonSwitch())
	{
		DrawSpriteColorRotate(onTex[drawMode],
			drawPos.x+pushedLength.x, drawPos.y+ pushedLength.y,
			size.x, size.y,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			rot
		);
	}
	else
	{
		DrawSpriteColorRotate(offTex[drawMode],
			drawPos.x + pushedLength.x, drawPos.y + pushedLength.y,
			size.x, size.y,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			rot
		);
	}

	// カメラの移動
	D3DXVECTOR2 pushedPos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 pushedSize = obj->GetTransform()->GetSize();
	D3DXVECTOR2 drawPushedPos = D3DXVECTOR2(pushedPos.x - camera->GetOriginPos().x, pushedPos.y - camera->GetOriginPos().y);

	DrawSpriteColorRotate(pushedTex[drawMode],
		drawPushedPos.x, drawPushedPos.y,
		pushedSize.x, pushedSize.y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		rot
	);
	
}

void SwitchIdle::ColidPlayer()
{
	Player* player = MainInGame::player;

	Colider2D moveColid = obj->GetComponent<ColiderPool>()->GetColider()[1];
	Colider2D playerColid = player->GetComponent<ColiderPool>()->GetColider()[0];
	D3DXVECTOR2 pos = moveColid.GetPos();
	D3DXVECTOR2 size = moveColid.GetSize();
	D3DXVECTOR2 playerPos = playerColid.GetPos();
	D3DXVECTOR2 playerSize = playerColid.GetSize();
	ColidDir dir = moveColid.IsDirColid(playerColid);
	if (dir == UP)
	{
		player->GetTransform()->SetPos(playerPos.x, pos.y - size.y * 0.5f - playerSize.y * 0.5f - 5.0f);
		player->SetGround();
		player->GetTransform()->SetVel(player->GetTransform()->GetVel().x, 0.0f);
	}
	if (dir == DOWN)
	{
		player->GetTransform()->SetPos(playerPos.x, pos.y + size.y * 0.5f + playerSize.y * 0.5f + 5.0f);
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
void SwitchIdle::ColidPlayerSwitch()
{
	Player* player = MainInGame::player;

	Colider2D moveColid = obj->GetComponent<ColiderPool>()->GetColider()[0];
	Colider2D playerColid = player->GetComponent<ColiderPool>()->GetColider()[0];

	if (moveColid.IsColid(playerColid))
	{
		pushed = true;
	}
	
}

void SwitchIdle::ColidObject()
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

		
		Colider2D moveColid = obj->GetComponent<ColiderPool>()->GetColider()[1];
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

void SwitchIdle::ColidObjectSwitch()
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
		
		if (moveColid.IsColid(targetColid))
		{
			pushed = true;
		}
		
	}

}

void SwitchIdle::Switched()
{
	if (!prevPshed && pushed)//押された瞬間
	{
		if (GetCommonSwitch())
		{
			SetCommonSwitch(false);
		}
		else
		{
			SetCommonSwitch(true);
		}

	}
}
