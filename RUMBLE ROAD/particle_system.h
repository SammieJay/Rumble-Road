#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_
#define GLM_FORCE_RADIANS

#include "game_object.h"
#include "defs.h"

namespace game {

    // Inherits from GameObject
    class ParticleSystem : public GameObject {

        public:
            ParticleSystem(const glm::vec3 &position, Geometry *geom, Shader *shader, TextureHandler* tex, GameObject *parent);

            void Update(double delta_time) override;

            void Render(glm::mat4 view_matrix, double current_time);

            inline bool isShowing() { return showing; }
            inline void show() { showing = true; }
            inline void hide() { showing = false; }
            inline void resetTime() { t = 0; }

        protected:
            GameObject* parent;
            TextureHandler* texHandler;
            bool showing;

            double t;

    }; // class ParticleSystem

} // namespace game

#endif // PARTICLE_SYSTEM_H_
