#include <gl/gl.h>
#include <GL/glu.h>
#include <windows.h>
#include <gl/gl.h>
#include "Vector3.h"
#include"Matrix4x4.h"
#include<vector>
#include"Triangle.h"
#include"PointLight.h"
#include"Face.h"
#include <fstream>
#include<string>
#include <sstream>
#include<math.h>
#include<iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <chrono>
#include <memory>
#include <cmath>
#include <thread>
#include <memory>
#include"Model.h"
#include"MonobEhaviourLike.h"
#include"GameObject.h"
class Time
{
private:
    // Using high_resolution_clock as our game timer
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    float deltaTime; // Time between current frame and last frame

public:
    // Constructor initializes the timing variables
    Time() : deltaTime(0.0f)
    {
        lastFrameTime = std::chrono::high_resolution_clock::now();
    }

    // Update the timer every frame
    void Update()
    {
        // Get the current time
        auto currentFrameTime = std::chrono::high_resolution_clock::now();

        // Calculate the time it took to render the last frame
        deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(
            currentFrameTime - lastFrameTime).count();

        // Update lastFrameTime to the current time for the next Update call
        lastFrameTime = currentFrameTime;
    }

    // Get the time it took to render the last frame
    float DeltaTime() const
    {
        return deltaTime;
    }
   
    // Since we're not dynamically allocating memory, we don't need a destructor in this case
};  