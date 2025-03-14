#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "dynamic_game_object.h"
#include "static_game_object.h"
#include "track_game_object.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public DynamicGameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLFWwindow* windowPtr, TextureHandler* texHandler);
            ~PlayerGameObject();//todo still

            //Boolean Returns
            inline bool deathTimerOver() { return deathTimer->isFinished(); }

            //Item Related
            void activateItem(int);//pass item type enum as int to avoid circular inclusions
            
            
            // Update function for moving the player object around
            void Update(double delta_time) override;

            //Getters for UI display
            int getBulletMag() { return turretMag; }
            int getRocketMag() { return rocketCount; }
            bool turretReloading() { return turretReloadClock->isRunning(); }
            bool Drifting() { return !wheelTraction; } //returns whether player is currently in state of drift

            inline std::list<TrackObject*> GetTrackObjArr(void) { return trackObjArr; } // retrn array of track objects
            inline bool hasTracks(void) { return !trackObjArr.empty(); }

        private:
            GLFWwindow* windowPtr;//Player keeps a pointer to the game's window so that the player obj can acess keypress updates
            const string ResourceDir = RESOURCES_DIRECTORY;//So the player object can acess texture assets

            //Player Movement Constants
            const float maxVelocity = 11.5f;
            const float maxTurnRate = 1.8f; //determines the maximum rate at which the player can rotate in degrees per update
            const float maxAccel = 8.0f; //determines how quickly the player can accelerate

            float turnRate; //the current turning force exerted on the player via player controls

            glm::vec3 velocity;    //Player's total velocity vector
            bool wheelTraction;    //boolean determines whether the wheels are currently in a state of high traction or low traction
            

            //Player Movement Functions
            void handlePlayerControls(double delta_time);
            void addVelocity(float amnt, glm::vec3 dir);
            void addWheelTraction(double delta_time);
            void capSpeed(double delta_time);
            const glm::vec3 applyVelocity(double delta_time);
            float applyRotation(double delta_time);
           

            //Projectile Related
            TextureHandler* rocketTexture;
            TextureHandler* bulletTexture;
            ParticleHandler* rocketParticleTemplate;
            Timer* rocketFireRateClock;

            //Sub Objects
            void initSubObjects();

            //Track Placing Code
            void placeTrackObj(glm::vec3 pos, double delta_time);//function to place track object in world at location
            std::list<TrackObject*> trackObjArr; //double linked list to store references to all track objects
            Timer* trackDelay; //timer to time the delay between track object placements

            //Position returns for track placement
            glm::vec3 getWheelPos(int end, int side);//return the position of the wheel depending on given inputs
            
            //Turret Related *note all turret related values are initialized in initSubObjects() function
            StaticGameObject* turret;//Turret object on car
            glm::vec3 localTurretPos;//position of turret relative to car
            Timer* turretFireRateClock;
            Timer* turretReloadClock;
            int turretMag;
            int turretMaxAmmo;
            float turretReloadTime;
            float turretShootCooldown;
            
            void updateTurretLocation(float carAngle, glm::vec3 carPos);
            void fireTurret(float speed, float lifeSpan);
            void reloadTurret();

            //Rocket Related
            int rocketCount;
            float rocketCooldown;
            void shootRocket(TextureHandler*, float speed, float lifeSpan, glm::vec2 scale);

            //Item Related
            bool infiniteAmmoActive;
            Timer* infiniteAmmoTimer;

    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
