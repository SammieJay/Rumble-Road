#include "object_manager.h"

namespace game {
	
	//Constructor
	ObjectManager::ObjectManager(Geometry* sprite, Shader* spriteShader, Shader* particleShader, std::string resourceDir, GLFWwindow* window_) :
	sprite_(sprite), spriteShader_(spriteShader), resourceDir_(resourceDir),window_(window_){
		//Constructor just calls generation of random generator
		//other initialization is triggered by game object when ready
		//see game->setup() for initialization calls
		randomGen = RandomGenerator();
	}

	ObjectManager::~ObjectManager() {

		//Deallocate Game Objects and Arrays
		for (EnemyGameObject* enemy : enemyArr) {
			delete enemy;
		}
		enemyArr.clear();

		for (CollectibleGameObject* item : itemArr) {
			delete item;
		}
		itemArr.clear();

		for (StaticGameObject* staticObj : staticObjArr) {
			delete staticObj;
		}
		staticObjArr.clear();

		delete player;
		delete background;


		//Deallocate Texture Handlers
		delete playerTexHandler;
		delete enemyTexHandler;
		delete backgroundTexHandler;
	}

	//Init Texture function
	//	Used to initialize all the texture handlers for each kind of game object
	void ObjectManager::initTextures() {

		/*
		Player Game Object Textures:
		death: texture for dead player object
		*/
		playerTexHandler = new TextureHandler("/textures/car.png", resourceDir_);
		playerTexHandler->loadTexture("/textures/Explosion.png", "death");

		/* Enemy Game Object Textures:
		tower: base of tower enemy
		boom_car: texture for explosive car
		*/
		enemyTexHandler = new TextureHandler("/textures/black_car.png", resourceDir_);
		enemyTexHandler->loadTexture("/textures/tower.png", "tower");
		enemyTexHandler->loadTexture("/textures/red_car.png", "boom_car");
		enemyTexHandler->loadTexture("/textures/lil_guy.png", "guy");


		//Background Texture (only holds a single texture)
		backgroundTexHandler = new TextureHandler("/textures/ground.png", resourceDir_);
	}


	//	Initialize any Game Objects present at the start of the game
	void ObjectManager::initObjects(void) {
		//Init Player Object
		player = new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, spriteShader_, window_, playerTexHandler);

