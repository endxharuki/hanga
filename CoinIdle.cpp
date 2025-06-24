#include "CoinIdle.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "Player.h"
#include "MainInGame.h"
#include "OnGameData.h"
#include "sound.h"

void CoinIdle::SetUp()
{
	CleanRequest();

	blockTex[0] = LoadTexture((char*)"data/TEXTURE/Coin_2.png");
	blockTex[1] = LoadTexture((char*)"data/TEXTURE/Coin_hanten.png");
	se = LoadSound((char*)"data/SE/coin.wav");
	SetVolume(se, 0.5f);

	drawMode = 0;

	transform = obj->GetTransform();
	colidPool = obj->GetComponent<ColiderPool>();

	prevHasSwap = OnGameData::GetInstance()->HasSwap();

	p_player = MainInGame::player;

	drawMode = 0;
}

void CoinIdle::CleanUp()
{

}

void CoinIdle::OnUpdate()
{
	if (prevHasSwap != OnGameData::GetInstance()->HasSwap())
	{
		drawMode = (drawMode + 1) % 2;
		prevHasSwap = !prevHasSwap;
	}

	ColidPlayer();
}

void CoinIdle::OnDraw()
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

void CoinIdle::ColidPlayer()
{
	if (colidPool->GetColider()[0].IsColid(p_player->GetComponent<ColiderPool>()->GetColider()[0]))
	{
		OnGameData::GetInstance()->AddCoinNum(1);
		MainInGame::objectPool.DeleteRequest(obj);
		OnGameData::GetInstance()->AddScore(100);

		PlaySound(se, 0);
	}
}


