#include "OneEyeEnemyIdle.h"
#include "MainInGame.h"
#include "sprite.h"
#include "texture.h"
#include "Camera.h"
#include "OnGameData.h"
#include "sound.h"
#include "Enemmy.h"

void OneEyeEnemyIdle::SetUp()
{

	CleanRequest();

	tex[0] = LoadTexture((char*)"data/TEXTURE/hitotume_mini.png");
	tex[1] = LoadTexture((char*)"data/TEXTURE/hitotume_mini_R.png");
	drawMode = 0;

	startPosY = obj->GetTransform()->GetPos().y;

	prevSwap = OnGameData::GetInstance()->HasSwap();
	colidPool = obj->GetComponent<ColiderPool>();

	obj->GetTransform()->SetVel(3.0f, 3.0f);

	obj->GetTransform()->SetGravity(5.0f);
	obj->GetTransform()->SetRotation(0.0f);
	obj->GetTransform()->SetRotationVel(0.0f);
	obj->GetTransform()->SetSize(obj->GetTransform()->GetSize().x * 0.5f, obj->GetTransform()->GetSize().y);

	colidPool = obj->GetComponent<ColiderPool>();

	// 左下判定
	colidPool->Add(Colider2D(
		obj->GetTransform(),
		D3DXVECTOR2(-obj->GetTransform()->GetSize().x *0.5f, obj->GetTransform()->GetSize().y/2),
		D3DXVECTOR2(0.0f, obj->GetTransform()->GetSize().y - 10.0f)
	));

	// 右下判定
	colidPool->Add(Colider2D(
		obj->GetTransform(),
		D3DXVECTOR2(obj->GetTransform()->GetSize().x *0.5f, obj->GetTransform()->GetSize().y/2),
		D3DXVECTOR2(0.0f, obj->GetTransform()->GetSize().y - 10.0f)
	));

	deathSE = LoadSound((char*)"data/SE/oneeye_death.wav");

}

void OneEyeEnemyIdle::CleanUp()
{
	write->Release();
	delete write;

}

void OneEyeEnemyIdle::OnUpdate()
{
	Transform2D* transform = obj->GetTransform();
	
	D3DXVECTOR2 enemPos = transform->GetPos();
	D3DXVECTOR2 playerPos = MainInGame::player->GetTransform()->GetPos();

	if (fabsf(playerPos.x - enemPos.x) > 3000.0f)
	{	// 3000よりも距離が離れている場合処理を行わない
		return;
	}

	if (prevSwap != OnGameData::GetInstance()->HasSwap())
	{
		drawMode = (drawMode + 1) % 2;
		prevSwap = !prevSwap;
	}

	if (deleteRequest) 
	{
		MainInGame::objectPool.DeleteRequest(obj);
	}
	//落ちたら死ぬ
	if (obj->GetTransform()->GetPos().y - startPosY >= 2000.0f) deleteRequest = true;

	// 重力
	if (isGround == false)
	{
		obj->GetTransform()->SetPos(obj->GetTransform()->GetPos().x, obj->GetTransform()->GetPos().y + 10.0f);
	}


	if (!isDie)
	{
		//trueなら右に動く
		if (moveSide == true && hitCount > 0) obj->GetTransform()->SetPos(obj->GetTransform()->GetPos().x + 3.0f, obj->GetTransform()->GetPos().y);
		//falseなら左に動く
		if (moveSide == false && hitCount > 0) obj->GetTransform()->SetPos(obj->GetTransform()->GetPos().x - 3.0f, obj->GetTransform()->GetPos().y);
	
		colidPool->GetColider()[0].SetPrevPos(obj->GetTransform()->GetPos());
		
		isGround = false;
		hitCount++;

		wUV.x = ((walkTexCount/4) % ONEEYE_TEX_X) * (1.0f / ONEEYE_TEX_X);
		wUV.y = ((walkTexCount/4) / ONEEYE_TEX_X) * (1.0f / ONEEYE_TEX_Y);
		wUV.z = 1.0f / ONEEYE_TEX_X;
		wUV.w = 1.0f / ONEEYE_TEX_Y;

		walkTexCount++;

		
		ColidBlock();
		ColidBox();
		ColidPlayer();
	}
	DeathRendition();

	prevColidPos = obj->GetTransform()->GetPos();

	
}

