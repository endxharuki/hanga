#pragma once
#include "GameObject.h"
#include "Enemmy.h"
#include "ColiderPool.h"
#include "Texture.h"
#include "MainInGame.h"

#include "BeanDemonIdle.h"
#include "BeanDemonMove.h"
#include "BeanDemonAttack.h"
#include "BeanDemonStan.h"
#include "BeanDemonDamaged.h"

#define MOVE_POS_NUM (5)

class BeanDemon : public GameObject
{
public:
	BeanDemon(D3DXVECTOR2 pos) : BeanDemon(pos, D3DXVECTOR2(100.0f, 100.0f)) {}
	BeanDemon(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	{
		// �e�N�X�`���̓ǂݍ���
		idleTex = LoadTexture((char*)"data/TEXTURE/boss.png");
		stanTex = LoadTexture((char*)"data/TEXTURE/bossStan.png");

		componentPool.Add(new ColiderPool());
		componentPool.Add(new Enemmy());

		transform.SetPos(pos);
		transform.SetSize(size);

		// �R���W�����̐���
		ColiderPool* colidPool = GetComponent<ColiderPool>();
		colidPool->Add(Colider2D(
			&transform,
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(size.x * 0.9f, transform.GetSize().y * 0.9f)
		));

		/*colidPool->Add(Colider2D(
			D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
			D3DXVECTOR2(300.0f, SCREEN_HEIGHT)
		));*/

		baseColid = &colidPool->GetColider()[0];
		//startColid = &colidPool->GetColider()[1];


		stateMachine.SceneRegister(Idle, std::make_shared<BeanDemonIdle>(Idle, this));
		stateMachine.SceneRegister(Move, std::make_shared<BeanDemonMove>(Move, this));
		stateMachine.SceneRegister(Attack, std::make_shared<BeanDemonAttack>(Attack, this));
		stateMachine.SceneRegister(Stan, std::make_shared<BeanDemonStan>(Stan, this));
		stateMachine.SceneRegister(Damage, std::make_shared<BeanDemonDamaged>(Damage, this));
	}

	void Start() override
	{
		// �J�n�V�[���̎w��
		stateMachine.SetStartState(Idle);

		D3DXVECTOR2 size = transform.GetSize();

		movePos[0] = D3DXVECTOR2(1920.0f, 600.0f);
		movePos[1] = D3DXVECTOR2(1920.0f + 600.0f, 918 - size.y / 2);
		movePos[2] = D3DXVECTOR2(1920.0f - 600.0f, 918 - size.y / 2);
		movePos[3] = D3DXVECTOR2(1920.0f + 600.0f, 250.0f);
		movePos[4] = D3DXVECTOR2(1920.0f - 600.0f, 250.0f);

		life = maxLife;

		// TODO: �悻�ŃA�N�e�B�u��on�ɂ���
		active = true;
	}

	void Update() override
	{
		if (life <= 0) MainInGame::objectPool.DeleteRequest(this);

		stateMachine.Update();
		overlapPlayerDamage();
		overlapBullet();
	}

	void Draw() override
	{
		stateMachine.Draw();
	}

	void StartActive() {
		active = true;
	}

	bool GetActive() { return active; }
	const D3DXVECTOR2* GetMoveTo() { return movePos; }

	int GetLife() { return life; }
	void SubLife() { --life; }

	void ReturnedStan() {
		isStan = false;
	}

	// �e�N�X�`��
	unsigned int idleTex;
	unsigned int stanTex;

private:
	// ������̃X�e�[�g�����v���C���[�Ƃ̓����蔻��������A�_���[�W��^����悤�ɂ���
	void overlapPlayerDamage();
	void overlapBullet();

	const std::string Idle = "Idle";
	const std::string Attack = "Attack";
	const std::string Move = "Move";
	const std::string Stan = "Stan";
	const std::string Damage = "Damage";

	Colider2D* baseColid;
	Colider2D* startColid;

	D3DXVECTOR2 movePos[5];	// �ړ���

	const int maxLife = 3;
	int life;

	bool active = false;
	bool isStan = false;
	bool damaged = false;
};
