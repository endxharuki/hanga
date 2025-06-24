#pragma once

#include <list>
#include "main.h"
#include "sprite.h"
#include "Camera.h"

struct EffectObj
{
	int tex[2];	// ���]���p�ɓ�� ��ڂɔ��]��
	int max_x = 1;	// �E�̏����
	int max_y = 1;	// ���̏����
	int allFrame = 1;	// �A�j���[�V��������
	int speed = 1;	// ���t���[���Ŏ��Ɉڍs���邩
	bool loop = false;	// ���[�v���邩

	D3DXVECTOR2 pos;
	D3DXVECTOR2 size;
	float alpha = 1.0f;

	int frameCnt = 0;	// �t���[���J�E���g
	int frame = 0;	// ���݂̖���
	D3DXVECTOR4 uv;
};

class EffectManager
{
public:
	EffectManager() = default;

	void Clear() {
		_effectList.clear();
		_alphaEffectList.clear();
	}

	void Add(EffectObj obj) { _effectList.push_back(obj); }
	void AlphaAdd(EffectObj obj) { _alphaEffectList.push_back(obj); }

	void OnUpdate() {
		for (EffectObj& obj : _effectList)
		{
			++obj.frameCnt;
			obj.frame = obj.frameCnt / obj.speed;
			obj.uv.x = (float)(obj.frame % obj.max_x * (1.0f / obj.max_x));
			obj.uv.y = (float)(obj.frame / obj.max_x * (1.0f / obj.max_y));
			obj.uv.z = (float)1.0f / obj.max_x;
			obj.uv.w = (float)1.0f / obj.max_y;
		}

		_effectList.remove_if([](const EffectObj& obj) {
			return !obj.loop && obj.frame >= obj.allFrame;
			});


		for (EffectObj& obj : _alphaEffectList)
		{
			++obj.frameCnt;
			obj.alpha = 1.0f - obj.frameCnt / obj.allFrame;
		}
		
		_alphaEffectList.remove_if([](const EffectObj& obj) {
			return obj.frameCnt >= obj.allFrame;
			});

	}

	void OnDraw() const {
		int drawMode = OnGameData::GetInstance()->HasSwap() ? 1 : 0;
		D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

		for (const EffectObj& obj : _effectList)
		{
			D3DXVECTOR2 drawPos = obj.pos - cameraPos;

			DrawSprite(obj.tex[drawMode],
				drawPos.x, drawPos.y,
				obj.size.x, obj.size.y,
				obj.uv.x, obj.uv.y,
				obj.uv.z, obj.uv.w);
		}

		for (const EffectObj& obj : _alphaEffectList)
		{
			D3DXVECTOR2 drawPos = obj.pos - cameraPos;

			DrawSpriteColor(obj.tex[drawMode],
				drawPos.x, drawPos.y,
				obj.size.x, obj.size.y,
				0.0f, 0.0f,
				1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, obj.alpha);
		}
	}

private:
	std::list<EffectObj> _effectList;
	std::list<EffectObj> _alphaEffectList;	// ���߂��Ă��������̃G�t�F�N�g
};