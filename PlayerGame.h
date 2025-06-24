#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "DirectWrite.h"
#include "ColiderPool.h"
#include "Pushed.h"
#include "Damaged.h"

#include <chrono>

class PlayerGame: public StateBase
{
public:
	PlayerGame(std::string _key, GameObject* _obj)
		: StateBase(_key), obj(_obj) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void Jump();
	void ColidBlock();
	void ColidLayer();
	void ColidPush(GameObject* target, Pushed* pushed);
	void ColidObject();
	void ColidBlockLine();

	GameObject* obj;
	D3DXVECTOR2 prevColidPos;
	bool isDie = false;


	ColiderPool* colidPool;
	Damaged* damage;

	std::chrono::milliseconds firstSec;
	std::chrono::milliseconds prevFlash;
	bool prevDamage;


	// �e�N�X�`��
	unsigned int idleTex[2];
	unsigned int runTex[2];
	int drawMode = 0;
	
	// ����pUV�l
	float rU, rV, rUW, rVH;

	// �A�j���[�V�����p
	int currentRunTex = 0;

	// font
	FontData* debugData;
	DirectWrite* write;

	// �t���O
	bool isGround;
	bool isJump;
	bool isGrab;
	bool pushJumpKey;
	bool pushGrabKey;
	bool drawPlayer = true;

	// ���]�p�ɑO�t���[���̔��]�󋵂�ۊ�
	bool prevHasSwap;
	bool requestInputSwap = false;
	bool prevInput = false;

	// �W�����v
	int currentJumpFrame = 0;
	static const int jumpFrame = 25;
	const float maxJumpHeight = 13.0f;
	float currentJumpHeight;

	// �ړ�
	float moveDir = 1.0f;
	float moveSign = 1.0f;
	bool isRun = false;

};

