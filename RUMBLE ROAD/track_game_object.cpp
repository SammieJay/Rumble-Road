#include "track_game_object.h"

namespace game {

	/*
		TrackObject inherits from StaticGameObject
		They are placed at each of the 4 wheel locations of the car when it is drifting to display the state to the player
	*/

	//Constructor passes the propper objType to the GameObject constructor so that the user doesnt need to except for default GameObject declarations
	TrackObject::TrackObject(const glm::vec3& position, float angle, Geometry* geom, Shader* shader) : 
	StaticGameObject(position, geom, shader, Track, angle, glm::vec2(0.4, 0.2)) {
		ResourceDir = RESOURCES_DIRECTORY;
		string TexDir = "/textures/orb.png";
		
		TextureHandler* textureHandler = new TextureHandler(ResourceDir+TexDir);

		texture_ = textureHandler->getMainTex();
		texHandler = textureHandler;
		
	}

	TrackObject::~TrackObject() {}

	void TrackObject::Update(double delta_time) {

		// Call the parent's update method to move the object in standard way, if desired
		StaticGameObject::Update(delta_time);
	}



} // namespace game