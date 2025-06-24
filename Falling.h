// ########################################
// プレイヤー設置ブロック: 落下中 [Falling.h]
// Author: 西窪 統吉桜	Date: 2023/09/08
// ########################################

#pragma once

#include "StateBase.h"
#include "GameObject.h"

class Falling: public StateBase
{
public:
	Falling(std::string _key, GameObject* _obj, D3DXVECTOR2 _setPos, bool* _request)
		: StateBase(_key), obj(_obj), setPos(_setPos), request(_request) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void Landing();

	void LeftMove();
	void RightMove();
	void LeftRotate();
	void RightRotate();

	void ColidBlock();		// 着地を検知して処理をする
	bool ColidAllOut();		// すべてのコリジョンが外に出たことを検知する
	bool ColidBlockZone(); // 衝突したことを検知するだけ
	bool ColidSetBlockZone(); // 設置済みブロックと衝突したことを検知する
	bool ColidOutZone();	// コリジョンが外に出ているか検知する

	GameObject* obj;
	unsigned int block_tex;

	bool* request;

	D3DXVECTOR2 setPos;
	D3DXVECTOR2 prevPos;

	std::vector<D3DXVECTOR2> prevColidPos;


};

