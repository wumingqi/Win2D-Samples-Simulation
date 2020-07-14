#pragma once

#include <d2d1_3.h>
#include <string>

enum class FADE_MODE : DWORD
{
	LAYER,
	PER_PRIMITIVE,
	OPACITY_BRUSH,
	//CLIP_GEOMETRY
};

struct BouncingBall
{
	std::wstring Text;			// 文本
	D2D1_COLOR_F Color;			// 颜色
	D2D1_VECTOR_2F Velocity;	// 移动速度
	D2D1_VECTOR_2F Position;	// 当前位置
	FLOAT Radius;				// 半径
	UINT Fade;
	FLOAT FadeSpeed;
	bool IsInc;
	BouncingBall(const std::wstring text, const UINT color) :
		Text(text),
		Color(D2D1::ColorF(color)),
		Velocity{},
		Position{},
		Radius(0),
		IsInc(false)
	{
		Velocity = D2D1::Vector2F(RandomBetween(-2, 2), RandomBetween(-2, 2));
		Radius = RandomBetween(25, 100);
		FadeSpeed = RandomBetween(0.005f, 0.01f);

		Position.x = Radius;
		Position.y = Radius;
	}

	void Update(const D2D1_SIZE_F& controlSize)
	{
		Position.x += Velocity.x;
		Position.y += Velocity.y;

		FLOAT bounceX = (Position.x < Radius || Position.x>(controlSize.width - Radius)) ? -1.0f : 1.0f;
		FLOAT bounceY = (Position.y < Radius || Position.y>(controlSize.height - Radius)) ? -1.0f : 1.0f;

		Velocity.x *= bounceX;
		Velocity.y *= bounceY;

		if (IsInc)
		{
			Color.a += FadeSpeed;
			if (Color.a >= 1.0)
			{
				IsInc = false;
			}
		}
		else
		{
			Color.a -= FadeSpeed;
			if (Color.a <= 0)
			{
				IsInc = true;
			}
		}

		if (Position.x < 0)
			Position.x = Radius;
		if (Position.y < 0)
			Position.y = Radius;

		if (controlSize.width < Position.x + Radius)
			Position.x = controlSize.width - Radius;

		if (controlSize.height < Position.y + Radius)
			Position.y = controlSize.height - Radius;
	}

	static float RandomBetween(float min, float max)
	{
		return rand() % 1000 / 1000.0f * (max - min) + min;
	}
};