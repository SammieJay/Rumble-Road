#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "particle_handler.h"
#include "game_object.h"

namespace game {

//Base Class Constructor
GameObject::GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, objType type)
{
    // Initialize all attributes
    position_ = position;
    scale_ = glm::vec2(1.0,1.0);
    angle_ = 0.0;
    geometry_ = geom;
    shader_ = shader;
    typeID = type;
    clear = false;
    particleHandler = nullptr;
    ghostMode = false;

    detonated = false;
    showing = true;

    time = 0;
}

GameObject::~GameObject() {

}


glm::vec3 GameObject::GetBearing(void) const {
    glm::vec3 dir(cos(angle_), sin(angle_), 0.0);
    return glm::normalize(dir);
}


glm::vec3 GameObject::GetRight(void) const {
    float pi_over_two = glm::pi<float>() / 2.0f;
    glm::vec3 dir(cos(angle_ - pi_over_two), sin(angle_ - pi_over_two), 0.0);
    return glm::normalize(dir);
}


void GameObject::SetRotation(float angle){ 

    // Set rotation angle of the game object
    float two_pi = 2.0f*glm::pi<float>();
    angle = fmod(angle, two_pi);
    if (angle < 0.0){
        angle += two_pi;
    }
    angle_ = angle;
}

//GHOST MODE TOGGLE
void GameObject::toggleGhost() {
    if (ghostMode) {
        ghostMode = false;
    }
    else {
        ghostMode = true;
    }
}

//Particle System Related
bool GameObject::hasParticleSys() { return particleHandler->hasParticleSystems(); }
int GameObject::numParticleSys()const { return particleHandler->numParticleSystems(); }
ParticleSystem* GameObject::GetParticleSys(int indx) { return particleHandler->getParticleSys(indx); }
void GameObject::setParticleHandler(ParticleHandler* handler) { particleHandler = handler; }


void GameObject::Update(double delta_time) {
    time += delta_time;//Update time step for parametric transforms
    if (hasParticleHandler()) particleHandler->Update(delta_time);
}

void GameObject::Render(glm::mat4 view_matrix, double current_time){
    // Set up the shader
    shader_->Enable();

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));

    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;


    

    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);
    

    if (typeID == Background) {
        shader_->SetUniform1f("tiling_factor", 3.0f);//If the object is the background, set the tiling factor to 3 tiles
    } else {
        shader_->SetUniform1f("tiling_factor", 1.0f);//if it's a normal object do not tile the texture
    }
    
    if (ghostMode){
        shader_->SetUniform1f("doGreyscale", 1);
    } else {
        shader_->SetUniform1f("doGreyscale", 0);
    }

    

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());
   
    

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

} // namespace game
