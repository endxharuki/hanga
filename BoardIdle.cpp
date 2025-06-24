#include "BoardIdle.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "Player.h"
#include "MainInGame.h"
#include "OnGameData.h"
#include "Board.h"

void BoardIdle::SetUp()
{

	CleanRequest();

	blockTex[0] = LoadTexture((char*)"data/TEXTURE/kanban.png");
	blockTex[1] = LoadTexture((char*)"data/TEXTURE/kanban_R.png");
	popUp = obj->GetTex();

	drawMode = 0;

	transform = obj->GetTransform();
	colidPool = obj->GetComponent<ColiderPool>();

	prevHasSwap = OnGameData::GetInstance()->HasSwap();

	p_player = MainInGame::player;

}

void BoardIdle::CleanUp()
{

}

void BoardIdle::OnUpdate()
{

	if (prevHasSwap != OnGameData::GetInstance()->HasSwap())
	{
		drawMode = (drawMode + 1) % 2;
		prevHasSwap = !prevHasSwap;
	}

	ColidPlayer();


}

void BoardIdle::OnDraw()
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
	DrawSprite(popUp,
		drawPos.x, drawPos.y - size.y*0.15f,
		size.x*0.65f, size.y*0.4f,
		0.0f, 0.0f,
		1.0f, 1.0f
	);

	if (Pop == true) {
		if (popUp < 0)return;
		DrawSpriteColor(popUp,
			drawPos.x + 200.0f, drawPos.y - 400.0f,
			950.0f, 500.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,1.0f,1.0f,1.0f,1.0f
		);
	}

}

void BoardIdle::ColidPlayer()
{
	if (colidPool->GetColider()[0].IsColid(p_player->GetComponent<ColiderPool>()->GetColider()[0]))
	{
		Pop = true;
	}
	else {
		Pop = false;
	}

}
