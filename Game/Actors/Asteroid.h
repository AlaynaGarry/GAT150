#pragma once
#include "Object/Actor.h"

class Asteroid : public nc::Actor {
public:
	Asteroid(const nc::Transform& transform, std::shared_ptr<nc::Texture> shape, float speed) : nc::Actor{ transform, shape }, speed{ speed }{};

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float speed = 300;
};