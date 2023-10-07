#include <orbiting_lights.h>
#include <global_context.h>
#include <helper_macros.h>

#define LIGHT_1_ID 7
#define LIGHT_2_ID 8
#define LIGHT_3_ID 9
#define PI (3.14159f)
#define TWO_PI (2.0f * PI)

OrbitingLights::OrbitingLights(rvr::type::EntityId id) : Ritual(id) {
    lightSpatials_.push_back(GetComponent<rvr::Spatial>(LIGHT_1_ID));
    lightSpatials_.push_back(GetComponent<rvr::Spatial>(LIGHT_2_ID));
    lightSpatials_.push_back(GetComponent<rvr::Spatial>(LIGHT_3_ID));
}

void OrbitingLights::Update(float delta) {
    currentTime_ += delta;

    float semiMajorAxis = (apoapsis_ + periapsis_) / 2.0f;
    float semiMinorAxis = sqrt(apoapsis_ * periapsis_);
    float linearEccentricity = semiMajorAxis - periapsis_;
    float eccentricity = linearEccentricity / semiMajorAxis;
    float meanAnomaly = currentTime_ * TWO_PI / period_;

    for (int i = 0; i < lightSpatials_.size(); i++) {
        float angleOffset = (TWO_PI / (float)lightSpatials_.size()) * (float)i;
        float eccentricAnomaly = SolveKepler(meanAnomaly + angleOffset, eccentricity);
        float x =  semiMajorAxis * ((float)cos(eccentricAnomaly) - eccentricity);
        float z = -semiMinorAxis * (float)sin(eccentricAnomaly);
        lightSpatials_[i]->SetLocalPosition({x, 0.0f, z});
    }
}

float OrbitingLights::KeplerEquation(float M, float E, float e) {
    return E - (e * sin(E)) - M;
}

float OrbitingLights::KeplerDifferentiated(float E, float e) {
    return 1 - (e * cos(E));
}

float OrbitingLights::SolveKepler(float M, float e) {
    float accuracy = 0.000001f;
    int iterations = 100;

    // Pick a suitable starting value for Newton's Method
    float E = (e > 0.8f) ? PI : M;
    for (int i = 0; i < iterations; i++) {
        float ENext = E - (KeplerEquation(M, E, e) / KeplerDifferentiated(E, e));
        float diff = abs(E - ENext);
        E = ENext;
        if (diff < accuracy)
            break;
    }

    return E;
}