void OneEyeEnemyIdle::OnDraw()
{
	//if (isDie)return;
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	float backAdd = 0.0f;
	int revarse = 1.0f;
	if (moveSide)
	{
		backAdd = 1.0f;
		revarse = -1;
	}
	if (drawMode == 1)revarse *= -1;
	DrawSpriteColorRotate(tex[drawMode],
		drawPos.x, drawPos.y,
		obj->GetTransform()->GetSize().x*2, obj->GetTransform()->GetSize().y,
		wUV.x + (wUV.z * backAdd), wUV.y,
		wUV.z * revarse, wUV.w,
		1.0f, 1.0f, 1.0f, 1.0f,dieRot*2.0f);

	Player* player = MainInGame::player;
	float P_B = player->GetTransform()->GetPos().y + (player->GetTransform()->GetSize().y *0.5f);
}

void OneEyeEnemyIdle::ColidBlock()
{
	//　初期化
	Transform2D* trans = obj->GetTransform();

	Colider2D enemyColid = obj->GetComponent<ColiderPool>()->GetColider()[0];
	std::vector<Colider2D> colids = colidPool->GetColider();
	D3DXVECTOR2 pos = trans->GetPos();
	D3DXVECTOR2 size = trans->GetSize();

	std::list<GameObject*>& map = BlockMap::GetInstance()->GetSortedMap(pos, size + D3DXVECTOR2(10.0f, 0.0f));

	bool damage = false;
	bool turnleft = true;
	bool turnright = true;


	// ==============
	// 衝突判定
	// ==============
	for (GameObject* object : map)
	{
		std::vector<Colider2D> target = object->GetComponent<ColiderPool>()->GetColider();
		damage = false;

		for (int j = 0; j < target.size(); j++) 
		{

			Colider2D colid = target[j];

			if (colid.GetTag() == "NOT")continue;
			if (colid.GetTag() == "Damage") damage = true;
			ColiderPool* targetColiderPool = object->GetComponent<ColiderPool>();
			Colider2D targetColid = targetColiderPool->GetColider()[0];

			if (colidPool->GetColider()[1].IsColid(targetColid))//左下
			{
				turnright = false;//一回でも当たったら変えない
			}
			if (colidPool->GetColider()[2].IsColid(targetColid))//右下
			{
				turnleft = false;//一回でも当たったら変えない
			}

			if (enemyColid.IsColid(targetColid)) {

				Transform2D* tTrans = object->GetTransform();

				D3DXVECTOR2 blockPos = tTrans->GetPos();
				D3DXVECTOR2 blockSize = tTrans->GetSize();

				float enemyTop = pos.y - size.y * 0.5f;
				float enemyBottom = pos.y + size.y * 0.5f;
				float enemyLeft = pos.x - size.x * 0.5f;
				float enemyRight = pos.x + size.x * 0.5f;

				float enemyPrevTop = prevColidPos.y - size.y * 0.5f;
				float enemyPrevBottom = prevColidPos.y + size.y * 0.5f;
				float enemyPrevLeft = prevColidPos.x - size.x * 0.5f;
				float enemyPrevRight = prevColidPos.x + size.x * 0.5f;

				float blockTop = blockPos.y - blockSize.y * 0.5f;
				float blockBottom = blockPos.y + blockSize.y * 0.5f;
				float blockLeft = blockPos.x - blockSize.x * 0.5f;
				float blockRight = blockPos.x + blockSize.x * 0.5f;

				if (enemyPrevRight <= blockLeft && enemyRight >= blockLeft && enemyLeft <= blockLeft)
				{	// ブロックの左に衝突
					trans->SetPos(blockLeft - size.x * 0.5f, pos.y);
					trans->SetPos(pos.x, blockPos.y);
					moveSide = false;
					hitCount = 0;
				}
				else if (enemyPrevLeft >= blockRight && enemyLeft <= blockRight && enemyRight >= blockRight)
				{	// ブロックの右に衝突
					trans->SetPos(blockRight + size.x * 0.5f, pos.y);
					trans->SetPos(pos.x, blockPos.y);
					moveSide = true;
					hitCount = 0;
				}
				else if (enemyPrevBottom <= blockTop && enemyTop <= blockTop && enemyBottom >= blockTop)
				{	// ブロックの上に衝突

					trans->SetPos(pos.x, blockTop - size.y *0.5f);
					isGround = true;
				}
				else if (enemyPrevTop >= blockBottom && enemyTop <= blockBottom && enemyBottom >= blockBottom)
				{	// ブロックの下に衝突
					trans->SetPos(pos.x, blockBottom + size.y *0.5f);
				}
				

				if (damage) deleteRequest = true;
				/*break;*/

			}
			
		}
		
	} // 衝突判定終了

	if (turnleft && turnright);
	else if (turnleft && hitCount > 1)
	{
		moveSide = false;
	}
	else if (turnright && hitCount > 1)
	{
		moveSide = true;
	}

}

