#include "BulletSwingEnemmyIdle.h"
#include "MainInGame.h"
#include "sprite.h"
#include "texture.h"
#include "Player.h"
#include "Camera.h"
#include "OnGameData.h"
#include "sound.h"

#define SWING_TEX_X (5)
#define SWING_TEX_Y (3)
#define SWING_TEX_MAX (11*3)
#define TATE_LANGE_MIN (3.14f/180.0f*70.0f)
#define TATE_LANGE_MAX (3.14f/180.0f*110.0f)

void BulletSwingEnemmyIdle::SetUp()
{
	CleanRequest();

	idleTex[0] = LoadTexture((char*)"data/TEXTURE/flowerRed_idle.png");
	idleTex[1] = LoadTexture((char*)"data/TEXTURE/flowerRed_idle_R.png");
	tateTex[0] = LoadTexture((char*)"data/TEXTURE/flowerRedattack_ue.png");
	tateTex[1] = LoadTexture((char*)"data/TEXTURE/flowerRedattack_ue_R.png");
	yokoTex[0] = LoadTexture((char*)"data/TEXTURE/flowerRedattack_yoko.png");
	yokoTex[1] = LoadTexture((char*)"data/TEXTURE/flowerRedattack_yoko_R.png");
	nanameTex[0] = LoadTexture((char*)"data/TEXTURE/flowerRedattack_naname.png");
	nanameTex[1] = LoadTexture((char*)"data/TEXTURE/flowerRedattack_naname_R.png");
	drawMode = 0;

	prevSwap = OnGameData::GetInstance()->HasSwap();

	obj->GetTransform()->SetGravity(0.0f);
	obj->GetTransform()->SetRotation(0.0f);
	obj->GetTransform()->SetRotationVel(0.0f);

	startPosY = obj->GetTransform()->GetPos().y;

	shotSE = LoadSound((char*)"data/SE/flowerattck.wav");
}

void BulletSwingEnemmyIdle::CleanUp()
{
}

void BulletSwingEnemmyIdle::OnUpdate()
{
	if (!isDie)
	{
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

		ColidPlayer();
		Shot();
	}
	DeathRendition();
}

void BulletSwingEnemmyIdle::OnDraw()
{
	drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	//obj->GetColider()[0].viewColid(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.3f));
	Player* player = MainInGame::player;
	Transform2D* transform = obj->GetTransform();
	D3DXVECTOR2 playerPos = player->GetTransform()->GetPos();
	int revarse = 1;
	if(drawMode ==1) revarse = -1;
	if (shotFrameCount < BULLET_ENEMY_SHOT_FRAME - SWING_TEX_MAX)//待機
	{
		iUV.x = (shotFrameCount % SWING_TEX_X) * (1.0f / SWING_TEX_X);
		iUV.y = (shotFrameCount / SWING_TEX_X) * (1.0f / (SWING_TEX_Y - 1));
		iUV.z = 1.0f / SWING_TEX_X;
		iUV.w = 1.0f / (SWING_TEX_Y - 1);

		DrawSpriteColorRotate(idleTex[drawMode],
			drawPos.x, drawPos.y,
			obj->GetTransform()->GetSize().x, obj->GetTransform()->GetSize().y,
			iUV.x, iUV.y,
			iUV.z * (-1.0f * shotDir* revarse), iUV.w,
			1.0f, 1.0f, 1.0f, 1.0f, dieRot * 2.0f);
	}
	else
	{
		int shotFrame = BULLET_ENEMY_SHOT_FRAME - shotFrameCount;
		shotFrame /= 3;
		aUV.x = (shotFrame % SWING_TEX_X) * (1.0f / SWING_TEX_X);
		aUV.y = (shotFrame / SWING_TEX_X) * (1.0f / SWING_TEX_Y);
		aUV.z = 1.0f / SWING_TEX_X;
		aUV.w = 1.0f / SWING_TEX_Y;
		
		float height = transform->GetPos().y - shotHeight;
		D3DXVECTOR2 bulletPos = D3DXVECTOR2(transform->GetPos().x + (75.0f * shotDir), height);
		double rad = atan2((bulletPos.y - playerPos.y), (bulletPos.x- playerPos.x));
		//rad = abs(rad);
		if (rad < 0)
		{
			DrawSpriteColorRotate(yokoTex[drawMode],
				drawPos.x, drawPos.y,
				obj->GetTransform()->GetSize().x, obj->GetTransform()->GetSize().y,
				aUV.x, aUV.y,
				aUV.z * (-1.0f * shotDir* revarse), aUV.w,
				1.0f, 1.0f, 1.0f, 1.0f, dieRot * 2.0f);
		}
		else if (TATE_LANGE_MIN < rad && rad < TATE_LANGE_MAX)
		{
			DrawSpriteColorRotate(tateTex[drawMode],
				drawPos.x, drawPos.y,
				obj->GetTransform()->GetSize().x, obj->GetTransform()->GetSize().y,
				aUV.x, aUV.y,
				aUV.z * (-1.0f * shotDir* revarse), aUV.w,
				1.0f, 1.0f, 1.0f, 1.0f, dieRot * 2.0f);
		}
		else
		{
			DrawSpriteColorRotate(nanameTex[drawMode],
				drawPos.x, drawPos.y,
				obj->GetTransform()->GetSize().x, obj->GetTransform()->GetSize().y,
				aUV.x, aUV.y,
				aUV.z * (-1.0f * shotDir* revarse), aUV.w,
				1.0f,1.0f,1.0f,1.0f,dieRot*2.0f);
		}
	}
	
}

