#pragma once
#include "myMath.h"


namespace GUI                   // I made this namespace as it does not contain any variables
{

    void gui_label(const Rectangle rect, const char* label_text, int font);

	bool gui_button(const Rectangle rect, const char* button_text, int font);
};