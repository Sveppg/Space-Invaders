#pragma once
#include <raylib.h>
#include <vector>
#include "block.hpp"

class Obstacle{
    public: 
        Obstacle(Vector2 position);
            void Draw();
            Vector2 position;
            std::vector<Block> blocks;
            static std::vector<std::vector<int>> grid;

    private:
};