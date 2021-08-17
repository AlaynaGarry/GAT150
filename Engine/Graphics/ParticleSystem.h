#pragma once
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Framework/System.h"
#include <vector>

namespace nc {

	class Renderer;
	class Texture;

	class ParticleSystem : public GraphicsSystem {
	private:
		struct Particle {
			Vector2 position;
			Vector2 prevPosition;
			Vector2 velocity;
			std::shared_ptr<Texture> texture;

			float lifetime;
			bool isActive{ false };

			static bool isNotActive(Particle particle) { return !particle.isActive; }
		};

	public: 
		void Startup() override;
		void Shutdown() override;

		void Update(float dt) override;
		void Draw(Renderer* graphics) override;

		void Create(Vector2 position, size_t count, float lifetime, std::shared_ptr<Texture> texture, float speed);
		void Create(Vector2 position, size_t count, float lifetime, std::shared_ptr<Texture> texture, float speed, float angle, float angleRange);
	
	public:
		//std::shared_ptr<Texture> texture;
	
	private:
		std::vector<Particle> particles;

	};

}
