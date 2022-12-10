#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "libraryBindings.h"
#include "game.h"

int resolutionX = 1024;
int resolutionY = 768;
int targetFps = 60;
int snakeSpeed = 7;
int snakeBodySize = 40;
int lengthGainPerFood = 1;
int foodAmount = 3;
int collisionSnakeLengthIgnored = 60;
int keyUp = 265;
int keyLeft = 263;
int keyDown = 264;
int keyRight = 262;
std::mt19937_64 rndGen;

int main(int argc, char** argv){
    rndGen.seed(time(NULL));
    if(argc > 1){
        std::cout << "Command-line arguments example: -resolution 1280 960 -targetFps 60 -snakeSpeed 10 -snakeBodySize 20 -lengthGainPerFood 1 -foodAmount 1 -keys w a s d" << std::endl;
        std::cout << "Smaller speed value is faster in classic Snake, larger value is faster in modern Snake." << std::endl;
        std::cout << "-keysNumeric can also be used instead of -keys to give keys in numeric format, for example -keysNumeric 265 263 264 262 for arrow keys." << std::endl;
        std::cout << "Default keys are arrow keys and wasd. Press some direction key in the classic Snake to start moving." << std::endl;
        std::cout << "-collisionSnakeLengthIgnored sets the length of the snake counted from the head to ignore when checking collisions, to prevent the snake head from colliding with the body when moving straight (the body pieces overlap, especially at slow snake speeds). This is a tricky value to calculate with so many variables, so you can set it yourself if the collisions don't work properly. Increase it until the snake stops colliding with itself without you actually hitting yourself, or decrease it if the snake does not hit itself when it should. Only applies to the modern version." << std::endl << std::endl;
    }
    for(int i=1; i<argc; i++){
        if(std::strcmp(argv[i], "-resolution") == 0 && argc > i+2){
            resolutionX = std::atoi(argv[i+1]);
            resolutionY = std::atoi(argv[i+2]);
        }
        else if(std::strcmp(argv[i], "-targetFps") == 0 && argc > i+1){
            targetFps = std::atoi(argv[i+1]);
        }
        else if(std::strcmp(argv[i], "-snakeSpeed") == 0 && argc > i+1){
            snakeSpeed = std::atoi(argv[i+1]);
        }
        else if(std::strcmp(argv[i], "-snakeBodySize") == 0 && argc > i+1){
            snakeBodySize = std::atoi(argv[i+1]);
        }
        else if(std::strcmp(argv[i], "-lengthGainPerFood") == 0 && argc > i+1){
            lengthGainPerFood = std::atoi(argv[i+1]);
        }
        else if(std::strcmp(argv[i], "-foodAmount") == 0 && argc > i+1){
            foodAmount = std::atoi(argv[i+1]);
        }
        else if(std::strcmp(argv[i], "-collisionSnakeLengthIgnored") == 0 && argc > i+1){
            collisionSnakeLengthIgnored = std::atoi(argv[i+1]);
        }
        else if(std::strcmp(argv[i], "-keys") == 0 && argc > i+4){
            keyUp = std::toupper(argv[i+1][0]);
            keyLeft = std::toupper(argv[i+2][0]);
            keyDown = std::toupper(argv[i+3][0]);
            keyRight = std::toupper(argv[i+4][0]);
        }
        else if(std::strcmp(argv[i], "-keysNumeric") == 0 && argc > i+4){
            keyUp = argv[i+1][0];
            keyLeft = argv[i+2][0];
            keyDown = argv[i+3][0];
            keyRight = argv[i+4][0];
        }
    }
    InitWindow(resolutionX, resolutionY, "Snake");
    SetTargetFPS(targetFps);
    Game(argc, argv);
    CloseWindow();
    return 0;
}
