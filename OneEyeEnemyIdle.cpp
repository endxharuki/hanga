#include "OneEyeEnemyIdle.h"
#include "MainInGame.h"
#include "sprite.h"
#include "texture.h"
#include "Camera.h"
#include "OnGameData.h"

void OneEyeEnemyIdle::SetUp()
{

	CleanRequest();

	tex[0] = LoadTexture((char*)"data/TEXTURE/Enemmy1.png");
	tex[1] = LoadTexture((char*)"data/TEXTURE/Enemmy2.png");
	drawMode = 0;

	prevSwap = OnGameData::GetInstance()->HasSwap();
	colidPool = obj->GetComponent<ColiderPool>();

	obj->GetTransform()->SetVel(3.0f, 3.0f);

	obj->GetTransform()->SetGravity(5.0f);
	obj->GetTransform()->SetRotation(0.0f);
	obj->GetTransform()->SetRotationVel(0.0f);

	colidPool = obj->GetComponent<ColiderPool>();

	// フォント設定
	debugData = new FontData();
	debugData->fontSize = 30;
	debugData->fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
	debugData->Color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);

	write = new DirectWrite(debugData);
	write->Init();

}

void OneEyeEnemyIdle::CleanUp()
{

	delete debugData;

	write->Release();
	delete write;

}

void OneEyeEnemyIdle::OnUpdate()
{
	Transform2D* transform = obj->GetTransform();

	if (prevSwap != OnGameData::GetInstance()->HasSwap())
	{
		drawMode = (drawMode + 1) % 2;
		prevSwap = !prevSwap;
	}

	if (deleteRequest) {

		MainInGame::objectPool.DeleteRequest(obj);
	}
	//落ちたら死ぬ
	if (obj->GetTransform()->GetPos().y >= 1100.0f) deleteRequest = true;

	// 重力
	if(isGround == false)obj->GetTransform()->SetPos(obj->GetTransform()->GetPos().x, obj->GetTransform()->GetPos().y + 10.0f);

	//trueなら右に動く
	if (moveSide == true) obj->GetTransform()->SetPos(obj->GetTransform()->GetPos().x + 3.0f, obj->GetTransform()->GetPos().y);
	//falseなら左に動く
	if (moveSide == false) obj->GetTransform()->SetPos(obj->GetTransform()->GetPos().x - 3.0f, obj->GetTransform()->GetPos().y);

	colidPool->GetColider()[0].SetPrevPos(obj->GetTransform()->GetPos());
	
	isGround = false;

	ColidPlayer();
	ColidBlock();
	ColidBox();

	prevColidPos = colidPool->GetColider()[0].GetPos();

	
}

void OneEyeEnemyIdle::OnDraw()
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
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f);

	Player* player = MainInGame::player;
	float P_B = player->GetTransform()->GetPos().y + (player->GetTransform()->GetSize().y / 2);

	////データを描画
	//write->SetFont(debugData);

	//// デバッグ
	//int E_y = pos.x;
	//std::string deb = "P: " + std::to_string(P_B);
	//write->DrawString("P: " + std::to_string(P_B), D3DXVECTOR2(1000, 50), D2D1_DRAW_TEXT_OPTIONS_NONE);
	//int y = pos.y;
	//write->DrawString("E: " + std::to_string(y), D3DXVECTOR2(1000, 75), D2D1_DRAW_TEXT_OPTIONS_NONE);

	//float x = obj->GetTransform()->GetVel().x;
	//write->DrawString("E_X: " + std::to_string(x), D3DXVECTOR2(1000, 100), D2D1_DRAW_TEXT_OPTIONS_NONE);
}

