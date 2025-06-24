#pragma once
#include <vector>
#include "Component.h"
#include "Colider2D.h"
class ColiderPool : public Component
{
public:
	ColiderPool() {};
	~ColiderPool() {
		pool.clear();
	};

	bool IsType(const std::type_info& type) const override {
		return type == typeid(ColiderPool);
	}

	void Add(Colider2D add) {
		pool.push_back(add);
	}

	void Clear() {
		pool.clear();
	}

	std::vector<Colider2D>& GetColider() {
		return pool;
	}

private:
	std::vector<Colider2D> pool;
};