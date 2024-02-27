#include "button.h"

void Button::Setup(float newPosX, float newPosY, float newWidth, float newHeight, const char* _text)
{
	x = newPosX;
	y = newPosY;
	width = newWidth;
	height = newHeight;
	text = _text;
}
void Button::SetupImage(Texture2D buttonTexture)
{
	texture = buttonTexture;
	hasNoTexture = false;
}
void Button::SetupImage(Texture2D buttonTexture, Texture2D otherTexture)
{
	texture = buttonTexture;
	bonusTexture = otherTexture;
	hasNoTexture = false;
}
void Button::SetupToolTip(const char* _text)
{
	hasToolTip = true;
	toolTipText = _text;
	// set toolTipBox size compared to text
	toolTipBox.width = static_cast<float>(strlen(toolTipText) * (fontSize / 1.5f));
	toolTipBox.height = static_cast<float>(fontSize * 1.5f);
	toolTipBox.x = x -(toolTipBox.width / 2.f);
	toolTipBox.y = y + (height / 2.f) + (config.PIXEL_SCALE * 2.f);

	if (toolTipBox.x < 0.f)
	{
		toolTipBox.x = 0.f;
	}
}
bool Button::Update(Vector2 mousePos)
{
	if (mousePos.x >= x &&
		mousePos.x <= (x + width) &&
		mousePos.y >= y &&
		mousePos.y <= (y + height))
	{
		Hover();
	}
	else
	{
		color = WHITE;
		isHover = false;
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isHover)
	{
		return true;
	}
	return false;
}
void Button::Render()
{
	if (hasNoTexture)
	{
		DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), color);

		if (text == nullptr)
		{
			return;
		}
		DrawText(text, static_cast<int>(x), static_cast<int>(y), fontSize, BLACK);

		return;
	}

	float rotation = 0.0f;

	Rectangle src = { 0.f, 0.f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	Rectangle dst = { x, y, static_cast<float>(texture.width * config.PIXEL_SCALE), static_cast<float>(texture.height * config.PIXEL_SCALE) };
	Vector2 origin = { static_cast<float>(texture.width / 2.0f), static_cast<float>(texture.height / 2.0f) };

	DrawTexturePro(texture, src, dst, origin, rotation, color);

	if (isHover && hasToolTip)
	{
		DrawRectangle(static_cast<int>(toolTipBox.x), static_cast<int>(toolTipBox.y), static_cast<int>(toolTipBox.width), static_cast<int>(toolTipBox.height), WHITE);
		DrawText(toolTipText, static_cast<int>(toolTipBox.x), static_cast<int>(toolTipBox.y), fontSize, BLACK);
	}

	if (bonusTexture.id == 0)
	{
		return;
	}

	src = { 0.f, 0.f, static_cast<float>(bonusTexture.width), static_cast<float>(bonusTexture.height) };
	dst = { x + (texture.width / 2) + (bonusTexture.width / 2), y + (texture.height / 2), static_cast<float>(bonusTexture.width * config.PIXEL_SCALE), static_cast<float>(bonusTexture.height * config.PIXEL_SCALE)};
	origin = { static_cast<float>(bonusTexture.width / 2.0f), static_cast<float>(bonusTexture.height / 2.0f) };

	DrawTexturePro(bonusTexture, src, dst, origin, rotation, WHITE);
}


void Button::Hover()
{
	isHover = true;
	color = RED;
}