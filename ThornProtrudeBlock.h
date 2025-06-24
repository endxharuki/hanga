#pragma once

#ifndef _THORN_PROTRUDE_BLOCK_H_
#define _THORN_PROTRUDE_BLOCK_H_

#include "GameObject.h"
#include "ThornProtrudeBlockIdle.h"
#include "Colider2D.h"
#include "ColiderPool.h"

class ThornProtrudeBlock : public GameObject
{
public:
	ThornProtrudeBlock(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	{
		// 左上座標から中心座標を格納
		transform.SetPos(pos.x, pos.y);
		transform.SetSize(size);

		// コンポーネントの追加
		componentPool.Add(new ColiderPool());
		
		// コリジョンの追加
		GetComponent<ColiderPool>()->Add(
			Colider2D(&transform,
				D3DXVECTOR2(0.0f, 0.0f),
				transform.GetSize()
		));

		// 針のダメージ判定
		GetComponent<ColiderPool>()->Add(
			Colider2D(&transform,
				D3DXVECTOR2(0.0f, transform.GetSize().y * 0.1f),
				D3DXVECTOR2(transform.GetSize().x, transform.GetSize().y * 0.8f),
				"Damage")
		);

		stateMachine.SceneRegister(IdleKey, std::make_shared<ThornProtrudeBlockIdle>(IdleKey, this));
	}
	ThornProtrudeBlock(D3DXVECTOR2 pos) : ThornProtrudeBlock(pos, D3DXVECTOR2(100.0f, 100.0f)) {}
	//ThornProtrudeBlock(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	//{
	//	componentPool.Add(new ColiderPool());
	//	componentPool.Add(new Enemmy());

	//	transform.SetSize(size);

	//	// コリジョンの生成
	//	GetComponent<ColiderPool>()->Add(Colider2D(
	//		&transform,
	//		D3DXVECTOR2(0.0f, 0.0f),
	//		D3DXVECTOR2(size.x * 0.6f, transform.GetSize().y)
	//	));

	//	stateMachine.SceneRegister(IdleKey, std::make_shared<BulletSwingEnemmyIdle>(IdleKey, this, pos));
	//}

	~ThornProtrudeBlock()
	{

	}

	void Start() override
	{
		// 開始シーンの指定
		stateMachine.SetStartState(IdleKey);
	}

	void Update() override
	{
		stateMachine.Update();
	}

	void Draw() override
	{
		stateMachine.Draw();
	}

private:
	const std::string IdleKey = "Idle";
};

#endif // !_LAND_BLOCK_H_

