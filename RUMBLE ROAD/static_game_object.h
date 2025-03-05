#ifndef STATIC_GAME_OBJECT_H_
#define STATIC_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    /*
    This class is the parent class for anything that does not include complex movement code
    This includes objects like: Items/Collectibles, non moving obstacles, the background, Sub elements like turrets & armor, etc
    */
    class StaticGameObject : public GameObject {

    public:
        StaticGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, TextureHandler* texhandler, objType type, float angle, glm::vec2 scale);
        StaticGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, objType type, float angle, glm::vec2 scale);
        ~StaticGameObject();

        //Static Object Update Function
        void Update(double delta_time);
        
        //Getters

    protected:

    }; // class StaticGameObject

} // namespace game

#endif // STATIC_GAME_OBJECT_H_
