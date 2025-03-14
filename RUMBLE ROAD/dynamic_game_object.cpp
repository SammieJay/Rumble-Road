#include "dynamic_game_object.h"
#include "particle_handler.h"

namespace game {
	string resourceDir = RESOURCES_DIRECTORY;//global declare for resource directory

	DynamicGameObject::DynamicGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, objType type, float hp) :
		GameObject(position, geom, shader, type)
	{
		deathTimer = new Timer("Death");
		health = float(hp);
		Alive = true;
	}

	DynamicGameObject::~DynamicGameObject() {
		if (!projectileArr.empty()) {
			for (ProjectileGameObject* projectile : projectileArr) {
				delete projectile;
			}
			projectileArr.clear();
		}
	}

	void DynamicGameObject::takeDmg(float dmg) {
		health -= dmg;
		if (typeID == Player) cout << "Player Health: " << health << endl;
		if (health <= 0 && !deathTimer->hasBeenRun()) {
			die();
		}
	}

	void DynamicGameObject::die() {
		Alive = false;
		particleHandler->explode();
		hide();
		deathTimer->Start(3.0f);
	}

	
	//This function refers to projectiles as bullets, but this class also applies to rocket projectiles
	bool DynamicGameObject::checkProjectileColisions(GameObject* target) {

		for (ProjectileGameObject* bullet : projectileArr) {//Itterate through currently existing projectiles shot by this object
			
			glm::vec3 targetPos = target->GetPosition();

			//This dont work, so I dont use it
			double t1 = NULL;
			double t2 = NULL;

			float radius = glm::length(target->GetScale()) / 2;
			bool rayCollision = bullet->rayCircleCollision(targetPos, radius , t1, t2);

			//decypher contact point of the bullet based on t1 and t2
			float bt = bullet->GetTime();
			bool bulletIsInContact;

			if (t2 == NULL || t1 == t2) {
				bulletIsInContact = bt == t1;
			}
			if (t1 > t2) {
				bulletIsInContact = t1 >= bt && bt >= t2;
			}
			else if (t1 < t2) {
				bulletIsInContact = t2 >= bt && bt >= t1;
			}
			else bulletIsInContact = false;

			if (rayCollision && bulletIsInContact) {
				if (bullet->isType(ProjectileGameObject::Rocket)) {
					bullet->explode();
					return false;
				}
				else if (bullet->isType(ProjectileGameObject::Bullet)) {
					bullet->clearObject();
					return true; //Only return true if enemy was hit by a bullet object so that ObjectManager can apply bullet dammage
				}
				
				return true;//return true in case of ambiguity on projectile type
				break;
			}
		}
		return false;
	}

	void DynamicGameObject::Update(double delta_time) {
		//If death timer has reached 0 and this is not the player, clear this entity
		if (deathTimer->isFinished() && typeID != Player) {
			clearObject();
		}

		//Update and Delete Necessary Projectiles
		auto It = projectileArr.begin();
		while (It != projectileArr.end()) {
			if ((*It)->isToBeCleared()) {
				delete* It;
				It = projectileArr.erase(It);
			}
			else {
				(*It)->Update(delta_time);
				++It;
			}
		}


		GameObject::Update(delta_time);
	}

}
