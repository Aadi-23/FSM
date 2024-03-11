#pragma once
#include "Entity.h"

#include "list"
#include "string"

class Level
{
private:

	int initial_goat_amount = 10;
	int initial_grass_amount = 4;
	int initial_wolves_amount = 1;

	std::string gamelevel{};

	std::list<Grass> grass;
	

	std::vector<Goat>tempgoats;
	std::vector<Grass>tempgrass;
	std::vector<Wolf>tempwolf;


	int _frameCountIndex = 0;
public:
	std::list<Goat> goats;
	std::list<Wolf>wolves;
	std::vector<Entity*> all_entities;
	bool game_over = false;

	int maxGrassThresold = 25;

	int grassCount() { return (int)grass.size(); }
	void LoadLevelFromTxt();
	void SpawnGoat(Vector2 spawnpos, float spawnhealth);
	void SpawnGrass(Vector2 spawnpos, float spawnhealth);
	void SpawnWolf(Vector2 spawnpos, float spawnhealth);


	void Draw_HealthBar();
	void LevelLoader();
	Vector2 TransformPosInGrid(float x, float y);
	Entity* findClosestEntity(EntityType kind, Vector2 PosInWorld, float max_distance);

	void RemoveEntity();
	void LevelReset();
	void LevelUpdate();
	void LevelRender();
};