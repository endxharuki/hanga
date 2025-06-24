#include "BulletIdle.h"
#include "sprite.h"
#include "texture.h"
#include "Camera.h"
#include "OnGameData.h"
#include "ObjectPool.h"
#include "MainInGame.h"
#include "Enemmy.h"

void BulletIdle::SetUp()
{
	CleanRequest();

	prevHasSwap = OnGameData::GetInstance()->HasSwap();

	tex[0] = LoadTexture((char*)"data/TEXTURE/Bullet1.png");
	tex[1] = LoadTexture((char*)"data/TEXTURE/Bullet2.png");
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;


	obj->GetTransform()->SetSize(100, 100);
	obj->GetTransform()->SetGravity(0.0f);
	obj->GetTransform()->SetRotation(0.0f);
	obj->GetTransform()->SetRotationVel(0.0f);
}

void BulletIdle::CleanUp()
{
}

void BulletIdle::OnUpdate()
{
	Transform2D* transform = obj->GetTransform();

	// 反転時の処理
	if (prevHasSwap != OnGameData::GetInstance()->HasSwap())
	{
		prevHasSwap = !prevHasSwap;
		drawMode = (drawMode + 1) % 2;
		
		transform->SetVel(-transform->GetVel().x, transform->GetVel().y);
	}

	// 移動処理
	transform->SetPos(transform->GetPos().x + transform->GetVel().x, transform->GetPos().y + transform->GetVel().y);
	
	ColidBlock();
	ColidPlayer();


	D3DXVECTOR2 pos = transform->GetPos();
	D3DXVECTOR2 cam = Camera::GetInstance()->GetOriginPos();
	D3DXVECTOR2 camSize = Camera::GetInstance()->GetSize();
	// 現在のカメラ枠から指定の値離れていた場合消す
	if (pos.x + eraseDistance < cam.x || cam.x + camSize.x < pos.x + eraseDistance)
	{	
		deleteRequest = true;
	}

	if (deleteRequest)
	{
		// 削除処理
		if (MainInGame::objectPool.DeleteRequest(obj))
		{
  			*enemBullet = nullptr;
			return;
		}
		*enemBullet = nullptr;
	}
}

void BulletIdle::OnDraw()
{
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	//obj->GetComponent<ColiderPool>()->GetColider()[0].viewColid(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f));

	DrawSpriteColor(tex[drawMode],
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
}


void BulletIdle::ColidBlock()
{
	if (BlockMap::GetInstance()->IsColidAllBlock(obj->GetTransform(), obj->GetComponent<ColiderPool>()->GetColider()[0]) )
	{	// 何かに衝突時
		deleteRequest = true;
	}

	// オブジェクトとの衝突判定
	std::list<GameObject*>* pool = MainInGame::objectPool.GetPool();

	Colider2D colid = obj->GetComponent<ColiderPool>()->GetColider()[0];

	for (GameObject* target : *pool)
	{
		if (obj == target) continue;
		if (target->GetComponent<Enemmy>() != nullptr) continue;

		ColiderPool* targetColidPool = target->GetComponent<ColiderPool>();
		if (targetColidPool == nullptr) continue;

		if (colid.IsColid(targetColidPool->GetColider()[0]))
		{
 			deleteRequest = true;
			break;
		}
	}
}

void BulletIdle::ColidPlayer()
{
	Colider2D colid = obj->GetComponent<ColiderPool>()->GetColider()[0];
	Colider2D playerColid = MainInGame::player->GetComponent<ColiderPool>()->GetColider()[0];

	if (colid.IsColid(playerColid))
	{
		MainInGame::player->GetComponent<Damaged>()->Damage(1);
		deleteRequest = true;
	}
}