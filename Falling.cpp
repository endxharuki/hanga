// ========================================
// �v���C���[�ݒu�u���b�N: ������ [Falling.cpp]
// Author: ���E ���g��	Date: 2023/09/08
// ========================================
#include "Falling.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"
#include "BlockMap.h"
#include "MapManager.h"


void Falling::SetUp()
{
	CleanRequest();

	block_tex = NULL;

	obj->GetTransform()->SetSize(0, 0);
	obj->GetTransform()->SetPos(setPos);
	prevPos = obj->GetTransform()->GetPos();
	obj->GetTransform()->SetGravity(0.5f);
	obj->GetTransform()->SetRotation(6.18 / 4 * 4);
	obj->GetTransform()->SetRotationVel(0.0f);

	// �R���W�����̑O�t���[���ʒu��������
	for (int i = 0; i < obj->GetColider().size(); i++)
		prevColidPos.push_back(obj->GetColider()[i].GetPos());
}

void Falling::CleanUp()
{

}

void Falling::OnUpdate()
{
	Transform2D* transform = obj->GetTransform();
	transform->SetVel(transform->GetVel().x, transform->GetVel().y + 1.0f);
	
	// todo: �u���b�N�ړ������ƃu���b�N���n����������

	// �ړ�����
	if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFT))
		LeftMove();
	if (GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHT))
		RightMove();
	if (GetKeyboardRepeat(DIK_S) || GetKeyboardRepeat(DIK_DOWN))
		transform->SetPos(transform->GetPos().x, transform->GetPos().y + 6.5f);
	if (GetKeyboardTrigger(DIK_E))
		RightRotate();
	else if (GetKeyboardTrigger(DIK_Q))
		LeftRotate();

	obj->GetColider();

	// �u���b�N�Ƃ̐ڐG����
	ColidBlock();

	// �O�ɏo����
	if (ColidAllOut())
	{
		// �폜�\�����o��
		*request = true;
	}

	D3DXVECTOR2 pos =  transform->GetPos();
	transform->SetPos(pos.x, pos.y + 1.0f);

	// �O�t���[���̈ʒu���i�[
	prevPos = transform->GetPos();

	for (int i = 0; i < obj->GetColider().size(); i++)
		prevColidPos.push_back(obj->GetColider()[i].GetPos());
}

void Falling::OnDraw()
{
	D3DXVECTOR2 pos =  obj->GetTransform()->GetPos();

	/*DrawSpriteColorRotate(block_tex,
		pos.x, pos.y,
		obj->GetTransform()->GetSize().x, obj->GetTransform()->GetSize().y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		obj->GetTransform()->GetRotation());*/

	// �f�o�b�O�p
	std::vector<Colider2D> colids = obj->GetColider();
	for (int i = 0; i < colids.size(); i++)
	{
		colids[i].viewColid( D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.3f) );
	}
}


