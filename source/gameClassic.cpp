#include <queue>
#include "libraryBindings.h"

enum class Direction{
    None,
    Up,
    Left,
    Down,
    Right
};

void CheckInput(std::queue<Direction>& playerCommands){
    if(IsKeyPressed(KEY_RIGHT)){
        playerCommands.push(Direction::Right);
    }
    else if(IsKeyPressed(KEY_LEFT)){
        playerCommands.push(Direction::Left);
    }
    else if(IsKeyPressed(KEY_DOWN)){
        playerCommands.push(Direction::Down);
    }
    else if(IsKeyPressed(KEY_UP)){
        playerCommands.push(Direction::Up);
    }
}

class Pos{
public:
    int x;
    int y;
    Pos() : x(0), y(0) {}
    Pos(int aX, int aY) : x(aX), y(aY) {}
    bool operator==(const Pos& b){
        if(x == b.x && y == b.y){
            return true;
        }
        return false;
    }
    bool operator!=(const Pos& b){
        return !operator==(b);
    }
};

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
    std::vector<SnakeBodyPiece> snake;
    Direction dir = Direction::None;
    int speed = 10;
    int squareSize = 20;
    const DirectionLookup dirLookup[5] = {
        {Direction::None, Pos(0, 0)},
        {Direction::Down, Pos(0, -1)},
        {Direction::Right, Pos(-1, 0)},
        {Direction::Up, Pos(0, 1)},
        {Direction::Left, Pos(1, 0)}
    };
    Snake(){
        for(int i=0; i<10; i++){
            snake.push_back(SnakeBodyPiece(Pos(50, 50), Color(0, 127, 255, 255)));
        }
    }
    void Move(std::queue<Direction>& playerCommands){
        for(int i=snake.size()-1; i>0; i--){
            snake[i].pos = snake[i-1].pos;
        }
        Pos previousPos = snake[0].pos;
        while(playerCommands.size() > 0){
            Direction newDir = playerCommands.front();
            playerCommands.pop();
            if(newDir != dirLookup[(int)dir].disallowed && dir != newDir){
                dir = newDir;
                snake[0].pos.x += dirLookup[(int)newDir].toPos.x * squareSize;
                snake[0].pos.y += dirLookup[(int)newDir].toPos.y * squareSize;
                break;
            }
        }
        // If player commands didn't move the snake, continue moving forward in same direction.
        if(previousPos == snake[0].pos){
            snake[0].pos.x += dirLookup[(int)dir].toPos.x * squareSize;
            snake[0].pos.y += dirLookup[(int)dir].toPos.y * squareSize;
        }
    }
    void Draw(){
        for(SnakeBodyPiece& p : snake){
            DrawSquare(p.pos.x, p.pos.y, squareSize, p.color);
        }
    }
};

void Game(){
    int x = 200, y = 200;
    std::queue<Direction> playerCommands;
    Snake snake;
    int moveTimer = 0;
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(Color(0, 0, 0, 255));
        DrawFPS(10, 10);
        snake.Draw();
        CheckInput(playerCommands);
        if(moveTimer >= snake.speed){
            snake.Move(playerCommands);
            moveTimer = 0;
        }
        moveTimer++;
        EndDrawing();
    }
}
