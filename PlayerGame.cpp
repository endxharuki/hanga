#include "PlayerGame.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "XInput.h"
#include "OnGameData.h"
#include "MainInGame.h"
#include "math.h"
#include "BlockMap.h"
#include "Camera.h"
#include "ObjectPool.h"
#include "Pushed.h"
#include "Input.h"


#define PLAYER_TEX_X (5)
#define PLAYER_TEX_Y (6)
#define PLAYER_PUSH (4.0f)

#define PI (3.1415f)

// グローバル変数
OnGameData*		g_GameData = OnGameData::GetInstance();

static int outSound;

float GetSign(float num)
{
	if (num > 0) return 1;
	else if (num < 0) return -1;
	else return 0;
}

float Cross(const D3DXVECTOR2& v1, const D3DXVECTOR2& v2)
{
	return v1.x * v2.y - v2.x * v1.y;
}

void PlayerGame::SetUp()
{
	CleanRequest();

	// テクスチャ読込
	idleTex[0] = LoadTexture((char*)"data/TEXTURE/PIdle1.png");
	idleTex[1] = LoadTexture((char*)"data/TEXTURE/PIdle2.png");
	runTex[0] = LoadTexture((char*)"data/TEXTURE/PWalk1.png");
	runTex[1] = LoadTexture((char*)"data/TEXTURE/PWalk2.png");


	obj->GetTransform()->SetSize(100 , 100);
	obj->GetTransform()->SetPos(200.0f, 800.0f);
	obj->GetTransform()->SetGravity(9.0f);
	obj->GetTransform()->SetRotation(0.0f);
	obj->GetTransform()->SetRotationVel(0.0f);

	colidPool = obj->GetComponent<ColiderPool>();
	damage = obj->GetComponent<Damaged>();
	prevDamage = false;


	prevHasSwap = g_GameData->HasSwap();
	isGround = false;
	isRun = false;
	isJump = false;
	pushJumpKey = false;
	pushGrabKey = false;
	drawPlayer = true;
	moveDir = 1.0f;

	isDie = false;

	currentJumpFrame = 0;

	// フォント設定
	debugData = new FontData();
	debugData->fontSize = 30;
	debugData->fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
	debugData->Color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);

	write = new DirectWrite(debugData);
	write->Init();
}

void PlayerGame::CleanUp()
{
	delete debugData;

	write->Release();
	delete write;
}