void Falling::ColidBlock()
{
	//�@������
	std::vector<Colider2D> colids = obj->GetColider();
	std::vector<GameObject*>& map = BlockMap::GetInstance()->GetMap();
	CUR_OBJ_MAP* setMap = MapManager::GetInstance()->GetObj();

	bool isFriction = false;

	D3DXVECTOR2 frictionColidPos;
	D3DXVECTOR2	frictionColidSize;
	D3DXVECTOR2 frictionBlockPos;
	D3DXVECTOR2 frictionBlockSize;

	D3DXVECTOR2 prevFrictionColid;

	// ==============
	// �X�e�[�W�Փ˔���
	// ==============
	for (int colidNum = 0; colidNum < colids.size(); colidNum++)
	{
		D3DXVECTOR2 pos = colids[colidNum].GetPos();
		D3DXVECTOR2 size = colids[colidNum].GetSize();


		//�R���W�����̊p�ݒ�
		D3DXVECTOR2 colidVtx[4] =
		{
			{pos.x - size.x / 2, pos.y - size.y / 2},	// ����
			{pos.x + size.x / 2, pos.y - size.y / 2},	// �E��
			{pos.x + size.x / 2, pos.y + size.y / 2},	// �E��
			{pos.x - size.x / 2, pos.y + size.y / 2}	// ����
		};


		// ==============
		// �Փ˔���
		// ==============
		for (int i = 0; i < map.size(); i++)
		{
			D3DXVECTOR2 BlockPos = map[i]->GetColider()[0].GetPos();
			D3DXVECTOR2 BlockSize = map[i]->GetColider()[0].GetSize();

			// �u���b�N�̊e��
			D3DXVECTOR2 BlockVtx[4] =
			{
				{BlockPos.x - BlockSize.x / 2, BlockPos.y - BlockSize.y / 2},	// ����
				{BlockPos.x + BlockSize.x / 2, BlockPos.y - BlockSize.y / 2},	// �E��
				{BlockPos.x + BlockSize.x / 2, BlockPos.y + BlockSize.y / 2},	// �E��
				{BlockPos.x - BlockSize.x / 2, BlockPos.y + BlockSize.y / 2}	// ����
			};

			D3DXVECTOR2 distance = D3DXVECTOR2(fabsf(pos.x - BlockPos.x), fabsf(pos.y - BlockPos.y));
			D3DXVECTOR2 sumSize = D3DXVECTOR2((size.x + BlockSize.x) / 2.0f, (size.y + BlockSize.y) / 2.0f);

			if (distance.x < sumSize.x &&
				distance.y < sumSize.y)
			{	// �Փˎ�
				isFriction = true;
				frictionColidPos = pos;
				frictionColidSize = size;
				frictionBlockPos = BlockPos;
				frictionBlockSize = BlockSize;

				prevFrictionColid = prevColidPos[colidNum];

				break;
			}
		} // �Փ˔���I��
	}

	// ==============
	// �ݒu�u���b�N�Փ˔���
	// ==============
	for (int colidNum = 0; colidNum < colids.size(); colidNum++)
	{
		D3DXVECTOR2 pos = colids[colidNum].GetPos();
		D3DXVECTOR2 size = colids[colidNum].GetSize();


		//�R���W�����̊p�ݒ�
		D3DXVECTOR2 colidVtx[4] =
		{
			{pos.x - size.x / 2, pos.y - size.y / 2},	// ����
			{pos.x + size.x / 2, pos.y - size.y / 2},	// �E��
			{pos.x + size.x / 2, pos.y + size.y / 2},	// �E��
			{pos.x - size.x / 2, pos.y + size.y / 2}	// ����
		};


		// ==============
		// �Փ˔���
		// ==============
		for (int yCnt = 0; yCnt < ONE_SCREEN_COLUMN && isFriction == false; yCnt++)
		{
			for (int xCnt = 0; xCnt < ONE_SCREEN_RAW; xCnt++)
			{
				if (setMap->objMap[yCnt][xCnt] == nullptr)continue;

				D3DXVECTOR2 BlockPos = setMap->objMap[yCnt][xCnt]->GetColider()[0].GetPos();
				D3DXVECTOR2 BlockSize = setMap->objMap[yCnt][xCnt]->GetColider()[0].GetSize();

				// �u���b�N�̊e��
				D3DXVECTOR2 BlockVtx[4] =
				{
					{BlockPos.x - BlockSize.x / 2, BlockPos.y - BlockSize.y / 2},	// ����
					{BlockPos.x + BlockSize.x / 2, BlockPos.y - BlockSize.y / 2},	// �E��
					{BlockPos.x + BlockSize.x / 2, BlockPos.y + BlockSize.y / 2},	// �E��
					{BlockPos.x - BlockSize.x / 2, BlockPos.y + BlockSize.y / 2}	// ����
				};

				D3DXVECTOR2 distance = D3DXVECTOR2(fabsf(pos.x - BlockPos.x), fabsf(pos.y - BlockPos.y));
				D3DXVECTOR2 sumSize = D3DXVECTOR2((size.x + BlockSize.x) / 2.0f, (size.y + BlockSize.y) / 2.0f);

				if (distance.x < sumSize.x &&
					distance.y < sumSize.y)
				{	// �Փˎ�
					isFriction = true;
					frictionColidPos = pos;
					frictionColidSize = size;
					frictionBlockPos = BlockPos;
					frictionBlockSize = BlockSize;

					prevFrictionColid = prevColidPos[colidNum];

					break;
				}
			}
		}// �Փ˔���I��

	}

	if (!isFriction)return;


	// ==============
	// �Փ˕ӂ̊m�F
	// ==============
	float playerTop = frictionColidPos.y - frictionColidSize.y / 2;
	float playerBottom = frictionColidPos.y + frictionColidSize.y / 2;
	float playerLeft = frictionColidPos.x - frictionColidSize.x / 2;
	float playerRight = frictionColidPos.x + frictionColidSize.x / 2;

	// todo: prev���R���W�����ɑΉ�������
	float playerPrevTop = prevFrictionColid.y - frictionColidSize.y / 2;
	float playerPrevBottom = prevFrictionColid.y + frictionColidSize.y / 2;
	float playerPrevLeft = prevFrictionColid.x - frictionColidSize.x / 2;
	float playerPrevRight = prevFrictionColid.x + frictionColidSize.x / 2;

	float blockTop = frictionBlockPos.y - frictionBlockSize.y / 2;
	float blockBottom = frictionBlockPos.y + frictionBlockSize.y / 2;
	float blockLeft = frictionBlockPos.x - frictionBlockSize.x / 2;
	float blockRight = frictionBlockPos.x + frictionBlockSize.x / 2;

	// �ӂ̔���
	if (playerPrevBottom <= blockTop && playerTop <= blockTop && playerBottom >= blockTop)
	{	// �u���b�N�̏�ɏՓ�
		Landing();
	}


	
}


