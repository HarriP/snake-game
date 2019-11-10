#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "libraryBindings.h"
#include "game.h"

int resolutionX = 1024;
int resolutionY = 768;
int targetFps = 60;
int snakeSpeed = 10;
int snakeSquareSize = 40;
int keyUp;
int keyLeft;
int keyDown;
int keyRight;

int main(int argc, char** argv){
    srand(time(NULL));
    if(argc > 1){
        std::cout << "Command-line arguments example: -resolution 1280 960 -targetFps 60 -snakeSpeed 10 -snakeSquareSize 20 -keys w a s d" << std::endl;
        std::cout << "-keysNumeric can also be used instead of -keys to give keys in numeric format, for example -keysNumeric 265 263 264 262 for arrow keys." << std::endl;
        std::cout << "Smaller speed value is faster." << std::endl;
    }
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i], "-resolution") == 0 && argc > i+2){
            resolutionX = atoi(argv[i+1]);
            resolutionY = atoi(argv[i+2]);
        }
        else if(strcmp(argv[i], "-targetFps") == 0 && argc > i+1){
            targetFps = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-snakeSpeed") == 0 && argc > i+1){
            snakeSpeed = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-snakeSquareSize") == 0 && argc > i+1){
            snakeSquareSize = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-keys") == 0 && argc > i+4){
            keyUp = argv[i+1][0];
            keyLeft = argv[i+2][0];
            keyDown = argv[i+3][0];
            keyRight = argv[i+4][0];
        }
        else if(strcmp(argv[i], "-keysNumeric") == 0 && argc > i+4){
            keyUp = argv[i+1][0];
            keyLeft = argv[i+2][0];
            keyDown = argv[i+3][0];
            keyRight = argv[i+4][0];
        }
    }
    InitWindow(resolutionX, resolutionY, "Snake");
    SetTargetFPS(targetFps);
    Game();
    CloseWindow();
    return 0;
}
