#include "Enemy.h"
#include "Engine.h"
#include "Player.h"
#include "Projectile.h"
#include "Asteroid.h"
#include "ShooterEnemy.h"

void Asteroid::Update(float dt)
{
	Actor::Update(dt);

	//if (scene->GetActor<Player>()) {
	//	nc::Vector2 direction = scene->GetActor<Player>()->transform.position - transform.position;
	//	nc::Vector2 forward = nc::Vector2::Rotate(nc::Vector2::right, transform.rotation);

	//	float angle = nc::Vector2::Angle(forward, direction.Normalized());
	//}

	//movement - mainly top one
	transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 866.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 800.0f);
}

void Asteroid::OnCollision(Actor* actor)
{
	//can be killed by players, enimies, and shooters
	//can kill player, enemies, and shooters
	if ((dynamic_cast<Player*>(actor)) || (dynamic_cast<Projectile*>(actor) && actor->tag == "Player")) {
		actor->destroy = true;
		destroy = true;
		scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 5, 1, scene->engine->Get<nc::ResourceSystem>()
			->Get<nc::Texture>("cloud.png", scene->engine->Get<nc::Renderer>()), 60, transform.rotation, nc::DegToRad(30));
		//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 0.5f, nc::Color::red, 50.0f);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");

		nc::Event event;
		event.name = "AddPoints";
		event.data = 300;
		scene->engine->Get<nc::EventSystem>()->Notify(event);
	}

	if (dynamic_cast<Player*>(actor)) {
		destroy = true;
		//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 0.5f, nc::Color::white, 50.0f);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");
	}
}
