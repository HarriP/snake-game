#include <string>
#include <vector>
#include <queue>
#include <random>
#include <cstring>
#include <cmath>
#include "libraryBindings.h"

float pi = atan(1) * 4.0f;
int borderPadding = 40;
extern int resolutionX;
extern int resolutionY;
extern int targetFps;
extern int snakeSpeed;
extern int snakeBodySize;
extern int lengthGainPerFood;
extern int foodAmount;
extern int collisionSnakeLengthIgnored;
extern int keyUp;
extern int keyLeft;
extern int keyDown;
extern int keyRight;
extern std::mt19937_64 rndGen;

enum class Direction{
    None,
    Up,
    Left,
    Down,
    Right
};

bool ButtonPressed(Direction dir){
    if(dir == Direction::Up){
        if(IsKeyDown(keyUp) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)){
            return true;
        }
        return false;
    }
    else if(dir == Direction::Left){
        if(IsKeyDown(keyLeft) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)){
            return true;
        }
        return false;
    }
    else if(dir == Direction::Down){
        if(IsKeyDown(keyDown) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
            return true;
        }
        return false;
    }
    else if(dir == Direction::Right){
        if(IsKeyDown(keyRight) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){
            return true;
        }
        return false;
    }
    return false;
}

float CheckInput(float currentDirection){
    if(ButtonPressed(Direction::Up)){
    }
    else if(ButtonPressed(Direction::Left)){
        return currentDirection - 0.08f;
    }
    else if(ButtonPressed(Direction::Down)){
    }
    else if(ButtonPressed(Direction::Right)){
        return currentDirection + 0.08f;
    }
    return currentDirection;
}

class Pos{
public:
    float x;
    float y;
    Pos() : x(0), y(0) {}
    Pos(float aX, float aY) : x(aX), y(aY) {}
    bool WithinRange(const Pos& b, float diameter) const{
        if(hypot(b.x-x, b.y-y) < diameter){
            return true;
        }
        return false;
    }
};

Pos RandomPos(){
    float randomX = rndGen() % (resolutionX - borderPadding * 2 - snakeBodySize) + borderPadding + snakeBodySize / 2;
    float randomY = rndGen() % (resolutionY - borderPadding * 2 - snakeBodySize) + borderPadding + snakeBodySize / 2;
    return Pos(randomX, randomY);
}

Pos RandomInitPos(){
    // resolutionX / 2 is because snake moves right initially, so it doesn't hit the wall when starting.
    float randomX = rndGen() % (resolutionX / 2) + borderPadding + snakeBodySize / 2;
    float randomY = rndGen() % (resolutionY - borderPadding * 2 - snakeBodySize) + borderPadding + snakeBodySize / 2;
    return Pos(randomX, randomY);
}

class SnakeBodyPiece{
public:
    Pos pos;
    Color color;
    SnakeBodyPiece(const Pos& aPos, const Color& aColor) : pos(aPos), color(aColor) {}
};

