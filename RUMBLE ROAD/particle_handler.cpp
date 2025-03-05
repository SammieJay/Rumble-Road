#include "particle_handler.h"

namespace game {
	//main constructor for particle systems
	//used when there is only one instance of this particle system (i.e. for the player)
	ParticleHandler::ParticleHandler(TextureHandler* tex, string dir, GameObject* parent) {
		tex_ = tex;
		resourceDir = RESOURCES_DIRECTORY;
		parent_ = parent;
		explosionTimer = new Timer();

		rocketShader.Init((resourceDir + "/rocket_particle_vertex_shader.glsl").c_str(), (resourceDir + "/rocket_particle_fragment_shader.glsl").c_str());
		exploShader.Init((resourceDir + "/explosion_particle_vertex_shader.glsl").c_str(), (resourceDir + "/explosion_particle_fragment_shader.glsl").c_str());
	}

	//Copy constructor
	//Used to copy a 'master handler' for objects like enemies that have one master particle system but each object needs their own with a parent link
	//this construtor coppies a particle handler using the provided pointer as the parent of this instance
	ParticleHandler::ParticleHandler(ParticleHandler* original, GameObject* parent) {
		
		tex_ = original->tex_;
		resourceDir = original->resourceDir;
		parent_ = parent;
		explosionTimer = new Timer();

		rocketShader.Init((resourceDir + "/rocket_particle_vertex_shader.glsl").c_str(), (resourceDir + "/rocket_particle_fragment_shader.glsl").c_str());
		exploShader.Init((resourceDir + "/explosion_particle_vertex_shader.glsl").c_str(), (resourceDir + "/explosion_particle_fragment_shader.glsl").c_str());

		//Initialize all particle systems created in original handler into this one
		for (const auto& pair : original->particleSysMap) {
			ParticleSystem* originalSys = pair.second;
			initParticleSystem(pair.first, originalSys->GetPosition());//match texture coords of original particle system
			particleSysMap[pair.first]->SetScale(originalSys->GetScale());//match size of original particle system
		}
	}

	ParticleHandler::~ParticleHandler() {
		if (!particleSysArr.empty()) {
			for (int i = 0; i < particleSysArr.size(); i++) {
				delete particleSysArr[i];
			}
			particleSysArr.clear();
		}
	}

	void ParticleHandler::Update(double delta_time) {
		//Update function for particle handler
		//Update particle systems
		for (ParticleSystem* sys : particleSysArr) sys->Update(delta_time);

		//check if timer over, if it is hid the explosion
		if (explosionTimer->isFinished()) particleSysMap[Explosion]->show();
	}

	void ParticleHandler::explode() {//Show explosion effect for 2 seconds
		if (!contains(Explosion)) {
			cout << "ERROR: GameObject cannot explode" << endl;
			return;
		}
		particleSysMap[Explosion]->resetTime();
		particleSysMap[Explosion]->show();
		explosionTimer->Start(2);
	}

	ParticleSystem* ParticleHandler::getParticleSys(int indx) {//get particle system at given index
		if (indx >= particleSysArr.size() || indx < 0) {
			cout << "ERROR: Acessing invalid particle system index" << endl;
			return nullptr;
		}
		else return particleSysArr[indx];
	}

	ParticleSystem* ParticleHandler::getParticleSys(ParticleEffect type) {//get particle system in charge of given effect type
		if (contains(type)) return particleSysMap[type];
		else cout << "ERROR: Handler does not contain given Effect type" << endl;
	}

	//create a particle system under this handler based on the given effect type
	void ParticleHandler::initParticleSystem(ParticleEffect type, glm::vec3 textureCoords) {
		Particles* geom = nullptr;//initialize as nullpointer
		
		//Set Particle properties in relation to effect type
		//Particles inputs are as follows:
		//Particles(spread degress, phase const, particle speed const)
		if (type == Explosion) geom = new Particles(360.0f, 1.5f, 1.7f);
		else if (type == Rocket) geom = new Particles(10.0f, 1.0f, 1.2f);
		else cout << "ERROR: INVALID Particle Type" << endl;
		
		geom->CreateGeometry();

		Shader* shader = nullptr;
		if (type == Rocket) shader = &rocketShader;
		else if (type == Explosion) shader = &exploShader;
		else cout << "ERROR: INVALID Shader Type" << endl;

		//actually make the particle system
		ParticleSystem* sys = new ParticleSystem(textureCoords , geom, shader, tex_, parent_);
		sys->SetScale(glm::vec2(0.8, 0.8));
		sys->SetRotation(270*(glm::pi<float>()/180));
		
		if (type == Explosion)sys->hide();//explosion effects should start hidden
		particleSysArr.push_back(sys);
		particleSysMap[type] = sys;
	}



};//namepsace game