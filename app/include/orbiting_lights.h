#pragma once

#include <ecs/component/types/ritual.h>
#include <ecs/component/types/spatial.h>

class OrbitingLights : public rvr::Ritual {
    std::vector<rvr::Spatial*> lightSpatials_;
    float period_ = 5.0f;
    float currentTime_ = 0.0f;
    float apoapsis_ = 2.2f;  //https://en.wikipedia.org/wiki/Apsis
    float periapsis_ = 0.5f; // https://en.wikipedia.org/wiki/Apsis

    // Mean anomaly (M) and eccentric anomaly (E) (https://en.wikipedia.org/wiki/Kepler%27s_equation)
    static float KeplerEquation(float M , float E, float e) ;
    static float KeplerDifferentiated(float E, float e) ;
    static float SolveKepler(float M, float e);
public:
    OrbitingLights(rvr::type::EntityId id);
    virtual void Update(float delta) override;
};
