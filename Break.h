#pragma once


#include "Component.h"
#include "GameObject.h"

class Break : public Component
{
public:
	Break(GameObject* obj):object(obj) {};
	~Break() {};

	bool IsType(const std::type_info& type) const override {
		return type == typeid(Break);
	}

	void BreakBlock() {
		object->GetTransform()->SetPos(0.0f, 10000.0f);
	}

private:
	GameObject* object;
};