void OneEyeEnemyIdle::ColidPlayer()
{

	Player* player = MainInGame::player;
	float PlayerBottom = player->GetTransform()->GetPos().y + (player->GetTransform()->GetSize().y *0.5f);

	Colider2D colid = obj->GetComponent<ColiderPool>()->GetColider()[0];
	Colider2D playerColid = MainInGame::player->GetComponent<ColiderPool>()->GetColider()[0];
	
	if (colid.IsColid(playerColid))
	{
		//横からあたるとダメージを食らう
		if (PlayerBottom > obj->GetTransform()->GetPos().y)
		{

			MainInGame::player->GetComponent<Damaged>()->Damage(1);
		}
		else if (PlayerBottom <= obj->GetTransform()->GetPos().y)
		{	//上から踏むと倒せる
			PlaySound(deathSE, 0);
			MainInGame::player->GetTransform()->SetVel(MainInGame::player->GetTransform()->GetVel().x, 0.0f);
			MainInGame::player->GetTransform()->SetOutVel(MainInGame::player->GetTransform()->GetOutVel().x, MainInGame::player->GetTransform()->GetOutVel().y - 20.0f);
			//deleteRequest = true;
			isDie = true;
			OnGameData::GetInstance()->AddScore(200);
			OnGameData::GetInstance()->AddHitEnemyNum();
			obj->GetComponent<ColiderPool>()->GetColider()[0].SetTag("Death");
		}
	}

}

void OneEyeEnemyIdle::ColidBox()
{

	// オブジェクトとの衝突判定
	std::list<GameObject*>* pool = MainInGame::objectPool.GetPool();

	//エネミー
	Transform2D* trans = obj->GetTransform();
	Colider2D colid = obj->GetComponent<ColiderPool>()->GetColider()[0];

	//オブジェクト
	for (GameObject* target : *pool)
	{
		if (obj == target) continue;
		if (target->GetComponent<Enemmy>() != nullptr) continue;

		
		ColiderPool* targetColidPool = target->GetComponent<ColiderPool>();
		if (targetColidPool == nullptr) continue;

		if (colid.IsColid(targetColidPool->GetColider()[0]))
		{
			
			Transform2D* tTrans = target->GetTransform();

			D3DXVECTOR2 pos = colid.GetPos();
			D3DXVECTOR2 size = colid.GetSize();

			D3DXVECTOR2 boxPos = tTrans->GetPos();
			D3DXVECTOR2 boxSize = tTrans->GetSize();

			float enemyTop = pos.y - size.y *0.5f;
			float enemyBottom = pos.y + size.y *0.5f;
			float enemyLeft = pos.x - size.x *0.5f;
			float enemyRight = pos.x + size.x *0.5f;

			float enemyPrevTop = prevColidPos.y - size.y *0.5f;
			float enemyPrevBottom = prevColidPos.y + size.y *0.5f;
			float enemyPrevLeft = prevColidPos.x - size.x *0.5f;
			float enemyPrevRight = prevColidPos.x + size.x *0.5f;

			float boxTop = boxPos.y - boxSize.y *0.5f;
			float boxBottom = boxPos.y + boxSize.y *0.5f;
			float boxLeft = boxPos.x - boxSize.x *0.5f;
			float boxRight = boxPos.x + boxSize.x *0.5f;

			if (colid.IsDirColid(targetColidPool->GetColider()[0]) == DOWN)
			{	// ブロックの上に衝突
				
				trans->SetPos(pos.x, boxTop - trans->GetSize().y *0.5f);
				isGround = true;
			}
			else if (colid.IsDirColid(targetColidPool->GetColider()[0]) == RIGHT)
			{	// ブロックの左に衝突
				trans->SetPos(boxLeft - trans->GetSize().x * 0.5f, pos.y);
				moveSide = false;
				hitCount = 0;
			}
			else if (colid.IsDirColid(targetColidPool->GetColider()[0]) == LEFT)
			{	// ブロックの右に衝突
				trans->SetPos(boxRight + trans->GetSize().x * 0.5f, pos.y);
				moveSide = true;
				hitCount = 0;
			}
			
		}
		
	}

}

void OneEyeEnemyIdle::DeathRendition()
{
	if(!isDie)return;
	dieRot += 3.14f/180*6;
	int revarse = 1;
	if (!moveSide)revarse = -1;
	if (dieRot < 3.14f)
	{
		obj->GetTransform()->SetPos(prevColidPos.x + cos(dieRot+1.57f)*4.0f* revarse, prevColidPos.y + sin(dieRot+4.71f) * 10.0f);
	}
	else
	{
		obj->GetTransform()->SetPos(prevColidPos.x - 2.0f* revarse, prevColidPos.y+dieRot*3.5f);
	}
}
