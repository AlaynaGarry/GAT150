#pragma once

#define REGISTER_CLASS(class) nc::ObjectFactory::Instance().Register<class>(#class);

//Math
#include"Math/Vector2.h"
#include"Math/Color.h"
#include"Math/Random.h"
#include"Math/MathUtils.h"
#include"Math/Transform.h"

//Core
#include "Core/Utilities.h"
#include "Core/FileSystem.h"
#include "Core/Timer.h"
#include "Core/Json.h"
#include "Core/Serializable.h"

//framework
#include "Framework/EventSystem.h"
#include "Framework/Singleton.h"
#include "Framework/System.h"
#include "Framework/Factory.h"

//Input
#include "Input/InputSystem.h"

//graphics
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Graphics/Font.h"
#include "Graphics/ParticleSystem.h"

//physics 
#include "Physics/PhysicsSystem.h"

//resource
#include "Resource/ResourceSystem.h"

//objects
#include "Object/Actor.h"
#include "Object/Scene.h"

//components
#include "Component/SpriteComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/SpriteAnimationComponent.h"
#include "Component/RBPhysicsComponent.h"
#include "Component/AudioComponent.h"
#include "Component/TextComponent.h"

//audio sysytem
#include "Audio/AudioSystem.h"

#define NOMINMAX
#include <vector>
#include <memory>
#include <algorithm>

namespace nc{

	using ObjectFactory = Singleton<Factory<std::string, Object>>;

	class Engine {
	public:
		void Startup();
		void Shutdown();

		void Update();

		void Draw(Renderer* renderer);

		template<typename T>
		T* Get();

	public: 
		FrameTimer time;

	private:
		std::vector<std::unique_ptr<System>> systems;
	};

	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems) {
			if(dynamic_cast<T*>(system.get())) return dynamic_cast<T*>(system.get());

		}
		return nullptr;
	}

}