		background = new StaticGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, spriteShader_, backgroundTexHandler, GameObject::Background, 0.0f, glm::vec2(100.0, 100.0));
	}

	//Initialize particle systems
	void ObjectManager::initParticles(void) {
		TextureHandler* particleTex = new TextureHandler(resourceDir_ + "/textures/orb.png");
		playerParticleHandler = new ParticleHandler(particleTex, resourceDir_, player);
		playerParticleHandler->initParticleSystem(ParticleHandler::Explosion);
		player->setParticleHandler(playerParticleHandler);
		
		enemyParticleHandler = new ParticleHandler(particleTex, resourceDir_, background);
		enemyParticleHandler->initParticleSystem(ParticleHandler::Explosion);
		enemyParticleHandler->getParticleSys(ParticleHandler::Explosion)->SetScale(glm::vec2(0.3, 0.3));//set explosion effect size
	}

	//Spawning Functions:
	//	Used to create and spawn a new enemy obj at a specified location
	void ObjectManager::spawnEnemy(const glm::vec3& position, EnemyGameObject::enemyType type) {
		EnemyGameObject* enemy = new EnemyGameObject(player, position, sprite_, spriteShader_, enemyTexHandler, type);
		enemy->setParticleHandler(new ParticleHandler(enemyParticleHandler, enemy));
		enemyArr.push_back(enemy);
	}

	//	Used to create and spawn a new ITEM obj at a specified location
	void ObjectManager::spawnItem(const glm::vec3& position, CollectibleGameObject::itemType itemID) {
		CollectibleGameObject* Item = new CollectibleGameObject(position, sprite_, spriteShader_, itemID);
		itemArr.push_back(Item);
	}

	//	Used to create and spawn a new STATIC obj at a specified location
	void ObjectManager::spawnStaticObj(const glm::vec3& position, float angle, glm::vec2 scale, GameObject::objType type, string textureDir) {
		//CREATE TEXTURE HANDLER HERE
		string tex = resourceDir_ + textureDir;
		TextureHandler* texHandler = new TextureHandler(tex);
		StaticGameObject* obj = new StaticGameObject(position, sprite_, spriteShader_, texHandler, type, angle, scale);
		delete texHandler;
		staticObjArr.push_back(obj);
	}

	//Retreival functions
	//Used to retreive a specific game object at a specific index
	//Not often used
	EnemyGameObject* ObjectManager::GetEnemy(int i) const {
		if (i >= enemyArr.size()) {
			cout << "ERROR: Requested index larger than Enemy array" << endl;
			return nullptr;
		} else return enemyArr[i];
	}

	CollectibleGameObject* ObjectManager::GetItem(int i) const {
		if (i >= itemArr.size()) {
			cout << "ERROR: Requested index larger than Item array" << endl;
			return nullptr;
		}
		else return itemArr[i];
	}

	StaticGameObject* ObjectManager::GetStaticObject(int i) const {
		if (i >= staticObjArr.size()) {
			cout << "ERROR: Requested index larger than Static Obj array" << endl;
			return nullptr;
		}
		else return staticObjArr[i];
	}

	//Dammage Distribution Functions
	//dammage everything 
	void ObjectManager::damageAOE(glm::vec3 origin, float radius, float damageAtOrigin, GameObject::objType targets) {
		if (targets == GameObject::Enemy) {
			for (EnemyGameObject* enemy : enemyArr) {
				//Calculate enemy distance to origin
				float distToOrigin = glm::distance(enemy->GetPosition(), origin);
				float dmg = damageAtOrigin;
				if (distToOrigin <= 0.5f + radius) {
					enemy->takeDmg(dmg);
				}
			}
		}

		if (targets == GameObject::Player) {
			float distToOrigin = glm::distance(player->GetPosition(), origin);
			float dmg = damageAtOrigin;
			if (distToOrigin <= 0.5f + radius) {
				player->takeDmg(dmg);
			}
		}
		
	}



	/*GAME OBJECT UPDATE FUNCTION
		* This function is in charge of performing all of the following:
		* Calling the update function for all objects
		* Detecting and signaling collisions to the relevent objects
		* Clearing Objects we want gone from the game world
	*/
	void ObjectManager::Update(double delta_time) {
		// Update PLAYER game object
		player->Update(delta_time);

		// check if any ENEMY game objects need to be cleared
		// Call Update for all ENEMY game objects
		auto EnemyIt = enemyArr.begin();
		while (EnemyIt != enemyArr.end()) {
			//Check if this enemy is colliding with any player projectiles
			if ((*EnemyIt)->isAlive() && player->checkProjectileColisions((*EnemyIt))) { 
				if(!(*EnemyIt)->onlyExploDmg())(*EnemyIt)->takeDmg(BULLET_DMG); 
			}
			
			//Check if player is coliding with any of this enemies projectiles
			if ((*EnemyIt)->hasProjectiles()) (*EnemyIt)->checkProjectileColisions(player);

			//Handle Collisions with enemies
			if ((*EnemyIt)->distToPlayer() < 1.5f && (*EnemyIt)->isAlive() && player->isAlive()) {
				if((*EnemyIt)->isEnemyType(EnemyGameObject::Tower))player->die();
				else {
					(*EnemyIt)->die();
					player->takeDmg(COLLISION_DMG);
				}
			}

			//deletion and clearing
			if ((*EnemyIt)->isToBeCleared()) {
				delete *EnemyIt;
				EnemyIt = enemyArr.erase(EnemyIt);
			}
			else {
				(*EnemyIt)->Update(delta_time);
				++EnemyIt;
			}
		}

		// check if any COLLECTIBLE game objects need to be cleared
		// Call Update for all COLLECTIBLE game objects
		auto ItemIt = itemArr.begin();
		while (ItemIt != itemArr.end()) {
			//Check for player colision with this item, if coliding tell player to collect it
			float distToPlayer = glm::distance((*ItemIt)->GetPosition(), player->GetPosition());
			float colisionDist = (glm::length(player->GetScale())/2) + (glm::length((*ItemIt)->GetScale())/3);
			
			if (distToPlayer < colisionDist) {
				(*ItemIt)->collect(player);
				(*ItemIt)->clearObject();
			}
			if ((*ItemIt)->isToBeCleared()) {
				delete *ItemIt;
				ItemIt = itemArr.erase(ItemIt);
			}
			else {
				(*ItemIt)->Update(delta_time);
				++ItemIt;
			}
		}

		// check if any STATIC game objects need to be cleared
		// Call Update for all STATIC game objects
		auto StaticIt = staticObjArr.begin();
		while (StaticIt != staticObjArr.end()) {
			if ((*StaticIt)->isToBeCleared()) {
				delete* StaticIt;
				StaticIt = staticObjArr.erase(StaticIt);
			}
			else {
				(*StaticIt)->Update(delta_time);
				++StaticIt;
			}
		}

		// Update BACKGROUND game object
		background->Update(delta_time);
	}
}