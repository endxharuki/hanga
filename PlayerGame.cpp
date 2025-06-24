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
#include "sound.h"
#include "EffectManager.h"


#define PLAYER_TEX_X (5)
#define PLAYER_TEX_Y (2)
#define PLAYER_PUSH (2.7f)

#define PI (3.1415f)

// グローバル変数
OnGameData*		g_GameData = OnGameData::GetInstance();

// 処理速度計測用
static std::chrono::high_resolution_clock::time_point start;
static std::chrono::high_resolution_clock::time_point end;

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
	idleTex[0] = LoadTexture((char*)"data/TEXTURE/PLAYER/idle.png");
	idleTex[1] = LoadTexture((char*)"data/TEXTURE/PLAYER/idle_R.png");
	walkTex[0] = LoadTexture((char*)"data/TEXTURE/PLAYER/walk.png");
	walkTex[1] = LoadTexture((char*)"data/TEXTURE/PLAYER/walk_R.png");
	runTex[0] = LoadTexture((char*)"data/TEXTURE/PLAYER/run.png");
	runTex[1] = LoadTexture((char*)"data/TEXTURE/PLAYER/run_R.png");
	jumpTex[0] = LoadTexture((char*)"data/TEXTURE/PLAYER/jump2.png");
	jumpTex[1] = LoadTexture((char*)"data/TEXTURE/PLAYER/jump2_R.png");
	pushTex[0] = LoadTexture((char*)"data/TEXTURE/PLAYER/push.png");
	pushTex[1] = LoadTexture((char*)"data/TEXTURE/PLAYER/push_R.png");
	
	JumpSand[0] = LoadTexture((char*)"data/TEXTURE/PLAYER/sunabokori_5.png");
	JumpSand[1] = LoadTexture((char*)"data/TEXTURE/PLAYER/sunabokori_5_R.png");

	dashSand[0] = LoadTexture((char*)"data/TEXTURE/PLAYER/sunabokori_3.png");
	dashSand[1] = LoadTexture((char*)"data/TEXTURE/PLAYER/sunabokori_3_R.png");

	X_buttonTex = LoadTexture((char*)"data/TEXTURE/Button_X.png");

	// 先に読み込んでおく
	LoadTexture((char*)"data/TEXTURE/GOAL/goal_motion.png");

	jumpSe = LoadSound((char*)"data/SE/jump.wav");
	SetVolume(jumpSe, 0.75f);

	currentWalkTex = 0;
	currentIdleTex = 0;
	currentIdleTex = 0;
	currentJumpTex = 0;

	circleTex = LoadTexture((char*)"data/TEXTURE/playerCircle.jpg");

	drawMode = g_GameData->HasSwap() ? 1 : 0;


	obj->GetTransform()->SetGravity(9.0f);
	obj->GetTransform()->SetRotation(0.0f);
	obj->GetTransform()->SetRotationVel(0.0f);

	colidPool = obj->GetComponent<ColiderPool>();
	damage = obj->GetComponent<Damaged>();
	prevDamage = false;


	prevHasSwap = g_GameData->HasSwap();
	isGround = false;
	isRun = false;
	isWalk = false;
	isIdle = true;
	isJump = false;
	inPush = false;
	pushJumpKey = false;
	pushGrabKey = false;
	drawPlayer = true;
	moveDir = 1.0f;
	moveSign = 1.0f;

	mapRightLimit = Camera::GetInstance()->GetRightLimit();

	isDie = false;

	currentJumpFrame = 0;
	frameCnt = 0;

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

	start = std::chrono::high_resolution_clock::now();

	++frameCnt;
	if (frameCnt > 30000)frameCnt = 0;

	// ================
	// 各種判定
	// ================
	// 反転した際の処理
	if (prevHasSwap != g_GameData->HasSwap())
	{
		prevHasSwap = !prevHasSwap;
		drawMode = (drawMode + 1) % 2;

		moveSign *= -1.0f;

		transform->SetVel(-transform->GetVel().x, transform->GetVel().y);
		transform->SetOutVel(-transform->GetOutVel().x, transform->GetOutVel().y);
	}

	// プレイヤー主体の速度の大きさが4.0以上なら走っているとみなす
	if (fabsf(transform->GetVel().x) >= 4.0f)
	{
		isRun = true;
		isWalk = false;
		isIdle = false;
	}
	else if (fabsf(transform->GetVel().x) >= 1.0f)// プレイヤー主体の速度の大きさが1.0以上なら動いているとみなす
	{
		isWalk = true;
		isRun = false;
		isIdle = false;
	}
	else
	{
		isIdle = true;
		isWalk = false;
		isRun = false;
	}


	// 走りアニメーション処理
	if (isRun)
	{
		rU = (currentRunTex % (PLAYER_TEX_X)) * (1.0f / (PLAYER_TEX_X));
		rV = (currentRunTex / (PLAYER_TEX_X)) * (1.0f / PLAYER_TEX_Y);
		rUW = 1.0f / (PLAYER_TEX_X);
		rVH = 1.0f / PLAYER_TEX_Y;

		if (frameCnt % 3 == 0)++currentRunTex;
		if (currentRunTex >= 8)currentRunTex = 0;

		// すなぼこり
		++dashCnt;
		if (dashCnt % 6 == 0 && isGround)
		{
			EffectObj sand;
			sand.allFrame = 15;
			sand.tex[0] = dashSand[0];
			sand.tex[1] = dashSand[1];
			sand.pos = { transform->GetPos().x + (-moveDir * 55.0f), transform->GetPos().y + 70.0f };
			sand.size = { 40.0f, 40.0f };
			MainInGame::effectManager.AlphaAdd(sand);
		}
	}
	else
	{
		currentRunTex = 0;
		dashCnt = 0;
	}
	// 歩きアニメーション処理
	if (isWalk)
	{
		currentWalkTex = frameCnt / 3;

		wUV.x = (currentWalkTex % PLAYER_TEX_X) * (1.0f / PLAYER_TEX_X);
		wUV.y = (currentWalkTex / PLAYER_TEX_X) * (1.0f / PLAYER_TEX_Y);
		wUV.z = 1.0f / PLAYER_TEX_X;
		wUV.w = 1.0f / PLAYER_TEX_Y;
	}
	else
	{
		currentWalkTex = 0;
	}
	// 待機アニメーション処理
	if (isIdle)
	{
		currentIdleTex = frameCnt / 3;

		iUV.x = (currentIdleTex % PLAYER_TEX_X) * (1.0f / PLAYER_TEX_X);
		iUV.y = (currentIdleTex / PLAYER_TEX_X) * (1.0f / PLAYER_TEX_Y);
		iUV.z = 1.0f / PLAYER_TEX_X;
		iUV.w = 1.0f / PLAYER_TEX_Y;
	}
	else
	{
		currentIdleTex = 0;
	}
	// ジャンプアニメーション処理
	if (isJump)
	{
		currentJumpTex = frameCnt / 3;

		jUV.x = (currentJumpTex % PLAYER_TEX_X) * (1.0f / PLAYER_TEX_X);
		jUV.y = (currentJumpTex / PLAYER_TEX_X) * (1.0f / PLAYER_TEX_Y);
		jUV.z = 1.0f / PLAYER_TEX_X;
		jUV.w = 1.0f / PLAYER_TEX_Y;
	}
	else
	{
		currentJumpTex = 0;
	}

	if (isGrab)
	{
		currentPushTex = frameCnt / 3;

		pUV.x = (currentPushTex % PLAYER_TEX_X) * (1.0f / PLAYER_TEX_X);
		pUV.y = (currentPushTex / PLAYER_TEX_X) * (1.0f / PLAYER_TEX_Y);
		pUV.z = 1.0f / PLAYER_TEX_X;
		pUV.w = 1.0f / PLAYER_TEX_Y;
	}
	else
	{
		currentPushTex = 0;
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
	notCtrl = !OnGameData::GetInstance()->IsCtrlUser();

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
	{
		g_GameData->SetSwapAnim(true);
		return;
	}

	if (GetKeyboardPress(DIK_O))
	{
		D3DXVECTOR2 zoom = Camera::GetInstance()->GetZoom();
		Camera::GetInstance()->SetZoom(zoom.x + 0.01f, zoom.y + 0.01f);
	}
	else if (GetKeyboardPress(DIK_P))
	{
		D3DXVECTOR2 zoom = Camera::GetInstance()->GetZoom();
		Camera::GetInstance()->SetZoom(zoom.x - 0.01f, zoom.y - 0.01f);
	}


	// 移動処理
	Move();

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
	inPush = false;

	isGround = obj->IsSetGround();

	// =======================
	// 衝突処理
	// =======================
	// yは枠外にでないようにする(一応)
	float downLimit = Camera::GetInstance()->GetInstance()->GetDownLimit();

	if (transform->GetPos().y + transform->GetSize().y * 0.5f > downLimit)
	{
		//transform->SetPos(transform->GetPos().x, SCREEN_HEIGHT - transform->GetSize().y * 0.5f);
		obj->GetTransform()->SetPos(100.0f, 900.0f);
		if (g_GameData->HasSwap()) g_GameData->ChangeSwap();
		isGround = true;
		isDie = true;
		OnGameData::GetInstance()->SetGameOver(true);
	}
	colidCheckNum = 0;
	ColidObject();
	ColidBlock();
	ColidOverMap();

	prevColidPos = colidPool->GetColider()[0].GetPos();

	Camera::GetInstance()->SetPos(transform->GetPos().x, transform->GetPos().y - 150.0f);

	if (!g_GameData->HasSwap())
	{
		D3DXVECTOR2 camPos = Camera::GetInstance()->GetOriginPos();
		D3DXVECTOR2 maskPos = transform->GetPos() - camPos;
		maskPos.x = (float)maskPos.x / SCREEN_WIDTH;
		maskPos.y = (float)maskPos.y / SCREEN_HEIGHT;
		SetMask(maskPos, { 0.25f, 0.35f });
	}
	else
	{
		SetMask({ 0.0f, 0.0f }, { 0.0f, 0.0f });
	}

	end = std::chrono::high_resolution_clock::now();
}

