#ifndef PARTICLE_HANDLER_H_
#define PARTICLE_HANDLER_H_

#include "defs.h"
#include "texture_handler.h"
#include "particle_system.h"
#include "particles.h"

namespace game {
	//class GameObject;//Forward declaration for pointers
	//class ParticleSystem;

	//Class for the management and activation of particle systems for a parent object
	//made to avoid circular includes and easy management of particle systems
	class ParticleHandler {
	public:
		enum ParticleEffect { Explosion, Rocket };

		ParticleHandler(TextureHandler* tex, string resourceDir, GameObject* parent);
		ParticleHandler(ParticleHandler*, GameObject*);
		~ParticleHandler();

		void Update(double delta_time);

		ParticleSystem* getParticleSys(int indx);
		ParticleSystem* getParticleSys(ParticleEffect);//Get particle system correwsponding to given effect

		//Functions for creating particle systems
		void initParticleSystem(ParticleEffect type, glm::vec3(textureCoords));
		inline void initParticleSystem(ParticleEffect type) { initParticleSystem(type, glm::vec3(0, 0, 1)); }//if no texture coord given default to 0,0
		
		//Minor info getter functions
		bool hasParticleSystems(void) { return !particleSysArr.empty(); }
		inline int numParticleSystems(void) { return particleSysArr.size(); }
		bool contains(ParticleEffect effect) { return particleSysMap.count(effect) > 0; }

		//Particle Effect Triggers
		void explode(void);

	private:

		std::vector<ParticleSystem*> particleSysArr; //stored for easy retreival of all particle systems in rendering function
		std::unordered_map<ParticleEffect, ParticleSystem*> particleSysMap; //Stored for the retreival of specific particle systems
		
		//The directory of the game folder
		string resourceDir;
		GameObject* parent_;
		TextureHandler* tex_;

		Shader rocketShader;
		Shader exploShader;

		Timer* explosionTimer;
		

	};
};//namespace game
#endif