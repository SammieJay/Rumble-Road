#ifndef TEXTURE_HANDLER_H_
#define TEXTURE_HANDLER_H_

#include "defs.h"

namespace game {

	class TextureHandler {
	public:
		TextureHandler(string texDir, string resourceDir); //Pass in the directory for the game's assets and the specific texture directory desired
		TextureHandler(string defaultTexDir); //This version of the constructor is for texture handlers that will only have one texture and the whole directory is passed in

		//Getters and Setters
		inline GLuint getMainTex(void) { return mainTex; }
		inline GLuint getTex(char* key) { return textures[key]; }
		inline string getResourceDir() { return resourceDir; } //return the games resource dir (this is a shortcut to avoid passing more parameters through constructors)
		
		//Member func
		void loadTexture(const char* fname, char* key);//loads a GLuint containing the image at the given dir to the member texture vector

	private:
		//Hashtable containing all of the textures for this handler that aren't the default texture
		//Keys are strings which should all be listed as a comment wherever a texture handler is loaded
		std::unordered_map<string, GLuint> textures;
		
		//The directory of the game folder
		string resourceDir;

		//The default texture that can be passed to the handler in the constructor
		GLuint mainTex;

		bool singleTex;//denotes whether the texture handler should be able to store more than the default texture

		//Private Funcs
		GLuint retreiveTex(const char* dir);//loads a GLuint with the immage at the specified directory and returns it
		

	};
};//namespace game
#endif