#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include "mysteryship.hpp"

class Game{
	public:
		Game();
		~Game();
		void Draw();
		void Update();
		void HandleInput();
		int lives;
		bool run;
		int score;
		int highscore;
		
	private:
		void DeleteInactiveLasers();
		std::vector<Obstacle> CreateObstacles();
		std::vector<Alien> CreateAliens();
		void MoveAliens();
		void MoveDownAliens(int distance);
		void AlienShootLaser();
		Spaceship spaceship;
		void GameOver();
		std::vector<Obstacle> obstacles;
		std::vector<Alien> aliens;
		int aliensDirection;
		std::vector<Laser> alienLasers;
		constexpr static float alienLaserShootInterval = 0.35;
		float timeLastAlienFired;
		MysteryShip mysteryship;
		float mysteryshipSpawnInterval;
		float timeLastSpawn;
		void CheckForCollisions();
		void Reset();
		void initgame();
		void CheckforHighscore();
		void saveHighscoretoFile(int highscore);
		int loadHighscoreFromFile();
};
