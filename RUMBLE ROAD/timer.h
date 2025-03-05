#pragma once
#ifndef TIMER_H_
#define TIMER_H_

#include "defs.h"

//THIS CLASS AND IT'S MEMBER FUNCTIONS WERE MADE WITH THE ASSISTANCE OF AI GENERATED CODE

//AI tools made the code used to generate a new thread, then sleep it for n seconds
//The rest of the implementation (refresh() system) was written by me
namespace game {

    // A class implementing a simple timer
    class Timer {

    public:
        // Constructor and destructor
        Timer();
        Timer(string name);
        ~Timer();

        // Start the timer now: end time given in seconds
        void Start(float duration_sec);

        // Check if timer has finished
        bool isFinished(void);
        inline bool isRunning(void) { return Running; }
        inline bool hasBeenRun(void) { return hasRun; }

        inline string GetName(void) { return name; }

    private:
        string name;
        float startTime;
        float endTime;
        bool Running;
        bool hasRun;
    }; // class Timer

} // namespace game

#endif // TIMER_H_
