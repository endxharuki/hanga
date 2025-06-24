#pragma once


#include "Component.h"
#include "GameObject.h"
#include "MainInGame.h"
#include "sound.h"

class Break : public Component
{
public:
	Break(GameObject* obj):object(obj) {};
	~Break() {};

	void SetEffect(EffectObj set) {
		_effect = new EffectObj(set);
	}

	bool IsType(const std::type_info& type) const override {
		return type == typeid(Break);
	}

	void BreakBlock() {
		if (_effect != nullptr)
		{
			_effect->pos = object->GetTransform()->GetPos();
  			MainInGame::effectManager.Add(*_effect);
			se = LoadSound((char*)"data/SE/wallbreak.wav");
			PlaySound(se, 0);
		}
		MainInGame::objectPool.DeleteRequest(object);
	}

private:
	GameObject* object;
	EffectObj* _effect = nullptr;

	unsigned int se;
};
