#include "Entity.h"
#include "Level.h"

#include "utility.h"
#include <filesystem>

Vector2 Entity::GetDestinationVector(Entity* entity, Vector2 destination)
{
	float max_vel = 20;
	Vector2 result_vector;
	Vector2 desired_velocity = Vector2Normalize(destination - entity->entitiesDesc.position) * max_vel;

	Vector2 steering_vector = desired_velocity - (entity->entitiesDesc.velocity);
	result_vector = entity->entitiesDesc.velocity + steering_vector;
	
	return result_vector;
}

void Entity::Wander(Entity* entity,Vector2 destination)
{
	Vector2 wander_veloctiy = GetDestinationVector(entity, destination);
	entity->entitiesDesc.velocity = wander_veloctiy;
	entity->entitiesDesc.position += entity->entitiesDesc.velocity * GetFrameTime();
}



void Entity::MoveToEntity(Entity* e, Vector2 destination, float max_velocity)
{

	Vector2 offset = { e->entitiesDesc.size,e->entitiesDesc.size};
	Vector2 destination_pos = destination - offset;

	Vector2 wander_veloctiy = GetDestinationVector(e, destination);
	e->entitiesDesc.velocity = wander_veloctiy*max_velocity;
	if (OutofScreen(e->entitiesDesc.position, e->entitiesDesc.size))return;
	e->entitiesDesc.position += e->entitiesDesc.velocity * GetFrameTime();
}


Vector2 Entity::SetRandomPosition(Vector2& position)
{
	return position + Random_Vector();
}

Goat::Goat(Vector2 pos, float hp)
{
	currentstate = States::IDLE;
	dead = false;
	entitiesDesc.position = pos;
	entitiesDesc.velocity = { 5,5 };
	entitiesDesc.size = 40;
	entitiesDesc.entitytype = GOAT;
	entitiesDesc.color = WHITE;
	_destination = SetRandomPosition(entitiesDesc.position);
	entitiesDesc.health = hp;
	entity = nullptr;
}

void Goat::ConsumeResource(Entity* entity)
{
	if (entity->entitiesDesc.entitytype == GRASS)
	{
		_food += _eatDrinkRate;
		entitiesDesc.health += _eatDrinkRate;
		entity->entitiesDesc.health -= _eatDrinkRate;
	}
}

Vector2 Goat::find_escape_route(Entity* entity)
{
	Vector2 result = {};
	float max_velocity = 30;
	if (wolfsAround.size() == 1)
	{
		result = GetDestinationVector(entity, wolfsAround.front()->entitiesDesc.position);
		return Vector2Negate(result);;
	}
	else
	{
		for (auto w : wolfsAround)
		{
			Vector2 temp_vector = (Vector2Normalize((w->entitiesDesc.position - entitiesDesc.position) + result));
			result = temp_vector * max_velocity;
		}
	}
	return Vector2Negate(result);
}

void Goat::Act(Level* level)
{
	switch (currentstate)
	{
	case IDLE:
	{
		entitiesDesc.color = WHITE;
		entitiesDesc.health -= _damageRate;
	}
		break;
	case WANDER:
	{
		entitiesDesc.color = WHITE;
		Wander(this, _destination);
		entitiesDesc.health -= _damageRate;
	}
		break;
	case EAT:
	{
		ConsumeResource(entity);
	}
		break;
	case EVADE:
	{
		_evadeIndex++;
		entitiesDesc.color = YELLOW;
		entitiesDesc.health -= _damageRate;
		Vector2 evadeVector = find_escape_route(this);
		entitiesDesc.velocity = evadeVector;
		if (OutofScreen(entitiesDesc.position, entitiesDesc.size)) return;
		entitiesDesc.position += entitiesDesc.velocity* GetFrameTime();
	}
		break;
	case FIND:
	{
		entitiesDesc.health -= _damageRate;
	}
		break;
	case PURSUE:
	{
		MoveToEntity(this, entity->entitiesDesc.position, 1.0f);
		entitiesDesc.health -= _damageRate;
	}
		break;
	case(BREED):
	{
		entitiesDesc.health -= _damageRate;
		if (!_canBreed) return;
		Vector2 spawnoffset = { 40,40 };
		Vector2 spawnPos = entitiesDesc.position + spawnoffset;
		float spawningHealth = 20;
		level->SpawnGoat(spawnPos, spawningHealth);

		_canBreed = false;
		entitiesDesc.health = 10;
	}
	break;
	default:
		break;
	}

	
}

