// ###############################################
// ステージセレクト制御　クリア [MainStageSelect.h]
// Author: タダ タケル	Date: 2023/12/08
// ###############################################

#pragma once
#ifndef _MAIN_STAGE_SELECT_H_
#define _MAIN_STAGE_SELECT_H_

#include <iostream>
#include "StateBase.h"
#include "main.h"

#define STAGE_MAX 2
#define STAGE_MIN 1

class MainStageSelect : public StateBase
{
public:
	MainStageSelect(std::string _key)
		: StateBase(_key){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:

	int stageSelect;
};

#endif // !_MAIN_TITLE_H_

