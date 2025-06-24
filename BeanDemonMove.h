#pragma once
#include "StateBase.h"
#include "main.h"

class BeanDemon;
class BeanDemonMove : public StateBase
{
public:
	BeanDemonMove(std::string _key, BeanDemon* _obj)
		: StateBase(_key), obj(_obj) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	// HACK: イージング関数用のライブラリを作ってみてもいい
	float easeOutQuint(float t);

	void Jump();
	void Randing();

	BeanDemon* obj;

	// 反転用
	bool prevHasSwap;
	int drawMode = 0;

	D3DXVECTOR2 originPos;
	D3DXVECTOR2 randingPos;
	D3DXVECTOR2 randingDir;
	D3DXVECTOR2 jumpDir;

	int frameCnt = 0;
	const int jumpFrame = 70;
	const int randingFrame = 90;

	bool jump;
	bool randing;

};

