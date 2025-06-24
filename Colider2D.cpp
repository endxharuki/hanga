// ========================================
// 2Dコリジョン雛形 [Colider2D.h]
// Author: 西窪 統吉桜	Date: 2023/09/08
// ========================================
#include "Colider2D.h"
#include "texture.h"
#include "sprite.h"
#include "OnGameData.h"
#include "Camera.h"

#include <algorithm>


void Colider2D::SetVertex()
{
	// 半径
	float hw, hh;
	hw = size.x* 0.5f;
	hh = size.y * 0.5f;

	// 座標変換
	vtx[0].x = (-hw) * cosf(rotate) - (-hh) * sinf(rotate) + drawPos.x;
	vtx[0].y = (-hw) * sinf(rotate) + (-hh) * cosf(rotate) + drawPos.y;
	vtx[1].x = (hw)*cosf(rotate) - (-hh) * sinf(rotate) + drawPos.x;
	vtx[1].y = (hw)*sinf(rotate) + (-hh) * cosf(rotate) + drawPos.y;
	vtx[2].x = (-hw) * cosf(rotate) - (hh)*sinf(rotate) + drawPos.x;
	vtx[2].y = (-hw) * sinf(rotate) + (hh)*cosf(rotate) + drawPos.y;
	vtx[3].x = (hw)*cosf(rotate) - (hh)*sinf(rotate) + drawPos.x;
	vtx[3].y = (hw)*sinf(rotate) + (hh)*cosf(rotate) + drawPos.y;
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

ColidDir Colider2D::IsDirColid(Colider2D obj)
{
	if (!IsColid(obj)) return NONE;

	// それぞれ横縦の半分
	float hw = size.x / 2;
	float hh = size.y / 2;
	float thw = obj.size.x / 2;
	float thh = obj.size.y / 2;

	// 方向検知
	D3DXVECTOR2 colidPos = GetPos();
	float right = colidPos.x + hw;
	float left = colidPos.x - hw;
	float up = colidPos.y - hh;
	float down = colidPos.y + hh;

	D3DXVECTOR2 target = obj.GetPos();
	float tRight = target.x + thw;
	float tLeft = target.x - thw;
	float tUp = target.y - thh;
	float tDown = target.y + thh;

	float rightDir = std::abs(right - tLeft);
	float leftDir = std::abs(tRight - left);
	float upDir = std::abs(tDown - up);
	float downDir = std::abs(down - tUp);

	// windows.hの方のminを使わないようにする(というか使えない)
	// 最も辺と辺の差が小さい辺
	float minDir = (std::min)({rightDir, leftDir, upDir, downDir});

	if (minDir == rightDir)return RIGHT;
	else if (minDir == leftDir) return LEFT;
	else if (minDir == upDir)return UP;
	else if (minDir == downDir)return DOWN;

	return NONE;
}

void Colider2D::viewColid()
{
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();
	drawPos = GetPos() - cameraPos;
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
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();
	drawPos = GetPos() - cameraPos;
	SetVertex();

	DrawSpriteVtxSetColor(OnGameData::GetInstance()->GetColidTex(),
		vtx,
		0.0f, 0.0f,
		1.0f, 1.0f,
		colidColor.r, colidColor.g, colidColor.b, colidColor.a);
}
