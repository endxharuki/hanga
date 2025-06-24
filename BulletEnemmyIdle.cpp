#include "BulletEnemmyIdle.h"
#include "MainInGame.h"
#include "sprite.h"
#include "texture.h"
#include "Player.h"
#include "Camera.h"
#include "OnGameData.h"

void BulletEnemmyIdle::SetUp()
{
	CleanRequest();

	tex[0] = LoadTexture((char*)"data/TEXTURE/Enemmy1.png");
	tex[1] = LoadTexture((char*)"data/TEXTURE/Enemmy2.png");
	drawMode = 0;

	prevSwap = OnGameData::GetInstance()->HasSwap();

	obj->GetTransform()->SetGravity(0.0f);
	obj->GetTransform()->SetRotation(0.0f);
	obj->GetTransform()->SetRotationVel(0.0f);
}

void BulletEnemmyIdle::CleanUp()
{
}

void BulletEnemmyIdle::OnUpdate()
{
	if (prevSwap != OnGameData::GetInstance()->HasSwap())
	{
		drawMode = (drawMode + 1) % 2;
		prevSwap = !prevSwap;
	}

	if (isDie)return;
	if (ColidBullet())
	{
		isDie = true;
		MainInGame::objectPool.DeleteRequest(obj);
		return;
	}

	// 常にプレイヤーを向く
	Player* player = MainInGame::player;
	if (player->GetTransform()->GetPos().x > obj->GetTransform()->GetPos().x)
	{
		shotDir = 1.0f;
	}
	else
	{
		shotDir = -1.0f;
	}

	Shot();
}

void BulletEnemmyIdle::OnDraw()
{
	if (isDie)return;
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	//obj->GetColider()[0].viewColid(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.3f));

	DrawSpriteColor(tex[drawMode],
		drawPos.x, drawPos.y,
		obj->GetTransform()->GetSize().x, obj->GetTransform()->GetSize().y,
		0.0f, 0.0f,
		1.0f * (-1.0f * shotDir), 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
}

bool BulletEnemmyIdle::ColidBullet()
{
	std::list<GameObject*>* pool = MainInGame::objectPool.GetPool();

	for (GameObject* object : *pool)
	{
		GameObject* gameObj = object;
		if (gameObj == nullptr) continue;

		ColiderPool* targetPool = gameObj->GetComponent<ColiderPool>();
		if (targetPool == nullptr) continue;

		std::vector<Colider2D> objColid = targetPool->GetColider();

		if (objColid[0].GetTag() != "Bullet") continue;

		if (obj->GetComponent<ColiderPool>()->GetColider()[0].IsColid(objColid[0]))
		{
			// 弾の消去
			MainInGame::objectPool.DeleteRequest(gameObj);
			return true;
		}
	}

	return false;
}

void BulletEnemmyIdle::Shot()
{
	// 弾が出ている場合はそれ以上発射しない
	if (bullet != nullptr) return;
	Transform2D* transform = obj->GetTransform();
	Player* player = MainInGame::player;
	D3DXVECTOR2 playerPos = player->GetTransform()->GetPos();
	D3DXVECTOR2 playerSize = player->GetTransform()->GetSize();

	float height = transform->GetPos().y - shotHeight;

	// プレイヤーの位置の確認
	if (playerPos.y - playerSize.y / 2 <= height && height <= playerPos.y + playerSize.y / 2
		&& fabsf(playerPos.x - transform->GetPos().x) <= 900.0f)
	{
		D3DXVECTOR2 bulletPos = D3DXVECTOR2(transform->GetPos().x + (75.0f * shotDir), height);

		// 弾の生成
		Bullet* generate = new Bullet(&bullet ,bulletPos, bulletSize, bulletVel * shotDir);
		bullet = generate;
		bullet->Start();

		MainInGame::objectPool.Add(bullet);
	}
}