void Goat::Sense(Level* level)
{
	// Check if hungry
	if (entitiesDesc.health < _foodThresold) _isHungry = true;
	else
	{
		_isHungry = false;
	}

	if (level->wolves.empty()) wolfsAround.clear();
	// Sense the wolfs around
	for (Entity& e : level->wolves)
	{
		
		if (Vector2Distance(entitiesDesc.position, e.entitiesDesc.position) < _detectionradius)
		{
			auto entity_found = std::find(wolfsAround.begin(), wolfsAround.end(), &e);
			if ( entity_found != wolfsAround.end())
			{
				continue;
			}
			wolfsAround.push_back(&e);
		}
		else
		{
			auto entity_found = std::find(wolfsAround.begin(), wolfsAround.end(), &e);
			if (entity_found != wolfsAround.end())
			{
				wolfsAround.erase(entity_found, wolfsAround.end());
			}
		}
	}

	// Check if max health reached
	if (entitiesDesc.health >= 100)
	{
		_canBreed = true;
	}

}

void Goat::Decide(Level* level)
{
	

	if (wolfsAround.size() > 0)
	{
		currentstate = EVADE;
		_inDanger = true;

	}

	
	switch (currentstate)
	{
	case IDLE:
	{
		if (GetTime() >= _idleTimer)
		{
			currentstate = WANDER;
			_idleTimer = 5.0f + GetTime();
		}

		if (_isHungry) currentstate = FIND;
	}
		break;
	case WANDER:
	{
		if (_canBreed)
		{
			currentstate = BREED;
			_idleTimer = 5.0f;
			return;
		}
		if (entity && Vector2Distance(entitiesDesc.position, entity->entitiesDesc.position) < entity->entitiesDesc.size)
		{
			currentstate = EAT;
			_idleTimer = 5.0f;
			return;
		}
		for (auto* e : level->all_entities)
		{
			if (Vector2Equals(e->entitiesDesc.position, entitiesDesc.position)) continue;
			if (_idleTimer >= GetTime() || OutofScreen(entitiesDesc.position, entitiesDesc.size) || CheckCollisionCircles(entitiesDesc.position, entitiesDesc.size, e->entitiesDesc.position, e->entitiesDesc.size))
			{
				_destination = SetRandomPosition(entitiesDesc.position);
				_idleTimer = 5.0f;
			}
			
			
		}
		if (CheckCollisionPointCircle(_destination, entitiesDesc.position, entitiesDesc.size)) _destinationReached = true;

		if (_destinationReached)
		{
			_idleTimer = 5.0f + GetTime();
			_destination = SetRandomPosition(entitiesDesc.position);

			currentstate = IDLE;
			_destinationReached = false;
		}
		if (_isHungry)
		{
			currentstate = FIND;
			_idleTimer = 5.0f;
		}
	}
		break;
	case EAT:
	{
		if (entity->entitiesDesc.health <= 0)
		{
			_idleTimer = 5.0f + GetTime();
			currentstate = IDLE;
			entity = nullptr;
		}
	}
		break;
	case EVADE:
	{
		
		if (_evadeIndex >= 300)
		{
			_inDanger = false;
		}
		if ((_evadeIndex >= 1200 || wolfsAround.empty()) && !_inDanger)
		{
			_idleTimer = 5.0f + GetTime();
			_evadeIndex = 0;
			currentstate = IDLE;
		}
	}
		break;
	case FIND:
	{
		if (_isHungry)
		{
			entity = level->findClosestEntity(GRASS, entitiesDesc.position, INFINITY);
			if (entity)currentstate = PURSUE;
			else
			{
				_idleTimer = 5.0f + GetTime();
				currentstate = IDLE;
			}
		}
	}
		break;
	case PURSUE:
	{
		if (CheckCollisionCircles(entity->entitiesDesc.position,entity->entitiesDesc.size, entitiesDesc.position, entitiesDesc.size)) _destinationReached = true;
		if (_destinationReached)
		{
			currentstate = EAT;
			_destinationReached = false;
		}
	}
		break;
	case(BREED):
	{
		if (_canBreed == false)
		{
			_idleTimer = 5.0f + GetTime();
			currentstate = IDLE;
		}
	}
	break;
	default:
		break;
	}
}





