#ifndef DYNAMIC_GAME_OBJECT_H_
#define DYNAMIC_GAME_OBJECT_H_

#include "projectile_game_object.h"
#include "game_object.h"
#include "defs.h"

namespace game {

    /*
    This class is the abstract parent class for all objects wtih advanced movement and pathing mechanics
    Children of this class are all moving entities wthin the game
    The functions from this class involve:
    Health and Dammage Mechanics
    Death Mechanics
    Projectile Creation and Management
    */
    class DynamicGameObject : public GameObject {

    public:
        DynamicGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, objType type, float hp);
        ~DynamicGameObject();
        
        
        // Update function for dynamic objects
        virtual void Update(double delta_time) override;

        //Misc Functions
        inline int getHealth(void) { return health; }
        void takeDmg(float);
        void die(void);
        inline bool isAlive(void) { return Alive; }
        
        //Projectile Related
        //return the list of projectiles
        inline std::vector<ProjectileGameObject*> GetProjectiles(void) { return projectileArr; }
        bool checkProjectileColisions(GameObject* target);

        //Sub Obj Related
        int numSubObj(void) { return subObjArr.size(); }
        bool hasSubObj(void) { return !subObjArr.empty(); }
        GameObject* GetSubObject(int indx) { return subObjArr[indx]; }

    protected:
        //Misc
        float health;
        bool Alive;
        Timer* deathTimer;

        //TEXTURE RELATED FUNCTIONS & VARIABLES
        inline void setToDeathTex(void) { texture_ = texHandler->getTex("death"); }

        //PROJECTILE RELATED
        std::vector<ProjectileGameObject*> projectileArr; //Array to store projectile object pointers

        //subObject related
        std::vector<GameObject*> subObjArr;


       
        

    }; // class DynamicGameObject

} // namespace game

#endif // Dynamic_GAME_OBJECT_H_
