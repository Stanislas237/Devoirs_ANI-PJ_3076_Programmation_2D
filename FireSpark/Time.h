#pragma once
#include <chrono>
#include <thread>

struct Time {
    static float deltaTime;
    static int targetFrameRate;
    static float targetFrameDuration();

    static void update();
};