bool BulletSwingEnemmyIdle::ColidBullet()
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

void BulletSwingEnemmyIdle::Shot()
{
	shotFrameCount++;
	if (shotFrameCount < BULLET_ENEMY_SHOT_FRAME - SWING_TEX_MAX)return;
	Transform2D* transform = obj->GetTransform();
	Player* player = MainInGame::player;
	D3DXVECTOR2 playerPos = player->GetTransform()->GetPos();
	if (!(fabsf(playerPos.x - transform->GetPos().x) <= 480.0f
		&& fabsf(playerPos.y - transform->GetPos().y) <= 400.0f))
	{
		shotFrameCount = BULLET_ENEMY_SHOT_FRAME - SWING_TEX_MAX-1;
		return;
	}
	//180fごとに発射
		if (shotFrameCount < BULLET_ENEMY_SHOT_FRAME)return;
	D3DXVECTOR2 playerSize = player->GetTransform()->GetSize();


	float height = transform->GetPos().y - shotHeight;

	D3DXVECTOR2 bulletPos = D3DXVECTOR2(transform->GetPos().x + (75.0f * shotDir), height);


	/*
	D3DXVECTOR2 vDistance;
	vDistance = bulletPos - playerPos;
	float length;
	length = vDistance.x * vDistance.x + vDistance.y * vDistance.y;
	float size = 480.0f * 480.0f;
	if (length < size)//円判定
	*/

	// プレイヤーの位置の確認
	if (fabsf(playerPos.x - transform->GetPos().x) <= 480.0f
		&& fabsf(playerPos.y - transform->GetPos().y) <= 400.0f)//長方形判定
	{
		

		double rad = atan2((playerPos.y - bulletPos.y), (playerPos.x - bulletPos.x));
		
		// 弾の生成
		Bullet* generate = new Bullet(&bullet ,bulletPos, bulletSize, D3DXVECTOR2(cos(rad) * bulletVel.x, sin(rad) * bulletVel.x));
		bullet = generate;
		bullet->Start();

		MainInGame::objectPool.Add(bullet);

		shotFrameCount = 0;

		PlaySound(shotSE, 0);
	}
}

void BulletSwingEnemmyIdle::ColidPlayer()
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

void BulletSwingEnemmyIdle::DeathRendition()
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
