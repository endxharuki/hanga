#pragma once
#include <vector>
#include "Component.h"
#include "Colider2D.h"
class Enemmy : public Component
{
public:
	Enemmy() {};
	~Enemmy() {};

	bool IsType(const std::type_info& type) const override {
		return type == typeid(Enemmy);
	}

private:
};
