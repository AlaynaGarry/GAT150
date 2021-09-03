#include "PlayerComponent.h"
#include "Engine.h"

using namespace nc;

PlayerComponent::~PlayerComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collisionEnter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collisionExit", owner);
}

void PlayerComponent::Create()
{

	owner->scene->engine->Get<EventSystem>()->Subscribe("collisionEnter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collisionExit", std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("hurt", "Audio/hurt.wav");
}

void PlayerComponent::Update()
{
	Vector2 force = Vector2::zero;
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_A) == InputSystem::eKeyState::HELD) {
		force.x -= speed;
	}
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_D) == InputSystem::eKeyState::HELD) {
		force.x += speed;
	}
	if (contacts.size() > 0 && owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_W) == InputSystem::eKeyState::PRESSED) {
		force.y -= jump;
	}

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);

	physicsComponent->ApplyForce(force);

	SpriteAnimationComponent* spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
	assert(spriteAnimationComponent);

	if (physicsComponent->velocity.x > 0) spriteAnimationComponent->StartSequence("walk_right");
	else if (physicsComponent->velocity.x < 0) spriteAnimationComponent->StartSequence("walk_left");
	else spriteAnimationComponent->StartSequence("idle");
}
void PlayerComponent::OnCollisionEnter(const Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground") || istring_compare(actor->tag, "platform")) {
		contacts.push_back(actor);
	}

	if (istring_compare(actor->tag, "enemy")) {
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("hurt");
		health -= 1;
		if (health <= 0)
		{
			//destroy if the player is hit by the bat.
			isAlive = false;

			Event event;
			event.name = "takeLife";
			event.data = 1;
		}

		actor->destroy = true;
	}

}
void PlayerComponent::OnCollisionExit(const Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground") || istring_compare(actor->tag, "platform")) {
		contacts.remove(actor);
	}
}
bool PlayerComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PlayerComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	JSON_READ(value, jump);
	return false;
}

