#pragma once

#include "StateBase.h"
#include "GameObject.h"

class PermationBlockIdle : public StateBase
{
public:
	PermationBlockIdle(std::string _key, GameObject* _obj, int _type)
		: StateBase(_key), obj(_obj), type(_type){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	GameObject *obj;

	unsigned int blockTex[2];
	int			drawMode;
	int			type;	// ƒuƒƒbƒN‚ÌŒ©‚½–Ú‚Ìí—Ş

	float alpha = 1.0f;
};

