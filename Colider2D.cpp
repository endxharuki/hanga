// ========================================
// 2Dコリジョン雛形 [Colider2D.h]
// Author: 西窪 統吉桜	Date: 2023/09/08
// ========================================
#include "Colider2D.h"
#include "texture.h"
#include "sprite.h"
#include "OnGameData.h"
#include "Camera.h"


void Colider2D::SetVertex()
{
	// 半径
	float hw, hh;
	hw = size.x* 0.5f;
	hh = size.y * 0.5f;

	// 座標変換
	vtx[0].x = (-hw) * cosf(rotate) - (-hh) * sinf(rotate) + pos.x;
	vtx[0].y = (-hw) * sinf(rotate) + (-hh) * cosf(rotate) + pos.y;
	vtx[1].x = (hw)*cosf(rotate) - (-hh) * sinf(rotate) + pos.x;
	vtx[1].y = (hw)*sinf(rotate) + (-hh) * cosf(rotate) + pos.y;
	vtx[2].x = (-hw) * cosf(rotate) - (hh)*sinf(rotate) + pos.x;
	vtx[2].y = (-hw) * sinf(rotate) + (hh)*cosf(rotate) + pos.y;
	vtx[3].x = (hw)*cosf(rotate) - (hh)*sinf(rotate) + pos.x;
	vtx[3].y = (hw)*sinf(rotate) + (hh)*cosf(rotate) + pos.y;
}

void Colider2D::Rotate(float radAngle)
{
	rotate = radAngle;
}


bool Colider2D::IsColid(Colider2D obj)
{
	// 相手のプロパティ
	D3DXVECTOR2 objPos = obj.GetPos();
	D3DXVECTOR2 objSize = obj.GetSize();

	D3DXVECTOR2 distance = D3DXVECTOR2(fabsf(GetPos().x - objPos.x), fabsf(GetPos().y - objPos.y));
	D3DXVECTOR2 sumSize = D3DXVECTOR2((size.x + objSize.x) / 2.0f, (size.y + objSize.y) / 2.0f);

	if (distance.x < sumSize.x &&
		distance.y < sumSize.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Colider2D::viewColid()
{
	pos = parent->GetPos() + offset;
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();
	pos = pos - cameraPos;
	SetVertex();

	color = isFriction ? D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f) : D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.4f);
	DrawSpriteVtxSetColor(OnGameData::GetInstance()->GetColidTex(),
		vtx,
		0.0f, 0.0f,
		1.0f, 1.0f,
		color.r, color.g, color.b, color.a);
}

// デバッグ用
void Colider2D::viewColid(D3DXCOLOR colidColor)
{
	pos = parent->GetPos() + offset;
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();
	pos = pos - cameraPos;
	SetVertex();

	DrawSpriteVtxSetColor(OnGameData::GetInstance()->GetColidTex(),
		vtx,
		0.0f, 0.0f,
		1.0f, 1.0f,
		colidColor.r, colidColor.g, colidColor.b, colidColor.a);
}
