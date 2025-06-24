#pragma once

#include <typeinfo>


class Component
{
public:
	// ======================
	// �������������܂���
	// ======================
	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void UnInit() {};

	// ======================
	// �����̍ۂ�typeid()�̈��������N���X�ɂ��Ă�������
	// ======================
	virtual bool IsType(const std::type_info& type) const {
		return type == typeid(Component);
	}

};

