#pragma once

#include <ecs/component/types/ritual.h>
#include <ecs/component/types/spatial.h>

class OrbitingLights : public rvr::Ritual {
    std::vector<rvr::Spatial*> lightSpatials_;
    float period_ = 5.0f;
    float currentTime_ = 0.0f;
    float a_ = 2.0f; // semi-major axis Kepler's First Law
    float e_ = 0.7f; // eccentricity Kepler's First Law

public:
    OrbitingLights(rvr::type::EntityId id);
    virtual void Update(float delta) override;
};
