#pragma once

#include "main.h"
#include "Component.h"
#include "OnGameData.h"
#include "sound.h"

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
		damageSe = LoadSound((char*)"data/SE/playerdamage.wav");
		PlaySound(damageSe, 0);
		for (int i = 0; i < damage; i++)OnGameData::GetInstance()->SubPlayerLife();
		isDamage = true;
	}

	bool IsDamage() { return isDamage; }
	void SetDamage(bool set) { isDamage = set; }

private:
	bool isDamage = false;
	unsigned int damageSe;
};

