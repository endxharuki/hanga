#pragma once

#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "GameObject.h"
#include "SwitchIdle.h"
#include "Colider2D.h"
#include "ColiderPool.h"

__declspec(selectany) bool isSwitch = false;
static void SetCommonSwitch(bool isswitch) { isSwitch = isswitch; }
static bool GetCommonSwitch() { return isSwitch; }

class Switch : public GameObject
{
public:
	Switch(D3DXVECTOR2 pos, D3DXVECTOR2 size,int vec)
	{
		dir = vec;

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
		));//判定

		


		stateMachine.SceneRegister(IdleKey, std::make_shared<SwitchIdle>(IdleKey, this,vec));
	}
	Switch(D3DXVECTOR2 pos) : Switch(pos, D3DXVECTOR2(100.0f, 100.0f),0) {}
	

	~Switch()
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

	int GetDir() { return dir; }
	void SetDir(int set) { dir = set; }

private:
	const std::string IdleKey = "Idle";
	int dir;
};

#endif // !_LAND_BLOCK_H_

