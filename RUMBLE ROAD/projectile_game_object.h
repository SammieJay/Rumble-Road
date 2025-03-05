#ifndef PROJECTILE_GAME_OBJECT_H_
#define PROJECTILE_GAME_OBJECT_H_

#include "game_object.h"
#include "timer.h"

namespace game {

class ProjectileGameObject : public GameObject {
public:
	enum projectileType {Rocket, Bullet};
	ProjectileGameObject(Geometry* geom, Shader* shader, TextureHandler* texHandler, float lifeSpan, float speed, glm::vec2 scale, GameObject* source, projectileType type);
	~ProjectileGameObject();

	void Update(double delta_time) override;
	bool rayCircleCollision(glm::vec3 targetPos, float radius, double &t1, double &t2);

	inline bool isType(projectileType type) { return type == type_; }

	void explode();

private:
	glm::vec3 direction;
	glm::vec3 velocity;
	glm::vec3 originPoint;
	Timer* lifeClock;
	Timer* deathClock;
	projectileType type_;
};


}
#endif