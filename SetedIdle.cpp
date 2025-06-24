// ========================================
// プレイヤー設置ブロック: 落下中 [SetedIdle.cpp]
// Author: 西窪 統吉桜	Date: 2023/09/08
// ========================================
#include "SetedIdle.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"
#include "BlockMap.h"
#include "OnGameData.h"


void SetedIdle::SetUp()
{
	CleanRequest();

	block_tex = OnGameData::GetInstance()->GetColidTex();

	obj->GetColider().push_back(Colider2D(obj->GetTransform()));
}

void SetedIdle::CleanUp()
{

}

void SetedIdle::OnUpdate()
{
	
}

void SetedIdle::OnDraw()
{
	if (!*shouldPrint) return;

	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();

	DrawSpriteColorRotate(block_tex,
		pos.x, pos.y,
		obj->GetTransform()->GetSize().x, obj->GetTransform()->GetSize().y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		obj->GetTransform()->GetRotation());
}



