#include "GameManager.h"


int main(void)
{    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 750;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    //--------------------------------------------------------------------------------------


    SetExitKey(0);

    GameManager gamemanager;

    gamemanager.gamestate.push(MENUSCREEN);
    gamemanager.level.LoadLevelFromTxt();

    // Main game loop
    while (gamemanager.isGameRunning)    // Detect window close button or ESC key
    {
       
       gamemanager.GameLoop();
    }
    
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}