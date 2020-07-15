#pragma once

#include <d2d1_3.h>
#include <string>

enum class LAYER_MODE : DWORD
{
	LAYER_FADE,
	PER_PRIMITIVE_FADE,
	OPACITY_BRUSH,
	CLIP_GEOMETRY
};

struct BouncingBall
{
	std::wstring			Text;									// 文本
	D2D1_COLOR_F			Color;									// 颜色
	D2D1_VECTOR_2F			Velocity;								// 移动速度
	D2D1_VECTOR_2F			Position;								// 当前位置
	FLOAT					Radius;									// 半径
	FLOAT					FadeSpeed;
	FLOAT					FadeAge;

	BouncingBall(const std::wstring text, const UINT color) :
		Text(text),
		Color(D2D1::ColorF(color)),
		Velocity{},
		Position{},
		Radius(0),
		FadeSpeed(0),
		FadeAge(0)
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

		D2D1_VECTOR_2F topLeft = { Radius, Radius };
		D2D1_VECTOR_2F bottomRight = { controlSize.width - Radius, controlSize.height - Radius };

		float bounceX = (Position.x < topLeft.x || Position.x > bottomRight.x) ? -1 : 1;
		float bounceY = (Position.y < topLeft.y || Position.y > bottomRight.y) ? -1 : 1;

		Velocity.x *= bounceX;
		Velocity.y *= bounceY;

		// Position = Vector2.Clamp(Position, topLeft, bottomRight);

		if (Position.x < topLeft.x) Position.x = topLeft.x;
		if (Position.y < topLeft.y) Position.y = topLeft.y;
		if (Position.x > bottomRight.x) Position.x = bottomRight.x;
		if (Position.y > bottomRight.y) Position.y = bottomRight.y;

		FadeAge += FadeSpeed;
	}

	static float RandomBetween(float min, float max)
	{
		return rand() % 1000 / 1000.0f * (max - min) + min;
	}

	FLOAT GetFadeAlpha() const
	{
		float alpha = FadeAge - (int)FadeAge;

		if ((int)FadeAge % 2 != 0)
			alpha = 1.0f - alpha;

		return alpha;
	}

};