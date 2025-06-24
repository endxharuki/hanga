// ###############################################
// 2Dゲーム座標設定 [Transform2D.h]
// Author: 西窪 統吉桜	Date: 2023/07/06
// ###############################################

#pragma once

#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "main.h"
#include "Component.h"


class Transform2D : public Component
{
public:
	Transform2D() {
		pos = vel = outVel = D3DXVECTOR2(0.0f, 0.0f);
		size = D3DXVECTOR2(100.0f, 100.0f);
		scaling = D3DXVECTOR2(1.0f, 1.0f);
		color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	~Transform2D() {}


	bool IsType(const std::type_info& type) const override{
		return type == typeid(Transform2D);
	}


	// ゲッター
	D3DXVECTOR2 GetPos() { return pos; }
	D3DXVECTOR2 GetVel() { return vel; }
	D3DXVECTOR2 GetOutVel() { return outVel; }
	D3DXVECTOR2 GetSize() { return size; }
	D3DXCOLOR GetColor() { return color; }
	float GetGravity() { return gravityPow; }
	float GetRotation() { return rotate; }
	float GetRotationVel() { return rotationVel; }

	// セッター
	void SetPos(float x, float y) { SetPos(D3DXVECTOR2(x, y)); }
	void SetPos(D3DXVECTOR2 _pos) { pos = _pos; }

	void SetVel(float x, float y) { SetVel(D3DXVECTOR2(x, y)); }
	void SetVel(D3DXVECTOR2 _vel) { vel = _vel; }

	void SetOutVel(float x, float y) { SetOutVel(D3DXVECTOR2(x, y)); }
	void SetOutVel(D3DXVECTOR2 _outVel) { outVel = _outVel; }

	void SetGravity(float set) { gravityPow = set; }

	void SetSize(float x, float y) { SetSize(D3DXVECTOR2(x, y)); }
	void SetSize(D3DXVECTOR2 _size) { size = _size; }

	void SetColor(float r, float g, float b, float a) { SetColor(D3DXCOLOR(r, g, b, a)); }
	void SetColor(D3DXCOLOR _color) { color = _color; }

	void SetRotation(float _rotation) { rotate = _rotation; }
	void SetRotationVel(float _vel) { rotationVel = _vel; }

private:

	// 基礎パラメータ
	D3DXVECTOR2 pos;		// 中心座標
	D3DXVECTOR2 vel;		// 速度
	D3DXVECTOR2 outVel;		// 外的要因からなる速度
	D3DXVECTOR2 size;	// 横、縦幅

	float rotate;		// 回転
	float rotationVel;	// 回転速度
	D3DXVECTOR2 scaling;		// 大きさ
	D3DXCOLOR color;		// 色データ

	// 物理関連パラメータ
	float gravityPow;	// かかる重力
	float mass;			// 重さ (1.0で重力そのまま)
};

#endif // !_TRANSFORM_H_

