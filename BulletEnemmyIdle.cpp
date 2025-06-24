#include "BulletEnemmyIdle.h"
#include "MainInGame.h"
#include "sprite.h"
#include "texture.h"
#include "Player.h"
#include "Camera.h"
#include "OnGameData.h"
#include "sound.h"

void BulletEnemmyIdle::SetUp()
{
	CleanRequest();

	tex[0] = LoadTexture((char*)"data/TEXTURE/flower.png");
	tex[1] = LoadTexture((char*)"data/TEXTURE/Enemmy2.png");
	idleTex[0] = LoadTexture((char*)"data/TEXTURE/flower_idle.png");
	idleTex[1] = LoadTexture((char*)"data/TEXTURE/flower_idle_R.png");
	attackTex[0] = LoadTexture((char*)"data/TEXTURE/flower_attack.png");
	attackTex[1] = LoadTexture((char*)"data/TEXTURE/flower_attack_R.png");
	drawMode = 0;

	prevSwap = OnGameData::GetInstance()->HasSwap();

	obj->GetTransform()->SetGravity(0.0f);
	obj->GetTransform()->SetRotation(0.0f);
	obj->GetTransform()->SetRotationVel(0.0f);

	shotSE = LoadSound((char*)"data/SE/flowerattck.wav");

	isIdle = true;
	frameCnt = 0;
	idleFrame = 0;
	shotFrame = 0;

	startPosY = obj->GetTransform()->GetPos().y;
}

void BulletEnemmyIdle::CleanUp()
{
}

void BulletEnemmyIdle::OnUpdate()
{
	if (!isDie)
	{
		if (ColidBullet())
		{
			isDie = true;
			obj->GetComponent<ColiderPool>()->GetColider()[0].SetTag("Death");
			OnGameData::GetInstance()->AddScore(200);
			OnGameData::GetInstance()->AddHitEnemyNum();
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

		shotFrameCount++;

		// アニメーション
		if (shotFrameCount >= BULLET_ENEMY_SHOT_FRAME - 40)
		{
			idleFrame = 0;

			if (isIdle)
			{
				frameCnt = 0;
				isIdle = false;
			}
			++frameCnt;
			if (frameCnt % 4 == 0) ++shotFrame;

			uv.x = (float)((shotFrame % idleX) * (1.0f / idleX));
			uv.y = (float)((shotFrame / idleX) * (1.0f / attackY));
			uv.z = (float)1.0f / idleX;
			uv.w = (float)1.0f / attackY;
		}
		else
		{
			shotFrame = 0;

			if (!isIdle)
			{
				frameCnt = 0;
				isIdle = true;
			}
			++frameCnt;
			if (frameCnt % 4 == 0)++idleFrame;

			uv.x = (float)((idleFrame % idleX) * (1.0f / idleX));
			uv.y = (float)((idleFrame / idleX) * (1.0f / idleY));
			uv.z = (float)1.0f / idleX;
			uv.w = (float)1.0f / idleY;
		}

		ColidPlayer();
		Shot();
	}
	DeathRendition();
}

void BulletEnemmyIdle::OnDraw()
{
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	//obj->GetColider()[0].viewColid(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.3f));
	int revarse = 1;
	if (drawMode == 1) revarse = -1;
	if (isIdle)
	{
		DrawSpriteColorRotate(idleTex[drawMode],
			drawPos.x, drawPos.y,
			size.x, size.y,
			uv.x, uv.y,
			uv.z * -shotDir* revarse, uv.w,
			1.0f, 1.0f, 1.0f, 1.0f, dieRot * 2.0f);
	}
	else
	{
		DrawSpriteColorRotate(attackTex[drawMode],
			drawPos.x, drawPos.y,
			obj->GetTransform()->GetSize().x, obj->GetTransform()->GetSize().y,
			uv.x, uv.y,
			uv.z * -shotDir* revarse, uv.w,
			1.0f, 1.0f, 1.0f, 1.0f, dieRot * 2.0f);
	}
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
	//180fごとに発射
	if (shotFrameCount < BULLET_ENEMY_SHOT_FRAME)return;
	Transform2D* transform = obj->GetTransform();
	Player* player = MainInGame::player;
	D3DXVECTOR2 playerPos = player->GetTransform()->GetPos();
	D3DXVECTOR2 playerSize = player->GetTransform()->GetSize();

	float height = transform->GetPos().y - shotHeight;

	// プレイヤーの位置の確認
	D3DXVECTOR2 bulletPos = D3DXVECTOR2(transform->GetPos().x + (75.0f * shotDir), height);

	// 弾の生成
	Bullet* generate = new Bullet(&bullet ,bulletPos, bulletSize, bulletVel * shotDir);
	bullet = generate;
	bullet->Start();

	MainInGame::objectPool.Add(bullet);

	shotFrameCount = 0;

	if (playerPos.y - playerSize.y / 2 <= height && height <= playerPos.y + playerSize.y / 2
		&& fabsf(playerPos.x - transform->GetPos().x) <= 900.0f)
	{
		PlaySound(shotSE, 0);
	}
}

void BulletEnemmyIdle::ColidPlayer()
{
	Player* player = MainInGame::player;
	float PlayerBottom = player->GetTransform()->GetPos().y + (player->GetTransform()->GetSize().y / 2);

	Colider2D colid = obj->GetComponent<ColiderPool>()->GetColider()[0];
	Colider2D playerColid = MainInGame::player->GetComponent<ColiderPool>()->GetColider()[0];

	if (colid.IsColid(playerColid))
	{
		MainInGame::player->GetComponent<Damaged>()->Damage(1);
	}
}

void BulletEnemmyIdle::DeathRendition()
{
	if (!isDie)return;
	dieRot += 3.14f / 180 * 6;
	int revarse = 1;
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	if (shotDir < 0)revarse = -1;
	if (dieRot < 3.14f)
	{
		obj->GetTransform()->SetPos(pos.x + cos(dieRot + 1.57f) * 4.0f * revarse, pos.y + sin(dieRot + 4.71f) * 10.0f);
	}
	else
	{
		obj->GetTransform()->SetPos(pos.x - 2.0f * revarse, pos.y + dieRot * 3.5f);
	}
	if (obj->GetTransform()->GetPos().y - startPosY >= 2000.0f)
	{
		MainInGame::objectPool.DeleteRequest(obj);
	}
}
