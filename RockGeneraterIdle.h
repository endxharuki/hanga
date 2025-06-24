#pragma once

#include "StateBase.h"
#include "GameObject.h"
#include "ColiderPool.h"
#include "Rock.h"

#include <chrono>

#define GENERATE_INTERVAL_TIME (5000)
#define GENERATE_INTERVAL_COUNT (300)

class RockGenerater;
class RockGeneraterIdle : public StateBase
{
public:
	RockGeneraterIdle(std::string _key, RockGenerater* _obj,int _generateMode)
		: StateBase(_key), obj(_obj), generateMode(_generateMode) {}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	void Generate();
	void MultiGenerate();

	RockGenerater* obj;
	Rock* rock = nullptr;

	const D3DXVECTOR2 rockVel = D3DXVECTOR2(5.0f, 0.0f);
	const D3DXVECTOR2 rockSize = D3DXVECTOR2(100.0f, 100.0f);

	unsigned int blockTex[2];
	unsigned int dirtTex[2]; 
	int			drawMode;

	float dir = -1.0f;

	int generateCount = 0;//岩出現カウンター
	std::chrono::milliseconds generateFirstTime;//岩出現間隔用

	int generateMode = 0;//岩の出現パターン 0岩が消えたら生成 1一定フレームで生成 2一定時間で生成
};

