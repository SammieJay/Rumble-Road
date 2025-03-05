#include "timer.h"

namespace game {

Timer::Timer(string name): name(name){
	Running = false;
	hasRun = false;
}

Timer::Timer() : name("Timer") {
	Running = false;
	hasRun = false;
}
Timer::~Timer() {}

//Function logs the end time of the timer and sets running bool to true when called
void Timer::Start(float duration_sec) {
	Running = true;
	hasRun = true;
	endTime = glfwGetTime()*1000 + (duration_sec*1000);
	//cout << "Time is: " << glfwGetTime() * 1000 << endl;
	//cout << "ending at: " << endTime << endl;
}

//This function is called to check and see if the timer is done. If checked and timer is past time, then return true and end timer.
bool Timer::isFinished(){
	if (glfwGetTime()*1000 >= endTime && Running) {
		Running = false;
		return true;
	} else if(Running){
		float remaining = endTime - glfwGetTime()*1000;
		if(name == "death") cout << "Time remaining: " << remaining << endl;
		return false;
	}
	return false;
}



}
