#include "LandBlockIdle.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "OnGameData.h"
#include "ColiderPool.h"

void LandBlockIdle::SetUp()
{
	CleanRequest();

	if (type == 0)
	{
		blockTex[0] = LoadTexture((char*)"data/TEXTURE/soil_2.png");
		blockTex[1] = LoadTexture((char*)"data/TEXTURE/soil_2_R.png");
	}
	else
	{
		blockTex[0] = LoadTexture((char*)"data/TEXTURE/grass.png");
		blockTex[1] = LoadTexture((char*)"data/TEXTURE/grass_R.png");
	}

	// ƒRƒŠƒWƒ‡ƒ“‚Ì’Ç‰Á
	obj->GetComponent<ColiderPool>()->Add(
		Colider2D(obj->GetTransform())
	);
}

void LandBlockIdle::CleanUp()
{

}

void LandBlockIdle::OnUpdate()
{
	
}

void LandBlockIdle::OnDraw()
{
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;

	// ƒJƒƒ‰‚ÌˆÚ“®
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

	//obj->GetColider()[0].viewColid(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.3f));
}
