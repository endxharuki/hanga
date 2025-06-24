#include "BeanDemonIdle.h"
#include "BeanDemon.h"
#include "sprite.h"
#include "Camera.h"

void BeanDemonIdle::SetUp()
{
	CleanRequest();
}

void BeanDemonIdle::CleanUp()
{
}

void BeanDemonIdle::OnUpdate()
{
	if (obj->GetActive()) ChangeRequest("Move");

	// TODO: �ݒu�����s���R�ȏꏊ�ɂȂ�Ȃ�d�͂ɏ]���ău���b�N�Ɠ����蔻�����������
	//		 ���܂���������ׂ���
}

void BeanDemonIdle::OnDraw()
{
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	DrawSpriteColor( obj->idleTex,
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
}
