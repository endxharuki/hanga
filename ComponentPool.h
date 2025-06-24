#pragma once


#include "Component.h"
#include <list>

class ComponentPool
{
public:
	ComponentPool() {};
	~ComponentPool() {
		UnInit();
		pool.clear();
	};

	// 指定したコンポーネントを追加します
	// コンストラクタで追加することを推奨します
	void Add(Component* add) {
		pool.push_back(add);
	}

	std::list<Component*>& GetPool() {
		return pool;
	}

	void Init() {
		for (Component* component : pool)
			component->Init();
	}

	void Update() {
		for (Component* component : pool)
			component->Update();
	}

	void Draw() {
		for (Component* component : pool)
			component->Draw();
	}

	void UnInit() {
		for (Component* component : pool)
		{
			component->UnInit();
			delete component;
		}
	}

private:
	std::list<Component*> pool;
};