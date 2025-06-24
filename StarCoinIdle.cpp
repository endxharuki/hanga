#include "StarCoinIdle.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "Player.h"
#include "MainInGame.h"
#include "OnGameData.h"
#include "sound.h"

void StarCoinIdle::SetUp()
{
	CleanRequest();

	blockTex[0] = LoadTexture((char*)"data/TEXTURE/11_Starcoin.png");
	blockTex[1] = LoadTexture((char*)"data/TEXTURE/11_Starcoin_hanten.png");
	drawMode = 0;

	se = LoadSound((char*)"data/SE/starcoin.wav");

	transform = obj->GetTransform();
	colidPool = obj->GetComponent<ColiderPool>();

	prevHasSwap = OnGameData::GetInstance()->HasSwap();

	p_player = MainInGame::player;
}

void StarCoinIdle::CleanUp()
{

}

void StarCoinIdle::OnUpdate()
{
	if (prevHasSwap != OnGameData::GetInstance()->HasSwap())
	{
		drawMode = (drawMode + 1) % 2;
		prevHasSwap = !prevHasSwap;
	}

	ColidPlayer();
}

void StarCoinIdle::OnDraw()
{
	// ƒJƒƒ‰‚ÌˆÚ“®
	Camera* camera = Camera::GetInstance();
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	D3DXVECTOR2 drawPos = D3DXVECTOR2(pos.x - camera->GetOriginPos().x, pos.y - camera->GetOriginPos().y);

	DrawSprite(blockTex[drawMode],
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		1.0f, 1.0f
	);

}

void StarCoinIdle::ColidPlayer()
{
	if (colidPool->GetColider()[0].IsColid(p_player->GetComponent<ColiderPool>()->GetColider()[0]))
	{
		OnGameData::GetInstance()->AddStarCoinNum();
		MainInGame::objectPool.DeleteRequest(obj);

		PlaySound(se, 0);
	}
}


