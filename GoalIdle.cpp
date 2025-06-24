#include "GoalIdle.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "Player.h"
#include "MainInGame.h"
#include "OnGameData.h"

void GoalIdle::SetUp()
{
	CleanRequest();

	blockTex[0] = LoadTexture((char*)"data/TEXTURE/GOAL/24_tourou.PNG");
	blockTex[1] = LoadTexture((char*)"data/TEXTURE/GOAL/24_tourou_h.PNG");

	// あらかじめ読み込んでおく
	LoadTexture((char*)"data/TEXTURE/GOAL/tourou_lit.png");

	drawMode = 0;

	transform = obj->GetTransform();
	colidPool = obj->GetComponent<ColiderPool>();
	
	//中心判定
	colidPool->Add(Colider2D(
		obj->GetTransform(),
		D3DXVECTOR2(0.0f, obj->GetTransform()->GetSize().y / 4),
		D3DXVECTOR2(obj->GetTransform()->GetSize().x/4, obj->GetTransform()->GetSize().y/2)
	));

	prevHasSwap = OnGameData::GetInstance()->HasSwap();

	p_player = MainInGame::player;
}

void GoalIdle::CleanUp()
{

}

void GoalIdle::OnUpdate()
{
	ColidPlayer();
}

void GoalIdle::OnDraw()
{
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;

	//colidPool->GetColider()[0].viewColid();

	// カメラの移動
	Camera* camera = Camera::GetInstance();
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	D3DXVECTOR2 drawPos = D3DXVECTOR2(pos.x - camera->GetOriginPos().x, pos.y - camera->GetOriginPos().y);

	
	DrawSpriteColor(blockTex[drawMode],
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
	

}

void GoalIdle::ColidPlayer()
{
	if (colidPool->GetColider()[0].IsColid(p_player->GetComponent<ColiderPool>()->GetColider()[0]))
	{
		OnGameData::GetInstance()->SetClear(true);
	}
}

