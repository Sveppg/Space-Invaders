#include "game.hpp"
#include "alien.hpp"
#include "laser.hpp"
#include "mysteryship.hpp"
#include <iostream>
#include <raylib.h>
#include <fstream>

Game::Game(){
	initgame();
}

Game::~Game(){
	Alien::UnloadImages();
}

void Game::Update(){
	if(run){
		double currentTime = GetTime();
		if(currentTime - timeLastSpawn > mysteryshipSpawnInterval){
			mysteryship.Spawn();
			timeLastSpawn = GetTime();
			mysteryshipSpawnInterval = GetRandomValue(10, 20);
		}

		for(auto& laser: spaceship.lasers){
			laser.Update();
		}
		AlienShootLaser();
		for(auto& laser: alienLasers){
			laser.Update();
		}

		MoveAliens();
		DeleteInactiveLasers();
		mysteryship.Update();
		CheckForCollisions();
	}
	else{
		if(IsKeyDown(KEY_ENTER)){
			Reset();
			initgame();
		}
	}
}

void Game::Draw(){
	spaceship.Draw();
	for(auto& laser: spaceship.lasers){
		laser.Draw();
	}
	for(auto& obstacle: obstacles){
		obstacle.Draw();
	}
	for(auto& alien: aliens){
		alien.Draw();
	}
	for(auto&  laser: alienLasers){
		laser.Draw();
	}
	mysteryship.Draw();
}

void Game::HandleInput(){
	if(run){	
		if(IsKeyDown(KEY_LEFT)){
			spaceship.MoveLeft();
		}else if(IsKeyDown(KEY_RIGHT)){
			spaceship.MoveRight();
		}else if(IsKeyDown(KEY_SPACE)){
			spaceship.FireLaser();
		}
	}
}

void Game::DeleteInactiveLasers(){
	for(auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();){
		if(!it -> active){
			it = spaceship.lasers.erase(it);
		}else{
			++ it;
		}
	}

	for(auto it = alienLasers.begin(); it != alienLasers.end();){
		if(!it -> active){
			it = alienLasers.erase(it);
		}else{
			++ it;
		}
	}
}

std::vector<Obstacle> Game::CreateObstacles(){
	int obstacleWidth = Obstacle::grid[0].size() * 3;
	float gap = (GetScreenWidth() - (4* obstacleWidth))/5;
	for(int i = 0; i < 4 ; i++){
		float offsetX = (i+1) * gap +i * obstacleWidth;
		obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() - 200)}));
	}
	return obstacles;  
}

/*
* Class for creating the alien image source /src/Graphics/alien 1-3
*/
std::vector<Alien> Game::CreateAliens(){
	std::vector<Alien> aliens;
	for(int row =0; row < 5 ; row++){
		for(int column = 0; column < 11; column++){
			int alienType; 
			if(row == 0){
				alienType = 3;
			}else if(row == 1){
				alienType = 2;
			}else{
				alienType = 1;
			}
			float cellSize = 55;
			float x = 75 + column * cellSize; 
			float y = 110 + row* cellSize;
			aliens.push_back(Alien(alienType, {x, y }));
		}
	}
	return aliens;
}

void Game::MoveAliens(){
	for(auto& alien: aliens){
		if(alien.position.x + alien.alienImages[alien.type -1].width > GetScreenWidth()-25){
			aliensDirection = -1;
			MoveDownAliens(4);
		}
		if(alien.position.x < 25){
			aliensDirection = 1;
			MoveDownAliens(4);
		}
	
		alien.Update(aliensDirection);
	}
}

/*
*	Moving Down Class Alien position y 
*/
void Game::MoveDownAliens(int distance){
	for(auto& alien: aliens){
		alien.position.y += distance;
	}
}


