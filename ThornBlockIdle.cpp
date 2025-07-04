#include "ThornBlockIdle.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "OnGameData.h"
#include "ColiderPool.h"
#include "MainInGame.h"
#include "Damaged.h"

void ThornBlockIdle::SetUp()
{
	CleanRequest();

	blockTex[0] = LoadTexture((char*)"data/TEXTURE/17_toge.png");
	blockTex[1] = LoadTexture((char*)"data/TEXTURE/toge_hanten.png");

	pool = obj->GetComponent<ColiderPool>();

	drawMode = 0;
}

void ThornBlockIdle::CleanUp()
{

}

void ThornBlockIdle::OnUpdate()
{
	ColidPlayer();
}

void ThornBlockIdle::OnDraw()
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
		size.x / CHIP_X_SIZE, size.y / CHIP_Y_SIZE
	);
}

void ThornBlockIdle::ColidPlayer()
{

}
