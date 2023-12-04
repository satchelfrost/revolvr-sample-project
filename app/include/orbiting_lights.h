#pragma once

#include <ecs/component/types/ritual.h>
#include <ecs/component/types/spatial.h>

class OrbitingLights : public rvr::Ritual {
    enum NumBands {
        NO_BANDS = 0,
        ONE_BAND,
        TWO_BANDS,
        THREE_BANDS,
        BAND_COUNT
    };

    static const int NUM_LIGHTS = 10;
    rvr::Spatial* lightSpatials_[NUM_LIGHTS]{};
    int bandCount_ = (int)NO_BANDS;

    bool orbiting_ = false;
    float currentThickness = 0.0125f;
    float currentHalfGradient = 0.05f;

    float period_ = 15.0f;
    float currentTime_ = 0.0f;
    float apoapsis_ = 5.0f;  //https://en.wikipedia.org/wiki/Apsis
    float periapsis_ = 1.0f; // https://en.wikipedia.org/wiki/Apsis
    float semiMajorAxis_;
    float semiMinorAxis_;
    float eccentricity_;

    // Mean anomaly (M) and eccentric anomaly (E), and eccentricity (e)
    // see https://en.wikipedia.org/wiki/Kepler%27s_equation for details
    static float KeplerEquation(float M , float E, float e) ;
    static float KeplerDifferentiated(float E, float e) ;
    static float SolveKepler(float M, float e);
    void UpdateOrbit(float delta);
    void UpdateUniform(float delta);
public:
    OrbitingLights(rvr::type::EntityId id);
    virtual void Update(float delta) override;
};
