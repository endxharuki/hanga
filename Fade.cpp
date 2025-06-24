#include "Fade.h"
#include "texture.h"
#include "sprite.h"
#include "OnGameData.h"

Fade* Fade::Instance = Fade::CreateInstance();

void Fade::UpdateDraw()
{
	if (state == NONE_FADE)return;

	if (state == FADE_IN)
	{
		currentAlpha += fadeSpeed;
		if (currentAlpha > 1.0f)
		{
			state = IN_COMP;
		}
	}
	else if (state == FADE_OUT)
	{
		currentAlpha -= fadeSpeed;
		if (currentAlpha < 0)
		{
			state = NONE_FADE;
			fadeChangeSceneKey = "";
		}
	}

	DrawSpriteLeftTopColor(OnGameData::GetInstance()->GetFadeTex(),
		0.0f, 0.0f,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f,
		1.0f, 1.0f,
		fadeColor.r, fadeColor.g, fadeColor.b, currentAlpha);
}

void Fade::StartFade(float speed, std::string key)
{
	fadeSpeed = speed;
	state = FADE_IN;
	fadeChangeSceneKey = key;
}

void Fade::EndFade(float speed)
{
	if (state != IN_COMP) return;
	fadeSpeed = speed;
	state = FADE_OUT;
}


