#include "enemy_game_object.h"
#include "static_game_object.h"
#include "particle_handler.h"

namespace game {

	/*
		EnemyGameObject inherits from DynamicGameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/
	EnemyGameObject::EnemyGameObject(GameObject* player, const glm::vec3& position, Geometry* geom, Shader* shader, TextureHandler* texHandler, enemyType type) : 
	DynamicGameObject(position, geom, shader, Enemy, 3)
	{
		behavior_ = Wait;
		velocity = glm::vec3(0.0f,0.0f,0.0f);
		this->player = player;
		this->texHandler = texHandler;
		ResourceDir = RESOURCES_DIRECTORY;
		enemyType_ = type;
		detonated = false;

		//init based on enemy type
		if (enemyType_ == Tower) initTower();
		else if (enemyType_ == Bomb) initBoomCar();
		else if (enemyType_ == Melee) initMelee();
		else texture_ = this->texHandler->getMainTex();

	}

	EnemyGameObject::~EnemyGameObject() {
		
		if (enemyType_ == Tower) {
			//delete turret;
			//delete turretTex;
			//delete rocketTex;
			//delete particleTex;
			//delete rocketParticleTemplate;
			//delete rocketCooldownTimer;
		}
	}

	//=====INITIALIZATION FUNCTIONS=====
	void EnemyGameObject::initTower() {
		//Initialize tower Base and various starting state variables
		texture_ = texHandler->getTex("tower");
		scale_ = glm::vec2(2, 2);
		angle_ = 0;
		behavior_ = Wait;
		health = TOWER_HEALTH;
		explosiveDmgOnly = true;

		//Initialize Tower Particle Systems
		//Rocket Particle Handler Template Init
		TextureHandler* particleTex = new TextureHandler((ResourceDir + "/textures/orb.png").c_str());
		rocketParticleTemplate = new ParticleHandler(particleTex, ResourceDir, nullptr);
		rocketParticleTemplate->initParticleSystem(ParticleHandler::Rocket, glm::vec3(-0.5, 0, 1));
		rocketParticleTemplate->getParticleSys(ParticleHandler::Rocket)->SetScale(glm::vec2(0.2, 0.2));
		rocketParticleTemplate->initParticleSystem(ParticleHandler::Explosion);
		rocketParticleTemplate->getParticleSys(ParticleHandler::Explosion)->SetScale(glm::vec2(0.6, 0.6));

		//initialize tower turret
		turretTex = new TextureHandler(ResourceDir+"/textures/rocket_launcher.png");
		turret = new StaticGameObject(position_, geometry_, shader_,turretTex, SubObject, 0, glm::vec2(1.7,1.7));
		subObjArr.push_back(turret);

		//Initialize Rocket System
		rocketTex = new TextureHandler(ResourceDir + "/textures/rocket.png");
		rocketCooldownTimer = new Timer("rocket tower cooldown");
	}

	void EnemyGameObject::initBoomCar() {
		texture_ = texHandler->getTex("boom_car");
		scale_ = glm::vec2(1.8,1);
		health = BOMB_CAR_HEALTH;
		behavior_ = Patrolling;
		explosiveDmgOnly = false;
		

		//Setup for patrol behavior
		patrolRad = 6.0f;
		patrolOrigin = position_ + patrolRad * GetRight();
		behavior_ = Patrolling;
		SetRotation(glm::radians(90.0f));
		t = 0;
		speed = 1200;
	}

	void EnemyGameObject::initMelee() {
		texture_ = texHandler->getTex("guy");
		scale_ = glm::vec2(1.0, 0.6);
		health = 0.75;
		explosiveDmgOnly = false;
	}

	//===== TOWER ENEMY FUNCTIONS =====
	void EnemyGameObject::shootRocket() {
		//If shooting cooldown timer is not currently running, run this code
		if ((rocketCooldownTimer->isFinished() || !rocketCooldownTimer->isRunning()) && isShowing()) {
			ProjectileGameObject* rocket = new ProjectileGameObject(geometry_, shader_, rocketTex, 0.6, 10, glm::vec2(1.0,0.5), turret, ProjectileGameObject::Rocket);

			//Copy particle handler and add to rocket
			ParticleHandler* rocketParticleHandler = new ParticleHandler(rocketParticleTemplate, rocket);
			rocket->setParticleHandler(rocketParticleHandler);
			projectileArr.push_back(rocket);//create and add new projectile to projectile array
			rocketCooldownTimer->Start(TOWER_ROCKET_COLLDOWN);
		}
	}


	//=====UPDATE FUNCTIONS=====
	void EnemyGameObject::updateTower(double delta_time) {
		
		//update state depending on player distance
		if (behavior_ == Wait && distToPlayer() < 15.0f) {
			behavior_ = Chase;
			rocketCooldownTimer->Start(6);
			//cout << "Tracking Player" << endl;
		} 
		if (behavior_ == Chase && distToPlayer() > 20.0f) {
			behavior_ = Wait;
			//cout << "Stopped Tracking Player" << endl;
		} 

		//Track player when in chase
		if (behavior_ == Chase) { 
			//Turn Turret Towards Player
			glm::vec3 playerPos = player->GetPosition();
			glm::vec3 vecToPlayer = glm::normalize(playerPos - turret->GetPosition());

			float angleToPlayer = glm::acos(glm::dot(turret->GetBearing(), vecToPlayer));

			float angleToPlayerRight = glm::acos((glm::dot(turret->GetRight(), vecToPlayer)));
			if (angleToPlayer > 0.08 || angleToPlayer > angleToPlayerRight) { turret->SetRotation(turret->GetRotation() + 0.02f); }
			else if (angleToPlayer > 0) { turret->SetRotation(turret->GetRotation() - 0.02f); }
			

			shootRocket();//shoot rocket if possible
		}
	
	}

	void EnemyGameObject::updateBoomCar(double delta_time) {
		//Update state to chase player if they get too close
		if (behavior_ == Patrolling && distToPlayer() <= 4.0f && Alive) {//if the player gets close, start intercept behavior
			behavior_ = Chase;
		}


		//Move in orbital fashion
		if (behavior_ == Patrolling && Alive) {
			float newX = patrolOrigin.x + patrolRad * cos(t * speed / 1200.0);
			float newY = patrolOrigin.y + patrolRad * sin(t * speed / 1200.0);
			t += delta_time;
			SetPosition(glm::vec3(newX, newY, 0.0f));
			
			angle_ += delta_time * speed / 1200.0;
		}

		//If in chase, turn towards the player and pursue them
		if (behavior_ == Chase && Alive) {
			glm::vec3 playerPos = player->GetPosition();
			glm::vec3 vecToPlayer = glm::normalize(playerPos - position_);

			float angleToPlayer = glm::acos(glm::dot(GetBearing(), vecToPlayer));

			float angleToPlayerRight = glm::acos((glm::dot(GetRight(), vecToPlayer)));
			
			if (angleToPlayer > 0.08 || angleToPlayer > angleToPlayerRight) { SetRotation(GetRotation() + 0.015f); }
			else if (angleToPlayer > 0) { SetRotation(GetRotation() - 0.015f); }

			position_ += GetBearing() * float(delta_time)*(speed/100);

			if (distToPlayer() < 2.75f) {//If player is close enough, go kaboom
				particleHandler->getParticleSys(ParticleHandler::Explosion)->SetScale(glm::vec2(0.6, 0.6));
				die();
				detonated = true;
			}
		}

	}

	void EnemyGameObject::updateMelee(double delta_time) {}

	// Update function for moving the player object around
	void EnemyGameObject::Update(double delta_time) {
		if (enemyType_ == Tower) updateTower(delta_time);
		if (enemyType_ == Bomb) updateBoomCar(delta_time);
		if (enemyType_ == Melee) updateMelee(delta_time);


		// Call the parent's update method to move the object in standard way, if desired
		DynamicGameObject::Update(delta_time);
	}

} // namespace game