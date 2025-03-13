#include "player_game_object.h"
#include "particle_handler.h"
#include "collectible_game_object.h"

namespace game {

/*
	PlayerGameObject inherits from DynamicGameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLFWwindow* windowPtr, TextureHandler* texHandler) : 
DynamicGameObject(position, geom, shader, Player, PLAYER_MAX_HEALTH) 
{
    //General Init
    this->windowPtr = windowPtr;

    //Texture Init
	this->texHandler = texHandler;
	texture_ = texHandler->getMainTex();
    
    //Movement Init
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    wheelTraction = true;

    turnRate = 0;//player turning variable *used to smooth out player turning
   
    isDrifting = false;

    //Projectile Related
    rocketTexture = new TextureHandler(ResourceDir + "/textures/rocket.png");
    bulletTexture = new TextureHandler(ResourceDir + "/textures/bullet.png");
    
    //Rocket Particle Handler Template Init
    TextureHandler* particleTex = new TextureHandler((ResourceDir + "/textures/orb.png").c_str());
    rocketParticleTemplate = new ParticleHandler(particleTex, ResourceDir, nullptr);
    rocketParticleTemplate->initParticleSystem(ParticleHandler::Rocket, glm::vec3(-0.5,0,1));
    rocketParticleTemplate->getParticleSys(ParticleHandler::Rocket)->SetScale(glm::vec2(0.2, 0.2));
    rocketParticleTemplate->initParticleSystem(ParticleHandler::Explosion);
    rocketParticleTemplate->getParticleSys(ParticleHandler::Explosion)->SetScale(glm::vec2(0.2, 0.2));

    //Timer Inits
    rocketFireRateClock = new Timer("Rocket Cooldown");
    turretFireRateClock = new Timer("Turret Cooldown");
    turretReloadClock = new Timer("Turret Reload");
    infiniteAmmoTimer = new Timer("Infinite Ammo");


    //Misc
    scale_ = glm::vec2(1.8, 1.0); //scale of the player


    initSubObjects();
}

PlayerGameObject::~PlayerGameObject() {
    if (hasSubObj()) {
        for (GameObject* obj : subObjArr) {
            delete obj;
        }
        subObjArr.clear();
    }
    delete turretFireRateClock;
    delete turretReloadClock;
    delete rocketFireRateClock;
    delete infiniteAmmoTimer;
    delete rocketTexture;
    delete bulletTexture;
    delete rocketParticleTemplate;
}

//=====PLAYER INPUT CONTROL FUNCTION=====

void PlayerGameObject::handlePlayerControls(double delta_time)
{

    float turnConst = 4.0f * delta_time;

    // Check for player input and make changes accordingly
    //Moving forward aclerates the fastest, then reversing, then sideways movement
    if (glfwGetKey(windowPtr, GLFW_KEY_W) == GLFW_PRESS) {
        addVelocity(maxAccel * delta_time, GetBearing()); // add velocity in direction of bearing
    }

    if (glfwGetKey(windowPtr, GLFW_KEY_S) == GLFW_PRESS) {
        addVelocity((maxAccel/1.25) * delta_time, GetBearing() * -1.0f);//Add velocity in opposite direction of bearing
    }

    if (glfwGetKey(windowPtr, GLFW_KEY_D) == GLFW_PRESS) {
        if (turnRate > maxTurnRate*-1) {
            turnRate -= turnConst;
        }
    }else if (turnRate < 0) turnRate += turnConst;

    if (glfwGetKey(windowPtr, GLFW_KEY_A) == GLFW_PRESS) {
        if (turnRate < maxTurnRate) {
            turnRate += turnConst;
        }
    }else if (turnRate > 0) turnRate -= turnConst;

    if (glfwGetKey(windowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(windowPtr, true);
    }

    if (glfwGetKey(windowPtr, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        shootRocket(rocketTexture, 8, 0.8, glm::vec2(0.8,0.4));
    }

    //TURRET CONTROLS
    if (glfwGetKey(windowPtr, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        turret->SetRotation(turret->GetRotation() - 2.5 * delta_time);
    }
    if (glfwGetKey(windowPtr, GLFW_KEY_LEFT) == GLFW_PRESS) {
        turret->SetRotation(turret->GetRotation() + 2.5 * delta_time);
    }
    if (glfwGetKey(windowPtr, GLFW_KEY_SPACE) == GLFW_PRESS) {
        fireTurret(20, 2);
    }
    if (glfwGetKey(windowPtr, GLFW_KEY_R) == GLFW_PRESS) {
        turretReloadClock->Start(turretReloadTime);
        turretMag = 0;
    }
}


//=====PLAYER MOVEMENT FUNCTIONS=====

//function to apply tractionary forces to the car, at different speeds the wheels act differently to make a good feeling drift effect
void PlayerGameObject::addWheelTraction() {
    float sideVelocity = glm::dot(velocity, GetRight());

    glm::vec3 brakingVector = glm::normalize(GetRight())*sideVelocity; //create a vector that represents the sideways movement of the player
    float curSpeed = glm::length(velocity);
    
    //after direction calculations, we want the absolute value of the players sideways velocity. It will make the rest of our calculations much easier
    sideVelocity = abs(sideVelocity); 


    float brakingConst = 0.1f;

    
    if ((sideVelocity>0.65*curSpeed || sideVelocity > 4)&&wheelTraction) {
        wheelTraction = false;
    }
    if ((curSpeed < 0.2 || sideVelocity < 0.02*curSpeed || sideVelocity < 2)&&!wheelTraction) {
        wheelTraction = true;
    }

    if (curSpeed > 0 && wheelTraction) {
        brakingConst = 0.08 / (1 + (std::exp(0.6*curSpeed + std::exp(2.0) - 11)));
    }
    else if (curSpeed > 0 && !wheelTraction) {
        brakingConst = 0.072 / (1 + (std::exp(5*curSpeed + std::exp(2.0) - 10)))+0.006;
    }
    
    //Apply the calculated braking scalar to the braking vector
    brakingVector *= brakingConst;

    //Apply Braking Force to Velocity Vector
    velocity -= brakingVector;

    //DEBUGGING OUTPUTS CAN IGNORE
    /*
    if (wheelTraction) { cout << "Wheel Traction!"; }
    else { cout << "No Wheel Traction!"; }
    cout << "Speed: " << curSpeed << "  ";
    cout << "Turn Rate: " << turnRate<<"  ";
    cout<<"Side Velocity: "<< sideVelocity << endl;
    */
}

