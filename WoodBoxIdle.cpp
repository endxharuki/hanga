#include "WoodBoxIdle.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "Player.h"
#include "MainInGame.h"
#include "OnGameData.h"

void WoodBoxIdle::SetUp()
{
	CleanRequest();

	blockTex[0] = LoadTexture((char*)"data/TEXTURE/BoxWood.png");
	blockTex[1] = LoadTexture((char*)"data/TEXTURE/Box2.png");
	drawMode = 0;

	transform = obj->GetTransform();
	colidPool = obj->GetComponent<ColiderPool>();

	prevHasSwap = OnGameData::GetInstance()->HasSwap();
}

void WoodBoxIdle::CleanUp()
{

}

void WoodBoxIdle::OnUpdate()
{
	if (prevHasSwap != OnGameData::GetInstance()->HasSwap())
	{
		drawMode = (drawMode + 1) % 2;
		prevHasSwap = !prevHasSwap;
	}


	D3DXVECTOR2 pos = transform->GetPos();

	// 重力
	transform->SetPos(pos.x, pos.y + 10.0f);

	BlockMap::GetInstance()->ColidAllBlock(transform, colidPool->GetColider()[0]);
	ColidBox();

	colidPool->GetColider()[0].SetPrevPos(obj->GetTransform()->GetPos());
	colidPool->GetColider()[0].SetFriction(false);
}

void WoodBoxIdle::OnDraw()
{
	// カメラの移動
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

	//colidPool->GetColider()[0].viewColid();
}

void WoodBoxIdle::ColidBox()
{
	std::list<GameObject*>* pool = MainInGame::objectPool.GetPool();

	for (GameObject* target : *pool)
	{
		if (target == obj)continue;
		Pushed* pPush = target->GetComponent<Pushed>();

		// 押せるものの場合計算する
		if (pPush) ColidPush(target, pPush);
	}
}

void WoodBoxIdle::ColidPush(GameObject* target, Pushed* pushed)
{
	Transform2D* trans = obj->GetTransform();
	std::vector<Colider2D> colids = colidPool->GetColider();
	Colider2D targetColid = target->GetComponent<ColiderPool>()->GetColider()[0];
	D3DXVECTOR2 prevColidPos = colids[0].GetPrevPos();

	// 衝突判定
	bool normalFriction = colids[0].IsColid(targetColid);

	// どこにも衝突していないなら処理をしない
	if (!normalFriction)return;

	D3DXVECTOR2 pos = colids[0].GetPos();
	D3DXVECTOR2 size = colids[0].GetSize();

	D3DXVECTOR2 targetPos = targetColid.GetPos();
	D3DXVECTOR2 targetSize = targetColid.GetSize();

	float moveDir = pos.x - prevColidPos.x;
	if (moveDir > 0) moveDir = 1.0f;
	else moveDir = -1.0f;

	// ==============
	// 衝突辺の確認
	// ==============
	float boxTop = pos.y - size.y / 2;
	float boxBottom = pos.y + size.y / 2;
	float boxLeft = pos.x - size.x / 2;
	float boxRight = pos.x + size.x / 2;

	float boxPrevTop = prevColidPos.y - size.y / 2;
	float boxPrevBottom = prevColidPos.y + size.y / 2;
	float boxPrevLeft = prevColidPos.x - size.x / 2;
	float boxPrevRight = prevColidPos.x + size.x / 2;

	float targetTop = targetPos.y - targetSize.y / 2;
	float targetBottom = targetPos.y + targetSize.y / 2;
	float targetLeft = targetPos.x - targetSize.x / 2;
	float targetRight = targetPos.x + targetSize.x / 2;

	// 辺の判定
	if (boxPrevBottom <= targetTop && boxTop <= targetTop && boxBottom >= targetTop)
	{	// ブロックの上に衝突
		trans->SetPos(pos.x, targetTop - size.y / 2 - 1.0f);
	}
	else if (boxPrevRight <= targetLeft && boxRight >= targetLeft && boxLeft <= targetLeft)
	{	// ブロックの左に衝突
		D3DXVECTOR2 move = pushed->Push(D3DXVECTOR2(4.0f * moveDir, 0.0f));
		trans->SetPos(targetLeft + move.x - size.x / 2 - 3.0f, pos.y + move.y);
	}
	else if (boxPrevLeft >= targetRight && boxLeft <= targetRight && boxRight >= targetRight)
	{	// ブロックの右に衝突
		D3DXVECTOR2 move = pushed->Push(D3DXVECTOR2(4.0f * moveDir, 0.0f));
		trans->SetPos(targetRight + move.x + size.x / 2, pos.y + move.y);
	}
}



