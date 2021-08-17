#pragma once
#include "Object/Actor.h"

class ShooterEnemy : public nc::Actor {
public:
	ShooterEnemy(const nc::Transform& transform, std::shared_ptr<nc::Texture> shape, float speed) : nc::Actor{ transform, shape }, speed{ speed }{};

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float speed = 300;
	float fireTimer = 0;
	float fireRate = 1;
};