Name: Samuel Abraham
Stu#: 101249272
OS: Windows 10 x64

Art Asset Credits can be found in /Textures/Texture Credits.txt

===INTRODUCTION TO RUMBLE ROAD=====
Welcome to Rumble Road! Kill all the missile towers that have cropped up in the desert to win!
Careful though because only your car's limited suply of rockets can take down a tower. Hit one with 2 rockets to take it down.
Your car has also been equiped with a new fangled bad guy deleter 9000 machine gun! so get out there and go wild!

CONTROLS:
WASD - car movement
LSHIFT - Fire Rocket
SPACE - Fire Roof Turret
LEFT/RIGHT ARROW - rotate roof turret
R - Reload Roof Turret

*note many damage, health, and enemy stats can be modified through definitions in the defs.h file if the game is too hard/easy so feel free to mess around with those

==Specification Notes==
1. the win condition for this game is destroying all 5 missile towers around the map with rockets (the towers are immune to bullet damage)
*number of towers spawned in the game can be adjusted in defs.h along with other in-game stats

2. there are 3 enemy types, 2 of them with implemented AI and attack patterns
MISSILE TOWER
- Tracks the player if they get too close
- fires rockets at the player every 6 seconds by default
- Immune to bullet damage
- player dies upon colision with a tower

BOMB CAR
- will patrol in a circle until the player gets too close
- getting too close causes the car to relentlessly chase the player through steering mechanics
- upon reaching the player it will self detonate, dealing significant dammage

LITTLE GUY
- he's just a little guy on a bike
- does his own thing
- low health and no AI
- running into this enemy will cause you damage when they die however

3. 
Player has acess to 2 weapons
ROOF TURRET
- Controlled through left and right arrows and space to shoot
- deals less dammage but has infinite ammo and high fire rate
- has a 16 round magazine and has to reload
- reload early by pressing r key

ROCKETS
- launched with shift key
- detonate after short ammount of time or upon connecting with an enemy
- Player can hold up to 3 rockets at a time, replenish your stock by picking up a rocket bundle item off the ground

4. 
There are 3 collectible items in the game
HEALTH PICKUP
- simple health item
- heals player for 3 health

INFINITE AMMO PICKUP
- gives player infinite turret ammo for 6 seconds

ROCKET BUNDLE
- replenishes player's rocket count to 3

5.
- player and enemy objects move via physcially based movement
- projectiles and patrolling bomb cars move via parametric movement

6. 
- colision detection is done through:
circle to circle (enemy-player colisions)
ray to circle (projectile-enemy/player colisions)

7. 
game world is a 2x2 tiled image

8. 
Particles are used for both rocket trails and explosions

9. 
UI not implemented

10. 
Bomb Car has implemented steering behavior to chase the player

==Implementation Notes==
- ObjectManager class
	- In charge of storing, spawning, clearing, deallocating, and updating all game objects
	- Stores objects in vector arrays
	- Only has one instance
	- in charge of all collision handling (except for the ray-circle collision which is in the ProjectileGameObject class)
- Texture Handler Class
	- A class used to store and manage textures for each kind of GameObject
	- Handlers are initialized in the object manager, then are passed to GameObjects by reference in their constructor
	- Stores textures using a hashtable
	- Was made so that game objects can have direct acess to their own textures, instead of having to change all the textures from Game::Update()
		(keys are visible above handler initialisation in object_manager.cpp)
- GameObject Subclasses
	- Dynamic Game Object
		- Parent of Enemy Game Object & Player Game Object
		- Handles health and dammage mechanics
		- Holds and updates projectile game Objects
	- Static Game Object
		- Parent of Collectible Game Object
		- Used for simple objects like the roof turret
- Game Class
	- Mostly just in charge of setup
	- Render functions remain largely unchanged, objects are just acessed via an the ObjectManager class
	- Tells objectmanager where to place AOE damage effects when an object 'detonates' (sets its 'detonated' boolean to true)
	- Moved Keyboard Input Handling into player_game_object.cpp

- Particle Handler
	- stores and produces particle effects for the parent class
		


