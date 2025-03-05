#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "dynamic_game_object.h"
#include "defs.h"

namespace game {
    class StaticGameObject;

    // Inherits from DynamicGameObject
    //Class in charge of controlling all enemy types and their behaviors
    class EnemyGameObject : public DynamicGameObject {
    public:
        enum enemyType { Tower, Bomb, Melee };//denote which type of enemy this object is

        EnemyGameObject(GameObject* player, const glm::vec3& position, Geometry* geom, Shader* shader, TextureHandler* texHandler, enemyType type);
        ~EnemyGameObject();

        // Update function for moving the object around
        void Update(double delta_time) override;
        
        inline float distToPlayer(void) { return glm::distance(position_, player->GetPosition()); }

        //Boolean Returns
        inline bool isEnemyType(enemyType type) { return type == enemyType_; }
        inline bool hasProjectiles() { return !projectileArr.empty(); }
        inline bool onlyExploDmg() { return explosiveDmgOnly; }//return true of enemy can only be dammaged by explosions

    protected:
        string ResourceDir;

        //Various Protected Memebr Variables
        enum behaviorType { Wait, Patrolling, Chase, Flee};
        enemyType enemyType_;
        behaviorType behavior_;
        GameObject* player;
        bool explosiveDmgOnly;

        //Init enemy types
        void initTower();
        void initBoomCar();
        void initMelee();

        //Update enemy types
        void updateTower(double delta_time);
        void updateBoomCar(double delta_time);
        void updateMelee(double delta_time);

        //Tower Enemy Exclusive
        StaticGameObject* turret;
        TextureHandler* turretTex;
        TextureHandler* rocketTex;
        TextureHandler* particleTex;
        ParticleHandler* rocketParticleTemplate;
        Timer* rocketCooldownTimer;
        void shootRocket();

        //Boom Car Enemy Exclusive
        float patrolRad;
        glm::vec3 patrolOrigin;
        float t;
        
        //MOVEMENT RELATED VARIABLES
        glm::vec3 velocity;
        float speed;

    }; // class EnemyGameObject

} // namespace game

#endif // ENEMY_GAME_OBJECT_H_
