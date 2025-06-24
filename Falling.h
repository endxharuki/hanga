// ########################################
// �v���C���[�ݒu�u���b�N: ������ [Falling.h]
// Author: ���E ���g��	Date: 2023/09/08
// ########################################

#pragma once

#include "StateBase.h"
#include "GameObject.h"

class Falling: public StateBase
{
public:
	Falling(std::string _key, GameObject* _obj, D3DXVECTOR2 _setPos, bool* _request)
		: StateBase(_key), obj(_obj), setPos(_setPos), request(_request) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void Landing();

	void LeftMove();
	void RightMove();
	void LeftRotate();
	void RightRotate();

	void ColidBlock();		// ���n�����m���ď���������
	bool ColidAllOut();		// ���ׂẴR���W�������O�ɏo�����Ƃ����m����
	bool ColidBlockZone(); // �Փ˂������Ƃ����m���邾��
	bool ColidSetBlockZone(); // �ݒu�ς݃u���b�N�ƏՓ˂������Ƃ����m����
	bool ColidOutZone();	// �R���W�������O�ɏo�Ă��邩���m����

	GameObject* obj;
	unsigned int block_tex;

	bool* request;

	D3DXVECTOR2 setPos;
	D3DXVECTOR2 prevPos;

	std::vector<D3DXVECTOR2> prevColidPos;


};

