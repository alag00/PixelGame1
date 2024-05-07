#pragma once
#include "raylib.h"
#include <cmath>
#include <algorithm>

class Slider
{
private:
	float x = 0.f;
	float y = 0.f;
	float width = 0.f;
	float height = 0.f;

	bool showBob = false;
	Vector2 bobPos = { 0.f, 0.f };
	float value = 0.f; // 0.f to 1.f
	Color color = WHITE;

	bool horizontal = true;
	bool isHover = false;

	Rectangle fillBar{ 0.f, 0.f, 0.f, 0.f };
public:
	void Setup(bool interactable, float xPos, float yPos, float newWidth, float newHeight)
	{
		showBob = interactable;
		x = xPos;
		y = yPos;
		width = newWidth;
		height = newHeight;
		bobPos = { x, y };
		if (width > height)
		{
			horizontal = true;
			bobPos.y += (height / 2.f);
			fillBar.height = height;
		}
		else
		{
			horizontal = false;
			bobPos.x += (width / 2.f);
			fillBar.width = width;
		}
		fillBar.x = xPos;
		fillBar.y = yPos;
	
	}
	void SetColor(Color newColor)
	{
		color = newColor;
	}
	void Update(Vector2 mousePos)
	{
		if (!showBob)
		{
			return;
		}
		float boxExtraSize = 20.f;
		// if mouse button is down and mouse inside slider
		if (mousePos.x >= x - boxExtraSize &&
			mousePos.x <= (x + width + boxExtraSize) &&
			mousePos.y >= y - boxExtraSize &&
			mousePos.y <= (y + height + boxExtraSize))
		{
			Hover(mousePos);
		}
		else
		{
			isHover = false;
		}
		mousePos;
	};
	void Render() 
	{
		DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), color);
		DrawText(TextFormat("Value: %i", static_cast<int>(value * 100)), static_cast<int>(x + 10), static_cast<int>(y + 10), 20, RED);
		if (!showBob)
		{
			return;
		}
		Color bobColor = WHITE;
		if (isHover)
		{
			bobColor = RED;
		}
		DrawRectangleRec(fillBar, RED);
		DrawCircle(static_cast<int>(bobPos.x), static_cast<int>(bobPos.y), 10.f, bobColor);
	};
	void SetValue(float newValue) 
	{
		value = newValue;
		if (!showBob)
		{
			return;
		}
		UpdateBobPos();
	}
	void AddValue(float amount)
	{
		value += amount;
		if (value > 1.f)
		{
			value = 1.f;
		}
		if (!showBob)
		{
			return;
		}
		UpdateBobPos();
	}
	float GetValue()
	{
		return value;
	}
	void UpdateValueBasedOnBob()
	{
		if (horizontal)
		{
			value = (bobPos.x - x) / width;
		}
		else
		{
			value = (bobPos.y - y) / height;
		}
	}
	void UpdateBobPos()
	{
		if (horizontal)
		{
			bobPos.x = std::lerp(x, x + width, value);
		}
		else
		{
			bobPos.y = std::lerp(y, y + height, value);
		}
	}
	void Hover(Vector2 mousePos)
	{
		isHover = true;
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			if (horizontal)
			{
				bobPos.x = std::clamp(mousePos.x, x, x + width); //bobPos.x = mousePos.x;
				fillBar.width = bobPos.x - x;
			}
			else
			{
				bobPos.y = std::clamp(mousePos.y, y, y + height); //bobPos.y = mousePos.y;
				fillBar.height = bobPos.y - y;
			}
			UpdateValueBasedOnBob();
			
		}
	}
};