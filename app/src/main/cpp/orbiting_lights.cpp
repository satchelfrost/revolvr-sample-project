#include <orbiting_lights.h>
#include <global_context.h>
#include <helper_macros.h>
#include "io/io.h"

#define FIRST_LIGHT_ID 7
#define PI (3.14159f)
#define TWO_PI (2.0f * PI)
#define MIN_THICKNESS 0.00125f
#define MAX_THICKNESS 0.0250f
#define OUTLINE_SPEED 0.007f

OrbitingLights::OrbitingLights(rvr::type::EntityId id) : Ritual(id) {
    semiMajorAxis_ = (apoapsis_ + periapsis_) / 2.0f;
    semiMinorAxis_ = sqrt(apoapsis_ * periapsis_);
    float linearEccentricity = semiMajorAxis_ - periapsis_;
    eccentricity_ = linearEccentricity / semiMajorAxis_;

    for (int i = 0; i < NUM_LIGHTS; i++) {
        if(lightSpatials_[i])
            rvr::PrintWarning("Light spatial was initialized with something");

        int eid = FIRST_LIGHT_ID + i;
        auto spatial = GetComponent<rvr::Spatial>(eid);
        if (spatial)
            lightSpatials_[i] = spatial;
        else
            rvr::PrintError("Null pointer found on eid " + std::to_string(eid));
    }

    // Do one update to initialize the lights
    UpdateOrbit(0);
}

void OrbitingLights::Update(float delta) {
    if (ButtonPressed(rvr::ActionType::A))
        orbiting_ = !orbiting_;

    UpdateUniform(delta);

    if (orbiting_)
        UpdateOrbit(delta);
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

void OrbitingLights::UpdateOrbit(float delta) {
    currentTime_ += delta;
    float meanAnomaly = currentTime_ * TWO_PI / period_;

    for (int i = 0; i < NUM_LIGHTS; i++) {
        float angleOffset = (TWO_PI / (float)NUM_LIGHTS) * (float)i;
        float eccentricAnomaly = SolveKepler(meanAnomaly + angleOffset, eccentricity_);
        float x =  semiMajorAxis_ * ((float)cos(eccentricAnomaly) - eccentricity_);
        float z = -semiMinorAxis_ * (float)sin(eccentricAnomaly);
        lightSpatials_[i]->SetLocalPosition({x, 0.0f, z});
    }
}

void OrbitingLights::UpdateUniform(float delta) {
    auto uniform = GetUniform();

    // Update line thickness in cel shading
    auto leftJoy = GetJoystickXY(rvr::Hand::Left);
    float newThickness = currentThickness + leftJoy.y * delta * OUTLINE_SPEED;
    if (newThickness > MAX_THICKNESS)
        currentThickness = MAX_THICKNESS;
    else if (newThickness < MIN_THICKNESS)
        currentThickness = MIN_THICKNESS;
    else
        currentThickness = newThickness;
    uniform.outlineWidth = currentThickness;

    // Update the band configuration in cel shading
    switch(currConfig_) {
    case ONE_BAND:
        uniform.numBands = 1;
        uniform.bands[0] = 0.5;
        break;
    case TWO_BANDS:   break;
    case THREE_BANDS: break;
    case FOUR_BANDS:  break;
    case FIVE_BANDS:  break;
    case NO_BANDS:    break;
    default:
        rvr::PrintError("Improper band configuration");
    }
//    switch(currConfig_) {
//    case ONE_BAND:    uniform.bandState = currConfig_; break;
//    case TWO_BANDS:   uniform.bandState = currConfig_; break;
//    case THREE_BANDS: uniform.bandState = currConfig_; break;
//    case FOUR_BANDS:  uniform.bandState = currConfig_; break;
//    case FIVE_BANDS:  uniform.bandState = currConfig_; break;
//    case NO_BANDS:    uniform.bandState = currConfig_; break;
//    default:
//        rvr::PrintError("Improper band configuration");
//    }

    SetUniform(uniform);
}