bool Falling::ColidBlockZone()
{
	std::vector<Colider2D> colids = obj->GetColider();
	std::vector<GameObject*>& map = BlockMap::GetInstance()->GetMap();

	for (int colidNum = 0; colidNum < colids.size(); colidNum++)
	{
		D3DXVECTOR2 pos = colids[colidNum].GetPos();
		D3DXVECTOR2 size = colids[colidNum].GetSize();


		//�R���W�����̊p�ݒ�
		D3DXVECTOR2 colidVtx[4] =
		{
			{pos.x - size.x / 2, pos.y - size.y / 2},	// ����
			{pos.x + size.x / 2, pos.y - size.y / 2},	// �E��
			{pos.x + size.x / 2, pos.y + size.y / 2},	// �E��
			{pos.x - size.x / 2, pos.y + size.y / 2}	// ����
		};


		// ==============
		// �Փ˔���
		// ==============
		for (int i = 0; i < map.size(); i++)
		{
			D3DXVECTOR2 BlockPos = map[i]->GetColider()[0].GetPos();
			D3DXVECTOR2 BlockSize = map[i]->GetColider()[0].GetSize();

			// �u���b�N�̊e��
			D3DXVECTOR2 BlockVtx[4] =
			{
				{BlockPos.x - BlockSize.x / 2, BlockPos.y - BlockSize.y / 2},	// ����
				{BlockPos.x + BlockSize.x / 2, BlockPos.y - BlockSize.y / 2},	// �E��
				{BlockPos.x + BlockSize.x / 2, BlockPos.y + BlockSize.y / 2},	// �E��
				{BlockPos.x - BlockSize.x / 2, BlockPos.y + BlockSize.y / 2}	// ����
			};

			D3DXVECTOR2 distance = D3DXVECTOR2(fabsf(pos.x - BlockPos.x), fabsf(pos.y - BlockPos.y));
			D3DXVECTOR2 sumSize = D3DXVECTOR2((size.x + BlockSize.x) / 2.0f, (size.y + BlockSize.y) / 2.0f);

			if (distance.x < sumSize.x &&
				distance.y < sumSize.y)
			{	// �Փˎ�
				return true;
			}
		} // �Փ˔���I��
	}

	return false;
}


