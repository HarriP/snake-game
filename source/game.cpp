#include "libraryBindings.h"

void CheckInput(int& x, int& y){
    if(IsKeyPressed(KEY_RIGHT)){
        x += 1;
    }
    else if(IsKeyPressed(KEY_LEFT)){
        x -= 1;
    }
    else if(IsKeyPressed(KEY_DOWN)){
        y += 1;
    }
    else if(IsKeyPressed(KEY_UP)){
        y -= 1;
    }
}

void Game(){
    int x = 200, y = 200;
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(Color(0, 0, 0, 255));
        DrawFPS(10, 10);
        DrawSquare(x, y, 20, Color(0, 0, 255, 255));
        CheckInput(x, y);
        EndDrawing();
    }
}
