#include "Game.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"
#include "Actors/ShooterEnemy.h"
#include "Actors/Asteroid.h"
#include "Actors/Projectile.h"
#include "Files/SaveFile.h"
#include <SDL_image.h>

void Game::Initialize(){
	//creat engine
	engine = std::make_unique<nc::Engine>(); //new Engine
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("GAT150", 866, 800);

	//create scene
	scene = std::make_unique<nc::Scene>();
	scene->engine = engine.get();

	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nc::SetFilePath("../Resources");
	
	//std::shared_ptr<nc::Texture> texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("space.png", engine->Get<nc::Renderer>());

	//Sf2.png 
	/*for (size_t i = 0; i < 10; i++) {
		nc::Transform transform{ nc::Vector2{nc::RandomRange(0, 800), nc::RandomRange(0, 600)}, nc::RandomRange(0, 360), 1.0f };
		std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, texture);
		scene->AddActor(std::move(actor));
	}*/

	//highscore for save file
	highscore = nc::SaveFile::LoadHighScore("score.txt");

	//Game
	engine->Get<nc::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("music", "audio/gamemusic.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("explosion", "audio/die.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("enemyfire", "audio/enemyfire.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("playerfire", "audio/playerfire.wav");

	playerTexture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("player.png", engine->Get<nc::Renderer>());
	particleTexture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("boom.png", engine->Get<nc::Renderer>());
	backgroudTexture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("space.png", engine->Get<nc::Renderer>());

	engine->Get<nc::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<nc::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1));

	musicChannel = engine->Get<nc::AudioSystem>()->PlayAudio("music", 1.0f, 1.0f, true);
	//not sure what this is tbh
	//stateFunction = &Game::UpdateTitle;
}

void Game::Shutdown(){

	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update(){
	engine->Update();

	float dt = engine->time.deltaTime;
	stateTimer += dt;
	gameOverTimer += dt;

	switch (state)
	{
	case Game::eState::Title:
	{
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::PRESSED) {
			state = eState::StartGame;
		}
		break;
	}
	case Game::eState::StartGame:
		score = 0;
		lives = 1;
		state = eState::StartLevel;
		
		break;
	case Game::eState::StartLevel:

		//scene->engine->Get<nc::AudioSystem>()->PlayAudio("music");
		UpdateStartLevel(dt);
		state = eState::Game;
		break;
	case Game::eState::Game: 
	{
		if ((scene->GetActors<Enemy>().size() == 0 && scene->GetActors<ShooterEnemy>().size() == 0) || scene->GetActors<Player>().size() == 0) {
			state = eState::GameOver;
		}
	}
		break;
	case Game::eState::GameOver:

		gameOverTimer -= 5 * dt;
		if (gameOverTimer <= 0) {
			scene->RemoveAllActors();
			state = eState::Title;
		}else{
			gameOverTimer -= dt;
		}
		
		break;
	default:
		break;
	}

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::PRESSED) {
		quit = true;
	}

	scene->Update(engine->time.deltaTime);
	//not sure what this is tbh
	//engine->time.timeScale = 0.2f;

}

