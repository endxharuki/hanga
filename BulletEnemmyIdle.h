#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "Bullet.h"

class BulletEnemmyIdle : public StateBase
{
public:
	BulletEnemmyIdle(std::string _key, GameObject* _obj, D3DXVECTOR2 _pos)
		: StateBase(_key), obj(_obj) {
		obj->GetTransform()->SetPos(_pos);
	}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	bool ColidBullet();
	void Shot();

	unsigned int tex[2];
	int drawMode;

	bool prevSwap = false;

	bool isDie = false;

	GameObject* obj;
	Bullet* bullet = nullptr;	// ���ݏo�Ă���e���i�[����

	// �e�̔��˂Ɋւ���X�e�[�^�X
	const float shotHeight = 10.0f;	// �e���I�u�W�F�N�g�̂ǂ̍����Ŕ��˂��邩
	const D3DXVECTOR2 bulletSize = D3DXVECTOR2(30.0f, 15.0f);
	const D3DXVECTOR2 bulletVel = D3DXVECTOR2(5.0f, 0.0f);
	float shotDir = -1.0f;
};
