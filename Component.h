#pragma once

#include <typeinfo>


class Component
{
public:
	// ======================
	// À‘•‚ğ‹­§‚µ‚Ü‚¹‚ñ
	// ======================
	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void UnInit() {};

	// ======================
	// À‘•‚ÌÛ‚Ítypeid()‚Ìˆø”‚ğ©ƒNƒ‰ƒX‚É‚µ‚Ä‚­‚¾‚³‚¢
	// ======================
	virtual bool IsType(const std::type_info& type) const {
		return type == typeid(Component);
	}

};

