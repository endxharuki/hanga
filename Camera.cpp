#include "Camera.h"

void Camera::SetPos(D3DXVECTOR2 setPos)
{
	float leftPos = setPos.x - size.x / 2;
	float rightPos = setPos.x + size.x / 2;
	float topPos = setPos.y - size.y / 2;
	float bottomPos = setPos.y + size.y / 2;

	// xé≤à⁄ìÆêßå¿
	if (leftPos < leftLimit)
	{
		centerPos.x = leftLimit + size.x / 2;
	}
	else
	{
		if (rightPos > rightLimit)
		{
			centerPos.x = 10000 - size.x / 2;
		}
		else
		{
			centerPos.x = setPos.x;
		}
	}


	// yé≤à⁄ìÆêßå¿
	if (topPos < 0)
	{
		centerPos.y = size.y / 2;
	}
	else
	{
		if (bottomPos > SCREEN_HEIGHT)
		{
			centerPos.y = SCREEN_HEIGHT - size.y / 2;
		}
		else
		{
			centerPos.y = setPos.y;
		}
	}

	originPos.x = centerPos.x - (size.x / 2);
	originPos.y = centerPos.y - (size.y / 2);
}
