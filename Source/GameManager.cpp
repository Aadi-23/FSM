#include "GameManager.h"
#include "CONSTANT.h"



void GameManager::GameLoop()
{
	// I put it inside if condition SO if it doesn't give error if the stack is empty
	if (!gamestate.empty())
	{
		currentgamestate = gamestate.top();
	}

	BeginDrawing();
	ClearBackground(BLACK);

	switch (currentgamestate)
	{
	case(MENUSCREEN):
	{
		
		DrawText("GOAT & BUSHES", GetRenderWidth() /4, GetRenderHeight() / 10, GAMEFONT_SIZE * 2, WHITE);
		DoMenuFrame();

		
	}
	break;

	case(GAMESCREEN):
	{
		
		level.LevelUpdate();
		level.LevelRender();

		if (IsKeyPressed(KEY_ESCAPE))
		{
			gamestate.push(PAUSESCREEN);
		}

		if (level.game_over)
		{
			gamestate.pop();
		}
	}
	break;

	case(PAUSESCREEN):
	{
		DoMenuFrame();
	}
	break;
	case(ENDSCREEN):
	{
		Rectangle button_rec;
		button_rec.width = 180;
		button_rec.height = 60;
		button_rec.x = (float)(GetRenderWidth() / 20);
		button_rec.y = (float)(GetRenderHeight() / 20);

		//const int button_margin_y = 10;
	}
	break;
	}

	EndDrawing();
}


void GameManager::DoMenuFrame()
{
	const Vector2 button_size = { 200,50 };
	const int button_margin_y = 10;
	const int button_count = 4;

	Rectangle button_rect;
	button_rect.x = (GetRenderWidth() - button_size.x) / 2;
	button_rect.y = (GetRenderHeight() - (button_size.y * button_count) - (button_count - 1) * button_margin_y) / 2;
	button_rect.width = button_size.x;
	button_rect.height = button_size.y;


	// Main menu for starting screen
	if (currentgamestate == MENUSCREEN)
	{
		if (GUI::gui_button(button_rect, "PLAY", 30))
		{
			level.LevelReset();
			isGameRunning = true;
			level.LevelLoader();
			gamestate.push(GAMESCREEN);
		}

		button_rect.y += button_size.y + button_margin_y;

		if (GUI::gui_button(button_rect, "EXIT", 30))
		{
			isGameRunning = false;
		}
	}

	// Menu for pause screen during game play
	if (currentgamestate == PAUSESCREEN)
	{
		if (GUI::gui_button(button_rect, "CONTINUE", 30))
		{
			gamestate.pop();
		}

		button_rect.y += button_size.y + button_margin_y;

		if (GUI::gui_button(button_rect, "RESTART", 30))
		{
			level.LevelReset();
			isGameRunning = true;
			level.LevelLoader();
			gamestate.pop();
			

		}

		button_rect.y += button_size.y + button_margin_y;

		if (GUI::gui_button(button_rect, "MAIN MENU", 30))
		{
			gamestate.pop();
			gamestate.pop();
		}
	}

	

	

}