void PlayerGame::OnUpdate()
{
	if (isDie) return;

	D3DXVECTOR2 moveSpeed = D3DXVECTOR2(0.0f, 0.0f);
	Transform2D* transform = obj->GetTransform();
	transform->SetVel(transform->GetVel().x, transform->GetVel().y + transform->GetGravity());


	// ================
	// 各種判定
	// ================
	// 反転した際の処理
	if (prevHasSwap != g_GameData->HasSwap())
	{
		prevHasSwap = !prevHasSwap;
		drawMode = (drawMode + 1) % 2;

		// 二連続で反転したときに移動が反転したままにならないように変数自体の否定を返す
		requestInputSwap = !requestInputSwap;	

		transform->SetVel(-transform->GetVel().x, transform->GetVel().y);
		transform->SetOutVel(-transform->GetOutVel().x, transform->GetOutVel().y);
	}

	// 反転した後、移動入力がない場合に移動の値も反転させる
	if (requestInputSwap && prevInput == false && isGround)
	{
		moveSign *= -1.0f;
		requestInputSwap = false;
	}
	else if (requestInputSwap && prevInput == true && !isGround)
	{
		moveSign *= -1.0f;
		requestInputSwap = false;
	}

	// プレイヤー主体の速度の大きさが1.0以上なら動いているとみなす
	if (fabsf(transform->GetVel().x) >= 1.0f)
	{
		isRun = true;
	}
	else
	{
		isRun = false;
	}


	// 走りアニメーション処理
	if (isRun)
	{
		rU = (currentRunTex % PLAYER_TEX_X) * (1.0f / PLAYER_TEX_X);
		rV = (currentRunTex / PLAYER_TEX_X) * (1.0f / PLAYER_TEX_Y);
		rUW = 1.0f / PLAYER_TEX_X;
		rVH = 1.0f / PLAYER_TEX_Y;

		currentRunTex++;
	}
	else
	{
		currentRunTex = 0;
	}

	// ダメージ用
	if (prevDamage == false && damage->IsDamage() == true)
	{	// ダメージを受けた瞬間

		std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
		firstSec = std::chrono::duration_cast<std::chrono::milliseconds>(p.time_since_epoch());
		prevFlash = firstSec;

		prevDamage = true;
		drawPlayer = false;
		if (OnGameData::GetInstance()->GetPlayerLife() <= 0)
		{
			isDie = true;
			OnGameData::GetInstance()->SetGameOver(true);
		}
	}
	if (isDie) return;

	if (damage->IsDamage())
	{
		std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
		std::chrono::milliseconds nowSec = std::chrono::duration_cast<std::chrono::milliseconds>(p.time_since_epoch());

		if (nowSec.count() - firstSec.count() > prevFlash.count() - firstSec.count() + 100)
		{
			drawPlayer = !drawPlayer;
			prevFlash = nowSec;
		}

		if (nowSec.count() - firstSec.count() > 1500)
		{
			drawPlayer = true;
			damage->SetDamage(false);
			prevDamage = false;
		}
	}

	// ================
	// 入力処理
	// ================
	prevInput = false;

	// レイヤー操作
	if (GetKeyboardPress(DIK_LEFT) || IsSwitchTriggerd(0, BUTTON_DOWN))
	{
		if (g_GameData->GetCurrentLayer() - 1 >= 0) g_GameData->SubLayer();
	}
	if (GetKeyboardPress(DIK_RIGHT) || IsSwitchTriggerd(0, BUTTON_UP))
	{
		if (g_GameData->GetCurrentLayer() + 1 <= 1) g_GameData->AddLayer();
	}

	// 移動処理
	if (GetKeyboardPress(DIK_A) || GetLeftStick(0).x < 0.0f)
	{// 左
		if (!isGrab)
		{
			transform->SetVel(transform->GetVel().x - 5.0f * moveSign, transform->GetVel().y);
		}
		prevInput = true;
		moveDir = -1.0f * moveSign;
	}
	if (GetKeyboardPress(DIK_D) || GetLeftStick(0).x > 0.0f)
	{// 右
		if (!isGrab)
		{
			transform->SetVel(transform->GetVel().x + 5.0f * moveSign, transform->GetVel().y);
		}
		prevInput = true;
		moveDir = 1.0f * moveSign;
	}

	// 回転処理
	if (GetKeyboardPress(DIK_RIGHT))
		transform->SetRotation(transform->GetRotation() - 0.05f);
	if(GetKeyboardPress(DIK_LEFT))
		transform->SetRotation(transform->GetRotation() + 0.05f);

	// ジャンプ入力
	if (isGround && (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_X)))
	{
		pushJumpKey = true;
	}
	else if(isJump && (GetKeyboardPress(DIK_SPACE) || IsButtonPressed(0, BUTTON_X)))
	{
		pushJumpKey = true;
	}
	else
	{
		pushJumpKey = false;
	}

	if (GetKeyboardPress(DIK_UP) || IsButtonPressed(0, BUTTON_B))
	{
		pushGrabKey = true;
	}
	else
	{
		pushGrabKey = false;
	}
		

	// 版画
	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_R)) // 反転
		OnGameData::GetInstance()->ChangeSwap();


	// ジャンプ処理
	Jump();

	// ================
	// 速度処理
	// ================
	// 速度反映
	transform->SetPos(transform->GetPos().x + transform->GetVel().x + transform->GetOutVel().x, transform->GetPos().y + transform->GetVel().y + transform->GetOutVel().y);
	// 速度減速処理
	if (isGround)
	{
		transform->SetVel(transform->GetVel().x * 0.6f, transform->GetVel().y * 0.5f);
		transform->SetOutVel(transform->GetOutVel().x * 0.7f, transform->GetOutVel().y * 0.6f);
	}
	else
	{
		transform->SetVel(transform->GetVel().x * 0.6f, transform->GetVel().y * 0.5f);
		transform->SetOutVel(transform->GetOutVel().x * 0.98f, transform->GetOutVel().y * 0.94f);
	}
	// 回転反映
	colidPool->GetColider()[0].Rotate(transform->GetRotation());


	isGround = false;
	isGrab = false;

	// =======================
	// 衝突処理
	// =======================
	// yは枠外にでないようにする(一応)
	if (transform->GetPos().y + transform->GetSize().y / 2 > SCREEN_HEIGHT)
	{
		//transform->SetPos(transform->GetPos().x, SCREEN_HEIGHT - transform->GetSize().y / 2);
		obj->GetTransform()->SetPos(100.0f, 900.0f);
		if (g_GameData->HasSwap()) g_GameData->ChangeSwap();
		isGround = true;
	}
	ColidObject();
	ColidBlock();

	prevColidPos = colidPool->GetColider()[0].GetPos();

	Camera::GetInstance()->SetPos(transform->GetPos());
}

