#include "WallEnemIdle.h"
#include "MainInGame.h"
#include "sprite.h"
#include "texture.h"
#include "Player.h"
#include "OnGameData.h"

#define ANIM_X (10)

void WallEnemIdle::SetUp()
{
	CleanRequest();

	tex[0] = LoadTexture((char*)"data/TEXTURE/mist.png");
	tex[1] = LoadTexture((char*)"data/TEXTURE/mist.png");
	drawMode = 0;

	prevSwap = OnGameData::GetInstance()->HasSwap();

	obj->GetTransform()->SetGravity(0.0f);
	obj->GetTransform()->SetRotation(0.0f);
	obj->GetTransform()->SetRotationVel(0.0f);

	camera = Camera::GetInstance();
	camera->SetLeftLimit(0);

	animFrame = 0;
}

void WallEnemIdle::CleanUp()
{
}

void WallEnemIdle::OnUpdate()
{
	if (isDie)return;

	Transform2D* transform = obj->GetTransform();

	// 常に移動し続ける
	D3DXVECTOR2 pos = transform->GetPos();
	transform->SetPos(pos.x + 1.0f, pos.y);

	// 常にプレイヤー視点の左端に合わせる
	if (camera->GetLeftLimit() < camera->GetOriginPos().x
		&& camera->GetOriginPos().x > transform->GetPos().x + transform->GetSize().x)
	{
		transform->SetPos(camera->GetOriginPos().x - transform->GetSize().x, transform->GetPos().y);
	}

	// 左端をカメラの移動制限幅に合わせる
	camera->SetLeftLimit(transform->GetPos().x - transform->GetSize().x / 2);

	// アニメーション
	++animFrame;
	uv.x = (float)(animFrame / 5 % ANIM_X * (1.0f / ANIM_X));
	uv.y = 0.0f;

	ColidPlayer();
}

void WallEnemIdle::OnDraw()
{
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;

	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	DrawSpriteColor(tex[drawMode],
		drawPos.x, drawPos.y,
		obj->GetTransform()->GetSize().x, obj->GetTransform()->GetSize().y,
		uv.x, 0.0f,
		(float)1.0f / ANIM_X - 0.01f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
}

void WallEnemIdle::ColidPlayer()
{
	Player* player = MainInGame::player;

	Colider2D pColid = player->GetComponent<ColiderPool>()->GetColider()[0];

	if (obj->GetComponent<ColiderPool>()->GetColider()[0].IsColid(pColid))
	{	// 接触した場合
		player->GetComponent<Damaged>()->Damage(3);
	}
}

