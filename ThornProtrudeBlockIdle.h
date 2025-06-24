#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"

#define PROTRUDE_WAIT_DURATION (60)
#define PROTRUDE_LITTLE_STRETCH_DURATION (10)
#define PROTRUDE_LITTLE_KEEP_DURATION (20)
#define PROTRUDE_STRETCH_DURATION (10)
#define PROTRUDE_KEEP_DURATION (20)
#define PROTRUDE_SHRINK_DURATION (60)


#define PROTRUDE_LITTLE_STRETCH_FIRST (PROTRUDE_WAIT_DURATION)
#define PROTRUDE_LITTLE_KEEP_FIRST (PROTRUDE_LITTLE_STRETCH_FIRST+PROTRUDE_LITTLE_STRETCH_DURATION)
#define PROTRUDE_STRETCH_FIRST (PROTRUDE_LITTLE_KEEP_FIRST+PROTRUDE_LITTLE_KEEP_DURATION)
#define PROTRUDE_KEEP_FIRST (PROTRUDE_STRETCH_FIRST+PROTRUDE_STRETCH_DURATION)
#define PROTRUDE_SHRINK_FIRST (PROTRUDE_KEEP_FIRST+PROTRUDE_KEEP_DURATION)
#define PROTRUDE_FIN (PROTRUDE_SHRINK_FIRST+PROTRUDE_SHRINK_DURATION)


class ThornProtrudeBlockIdle : public StateBase
{
public:
	ThornProtrudeBlockIdle(std::string _key, GameObject* _obj)
		: StateBase(_key), obj(_obj){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

	

private:
	void ColidPlayer();
	void ColidObject();

	void LittleStretch();
	void Stretch();
	void Shrink();

	GameObject *obj;
	ColiderPool* pool;

	D3DXVECTOR2 firstPos;

	unsigned int blockTex[2];
	unsigned int thornTex[2];
	int			drawMode;

	int protrudeCount = 0;
};

