#include "BeanDemonMove.h"
#include "BeanDemon.h"
#include "Camera.h"
#include "Sprite.h"

#include <random>
#include <math.h>

void BeanDemonMove::SetUp()
{
	CleanRequest();

	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());

	engine();
	std::uniform_int_distribution<int> dist(0, MOVE_POS_NUM - 1);

	Transform2D* transform = obj->GetTransform();

	// �ړ�������߂�
	const D3DXVECTOR2* moveTo = obj->GetMoveTo();
	randingPos = moveTo[dist(engine)];

	// �W�����v�O�̌Œ�ꏊ
	originPos = transform->GetPos();

	// �W�����v�̕��������߂�
	jumpDir.x = 0.0f;
	jumpDir.y = (0.0f - transform->GetSize().y / 2) - transform->GetPos().y;

	frameCnt = 0;

	jump = true;
	randing = false;
}

void BeanDemonMove::CleanUp()
{
}

void BeanDemonMove::OnUpdate()
{
	if (jump)
	{
		++frameCnt;
		Jump();

		if (frameCnt >= jumpFrame)
		{	// ���ɔ�񂾂���
			jump = false;
			randing = true;
			frameCnt = 0;

			originPos = obj->GetTransform()->GetPos();
			// �ړ��̕��������߂�
			randingDir.x = randingPos.x - originPos.x;
			randingDir.y = randingPos.y - originPos.y;
		}

		return;
	}
	else if (randing)
	{
		++frameCnt;
		Randing();

		if (frameCnt >= randingFrame)
		{	// ���n�����ꍇ
			randing = false;
			frameCnt = 0;
			originPos = obj->GetTransform()->GetPos();

			ChangeRequest("Attack");
		}
		return;
	}
}

void BeanDemonMove::OnDraw()
{
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	DrawSpriteColor(obj->idleTex,
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
}

float BeanDemonMove::easeOutQuint(float t)
{
	return 1 - powf(1- t, 5);
}

void BeanDemonMove::Jump()
{
	Transform2D* transform = obj->GetTransform();

	float time = (float)frameCnt / jumpFrame;
	float ease = easeOutQuint(time);
	float y = originPos.y + jumpDir.y * ease;

	transform->SetPos(transform->GetPos().x, y);
}

void BeanDemonMove::Randing()
{
	Transform2D* transform = obj->GetTransform();

	float time = (float)frameCnt /randingFrame;
	float ease = easeOutQuint(time);
	float x = originPos.x + randingDir.x * ease;
	float y = originPos.y + randingDir.y * ease;

	transform->SetPos(x, y);
}
