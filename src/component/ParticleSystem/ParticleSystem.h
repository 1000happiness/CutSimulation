#ifndef CMP_PARTICLESYSTEM_H
#define CMP_PARTICLESYSTEM_H

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <random>

#include <glm/ext.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../../component/Component/Component.h"

namespace TIME
{
	extern float delta_time;
} // namespace TIME

namespace CMP
{
	class ParticleSystem : public Component
	{
	public:
		class Particle {
		public:
			glm::vec3 position;
			glm::vec3 color;
			glm::vec3 speed;
			glm::vec3 acceleration;
			bool active;
			float life;
			Particle() {
				this->position = glm::vec3(0.0f, 0.0f, 0.0f);
				this->speed = glm::vec3(0.0f, 0.0f, 0.0f);
				this->acceleration = glm::vec3(0.0f, -9.8f, 0.0f);
				this->color = glm::vec3(1.0f, 1.0f, 1.0f);
				this->active = true;
				this->life = 0.0f;
			}
			Particle(glm::vec3 position, glm::vec3 speed, glm::vec3 acceleration, glm::vec3 color) {
				this->position = position;
				this->speed = speed;
				this->acceleration = acceleration;
				this->color = color;
				this->active = true;
				this->life = 0;
			}
		};

		class ParticleSet {
		public:
			unsigned int VAO;
			unsigned int VBO;
			Particle* particles;
			float exist_time;
			int num;
		};

		int particle_num;
		float life;
		float speed;
		float speed_random_delta;
		glm::vec3 direction;
		float angle;
		glm::vec3 color;
		glm::vec3 acceleration;

		ParticleSystem(OBJ::Object* game_object = NULL, int particle_num = 50, float life = 0.1f, float speed = 20.0f, float speed_random_delta = 19.5f, glm::vec3 dirction = glm::vec3(0.0f, 1.0f, 0.0f), float angle = 90.0f, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 acceleration = glm::vec3(0.0f, -9.8f, 0.0f));
		~ParticleSystem();

		void start();
		void update();
		void render(MAT::ShaderProgram* shader);

		void start_create_particles();
		void stop_create_particles();
	private:
		std::vector<ParticleSet*> particle_sets;
		bool create_flag;

		ParticleSet* create_single_particle_set(int num);
		void update_single_particle_set(ParticleSet* particle_set);
		void render_single_particle_set(ParticleSet* particle_set, MAT::ShaderProgram* shader);
		void erase_single_particle_set(ParticleSet* particle_set);
	};
}

#endif
