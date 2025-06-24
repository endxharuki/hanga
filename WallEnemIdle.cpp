#include "WallEnemIdle.h"
#include "MainInGame.h"
#include "sprite.h"
#include "texture.h"
#include "Player.h"
#include "OnGameData.h"

void WallEnemIdle::SetUp()
{
	CleanRequest();

	tex[0] = LoadTexture((char*)"data/TEXTURE/WallEnem1.png");
	tex[1] = LoadTexture((char*)"data/TEXTURE/WallEnem2.png");
	drawMode = 0;

	prevSwap = OnGameData::GetInstance()->HasSwap();

	obj->GetTransform()->SetGravity(0.0f);
	obj->GetTransform()->SetRotation(0.0f);
	obj->GetTransform()->SetRotationVel(0.0f);

	camera = Camera::GetInstance();
}

void WallEnemIdle::CleanUp()
{
}

void WallEnemIdle::OnUpdate()
{
	Transform2D* transform = obj->GetTransform();

	if (prevSwap != OnGameData::GetInstance()->HasSwap())
	{
		drawMode = (drawMode + 1) % 2;
		prevSwap = !prevSwap;
	}

	if (isDie)return;

	// 常に移動し続ける
	D3DXVECTOR2 pos = transform->GetPos();
	transform->SetPos(pos.x + 1.0f, pos.y);

	// 常にプレイヤー視点の左端に合わせる
	if (camera->GetLeftLimit() < camera->GetOriginPos().x
		&& camera->GetOriginPos().x > transform->GetPos().x - transform->GetSize().x / 2)
	{
		transform->SetPos(camera->GetOriginPos().x + transform->GetSize().x / 2, transform->GetPos().y);
	}

	// 左端をカメラの移動制限幅に合わせる
	camera->SetLeftLimit(transform->GetPos().x - transform->GetSize().x / 2);
;
	ColidPlayer();
}

void WallEnemIdle::OnDraw()
{
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	obj->GetComponent<ColiderPool>()->GetColider()[0].viewColid(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.3f));

	DrawSpriteColor(tex[drawMode],
		drawPos.x, drawPos.y,
		obj->GetTransform()->GetSize().x, obj->GetTransform()->GetSize().y,
		0.0f, 0.0f,
		1.0f, 10.0f,
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

