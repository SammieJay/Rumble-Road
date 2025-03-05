#ifndef OBJECT_MANAGER_H_
#define OBJECT_MANAGER_H_

#include "defs.h"

//GameObject includes
#include "game_object.h"
#include "enemy_game_object.h"
#include "player_game_object.h"
#include "collectible_game_object.h"
#include "static_game_object.h"
#include "projectile_game_object.h"

//Tool Includes
#include "random_gen.h"
#include "glm/ext.hpp"

//Particle Includes
#include "particles.h"
#include "particle_system.h"
#include "particle_handler.h"

namespace game {

    /*
    This class was made explicitly to manage arrays of GameObject pointers
    It should only have one instance as a member of the Game class
    It catalogues game objects into various arrays based on their subclass of game object
    It also returns pointers to objects when necessary
    */
    class ObjectManager {

    public:
        //The constructor gets passed pointers to important information that's required for making new objects
        //so that the manager can create new objects without getting passed redundant variables
        ObjectManager(Geometry* sprite, Shader* spriteShader, Shader* particleShader, string resourceDir, GLFWwindow* window_);
        ~ObjectManager();

        //Update function for operations that need to happen every update
        void Update(double delta_time);

        //Initialization Functions
        void initTextures(void);
        void initObjects(void);
        void initParticles(void);
        void initUI(void);

        //Fetch functions for the various kinds of GameObject
        PlayerGameObject* Player(void) { return player; }
        StaticGameObject* Background(void) const { return background; }
        
        std::vector<EnemyGameObject*> Enemies(void) const { return enemyArr; }
        EnemyGameObject* GetEnemy(int i)const;//return a specific enemy at indx i

        std::vector<CollectibleGameObject*> Items(void) const { return itemArr; }
        CollectibleGameObject* GetItem(int i)const;//return a specific item at indx i
        
        std::vector<StaticGameObject*> StaticObjects(void) const { return staticObjArr; }
        StaticGameObject* GetStaticObject(int i)const;//return specific static object at indx i

        //Add functions for the obj arrays
        void addEnemy(EnemyGameObject* enemy) { enemyArr.push_back(enemy); }
        void spawnEnemy(const glm::vec3& pos, EnemyGameObject::enemyType);

        void addItem(CollectibleGameObject* item) { itemArr.push_back(item); }
        void spawnItem(const glm::vec3& pos, CollectibleGameObject::itemType itemID);

        void addStaticObj(StaticGameObject* obj) { staticObjArr.push_back(obj); }
        void spawnStaticObj(const glm::vec3& pos, float angle, glm::vec2 scale, GameObject::objType type, string texDir);

        //functions for returning number of objects
        int numEnemies(void) const { return enemyArr.size(); }
        int numItems(void) const { return itemArr.size(); }
        int numStaticObj(void) const { return staticObjArr.size(); }

        //Dammage Distribution Functions
        void damageAOE(glm::vec3 origin, float radius, float dammageAtOrigin, GameObject::objType targets);

    private:
        //Object Pointers
        std::vector<EnemyGameObject*> enemyArr;
        std::vector<CollectibleGameObject*> itemArr;
        std::vector<StaticGameObject*> staticObjArr;
        PlayerGameObject* player;
        StaticGameObject* background;

        //Pointers to Game class members out of scope
        std::string resourceDir_;
        GLFWwindow* window_;

        //Shaders and geometry
        //Sprite
        Geometry* sprite_;
        Shader* spriteShader_;

        //Particles
        Geometry* explosionParticles_;
        Shader explosionParticleShader_;

        Geometry* rocketParticles_;
        Shader rocketParticleShader_;

        //Commonly Used Texture Handlers
        TextureHandler* playerTexHandler;
        TextureHandler* enemyTexHandler;
        TextureHandler* backgroundTexHandler;

        //Commonly Used Particle Handlers
        ParticleHandler* playerParticleHandler;
        ParticleHandler* enemyParticleHandler;

        //Misc
        RandomGenerator randomGen;

    }; // class ObjectManager

} // namespace game

#endif // OBJECT_MANAGER_H_
