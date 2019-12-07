#include <string>
#include <vector>
#include <queue>
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

enum class Direction{
    None,
    Up,
    Left,
    Down,
    Right
};

bool ButtonPressed(Direction dir){
    if(dir == Direction::Up){
        if(IsKeyPressed(keyUp) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
            return true;
        }
        return false;
    }
    else if(dir == Direction::Left){
        if(IsKeyPressed(keyLeft) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){
            return true;
        }
        return false;
    }
    else if(dir == Direction::Down){
        if(IsKeyPressed(keyDown) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
            return true;
        }
        return false;
    }
    else if(dir == Direction::Right){
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

class Pos{
public:
    int x;
    int y;
    Pos() : x(0), y(0) {}
    Pos(int aX, int aY) : x(aX), y(aY) {}
    bool operator==(const Pos& b) const{
        if(x == b.x && y == b.y){
            return true;
        }
        return false;
    }
    bool operator!=(const Pos& b) const{
        return !operator==(b);
    }
};

Pos RandomPos(){
    int squaresX = (resolutionX - 2 * snakeBodySize) / snakeBodySize;
    int squaresY = (resolutionY - 2 * snakeBodySize) / snakeBodySize;
    int randomX = rand() % squaresX * snakeBodySize + snakeBodySize;
    int randomY = rand() % squaresY * snakeBodySize + snakeBodySize;
    return Pos(randomX, randomY);
}

class DirectionLookup{
public:
    Direction disallowed;
    Pos toPos;
};

class SnakeBodyPiece{
public:
    Pos pos;
    Color color;
    SnakeBodyPiece(const Pos& aPos, const Color& aColor) : pos(aPos), color(aColor) {}
};

class Snake{
public:
    std::vector<SnakeBodyPiece> body;
    Direction dir = Direction::None;
    int speed;
    int bodySize;
    Pos previousPos;
    const DirectionLookup dirLookup[5] = {
        {Direction::None, Pos(0, 0)},
        {Direction::Down, Pos(0, -1)},
        {Direction::Right, Pos(-1, 0)},
        {Direction::Up, Pos(0, 1)},
        {Direction::Left, Pos(1, 0)}
    };
    Snake(int aSpeed, int abodySize) : speed(aSpeed), bodySize(abodySize){
        body.push_back(SnakeBodyPiece(RandomPos(), Color(0, 127, 255, 255)));
    }
    void Reset(){
        body.clear();
        body.push_back(SnakeBodyPiece(RandomPos(), Color(0, 127, 255, 255)));
        dir = Direction::None;
    }
    void Move(std::queue<Direction>& playerCommands){
        for(int i=body.size()-1; i>0; i--){
            body[i].pos = body[i-1].pos;
        }
        previousPos = body[0].pos;
        while(playerCommands.size() > 0){
            Direction newDir = playerCommands.front();
            playerCommands.pop();
            if(newDir != dirLookup[(int)dir].disallowed && dir != newDir){
                dir = newDir;
                body[0].pos.x += dirLookup[(int)newDir].toPos.x * bodySize;
                body[0].pos.y += dirLookup[(int)newDir].toPos.y * bodySize;
                break;
            }
        }
        // If player commands didn't move the snake, continue moving forward in the same direction.
        if(previousPos == body[0].pos){
            body[0].pos.x += dirLookup[(int)dir].toPos.x * bodySize;
            body[0].pos.y += dirLookup[(int)dir].toPos.y * bodySize;
        }
    }
    void Draw() const{
        for(const SnakeBodyPiece& p : body){
            DrawSquare(p.pos.x, p.pos.y, bodySize, p.color);
        }
    }
    bool PosCollidesWithSnakeOrFood(const Pos& pos, const std::vector<SnakeBodyPiece>& foodList) const{
        for(const SnakeBodyPiece& p : body){
            if(pos == p.pos){
                return true;
            }
        }
        for(const SnakeBodyPiece& food : foodList){
            if(pos == food.pos){
                return true;
            }
        }
        return false;
    }
    Pos RandomPosOutsideSnakeOrFood(const std::vector<SnakeBodyPiece>& foodList) const{
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
            foodList.push_back(SnakeBodyPiece(RandomPosOutsideSnakeOrFood(foodList), Color(255, 0, 0, 255)));
        }
        for(int i=0; i<(int)foodList.size(); i++){
            if(body[0].pos == foodList[i].pos){
                for(int j=0; j<lengthGainPerFood; j++){
                    body.push_back(SnakeBodyPiece(previousPos, Color(0, 127, 255, 255)));
                }
                foodList.erase(foodList.begin()+i);
                i--;
            }
        }
        for(int i=1; i<(int)body.size(); i++){
            if(body[0].pos == body[i].pos){
                return true;
            }
        }
        if(body[0].pos.x > resolutionX - bodySize*2 || body[0].pos.x < bodySize){
            return true;
        }
        else if(body[0].pos.y > resolutionY - bodySize*2 || body[0].pos.y < bodySize){
            return true;
        }
        return false;
    }
};

void Game(){
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
            DrawSquare(food.pos.x, food.pos.y, snake.bodySize, food.color);
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
