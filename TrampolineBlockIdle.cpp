#include "TrampolineBlockIdle.h"
#include "BlockMap.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "Camera.h"
#include "Player.h"
#include "MainInGame.h"
#include "OnGameData.h"

D3DXVECTOR2 oldTransPos;

void TrampolineBlockIdle::SetUp()
{
	CleanRequest();

	blockTex[0] = LoadTexture((char*)"data/TEXTURE/DashBlock1.png");
	blockTex[1] = LoadTexture((char*)"data/TEXTURE/DashBlock2.png");
	drawMode = 0;

	// コリジョンの生成
	pool = obj->GetComponent<ColiderPool>();

	pool->Add(Colider2D(obj->GetTransform()));

	// 左判定
	pool->Add(Colider2D(
		obj->GetTransform(),
		D3DXVECTOR2( -obj->GetTransform()->GetSize().x / 2,0.0f),
		D3DXVECTOR2(0.0f, obj->GetTransform()->GetSize().y - 10.0f)
	));

	// 右判定
	pool->Add(Colider2D(
		obj->GetTransform(),
		D3DXVECTOR2(obj->GetTransform()->GetSize().x / 2, 0.0f),
		D3DXVECTOR2(0.0f, obj->GetTransform()->GetSize().y - 10.0f)
	));

	/*
	// 上判定
	pool->Add(Colider2D(
		obj->GetTransform(),
		D3DXVECTOR2(0.0f,-obj->GetTransform()->GetSize().y / 2),
		D3DXVECTOR2(obj->GetTransform()->GetSize().x, 0.0f)
	));

	// 下判定
	pool->Add(Colider2D(
		obj->GetTransform(),
		D3DXVECTOR2(0.0f, obj->GetTransform()->GetSize().y / 2),
		D3DXVECTOR2(obj->GetTransform()->GetSize().x, 0.0f)
	));
	*/


	// 向きの指定
	if (boundDirection == 0)
	{
		moveSign = -1.0f;
	}
	if (boundDirection == 1)
	{
		moveSign = 1.0f;
	}

	oldTransPos = MainInGame::player->GetTransform()->GetPos();

	prevHasSwap = OnGameData::GetInstance()->HasSwap();
}

void TrampolineBlockIdle::CleanUp()
{

}

void TrampolineBlockIdle::OnUpdate()
{
	if (prevHasSwap != OnGameData::GetInstance()->HasSwap())
	{
		drawMode = (drawMode + 1) % 2;
		prevHasSwap = !prevHasSwap;
	}

	Player* pPlayer = MainInGame::player;
	Transform2D* transform = pPlayer->GetTransform();
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	//D3DXVECTOR2 oldTransPos = pPlayer->GetTransform()->GetPos() - pPlayer->GetTransform()->GetVel();

	// 左右で跳ねる向きの判定
	if ( pool->GetColider()[boundDirection + 1].IsColid(pPlayer->GetComponent<ColiderPool>()->GetColider()[0]))
	{
		transform->SetVel(-transform->GetVel());//移動値を反転
		transform->SetOutVel(transform->GetVel().x + 15.0f * moveSign, transform->GetVel().y);//外部速度に加算
	}
	//埋まり解消
	if (pool->GetColider()[0].IsColid(pPlayer->GetComponent<ColiderPool>()->GetColider()[0]))
	{
		float blockTop = pos.y - size.y / 2;
		float blockLeft = pos.x - size.x / 2;
		float blockRight = pos.x + size.x / 2;
		float blockBottom = pos.y + size.y / 2;

		float playerTop = transform->GetPos().y - transform->GetSize().y / 2;
		float playerLeft = transform->GetPos().x - transform->GetSize().x / 2;
		float playerRight = transform->GetPos().x + transform->GetSize().x / 2;
		float playerBottom = transform->GetPos().y + transform->GetSize().y / 2;

		float playerOldTop = oldTransPos.y - transform->GetSize().y / 2;
		float playerOldLeft = oldTransPos.x - transform->GetSize().x / 2;
		float playerOldRight = oldTransPos.x + transform->GetSize().x / 2;
		float playerOldBottom = oldTransPos.y + transform->GetSize().y / 2;

		if (blockTop < playerBottom && blockTop > playerOldBottom)//上
		{ 
			transform->SetPos(transform->GetPos().x, blockTop - transform->GetSize().y / 2);
		}
		if (blockLeft < playerRight && blockLeft > playerOldRight)//左
		{
			transform->SetPos(blockLeft - transform->GetSize().x / 2, transform->GetPos().y);
		}
		if (blockRight > playerLeft && blockRight < playerOldLeft)//右
		{
			transform->SetPos(blockRight + transform->GetSize().x / 2, transform->GetPos().y);
		}
		if (blockBottom > playerTop && blockBottom < playerOldTop)//下
		{
			transform->SetPos(transform->GetPos().x, blockBottom + transform->GetSize().y / 2);
		}
	}
	/*
	for (int i = 0; i < 4; i++)
	{
		if (pool->GetColider()[i + 1].IsColid(pPlayer->GetComponent<ColiderPool>()->GetColider()[0]))
		{
			switch (i)
			{
			case 0://左
				transform->SetPos(pos.x - size.x/2 - transform->GetSize().x/2, transform->GetPos().y);
				break;
			case 1://右
				transform->SetPos(pos.x + size.x / 2 + transform->GetSize().x / 2, transform->GetPos().y);
				break;
			case 2://上
				transform->SetPos(transform->GetPos().x, pos.y - size.y / 2 - transform->GetSize().y / 2);
				break;
			case 3://下
				transform->SetPos(transform->GetPos().x, pos.y + size.y / 2 + transform->GetSize().y / 2);
				break;

			default:
				break;
			}
		}
	}
	*/

	oldTransPos = pPlayer->GetTransform()->GetPos();
}

void TrampolineBlockIdle::OnDraw()
{
	// カメラの移動
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	Camera* camera = Camera::GetInstance();
	D3DXVECTOR2 drawPos = D3DXVECTOR2(pos.x - camera->GetOriginPos().x, pos.y - camera->GetOriginPos().y);

	DrawSprite(blockTex[drawMode],
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		size.x / CHIP_X_SIZE * moveSign, size.y / CHIP_Y_SIZE
	);

	//obj->GetColider()[0].viewColid(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.3f));
	//obj->GetColider()[1].viewColid(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f));
}