void Game::Draw(){
	engine->Get<nc::Renderer>()->BeginFrame();

	//text
	int size = 16;
	std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/space/space age.ttf", &size);
	nc::Transform t;
	// position of your pixel
	t.position = { 400, 300 };
	t.rotation = 0;

	nc::Transform transform{ nc::Vector2{433, 400}, 0, 1.0f };
	engine->Get<nc::Renderer>()->Draw(backgroudTexture, transform);
	//std::unique_ptr<nc::Texture> background = std::make_unique<nc::Texture>(transform, backgroudTexture);

	switch (state)
	{
	case Game::eState::Title:
	{

		// create font texture
		textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		// set font texture with font surface
		textTexture->Create(font->CreateSurface("Space Game", nc::Color{ 0, 0, 1 }));
		// add font texture to resource system
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
		engine->Get<nc::Renderer>()->Draw(textTexture, t);

		textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		textTexture->Create(font->CreateSurface("press SPACE to start!", nc::Color{ 0, 0, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);

		t.position = { 400, 330 };
		t.rotation = 0;
		engine->Get<nc::Renderer>()->Draw(textTexture, t);

		break;
	}
	case Game::eState::StartGame:
		
		break;
	case Game::eState::StartLevel:

		break;
	case Game::eState::Game:

		break;
	case Game::eState::GameOver:
		
		if (scene->GetActors<Enemy>().size() == 0 && scene->GetActors<ShooterEnemy>().size() == 0) {
			//You Win!
			textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
			textTexture->Create(font->CreateSurface("You Win!", nc::Color{ 0, 0, 1 }));
			engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
			
			t.position = { 400, 330 };
			t.rotation = 0;
			engine->Get<nc::Renderer>()->Draw(textTexture, t);
		}
		else if (scene->GetActors<Player>().size() <= 0) {
			//Game Over
			textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
			textTexture->Create(font->CreateSurface("Game Over", nc::Color{ 1, 0, 0 }));
			engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
			
			t.position = { 400, 330 };
			t.rotation = 0;
			engine->Get<nc::Renderer>()->Draw(textTexture, t);
		}
		else if ((scene->GetActors<Enemy>().size() == 0 && scene->GetActors<ShooterEnemy>().size() == 0 ) && scene->GetActors<Player>().size() <= 0) {
			//Draw
			textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
			textTexture->Create(font->CreateSurface("Draw", nc::Color{ 1, 0, 0 }));
			engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);

			t.position = { 400, 330 };
			t.rotation = 0;
			engine->Get<nc::Renderer>()->Draw(textTexture, t);
		}
		
		break;
	default:
		break;
	}

	//score
	size = 6;
	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface(std::to_string(score).c_str(), nc::Color{ 0, 0, 1 }));
	engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
	t.position = { 30, 40 };
	engine->Get<nc::Renderer>()->Draw(textTexture, t);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface(std::to_string(highscore).c_str(), nc::Color{ 0, 0, 1 }));
	engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
	t.position = { 40, 20 };
	engine->Get<nc::Renderer>()->Draw(textTexture, t);

	//scene & enine draws
	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());
	//last draw on the scene
	engine->Get<nc::Renderer>()->EndFrame();
}

void Game::UpdateTitle(float dt){
	/*
	if (Core::Input::IsPressed(VK_SPACE)) {
		stateFunction = &Game::UpdateTitle;
	}
	*/
}

void Game::UpdateStartLevel(float dt)
{
		//spawning the player icons
		scene->AddActor(std::make_unique<Player>(nc::Transform(nc::Vector2(400.0f, 300.0f), 0.0f, 0.5f), engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("player.png", engine->Get<nc::Renderer>()), 1500.0f));
		
		//spawns enemies
		for (size_t i = 0; i < 4; i++) {
		//	enemies
			scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange (840.0f, 866.0f), nc::RandomRange(780.0f, 800.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 0.3f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("scout.png", engine->Get<nc::Renderer>()), 200.0f));
		}
		
		for (size_t i = 0; i < 2; i++) {
			//shooter enemies
			scene->AddActor(std::make_unique<ShooterEnemy>(nc::Transform{ nc::Vector2{nc::RandomRange(1.0f, 25.0f), nc::RandomRange(780.0f, 800.0f)}, nc::RandomRange(1.0f, nc::TwoPi), 0.3f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("shooter.png", engine->Get<nc::Renderer>()), 100.0f));
		}
		
		for (size_t i = 0; i < 3; i++) {
			//	astroids
			scene->AddActor(std::make_unique<Asteroid>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(780.0f, 800.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 0.5f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("astroid.png", engine->Get<nc::Renderer>()), 75.0f));
		}
}

void Game::OnAddPoints(const nc::Event& event)
{
	if (state != eState::GameOver) {
		score += std::get<int>(event.data);
		if (score > highscore) {
			nc::SaveFile::WriteHighScore("score.txt", score);
			highscore = score;
		}
	}
}

void Game::OnPlayerDead(const nc::Event& event)
{
	lives--;
	std::cout << std::get<std::string>(event.data) << std::endl;
	state = eState::GameOver;
}
