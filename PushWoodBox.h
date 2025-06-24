#pragma once

#include "Pushed.h"
#include "Transform2D.h"
#include "GameObject.h"

class PushWoodBox : public Pushed
{
public:
	PushWoodBox(GameObject* setObj) :obj(setObj) {};
	~PushWoodBox() {};

	D3DXVECTOR2 Push(D3DXVECTOR2 move)override;

private:
	GameObject* obj;
	Transform2D* transform;
};
