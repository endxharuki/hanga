// ###############################################
// 2Dコリジョン雛形 [Colider2D.h]
// Author: 西窪 統吉桜	Date: 2023/09/08
// ###############################################

#pragma once

#ifndef _COLIDER_2D_H_
#define _COLIDER_2D_H_
#include "Transform2D.h"
#include "Component.h"
#include <string>

// 当たった方向
enum ColidDir {
	NONE,	// 当たっていない
	RIGHT,
	LEFT,
	UP,
	DOWN
};

class Colider2D : public Component
{
public:
	Colider2D()
		: Colider2D(nullptr, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f)) {}

	Colider2D(Transform2D* _parent)
		: Colider2D(_parent, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(_parent->GetSize().x, _parent->GetSize().y)) {}

	Colider2D(Transform2D* _parent, D3DXVECTOR2 _offset, D3DXVECTOR2 _size) 
	: Colider2D(_parent, _offset, _size, ""){}

	Colider2D(Transform2D* _parent, D3DXVECTOR2 _offset, D3DXVECTOR2 _size, std::string _tag)
		: parent(_parent), offset(_offset), size(_size), pos(_parent->GetPos() + offset), tag(_tag) {
		SetVertex();
	}

	Colider2D(D3DXVECTOR2 _pos, D3DXVECTOR2 _size)
		: offset(D3DXVECTOR2(0.0f, 0.0f)), size(_size), pos(_pos), parent(nullptr){
		SetVertex();
	}

	bool IsType(const std::type_info& type) const override {
		return type == typeid(Colider2D);
	}

	// 当たり判定
	// 指定したオブジェクトが自身のコリジョンと重なっているか確認
	// 返り値: 重なっている場合にtrue、それ以外はfalse
	bool IsColid(Colider2D obj);

	// 当たった方向を返します
	// 指定したオブジェクトが自身のコリジョンが重なっている場合方向を返します
	// 返り値: 重なってない場合NONE, それ以外は方向を返す
	ColidDir IsDirColid(Colider2D obj);

	// 移動
	void SetPos(float x, float y) { SetPos(D3DXVECTOR2(x, y)); }
	void SetPos(const D3DXVECTOR2& set) {
		pos = set;
		if (parent != nullptr) parent->SetPos(pos - offset);
	}

	void Move(float x, float y) { Move(D3DXVECTOR2(x, y)); }
	void Move(const D3DXVECTOR2& move) {
		pos += move;
		if (parent != nullptr) parent->SetPos(pos - offset);
	}

	// ゲッタ
	D3DXVECTOR2 GetPos()const { 
		if(parent != nullptr)return parent->GetPos() + offset;
		return pos;
	}
	D3DXVECTOR2 GetPrevPos()const { return prevPos; }
	D3DXVECTOR2 GetOffset()const { return offset; }
	D3DXVECTOR2 GetSize()const { return size; }
	const D3DXVECTOR2* GetVertex() const { return vtx; }
	float GetRotate()const { return rotate; }
	std::string GetTag()const { return tag; }

	// セッタ
	void SetParent(Transform2D* _parent) { parent = _parent; }
	void SetPrevPos(D3DXVECTOR2 prev) { prevPos = prev; }
	void SetOffSet(D3DXVECTOR2 _offset) { offset = _offset; }
	void SetSize(D3DXVECTOR2 _size) { size = _size; }
	void SetTag(std::string _tag) { tag = _tag; }
	void RotateAt(float radAngle) {
		rotate += radAngle;
		Rotate(rotate);
	};
	void Rotate(float radAngle);

	void SetFriction(bool set) { isFriction = set; }

	// デバッグ用 当たり判定可視化
	void viewColid();
	void viewColid(D3DXCOLOR colidColor);

private:
	void SetVertex();
	Transform2D* parent = nullptr;	// 当たり判定が付く親の位置系を格納

	D3DXVECTOR2 vtx[4];		// 頂点位置
	D3DXVECTOR2 offset;		// 親からの相対座標
	D3DXVECTOR2 pos;		// ワールド座標
	D3DXVECTOR2 drawPos;
	D3DXVECTOR2 prevPos;		// 前のフレームの座標(緊急的に実装,ここに入れるべきでない)
	D3DXVECTOR2 size;
	D3DXCOLOR color;

	bool isFriction = false;

	float rotate = 0.0f;		// 回転

	std::string tag;		// タグ

};

#endif // !_COLIDER_2D_H_


