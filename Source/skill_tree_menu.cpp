#include "skill_tree_menu.h"

void SkillTreeMenu::Setup()
{
	backButton.Setup(0.f, 0.f, 200.f, 75.f, "Back");
}

void SkillTreeMenu::Activate(Camera2D cam)
{
	Vector2 backPos = { 0.f, 0.f };

	backPos = GetScreenToWorld2D(backPos, cam);
	
	backButton.x = backPos.x;
	backButton.y = backPos.y;
}

bool SkillTreeMenu::Update()
{
	Vector2 mousePos = GetMousePosition();
	if (backButton.Update(mousePos))
	{
		return true;
	}
	return false;
}

void SkillTreeMenu::Render()
{
	backButton.Render();
	DrawText("SKILLS", static_cast<int>(backButton.x), static_cast<int>(backButton.y - 200), 40, YELLOW);
	DrawText("Nothing here yet", static_cast<int>(backButton.x), static_cast<int>(backButton.y - 100), 25, YELLOW);
}
