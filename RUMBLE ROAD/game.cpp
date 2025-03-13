#include "game.h"

namespace game {
    // Globals that define the OpenGL window and viewport
    const char* window_title_g = "Game Demo";
    const unsigned int window_width_g = 1400;
    const unsigned int window_height_g = 1000;
    const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

    // Directory with game resources such as textures
    const std::string resources_directory_g = RESOURCES_DIRECTORY;
Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}

void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set whether window can be resized
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }




    //Allow polling of cursor position
    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    //Print Version Info
    const GLubyte* version = glGetString(GL_VERSION);
    cout << "OpenGL Version: " << version << endl;

    // Initialize sprite geometry
    sprite_ = new Sprite();
    sprite_->CreateGeometry();

    particles_ = new Particles();
    particles_->CreateGeometry();

    // Initialize sprite shader
    sprite_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/sprite_fragment_shader.glsl")).c_str());
    particle_shader_.Init((resources_directory_g + std::string("/particle_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/particle_fragment_shader.glsl")).c_str());
    
    
    //Initialize Game Object Manager and pass in initialized shader pointers
    gameObjectManager = new ObjectManager(sprite_, &sprite_shader_, &particle_shader_, resources_directory_g, window_);

    //ui setup
    uiHandler_ = new uiHandler(gameObjectManager, window_, &towerCount);


    // Initialize time
    current_time_ = 0.0;
}


Game::~Game()
{
    // Free memory for all objects
    // Only need to delete objects that are not automatically freed
    
    //std::cout << "Game Destruct Called, Program Ending" << std::endl;
    delete uiHandler_;
    delete sprite_;
    delete gameObjectManager;

    // Close window
    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Game::Setup(void)
{
    randomGen = RandomGenerator();
 
    // Setup the game world
    gameObjectManager->initTextures();
    //gameObjectManager->initUI();
    gameObjectManager->initObjects();
    gameObjectManager->initParticles();
    
    //Init UI
    uiHandler_->initUI();

    GameOverTimer = new Timer();
    enemySpawnTimer = new Timer();
    itemSpawnTimer = new Timer();

    gameObjectManager->spawnEnemy(glm::vec3(3, 3, 0), EnemyGameObject::Bomb);
    
    //spawn in NUM_TOWERS towers at random locations at start of game
    for (int i = 0; i < NUM_TOWERS; i++) {
        glm::vec3 pos = randomGen.randomPoint();
        gameObjectManager->spawnEnemy(pos, EnemyGameObject::Tower);
    }

    //Spawn other enemies across the map
    for (int i = 0; i < NUM_CARS; i++) {
        glm::vec3 pos = randomGen.randomPoint();
        gameObjectManager->spawnEnemy(pos, EnemyGameObject::Bomb);
    }

    for (int i = 0; i < NUM_BIKE; i++) {
        glm::vec3 pos = randomGen.randomPoint();
        gameObjectManager->spawnEnemy(pos, EnemyGameObject::Melee);
    }



    //Spawn in some items randomly at the start of the game
    for (int i = 0; i < 5; i++) {
        glm::vec3 pos = randomGen.randomPoint();
        gameObjectManager->spawnItem(pos, CollectibleGameObject::rocket);
    }

    for (int i = 0; i < 2; i++) {
        glm::vec3 pos = randomGen.randomPoint();
        gameObjectManager->spawnItem(pos, CollectibleGameObject::infiniteAmmo);
    }

    for (int i = 0; i < 3; i++) {
        glm::vec3 pos = randomGen.randomPoint();
        gameObjectManager->spawnItem(pos, CollectibleGameObject::health);
    }
}

void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


void Game::MainLoop(void)
{
    // Loop while the user did not close the window
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window_)){

        // Calculate delta time
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        // Update window events like input handling
        glfwPollEvents();

        // Update all the game objects
        Update(delta_time);

        uiHandler_->uiUpdate();

        // Render all the game objects
        Render();

        //Render UI elements
        uiHandler_->RenderUI();

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);
    }
}


void Game::Update(double delta_time)
{
    // Update time
    current_time_ += delta_time;

    //Update Game Objects
    gameObjectManager->Update(delta_time);
    glm::vec3 pos = gameObjectManager->Player()->GetPosition();


    //spawn enemy at random times
    if (enemySpawnTimer->isFinished() || !enemySpawnTimer->hasBeenRun()) {
        gameObjectManager->spawnEnemy(randomGen.randomPoint(), EnemyGameObject::Bomb);
        enemySpawnTimer->Start(randomGen.randomInt(4,7));
    }

    if (itemSpawnTimer->isFinished() || !itemSpawnTimer->hasBeenRun()) {
        int itemType;
        int rand = randomGen.randomInt(0, 6);
        if (rand == 1) itemType = 2;
        else if (rand < 3) itemType = 3;
        else itemType = 1;
        enemySpawnTimer->Start(randomGen.randomInt(3,6));
    }


    //if player dead then end game
    if (!gameObjectManager->Player()->isAlive() && !GameOverTimer->hasBeenRun()) {
        GameOverTimer->Start(3);
        cout << "GAME OVER" << endl;
    }

    if (GameOverTimer->isFinished()) { 
        delete this; 
    }
    
}

