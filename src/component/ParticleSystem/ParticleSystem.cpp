#include "ParticleSystem.h"
#include "../Transform/Transform.h"
#include "../../object/Object/Object.h"
#include <glad/glad.h>

namespace CMP
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<float> u(-1.0f, 1.0f);

	ParticleSystem::ParticleSystem(OBJ::Object* game_object, int particle_num, float life, float speed, float speed_random_delta, glm::vec3 direction, float angle, glm::vec3 color, glm::vec3 acceleration) : Component(Kind::PARTICLESYSTEM, game_object)
	{
		this->game_object = game_object;
		this->particle_num = particle_num;
		this->life = life;
		this->speed = speed;
		this->speed_random_delta = speed_random_delta;
		this->direction = direction;
		this->angle = angle;
		this->color = color;
		this->acceleration = acceleration;
		this->create_flag = false;
	}

	ParticleSystem::~ParticleSystem()
	{
	}

	void ParticleSystem::start()
	{

	}

	void ParticleSystem::update()
	{
		float delta_time = TIME::delta_time;

		//erase
		for (auto item = particle_sets.begin(); item != particle_sets.end(); ) {
			(*item)->exist_time += delta_time;
			if ((*item)->exist_time > life) {
				erase_single_particle_set(*item);
				item = particle_sets.erase(item);
			}
			else {
				item++;
			}
		}

		//update
		for (auto item = particle_sets.begin(); item != particle_sets.end(); item++) {
			update_single_particle_set(*item);
		}

		if (!create_flag) {
			return;
		}

		//create
		int num = delta_time < life ? int(delta_time / life * particle_num) : particle_num;
		ParticleSet* new_particle_set = create_single_particle_set(num);
		if (new_particle_set == NULL) {
			return;
		}
		particle_sets.push_back(new_particle_set);
	}

	void ParticleSystem::render(MAT::ShaderProgram *shader)
	{
		Transform* transform = (Transform*)game_object->getComponent(CMP::Kind::TRANSFORM);
		if (transform == NULL)
		{
			std::cout << "Particle system component depends on transform" << std::endl;
			return;
		}

		for (auto item = particle_sets.begin(); item != particle_sets.end(); item++) {
			render_single_particle_set(*item, shader);
		}
	}

	void ParticleSystem::start_create_particles()
	{
		this->create_flag = true;
	}

	void ParticleSystem::stop_create_particles()
	{
		this->create_flag = false;
	}
	
	ParticleSystem::ParticleSet* ParticleSystem::create_single_particle_set(int num)
	{
		ParticleSet* particle_set = new ParticleSet();

		//init partices data
		particle_set->particles = new Particle[num];
		particle_set->num = num;
		
		
		Transform* transform = (Transform*)game_object->getComponent(CMP::Kind::TRANSFORM);
		if (transform == NULL)
		{
			std::cout << "Particle system component depends on transform" << std::endl;
			return NULL;
		}
		glm::mat4 model_mat = transform->model_mat;
		glm::vec3 init_position = glm::vec3(model_mat * glm::vec4(0.0f, 0.0f, 0.0f, 1));
		for (int i = 0; i < num; i++) {
			//speed
			float random_speed = speed + u(e) * speed_random_delta;
			glm::mat4 rotation_mat = glm::mat4(1.0f);
			glm::vec3 normal = glm::normalize(glm::cross(direction, glm::vec3(u(e), u(e), u(e))));
			rotation_mat = glm::rotate(rotation_mat, glm::radians(u(e) * angle), normal);
			glm::vec4 random_direction_vec4 = rotation_mat * glm::vec4(direction, 1.0f);
			glm::vec3 random_direction = glm::normalize(glm::vec3(random_direction_vec4.x, random_direction_vec4.y, random_direction_vec4.z));
			particle_set->particles[i].speed = random_speed * random_direction;
			/*std::cout << random_direction.x << " " << random_direction.y << " " << random_direction.z << std::endl;*/

			//color
			particle_set->particles[i].color = color;

			//acceleration
			particle_set->particles[i].acceleration = acceleration;

			//postion
			particle_set->particles[i].position = init_position;
		}

		//init VAO VBO
		glGenVertexArrays(1, &particle_set->VAO);
		glGenBuffers(1, &particle_set->VBO);

		return particle_set;
	}

	void ParticleSystem::update_single_particle_set(ParticleSet* particle_set)
	{
		float delta_time = TIME::delta_time;
		for (int i = 0; i < particle_set->num; i++) {
			Particle* particle = &particle_set->particles[i];
			particle->position += particle->speed * delta_time;
			particle->speed += particle->acceleration * delta_time;
		}
	}

	void ParticleSystem::render_single_particle_set(ParticleSet* particle_set, MAT::ShaderProgram* shader)
	{
		shader->use();
		glBindVertexArray(particle_set->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, particle_set->VBO);
		glBufferData(GL_ARRAY_BUFFER, particle_set->num * sizeof(Particle), particle_set->particles, GL_STATIC_DRAW);
		int pos_layout = 0;
		glVertexAttribPointer(pos_layout, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
		glEnableVertexAttribArray(pos_layout);
		int color_layout = 1;
		glVertexAttribPointer(color_layout, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(color_layout);
		int speed_layout = 2;
		glVertexAttribPointer(speed_layout, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(speed_layout);
		//glPointSize(50.0f);
		/*for (int i = 0; i < particle_set->num; i++) {
			Particle* particle = &particle_set->particles[i];
			
		}*/
		//std::cout << particle_set->num << std::endl;
		glDrawArrays(GL_POINTS, 0, particle_set->num);
		

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void ParticleSystem::erase_single_particle_set(ParticleSet* particle_set)
	{
		glDeleteVertexArrays(1, &particle_set->VAO);
		glDeleteBuffers(1, &particle_set->VBO);
		delete particle_set->particles;
		delete particle_set;
	}
}