bool Falling::ColidSetBlockZone()
{
	//�@������
	std::vector<Colider2D> colids = obj->GetColider();
	std::vector<GameObject*>& map = BlockMap::GetInstance()->GetMap();
	CUR_OBJ_MAP* setMap = MapManager::GetInstance()->GetObj();


	// ==============
	// �ݒu�u���b�N�Փ˔���
	// ==============
	for (int colidNum = 0; colidNum < colids.size(); colidNum++)
	{
		D3DXVECTOR2 pos = colids[colidNum].GetPos();
		D3DXVECTOR2 size = colids[colidNum].GetSize();


		//�R���W�����̊p�ݒ�
		D3DXVECTOR2 colidVtx[4] =
		{
			{pos.x - size.x / 2, pos.y - size.y / 2},	// ����
			{pos.x + size.x / 2, pos.y - size.y / 2},	// �E��
			{pos.x + size.x / 2, pos.y + size.y / 2},	// �E��
			{pos.x - size.x / 2, pos.y + size.y / 2}	// ����
		};


		// ==============
		// �Փ˔���
		// ==============
		for (int yCnt = 0; yCnt < ONE_SCREEN_COLUMN; yCnt++)
		{
			for (int xCnt = 0; xCnt < ONE_SCREEN_RAW; xCnt++)
			{
				if (setMap->objMap[yCnt][xCnt] == nullptr)continue;

				D3DXVECTOR2 BlockPos = setMap->objMap[yCnt][xCnt]->GetColider()[0].GetPos();
				D3DXVECTOR2 BlockSize = setMap->objMap[yCnt][xCnt]->GetColider()[0].GetSize();

				// �u���b�N�̊e��
				D3DXVECTOR2 BlockVtx[4] =
				{
					{BlockPos.x - BlockSize.x / 2, BlockPos.y - BlockSize.y / 2},	// ����
					{BlockPos.x + BlockSize.x / 2, BlockPos.y - BlockSize.y / 2},	// �E��
					{BlockPos.x + BlockSize.x / 2, BlockPos.y + BlockSize.y / 2},	// �E��
					{BlockPos.x - BlockSize.x / 2, BlockPos.y + BlockSize.y / 2}	// ����
				};

				D3DXVECTOR2 distance = D3DXVECTOR2(fabsf(pos.x - BlockPos.x), fabsf(pos.y - BlockPos.y));
				D3DXVECTOR2 sumSize = D3DXVECTOR2((size.x + BlockSize.x) / 2.0f, (size.y + BlockSize.y) / 2.0f);

				if (distance.x < sumSize.x &&
					distance.y < sumSize.y)
				{	// �Փˎ�
					return true;
				}
			}
		}// �Փ˔���I��

	}
	return false;
}

bool Falling::ColidOutZone()
{
	// ������
	std::vector<Colider2D> colids = obj->GetColider();

	// �O�ɏo�Ă��邩���f
	for (int i = 0; i < colids.size(); i++)
	{
		D3DXVECTOR2 pos = colids[i].GetPos();
		D3DXVECTOR2 size = colids[i].GetSize();
		float leftLine = pos.x - size.x / 2;
		float rightLine = pos.x + size.x / 2;

		// ���E�ǂ��炩�̕ӂ��O�ɏo�Ă�����^��Ԃ�
		if (leftLine < 0 || rightLine > SCREEN_WIDTH)
			return true;
	}

	return false;
}


