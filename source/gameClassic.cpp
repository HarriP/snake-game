#include <string>
#include <vector>
#include <queue>
#include <random>
#include "libraryBindings.h"

extern int resolutionX;
extern int resolutionY;
extern int targetFps;
extern int snakeSpeed;
extern int snakeBodySize;
extern int lengthGainPerFood;
extern int foodAmount;
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

bool ButtonPressed(Direction direction){
    if(direction == Direction::Up){
        if(IsKeyPressed(keyUp) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
            return true;
        }
        return false;
    }
    else if(direction == Direction::Left){
        if(IsKeyPressed(keyLeft) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){
            return true;
        }
        return false;
    }
    else if(direction == Direction::Down){
        if(IsKeyPressed(keyDown) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
            return true;
        }
        return false;
    }
    else if(direction == Direction::Right){
        if(IsKeyPressed(keyRight) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)){
            return true;
        }
        return false;
    }
    return false;
}

void CheckInput(std::queue<Direction>& playerCommands){
    if(ButtonPressed(Direction::Up)){
        playerCommands.push(Direction::Up);
    }
    else if(ButtonPressed(Direction::Left)){
        playerCommands.push(Direction::Left);
    }
    else if(ButtonPressed(Direction::Down)){
        playerCommands.push(Direction::Down);
    }
    else if(ButtonPressed(Direction::Right)){
        playerCommands.push(Direction::Right);
    }
}

class Position{
public:
    int x;
    int y;
    Position() : x(0), y(0) {}
    Position(int aX, int aY) : x(aX), y(aY) {}
    bool operator==(const Position& b) const{
        if(x == b.x && y == b.y){
            return true;
        }
        return false;
    }
    bool operator!=(const Position& b) const{
        return !operator==(b);
    }
};

Position RandomPosition(){
    int squaresX = (resolutionX - 2 * snakeBodySize) / snakeBodySize;
    int squaresY = (resolutionY - 2 * snakeBodySize) / snakeBodySize;
    int randomX = rndGen() % squaresX * snakeBodySize + snakeBodySize;
    int randomY = rndGen() % squaresY * snakeBodySize + snakeBodySize;
    return Position(randomX, randomY);
}

class DirectionLookup{
public:
    Direction disallowed;
    Position toPosition;
};

class SnakeBodyPiece{
public:
    Position position;
    Color color;
    SnakeBodyPiece(const Position& aPosition, const Color& aColor) : position(aPosition), color(aColor) {}
};

class Snake{
public:
    std::vector<SnakeBodyPiece> body;
    Direction direction = Direction::None;
    int speed;
    int bodySize;
    Position previousPosition;
    const DirectionLookup directionLookup[5] = {
        {Direction::None, Position(0, 0)},
        {Direction::Down, Position(0, -1)},
        {Direction::Right, Position(-1, 0)},
        {Direction::Up, Position(0, 1)},
        {Direction::Left, Position(1, 0)}
    };
    Snake(int aSpeed, int abodySize) : speed(aSpeed), bodySize(abodySize){
        body.push_back(SnakeBodyPiece(RandomPosition(), Color(0, 127, 255, 255)));
    }
    void Reset(){
        body.clear();
        body.push_back(SnakeBodyPiece(RandomPosition(), Color(0, 127, 255, 255)));
        direction = Direction::None;
    }
    void Move(std::queue<Direction>& playerCommands){
        for(int i=body.size()-1; i>0; i--){
            body[i].position = body[i-1].position;
        }
        previousPosition = body[0].position;
        while(playerCommands.size() > 0){
            Direction newDirection = playerCommands.front();
            playerCommands.pop();
            if(newDirection != directionLookup[(int)direction].disallowed && direction != newDirection){
                direction = newDirection;
                body[0].position.x += directionLookup[(int)newDirection].toPosition.x * bodySize;
                body[0].position.y += directionLookup[(int)newDirection].toPosition.y * bodySize;
                break;
            }
        }
        // If player commands didn't move the snake, continue moving forward in the same direction.
        if(previousPosition == body[0].position){
            body[0].position.x += directionLookup[(int)direction].toPosition.x * bodySize;
            body[0].position.y += directionLookup[(int)direction].toPosition.y * bodySize;
        }
    }
    void Draw() const{
        for(const SnakeBodyPiece& p : body){
            DrawSquare(p.position.x, p.position.y, bodySize, p.color);
        }
    }
    bool PositionCollidesWithSnakeOrFood(const Position& position, const std::vector<SnakeBodyPiece>& foodList) const{
        for(const SnakeBodyPiece& p : body){
            if(position == p.position){
                return true;
            }
        }
        for(const SnakeBodyPiece& food : foodList){
            if(position == food.position){
                return true;
            }
        }
        return false;
    }
    Position RandomPositionOutsideSnakeOrFood(const std::vector<SnakeBodyPiece>& foodList) const{
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
            foodList.push_back(SnakeBodyPiece(RandomPositionOutsideSnakeOrFood(foodList), Color(255, 0, 0, 255)));
        }
        for(int i=0; i<(int)foodList.size(); i++){
            if(body[0].position == foodList[i].position){
                for(int j=0; j<lengthGainPerFood; j++){
                    body.push_back(SnakeBodyPiece(previousPosition, Color(0, 127, 255, 255)));
                }
                foodList.erase(foodList.begin()+i);
                i--;
            }
        }
        for(int i=1; i<(int)body.size(); i++){
            if(body[0].position == body[i].position){
                return true;
            }
        }
        if(body[0].position.x > resolutionX - bodySize*2 || body[0].position.x < bodySize){
            return true;
        }
        else if(body[0].position.y > resolutionY - bodySize*2 || body[0].position.y < bodySize){
            return true;
        }
        return false;
    }
};

void Game(int argc, char** argv){
    std::queue<Direction> playerCommands;
    Snake snake(snakeSpeed, snakeBodySize);
    std::vector<SnakeBodyPiece> foodList;
    int moveTimer = 0;
    bool gameEnd = false;
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(Color(0, 0, 0, 255));
        // Draw level boundaries.
        int boundaryX = resolutionX / snakeBodySize * snakeBodySize;
        int boundaryY = resolutionY / snakeBodySize * snakeBodySize;
        DrawLine(snakeBodySize, snakeBodySize, boundaryX-snakeBodySize, snakeBodySize, Color(255, 255, 255, 255));
        DrawLine(snakeBodySize, boundaryY-snakeBodySize, boundaryX-snakeBodySize, boundaryY-snakeBodySize, Color(255, 255, 255, 255));
        DrawLine(snakeBodySize, snakeBodySize, snakeBodySize, boundaryY-snakeBodySize, Color(255, 255, 255, 255));
        DrawLine(boundaryX-snakeBodySize, snakeBodySize, boundaryX-snakeBodySize, boundaryY-snakeBodySize, Color(255, 255, 255, 255));
        // End drawing level boundaries.
        DrawFPS(10, 1);
        DrawText("Length: " + std::to_string(snake.body.size()), 150, 1, 20, Color(200, 200, 255, 255));
        snake.Draw();
        for(SnakeBodyPiece& food : foodList){
            DrawSquare(food.position.x, food.position.y, snake.bodySize, food.color);
        }
        if(!gameEnd){
            CheckInput(playerCommands);
            if(moveTimer >= snake.speed){
                snake.Move(playerCommands);
                moveTimer = 0;
            }
            moveTimer++;
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
