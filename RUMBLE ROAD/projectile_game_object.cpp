#include "projectile_game_object.h"
#include "particle_handler.h"

namespace game {

	ProjectileGameObject::ProjectileGameObject(Geometry* geom, Shader* shader, TextureHandler* texHandler, float lifeSpan, float speed, glm::vec2 scale, GameObject* source, projectileType type) :
		GameObject(source->GetPosition(), geom, shader, Projectile)
	{
		texture_ = texHandler->getMainTex();
		direction = source->GetBearing();
		SetRotation(source->GetRotation());
		originPoint = source->GetPosition();
		scale_ = scale;
		type_ = type;


		velocity = glm::normalize(direction) * speed;

		lifeClock = new Timer();
		deathClock = new Timer();
		lifeClock->Start(lifeSpan);
	}

	ProjectileGameObject::~ProjectileGameObject() {
		delete lifeClock;
	}

	void ProjectileGameObject::explode() {
		particleHandler->getParticleSys(ParticleHandler::Rocket)->hide();
		particleHandler->explode();
		detonated = true;
		hide();
		deathClock->Start(3);
	}

	bool ProjectileGameObject::rayCircleCollision(glm::vec3 targetPos, float tagetRadius, double &t1, double &t2) {
		if (!showing) {
			return false;
		}
		glm::vec3 originToCenter = originPoint - targetPos;

		//Calculate quadratic coefficients
		double a = glm::dot(velocity, velocity);

		double b = 2 * glm::dot(velocity, originToCenter);

		double c = glm::dot(originToCenter, originToCenter) - (tagetRadius * tagetRadius);

		//Calculate Discriminant
		double discriminant = b * b - 4 * a * c;

		//Use Quadratic Formula to calculate t values of collision points
		if (discriminant > 0) { //2 real sollutions (entry and exit points)
			t1 = (-b + std::sqrt(discriminant)) / (2 * a);
			t2 = (-b - std::sqrt(discriminant)) / (2 * a);
			return true;
		}
		else if (discriminant == 0) { //1 real sollution, a tangential collision *unlikely case
			t1 = -b / (2 * a);
			return true;
		}
		else { // no sollution found, ray missed entirely, return false
			return false;
		}
	}

	//projectle update function
	void ProjectileGameObject::Update(double delta_time) {
		if (deathClock->isFinished()) {//If explosion animation is over, tell Parent to delete this object
			clear = true;
		}
		if(lifeClock->isFinished() && showing && type_ == Rocket) explode();
		if (lifeClock->isFinished() && type_ == Bullet) clearObject();

		//update position
		if (!deathClock->isRunning()) {
			float positionIncrement;
			SetPosition(originPoint + velocity * float(time));
			
		}
		time += delta_time;

		GameObject::Update(delta_time);
	}
}