class Snake{
public:
    std::vector<SnakeBodyPiece> body;
    float dir = 0;
    int speed;
    int bodySize;
    Snake(int aSpeed, int aBodySize) : speed(aSpeed), bodySize(aBodySize){
        body.push_back(SnakeBodyPiece(RandomInitPos(), Color(0, 127, 255, 255)));
    }
    void Reset(){
        body.clear();
        body.push_back(SnakeBodyPiece(RandomInitPos(), Color(0, 127, 255, 255)));
        dir = 0;
    }
    void Move(){
        for(int i=body.size()-1; i>0; i--){
            body[i].pos = body[i-1].pos;
        }
        if(dir > pi*2){
            dir -= pi*2;
        }
        else if(dir < -pi*2){
            dir += pi*2;
        }
        body[0].pos.x += cos(dir) * speed / 100;
        body[0].pos.y += sin(dir) * speed / 100;
    }
    void Draw() const{
        for(const SnakeBodyPiece& p : body){
            DrawCircle(p.pos.x, p.pos.y, bodySize/2, p.color);
        }
    }
    bool PosCollidesWithSnakeOrFood(const Pos& pos, const std::vector<SnakeBodyPiece>& foodList) const{
        for(const SnakeBodyPiece& p : body){
            if(p.pos.WithinRange(pos, bodySize)){
                return true;
            }
        }
        for(const SnakeBodyPiece& food : foodList){
            if(food.pos.WithinRange(pos, bodySize)){
                return true;
            }
        }
        return false;
    }
    Pos RandomPosOutsideSnakeAndFood(const std::vector<SnakeBodyPiece>& foodList) const{
        int tries = 0;
        Pos pos = RandomPos();
        while(PosCollidesWithSnakeOrFood(pos, foodList) && tries < 20){
            pos = RandomPos();
            tries++;
        }
        return pos;
    }
    bool CheckCollision(std::vector<SnakeBodyPiece>& foodList){
        if((int)foodList.size() < foodAmount){
            foodList.push_back(SnakeBodyPiece(RandomPosOutsideSnakeAndFood(foodList), Color(255, 0, 0, 255)));
        }
        for(int i=0; i<(int)foodList.size(); i++){
            if(body[0].pos.WithinRange(foodList[i].pos, bodySize)){
                for(int j=0; j<lengthGainPerFood; j++){
                    body.push_back(SnakeBodyPiece(Pos(-1000, -1000), Color(0, 127, 255, 255)));
                }
                foodList.erase(foodList.begin()+i);
                i--;
            }
        }
        for(int i=collisionSnakeLengthIgnored; i<(int)body.size(); i++){
            if(body[0].pos.WithinRange(body[i].pos, bodySize)){
                return true;
            }
        }
        if(body[0].pos.x > resolutionX-borderPadding-bodySize/2 || body[0].pos.x < borderPadding+bodySize/2){
            return true;
        }
        else if(body[0].pos.y > resolutionY-borderPadding-bodySize/2 || body[0].pos.y < borderPadding+bodySize/2){
            return true;
        }
        return false;
    }
};

void Game(int argc, char** argv){
    {
        bool speedSet = false;
        for(int i=1; i<argc; i++){
            if(std::strcmp(argv[i], "-snakeSpeed") == 0 && argc > i+1){
                speedSet = true;
            }
        }
        if(!speedSet){
            snakeSpeed = 350;
        }
    }
    Snake snake(snakeSpeed, snakeBodySize);
    std::vector<SnakeBodyPiece> foodList;
    bool gameEnd = false;
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(Color(0, 0, 0, 255));
        // Draw level boundaries.
        DrawLine(borderPadding, borderPadding, resolutionX-borderPadding, borderPadding, Color(255, 255, 255, 255));
        DrawLine(borderPadding, resolutionY-borderPadding, resolutionX-borderPadding, resolutionY-borderPadding, Color(255, 255, 255, 255));
        DrawLine(borderPadding, borderPadding, borderPadding, resolutionY-borderPadding, Color(255, 255, 255, 255));
        DrawLine(resolutionX-borderPadding, borderPadding, resolutionX-borderPadding, resolutionY-borderPadding, Color(255, 255, 255, 255));
        // End drawing level boundaries.
        DrawFPS(10, 1);
        DrawText("Length: " + std::to_string(snake.body.size()), 150, 1, 20, Color(200, 200, 255, 255));
        snake.Draw();
        for(const SnakeBodyPiece& food : foodList){
            DrawCircle(food.pos.x, food.pos.y, snake.bodySize/2, food.color);
        }
        if(!gameEnd){
            snake.dir = CheckInput(snake.dir);
            snake.Move();
            if(snake.CheckCollision(foodList)){
                gameEnd = true;
            }
        }
        else{
            DrawText("Bummer! You lost.\nPress enter to play again.", resolutionX/2-300, resolutionY/2-100, 40, Color(180, 0, 0, 255));
            if(IsKeyPressed(KEY_ENTER)){
                // Restart game.
                snake.Reset();
                foodList.clear();
                gameEnd = false;
            }
        }
        EndDrawing();
    }
}
