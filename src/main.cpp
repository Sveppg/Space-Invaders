#include <raylib.h>
#include "game.hpp"
#include <string>

std::string FormatWithLeadingZeros(int number, int width){
    std::string numberText = std::to_string(number);
    int leadingZeros = width - numberText.length();
    return numberText = std::string(leadingZeros, '0') +numberText;
}

int main(){
    
    Color pink = {255, 97, 202, 255};
    Color grey = {29, 29, 27, 255};
    Color magenta = {133, 133, 173, 255};

    //Color yellow = {243, 216,63, 255};
    int offset = 50;
    int windowWidth = 750; 
    int windowHeight = 700;
       
    InitWindow(windowWidth +offset, windowHeight + 2*offset, "C++ Space Invaders");
    
    Font font = LoadFontEx("Font/GrahamBlack-PV3xg.ttf",64, 0, 0);
    Texture2D spaceshipimage = LoadTexture("Graphics/spaceship.png");
    SetTargetFPS(60);

    Game game;

    while(WindowShouldClose() == false){   
        game.HandleInput();
        game.Update();
        BeginDrawing();
        ClearBackground(grey);
        //DrawRectangleRoundedLines({10,10,780,780,},0.18f,20,2,yellow);
        DrawLineEx({25, 730},{775, 730}, 3, magenta);
        if(game.run){
            DrawTextEx(font, "LEVEL 01", {540, 740}, 30, 2, magenta);    
        }else{
            DrawTextEx(font, "GAME OVER!", {540, 740}, 30, 2, magenta);
        }
        float x = 50.0;
        for(int i = 0; i  < game.lives; i++){
            DrawTextureV(spaceshipimage, {x, 745}, WHITE);
            x += 50.0;
        }
        DrawTextEx(font,"SCORE", {50, 15},30, 2, magenta);
        std::string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {50, 40}, 30, 2, magenta);
        
        DrawTextEx(font, "HIGH SCORE", {570, 15}, 34, 2, pink);
        std::string highscoreText = FormatWithLeadingZeros(game.highscore, 5);
        DrawTextEx(font, highscoreText.c_str(), {655, 40}, 43, 2, magenta);
        
        game.Draw();
        EndDrawing();
    }
    UnloadFont(font);
    UnloadTexture(spaceshipimage);
    //game.~Game();
    CloseWindow();   
    return 0;
}