void Falling::Landing()
{
	// todo: ���W���m���}�b�v�Ɏ��g�𔽉f�����鏈��������
	std::vector<Colider2D> colids = obj->GetColider();

	MapManager* manager = MapManager::GetInstance();

	// �R���W�������ƂɊi�[���Ă���
	for (int i = 0; i < colids.size(); i++)
	{
		D3DXVECTOR2 colidPos = colids[i].GetPos();

		int x = colidPos.x / CHIP_X_SIZE;
		int y = (colidPos.y - (SCREEN_HEIGHT - MAP_HEIGHT) ) / CHIP_Y_SIZE;

		// �u���b�N�i�[
		manager->SetBlock(x, y);
	}

	// �폜�\�����o��
	*request = true;
}


void Falling::LeftMove()
{
	// ������
	Transform2D* trans = obj->GetTransform();
	D3DXVECTOR2 firstPos = trans->GetPos();
	std::vector<Colider2D> colids = obj->GetColider();
	CUR_OBJ_MAP* setMap = MapManager::GetInstance()->GetObj();

	// �ړ���̍��W�����߂Ċi�[
	D3DXVECTOR2 movedPos = D3DXVECTOR2(firstPos.x - CHIP_X_SIZE, firstPos.y);
	trans->SetPos(movedPos);

	// ���ꂼ��̓����蔻�肪�O�ɏo�Ă��Ȃ����m�F, �O�ɂ���ꍇ�����ʒu�ɖ߂�
	for (int i = 0; i < colids.size(); i++)
	{
		D3DXVECTOR2  colidPos = colids[i].GetPos();
		D3DXVECTOR2 colidSize = colids[i].GetSize();

		int x = colidPos.x / CHIP_X_SIZE;
		int y = (colidPos.y - (SCREEN_HEIGHT - MAP_HEIGHT)) / CHIP_Y_SIZE;

		// ��ʊO����
		if (colidPos.x - colidSize.x / 2 < 0)
		{	// �O�ɏo�Ă����ꍇ
			trans->SetPos(firstPos);
			return;
		}


		// �ݒu�u���b�N����
		for (int yCnt = 0; yCnt < ONE_SCREEN_COLUMN; yCnt++)
		{
			for (int xCnt = 0; xCnt < ONE_SCREEN_RAW; xCnt++)
			{
				if (setMap->objMap[yCnt][xCnt] != nullptr && x == xCnt && y == yCnt)
				{	// �ړ���Ƀu���b�N������ꍇ
					trans->SetPos(firstPos);
					return;
				}
			}
		}
	}

	if (ColidBlockZone())
	{	// �ړ���ɃX�e�[�W�u���b�N������ꍇ
		trans->SetPos(firstPos);
		return;
	}
}


void Falling::RightMove()
{
	// ������
	Transform2D* trans = obj->GetTransform();
	D3DXVECTOR2 firstPos = trans->GetPos();
	std::vector<Colider2D> colids = obj->GetColider();
	CUR_OBJ_MAP* setMap = MapManager::GetInstance()->GetObj();

	// �ړ���̍��W�����߂Ċi�[
	D3DXVECTOR2 movedPos = D3DXVECTOR2(firstPos.x + CHIP_X_SIZE, firstPos.y);
	trans->SetPos(movedPos);

	// ���ꂼ��̓����蔻�肪�O�ɏo�Ă��Ȃ����m�F, �O�ɂ���ꍇ�����ʒu�ɖ߂�
	for (int i = 0; i < colids.size(); i++)
	{
		D3DXVECTOR2  colidPos = colids[i].GetPos();
		D3DXVECTOR2 colidSize = colids[i].GetSize();

		int x = colidPos.x / CHIP_X_SIZE;
		int y = (colidPos.y - (SCREEN_HEIGHT - MAP_HEIGHT)) / CHIP_Y_SIZE;

		if (colidPos.x + colidSize.x / 2 > SCREEN_WIDTH)
		{	// �O�ɏo�Ă����ꍇ
			trans->SetPos(firstPos);
			return;
		}

		// �ݒu�u���b�N����
		for (int yCnt = 0; yCnt < ONE_SCREEN_COLUMN; yCnt++)
		{
			for (int xCnt = 0; xCnt < ONE_SCREEN_RAW; xCnt++)
			{
				if (setMap->objMap[yCnt][xCnt] != nullptr && x == xCnt && y == yCnt)
				{	// �ړ���Ƀu���b�N������ꍇ
					trans->SetPos(firstPos);
					return;
				}
			}
		}
	}


	if (ColidBlockZone())
	{	// �ړ���ɃX�e�[�W�u���b�N������ꍇ
		trans->SetPos(firstPos);
		return;
	}
}


