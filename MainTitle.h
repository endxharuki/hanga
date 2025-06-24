// ###############################################
// メイン制御　タイトル [MainTitle.h]
// Author: 西窪 統吉桜	Date: 2023/07/06
// ###############################################

#pragma once
#ifndef _MAIN_TITLE_H_
#define _MAIN_TITLE_H_

#include <iostream>
#include "StateBase.h"
#include "main.h"

class MainTitle : public StateBase
{
public:
	MainTitle(std::string _key)
		: StateBase(_key){}

	void SetUp() override;
	void CleanUp() override;
	void OnUpdate() override;
	void OnDraw() override;

private:
	
};

#endif // !_MAIN_TITLE_H_

