#pragma once

#include "main.h"
#include "Component.h"
#include "OnGameData.h"

class Damaged : public Component
{
public:
	Damaged() {
		isDamage = false;
	};
	~Damaged() {};

	bool IsType(const std::type_info& type) const override {
		return type == typeid(Damaged);
	}

	void Damage(int damage)
	{
		if (isDamage)return;
		for (int i = 0; i < damage; i++)OnGameData::GetInstance()->SubPlayerLife();
		isDamage = true;
	}

	bool IsDamage() { return isDamage; }
	void SetDamage(bool set) { isDamage = set; }

private:
	bool isDamage = false;
};

