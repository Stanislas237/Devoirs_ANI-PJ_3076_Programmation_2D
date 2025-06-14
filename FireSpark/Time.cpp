#include "Time.h"

float Time::deltaTime = 0.0f;
int Time::targetFrameRate = 60; // 60 FPS

float Time::targetFrameDuration() {
    return 1.0f / static_cast<float>(targetFrameRate);
}

void Time::update() {
    using clock = std::chrono::high_resolution_clock;
    static auto last = clock::now();

    auto now = clock::now();
    std::chrono::duration<float> elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(now - last);
    float frameTime = elapsed.count();

    // si la frame est plus rapide que prévu, on attend
    float waitTime = targetFrameDuration() - frameTime;
    if (waitTime > 0.0f) {
        std::this_thread::sleep_for(std::chrono::duration<float>(waitTime));
        now = clock::now();
        frameTime = std::chrono::duration<float>(now - last).count();
    }

    deltaTime = frameTime;
    last = now;
}