void PlayerGame::OnDraw()
{
	if (isDie) return;

	D3DXVECTOR2 pos =  obj->GetTransform()->GetPos();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	//colidPool->GetColider()[1].viewColid(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f));
	//colidPool->GetColider()[2].viewColid(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f));

	if (drawPlayer)
	{
		if (isRun)
		{
			DrawSpriteColor(runTex[drawMode],
				drawPos.x, drawPos.y,
				obj->GetTransform()->GetSize().x, obj->GetTransform()->GetSize().y,
				rU, rV,
				rUW * moveDir, rVH,
				1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			DrawSpriteColor(idleTex[drawMode],
				drawPos.x, drawPos.y,
				obj->GetTransform()->GetSize().x, obj->GetTransform()->GetSize().y,
				0.0f, 0.0f,
				1.0f * moveDir, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	write->SetFont(debugData);

	// デバッグ
	/*int x = pos.x;
	std::string deb = "X: " + std::to_string(x);
	write->DrawString("X: " + std::to_string(x), D3DXVECTOR2(1000, 50), D2D1_DRAW_TEXT_OPTIONS_NONE);
	int y = pos.y;
	write->DrawString("Y: " + std::to_string(y), D3DXVECTOR2(1000, 75), D2D1_DRAW_TEXT_OPTIONS_NONE);

	D3DXVECTOR2 rs = GetRightStick(0);
	D3DXVECTOR2 ls = GetLeftStick(0);

	write->DrawString("rs x:" + std::to_string(rs.x) + " y: " + std::to_string(rs.y), D3DXVECTOR2(1000, 120), D2D1_DRAW_TEXT_OPTIONS_NONE);

	write->DrawString("ls x:" + std::to_string(ls.x) + " y: " + std::to_string(ls.y), D3DXVECTOR2(1000, 150), D2D1_DRAW_TEXT_OPTIONS_NONE);

	float t = GetTriger(0);
	write->DrawString("t:" + std::to_string(t), D3DXVECTOR2(1000, 250), D2D1_DRAW_TEXT_OPTIONS_NONE);

	bool rt = IsRightTrigger(0);

	if(rt)write->DrawString("rt: true", D3DXVECTOR2(1000, 300), D2D1_DRAW_TEXT_OPTIONS_NONE);
	else write->DrawString("rt: false", D3DXVECTOR2(1000, 300), D2D1_DRAW_TEXT_OPTIONS_NONE);
	

	bool lt = IsLeftTrigger(0);

	if (lt)write->DrawString("lt: true", D3DXVECTOR2(1000, 350), D2D1_DRAW_TEXT_OPTIONS_NONE);
	else write->DrawString("lt: false", D3DXVECTOR2(1000, 350), D2D1_DRAW_TEXT_OPTIONS_NONE);*/
}

void PlayerGame::Jump()
{
	if (!pushJumpKey || currentJumpFrame >= jumpFrame)
	{
		isJump = false;
		currentJumpFrame = 0;
		return;
	}

	// キーを押した瞬間にジャンプの値を最大値に初期化
	if (currentJumpFrame == 0)
	{
		currentJumpHeight = maxJumpHeight;
		isJump = true;
	}

	Transform2D* transform = obj->GetTransform();

	// 重力分も強さを加算する(重力を無視する)
	float jumpStr = currentJumpHeight + transform->GetGravity();
	transform->SetVel(transform->GetVel().x, transform->GetVel().y - jumpStr);

	// ジャンプ時間が過ぎたらジャンプの値が0になる
	float ret = maxJumpHeight / jumpFrame;
	currentJumpHeight -= ret;

	currentJumpFrame++;
}

void PlayerGame::ColidBlock()
{
	//　初期化
	Transform2D* trans = obj->GetTransform();
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

	bool isFriction = false;
	bool damage = false;

	D3DXVECTOR2 frictionColidPos;
	D3DXVECTOR2	frictionColidSize;
	D3DXVECTOR2 frictionBlockPos;
	D3DXVECTOR2 frictionBlockSize;
	std::string targetTag = "";

	// ==============
	// ステージ衝突判定
	// ==============
	D3DXVECTOR2 pos = colids[0].GetPos();
	D3DXVECTOR2 size = colids[0].GetSize();


	// ==============
	// 衝突判定
	// ==============
	for (GameObject* object : checkMap)
	{
		std::vector<Colider2D> target = object->GetComponent<ColiderPool>()->GetColider();
		damage = false;

		for (int j = 0; j < target.size(); j++)
		{
			Colider2D colid = target[j];
			if (colid.GetTag() == "NOT")continue;
			if (colid.GetTag() == "Damage") damage = true;

			D3DXVECTOR2 BlockPos = colid.GetPos();
			D3DXVECTOR2 BlockSize = colid.GetSize();

			D3DXVECTOR2 distance = D3DXVECTOR2(fabsf(pos.x - BlockPos.x), fabsf(pos.y - BlockPos.y));
			D3DXVECTOR2 sumSize = D3DXVECTOR2((size.x + BlockSize.x) / 2.0f, (size.y + BlockSize.y) / 2.0f);

			if (distance.x < sumSize.x &&
				distance.y < sumSize.y)
			{	// 衝突時
				isFriction = true;
				frictionColidPos = pos;
				frictionColidSize = size;
				frictionBlockPos = BlockPos;
				frictionBlockSize = BlockSize;
				targetTag = colid.GetTag();
				break;
			}
		}
		if (isFriction)break;
	} // 衝突判定終了
	if (!isFriction)return;

	// ==============
	// 衝突辺の確認
	// ==============
	float playerTop = frictionColidPos.y - frictionColidSize.y / 2;
	float playerBottom = frictionColidPos.y + frictionColidSize.y / 2;
	float playerLeft = frictionColidPos.x - frictionColidSize.x / 2;
	float playerRight = frictionColidPos.x + frictionColidSize.x / 2;

	float playerPrevTop = prevColidPos.y - frictionColidSize.y / 2;
	float playerPrevBottom = prevColidPos.y + frictionColidSize.y / 2;
	float playerPrevLeft = prevColidPos.x - frictionColidSize.x / 2;
	float playerPrevRight = prevColidPos.x + frictionColidSize.x / 2;

	float blockTop = frictionBlockPos.y - frictionBlockSize.y / 2;
	float blockBottom = frictionBlockPos.y + frictionBlockSize.y / 2;
	float blockLeft = frictionBlockPos.x - frictionBlockSize.x / 2;
	float blockRight = frictionBlockPos.x + frictionBlockSize.x / 2;

	// 辺の判定
	if (playerPrevBottom <= blockTop && playerTop <= blockTop && playerBottom >= blockTop)
	{	// ブロックの上に衝突
		trans->SetPos(frictionColidPos.x, blockTop - trans->GetSize().y / 2);
	}
	else if (playerPrevTop >= blockBottom && playerTop <= blockBottom && playerBottom >= blockBottom)
	{	// ブロックの下に衝突
		trans->SetPos(frictionColidPos.x, blockBottom + trans->GetSize().y / 2);
	}
	else if (playerPrevRight <= blockLeft && playerRight >= blockLeft && playerLeft <= blockLeft)
	{	// ブロックの左に衝突
		trans->SetPos(blockLeft - frictionColidSize.x / 2, frictionColidPos.y);
	}
	else if (playerPrevLeft >= blockRight && playerLeft <= blockRight && playerRight >= blockRight)
	{	// ブロックの右に衝突
		trans->SetPos(blockRight + frictionColidSize.x / 2, frictionColidPos.y);
	}

	if (damage)obj->GetComponent<Damaged>()->Damage(1);

	// 再度衝突判定を取る
	ColidBlock();
	ColidLayer();

	isGround = true;
}

void PlayerGame::ColidLayer()
{
	//　初期化
	Transform2D* trans = obj->GetTransform();
	std::vector<Colider2D> colids = colidPool->GetColider();
	std::list<GameObject*>& map = BlockMap::GetInstance()->GetLayer2();

	bool isFriction = false;

	D3DXVECTOR2 frictionColidPos;
	D3DXVECTOR2	frictionColidSize;
	D3DXVECTOR2 frictionBlockPos;
	D3DXVECTOR2 frictionBlockSize;
	std::string targetTag = "";

	// ==============
	// ステージ衝突判定
	// ==============
	D3DXVECTOR2 pos = colids[0].GetPos();
	D3DXVECTOR2 size = colids[0].GetSize();


	// ==============
	// 衝突判定
	// ==============
	for (GameObject* object : map)
	{
		std::vector<Colider2D> target = object->GetComponent<ColiderPool>()->GetColider();
		for (int j = 0; j < target.size(); j++)
		{
			Colider2D colid = target[j];
			if (colid.GetTag() == "NOT")continue;

			D3DXVECTOR2 BlockPos = colid.GetPos();
			D3DXVECTOR2 BlockSize = colid.GetSize();

			D3DXVECTOR2 distance = D3DXVECTOR2(fabsf(pos.x - BlockPos.x), fabsf(pos.y - BlockPos.y));
			D3DXVECTOR2 sumSize = D3DXVECTOR2((size.x + BlockSize.x) / 2.0f, (size.y + BlockSize.y) / 2.0f);

			if (distance.x < sumSize.x &&
				distance.y < sumSize.y)
			{	// 衝突時
				g_GameData->SetLayerAlpha(0.6f);
				return;
			}
		}
		if (isFriction)break;
	} // 衝突判定終了

	// 衝突していないなら無透過
	g_GameData->SetLayerAlpha(1.0f);
}

void PlayerGame::ColidObject()
{
	//　初期化
	std::list<GameObject*>* pool = MainInGame::objectPool.GetPool();

	for (GameObject* object : *pool)
	{
		GameObject* target = object;
		Pushed* pPush = object->GetComponent<Pushed>();

		// 押せるものの場合計算する
		if (pPush) ColidPush(target, pPush);
	}
}

void PlayerGame::ColidPush(GameObject* target, Pushed* pushed)
{
	Transform2D* trans = obj->GetTransform();
	std::vector<Colider2D> colids = colidPool->GetColider();
	Colider2D* boxColid = &target->GetComponent<ColiderPool>()->GetColider()[0];

	// 衝突判定
	bool normalFriction = colids[0].IsColid(*boxColid);
	bool rightGrab = colids[1].IsColid(*boxColid);
	bool leftGrab = colids[2].IsColid(*boxColid);

	// どこにも衝突していないなら処理をしない
	if (!normalFriction && !rightGrab && !leftGrab)return;

	if (rightGrab && pushGrabKey) isGrab = true;
	else if (leftGrab && pushGrabKey) isGrab = true;
	else isGrab = false;

	D3DXVECTOR2 pos = colids[0].GetPos();
	D3DXVECTOR2 size = colids[0].GetSize();

	D3DXVECTOR2 boxPos = boxColid->GetPos();
	D3DXVECTOR2 boxSize = boxColid->GetSize();

	// ==============
	// 衝突辺の確認
	// ==============
	float playerTop = pos.y - size.y / 2;
	float playerBottom = pos.y + size.y / 2;
	float playerLeft = pos.x - size.x / 2;
	float playerRight = pos.x + size.x / 2;

	float playerPrevTop = prevColidPos.y - size.y / 2;
	float playerPrevBottom = prevColidPos.y + size.y / 2;
	float playerPrevLeft = prevColidPos.x - size.x / 2;
	float playerPrevRight = prevColidPos.x + size.x / 2;

	float boxTop = boxPos.y - boxSize.y / 2;
	float boxBottom = boxPos.y + boxSize.y / 2;
	float boxLeft = boxPos.x - boxSize.x / 2;
	float boxRight = boxPos.x + boxSize.x / 2;

	// 辺の判定
	if (playerPrevBottom <= boxTop && playerTop <= boxTop && playerBottom >= boxTop)
	{	// ブロックの上に衝突
		trans->SetPos(pos.x ,boxTop - size.y / 2 - 5.0f);
		isGround = true;
	}
	else if (playerPrevRight <= boxLeft && playerRight >= boxLeft && playerLeft <= boxLeft)
	{	// ブロックの左に衝突
		trans->SetPos(boxLeft  - size.x / 2, pos.y - 2.0f);
	}
	else if (playerPrevLeft >= boxRight && playerLeft <= boxRight && playerRight >= boxRight)
	{	// ブロックの右に衝突
		trans->SetPos(boxRight + size.x / 2, pos.y - 2.0f);
	}


	if ( !prevInput || !pushGrabKey) return;


	if (rightGrab)
	{
		D3DXVECTOR2 move = pushed->Push(D3DXVECTOR2(PLAYER_PUSH * moveDir, 0.0f));
		trans->SetPos(boxLeft + move.x - size.x / 2 - 3.0f, pos.y + move.y);
		boxColid->SetFriction(true);
	}
	else if (leftGrab)
	{
		D3DXVECTOR2 move = pushed->Push(D3DXVECTOR2(PLAYER_PUSH * moveDir, 0.0f));
		trans->SetPos(boxRight + move.x + size.x / 2, pos.y + move.y);
		boxColid->SetFriction(true);
	}
}

//void PlayerGame::ColidBlockLine()
//{
//	bool Colid = false;
//
//	// プレイヤーの四辺を取得
//	const D3DXVECTOR2* vtx =  obj->GetColider()[0].GetVertex();
//	D3DXVECTOR2 playerVec[4] = {
//		D3DXVECTOR2(vtx[1].x - vtx[0].x, vtx[1].y - vtx[0].y),
//		D3DXVECTOR2(vtx[2].x - vtx[1].x, vtx[2].y - vtx[1].y),
//		D3DXVECTOR2(vtx[3].x - vtx[2].x, vtx[3].y - vtx[2].y),
//		D3DXVECTOR2(vtx[0].x - vtx[3].x, vtx[0].y - vtx[3].y),
//	};
//
//	std::vector<GameObject*>map = BlockMap::GetInstance()->GetMap();
//
//	for (int i = 0; i < map.size(); i++)
//	{
//		// ブロックの四辺を取得
//		const D3DXVECTOR2* blockVtx = map[i]->GetColider()[0].GetVertex();
//		D3DXVECTOR2 blockVec[4] = {
//			D3DXVECTOR2(blockVtx[1].x - blockVtx[0].x, blockVtx[1].y - blockVtx[0].y),
//			D3DXVECTOR2(blockVtx[2].x - blockVtx[1].x, blockVtx[2].y - blockVtx[1].y),
//			D3DXVECTOR2(blockVtx[3].x - blockVtx[2].x, blockVtx[3].y - blockVtx[2].y),
//			D3DXVECTOR2(blockVtx[0].x - blockVtx[3].x, blockVtx[0].y - blockVtx[3].y),
//		};
//
//		for (int playerCnt = 0; playerCnt < 4; playerCnt++)
//		{
//			for (int blockCnt = 0; blockCnt < 4; blockCnt++)
//			{
//				// 平行時は抜ける (衝突していない)
//				if (Cross(playerVec[playerCnt], blockVec[blockCnt]) == 0.0f) break;
//
//				// ブロックの辺の先頭からプレイヤー先頭まで
//				D3DXVECTOR2 startToPlayer = D3DXVECTOR2(blockVtx[blockCnt % 4].x - playerVec[playerCnt % 4].x, blockVtx[blockCnt % 4].y - playerVec[playerCnt % 4].y);
//				// ブロックの辺の終端からプレイヤー先頭まで
//				D3DXVECTOR2 endToPlayer = D3DXVECTOR2(blockVtx[(blockCnt + 1) % 4].x - playerVec[playerCnt % 4].x, blockVtx[(blockCnt + 1) % 4].y - playerVec[playerCnt % 4].y);
//				// プレイヤー先頭からブロック先頭まで
//				D3DXVECTOR2 playerToStart = D3DXVECTOR2(playerVec[playerCnt % 4].x - blockVtx[blockCnt % 4].x, playerVec[playerCnt % 4].y - blockVtx[blockCnt % 4].y);
//				// プレイヤー先頭からブロック終端まで
//				D3DXVECTOR2 playerToEnd = D3DXVECTOR2(playerVec[playerCnt % 4].x - blockVtx[(blockCnt + 1) % 4].x, playerVec[playerCnt % 4].y - blockVtx[(blockCnt + 1) % 4].y);
//
//				float acCross = Cross(playerToStart, blockVec[blockCnt]);
//				float adCross = Cross(playerToEnd, blockVec[blockCnt]);
//				float caCross = Cross(startToPlayer, playerVec[playerCnt]);
//				float cbCross = Cross(endToPlayer, playerVec[playerCnt]);
//
//				float crossA = acCross * adCross;
//				float crossB = caCross * cbCross;
//
//				// 衝突した
//				if (crossA < 0.0f && crossB < 0.0f)
//				{
//					Colid = true;
//				}
//			}
//		}
//	}
//
//
//}