Grass::Grass(Vector2 pos, float hp)
{
	currentstate = States::IDLE;
	dead = false;
	entitiesDesc.position = pos;
	entitiesDesc.size = hp* 0.4f;
	entitiesDesc.velocity = { 10,0 };
	entitiesDesc.entitytype = GRASS;
	entitiesDesc.health = hp;
	entitiesDesc.color = GREEN;
	_tramplingEntity = nullptr;
}


void Grass::Act(Level* level)
{
	switch (currentstate)
	{
	case IDLE:
		if (_beingTrampled) entitiesDesc.health -= _growth_rate/2.0f;
		break;
	case EAT:
		break;
	case GROW:
	{
		entitiesDesc.size += _growth_rate;
		entitiesDesc.health += _healthGrowingRate;
	}
		break;
	case GROWN:
	{
		_grownTimer--;
		entitiesDesc.color = DARKGREEN;

		if (_grownTimer < 0)
		{
			dead = true;
		}
	}
		break;
	default:
		break;
	}
}
void Grass::Sense(Level* level)
{
	if (entitiesDesc.size >= _maxSize)
	{
		_isGrown = true;
	}

	if (currentstate == IDLE && _tramplingEntity)
	{
		if (!CheckCollisionCircles(entitiesDesc.position, entitiesDesc.size, _tramplingEntity->entitiesDesc.position, _tramplingEntity->entitiesDesc.size))
		{
			_beingTrampled = false;
		}
	}
	// Check for trampling on grass
	for (auto& e : level->all_entities)
	{
		if (e->entitiesDesc.entitytype == GRASS) continue;
		if (CheckCollisionCircles(entitiesDesc.position,entitiesDesc.size, e->entitiesDesc.position, e->entitiesDesc.size))
		{
			_beingTrampled = true;
			_tramplingEntity = e;
			break;
		}
	}

	
}

void Grass::Decide(Level* level)
{
	switch (currentstate)
	{
	case IDLE:
	{
		if (_isGrown && !_beingTrampled)
		{
			currentstate = GROWN;
			return;
		}
		if (!_beingTrampled) currentstate = GROW;
	}
		break;
	case GROW:
	{
		if (_beingTrampled)
		{
			currentstate = IDLE;
			return;
		}
		if (_isGrown) currentstate = GROWN;
	}
		break;
	case GROWN:
	{
		if (_beingTrampled) currentstate = IDLE;
		if (_spawnGrass && level->grassCount() < level->maxGrassThresold)
		{
			_spawnGrass = false;
			for (int i = 0; i < 2; i++)
			{
				Vector2 adjacent_tile = GetRandomAdjacentTile();
				Vector2 spawn_pos = entitiesDesc.position + level->TransformPosInGrid(adjacent_tile.x, adjacent_tile.y);
				if (OutofScreen(spawn_pos, entitiesDesc.size)) break;
				for (auto g : level->all_entities)
				{
					if (Vector2Equals(spawn_pos, g->entitiesDesc.position)) return;
					
				}
				float spawningHealth = 20;
				level->SpawnGrass(spawn_pos, spawningHealth);
			}
				
		}
		
		
	}
		break;
	default:
		break;
	}
}







Wolf::Wolf(Vector2 pos, float hp)
{
	currentstate = States::WANDER;
	dead = false;
	entitiesDesc.position = pos;
	entitiesDesc.velocity = { 1,1 };
	entitiesDesc.size = 40;
	entitiesDesc.entitytype = WOLF;
	_destination = SetRandomPosition(entitiesDesc.position);
	entitiesDesc.color = ORANGE;
	entitiesDesc.health = hp;
}



