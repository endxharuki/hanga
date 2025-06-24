#pragma once

#include "main.h"
#include "Component.h"

class Pushed : public Component
{
public:
	Pushed() {};
	~Pushed() {};

	bool IsType(const std::type_info& type) const override {
		return type == typeid(Pushed);
	}

	virtual D3DXVECTOR2 Push(D3DXVECTOR2 move) = 0;

private:
};
