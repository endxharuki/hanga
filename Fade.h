#pragma once

#include "main.h"
#include <string>

#ifndef _FADE_H_
#define _FADE_H_

typedef enum
{
	NONE_FADE = 0,
	FADE_IN,
	IN_COMP,
	FADE_OUT,
} FadeState;

class Fade
{
public:
	static Fade* GetInstance() { return Instance; }

	static Fade* CreateInstance()
	{
		if (Instance != nullptr) return Instance;

		Instance = new Fade();
		return Instance;
	}

	void UpdateDraw();

	void StartFade(float speed, std::string key);
	void EndFade(float speed);

	// ゲッタ
	FadeState GetFadeState() { return state; };
	std::string GetChangeSceneKey() { return fadeChangeSceneKey; }

	// セッタ
	void SetFadeColor(D3DXCOLOR setColor) { fadeColor = setColor; }

private:
	Fade()
	{
		fadeColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		currentAlpha = 0.0f;
		state = NONE_FADE;
	};

	static Fade* Instance;

	D3DXCOLOR fadeColor;
	float currentAlpha;
	float fadeSpeed;
	FadeState state;

	std::string fadeChangeSceneKey;

};

#endif // !_FADE_H_