void PlayerGameObject::capSpeed() {
    float curSpeed = glm::length(velocity);
    float bearingVelocity = glm::dot(GetBearing(), velocity);
    
    //Limit maximum overall velocity to maxVelocity
    if (curSpeed > maxVelocity) {
        velocity = glm::normalize(velocity) * maxVelocity;
    }
    
    //If player speed is below 0.01, stop the player
    if (curSpeed < 0.01) {
        velocity = glm::vec3(0, 0, 0);
    }

    //if player's forward speed is less than 20% of max velocity and more than 0, apply a slight passive braking effect along the bearing vector
    if (bearingVelocity < 0.2 * maxVelocity && bearingVelocity > 0) {
        velocity -= GetBearing()*0.003f;
    }
}

void PlayerGameObject::addVelocity(float magnitude, glm::vec3 dir) {
    glm::normalize(dir); // potentially redundant normalization of the direction vector just to be safe
    velocity += (dir * magnitude);
}

//function to calculate and apply the player's velocity vector each frame
const glm::vec3 PlayerGameObject::applyVelocity(double delta_time) {
    addWheelTraction();// apply sideways wheel friction to velocity vector
    capSpeed();//apply passive braking and enforce player speed limit

    cout << glm::length(velocity) << endl;

    const glm::vec3 newPos = position_ + (velocity * float(delta_time));
    SetPosition(newPos);
    return (newPos);
}

//Function to apply rotation to the player object
float PlayerGameObject::applyRotation(double delta_time) {
    
    float bearingVelocity = glm::abs(glm::dot(GetBearing(), velocity)); //the speed that the player is moving along the bearing vector (forwards or back)
    
    if (turnRate < 0.0149 && turnRate > -0.0149) turnRate = 0;

    float finalRotationVal = angle_;

    if (glm::length(velocity) > 0.25) {
        finalRotationVal = GetRotation() + (turnRate * delta_time);
    }
    
    SetRotation(finalRotationVal);
    return finalRotationVal;
}

//=====INITIALIZE SUB OBJECTS AND RELATED VARIABLES======

void PlayerGameObject::initSubObjects() {
    //This is where you init game objects for the player.
    
    //Initialize Turret Related Values
    localTurretPos = glm::vec3(-0.4, 0, 0);
    turretMaxAmmo = MAX_BULLETS;
    turretMag = turretMaxAmmo;
    turretReloadTime = 1.5f;
    turretShootCooldown = 0.1f;
    infiniteAmmoActive = false;

    //Initialize rocket related values
    rocketCount = MAX_ROCKETS;
    rocketCooldown = 2.5f;

    //Initialize Turret Object
    TextureHandler* turretTex = new TextureHandler(ResourceDir + "/textures/mg.png");
    glm::vec3 turretPos = position_ + localTurretPos;
    turret = new StaticGameObject(turretPos, geometry_, shader_, turretTex, SubObject, angle_, glm::vec2(1.5, 0.8));

    subObjArr.push_back(turret);
}

