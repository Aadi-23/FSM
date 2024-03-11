#pragma once
#include "raylib.h"
#include "IStateMachine.h"
#include "myMath.h"
#include "raymath.h"
#include "vector"



enum EntityType {NONE, GOAT,GRASS,WOLF};
struct EntityDescription
{
	Vector2 position;
	float size;
	Vector2 velocity;
	EntityType entitytype;
	float health = 100;
	Color color;
};

class Level;

class Entity
{
protected:
	void Wander(Entity* entity,Vector2 destination);
	Vector2 SetRandomPosition(Vector2& position);
	void MoveToEntity(Entity* e, Vector2 destination, float max_velocity);
	Vector2 GetDestinationVector(Entity* e, Vector2 destination);
public:
	EntityDescription entitiesDesc{};
	States currentstate{};
	bool dead = false;
	virtual void Act(Level* level) = 0;
	virtual void Sense(Level* level) = 0;
	virtual void Decide(Level* level) = 0;


	~Entity() = default;
};

class Goat: public Entity
{
	bool _destinationReached = false;
	Vector2 _destination = { 0,0 };
	double _idleTimer = 5.0f;

	float _detectionradius = 200.0f;
	float _foodThresold = 30;     // Minimum amount when goat gets hungry
	bool _isHungry = false;
	float _food = 50;
	float _eatDrinkRate = 0.2f;
	float _damageRate = 0.01f;
	bool _inDanger = true;
	bool _canBreed = false;

	int _evadeIndex = 0;

	Entity* entity;
	
	Vector2 find_escape_route(Entity* entity);
	std::vector<Entity*>wolfsAround;
	void ConsumeResource(Entity* entity);
	
public:
	Goat(Vector2 pos, float hp);
	void Act(Level* level) override;
	void Sense(Level* level) override;
	void Decide(Level* level) override;
};

class Grass : public Entity
{
	float _growth_rate = 0.05f;
	bool _beingTrampled = false;
	bool _spawnGrass = true;
	float _maxSize = 35;
	bool _isGrown = false;


	float _healthGrowingRate = 0.2f;

	double _grownTimer = 2400.0f;

	Entity* _tramplingEntity;
public:
	Grass(Vector2 pos, float hp);
	void Act(Level* level) override;
	void Sense(Level* level) override;
	void Decide(Level* level) override;
};



class Wolf : public Entity
{
	bool _destinationReached = true;
	Vector2 _destination = { 0,0 };
	double _idleTimer = 5.0f;

	float _healthThresold = 100;
	float _eatDrinkRate = 0.2f;
	float _damageRate = 0.025f;
	bool _canBreed = false;

	float _maxSensingDistance = 200;

	Entity* _targetGoat = nullptr;
	void Eat(Entity* entity);
public:
	Wolf(Vector2 pos, float hp);
	void Act(Level* level) override;
	void Sense(Level* level) override;
	void Decide(Level* level) override;
};