void PlayerGame::OnDraw()
{
	if (isDie) return;

	D3DXVECTOR2 pos =  obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	if (drawPlayer)
	{
		float backAdd = 0.0f;
		if (moveDir < 0)
		{
			backAdd = 1.0f;
		}

		if (isJump)
		{
			DrawSpriteColor(jumpTex[drawMode],
				drawPos.x, drawPos.y,
				size.x, size.y,
				jUV.x + (jUV.z * backAdd), jUV.y,
				jUV.z * moveDir, jUV.w,
				1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (isGrab)
		{
			DrawSpriteColor(pushTex[drawMode],
				drawPos.x, drawPos.y,
				size.x, size.y,
				pUV.x + (pUV.z * backAdd), pUV.y,
				pUV.z * pushDir, pUV.w,
				1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (isRun)
		{
			DrawSpriteColor(runTex[drawMode],
				drawPos.x, drawPos.y,
				size.x, size.y,
				rU + (rUW * backAdd), rV,
				rUW * moveDir, rVH,
				1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (isWalk)
		{
			DrawSpriteColor(walkTex[drawMode],
				drawPos.x, drawPos.y,
				size.x, size.y,
				wUV.x + (wUV.z * backAdd), wUV.y,
				wUV.z * moveDir, wUV.w,
				1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			DrawSpriteColor(idleTex[drawMode],
				drawPos.x, drawPos.y,
				size.x, size.y,
				iUV.x + (iUV.z * backAdd), iUV.y,
				iUV.z * moveDir, iUV.w,
				1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	if (inPush && !pushGrabKey)
	{
		DrawSprite(X_buttonTex,
			drawPos.x, drawPos.y - 100.0f,
			pushAnounceSize.x, pushAnounceSize.y, 
			0.0f, 0.0f,
			1.0f, 1.0f);
	}

	write->SetFont(debugData);
	SetPixelShader(0);
}

void PlayerGame::Move()
{
	Transform2D* transform = obj->GetTransform();
	float moveVel = 2.5f;

	// ダッシュ判定
	// TODO: 着地時じゃないとダッシュ開始できないようにしてもいいかも
	if (GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, BUTTON_A)) moveVel = 4.35f;

	// 移動処理
	if (GetKeyboardPress(DIK_A) || GetLeftStick(0).x < 0.0f)
	{// 左
		if (!isGrab)
		{
			transform->SetVel(transform->GetVel().x - moveVel * moveSign, transform->GetVel().y);
		}
		prevInput = true;
		moveDir = -1.0f * moveSign;
	}
	if (GetKeyboardPress(DIK_D) || GetLeftStick(0).x > 0.0f)
	{// 右
		if (!isGrab)
		{
			transform->SetVel(transform->GetVel().x + moveVel * moveSign, transform->GetVel().y);
		}
		prevInput = true;
		moveDir = 1.0f * moveSign;
	}
}

void PlayerGame::Jump()
{
	if (!pushJumpKey || currentJumpFrame >= jumpFrame)
	{
		isJump = false;
		currentJumpFrame = 0;
		return;
	}

	Transform2D* transform = obj->GetTransform();

	// キーを押した瞬間にジャンプの値を最大値に初期化
	if (currentJumpFrame == 0)
	{
		currentJumpHeight = maxJumpHeight;
		isJump = true;

		// すなぼこり
		EffectObj sand;
		sand.allFrame = 13;
		sand.tex[0] = JumpSand[0];
		sand.tex[1] = JumpSand[1];
		sand.size = { 65.0f, 65.0f };
		sand.pos = { transform->GetPos().x - 40.0f, transform->GetPos().y + 40.0f };

		MainInGame::effectManager.AlphaAdd(sand);
		sand.pos.x += 80.0f;
		MainInGame::effectManager.AlphaAdd(sand);

		PlaySound(jumpSe, 0);
	}

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

	bool isFriction = false;
	bool damageColid = false;
	bool damaged = false;

	D3DXVECTOR2 frictionColidPos;
	D3DXVECTOR2	frictionColidSize;
	D3DXVECTOR2 frictionBlockPos;
	D3DXVECTOR2 frictionBlockSize;
	std::string targetTag = "";

	D3DXVECTOR2 moveAll = {0.0f, 0.0f};

	// ==============
	// ステージ衝突判定
	// ==============
	D3DXVECTOR2 pos = colids[0].GetPos();
	D3DXVECTOR2 size = colids[0].GetSize();

	std::list<GameObject*>& map = BlockMap::GetInstance()->GetSortedMap(pos, size + D3DXVECTOR2(10.0f, 0.0f));

	// ==============
	// 衝突判定
	// ==============
	for (GameObject* object : map)
	{
		std::vector<Colider2D> target = object->GetComponent<ColiderPool>()->GetColider();
		damageColid = false;

		for (int j = 0; j < target.size(); j++)
		{
			Colider2D colid = target[j];
			if (colid.GetTag() == "NOT")continue;
			if (colid.GetTag() == "Damage") damageColid = true;

			D3DXVECTOR2 BlockPos = colid.GetPos();
			D3DXVECTOR2 BlockSize = colid.GetSize();

			D3DXVECTOR2 playerPos = pos + moveAll;

			D3DXVECTOR2 distance = D3DXVECTOR2(fabsf(playerPos.x - BlockPos.x), fabsf(playerPos.y - BlockPos.y));
			D3DXVECTOR2 sumSize = D3DXVECTOR2((size.x + BlockSize.x) * 0.5f, (size.y + BlockSize.y) * 0.5f);

			if (distance.x < sumSize.x &&
				distance.y < sumSize.y)
			{	// 衝突時
				isFriction = true;
				frictionColidPos = playerPos;
				frictionColidSize = size;
				frictionBlockPos = BlockPos;
				frictionBlockSize = BlockSize;
				targetTag = colid.GetTag();
				
				if (damageColid) damaged = true;

				// ==============
				// 衝突辺の確認
				// ==============
				float playerTop = frictionColidPos.y - frictionColidSize.y * 0.5f;
				float playerBottom = frictionColidPos.y + frictionColidSize.y * 0.5f;
				float playerLeft = frictionColidPos.x - frictionColidSize.x * 0.5f;
				float playerRight = frictionColidPos.x + frictionColidSize.x * 0.5f;

				float playerPrevTop = prevColidPos.y - frictionColidSize.y * 0.5f;
				float playerPrevBottom = prevColidPos.y + frictionColidSize.y * 0.5f;
				float playerPrevLeft = prevColidPos.x - frictionColidSize.x * 0.5f;
				float playerPrevRight = prevColidPos.x + frictionColidSize.x * 0.5f;

				float blockTop = frictionBlockPos.y - frictionBlockSize.y * 0.5f;
				float blockBottom = frictionBlockPos.y + frictionBlockSize.y * 0.5f;
				float blockLeft = frictionBlockPos.x - frictionBlockSize.x * 0.5f;
				float blockRight = frictionBlockPos.x + frictionBlockSize.x * 0.5f;

				// 辺の判定
	
				if (playerPrevRight <= blockLeft && playerRight >= blockLeft && playerLeft <= blockLeft)
				{	// ブロックの左に衝突
					if(fabsf(playerPrevBottom - blockTop) > 0.01f )moveAll.x += blockLeft - playerRight;
				}
				else if (playerPrevLeft >= blockRight && playerLeft <= blockRight && playerRight >= blockRight)
				{	// ブロックの右に衝突
					if (fabsf(playerPrevBottom - blockTop) > 0.01f)moveAll.x += blockRight - playerLeft;
				}
				else if (playerPrevBottom <= blockTop && playerTop <= blockTop && playerBottom >= blockTop)
				{	// ブロックの上に衝突
					moveAll.y += blockTop - playerBottom;
					isGround += true;
				}
				else if (playerPrevTop >= blockBottom && playerTop <= blockBottom && playerBottom >= blockBottom)
				{	// ブロックの下に衝突
					moveAll.y += blockBottom - playerTop;
				}
			}
		}
	}

	colids[0].SetPos(colids[0].GetPos().x + moveAll.x, colids[0].GetPos().y + moveAll.y);
	if (damaged)
	{
		obj->GetComponent<Damaged>()->Damage(1);
	}
}

void PlayerGame::ColidObject()
{
	//　初期化
	std::list<GameObject*>* pool = MainInGame::objectPool.GetPool();

	for (GameObject* object : *pool)
	{
		GameObject* target = object;
		Pushed* pPush = object->GetComponent<Pushed>();

		if (fabsf(obj->GetTransform()->GetPos().x - target->GetTransform()->GetPos().x) > 2000.0f)continue;

		// 押せるものの場合計算する
		if (pPush) ColidPush(target, pPush);
	}
}

void PlayerGame::ColidOverMap()
{
	std::vector<Colider2D> colids = colidPool->GetColider();
	D3DXVECTOR2 pos = colids[0].GetPos();
	D3DXVECTOR2 size = colids[0].GetSize();

	if (pos.x - size.x * 0.5f < 0.0f)
	{
		colids[0].SetPos(size.x * 0.5f, pos.y);
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

	inPush = rightGrab || leftGrab || inPush;

	// どこにも衝突していないなら処理をしない
	if (!normalFriction && !rightGrab && !leftGrab)return;

	if (rightGrab && pushGrabKey)
	{
		pushDir = 1.0f;
		isGrab = true;
	}
	else if (leftGrab && pushGrabKey)
	{
		isGrab = true;
		pushDir = -1.0f;
	}
	else isGrab = false;

	D3DXVECTOR2 pos = colids[0].GetPos();
	D3DXVECTOR2 size = colids[0].GetSize();
	D3DXVECTOR2 moveAll = { 0.0f, 0.0f };

	D3DXVECTOR2 boxPos = boxColid->GetPos();
	D3DXVECTOR2 boxSize = boxColid->GetSize();

	// ==============
	// 衝突辺の確認
	// ==============
	float playerTop = pos.y - size.y * 0.5f;
	float playerBottom = pos.y + size.y * 0.5f;
	float playerLeft = pos.x - size.x * 0.5f;
	float playerRight = pos.x + size.x * 0.5f;

	float playerPrevTop = prevColidPos.y - size.y * 0.5f;
	float playerPrevBottom = prevColidPos.y + size.y * 0.5f;
	float playerPrevLeft = prevColidPos.x - size.x * 0.5f;
	float playerPrevRight = prevColidPos.x + size.x * 0.5f;

	float boxTop = boxPos.y - boxSize.y * 0.5f;
	float boxBottom = boxPos.y + boxSize.y * 0.5f;
	float boxLeft = boxPos.x - boxSize.x * 0.5f;
	float boxRight = boxPos.x + boxSize.x * 0.5f;

	// 辺の判定
	if (playerPrevBottom <= boxTop && playerTop <= boxTop && playerBottom >= boxTop)
	{	// ブロックの上に衝突
		colids[0].SetPos(pos.x ,boxTop - size.y * 0.5f - 5.0f);
		isGround = true;
	}
	else if (playerPrevTop >= boxBottom && playerBottom  >= boxBottom && playerTop <= boxBottom)
	{	// ブロックの下に衝突
		colids[0].SetPos(pos.x, boxBottom + size.y * 0.5f + 5.0f);
	}
	else if (playerPrevRight <= boxLeft && playerRight >= boxLeft && playerLeft <= boxLeft)
	{	// ブロックの左に衝突
		moveAll.x = boxLeft - playerRight - 0.5f;
	}
	else if (playerPrevLeft >= boxRight && playerLeft <= boxRight && playerRight >= boxRight)
	{	// ブロックの右に衝突
		moveAll.x = boxRight - playerLeft + 0.5f;
	}

	colids[0].SetPos(colids[0].GetPos().x + moveAll.x, colids[0].GetPos().y + moveAll.y);

	if ( !prevInput || !pushGrabKey) return;


	if (rightGrab)
	{
		if (moveDir > 0)
		{	// 箱方向に動かすなら
			D3DXVECTOR2 move = pushed->Push(D3DXVECTOR2(PLAYER_PUSH * moveDir, 0.0f));
			colids[0].SetPos(boxLeft + move.x - size.x * 0.5f, pos.y + move.y);
			boxColid->SetFriction(true);
		}
		else
		{	// 後ろ方向に動かすなら
			D3DXVECTOR2 first = trans->GetPos();
			D3DXVECTOR2 move = D3DXVECTOR2(PLAYER_PUSH * moveDir, 0.0f);
			colids[0].SetPos(boxLeft + move.x - size.x * 0.5f, pos.y + move.y);

			ColidBlock();
			D3DXVECTOR2 moved = colids[0].GetPos();
			float boxMove = moved.x - first.x;
			pushed->Push(D3DXVECTOR2(boxMove, 0.0f));
		}
	}
	else if (leftGrab)
	{
		if (moveDir < 0)
		{	// 箱方向に動かすなら
			D3DXVECTOR2 move = pushed->Push(D3DXVECTOR2(PLAYER_PUSH * moveDir, 0.0f));
			colids[0].SetPos(boxRight + move.x + size.x * 0.5f, pos.y + move.y);
			boxColid->SetFriction(true);
		}
		else
		{	// 後ろ方向に動かすなら
			D3DXVECTOR2 first = trans->GetPos();
			D3DXVECTOR2 move = D3DXVECTOR2(PLAYER_PUSH * moveDir, 0.0f);
			colids[0].SetPos(boxRight + move.x + size.x * 0.5f, pos.y + move.y);

			ColidBlock();
			D3DXVECTOR2 moved = colids[0].GetPos();
			float boxMove = moved.x - first.x;
			pushed->Push(D3DXVECTOR2(boxMove, 0.0f));
		}
	}
}