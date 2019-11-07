#include "libraryBindings.h"

Color::Color(unsigned char aR, unsigned char aG, unsigned char aB, unsigned char aA) : r(aR), g(aG), b(aB), a(aA){}

#ifdef RAYLIB
namespace Raylib{
    #include "raylib.h"
}

void InitWindow(int resolutionX, int resolutionY, const char* windowTitle){
    Raylib::InitWindow(resolutionX, resolutionY, windowTitle);
}

void CloseWindow(){
    Raylib::CloseWindow();
}

void SetTargetFPS(int fps){
    Raylib::SetTargetFPS(fps);
}

bool WindowShouldClose(){
    return Raylib::WindowShouldClose();
}

void BeginDrawing(){
    Raylib::BeginDrawing();
}

void EndDrawing(){
    Raylib::EndDrawing();
}

void ClearBackground(const Color& c){
    Raylib::ClearBackground((Raylib::Color){c.r, c.g, c.b, c.a});
}

void DrawFPS(int posX, int posY){
    Raylib::DrawFPS(posX, posY);
}

void DrawSquare(int posX, int posY, int width, const Color& c){
    Raylib::DrawRectangle(posX, posY, width, width, (Raylib::Color){c.r, c.g, c.b, c.a}); // Second width acts as height to draw a square.
}

bool IsKeyPressed(int key){
    return Raylib::IsKeyPressed(key);
}

bool IsKeyReleased(int key){
    return Raylib::IsKeyReleased(key);
}
#elif defined(SDL2)
#endif
