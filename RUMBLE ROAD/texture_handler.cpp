#include "texture_handler.h"

namespace game {
    TextureHandler::TextureHandler(string textureDir, string resourceDir) {
        this->resourceDir = resourceDir;
        mainTex = retreiveTex((resourceDir + textureDir).c_str());
        singleTex = false;
    }

    TextureHandler::TextureHandler(string fullTextureDir) {
        mainTex = retreiveTex((fullTextureDir).c_str());
        singleTex = true;
    }

    GLuint TextureHandler::retreiveTex(const char* dir) {
        //declare GLuint obj
        GLuint w;
        glGenTextures(1, &w);

        // Bind texture buffer
        glBindTexture(GL_TEXTURE_2D, w);

        // Load texture from a file to the buffer
        int width, height;
        unsigned char* image = SOIL_load_image(dir, &width, &height, 0, SOIL_LOAD_RGBA);
        if (!image) {
            std::cout << "ERROR: Cannot load texture " << dir << " FILE NOT FOUND" << std::endl;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);

        // Texture Wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Texture Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        return w;
    }

    void TextureHandler::loadTexture(const char* dir, char* key) {
        if (!singleTex) {
            GLuint tex = retreiveTex((resourceDir + dir).c_str());
            textures[key] = tex;
        }
        else cout << "ERROR: Trying to give multiple textures to a static tex handler" << endl;
        
    }

};//namepsace game