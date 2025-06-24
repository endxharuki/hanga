#include "RockIdle.h"
#include "sprite.h"
#include "texture.h"
#include "Camera.h"
#include "OnGameData.h"
#include "MainInGame.h"
#include "Break.h"

void RockIdle::SetUp()
{
	CleanRequest();

	prevHasSwap = OnGameData::GetInstance()->HasSwap();

	tex[0] = LoadTexture((char*)"data/TEXTURE/Rock1.png");
	tex[1] = LoadTexture((char*)"data/TEXTURE/Rock2.png");
	drawMode = OnGameData::GetInstance()->HasSwap()? 1: 0;


	obj->GetTransform()->SetSize(100, 100);
	obj->GetTransform()->SetGravity(0.0f);
	obj->GetTransform()->SetRotation(0.0f);
	obj->GetTransform()->SetRotationVel(0.0f);

	pool = obj->GetComponent<ColiderPool>();
}

void RockIdle::CleanUp()
{
}

void RockIdle::OnUpdate()
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

	D3DXVECTOR2 pos = transform->GetPos();
	D3DXVECTOR2 cam = Camera::GetInstance()->GetOriginPos();
	D3DXVECTOR2 camSize = Camera::GetInstance()->GetSize();
	// 現在のカメラ枠から指定の値離れていた場合消す
	if (pos.x < cam.x - eraseDistance || cam.x + camSize.x + eraseDistance < pos.x)
	{	// 削除処理
		if (MainInGame::objectPool.DeleteRequest(obj))
		{
			*enemRock = nullptr;
			return;
		}
	}

	transform->SetPos(pos.x, pos.y + 15.0f);

	// 衝突判定
	ColidBlock();
	ColidObject();
	ColidPlayer();

	pool->GetColider()[0].SetPrevPos(transform->GetPos());

	rot += transform->GetVel().x;

	// 削除処理
	if (deleteRequest)
	{
		MainInGame::objectPool.DeleteRequest(obj);

		*enemRock = nullptr;
		return; 
	}
}

void RockIdle::OnDraw()
{
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	DrawSpriteColorRotate(tex[drawMode],
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, rot * 0.01f);

	//pool->GetColider()[0].viewColid(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f));
}

void RockIdle::ColidBlock()
{
	Transform2D* transform = obj->GetTransform();

	Colider2D colid = pool->GetColider()[0];
	std::list<GameObject*>& map = BlockMap::GetInstance()->GetMap();

	D3DXVECTOR2 pos = colid.GetPos();
	D3DXVECTOR2 prevPos = colid.GetPrevPos();
	D3DXVECTOR2 size = colid.GetSize();

	D3DXVECTOR2 targetPos;
	D3DXVECTOR2 targetSize;

	bool isFriction = false;

	GameObject* frictionObject = nullptr;
	Break* targetBreak = nullptr;

	for (GameObject* object : map)
	{
		ColiderPool* targetPool = object->GetComponent<ColiderPool>();
		if (targetPool == nullptr)continue;

		Colider2D target = targetPool->GetColider()[0];

		D3DXVECTOR2 blockPos = target.GetPos();
		D3DXVECTOR2 blockSize = target.GetSize();

		D3DXVECTOR2 distance = D3DXVECTOR2(fabsf(pos.x - blockPos.x), fabsf(pos.y - blockPos.y));
		D3DXVECTOR2 sumSize = D3DXVECTOR2((size.x + blockSize.x) / 2.0f, (size.y + blockSize.y) / 2.0f);

		if (distance.x < sumSize.x &&
			distance.y < sumSize.y)
		{	// 衝突時
			frictionObject = object;
			targetPos = blockPos;
			targetSize = blockSize;
			isFriction = true;
			break;
		}
	}

	if (!isFriction) return;


	// ==============
	// 衝突辺の確認
	// ==============
	float top = pos.y - size.y / 2;
	float bottom = pos.y + size.y / 2;
	float left = pos.x - size.x / 2;
	float right = pos.x + size.x / 2;

	float prevTop = prevPos.y - size.y / 2;
	float prevBottom = prevPos.y + size.y / 2;
	float prevLeft = prevPos.x - size.x / 2;
	float prevRight = prevPos.x + size.x / 2;

	float blockTop = targetPos.y - targetSize.y / 2;
	float blockBottom = targetPos.y + targetSize.y / 2;
	float blockLeft = targetPos.x - targetSize.x / 2;
	float blockRight = targetPos.x + targetSize.x / 2;

	// 辺の判定
	if (prevBottom <= blockTop && top <= blockTop && bottom >= blockTop)
	{	// ブロックの上に衝突
		transform->SetPos(pos.x, blockTop - transform->GetSize().y / 2);
	}
	else if (prevTop >= blockBottom && top <= blockBottom && bottom >= blockBottom)
	{	// ブロックの下に衝突
		transform->SetPos(pos.x, blockBottom + transform->GetSize().y / 2);
	}
	else if (prevRight <= blockLeft && right >= blockLeft && left <= blockLeft)
	{	// ブロックの左に衝突
		// 削除処理
		if (MainInGame::objectPool.DeleteRequest(obj))
		{
			deleteRequest = true;
		}

		targetBreak = frictionObject->GetComponent<Break>();
		if (targetBreak != nullptr) targetBreak->BreakBlock();
	}
	else if (prevLeft >= blockRight && left <= blockRight && right >= blockRight)
	{	// ブロックの右に衝突
		// 削除処理
		if (MainInGame::objectPool.DeleteRequest(obj))
		{
			deleteRequest = true;
		}

		targetBreak =  frictionObject->GetComponent<Break>();
		if (targetBreak != nullptr) targetBreak->BreakBlock();
	}

	if (targetBreak != nullptr)
	{
		targetBreak->BreakBlock();
	}
}

void RockIdle::ColidObject()
{
	// オブジェクトとの衝突判定
	std::list<GameObject*>* pool = MainInGame::objectPool.GetPool();

	Colider2D colid = obj->GetComponent<ColiderPool>()->GetColider()[0];

	for (GameObject* target : *pool)
	{
		if (obj == target) continue;

		ColiderPool* targetColidPool = target->GetComponent<ColiderPool>();
		if (targetColidPool == nullptr) continue;

		if (colid.IsColid(targetColidPool->GetColider()[0]))
		{
			deleteRequest = true;
			break;
		}
	}
}

void RockIdle::ColidPlayer()
{
	Colider2D colid = obj->GetComponent<ColiderPool>()->GetColider()[0];
	Colider2D playerColid = MainInGame::player->GetComponent<ColiderPool>()->GetColider()[0];

	if (colid.IsColid(playerColid))
	{
		MainInGame::player->GetComponent<Damaged>()->Damage(1);
		deleteRequest = true;
	}
}
