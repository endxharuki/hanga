#include "CheckPointIdle.h"
#include "MainInGame.h"
#include "texture.h"
#include "sprite.h"

void CheckPointIdle::SetUp()
{
	CleanRequest();

	colidPool = obj->GetComponent<ColiderPool>();
	transform = obj->GetTransform();

	checkPointActive[0] = LoadTexture((char*)"data/TEXTURE/CheckPoint/Flag_blue.png");
	checkPointActive[1] = LoadTexture((char*)"data/TEXTURE/CheckPoint/Flag_R.png");

	checkPointIdle[0] = LoadTexture((char*)"data/TEXTURE/CheckPoint/Flag_red.png");
	checkPointIdle[1] = LoadTexture((char*)"data/TEXTURE/CheckPoint/Flag_R.png");
}

void CheckPointIdle::CleanUp()
{
}

void CheckPointIdle::OnUpdate()
{
	isCheckPointed = OnGameData::GetInstance()->IsCheckPoint();

	float px = MainInGame::player->GetTransform()->GetPos().x;
	if (fabsf(px - transform->GetPos().x) > 2000.0f)return;

	if (!isCheckPointed)
	{
		ColidPlayer();
	}
}

void CheckPointIdle::OnDraw()
{
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;

	D3DXVECTOR2 pos = transform->GetPos();
	D3DXVECTOR2 size = transform->GetSize();
	D3DXVECTOR2 camera = Camera::GetInstance()->GetOriginPos();

	pos = pos - camera;

	if (isCheckPointed)
	{
		DrawSpriteColor(checkPointActive[drawMode],
			pos.x, pos.y,
			size.x, size.y,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		DrawSpriteColor(checkPointIdle[drawMode],
			pos.x, pos.y,
			size.x, size.y,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void CheckPointIdle::ColidPlayer()
{
	if (MainInGame::player->GetComponent<ColiderPool>()->GetColider()[0].IsColid(colidPool->GetColider()[0]))
	{
		OnGameData::GetInstance()->SetIsCheckPoint(true);
	}
}
