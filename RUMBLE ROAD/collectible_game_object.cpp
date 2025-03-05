#include "collectible_game_object.h"
#include "player_game_object.h"

namespace game {

	/*
		CollectibleGameObject inherits from StaticGameObject
		its used for any object that the player can pick up off the ground
		examples include: cash/coins, powerups, etc
	*/

	//Constructor passes the propper objType to the GameObject constructor so that the user doesnt need to except for default GameObject declarations
	CollectibleGameObject::CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, itemType itemID) : 
	StaticGameObject(position, geom, shader, Item, 0, glm::vec2(1.0, 1.0)) {
		ResourceDir = RESOURCES_DIRECTORY;
		string TexDir;

		//Set scale and texture of the item based on it's itemID
		if (itemID == health) {
			TexDir = "/textures/health_item.png";
			scale_ = glm::vec2(1, 1);
		} 
		if (itemID == infiniteAmmo) {
			TexDir = "/textures/ammo_item.png";
			scale_ = glm::vec2(1, 1);
		} 
		if (itemID == rocket) {
			TexDir = "/textures/rocket_bundle_item.png";
			scale_ = glm::vec2(0.8, 1.2);
		} 

		this->itemID = itemID;
		
		TextureHandler* textureHandler = new TextureHandler(ResourceDir+TexDir);
		texture_ = textureHandler->getMainTex();
		texHandler = textureHandler;
		
	}

	CollectibleGameObject::~CollectibleGameObject() {}

	// Update function for moving the player object around
	void CollectibleGameObject::Update(double delta_time) {

		// Special collectible updates go here


		// Call the parent's update method to move the object in standard way, if desired
		StaticGameObject::Update(delta_time);
	}

	void CollectibleGameObject::collect(PlayerGameObject* player) {//when this function is called, the object is 'collected' and therefore deleted from the game world
		cout << "Item Collected" << endl;
		player->activateItem(itemID);
	}



} // namespace game