void Falling::LeftRotate()
{
	std::vector<Colider2D>& colids = obj->GetColider();

	const float wideLen = CHIP_X_SIZE * 3;
	const float hightLen = CHIP_Y_SIZE * 3;

	// ���̃I�t�Z�b�g�ʒu�ƃT�C�Y���L�^����
	std::vector<D3DXVECTOR2> tmpOffset;
	for (int i = 0; i < colids.size(); i++)
	{
		tmpOffset.push_back(colids[i].GetOffset());
	}

	// ��]����
	for (int i = 0; i < colids.size(); i++)
	{
		D3DXVECTOR2 offset = colids[i].GetOffset();
		offset.x += wideLen / 2;
		offset.y += hightLen / 2;
		int x = offset.x / CHIP_X_SIZE;
		int y = offset.y / CHIP_Y_SIZE;

		int rotateX = y;
		int rotateY = 2 - x;

		D3DXVECTOR2 set;
		set.x = rotateX * CHIP_X_SIZE - wideLen / 2 + CHIP_X_SIZE / 2;
		set.y = rotateY * CHIP_Y_SIZE - hightLen / 2 + CHIP_Y_SIZE / 2;

		colids[i].SetOffSet(set);

	}

	if (ColidBlockZone() || ColidSetBlockZone() || ColidOutZone())
	{	// ��]��Փ˂����������ꍇ��]�����Ƃɖ߂�
		for (int i = 0; i < colids.size(); i++)
		{
			colids[i].SetOffSet(tmpOffset[i]);
		}
	}

}


void Falling::RightRotate()
{
	std::vector<Colider2D>& colids = obj->GetColider();

	const float wideLen = CHIP_X_SIZE * 3;
	const float hightLen = CHIP_Y_SIZE * 3;

	// ���̃I�t�Z�b�g�ʒu�ƃT�C�Y���L�^����
	std::vector<D3DXVECTOR2> tmpOffset;
	for (int i = 0; i < colids.size(); i++)
	{
		tmpOffset.push_back(colids[i].GetOffset());
	}

	// ��]����
	for (int i = 0; i < colids.size(); i++)
	{
		D3DXVECTOR2 offset = colids[i].GetOffset();
		offset.x += wideLen / 2;
		offset.y += hightLen / 2;
		int x = offset.x / CHIP_X_SIZE;
		int y = offset.y / CHIP_Y_SIZE;

		int rotateX = 2 - y;
		int rotateY = x;

		D3DXVECTOR2 set;
		set.x = rotateX * CHIP_X_SIZE - wideLen / 2 + CHIP_X_SIZE / 2;
		set.y = rotateY * CHIP_Y_SIZE - hightLen / 2 + CHIP_Y_SIZE / 2;

		colids[i].SetOffSet(set);
	
	}

	if (ColidBlockZone() || ColidSetBlockZone() || ColidOutZone())
	{	// ��]��Փ˂����������ꍇ��]�����Ƃɖ߂�
		for (int i = 0; i < colids.size(); i++)
		{
			colids[i].SetOffSet(tmpOffset[i]);
		}
	}

}

bool Falling::ColidAllOut()
{
	std::vector<Colider2D> colids = obj->GetColider();

	for (int i = 0; i < colids.size(); i++)
	{
		D3DXVECTOR2 pos = colids[i].GetPos();
		D3DXVECTOR2 size = colids[i].GetSize();
		float topLine = pos.y - size.y / 2;

		if (topLine < SCREEN_HEIGHT)
			return false;
	}

	return true;
}
