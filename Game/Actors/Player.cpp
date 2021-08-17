#include "Player.h"
//#include "Engine.h"
#include "Enemy.h"
#include "ShooterEnemy.h"
#include "Asteroid.h"
#include "Math/MathUtils.h"
#include "Projectile.h"
#include "Engine.h"
#include <memory>

Player::Player(const nc::Transform& transform, std::shared_ptr<nc::Texture> shape, float speed) : nc::Actor{ transform, shape }, speed{ speed }{

}

void Player::Initialize() {

	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ -8,0 };
	//fixes particles 
	//locator->transform.localPosition = nc::DegToRad(180);
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0, 5 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0, -5 };
	AddChild(std::move(locator));

	//rotating engine
	//std::unique_ptr engine = std::make_unique<Actor>(nc::Transform{}, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("engine.txt"));
	//engine->transform.localPosition = nc::Vector2{ -8,0 };
	//AddChild(std::move(engine));
}

void Player::Update(float dt) {
	Actor::Update(dt);

	//children[3]->transform.localRotation += 5 * dt;

	//movement
	float thrust = 0;
	if ((scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == nc::InputSystem::eKeyState::HELD)) transform.rotation -= 5 * dt;
	if ((scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == nc::InputSystem::eKeyState::HELD)) transform.rotation += 5 * dt;
	if ((scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == nc::InputSystem::eKeyState::HELD)) thrust = speed;
	if ((scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_S) == nc::InputSystem::eKeyState::HELD)) thrust = -speed;

	//acceleration
	nc::Vector2 acceleration;
	acceleration += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * thrust;

	//gravity
	//nc::Vector2 gravity = (nc::Vector2{ 400, 300 } - transform.position).Normalized() * 20;
	//nc::Vector2 gravity = (nc::Vector2::down - transform.position).Normalized() * 20;
	//acceleration += gravity;

	velocity += acceleration * dt;
	velocity *= 0.98f;

	transform.position += velocity * dt;
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 866.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 800.0f);

	fireTimer -= dt;
	iTimer -= dt;
	if (fireTimer <= 0 && (scene->engine->Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::PRESSED)) {
		fireTimer = fireRate;

		std::shared_ptr<nc::Texture> bullet = scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("playerbullet.png", scene->engine->Get<nc::Renderer>());
		//first bullet
		{
			nc::Transform t = children[1]->transform;
			t.scale = 0.5f;

			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, bullet, 500.0f);
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
		}

		//second bullet
		/*nc::Transform t = children[2]->transform;
		t.scale = 0.5f;
		*/

		/*std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("bullet.txt"), 300);
		projectile->tag = "Player";
		scene->AddActor(std::move(projectile));
		*/

		scene->engine->Get<nc::AudioSystem>()->PlayAudio("playerfire");
	}
	
	scene->engine->Get<nc::ParticleSystem>()->Create(children[0]->transform.position, 1, 1, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("shipeffect.png", scene->engine->Get<nc::Renderer>()), 60, children[0]->transform.rotation, nc::DegToRad(30));
}

void Player::OnCollision(Actor* actor) {
	//return;

	//should kill enemies, shooters, and astroids. 
	//can be killed by astroids, projectiles, enimies, shooters, and enemies 
	
	if ((dynamic_cast<Asteroid*>(actor)) || (dynamic_cast<Projectile*>(actor) && actor->tag == "Enemy") || (dynamic_cast<ShooterEnemy*>(actor)) || (dynamic_cast<Enemy*>(actor)) && iTimer <= 0) {

		//actor->destroy = true;
		destroy = true;
		// 
		//create has an error and color is the issue but how to fix?
		//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 0.5f, nc::Color::red, 50);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");

		nc::Event event;
		event.name = "PlayerDead";
		event.data = std::string("Yes I'm dead.");

		scene->engine->Get<nc::EventSystem>()->Notify(event);
		
		iTimer = iDir;
	}
}
