/**
 * @File Easing.cpp
 * @Brief イージング関数ユーティリティー
 * @Author 西窪 統吉桜
 * @LastDate 2024/01/23
 */
#include "Easing.h"
#include <math.h>

float easeOutBack(float x)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return (float)1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}
