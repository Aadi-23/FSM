#include "GUI.h"




void GUI::gui_label(const Rectangle rect, const char* label_text, int font)
{
	Vector2 text_size = { (float)MeasureText(label_text, font), (float)font };
	Vector2 text_top_left = (Vector2{ rect.x,rect.y } + (Vector2{ rect.width,rect.height } - text_size) / 2);

	DrawText(label_text, (int)text_top_left.x, (int)text_top_left.y, font, WHITE);
}

// This bool function i use which returns true if the mouse clicks on the menu labels

bool GUI::gui_button(const Rectangle rect, const char* button_text, int font)
{
	bool clicked = false;

	Color background_color = Color{ 128, 128, 128, 255 };

	if (CheckCollisionPointRec(GetMousePosition(), rect))
	{
		background_color = Color{ 96, 96, 96, 255 };

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			background_color = Color{ 32, 32, 32, 255 };
		}

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
		{
			clicked = true;
		}
	}

	DrawRectangleV({ rect.x, rect.y }, { rect.width, rect.height }, background_color);
	GUI::gui_label(rect, button_text, font);

	return clicked;
}
