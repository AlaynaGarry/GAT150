#include "ShooterEnemy.h"
#include "Engine.h"
#include "Player.h"
#include "Projectile.h"

void ShooterEnemy::Update(float dt) {
	Actor::Update(dt);

	if (scene->GetActor<Player>()) {
		nc::Vector2 direction = scene->GetActor<Player>()->transform.position - transform.position;
		nc::Vector2 forward = nc::Vector2::Rotate(nc::Vector2::right, transform.rotation);

		float turnAngle = nc::Vector2::SignedAngle(forward, direction.Normalized());
		transform.rotation = transform.rotation + turnAngle * (3 * dt);

		float angle = nc::Vector2::Angle(forward, direction.Normalized());

		
		fireTimer -= dt;
		if (fireTimer <= 0 && angle <= nc::DegToRad(10)) {
			fireTimer = fireRate;
			
			//std::vector<nc::Vector2> points = { {-5,-5}, {5,-5}, {0,10}, {-5,-5} };
			
			nc::Transform t = transform;
			t.scale = 0.5f;

			std::unique_ptr<Projectile> projectile = std::make_unique <Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("enemybullet.png", scene->engine->Get<nc::Renderer>()), 600.0f);
			projectile->tag = "Enemy";
			scene->AddActor(std::move(projectile));
			scene->engine->Get<nc::AudioSystem>()->PlayAudio("enemyfire");
		}
	}

	transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 866.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 800.0f);
}

void ShooterEnemy::OnCollision(Actor* actor) {
	//can be killed by player and astroids
	//can kill players and astroids
	if ((dynamic_cast<Player*>(actor)) || (dynamic_cast<Projectile*>(actor)) && actor->tag == "Player") {
		actor->destroy = true;

		destroy = true;
		scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 5, 1, scene->engine->Get<nc::ResourceSystem>()
			->Get<nc::Texture>("bigboom.png", scene->engine->Get<nc::Renderer>()), 60, transform.rotation, nc::DegToRad(30));
		//scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("bigboom.png", scene->engine->Get<nc::Renderer>());
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");

		nc::Event event;
		event.name = "AddPoints";
		event.data = 300;
		scene->engine->Get<nc::EventSystem>()->Notify(event);
	}
}