void Game::AlienShootLaser(){
	
	double currentTime = GetTime();
	if(currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty()){
		int randomIndex = GetRandomValue(0, aliens.size()-1);
		Alien& alien = aliens[randomIndex];
		alienLasers.push_back(Laser({alien.position.x + alien.alienImages[alien.type -1].width/2,
														alien.position.y + alien.alienImages[alien.type - 1].height}, 6));
		timeLastAlienFired = GetTime();
	}
}



void Game::CheckForCollisions(){
	for(auto& laser: spaceship.lasers){
		auto it = aliens.begin();
		while(it != aliens.end()){
			if(CheckCollisionRecs(it -> getRect(), laser.getRect())){
				if(it -> type == 1){
					score += 100;
				}else if(it -> type == 2){
					score += 200;
				}else if(it -> type == 3){
					score += 300;
				}
				CheckforHighscore();
				
				
				it = aliens.erase(it);
				laser.active = false;

			}else{
				++it;
			}
		}
		for(auto& obstacle: obstacles){
			auto it = obstacle.blocks.begin();
			while(it != obstacle.blocks.end()){
				if(CheckCollisionRecs(it-> getRect(), laser.getRect())){
					it = obstacle.blocks.erase(it);
					laser.active = false;
				}else{
					++it;
				}
			}
		}
		if(CheckCollisionRecs(mysteryship.getRect(), laser.getRect())){
			mysteryship.alive = false;
			laser.active = false;
			score += 500;
			CheckforHighscore();
		}
	}
	//alien lasers
	for(auto& laser: alienLasers){
		if(CheckCollisionRecs(laser.getRect(),spaceship.getRect())){
			laser.active = false;
			lives --;
			if(lives == 0){
				GameOver();
			}
		}
			for(auto& obstacle: obstacles){
			auto it = obstacle.blocks.begin();
			while(it != obstacle.blocks.end()){
				if(CheckCollisionRecs(it-> getRect(), laser.getRect())){
					it = obstacle.blocks.erase(it);
					laser.active = false;
				}else{
					++it;
				}
			}
		}
	}

	//alien collision with obstacle
	for(auto& alien: aliens){
		for(auto& obstacle: obstacles){
			auto it = obstacle.blocks.begin();
			while(it != obstacle.blocks.end()){
				if(CheckCollisionRecs(it -> getRect(), alien.getRect())){
					it = obstacle.blocks.erase(it);
				}else{
					it ++;
				}
			}
		}
	
	if(CheckCollisionRecs(alien.getRect(), spaceship.getRect())){
		std::cout <<"spaceship hit by alien" << std::endl;
	}
	}
}

void Game::GameOver(){
	std::cout << "Game Over!" << std::endl;
	run = false;
}


void Game::initgame(){
	obstacles = CreateObstacles();
	aliens = CreateAliens();
	aliensDirection = 1;
	timeLastAlienFired = 0.0;
	timeLastSpawn = 0.0;
	mysteryshipSpawnInterval = GetRandomValue(10,20);
	lives = 3;
	run = true;
	score = 0;
	highscore = loadHighscoreFromFile();
}


void Game::CheckforHighscore(){
	if(score > highscore){
		highscore = score;
		saveHighscoretoFile(highscore);
	}
}

void Game::saveHighscoretoFile(int highscore){
	std::ofstream highScoreFile("highscore.txt");
	if(highScoreFile.is_open()){
		highScoreFile << highscore;
		highScoreFile.close();
	}else{
		std::cerr << "failed to save highscore" << std::endl;
	}

}
int Game::loadHighscoreFromFile(){
	int loadedHighScore = 0;
	std::ifstream highScoreFile("highscore.txt");
	if(highScoreFile.is_open()){
		highScoreFile >> loadedHighScore;
		highScoreFile.close();
	}else{
		std::cerr << "Failed to load file" << std::endl;
	}
	return loadedHighScore;
}

void Game::Reset(){
	spaceship.Reset();
	aliens.clear();
	alienLasers.clear();
	obstacles.clear();
}

