#pragma once

#include "Pushed.h"
#include "Transform2D.h"
#include "GameObject.h"

class PushBox : public Pushed
{
public:
	PushBox(GameObject* setObj):obj(setObj)  {};
	~PushBox() {};

	D3DXVECTOR2 Push(D3DXVECTOR2 move)override;

private:
	GameObject* obj;
	Transform2D* transform;
};
