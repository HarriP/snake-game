#include <string>
#include <vector>
#include <queue>
#include <random>
#include <algorithm>
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
int keyMouseControl = 'M';
extern std::mt19937_64 rndGen;

class Position{
public:
    float x;
    float y;
    Position() : x(0), y(0) {}
    Position(float aX, float aY) : x(aX), y(aY) {}
    explicit Position(Vector2 vector) : x(vector.x), y(vector.y) {}
    bool WithinRange(const Position& b, float diameter) const{
        if(hypot(b.x-x, b.y-y) < diameter){
            return true;
        }
        return false;
    }
    float GetDirectionTo(const Position& targetPosition) const{
        return std::atan2(targetPosition.y-y, targetPosition.x-x);
    }
};

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

float CheckInput(float currentDirection, const Position& snakePosition){
    static bool mouseControl = false;
    if(ButtonPressed(Direction::Up)){
        mouseControl = false;
    }
    else if(ButtonPressed(Direction::Left)){
        mouseControl = false;
        return currentDirection - 0.08f;
    }
    else if(ButtonPressed(Direction::Down)){
        mouseControl = false;
    }
    else if(ButtonPressed(Direction::Right)){
        mouseControl = false;
        return currentDirection + 0.08f;
    }
    else if(IsKeyPressed(keyMouseControl)){
        mouseControl = !mouseControl;
    }
    if(mouseControl){
        float newDirection = snakePosition.GetDirectionTo(Position(GetMousePosition()));
        float difference = newDirection - currentDirection;
        float sign = std::abs(difference) <= pi ? -1 : 1;
        if(difference > 0){
            sign = std::abs(difference) <= pi ? 1 : -1;
        }
        return currentDirection + sign * std::min(std::abs(difference), 0.08f);
    }
    return currentDirection;
}

Position RandomPosition(){
    float randomX = rndGen() % (resolutionX - borderPadding * 2 - snakeBodySize) + borderPadding + snakeBodySize / 2;
    float randomY = rndGen() % (resolutionY - borderPadding * 2 - snakeBodySize) + borderPadding + snakeBodySize / 2;
    return Position(randomX, randomY);
}

Position RandomInitialPosition(){
    // resolutionX / 2 is because snake moves right initially, so it doesn't hit the wall when starting.
    float randomX = rndGen() % (resolutionX / 2) + borderPadding + snakeBodySize / 2;
    float randomY = rndGen() % (resolutionY - borderPadding * 2 - snakeBodySize) + borderPadding + snakeBodySize / 2;
    return Position(randomX, randomY);
}

class SnakeBodyPiece{
public:
    Position position;
    Color color;
    SnakeBodyPiece(const Position& aPosition, const Color& aColor) : position(aPosition), color(aColor) {}
};

class Snake{
public:
    std::vector<SnakeBodyPiece> body;
    float direction = 0;
    int speed;
    int bodySize;
    Snake(int aSpeed, int aBodySize) : speed(aSpeed), bodySize(aBodySize){
        body.push_back(SnakeBodyPiece(RandomInitialPosition(), Color(0, 127, 255, 255)));
    }
    void Reset(){
        body.clear();
        body.push_back(SnakeBodyPiece(RandomInitialPosition(), Color(0, 127, 255, 255)));
        direction = 0;
    }
    void Move(){
        for(int i=body.size()-1; i>0; i--){
            body[i].position = body[i-1].position;
        }
        if(direction > pi){
            direction -= pi*2;
        }
        else if(direction < -pi){
            direction += pi*2;
        }
        body[0].position.x += cos(direction) * speed / 100;
        body[0].position.y += sin(direction) * speed / 100;
    }
    void Draw() const{
        for(const SnakeBodyPiece& p : body){
            DrawCircle(p.position.x, p.position.y, bodySize/2, p.color);
        }
    }
    bool PositionCollidesWithSnakeOrFood(const Position& position, const std::vector<SnakeBodyPiece>& foodList) const{
        for(const SnakeBodyPiece& p : body){
            if(p.position.WithinRange(position, bodySize)){
                return true;
            }
        }
        for(const SnakeBodyPiece& food : foodList){
            if(food.position.WithinRange(position, bodySize)){
                return true;
            }
        }
        return false;
    }
    Position RandomPositionOutsideSnakeAndFood(const std::vector<SnakeBodyPiece>& foodList) const{
        int tries = 0;
        Position position = RandomPosition();
        while(PositionCollidesWithSnakeOrFood(position, foodList) && tries < 20){
            position = RandomPosition();
            tries++;
        }
        return position;
    }
    bool CheckCollision(std::vector<SnakeBodyPiece>& foodList){
        if((int)foodList.size() < foodAmount){
            foodList.push_back(SnakeBodyPiece(RandomPositionOutsideSnakeAndFood(foodList), Color(255, 0, 0, 255)));
        }
        for(int i=0; i<(int)foodList.size(); i++){
            if(body[0].position.WithinRange(foodList[i].position, bodySize)){
                for(int j=0; j<lengthGainPerFood; j++){
                    body.push_back(SnakeBodyPiece(Position(-1000, -1000), Color(0, 127, 255, 255)));
                }
                foodList.erase(foodList.begin()+i);
                i--;
            }
        }
        for(int i=collisionSnakeLengthIgnored; i<(int)body.size(); i++){
            if(body[0].position.WithinRange(body[i].position, bodySize)){
                return true;
            }
        }
        if(body[0].position.x > resolutionX-borderPadding-bodySize/2 || body[0].position.x < borderPadding+bodySize/2){
            return true;
        }
        else if(body[0].position.y > resolutionY-borderPadding-bodySize/2 || body[0].position.y < borderPadding+bodySize/2){
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
            else if(std::strcmp(argv[i], "-mouseControlKey") == 0 && argc > i+1){
                keyMouseControl = std::toupper(argv[i+1][0]);
            }
            else if(std::strcmp(argv[i], "-mouseControlKeyNumeric") == 0 && argc > i+1){
                keyMouseControl = std::atoi(argv[i+1]);
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
            DrawCircle(food.position.x, food.position.y, snake.bodySize/2, food.color);
        }
        if(!gameEnd){
            snake.direction = CheckInput(snake.direction, snake.body[0].position);
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
