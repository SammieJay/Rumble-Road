#ifndef PARTICLES_H_
#define PARTICLES_H_

#include "geometry.h"

#define NUM_PARTICLES 4000

namespace game {

    // A set of particles that can be rendered
    class Particles : public Geometry {
        public:
            Particles(void);
            Particles(float spread, float phase, float speed);

            // Create the geometry (called once)
            void CreateGeometry(void);

            // Use the geometry
            void SetGeometry(GLuint shader_program);
        private:
            float spread_;
            float phaseConst_;
            float speedConst_;

    }; // class Particles
} // namespace game

#endif // PARTICLES_H_
