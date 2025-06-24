#include "GoalLight.h"
#include "sprite.h"
#include "texture.h"
#include "Camera.h"

#include "Goal.h"

#define TEX_X (5)
#define TEX_Y (4)
#define TEX_ALL (20)

void GoalLight::SetUp()
{
	CleanRequest();
	tex = LoadTexture((char*)"data/TEXTURE/GOAL/tourou_lit.png");
	OnUpdate();
}

void GoalLight::CleanUp()
{

}

void GoalLight::OnUpdate()
{
	++frameCnt;
	texCnt = frameCnt / 3;
	if (texCnt >= TEX_ALL)return;

	uv.x = (float)(texCnt % TEX_X * (1.0f / TEX_X));
	uv.y = (float)(texCnt / TEX_X * (1.0f / TEX_Y));
	uv.z = (float)(1.0f / TEX_X);
	uv.w = (float)(1.0f / TEX_Y);
}

void GoalLight::OnDraw()
{
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	DrawSpriteColor(tex,
		drawPos.x, drawPos.y,
		size.x, size.y,
		uv.x, uv.y,
		uv.z, uv.w,
		1.0f, 1.0f, 1.0f, 1.0f
	);
}
