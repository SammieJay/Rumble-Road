#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#define GLEW_STATIC

#include "defs.h"

#include "shader.h"
#include "geometry.h"
#include "texture_handler.h"
#include "timer.h"

namespace game {

    class ParticleSystem;
    class ParticleHandler;

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */

    class GameObject {

        public:
            // Enum used for denoting the purpose of an object
            enum objType { Player, Enemy, Item, World, Background, Projectile, SubObject , ParticleSys, UI, Track};
                //Player:       is the player object
                //Enemy:        is an enemy object
                //Item:         is something that the player can pick up off the ground
                //World:        static item in the game world
                //Background:   non interactable background object
                //Projectile:   moving object originating from another object with a limited lifespan
                //SubObject:    an object that belongs and moves alongside another (typically dynamic) object
                //ParticleSys:  a particle emiter

            // Constructor
            GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, objType type);
            ~GameObject();

            //GETTERS
            inline glm::vec3 GetPosition(void) const { return position_; }
            inline glm::vec2 GetScale(void) const { return scale_; }
            inline float GetRotation(void) const { return angle_; }
            inline float GetTime(void) const { return time; }
            int numParticleSys(void) const;
            ParticleSystem* GetParticleSys(int indx);
            
            
            //SETTERS
            inline void SetPosition(const glm::vec3& position) { position_ = position; }
            inline void SetScale(glm::vec2 scale) { scale_ = scale; }
            void SetRotation(float angle);
            void clearObject(void) { clear = true; }
            void setParticleHandler(ParticleHandler*);
            
            //BOOLEAN RETURNS
            bool isType(objType isType) const { return typeID == isType; }
            bool isMoveable(void) const { return isStatic; }
            bool isToBeCleared(void) const { return clear; }
            bool hasParticleSys(void);
            bool hasParticleHandler(void) const { return particleHandler != nullptr; }

            //POSITIONING AND VECTOR FUNCTIONS
            glm::vec3 GetBearing(void) const;   // Get bearing direction vector
            glm::vec3 GetRight(void) const;     // Get vector pointing to the right side of the game object

            //RENDERING AND UPDATING
            virtual void Render(glm::mat4 view_matrix, double current_time);// Renders the GameObject
            virtual void Update(double delta_time);// Update the GameObject's state. Can be overriden in children

            //TOGGLE GHOST MODE
            void toggleGhost();

            //Show and hide object
            inline void show() { showing = true; }
            inline void hide() { showing = false; }
            inline bool isShowing() { return showing; }

            //Detonation Related Functions
            //Detonation is checked by ObjectHandler to determine if an object is exploding and should deal dammage in an AOE
            inline bool isDetonated() { return detonated; }
            inline void resetDetonation() { detonated = false; }//this is used to make sure enemies are not dammaged multiple times per detonation



        protected:
            objType typeID;
            bool isStatic;//a boolean to describe whether the object can be currently moved
            bool clear; // if this boolean is true, the obect should be cleared from memory on the next update
            bool showing;
            bool detonated; 

            // Object's Transform Variables
            glm::vec3 position_;
            glm::vec2 scale_;
            float angle_;

            //Time Variable (used in parametric equations)
            double time;

            // Geometry
            Geometry *geometry_;
 
            // Shader & textures
            Shader *shader_;
            GLuint texture_;
            TextureHandler* texHandler;

            //GHOST MODE
            bool ghostMode;

            //Particle Systems
            ParticleHandler* particleHandler;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