void Wolf::Eat(Entity* entity)
{
	entitiesDesc.health += _eatDrinkRate;
	entity->entitiesDesc.health -= _eatDrinkRate;
}

void Wolf::Act(Level* level)
{
	switch (currentstate)
	{
	case IDLE:
	{
		if (GetTime() >= _idleTimer)
		{
			currentstate = WANDER;
			_idleTimer = 5.0f;
		}
		entitiesDesc.health -= _damageRate;
	}
		break;
	case WANDER:
	{
		Wander(this, _destination);
		entitiesDesc.health -= _damageRate;
	}
		break;
	case EAT:
	{
		Eat(_targetGoat);
		entitiesDesc.color = RED;
	}
		break;
	case BREED:
	{
		entitiesDesc.health -= _damageRate;
		if (!_canBreed) return;
		Vector2 spawnoffset = { 40,40 };
		Vector2 spawnPos = entitiesDesc.position + spawnoffset;
		float spawningHealth = 50;
		level->SpawnWolf(spawnPos, spawningHealth);
		_canBreed = false;
		entitiesDesc.health = 30;
	}
		break;
	case PURSUE:
	{
		entitiesDesc.health -= _damageRate;
		MoveToEntity(this, _targetGoat->entitiesDesc.position, 1.75f);
	}
		break;
	default:
		break;
	}
}

void Wolf::Sense(Level* level)
{
	if (_destinationReached)
	{
		_destinationReached = false;
		_idleTimer += GetTime();
	}

	if (entitiesDesc.health >= _healthThresold) _canBreed = true;

	for (Goat& g : level->goats)
	{

		if (Vector2Distance(g.entitiesDesc.position, entitiesDesc.position) <= _maxSensingDistance)
		{
			_targetGoat = &g;
		}
	}
}

void Wolf::Decide(Level* level)
{
	if (_canBreed) currentstate = BREED;

	switch (currentstate)
	{
	case IDLE:
	{
		if (GetTime() >= _idleTimer)
		{
			currentstate = WANDER;
			_idleTimer = 5.0f;
		}
	}
		break;
	case WANDER:
	{
		if (_targetGoat && entitiesDesc.health < 90) currentstate = PURSUE;
		for (auto* e : level->all_entities)
		{
			if (Vector2Equals(e->entitiesDesc.position, entitiesDesc.position)) continue;
			if (OutofScreen(entitiesDesc.position, entitiesDesc.size) || CheckCollisionCircles(entitiesDesc.position, entitiesDesc.size, e->entitiesDesc.position, e->entitiesDesc.size))
			{
				_destination = SetRandomPosition(entitiesDesc.position);
			}


		}
		if (CheckCollisionPointCircle(_destination, entitiesDesc.position, entitiesDesc.size)) _destinationReached = true;

		if (_destinationReached)
		{
			_idleTimer += GetTime();
			_destination = SetRandomPosition(entitiesDesc.position);

			currentstate = IDLE;
			_destinationReached = false;
		}
	}
		break;
	case EAT:
	{
		if (Vector2Distance(_targetGoat->entitiesDesc.position, entitiesDesc.position) > 10.0f || entitiesDesc.health >= 100) currentstate = WANDER;
		if (_targetGoat->entitiesDesc.health <= 0)
		{
			entitiesDesc.color = ORANGE;
			currentstate = WANDER;
			_targetGoat = nullptr;
		}
	}
		break;
	case BREED:
	{
		if (!_canBreed) currentstate = WANDER;
	}
		break;
	case PURSUE:
	{
		if (Vector2Distance(_targetGoat->entitiesDesc.position, entitiesDesc.position) < _targetGoat->entitiesDesc.size) _destinationReached = true;
		if (_destinationReached)
		{
			currentstate = EAT;
			_destinationReached = false;
		}
	}
		break;
	default:
		break;
	}
}