void OneEyeEnemyIdle::ColidBlock()
{
	
    //別バージョン
	//　初期化
	Transform2D* trans = obj->GetTransform();

	Colider2D enemyColid = obj->GetComponent<ColiderPool>()->GetColider()[0];
	std::vector<Colider2D> colids = colidPool->GetColider();

	std::list<GameObject*>& map = BlockMap::GetInstance()->GetMap();
	std::list<GameObject*>& map2 = BlockMap::GetInstance()->GetMap2();
	std::list<GameObject*> checkMap;
	checkMap = map;

	// レイヤー2の追加
	if (OnGameData::GetInstance()->GetCurrentLayer() >= 1)
	{
		for (GameObject* object : map2)
		{
			checkMap.push_back(object);
		}
	}
	bool damage = false;

	// ==============
	// 衝突判定
	// ==============
	for (GameObject* object : checkMap)
	{
		std::vector<Colider2D> target = object->GetComponent<ColiderPool>()->GetColider();
		damage = false;

		for (int j = 0; j < target.size(); j++) {

			Colider2D colid = target[j];

			if (colid.GetTag() == "NOT")continue;
			if (colid.GetTag() == "Damage") damage = true;
			ColiderPool* targetColider = object->GetComponent<ColiderPool>();

			if (enemyColid.IsColid(targetColider->GetColider()[0])) {

				Transform2D* tTrans = object->GetTransform();

				D3DXVECTOR2 pos = colids[0].GetPos();
				D3DXVECTOR2 size = colids[0].GetSize();

				D3DXVECTOR2 blockPos = tTrans->GetPos();
				D3DXVECTOR2 blockSize = tTrans->GetSize();

				float enemyTop = pos.y - size.y / 2;
				float enemyBottom = pos.y + size.y / 2;
				float enemyLeft = pos.x - size.x / 2;
				float enemyRight = pos.x + size.x / 2;

				float enemyPrevTop = prevColidPos.y - size.y / 2;
				float enemyPrevBottom = prevColidPos.y + size.y / 2;
				float enemyPrevLeft = prevColidPos.x - size.x / 2;
				float enemyPrevRight = prevColidPos.x + size.x / 2;

				float blockTop = blockPos.y - blockSize.y / 2;
				float blockBottom = blockPos.y + blockSize.y / 2;
				float blockLeft = blockPos.x - blockSize.x / 2;
				float blockRight = blockPos.x + blockSize.x / 2;

				if (enemyPrevBottom <= blockTop && enemyTop <= blockTop && enemyBottom >= blockTop)
				{	// ブロックの上に衝突

					trans->SetPos(pos.x, blockTop - trans->GetSize().y / 2);
					isGround = true;
				}
				else if (enemyPrevTop >= blockBottom && enemyTop <= blockBottom && enemyBottom >= blockBottom)
				{	// ブロックの下に衝突
					trans->SetPos(pos.x, blockBottom + trans->GetSize().y / 2);

				}
				else if (enemyPrevRight <= blockLeft && enemyRight >= blockLeft && enemyLeft <= blockLeft)
				{	// ブロックの左に衝突

					moveSide = false;
				}
				else if (enemyPrevLeft >= blockRight && enemyLeft <= blockRight && enemyRight >= blockRight)
				{	// ブロックの右に衝突
					/*trans->SetPos(blockRight + size.x, pos.y);*/
					moveSide = true;
				}

				if (damage) deleteRequest = true;
				/*break;*/

			}
		}
		
	} // 衝突判定終了

	

}

void OneEyeEnemyIdle::ColidPlayer()
{

	Player* player = MainInGame::player;
	float PlayerBottom = player->GetTransform()->GetPos().y + (player->GetTransform()->GetSize().y / 2);

	Colider2D colid = obj->GetComponent<ColiderPool>()->GetColider()[0];
	Colider2D playerColid = MainInGame::player->GetComponent<ColiderPool>()->GetColider()[0];
	
	if (colid.IsColid(playerColid))
	{
		//横からあたるとダメージを食らう
		if (PlayerBottom > obj->GetTransform()->GetPos().y) {

			MainInGame::player->GetComponent<Damaged>()->Damage(1);
		}
		//上から踏むと倒せる
		else if (PlayerBottom <= obj->GetTransform()->GetPos().y) {

			deleteRequest = true;
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

			float enemyTop = pos.y - size.y / 2;
			float enemyBottom = pos.y + size.y / 2;
			float enemyLeft = pos.x - size.x / 2;
			float enemyRight = pos.x + size.x / 2;

			float enemyPrevTop = prevColidPos.y - size.y / 2;
			float enemyPrevBottom = prevColidPos.y + size.y / 2;
			float enemyPrevLeft = prevColidPos.x - size.x / 2;
			float enemyPrevRight = prevColidPos.x + size.x / 2;

			float boxTop = boxPos.y - boxSize.y / 2;
			float boxBottom = boxPos.y + boxSize.y / 2;
			float boxLeft = boxPos.x - boxSize.x / 2;
			float boxRight = boxPos.x + boxSize.x / 2;

			if (enemyPrevBottom <= boxTop && enemyTop <= boxTop && enemyBottom >= boxTop)
			{	// ブロックの上に衝突
				
				trans->SetPos(pos.x, boxTop - trans->GetSize().y / 2);
				isGround = true;
			}
			else if (enemyPrevRight <= boxLeft && enemyRight >= boxLeft && enemyLeft <= boxLeft)
			{	// ブロックの左に衝突
				/*trans->SetPos(boxLeft - size.x, pos.y);*/
				moveSide = false;
			}
			else if (enemyPrevLeft >= boxRight && enemyLeft <= boxRight && enemyRight >= boxRight)
			{	// ブロックの右に衝突
				/*trans->SetPos(boxLeft + size.x, pos.y);*/
				moveSide = true;
			}
			
		}
	}

}
