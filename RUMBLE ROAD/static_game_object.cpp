#include "static_game_object.h"

namespace game {
	StaticGameObject::StaticGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, objType type, float angle, glm::vec2 scale) : 
	GameObject(position, geom, shader, type) {
		angle_ = angle;
		scale_ = scale;
	}

	StaticGameObject::StaticGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, TextureHandler* texhandler, objType type, float angle, glm::vec2 scale) :
		GameObject(position, geom, shader, type) {
		texHandler = texhandler;
		texture_ = texHandler->getMainTex();
		angle_ = angle;
		scale_ = scale;
	}

	StaticGameObject::~StaticGameObject() {
		delete texHandler;
	}

	void StaticGameObject::Update(double delta_time) {

		GameObject::Update(delta_time);
	}

	

}