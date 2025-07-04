#pragma once

#include <typeinfo>


class Component
{
public:
	// ======================
	// 実装を強制しません
	// ======================
	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void UnInit() {};

	// ======================
	// 実装の際はtypeid()の引数を自クラスにしてください
	// ======================
	virtual bool IsType(const std::type_info& type) const {
		return type == typeid(Component);
	}

};

