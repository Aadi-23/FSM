#include "Level.h"


void Level::LoadLevelFromTxt()
{
	constexpr const char* LevelFromTxt = "./level.txt";
    
	gamelevel = LoadFileText(LevelFromTxt);
}

void Level::SpawnGoat(Vector2 spawnpos, float spawnhealth)
{
	Goat goat(spawnpos, spawnhealth);

	tempgoats.push_back(goat);
}

void Level::SpawnGrass(Vector2 spawnpos, float spawnhealth)
{
	Grass t_grass{ spawnpos,spawnhealth };
	tempgrass.push_back(t_grass);
}

void Level::SpawnWolf(Vector2 spawnpos, float spawnhealth)
{
	Wolf t_wolf(spawnpos, spawnhealth);
	tempwolf.push_back(t_wolf);
}





Entity* Level::findClosestEntity(EntityType kind, Vector2 position_in_world, float max_distance) {
	Entity* result = nullptr;
	float current_closest_distance = max_distance;

	for (Entity* e : all_entities)
	{
		if (e->entitiesDesc.entitytype != kind) continue;

		float distance = Vector2Distance(position_in_world , e->entitiesDesc.position);
		if (distance < current_closest_distance) {
			current_closest_distance = distance;
			result = e;
		}
	}

	return result;
}


void Level::RemoveEntity()
{
	auto last_enetity = std::remove_if(all_entities.begin(), all_entities.end(), [](const Entity* e)->bool {return e->dead; });
	all_entities.erase(last_enetity, all_entities.end());

	goats.remove_if([](const Goat& g)->bool {return g.dead; });
	grass.remove_if([](const Grass& g)->bool {return g.dead; });
	wolves.remove_if([](const Wolf& w)->bool {return w.dead; });


	while (tempgoats.size () > 0)
	{
		goats.push_back(tempgoats.back());
		all_entities.push_back(&goats.back());
		tempgoats.pop_back();
	}
	while (tempgrass.size() > 0)
	{
		grass.push_back(tempgrass.back());
		all_entities.push_back(&grass.back());
		tempgrass.pop_back();
	}
	while (tempwolf.size() > 0)
	{
		wolves.push_back(tempwolf.back());
		all_entities.push_back(&wolves.back());
		tempwolf.pop_back();
	}
}


void Level::LevelUpdate()
{
	if (++_frameCountIndex >= 5)
	{
		for (auto& e : all_entities)
		{
			e->Sense(this);
			e->Decide(this);
		}
		
		_frameCountIndex = 0;
	}
	
	for (auto& e : all_entities)
	{
		e->Act(this);
		if (e->entitiesDesc.health < 0)
		{
			e->dead = true;
		}
	}
	RemoveEntity();

	if (all_entities.empty())
	{
		game_over = true;
	}


	
}


void Level::Draw_HealthBar()
{

	for (auto e : all_entities)
	{
		Vector2 healthBar_offset = { 25,50 };
		Vector2 healthbar_size = { (float)e->entitiesDesc.health/2, 10 };
		DrawRectangleV((e->entitiesDesc.position - healthBar_offset), healthbar_size, GREEN);
	}
}

void Level::LevelRender()
{
	for (Entity* e : all_entities)
	{
		switch (e->entitiesDesc.entitytype)
		{
		case(GOAT):
		{
			DrawCircleV(e->entitiesDesc.position, 40,e->entitiesDesc.color);
		}
		break;
		case(GRASS):
		{
			DrawCircleV(e->entitiesDesc.position, e->entitiesDesc.size, e->entitiesDesc.color);
		}
		break;
		case(WOLF):
		{
			DrawCircleV(e->entitiesDesc.position, 40, e->entitiesDesc.color);
		}
		break;
		default:
			break;
		}
	}

	Draw_HealthBar();
}

void Level:: LevelReset()
{
	if (!all_entities.empty())all_entities.clear();
	goats.clear();
	grass.clear();
	wolves.clear();

	game_over = false;
}


Vector2 Level::TransformPosInGrid(float x,float y)
{
	Vector2 transformed_position;

	transformed_position.x = x * 40;
	transformed_position.y = y * 40;


	return transformed_position;
}

void Level::LevelLoader()
{
	
	//Spawn Goat
	for (int i = 0; i < initial_goat_amount; i++)
	{
		Vector2 random_spamn_pos = { (float)GetRandomValue(50, GetRenderWidth()- 40), (float)GetRandomValue(50,GetRenderHeight()- 40) };
		SpawnGoat(random_spamn_pos, 100);
	}

	//SpawnGrass
	for (int j = 0; j < initial_grass_amount; j++)
	{
		Vector2 random_spamn_pos = { (float)GetRandomValue(50, GetRenderWidth()-50), (float)GetRandomValue(50,GetRenderHeight()-50) };
		SpawnGrass(random_spamn_pos, 100);
	}

	// SpawnWolves
	for (int k = 0; k < initial_wolves_amount; k++)
	{
		Vector2 random_spamn_pos = { (float)GetRandomValue(50, GetRenderWidth()-50), (float)GetRandomValue(50,GetRenderHeight()-50) };
		SpawnWolf(random_spamn_pos, 100);
	}
}

