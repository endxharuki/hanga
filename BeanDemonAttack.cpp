#include "BeanDemonAttack.h"
#include "BeanDemon.h"
#include "Player.h"
#include "MainInGame.h"
#include "Bullet.h"
#include "Camera.h"
#include "Sprite.h"

void BeanDemonAttack::SetUp()
{
	CleanRequest();

	int bossLife = obj->GetLife();

	// ”­ŽËŠÔŠu‚ÌÝ’è
	if (bossLife >= 3) shootFrame = 180;
	else if (bossLife >= 2) shootFrame = 120;
	else if (bossLife >= 1) shootFrame = 100;

	frameCnt = 0;
	shootCnt = 0;
}

void BeanDemonAttack::CleanUp()
{
}

void BeanDemonAttack::OnUpdate()
{
	++frameCnt;
	if (frameCnt >= shootFrame)
	{
		Shoot();
		frameCnt = 0;
		++shootCnt;
	}

	if (shootCnt >= maxShootCnt) ChangeRequest("Move");
}

void BeanDemonAttack::OnDraw()
{
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	DrawSpriteColor(obj->idleTex,
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);

}

void BeanDemonAttack::Shoot()
{
	Transform2D* trans = obj->GetTransform();

	// ƒvƒŒƒCƒ„[î•ñŽæ“¾
	Player* player = MainInGame::player;
	Transform2D* pTrans = player->GetTransform();

	D3DXVECTOR2 pos = trans->GetPos();
	D3DXVECTOR2 size = trans->GetSize();
	D3DXVECTOR2 target = pTrans->GetPos();

	D3DXVECTOR2 shootDir = target - pos;
	D3DXVECTOR2 normalDir;
	float dirLength = sqrtf(shootDir.x * shootDir.x + shootDir.y * shootDir.y);
	normalDir.x = shootDir.x / dirLength;
	normalDir.y = shootDir.y / dirLength;

	D3DXVECTOR2 shootPos;
	shootPos.x = normalDir.x * (size.x * 0.85f) + pos.x;
	shootPos.y = normalDir.y * (size.y * 0.75f) + pos.y;

	D3DXVECTOR2 shootSize = D3DXVECTOR2(CHIP_X_SIZE, CHIP_Y_SIZE);

	Bullet* bullet = new Bullet(shootPos, shootSize, normalDir * 10);
	bullet->Start();

	MainInGame::objectPool.Add(bullet);
}
