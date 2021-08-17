#pragma once
#include "Object/Actor.h"

class Player : public nc::Actor {
public:
	Player(const nc::Transform& transform, std::shared_ptr<nc::Texture> shape, float speed);

	void Initialize() override;

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float fireTimer = 1;
	float fireRate = 0;
	float speed = 300;
	nc::Vector2 velocity;

	const float iDir = 2.0f;
	float iTimer = iDir;

};