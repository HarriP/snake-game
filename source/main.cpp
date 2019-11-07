#include <iostream>
#include <string>
#include <vector>
#include "libraryBindings.h"
#include "game.h"

int main(int argc, char** argv){
    int resolutionX = 1280;
    int resolutionY = 960;
    int frames = 0;
    InitWindow(resolutionX, resolutionY, "Snake");
    SetTargetFPS(60);
    Game();
    CloseWindow();
    return 0;
}