void Game::Render(void){
    // Clear background
    glClearColor(viewport_background_color_g.r,
                 viewport_background_color_g.g,
                 viewport_background_color_g.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use aspect ratio to properly scale the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    glm::mat4 window_scale_matrix;
    if (width > height){
        float aspect_ratio = ((float) width)/((float) height);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
    } else {
        float aspect_ratio = ((float) height)/((float) width);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f/aspect_ratio, 1.0f));
    }

    //Create a camera position matrix and move it to the position of the player
    glm::mat4 camera_position_matrix;
    camera_position_matrix = glm::translate(camera_position_matrix, gameObjectManager->Player()->GetPosition()*(-1.0f));

    // Set view to zoom out, centered by default at 0,0
    float camera_zoom = 0.06f;
    glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom));
    glm::mat4 view_matrix = window_scale_matrix * camera_zoom_matrix * camera_position_matrix;

    std::vector<ParticleSystem*> particleSystems;


    //RENDER ALL GAME OBJECTS

    // Render all ENEMY game objects
    towerCount = 0;
    for (int i = 0; i < gameObjectManager->numEnemies(); i++) {
        EnemyGameObject* enemy = gameObjectManager->GetEnemy(i);
        
        if (enemy->hasParticleHandler() && enemy->hasParticleSys()) {
            for (int i = 0; i < enemy->numParticleSys(); i++) {
                particleSystems.push_back(enemy->GetParticleSys(i));
            }
        }

        if (enemy->hasSubObj()) {//If player has sub objects, render them on top of the player sprite
            for (int i = 0; i < enemy->numSubObj(); i++) {
                if (enemy->isShowing())enemy->GetSubObject(i)->Render(view_matrix, current_time_);
            }
        }

        //Check for enemy projectiles to render
        if (enemy->hasProjectiles()) {//run this if enemy has projectiles
            std::vector<ProjectileGameObject*> projectiles = enemy->GetProjectiles();
            for (ProjectileGameObject* obj : projectiles) {
                if (obj->hasParticleSys()) {//if the projectile has particle systems, add them to particle render que
                    //Add projectiles particle systems to particle system que
                    for (int i = 0; i < obj->numParticleSys(); i++) particleSystems.push_back(obj->GetParticleSys(i));
                }

                if (obj->isDetonated()) {
                    gameObjectManager->damageAOE(obj->GetPosition(), 3.5f, TOWER_DMG, GameObject::Player);
                    obj->resetDetonation();
                } 

                if (obj->isShowing())obj->Render(view_matrix, current_time_);
            }
        }

        //Check if enemy has self detonated, if so create AOE dammage at that location
        if (enemy->isDetonated()) {
            gameObjectManager->damageAOE(enemy->GetPosition(), 3.5f, BOMB_CAR_DMG, GameObject::Player);
            enemy->resetDetonation();
        }


        //Count how many missile towers remain
        if (enemy->isEnemyType(EnemyGameObject::Tower)) towerCount++;
        
        if(enemy->isShowing())enemy->Render(view_matrix, current_time_);
    }

    //If no towers remain in the world, end the game
    if (towerCount == 0 && !GameOverTimer->hasBeenRun()) {
        cout << "CONGRATULATIONS! YOU DEFEATED ALL TOWERS!!!" << endl;
        GameOverTimer->Start(4);
    }
    
    // Render all WORLD game objects
    for (int i = 0; i < gameObjectManager->numStaticObj(); i++) {
        StaticGameObject* obj = gameObjectManager->GetStaticObject(i);
        if(obj->isShowing())obj->Render(view_matrix, current_time_);
        if (obj->hasParticleHandler() && obj->hasParticleSys()) {
            for (int i = 0; i < obj->numParticleSys(); i++) {
                particleSystems.push_back(obj->GetParticleSys(i));
            }
        }
    }

    // Render all ITEM game objects
    for (int i = 0; i < gameObjectManager->numItems(); i++) {
        CollectibleGameObject* item = gameObjectManager->GetItem(i);
        //cout << "Item at " << item->GetPosition().x << ", " << item->GetPosition().y << endl;
        item->Render(view_matrix, current_time_);
    }

    // Render PLAYER game object
    PlayerGameObject* player = gameObjectManager->Player();

    if (player->hasSubObj()) {//If player has sub objects, render them on top of the player sprite
        for (int i = 0; i < player->numSubObj(); i++) {
            if(player->isShowing())player->GetSubObject(i)->Render(view_matrix, current_time_);
        }
    }
    if (player->hasParticleHandler() && player->hasParticleSys()) {
        for (int i = 0; i < player->numParticleSys(); i++) {
            particleSystems.push_back(player->GetParticleSys(i));
        }
    }
   
    
    //Render the player's projectiles
    std::vector<ProjectileGameObject*> playerBullets = player->GetProjectiles();
    for (ProjectileGameObject* bullet : playerBullets) {
        if (bullet->hasParticleHandler() && bullet->hasParticleSys()) {
            
            for (int i = 0; i < bullet->numParticleSys(); i++) {
                particleSystems.push_back(bullet->GetParticleSys(i));
            }
        }
        //Check if bullet is exploding, if it is inform the objectManager
        //usually this is done in objectmanager->update() but this is the only place we loop through all the projectiles in the game that would work
        //Same goes for enemy projectiles rendered and lister earlier
        if (bullet->isDetonated()) {
            gameObjectManager->damageAOE(bullet->GetPosition(), 3.5f, ROCKET_DMG, GameObject::Enemy);
            bullet->resetDetonation();
        }
        if(bullet->isShowing())bullet->Render(view_matrix, current_time_);
    }
    if(player->isShowing()) player->Render(view_matrix, current_time_);
    
    gameObjectManager->Background()->Render(view_matrix, current_time_);
    

    //RENDER PARTICLE SYSTEMS
    if (!particleSystems.empty()) {
        for (ParticleSystem* particleSys : particleSystems) {
            if(particleSys->isShowing()) particleSys->Render(view_matrix, current_time_);
        }
    }
    particleSystems.clear();

    
}
      
} // namespace game
