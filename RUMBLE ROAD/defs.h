#ifndef DEFS_H_
#define DEFS_H_
//This file is for the easy inclusion of libraries
#include <path_config.h>
#include <unordered_map>
#include <stdexcept>
#include <string>
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <thread>

//Definitions for game stats
#define ROCKET_DMG 3
#define BULLET_DMG 0.75
#define HEALTH_ITEM_VALUE 3
#define INFINITE_AMMO_DURRATION 6
#define PLAYER_MAX_HEALTH 999999
#define COLLISION_DMG 3

//Enemy Stat Definitions
#define TOWER_HEALTH 6
#define TOWER_ROCKET_COLLDOWN 5
#define TOWER_DMG 4
#define NUM_TOWERS 5

#define BOMB_CAR_HEALTH 1.5
#define BOMB_CAR_DMG 4

//Startup
#define NUM_CARS 10
#define NUM_BIKE 5


namespace game { 
	//I got tired of tagging these things as std::
	using std::string;
	using std::cout;
	using std::endl;

}

#endif