//=====TURRET OBJECT FUNCTIONS=====

// THE MATH IN THIS FUNCTION WAS IMPLEMENTED WITH THE HELP OF AI LLM TOOLS
//Apply heirarchichal transformation to the turret on the car so that it stays in given position
void PlayerGameObject::updateTurretLocation(float carAngle, glm::vec3 carPos) {

    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), carAngle, glm::vec3(0, 0, 1));

    glm::vec3 turretOffset = glm::vec3(rotationMatrix * glm::vec4(localTurretPos, 1.0));

    turret->SetPosition(position_ + turretOffset);
}

//Function that shoots the turret and reloads when necessary
void PlayerGameObject::fireTurret(float speed, float lifeSpan) {
    if ((turretFireRateClock->isFinished() && (turretReloadClock->isFinished()))
        || (!turretFireRateClock->isRunning() && !turretReloadClock->isRunning())) {//if the fire rate clock is not running or is finished, shoot a bullet
        ProjectileGameObject* bullet = new ProjectileGameObject(geometry_, shader_, bulletTexture, lifeSpan, speed, glm::vec2(1.5,0.5), turret, ProjectileGameObject::Bullet);

        projectileArr.push_back(bullet);
        if(!infiniteAmmoActive)turretMag--;
        if (turretMag == 0 && !infiniteAmmoActive) {
            turretReloadClock->Start(turretReloadTime);
        }
        else turretFireRateClock->Start(turretShootCooldown);
    }
}

//Function to reload turret (set magazine value to MAX_BULLETS if reload is complete)
void PlayerGameObject::reloadTurret() {
    turretMag = MAX_BULLETS;
}

//=====PLAYAER ROCKET FUNCTION=====
void PlayerGameObject::shootRocket(TextureHandler* texHandler, float speed, float lifeSpan, glm::vec2 scale) {
    //If shooting cooldown timer is not currently running, run this code
    if ((rocketFireRateClock->isFinished() || !rocketFireRateClock->isRunning()) && rocketCount > 0) {
        GameObject* ptr = this;
        ProjectileGameObject* rocket = new ProjectileGameObject(geometry_, shader_, texHandler, lifeSpan, speed, scale, ptr, ProjectileGameObject::Rocket);

        //Copy particle handler and add to rocket
        ParticleHandler* rocketParticleHandler = new ParticleHandler(rocketParticleTemplate, rocket);
        rocket->setParticleHandler(rocketParticleHandler);

        projectileArr.push_back(rocket);//create and add new projectile to projectile array
        rocketCount--;
        rocketFireRateClock->Start(rocketCooldown);
        cout << "Rocket Count is: " << rocketCount << endl;
    }
}

//=====ITEM RELATED FUNCTIONS=====
void PlayerGameObject::activateItem(int type) {
    if (type == CollectibleGameObject::health) {
        health += HEALTH_ITEM_VALUE;
        if (health > PLAYER_MAX_HEALTH) health = PLAYER_MAX_HEALTH;
        cout << "Health Pack Collected, Health is now: " << health << endl;
    }
    if (type == CollectibleGameObject::rocket) {
        rocketCount = MAX_ROCKETS;
        cout << "Rockets Restocked to 3" << endl;
    }
    if (type == CollectibleGameObject::infiniteAmmo) {
        infiniteAmmoActive = true;
        infiniteAmmoTimer->Start(INFINITE_AMMO_DURRATION);
        cout << "Infinite Turret Ammo Activated For the next " << INFINITE_AMMO_DURRATION << " seconds!" << endl;
    }
}


//=====PLAYER UPDATE FUNCTION=====
// Update function for the player object
void PlayerGameObject::Update(double delta_time) {
    //Getting Player Keyboard Input and Acting Accordingly
    if(Alive) handlePlayerControls(delta_time);

    //Update Player's Sub Objects
    if (hasSubObj()) {
        for (GameObject* obj : subObjArr) {
            obj->Update(delta_time);
        }
    }
    
    //Apply transformations to player and it's sub objects
    if (Alive) {
        float newAngle = applyRotation(delta_time);
        const glm::vec3 newPos = applyVelocity(delta_time);
        updateTurretLocation(newAngle, newPos);
    }

    //Check Infinite Ammo Timer
    if (infiniteAmmoTimer->isFinished()) {
        infiniteAmmoActive = false;
        cout << "Infinite Ammo Expired" << endl;
    }

    //Check reloading timer
    if (turretReloadClock->isFinished() && turretMag == 0) {
        reloadTurret();
    }
    
    // Call the parent's update method to move the object in standard way, if desired
    DynamicGameObject::Update(delta_time);
}


} // namespace game
