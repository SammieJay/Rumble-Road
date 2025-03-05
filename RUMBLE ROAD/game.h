#ifndef GAME_H_
#define GAME_H_

#define GLEW_STATIC
#define GLM_FORCE_RADIANS

#include "defs.h" //Includes all required libraries

#include "object_manager.h"
#include "texture_handler.h"
#include "particle_system.h"
#include "random_gen.h"
#include "sprite.h"
#include "shader.h"
#include "particles.h"

namespace game {


    // A class for holding the main game objects
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();

            // Call Init() before calling any other method
            // Initialize graphics libraries and main window
            void Init(void);

            // Set up the game (scene, game objects, etc.)
            void Setup(void);

            // Run the game (keep the game active)
            void MainLoop(void); 

        private:
            // Main window: pointer to the GLFW window structure
            GLFWwindow *window_;

            // Sprite geometry
            Geometry *sprite_;

            // Particle geometry
            Geometry *particles_;

            // Shader for rendering sprites in the scene
            Shader sprite_shader_;

            // Shader for rendering particles
            Shader particle_shader_;

            // References to textures
            // This needs to be a pointer
            GLuint *tex_;

            // Object manager to contain & manage all game objects
            ObjectManager* gameObjectManager;

            RandomGenerator randomGen;

            // Keep track of time
            double current_time_;

            // Callback for when the window is resized
            static void ResizeCallback(GLFWwindow* window, int width, int height);

            // Update all the game objects
            void Update(double delta_time);
 
            // Render the game world
            void Render(void);

            Timer* GameOverTimer;
            Timer* enemySpawnTimer;
            Timer* itemSpawnTimer;


    }; // class Game

} // namespace game

#endif // GAME_H_
