#ifndef RANDOM_GEN_H_
#define RANDOM_GEN_H_

#include "defs.h"
#include <random>

namespace game {

	//A general purpose class with functions to generate various types of random numbers
	//THIS CLASS WAS MADE WITH THE ASSISTANCE OF AI
	class RandomGenerator {
	public:
		RandomGenerator() : rand(std::random_device{}()) {};

		glm::vec3 randomPoint() { return randomPoint(glm::vec2(-45, -45), glm::vec2(45, 45)); } //If no coordinates provided, return this as default

		glm::vec3 randomPoint(glm::vec2 minPoint, glm::vec2 maxPoint) {
			std::uniform_real_distribution<float> xDist(minPoint.x, maxPoint.x);
			std::uniform_real_distribution<float> yDist(minPoint.y, maxPoint.y);

			return glm::vec3(xDist(rand), yDist(rand), 0);
		}

		int randomInt(int min, int max) {
			std::uniform_int_distribution<int> num(min, max);
			return num(rand);
		}

	private:
		std::mt19937 rand;
	};
}
#endif
