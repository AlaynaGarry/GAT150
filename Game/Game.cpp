#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"

void Game::Initialize(){
	//creat engine
	engine = std::make_unique<nc::Engine>(); //new Engine
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("GAT150", 896, 800);

	//register class
	REGISTER_CLASS(PlayerComponent);
	REGISTER_CLASS(EnemyComponent);
	REGISTER_CLASS(PickupComponent);

	//create scene
	scene = std::make_unique<nc::Scene>();
	scene->engine = engine.get();

	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nc::SetFilePath("../Resources");

	//events
	engine->Get<nc::EventSystem>()->Subscribe("addScore", std::bind(&Game::OnAddScore, this, std::placeholders::_1));
	engine->Get<nc::EventSystem>()->Subscribe("takeLife", std::bind(&Game::OnTakeLives, this, std::placeholders::_1));

	engine->Get<nc::AudioSystem>()->AddAudio("music", "Audio/gamemusicyay.wav");
	engine->Get<nc::AudioSystem>()->PlayAudio("music");

	lives = 3;
	score = 0;
}

void Game::Shutdown(){

	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update(){
	engine->Update();

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::PRESSED) {
		quit = true;
	}

	switch (state)
	{
	case Game::eState::Reset:
		Reset();
		break;
	case Game::eState::Title:
		Title();
		break;
	case Game::eState::StartGame:
		StartGame();
		break;
	case Game::eState::StartLevel:
		StartLevel();
		break;
	case Game::eState::Level:
		Level();
		if (!IsPlayerAlive()) {
			state = eState::PlayerDead;
			lives--;
		}

		break;
	case Game::eState::PlayerDead:
		if (lives >= 0) {
			PlayerDead();
		}
		else {
			state = eState::GameOver;
		}
		break;
	case Game::eState::GameOver:
		GameOver();
		break;
	default:
		break;
	}

	//update score
	auto scoreActor = scene->FindActor("Score");
	if (scoreActor) {
		scoreActor->GetComponent<nc::TextComponent>()->SetText(std::to_string(score));
	}

	auto livesActor = scene->FindActor("lives");
	if (livesActor) {
		livesActor->GetComponent<nc::TextComponent>()->SetText(std::to_string(lives));
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw(){
	engine->Get<nc::Renderer>()->BeginFrame();

	//scene & enine draws
	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());

	//last draw on the scene
	engine->Get<nc::Renderer>()->EndFrame();
}

bool Game::IsPlayerAlive()
{
	for (nc::Actor* actor : scene->GetActors<nc::Actor>()) {
			if (actor->name == "Player") return actor->GetComponent<PlayerComponent>()->isAlive;
		}
	return false;
}

void Game::Reset()
{
	scene->RemoveAllActors();

	rapidjson::Document document;

	bool success = nc::json::Load("title.txt", document);
	assert(success);

	scene->Read(document);
	state = eState::Title;
}

void Game::Title()
{

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::PRESSED) {
		auto title = scene->FindActor("Title");
		assert(title);
		title->active = false;

		state = eState::StartGame;
	}
}

void Game::StartGame()
{
	rapidjson::Document document;
	bool success = nc::json::Load("scene.txt", document);
	assert(success);

	scene->Read(document);

	nc::Tilemap tilemap;
	tilemap.scene = scene.get();
	success = nc::json::Load("map.txt", document);
	assert(success);
	tilemap.Read(document);
	tilemap.Create();

	state = eState::StartLevel;
	stateTimer = 0;
}

void Game::StartLevel()
{
	stateTimer += engine->time.deltaTime;
	if (stateTimer >= 1) {
		auto player = nc::ObjectFactory::Instance().Create<nc::Actor>("Player");
		player->transform.position = { 432, 650 };
		scene->AddActor(std::move(player));

		spawnTimer = 2;
		state = eState::Level;
	}

}

void Game::Level()
{
	spawnTimer -= engine->time.deltaTime;
	if (spawnTimer <= 0) {
		spawnTimer = nc::RandomRange(0.5f, 2);

		auto coin = nc::ObjectFactory::Instance().Create<nc::Actor>("Coin");
		coin->transform.position = nc::Vector2{nc::RandomRange(100, 700), nc::RandomRange(150, 600)};
		scene->AddActor(std::move(coin));

	}

	enemySpawnTimer -= engine->time.deltaTime;
	if (enemySpawnTimer <= 0) {
		enemySpawnTimer = nc::RandomRange(3, 6);

		auto enemy = nc::ObjectFactory::Instance().Create<nc::Actor>("Bat");
		enemy->transform.position = nc::Vector2{ nc::RandomRange(100, 700), nc::RandomRange(150, 600) };
		scene->AddActor(std::move(enemy));

	}
}

void Game::PlayerDead()
{
	//reset game
	Reset();
}

void Game::GameOver()
{
	//return to title after a couple seconds. 
	scene->RemoveAllActors();
	rapidjson::Document document;

	bool success = nc::json::Load("gameover.txt", document);
	assert(success);

	scene->Read(document);

	stateTimer -= engine->time.deltaTime;
	if (stateTimer <= 0) {
		lives = 3;
		score = 0;
		state = eState::Reset;
	}
}

void Game::OnAddScore(const nc::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::OnTakeLives(const nc::Event& event)
{
	lives += std::get<int>(event.data);
}
