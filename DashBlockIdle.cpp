#include "DashBlockIdle.h"
#include "DashBlock.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "Player.h"
#include "MainInGame.h"
#include "OnGameData.h"

void DashBlockIdle::SetUp()
{
	CleanRequest();

	blockTex[0] = LoadTexture((char*)"data/TEXTURE/DashBlock1.png");
	blockTex[1] = LoadTexture((char*)"data/TEXTURE/DashBlock2.png");
	drawMode = 0;

	// コリジョンの生成
	pool = obj->GetComponent<ColiderPool>();

	pool->Add(Colider2D(obj->GetTransform()));

	// 上判定
	pool->Add(Colider2D(
		obj->GetTransform(),
		D3DXVECTOR2(0.0f, -obj->GetTransform()->GetSize().y / 2),
		D3DXVECTOR2(obj->GetTransform()->GetSize().x - 10.0f, obj->GetTransform()->GetSize().y),
		"NOT"
	));

	// 向きの指定
	moveSign = obj->IsRight() ? 1.0f : -1.0f;

	prevHasSwap = OnGameData::GetInstance()->HasSwap();
}

void DashBlockIdle::CleanUp()
{

}

void DashBlockIdle::OnUpdate()
{
	Player* pPlayer = MainInGame::player;

	// 上の判定に重なっている場合
	if ( pool->GetColider()[1].IsColid(pPlayer->GetComponent<ColiderPool>()->GetColider()[0]))
	{
		Transform2D* transform = pPlayer->GetTransform();
		transform->SetOutVel(transform->GetVel().x + 15.0f * moveSign, transform->GetVel().y);
	}
}

void DashBlockIdle::OnDraw()
{
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;

	// カメラの移動
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	Camera* camera = Camera::GetInstance();
	D3DXVECTOR2 drawPos = D3DXVECTOR2(pos.x - camera->GetOriginPos().x, pos.y - camera->GetOriginPos().y);

	DrawSprite(blockTex[drawMode],
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		size.x / CHIP_X_SIZE * moveSign, size.y / CHIP_Y_SIZE
	);
}
