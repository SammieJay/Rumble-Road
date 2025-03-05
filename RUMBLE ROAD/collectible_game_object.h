#ifndef COLLECTIBLE_GAME_OBJECT_H_
#define COLLECTIBLE_GAME_OBJECT_H_

#include "static_game_object.h"

namespace game {
    class PlayerGameObject;

    // Inherits from StaticGameObject
    /*
    This class is a child of StaticGameObject that handles the code for any items that the player can pick up
    This class get's a texture handler passed to it through the constructor, since there may be many kinds of item to handle immages for
    */
    class CollectibleGameObject : public StaticGameObject {
    public:
        //enum to specify the purpose of the item or the effect it gives the player when attained
        enum itemType { health, infiniteAmmo, rocket};
        
        CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, itemType itemID);
        ~CollectibleGameObject();
        
        void Update(double delta_time) override;

        void collect(PlayerGameObject*);
    protected:
        itemType itemID;
        string ResourceDir;

    }; // class CollectibleGameObject

} // namespace game

#endif // COLLECTIBLE_GAME_OBJECT_H_