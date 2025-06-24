#pragma once
#include <algorithm>
#include "GameObject.h"


class ObjectPool
{
public:
	ObjectPool() {};
	~ObjectPool() {
	}

	// 指定したオブジェクトを追加します
	void Add(GameObject* obj) {
		objectPool.push_back(obj);
	}

	// 指定したオブジェクトを削除します
	// 返り値: 削除に成功したらtrue、失敗したらfalse
	bool DeleteRequest(GameObject* deleteObj) {

		auto it = std::find(objectPool.begin(), objectPool.end(), deleteObj);

		if (it != objectPool.end())
		{
			deletePool.push_back(deleteObj);
			return true;
		}
		else
		{
			return false;
		}
	}

	void Start() {
		for (GameObject* object : objectPool) object->Start();
	}

	void OnUpdate() {
		for (GameObject* object : objectPool) object->Update();
		
		// 削除リクエストの応答
		auto it = deletePool.begin();
		while (it != deletePool.end())
		{
			auto deleteIt = std::find(objectPool.begin(), objectPool.end(), *it);
			if (deleteIt != objectPool.end())
			{
				delete (*deleteIt);
				objectPool.erase(deleteIt);
			}

			++it;
		}
		deletePool.clear();
	}

	void OnDraw() {
		for (GameObject* object : objectPool) object->Draw();
	}

	void UnInit(){
		for (GameObject* object : objectPool) delete object;
		objectPool.clear();
	}

	int GetSize() {
		return objectPool.size();
	}


	std::list<GameObject*>* GetPool() {
		return &objectPool;
	}


private:
	std::list<GameObject*> objectPool;
	std::list<GameObject*> deletePool;	// 削除リクエスト用配列
};

